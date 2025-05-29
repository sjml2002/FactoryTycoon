#include "INCLUDES.H"

/*
    @ : 사용자
    # : 채굴기
    ^ < v >: 컨베이어 벨트
    + : 분배기
    @ : 상점
    * : 이동 중인 물건
*/
char convert_landInfo_to_char(int landinfo) {
    if (landinfo == EMPTY)
        return ('.');
    else if (WOOD_DIGGER <= landinfo && landinfo <= GOLD_DIGGER)
        return ('#');
    else if (landinfo == LEFT_CONVEYOR)
        return ('<');
    else if (landinfo == RIGHT_CONVEYOR)
        return ('>');
    else if (landinfo == UP_CONVEYOR)
        return ('^');
    else if (landinfo == DOWN_CONVEYOR)
        return ('v');
    else if (landinfo == SEPERATEOR)
        return ('+');
}

void DrawGrid(enum LandInfo map[MAP_SIZE][MAP_SIZE], int cursor_x, int cursor_y, int somethingMap[MAP_SIZE][MAP_SIZE]) {
    system("cls"); // 화면 지우기
    for (int y = 0; y < MAP_SIZE; y++) {
        for (int x = 0; x < MAP_SIZE; x++) {
            if (x == cursor_x && y == cursor_y) {
                printf("(%c", convert_landInfo_to_char(map[y][x]));
                somethingMap[y][x] ? printf("*") : 0;
                printf(")\t");
            }
            else if (x == MAP_SIZE-1 && y == MAP_SIZE-1)
                printf("@\t"); //상점
            else {
                printf("%c", convert_landInfo_to_char(map[y][x]));
                somethingMap[y][x] ? printf("*") : 0;
                printf("\t");
            }
        }
        printf("\n");
    }
    fflush(stdout);
}

void Display_select(Resource rs) {
    system("cls"); //화면 지우기
    printf("Current Resources\n");
    printf("Wood: %d, Sand: %d, Coal: %d, Iron: %d, Gold: %d\n", rs.wood, rs.sand, rs.coal, rs.iron, rs.gold);
    printf("What you want to build?\n");
    printf("a : Digger => #\n");
    printf("b : left Conveyor => <\n");
    printf("c : right Conveyor => >\n");
    printf("d : up Conveyor => ^\n");
    printf("e : down Conveyor => v\n");
    printf("f : Seperator => +\n");
    printf("q : destory\n");
    printf("esc : back to main\n");
}

void Display_Digger_select() {
    system("cls");
    printf("Choose Digger\n");
    printf("a : Wood\n");
    printf("b : Sand\n");
    printf("c : Coal\n");
    printf("d : Iron\n");
    printf("e : Gold\n");
}