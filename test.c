#include "INCLUDES.H"
#include "custom.h"
#define TASK_STK_SIZE 512
#define USER_INPUT_PRIO 2 // 입력 우선순위

// Task Stack Memory 지정
OS_STK MainTaskStk[TASK_STK_SIZE];
OS_STK UserInputStk[TASK_STK_SIZE];
void MainTask(void *data);
void UserInputTask(void *pdata);

// 변수 정의
OS_EVENT *mutex;
int cursor_x = 0;
int cursor_y = 0;

int main(void) {
    OSInit();

    INT8U err;
    mutex = OSMutexCreate(1, &err); // 우선순위 10의 뮤텍스 생성
    if (mutex == NULL) {
        printf("mutex Error: %d\n", err);
        return (-1);
    }
    err = OSTaskCreate(UserInputTask, (void *)0, &UserInputStk[TASK_STK_SIZE - 1], USER_INPUT_PRIO);
    err = OSTaskCreate(MainTask, (void *)0, &MainTaskStk[TASK_STK_SIZE - 1], 3);
    OSStart();
    return 0;
}

void MainTask(void *pdata) {
    while (1) {
        // 격자 출력 (위치 표시)
        INT8U err;
        OSMutexPend(mutex, 0, &err); // 위치 공유를 위한 뮤텍스 획득
        DrawGrid(cursor_x, cursor_y);
        OSMutexPost(mutex); // 뮤텍스 반환
        OSTimeDly(1);
    }
}

void UserInputTask(void *pdata) {
    INT8U err;
    while (1) {
        if (_kbhit()) {
            int ch = _getch();

            if (ch == 0 || ch == 0xE0) { // 방향키는 0 또는 0xE0 후에 실제 코드가 옴
                int key = _getch();
                OSMutexPend(mutex, 0, &err); // 위치 갱신을 위해 뮤텍스 잠금
                switch (key) {
                case 72: // ↑
                    if (cursor_y > 0)
                        cursor_y--;
                    break;
                case 80: // ↓
                    if (cursor_y < GRID_SIZE - 1)
                        cursor_y++;
                    break;
                case 75: // ←
                    if (cursor_x > 0)
                        cursor_x--;
                    break;
                case 77: // →
                    if (cursor_x < GRID_SIZE - 1)
                        cursor_x++;
                    break;
                }
                OSMutexPost(mutex); // 뮤텍스 해제
            }
            else if (ch == 13) {                                 // Enter key
                printf("아니 enter는 눌림?"); // debug
                Display_select();
                while (1) {
                    if (_kbhit()) {
                        ch = _getch();
                        printf("ch after enter: %d\n", ch); //debug
                        if (ch == 27) // esc : 돌아가기
                            break;
                    }
                }
            }
            else {
                OSTimeDly(1);
            }
        }
        else {
            OSTimeDly(1);
        }
    }
}