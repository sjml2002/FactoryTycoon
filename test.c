#include "INCLUDES.H"
#include "custom.h"
# define TASK_STK_SIZE 512
# define DIGGER_TIME 3 //채굴기는 3초에 한번씩

// Task Stack Memory 지정
OS_STK MainTaskStk[TASK_STK_SIZE];
OS_STK UserInputStk[TASK_STK_SIZE];
OS_STK ConveyorTaskStk[TASK_STK_SIZE];
void MainTask(void *pdata);
void UserInputTask(void *pdata);
void ConveyorTask(void *pdata);

// 변수 정의
OS_EVENT *mutex;
int cursor_x = 0;
int cursor_y = 0;
int diggerTime = 0;

enum LandInfo map[MAP_SIZE][MAP_SIZE];
Resource somethingMap [MAP_SIZE][MAP_SIZE]; //물건이 있는지만의 정보를 담은
void map_init() {
    for(int i=0; i<MAP_SIZE; i++) {
        for(int j=0; j<MAP_SIZE; j++)
            map[i][j] = EMPTY;
    }
}

int main(void) {
    map_init();
    OSInit();
    SetConsoleOutputCP(CP_UTF8);   // UTF-8 출력
    SetConsoleCP(CP_UTF8);         // UTF-8 입력

    INT8U err;
    mutex = OSMutexCreate(1, &err); // 우선순위 10의 뮤텍스 생성
    if (mutex == NULL) {
        printf("mutex Error: %d\n", err);
        return (-1);
    }
    OSTaskCreate(UserInputTask, (void *)0, &UserInputStk[TASK_STK_SIZE - 1], 2);
    OSTaskCreate(MainTask, (void *)0, &MainTaskStk[TASK_STK_SIZE - 1], 3);
    OSTaskCreate(ConveyorTask, (void*)0, &ConveyorTaskStk[TASK_STK_SIZE-1],4);
    OSStart();
    return 0;
}

