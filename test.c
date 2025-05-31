#include "INCLUDES.H"
#include "custom.h"
# define TASK_STK_SIZE 512
# define DIGGER_TIME 5 //채굴기는 x초에 한번씩
# define WOOD_MONEY 1
# define SAND_MONEY 2
# define COAL_MONEY 3
# define IRON_MONEY 4
# define GOLD_MONEY 5

// Task Stack Memory 지정
OS_STK MainStk[TASK_STK_SIZE];
OS_STK UserInputStk[TASK_STK_SIZE];
OS_STK DiggerStk[TASK_STK_SIZE];
OS_STK ConveyorStk[TASK_STK_SIZE];
OS_STK ShopStk[TASK_STK_SIZE];
void MainTask(void *pdata);
void UserInputTask(void *pdata);
void DiggerTask(void *pdata);
void ConveyorTask(void *pdata);
void ShopTask(void* pdata);

// 변수 정의
OS_EVENT *mutex;
OS_EVENT *queuemsg;
int cursor_x = 0;
int cursor_y = 0;
int diggerTime = -1;
int money = 30;

enum LandInfo map[MAP_SIZE][MAP_SIZE];
Resource somethingMap[MAP_SIZE][MAP_SIZE]; //물건이 있는지만의 정보를 담은
void map_init() {
    for(int i=0; i<MAP_SIZE; i++) {
        for(int j=0; j<MAP_SIZE; j++)
            map[i][j] = EMPTY;
    }
    map[3][4] = ORIGIN_WOOD;
    map[5][2] = ORIGIN_SAND;
    map[8][3] = ORIGIN_MINE;
    map[0][7] = ORIGIN_MINE; 
    map[MAP_SIZE-1][MAP_SIZE-1] = SHOP;
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
    void* q_buf[20]; //message queue buffer
    queuemsg = OSQCreate(&q_buf[0], 20);

    OSTaskCreate(UserInputTask, (void *)0, &UserInputStk[TASK_STK_SIZE - 1], 2);
    OSTaskCreate(MainTask, (void *)0, &MainStk[TASK_STK_SIZE - 1], 3);
    OSTaskCreate(DiggerTask, (void*)0, &DiggerStk[TASK_STK_SIZE-1], 4);
    OSTaskCreate(ConveyorTask, (void*)0, &ConveyorStk[TASK_STK_SIZE-1], 5);
    OSTaskCreate(ShopTask, (void*)0, &ShopStk[TASK_STK_SIZE-1], 6);
    OSStart();
    return 0;
}

void MainTask(void *pdata) {
    while (1) {
        // 격자 출력 (위치 표시)
        INT8U err;
        INT32U* msg;
        do { //message queue가 빌 때까지 money 처리
            msg = (INT32U*)OSQAccept(queuemsg);
            if (msg != NULL) {
                money += *msg;
                free(msg); //msg는 사용 후 해제
            }
        } while (msg != NULL);
        OSMutexPend(mutex, 0, &err); // 위치 공유를 위한 뮤텍스 획득
        DrawGrid(map, cursor_x, cursor_y, somethingMap, money);
        OSMutexPost(mutex); // 뮤텍스 반환
        OSTimeDly(1);
    }
}

