#include "config.h"

#include "lz4.h"
#include "lz4hc.h"

#define LZ4AR_VERSION 1
#define LZ4AR_MARK 'RAZL'
#define LZ4AR_INITIAL_ENTRIES 256

#include <stdlib.h>

#include "LZ4Ar.h"
#include "xxhash.h"

namespace engine {
namespace storage {

struct LZ4ArHeader {
  uint32_t mark;
  uint32_t version;
  uint32_t datapos;
  uint32_t tablepos;
  uint32_t entrycount;
  uint32_t reserved[3];
};

struct LZ4ArTableEntry {
  uint32_t nhash1;
  uint32_t nhash2;
  uint32_t position;
  uint32_t complen;
  uint32_t origlen;
};

LZ4Ar::Ptr LZ4Ar::Create(const std::string &path) {
  return Ptr(new LZ4Ar(path));
}

LZ4Ar::LZ4Ar(const std::string &path) {
  fullpath = path;
  header = nullptr; table = nullptr;
  fs.open(path, fs.binary | fs.in | fs.out);
  tableDirty = false;
  success = fs.good();
  if (!success) {
    fs.open(path, fs.binary | fs.in | fs.out | fs.trunc);
    success = fs.good();
    if (!success)
      return;
  }
  fs.seekg(0, fs.end);
  uint64_t length = fs.tellg();

  header = new LZ4ArHeader;
  bool isNewFile = false;
  if (length < sizeof(LZ4ArHeader)) isNewFile = true;
  else {
    fs.seekg(0, fs.beg);
    fs.read(reinterpret_cast<char*>(header), sizeof(LZ4ArHeader));
    size_t sz = sizeof(LZ4ArHeader)+header->entrycount * sizeof(LZ4ArTableEntry);
    if (header->mark != LZ4AR_MARK || header->datapos < sizeof(LZ4ArHeader) || header->tablepos < header->datapos)
      isNewFile = true;
  }
  if(isNewFile) {
    header->mark = LZ4AR_MARK;
    header->version = LZ4AR_VERSION;
    header->datapos = header->tablepos = sizeof(LZ4ArHeader);
    header->entrycount = LZ4AR_INITIAL_ENTRIES;
    size_t sz = header->entrycount * sizeof(LZ4ArTableEntry);
    memset(header->reserved, 0, sizeof(header->reserved));
    fs.seekg(0, fs.beg);
    fs.write(reinterpret_cast<const char*>(header), sizeof(LZ4ArHeader));
    table = new LZ4ArTableEntry[header->entrycount];
    memset(table, 0, sz);
    fs.seekg(header->tablepos);
    fs.write(reinterpret_cast<const char*>(table), sz);
  } else {
    table = new LZ4ArTableEntry[header->entrycount];
    fs.seekg(header->tablepos);
    fs.read(reinterpret_cast<char*>(table), header->entrycount * sizeof(LZ4ArTableEntry));
  }
}

LZ4Ar::~LZ4Ar() {
  if (tableDirty)
    RewriteTable();
  delete header;
  delete[] table;
}

uint32_t LZ4Ar::FindEntry(uint32_t hash1, uint32_t hash2, uint32_t hash3, bool match) {
  uint32_t zidx = hash3 % header->entrycount;
  LZ4ArTableEntry &te = table[zidx];
  if (te.position == 0 || (te.nhash1 == hash1 && te.nhash2 == hash2))
    return zidx;
  uint32_t tidx = (zidx + 1) % header->entrycount;
  do {
    tidx = (tidx + 1) % header->entrycount;
    te = table[tidx];
    if (te.position == 0)
      break;
    if (tidx == zidx) return 0xFFFFFFFF;
    if (te.nhash1 == hash1 && te.nhash2 == hash2) {
      break;
    } else {
      if (match) return 0xFFFFFFFF;
    }
  } while (1);
  return tidx;
}

bool LZ4Ar::AddFileFromMemory(const std::string &path, const char *src, int length) {
  uint32_t hash1 = XXH32(&path[0], path.length(), 0);
  uint32_t hash2 = XXH32(&path[0], path.length(), 1);
  uint32_t hash3 = XXH32(&path[0], path.length(), 16);
  uint32_t zidx = FindEntry(hash1, hash2, hash3, false);
  if (zidx >= header->entrycount) return false;
  LZ4ArTableEntry &te = table[zidx];
  te.nhash1 = hash1;
  te.nhash2 = hash2;
  te.origlen = length;
  uint32_t maxl = LZ4_compressBound(length);
  char *dest = new char[maxl];
  uint32_t r = LZ4_compressHC(src, dest, length);
  delete[] src;
  if (r == 0) {
    delete[] dest;
    return false;
  }
  if (te.complen < r) {
    te.complen = r;
    te.position = header->tablepos;
    header->tablepos += r;
  }
  else {
    te.complen = r;
  }
  fs.seekg(te.position, fs.beg);
  fs.write(dest, r);
  delete[] dest;
  tableDirty = true;
  return true;
}

bool LZ4Ar::AddFile(const std::string &path) {
  std::ifstream ifs;
  ifs.open(path, fs.binary | fs.in);
  ifs.seekg(0, fs.end);
  uint32_t length = static_cast<uint32_t>(ifs.tellg());
  char *src = new char[length];
  ifs.seekg(0, fs.beg);
  ifs.read(src, length);
  ifs.close();

  bool res = AddFileFromMemory(path, src, length);
  delete[] src;

  return res;
}

bool LZ4Ar::ReadFile(const std::string &path, std::string &data) {
  uint32_t hash1 = XXH32(&path[0], path.length(), 0);
  uint32_t hash2 = XXH32(&path[0], path.length(), 1);
  uint32_t hash3 = XXH32(&path[0], path.length(), 16);
  uint32_t zidx = FindEntry(hash1, hash2, hash3, true);
  if (zidx >= header->entrycount) return false;
  LZ4ArTableEntry &te = table[zidx];
  char *idata = new char[te.complen];
  fs.seekg(te.position, fs.beg);
  fs.read(idata, te.complen);
  data.resize(te.origlen);
  if (LZ4_decompress_safe(idata, &data[0], te.complen, te.origlen) < 0) {
    delete[] idata;
    return false;
  }
  delete[] idata;
  return true;
}

bool LZ4Ar::ExtractFile(const std::string &path, const std::string &outPath ) {
  std::string data;
  if (!ReadFile(path, data)) return false;
  std::ofstream ofs;
  ofs.open(outPath, ofs.binary | ofs.out | ofs.trunc);
  if (!ofs.good()) return false;
  ofs.write(&data[0], data.length());
  ofs.close();
  return true;
}

bool LZ4Ar::DeleteFile(const std::string &path) {
  uint32_t hash1 = XXH32(&path[0], path.length(), 0);
  uint32_t hash2 = XXH32(&path[0], path.length(), 1);
  uint32_t hash3 = XXH32(&path[0], path.length(), 16);
  uint32_t zidx = FindEntry(hash1, hash2, hash3, true);
  if (zidx >= header->entrycount) return false;
  LZ4ArTableEntry &te = table[zidx];
  te.nhash1 = 0;
  te.nhash2 = 0;
  te.position = 0;
  te.complen = 0;
  te.origlen = 0;
  tableDirty = true;
  return true;
}

void LZ4Ar::WriteHeader() {
  fs.seekg(0, fs.beg);
  fs.write(reinterpret_cast<const char*>(header), sizeof(LZ4ArHeader));
}

void LZ4Ar::RewriteTable() {
  fs.seekg(header->tablepos, fs.beg);
  fs.write(reinterpret_cast<const char*>(table), header->entrycount * sizeof(LZ4ArTableEntry));
  WriteHeader();
}

void LZ4Ar::Compact() {

}

}
}
