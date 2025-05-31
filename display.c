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
    else if (landinfo == SHOP)
        return ('@');
    else if (ORIGIN_WOOD <= landinfo && landinfo <= ORIGIN_MINE)
        return ('&');
}

void DrawGrid(enum LandInfo map[MAP_SIZE][MAP_SIZE], int cursor_x, int cursor_y, Resource somethingMap[MAP_SIZE][MAP_SIZE], int money, int diggerTime) {
    system("cls"); // 화면 지우기
    printf("diggerTime : %d(s)\n", diggerTime);
    printf("money : %d$\n", money);
    for (int y = 0; y < MAP_SIZE; y++) {
        for (int x = 0; x < MAP_SIZE; x++) {
            if (x == cursor_x && y == cursor_y) {
                printf("(%c", convert_landInfo_to_char(map[y][x]));
                somethingMap[y][x].have ? printf("*") : 0;
                printf(")\t");
            }
            else {
                printf("%c", convert_landInfo_to_char(map[y][x]));
                somethingMap[y][x].have ? printf("*") : 0;
                printf("\t");
            }
        }
        printf("\n");
    }
    printf("Press 'J' maked money\n");
    fflush(stdout);
}

void Display_select(Resource rs, enum LandInfo land) {
    system("cls"); //화면 지우기
    printf("- Land Information: %s\n", landinfoString[land]);
    printf("- Current Resources -> Wood: %d, Sand: %d, Coal: %d, Iron: %d, Gold: %d\n", rs.wood, rs.sand, rs.coal, rs.iron, rs.gold);
    printf("What you want to build?\n");
    printf("a : Digger => #\n");
    printf("b : left Conveyor => <\t(%d$)\n", BUY_CONVEYOR);
    printf("c : right Conveyor=> >\t(%d$)\n", BUY_CONVEYOR);
    printf("d : up Conveyor => ^\t(%d$)\n", BUY_CONVEYOR);
    printf("e : down Conveyor => v\t(%d$)\n", BUY_CONVEYOR);
    printf("f : Seperator => +\t(%d$)\n", BUY_SEPERATOR);
    printf("q : destory\n");
    printf("esc : back to main\n");
}

void Display_Digger_select(enum LandInfo land) {
    system("cls");
    printf("- Land Information: %s\n", landinfoString[land]);
    printf("Choose Digger\n");
    printf("a : Wood (%d$)\n", BUY_WOOD_DIGGER);
    printf("b : Sand (%d$)\n", BUY_SAND_DIGGER);
    printf("c : Coal (%d$)\n", BUY_COAL_DIGGER);
    printf("d : Iron (%d$)\n", BUY_IRON_DIGGER);
    printf("e : Gold (%d$)\n", BUY_GOLD_DIGGER);
}

void Display_Error(const char* errorMsg) {
    system("cls");
    printf("%s\n", errorMsg);
    printf("Press any key : to continue...\n");
}