void MainTask(void *pdata) {
    while (1) {
        // 격자 출력 (위치 표시)
        INT8U err;
        OSMutexPend(mutex, 0, &err); // 위치 공유를 위한 뮤텍스 획득
        DrawGrid(map, cursor_x, cursor_y, somethingMap);
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
                    if (cursor_y < MAP_SIZE - 1)
                        cursor_y++;
                    break;
                case 75: // ←
                    if (cursor_x > 0)
                        cursor_x--;
                    break;
                case 77: // →
                    if (cursor_x < MAP_SIZE - 1)
                        cursor_x++;
                    break;
                }
                OSMutexPost(mutex); // 뮤텍스 해제
            }
            else if (ch == 13) {  //Enter Key                   
                Display_select(somethingMap[cursor_y][cursor_x]);
                while (1) {
                    if (_kbhit()) {
                        ch = _getch();
                        if (ch == 27) // esc : 돌아가기
                            break;
                        else if (ch == 97 || ch == 65) { // 'a,A'
                            Display_Digger_select();
                            while (1) { //채굴기 고르기
                                if (_kbhit()) {
                                    ch = _getch();
                                    if (ch==97 || ch==65) { //'a, A' : woodDigger
                                        map[cursor_y][cursor_x] = WOOD_DIGGER;
                                        break ;
                                    }
                                    else if (ch == 98 || ch == 66) { // 'b,B' : sandDigger
                                        map[cursor_y][cursor_x] = SAND_DIGGER;
                                        break ;
                                    }
                                    else if (ch == 99 || ch == 67) { // 'c,C' : coalDigger
                                        map[cursor_y][cursor_x] = COAL_DIGGER;
                                        break ;
                                    }
                                    else if (ch == 100 || ch == 68) { // 'd,D' : ironDigger
                                        map[cursor_y][cursor_x] = IRON_DIGGER;
                                        break ;
                                    }
                                    else if (ch == 101 || ch == 69) { // 'e,E' : goldDigger
                                        map[cursor_y][cursor_x] = GOLD_DIGGER;
                                        break ;
                                    }
                                }
                            }
                            break ;
                        }
                        else if (ch == 98 || ch == 66) { // 'b,B'
                            map[cursor_y][cursor_x] = LEFT_CONVEYOR;
                            break ;
                        }
                        else if (ch == 99 || ch == 67) { // 'c,C'
                            map[cursor_y][cursor_x] = RIGHT_CONVEYOR;
                            break ;
                        }
                        else if (ch == 100 || ch == 68) { // 'd,D'
                            map[cursor_y][cursor_x] = UP_CONVEYOR;
                            break ;
                        }
                        else if (ch == 101 || ch == 69) { // 'e,E'
                            map[cursor_y][cursor_x] = DOWN_CONVEYOR;
                            break ;
                        }
                        else if (ch == 102 || ch == 70) { // 'f,F'
                            map[cursor_y][cursor_x] = SEPERATEOR;
                            break ;
                        }
                        else if (ch == 113 || ch == 81) { // 'q,Q'
                            map[cursor_y][cursor_x] = EMPTY;
                            break ;
                        }
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

void ConveyorTask(void* pdata) {
    //이미 이동한 것은 또 이동하지 못하도록
    int cacheMap[MAP_SIZE][MAP_SIZE] = {0};

    while (1) {
        //initialization
        for(int y=0; y<MAP_SIZE; y++) {
            for(int x=0; x<MAP_SIZE; x++)
                cacheMap[y][x] = 0;
        }

        for(int y=0; y<MAP_SIZE; y++) {
            for(int x=0; x<MAP_SIZE; x++) {
                if (cacheMap[y][x]) { //이미 이동했다면 continue
                    continue;
                }
                if ((WOOD_DIGGER <= map[y][x] && map[y][x] <= GOLD_DIGGER) || (somethingMap[y][x].have && map[y][x] == SEPERATEOR)) {
                    //somethingMap[y][x] > 0 ? somethingMap[y][x]-- : somethingMap = 0;
                    if (diggerTime % DIGGER_TIME != 0) {
                        diggerTime++;
                        continue ;
                    }
                    diggerTime++;
                    //Digger || Seperator
                    int pg = possibleGoAll(map, y, x);
                    if (pg & 8) { //possible go to left
                        //somethingMap[y][x-1]++;
                        cacheMap[y][x-1] = 1;
                    }
                    if (pg & 4) { //possible go to right
                        //somethingMap[y][x+1]++;
                        cacheMap[y][x+1] = 1;
                    }
                    if (pg & 2) { //possible go to up
                        //somethingMap[y-1][x]++;
                        cacheMap[y-1][x] = 1;
                    }
                    if (pg & 1) { //possible go to down
                        //somethingMap[y+1][x]++;
                        cacheMap[y+1][x] = 1;
                    }
                }
                else if (somethingMap[y][x] && map[y][x] == LEFT_CONVEYOR && possibleGoLeft(map, y, x)) {
                    //somethingMap[y][x] > 0 ? somethingMap[y][x]-- : somethingMap = 0;
                    //somethingMap[y][x-1]++;
                    cacheMap[y][x-1] = 1;
                }
                else if (somethingMap[y][x] && map[y][x] == RIGHT_CONVEYOR && possibleGoRight(map, y, x)) {
                    //somethingMap[y][x] > 0 ? somethingMap[y][x]-- : somethingMap = 0;
                    //somethingMap[y][x+1]++;
                    cacheMap[y][x+1] = 1;
                }
                else if (somethingMap[y][x] && map[y][x] == UP_CONVEYOR && possibleGoUp(map, y, x)) {
                    //somethingMap[y][x] > 0 ? somethingMap[y][x]-- : somethingMap = 0;
                    //somethingMap[y-1][x]++;
                    cacheMap[y-1][x] = 1;
                }
                else if (somethingMap[y][x] && map[y][x] == DOWN_CONVEYOR && possibleGoDown(map, y, x)) {
                    //somethingMap[y][x] > 0 ? somethingMap[y][x]-- : somethingMap = 0;
                    //somethingMap[y+1][x]++;
                    cacheMap[y+1][x] = 1;
                }
            }
        }  
        OSTimeDly(1);
    }
}