#ifndef _LZ4AR_H_
#define _LZ4AR_H_

namespace engine {
namespace storage {

struct LZ4ArHeader;
struct LZ4ArTableEntry;

class LZ4Ar {
public:
    typedef std::shared_ptr<LZ4Ar> Ptr; //!< Shared pointer.
    typedef std::shared_ptr<const LZ4Ar> PtrConst; //!< Shared pointer.

    static Ptr Create(const std::string &path);

    virtual ~LZ4Ar();

    bool AddFileFromMemory(const std::string &path, const char *src, int length);
    bool AddFile(const std::string &path);
    bool ReadFile(const std::string &path, std::string &data);
    bool ExtractFile(const std::string &path, const std::string &data);
    bool DeleteFile(const std::string &path);
    bool WasSuccess();

protected:
    LZ4Ar(const std::string &path);

    void WriteHeader();
    void RewriteTable();
    void Compact();

    uint32_t FindEntry(uint32_t, uint32_t, uint32_t, bool);

protected:
    std::string fullpath;
    std::fstream fs;
    bool success;
    bool tableDirty;
    LZ4ArHeader *header;
    LZ4ArTableEntry *table;
};


inline bool LZ4Ar::WasSuccess() {
    return success;
}

}
}

#endif // _LZ4AR_H_