void UserInputTask(void *pdata) {
    INT8U err;
    while (1) {
        int* msg = (int*)malloc(sizeof(int));
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
                Display_select(somethingMap[cursor_y][cursor_x], map[cursor_y][cursor_x]);
                while (1) {
                    if (_kbhit()) {
                        ch = _getch();
                        if (ch == 27) // esc : 돌아가기
                            break;
                        else if (ch == 97 || ch == 65) { // 'a,A'
                            Display_Digger_select(map[cursor_y][cursor_x]);
                            while (1) { //채굴기 고르기
                                if (_kbhit()) {
                                    ch = _getch();
                                    if (ch==97 || ch==65) { //'a, A' : woodDigger
                                        if (isPossibleBuildDigger(WOOD_DIGGER, map[cursor_y][cursor_x], money)) {
                                            map[cursor_y][cursor_x] = WOOD_DIGGER;
                                            *msg = -BUY_WOOD_DIGGER;
                                            OSQPost(queuemsg, (void *)msg);
                                            //money -= BUY_WOOD_DIGGER;
                                        }
                                        else {
                                            while (1) {
                                                Display_Error("Is not possible to Build Digger");
                                                if (_kbhit())
                                                    break ;
                                            }
                                        }
                                        break ;
                                    }
                                    else if (ch == 98 || ch == 66) { // 'b,B' : sandDigger
                                        if (isPossibleBuildDigger(SAND_DIGGER, map[cursor_y][cursor_x], money)) {
                                            map[cursor_y][cursor_x] = SAND_DIGGER;
                                            *msg = -BUY_SAND_DIGGER;
                                            OSQPost(queuemsg, (void *)msg);
                                            //money -= BUY_SAND_DIGGER;
                                        }
                                        else {
                                            while (1) {
                                                Display_Error("Is not possible to Build Digger");
                                                if (_kbhit())
                                                    break ;
                                            }
                                        }
                                        break ;
                                    }
                                    else if (ch == 99 || ch == 67) { // 'c,C' : coalDigger
                                        if (isPossibleBuildDigger(COAL_DIGGER, map[cursor_y][cursor_x], money)) {
                                            map[cursor_y][cursor_x] = COAL_DIGGER;
                                            *msg = -BUY_COAL_DIGGER;
                                            OSQPost(queuemsg, (void *)msg);
                                            //money -= BUY_COAL_DIGGER;
                                        }
                                        else {
                                            while (1) {
                                                Display_Error("Is not possible to Build Digger");
                                                if (_kbhit())
                                                    break ;
                                            }
                                        }
                                        break ;
                                    }
                                    else if (ch == 100 || ch == 68) { // 'd,D' : ironDigger
                                        if (isPossibleBuildDigger(IRON_DIGGER, map[cursor_y][cursor_x], money)) {
                                            map[cursor_y][cursor_x] = IRON_DIGGER;
                                            *msg = -BUY_IRON_DIGGER;
                                            OSQPost(queuemsg, (void *)msg);
                                            //money -= BUY_IRON_DIGGER;
                                        }
                                        else {
                                            while (1) {
                                                Display_Error("Is not possible to Build Digger");
                                                if (_kbhit())
                                                    break ;
                                            }
                                        }
                                        break ;
                                    }
                                    else if (ch == 101 || ch == 69) { // 'e,E' : goldDigger
                                        if (isPossibleBuildDigger(GOLD_DIGGER, map[cursor_y][cursor_x], money)) {
                                            map[cursor_y][cursor_x] = GOLD_DIGGER;
                                            *msg = -BUY_GOLD_DIGGER;
                                            OSQPost(queuemsg, (void *)msg);
                                            //money -= BUY_GOLD_DIGGER;
                                        }
                                        else {
                                            while (1) {
                                                Display_Error("Is not possible to Build Digger");
                                                if (_kbhit())
                                                    break ;
                                            }
                                        }
                                        break ;
                                    }
                                }
                            }
                            break ;
                        }
                        else if (ch == 98 || ch == 66) { // 'b,B'
                            if (money >= BUY_CONVEYOR) {
                                map[cursor_y][cursor_x] = LEFT_CONVEYOR;
                                *msg = -BUY_CONVEYOR;
                                OSQPost(queuemsg, (void *)msg);
                                //money -= BUY_CONVEYOR;
                            }
                            else {
                                while (1) {
                                    Display_Error("Short of Money...");
                                    if (_kbhit())
                                        break ;
                                }
                            }
                            break ;
                        }
                        else if (ch == 99 || ch == 67) { // 'c,C'
                            if (money >= BUY_CONVEYOR) {
                                map[cursor_y][cursor_x] = RIGHT_CONVEYOR;
                                *msg = -BUY_CONVEYOR;
                                OSQPost(queuemsg, (void *)msg);
                                //money -= BUY_CONVEYOR;
                            }
                            else {
                                while (1) {
                                    Display_Error("Short of Money...");
                                    if (_kbhit())
                                        break ;
                                }
                            }
                            break ;
                        }
                        else if (ch == 100 || ch == 68) { // 'd,D'
                            if (money >= BUY_CONVEYOR) {
                                map[cursor_y][cursor_x] = UP_CONVEYOR;
                                *msg = -BUY_CONVEYOR;
                                OSQPost(queuemsg, (void *)msg);
                                //money -= BUY_CONVEYOR;
                            }
                            else {
                                while (1) {
                                    Display_Error("Short of Money...");
                                    if (_kbhit())
                                        break ;
                                }
                            }
                            break ;
                        }
                        else if (ch == 101 || ch == 69) { // 'e,E'
                            if (money >= BUY_CONVEYOR) {
                                map[cursor_y][cursor_x] = DOWN_CONVEYOR;
                                *msg = -BUY_CONVEYOR;
                                OSQPost(queuemsg, (void *)msg);
                                //money -= BUY_CONVEYOR;
                            }
                            else {
                                while (1) {
                                    Display_Error("Short of Money...");
                                    if (_kbhit())
                                        break ;
                                }
                            }
                            break ;
                        }
                        else if (ch == 102 || ch == 70) { // 'f,F'
                            if (money >= BUY_SEPERATOR) {
                                map[cursor_y][cursor_x] = SEPERATEOR;
                                *msg = -BUY_SEPERATOR;
                                OSQPost(queuemsg, (void *)msg);
                                //money -= BUY_SEPERATOR;
                            }
                            else {
                                while (1) {
                                    Display_Error("Short of Money...");
                                    if (_kbhit())
                                        break ;
                                }
                            }
                            break ;
                        }
                        else if (ch == 113 || ch == 81) { // 'q,Q'
                            map[cursor_y][cursor_x] = EMPTY;
                            break ;
                        }
                    }
                }
            }
            else if (ch == 74 || ch == 106) { //'j, J' key
                *msg = 1;
                OSQPost(queuemsg, (void *)msg);
                //money++;
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

void DiggerTask(void* pdata) {
    while (1) {
        diggerTime = (diggerTime + 1) % DIGGER_TIME;
        if (diggerTime == 0) {
            //create resource
            for(int y=0; y<MAP_SIZE; y++) {
                for(int x=0; x<MAP_SIZE; x++) {
                    if (somethingMap[y][x].have) //이미 생산한 물품이 있다면 또 생산하지는 않음.
                        continue ;
                    else if (map[y][x] == WOOD_DIGGER)
                        resource_operation(&somethingMap[y][x], 1, 1);
                    else if (map[y][x] == SAND_DIGGER)
                        resource_operation(&somethingMap[y][x], 2, 1);
                    else if (map[y][x] == COAL_DIGGER)
                        resource_operation(&somethingMap[y][x], 3, 1);
                    else if (map[y][x] == IRON_DIGGER)
                        resource_operation(&somethingMap[y][x], 4, 1);
                    else if (map[y][x] == GOLD_DIGGER)
                        resource_operation(&somethingMap[y][x], 5, 1);
                }
            }
        }
        OSTimeDly(1);
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
                    if ((WOOD_DIGGER <= map[y][x] && map[y][x] <= GOLD_DIGGER) && (diggerTime % DIGGER_TIME != 0))
                        continue ;
                    //Digger || Seperator
                    int pg = possibleGoAll(map, y, x);
                    if (pg & 8) { //possible go to left
                        somethingMap[y][x-1] = resource_plus(somethingMap[y][x-1], somethingMap[y][x]);
                        cacheMap[y][x-1] = 1;
                    }
                    if (pg & 4) { //possible go to right
                        somethingMap[y][x+1] = resource_plus(somethingMap[y][x+1], somethingMap[y][x]);
                        cacheMap[y][x+1] = 1;
                    }
                    if (pg & 2) { //possible go to up
                        somethingMap[y-1][x] = resource_plus(somethingMap[y-1][x], somethingMap[y][x]);
                        cacheMap[y-1][x] = 1;
                    }
                    if (pg & 1) { //possible go to down
                        somethingMap[y+1][x] = resource_plus(somethingMap[y+1][x], somethingMap[y][x]);
                        cacheMap[y+1][x] = 1;
                    }
                    if ((pg & 15) != 0)
                        somethingMap[y][x] = init_resources(0, 0, 0, 0, 0);

                }
                else if (somethingMap[y][x].have && map[y][x] == LEFT_CONVEYOR && possibleGoLeft(map, y, x)) {
                    somethingMap[y][x-1] = resource_plus(somethingMap[y][x-1], somethingMap[y][x]);
                    somethingMap[y][x] = init_resources(0, 0, 0, 0, 0);
                    cacheMap[y][x-1] = 1;
                }
                else if (somethingMap[y][x].have && map[y][x] == RIGHT_CONVEYOR && possibleGoRight(map, y, x)) {
                    somethingMap[y][x+1] = resource_plus(somethingMap[y][x+1], somethingMap[y][x]);
                    somethingMap[y][x] = init_resources(0, 0, 0, 0, 0);
                    cacheMap[y][x+1] = 1;
                }
                else if (somethingMap[y][x].have && map[y][x] == UP_CONVEYOR && possibleGoUp(map, y, x)) {
                    somethingMap[y-1][x] = resource_plus(somethingMap[y-1][x], somethingMap[y][x]);
                    somethingMap[y][x] = init_resources(0, 0, 0, 0, 0);
                    cacheMap[y-1][x] = 1;
                }
                else if (somethingMap[y][x].have && map[y][x] == DOWN_CONVEYOR && possibleGoDown(map, y, x)) {
                    somethingMap[y+1][x] = resource_plus(somethingMap[y+1][x], somethingMap[y][x]);
                    somethingMap[y][x] = init_resources(0, 0, 0, 0, 0);
                    cacheMap[y+1][x] = 1;
                }
            }
        }  
        OSTimeDly(1);
    }
}

void ShopTask(void* pdata) {
    while (1) {
        int* msg = (int*)malloc(sizeof(int));
        *msg = 0;
        Resource sellResource = somethingMap[MAP_SIZE-1][MAP_SIZE-1];
        *msg += (sellResource.wood*WOOD_MONEY);
        *msg += (sellResource.sand*SAND_MONEY);
        *msg += (sellResource.coal*COAL_MONEY);
        *msg += (sellResource.iron*IRON_MONEY);
        *msg += (sellResource.gold*GOLD_MONEY);
        somethingMap[MAP_SIZE-1][MAP_SIZE-1] = init_resources(0, 0, 0, 0, 0);
        OSQPost(queuemsg, (void *)msg);
        OSTimeDly(1);
    }
}