#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <string>
#include <utility>

class FileHandle
{
public:
    explicit FileHandle(const char* path) : _fp(std::fopen(path, "w")), _path(path) {}
    ~FileHandle()
    {
        std::printf("fclose(%s, %p)\n", _path.c_str(), (void*)_fp);
        if (_fp)
        {
            std::fclose(_fp);
        }
    }
    FileHandle(const FileHandle& fileHandle) = delete;
    FileHandle& operator=(const FileHandle& fileHandle) = delete;

    FileHandle(FileHandle&& other) noexcept : _fp(std::exchange(other._fp, nullptr)), _path(std::move(other._path)) {}
    FileHandle& operator=(FileHandle&& other) noexcept
    {
        if (this != &other)
        {
            if (_fp)
                std::fclose(_fp);
            _fp = std::exchange(other._fp, nullptr);
            _path = std::move(other._path);
        }
        return *this;
    }
    // TODO 1: 소멸자 — _fp가 nullptr 아니면 fclose
    // TODO 2: 이대로 복사하면 무슨 일이 생기는지 주석으로 먼저 예측을 쓰고,
    //         main에서 복사해서 실제로 이중 fclose를 관찰 (크래시 또는 ASan)
    // TODO 3: 복사 생성자/대입을 = delete
    // TODO 4: 이동 생성자/대입을 구현 (원본의 _fp를 nullptr로)
private:
    std::FILE* _fp;
    std::string _path;
};