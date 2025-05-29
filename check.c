#include "INCLUDES.H"

/**
 * 물건을 이동할 때, 현재 y, x에서 left,right,up,down 중 어디를 갈 수 있는지 return
 * bitmasking으로 left,right,up,down 갈 수 있다는 것을 표시
 * 예를 들어, return 10 이라면, 1010 이므로 left와 up을 갈 수 있음.
 */
int possibleGoAll(enum LandInfo map[MAP_SIZE][MAP_SIZE], int y, int x) {
    int ret = 0;
    if (x-1>=0 && (map[y][x-1] == LEFT_CONVEYOR ||  map[y][x-1] == SEPERATEOR))
        ret += (1 << 3); //possible go to left
    if (x+1<MAP_SIZE && (map[y][x+1] == RIGHT_CONVEYOR || map[y][x+1] == SEPERATEOR))
        ret += (1 << 2); //possible go to right
    if (y-1>=0 && (map[y-1][x] == UP_CONVEYOR || map[y-1][x] == SEPERATEOR))
        ret += (1 << 1); //possible go to up
    if (y+1<MAP_SIZE && (map[y+1][x] == DOWN_CONVEYOR || map[y+1][x] == SEPERATEOR))
        ret += (1 << 0); //possible go to down
    
    return (ret);
}

int possibleGoLeft(enum LandInfo map[MAP_SIZE][MAP_SIZE], int y, int x) {
    int ret = 0;
    if (x-1>=0 && (map[y][x-1] == LEFT_CONVEYOR || map[y][x-1] == SEPERATEOR))
        ret = 1 ; //possible go to left
    return (ret);
}

int possibleGoRight(enum LandInfo map[MAP_SIZE][MAP_SIZE], int y, int x) {
    int ret = 0;
    if (x+1<MAP_SIZE && (map[y][x+1] == RIGHT_CONVEYOR || map[y][x+1] == SEPERATEOR))
        ret = 1; //possible go to right
    return (ret);
}

int possibleGoUp(enum LandInfo map[MAP_SIZE][MAP_SIZE], int y, int x) {
    int ret = 0;
    if (y-1>=0 && (map[y-1][x] == UP_CONVEYOR || map[y-1][x] == SEPERATEOR))
        ret = 1; //possible go to up
    return (ret);
}

int possibleGoDown(enum LandInfo map[MAP_SIZE][MAP_SIZE], int y, int x) {
    int ret = 0;
    if (y+1<MAP_SIZE && (map[y+1][x] == DOWN_CONVEYOR || map[y+1][x] == SEPERATEOR))
        ret = 1; //possible go to down
    return (ret);
}