#pragma once
#include <cstdio>
#include <memory>

// STEP 0-3: 이중 제어 블록 사고 재현
//
// shared_ptr은 포인터 2개다: T를 가리키는 포인터 + 제어 블록 포인터.
// 제어 블록에는 참조 카운트, weak 카운트, deleter가 산다.
// 복사하면 카운트 +1, 소멸하면 -1, 0이 되는 순간 deleter 실행.
//
// 이번 실험의 질문: 같은 raw 포인터로 shared_ptr을 두 번 만들면?

struct Widget : std::enable_shared_from_this<Widget>
{
    ~Widget() { std::puts("~Widget"); }
};

// ─── 실험 A ────────────────────────────────────────────────
void ExperimentA()
{
    auto* raw = new Widget;
    std::shared_ptr<Widget> a(raw);
    std::shared_ptr<Widget> b(raw);   // 같은 raw로 두 번째 shared_ptr

    printf("a.use_count = %ld\n", a.use_count());
    printf("b.use_count = %ld\n", b.use_count());
}
//
// Q1. ~Widget은 몇 번 찍히는가? 그리고 무슨 일이 생기는가?
//     예측: 아 ~Widget은 2번 찍힘. 각자 a,b가 각각 use_count 가 0이 되어서... 무섭다...
// 
// Q2. a.use_count()와 b.use_count()는 각각 몇으로 찍힐까?
//     둘이 같은 객체를 가리키는데 카운트는 왜 그렇게 나오는가?
//     예측: a=1 , b=1 얘는 지금 쌩포인터를 각각 shared_ptr로 만들어서 얘를 가지고 있는 애가 몇명있는지 모른다.
//
// Q3. 1편에서는 ASan이 이중 fclose를 못 잡았다 (CRT 내부라서).
//     이번 이중 해제는 ASan이 잡아줄까?
//     예측: 이번에는 실제로 코드로 잘 막혀있을 거 같아서 잡힐듯
//
// ─── 실험 B ────────────────────────────────────────────────
// 위를 고친다. Widget에 enable_shared_from_this를 상속시키고,
// 두 번째 shared_ptr을 raw가 아니라 a로부터 얻는다.
//
// struct Widget : std::enable_shared_from_this<Widget> { ... };
//
void ExperimentB()
{
    //auto a = std::make_shared<Widget>();
    std::shared_ptr<Widget> a(new Widget);
    std::shared_ptr<Widget> b = a->shared_from_this();
    printf("a.use_count = %ld\n", a.use_count());
    printf("b.use_count = %ld\n", b.use_count());
    // use_count를 다시 찍어본다
}
//
// Q4. 이번엔 use_count가 몇으로 찍히고 ~Widget은 몇 번 찍힐까?
//     실험 A와 뭐가 달라졌길래 결과가 달라지는가?
//     예측: 이렇게하면 각각 3으로 찍힐거 같고 소멸자는 한번 호출될듯.
// 
//
// Q5. (선택) 실험 B에서 make_shared 대신
//     std::shared_ptr<Widget> a(new Widget); 로 만들면 결과가 달라질까?
//     예측:왠지 달라지지 않을거 같아.. 왜냐하면 이게 생성자로 잘 만들어져잇어서 이것도 해결해줄거 같아
//
// ─── 졸업 조건 ──────────────────────────────────────────────
// "shared_from_this는 왜 필요한가?"를
// **제어 블록**이라는 단어를 써서 한 문장으로 답할 수 있으면 통과.
