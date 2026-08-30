# cpp-server-study

C++ 게임서버를 **밑바닥부터 직접 만들면서** 이해하는 학습 저장소.

완성된 코드를 읽고 이해하는 게 아니라, 빈 파일에서 시작해 패킷 파서 → 블로킹 소켓 → 멀티스레드 → JobQueue → IOCP 순서로 직접 만들고, 각 단계에서 버그를 **일부러 터뜨려 관찰**하는 방식으로 진행한다.

## 학습 원칙

1. **예측 먼저, 실행은 나중.** 코드를 돌리기 전에 무슨 일이 생길지 먼저 적는다. 예측이 틀린 지점이 수확물이다.
2. **답안지는 내가 짠 뒤에만.** 완성된 서버 코드([IocpGameRoomServer](https://github.com/hidenori100/IocpGameRoomServer))는 각 단계를 스스로 구현한 뒤 대조용으로만 연다.
3. **말로 설명 못 하면 모르는 것.** 각 단계의 졸업 조건은 코드 안 보고 개념을 설명하는 것이다.
4. **배운 것은 글로 남긴다.** 단계마다 velog에 학습 기록을 쓴다.

## 진행 상황

| 단계 | 주제 | 상태 | 기록 |
|------|------|------|------|
| STEP 0-1 | Rule of Five — 이중 해제, ASan, 이동 시맨틱 | ✅ 완료 | [velog](https://velog.io/@hidenori100/C-Rule-of-Five-%EC%9D%B4%EC%A4%91-%ED%95%B4%EC%A0%9C-ASan-%EC%9D%B4%EB%8F%99-%EC%8B%9C%EB%A7%A8%ED%8B%B1) |
| STEP 0-2 | Rule of Zero — unique_ptr + 커스텀 deleter, EBO | ✅ 완료 | velog 예정 |
| STEP 0-3 | shared_ptr 제어 블록, shared_from_this | | |
| STEP 0-4 | 미니 ObjectPool | | |
| STEP 0-5 | 비동기 keep-alive 이디엄 | | |
| STEP 1 | 패킷 파서 — TCP 스트림에서 패킷 추출 | | |
| STEP 2 | 블로킹 에코 서버 | | |
| STEP 3 | 멀티클라이언트와 데이터 레이스 | | |
| STEP 4 | JobQueue — 락 최소화 직렬화 | | |
| STEP 5 | IOCP 전환 | | |
| STEP 6 | 졸업시험 — 실제 서버 코드의 버그 분석·수정 | | |

## 저장소 구조

```
├─ step0-ownership/      # 소유권: Rule of Five/Zero, shared_ptr, ObjectPool
├─ step1-packet-parser/  # [uint16 size][uint16 id][payload] 파서
├─ step2-echo-server/    # 블로킹 소켓 에코 서버
├─ step3-multiclient/    # thread-per-client, 데이터 레이스 실험
├─ step4-jobqueue/       # 직렬화 프리미티브
├─ step5-iocp/           # IOCP 에코 서버
└─ CLAUDE.md             # 학습 규칙과 진도 (AI 튜터 세션용 컨텍스트)
```

## 환경

- Visual Studio 2026 / x64 / C++17
- 메모리 버그 관찰: AddressSanitizer (`/fsanitize=address`)
- 테스트 클라이언트: Python 3 (`socket`, `struct`)
