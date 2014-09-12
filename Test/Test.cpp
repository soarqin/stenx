#include "../Engine/config.h"
#include "../Engine/storage/LZ4Ar.h"

using namespace engine::storage;

int main(int argc, char* argv[]) {
    auto ar = LZ4Ar::Create("a.ar");
    //  ar->AddFile(argv[1], false);
    //  ar->AddFile(argv[2]);
    ar->ExtractFile(argv[1], "ttt");
    return 0;
}
