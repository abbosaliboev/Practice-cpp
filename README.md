
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>
#include <errno.h>

#define TimeLimit 3 // 에이전트의 시간 제한을 3초로 설정

pid_t Child = -1; // 유효하지 않은 PID로 설정

void SigintHandler(int sig) { // SIGINT 시그널 처리 함수
    if (Child > 0) {
        kill(Child, SIGKILL); // 자식 프로세스를 강제 종료
    }
    exit(1);
}

void SigalrmHandler(int sig) {
    if (Child > 0) {
        kill(Child, SIGKILL); // 자식 프로세스 강제 종료 (시간 초과)
    }
}

void SigtermHandler(int sig) {
    if (Child > 0) {
        kill(Child, SIGKILL);
    }
    exit(0);
}

void SigchldHandler(int sig) {
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

void PlayerBoard(int player, int board[6][7], int toPIPE) { // 에이전트에게 입력을 생성하는 함수
    char buf[16];
    snprintf(buf, sizeof(buf), "%d\n", player);
    write(toPIPE, buf, strlen(buf)); // 현재 플레이어 정보를 자식에게 전송
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 7; ++j) {
            snprintf(buf, sizeof(buf), "%d ", board[i][j]); // 보드의 현재 칸을 문자열로 변환
            write(toPIPE, buf, strlen(buf)); // 자식에게 전송
        }
        snprintf(buf, sizeof(buf), "\n"); // 행 끝에 개행 문자 전송
        write(toPIPE, buf, strlen(buf)); // 자식에게 전송
    }
}

char RunAgent(char *AgentAddress, int player, int board[6][7]) { // 에이전트를 실행하고 문자 입력 받는 함수
    int to[2], from[2];
    pipe(to); // 부모 -> 자식 파이프
    pipe(from); // 자식 -> 부모 파이프

    pid_t pid = fork(); // 부모와 동일한 프로세스 복제
    if (pid == 0) { // 자식 프로세스
        dup2(to[0], STDIN_FILENO); // 부모가 쓴 데이터를 자식이 읽을 수 있는 읽기의 끝부분
        dup2(from[1], STDOUT_FILENO); // 자식이 쓴 데이터를 부모가 읽을 수 있도록 하는 쓰기의 끝부분

        close(to[1]);
        close(from[0]);

        execl(AgentAddress, AgentAddress, NULL); // 에이전트 파일 실행
    }

    Child = pid; // 자식 프로세스 ID 저장
    alarm(TimeLimit); // 에이전트 실행 시간 제한 시작
    close(to[0]);
    close(from[1]);

    PlayerBoard(player, board, to[1]); // 에이전트에게 입력 전달
    close(to[1]); // 쓰기 닫기

    char move = 0;
    read(from[0], &move, 1); // 자식 프로세스에서 받은 출력값을 move에 저장

    close(from[0]); // 읽기 끝 닫기
    waitpid(pid, NULL, 0); // 자식 프로세스 종료 대기
    alarm(0); // 알람 취소
    Child = -1; // 현재 자식 프로세스 ID 초기화

    return move; // 에이전트의 이동 반환
}

int CheckWinner(int board[6][7], int player) { // 승리 조건을 체크하는 함수
    for (int i = 0; i < 6; ++i)
        for (int j = 0; j < 4; ++j)
            if (board[i][j] == player && board[i][j+1] == player && board[i][j+2] == player && board[i][j+3] == player)
                return 1; // 승리 조건 충족 시 1 반환

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 7; ++j)
            if (board[i][j] == player && board[i+1][j] == player && board[i+2][j] == player && board[i+3][j] == player)
                return 1;

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 4; ++j)
            if (board[i][j] == player && board[i+1][j+1] == player && board[i+2][j+2] == player && board[i+3][j+3] == player)
                return 1;

    for (int i = 0; i < 3; ++i)
        for (int j = 3; j < 7; ++j)
            if (board[i][j] == player && board[i+1][j-1] == player && board[i+2][j-2] == player && board[i+3][j-3] == player)
                return 1;

    return 0; // 승리 조건 불충족 시 0 반환
}

int BoardFull(int board[6][7]) { // 보드가 가득 찼는지 체크하는 함수
    for (int j = 0; j < 7; ++j) {
        if (board[0][j] == 0) return 0;  // 가장 위가 비어있으면 여유 있음
    }
    return 1; // 모든 열이 가득 차면 1 반환
}

void print_board(int board[6][7]) {
    printf("\n   A B C D E F G\n");
    printf("  ---------------\n");
    for (int i = 0; i < 6; ++i) {
        printf("%d| ", i + 1);
        for (int j = 0; j < 7; ++j) {
            if (board[i][j] == 0) printf(". ");
            else printf("%d ", board[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    char *AgentX = argv[2]; // 첫번째 에이전트 경로 저장
    char *AgentY = argv[4]; // 두번째 에이전트 경로 저장

    signal(SIGINT, SigintHandler);
    signal(SIGALRM, SigalrmHandler);
    signal(SIGTERM, SigtermHandler);
    signal(SIGCHLD, SigchldHandler);

    int board[6][7] = {0};
    int turn = 0;

    while (1) {
        int player = (turn % 2 == 0) ? 1 : 2; // 현재 플레이어 결정

        char *agent = (player == 1) ? AgentX : AgentY; // 플레이어에 맞춰 에이전트 선택
        printf("\n");

        char move = RunAgent(agent, player, board); // 에이전트의 이동 요청
        if (move < 'A' || move > 'G') { // 잘못된 이동
            printf("%c 패배\n", (player == 1) ? 'X' : 'Y');
            break;
        }

        int col = move - 'A'; // 이동한 열 계산
        int place = 0; // 놓인 여부 초기화
        for (int i = 5; i >= 0; --i) { // 보드의 각 행을 아래에서 위로 반복
            if (board[i][col] == 0) { // 현재 위치가 비어있으면
                board[i][col] = player; // 플레이어의 말을 놓음
                place = 1; // 놓인 상태 업데이트
                break; // 반복 종료
            }
        }

        printf("%d\n", player);
        print_board(board); // 보드 출력

        if (CheckWinner(board, player)) { // 현재 플레이어의 승리 체크
            printf("%c 승리\n", (player == 1) ? 'X' : 'Y');
            break;
        }
        if (BoardFull(board)) {
            printf("무승부\n");
            break;
        }
        if (!place) { // 놓지 못한 경우
            printf("가득 찬 열 선택, %c 패배\n", (player == 1) ? 'X' : 'Y'); // 패배 메시지 출력
            break; // 게임 종료
        }
        turn++; // 턴 증가
        sleep(1);  // 턴 간 대기
    }
    return 0; // 프로그램 종료
}
