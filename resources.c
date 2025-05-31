#include "INCLUDES.H"

const char* landinfoString[15] = {
    "EMPTY", // .
    "WOOD_DIGGER",
    "SAND_DIGGER",
    "COAL_DIGGER",
    "IRON_DIGGER",
    "GOLD_DIGGER", //Digger END
    "LEFT_CONVEYOR",
    "RIGHT_CONVEYOR",
    "UP_CONVEYOR",
    "DOWN_CONVEYOR",
    "SEPERATEOR",
    "SHOP",
    "ORIGIN_WOOD",
    "ORIGIN_SAND",
    "ORIGIN_MINE", //possible dig to coal, iron, gold
};

//자원을 가지고 있는지
int resourceHaved(Resource rs) {
    if (rs.wood==0 && rs.sand==0 && rs.coal==0 && rs.iron==0 && rs.gold==0)
        return (0);
    return (1);
}

Resource init_resources(int w, int s, int c, int i, int g) {
    Resource rs;
    rs.wood = w;
    rs.sand = s;
    rs.coal = c;
    rs.iron = i;
    rs.gold = g;
    rs.have = resourceHaved(rs);
    return (rs);
}

/**
 * 자원의 증가, 감소 연산
 * @param r 자원의 종류 (1:wood, 2:sand: 3:coal, 4:iron, 5:gold)
 * @param o 연산의 종류 (1 or -1)
 */
void resource_operation(Resource* rs, int r, int o) {
    if (r == 1)
        rs->wood += o;
    else if (r == 2)
        rs->sand += o;
    else if (r == 3)
        rs->coal += o;
    else if (r == 4)
        rs->iron += o;
    else if (r == 5)
        rs->gold += o;
    rs->have = resourceHaved(*rs);
}

/**
 * Plus(+) operation in Two Resources
 */
Resource resource_plus(Resource r1, Resource r2) {
    Resource ret;
    ret.wood = r1.wood + r2.wood;
    ret.sand = r1.sand + r2.sand;
    ret.coal = r1.coal + r2.coal;
    ret.iron = r1.iron + r2.iron;
    ret.gold = r1.gold + r2.gold;
    ret.have = resourceHaved(ret);
    return (ret);
}