#include "FileHandle.h"
#include "FileHandle2.h"

int main()
{

    if (1 == 2)
    {
        FileHandle a("a.txt");
        FileHandle b = std::move(a);   // 이동 생성: fclose 0회 시점
        b = std::move(b);              // 자기 대입: 아무 일 없어야 함
        FileHandle c("c.txt");
        c = std::move(b);              // c.txt 닫힘(1회), c는 a.txt 소유
    }
    if (1 == 1)
    {
        FileHandle2 a("a.txt");

        //FileHandle2 b = a; Q1 => 자동 delete 되어 실행 할 수없음.
        FileHandle2 c = std::move(a);
        size_t fileHandle2Size = sizeof(FileHandle2);
        size_t filePointerSize = sizeof(std::FILE*);
        size_t uniqePtrSize = sizeof(std::unique_ptr<std::FILE, void(*)(std::FILE*)>);

        printf("size of FileHandle2 : %zu\n", fileHandle2Size);
        printf("size of filePointerSize : %zu\n", filePointerSize);
        printf("size of uniqePtrSize : %zu\n", uniqePtrSize);


    }


    return 0;
}