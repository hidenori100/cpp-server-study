#include "FileHandle.h"

int main()
{

    FileHandle a("a.txt");
    FileHandle b = std::move(a);   // 이동 생성: fclose 0회 시점
    b = std::move(b);              // 자기 대입: 아무 일 없어야 함
    FileHandle c("c.txt");
    c = std::move(b);              // c.txt 닫힘(1회), c는 a.txt 소유

    return 0;
}