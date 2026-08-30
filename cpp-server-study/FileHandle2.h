#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <cstdio>
#include <memory>

// STEP 0-2: Rule of Zero — unique_ptr + 커스텀 deleter
//
// 연습 1의 FileHandle과 "완전히 같은 일"을 하는 클래스를,
// 이번에는 특수 함수 5개를 하나도 쓰지 않고 만든다.

// TODO 1: FileCloser 구현 — fp가 nullptr 아니면 fclose 하는 함수 객체
//         (관찰용 printf를 넣어도 좋다)
struct FileCloser
{
    void operator()(std::FILE* fp) const
    {
        // 여기를 채운다
        printf("deleter called, fp=%p\n", (void*)fp);
        if (fp)
        {
            printf("real deleter called");
            std::fclose(fp);
        }
    }
};

class FileHandle2
{
public:
    explicit FileHandle2(const char* path) : _fp(std::fopen(path, "w")) {}

    // 특수 함수 5개: 아무것도 쓰지 않는다. 이게 Rule of Zero다.

private:
    std::unique_ptr<std::FILE, FileCloser> _fp;
};

// ─── 예측 먼저 (코드 돌리기 전에 주석으로 답 쓰기) ───────────────
//
// Q1. main에서 FileHandle2 b = a; (복사)를 하면 어떻게 되는가?
//     아무 특수 함수도 안 썼는데 왜 그런 결과가 나오는가?
//     예측: 복사가 컴파일 단계에서 되지않는다. cpp 컴파일러는 자동으로 생성자, 이동생성자, 복사생성자, 이동대입연산자, 복사대입연산자를 생성하려고 한다.
//           그리고 자동으로 생성될 때 각 멤버의 연산자를 자동으로 만드는데 여기서 uniqe_ptr의 복사와 관련된 연산이 delete 되어있을 것.
//
// Q2. FileHandle2 b = std::move(a); (이동)는 되는가?
//     된다면, 이동 후 a의 _fp는 어떤 상태인가? 소멸 시 fclose는 총 몇 번 찍히는가?
//     예측: 이동이 되고 _fp 는 자동으로 b에게 소유권이 넘어가있다. b의 소멸이 되고 a가 마지막에 소멸된다. 따라서 총 2번이다.
//
// Q3. sizeof(FileHandle2)는 sizeof(std::FILE*)와 같을까 다를까?
//     FileCloser 대신 deleter로 람다나 함수 포인터
//     (std::unique_ptr<std::FILE, void(*)(std::FILE*)>)를 쓰면 크기가 달라질까?
//     예측: 다르다. sizeof(std::FILE*) 는 8 바이트, 그리고 uniqe_ptr은 포인터 + deleter 를 들고 있어서 deleter가 얼마일지는 내가 알 수 없는데...
//          void(*)(std::FILE) 서로 다른 함수를 가리킬 수 있기 때문에 이 값은 인스턴스마다 저장되어있어야한다. 
//
// ─── 졸업 조건 ──────────────────────────────────────────────
// "왜 아무것도 안 썼는데 복사가 컴파일 에러인가?"에
// unique_ptr의 특수 함수가 어떻게 전파되는지로 답할 수 있으면 통과.
