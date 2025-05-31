/*
*********************************************************************************************************
*                                                uC/OS-II
*                                The Real-Time Kernel (by Jean J. Labrosse)
*
*					WIN32 PORT & LINUX PORT
*
*                          (c) Copyright 2004-... Werner.Zimmermann@fht-esslingen.de
*                                           All Rights Reserved
*
* File : INCLUDES.H
* By   : Werner Zimmermann
*********************************************************************************************************
*/
#ifndef INCLUDES_H
#define INCLUDES_H

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <time.h>
#include <string.h>

#ifndef __GNUC__
#include <conio.h>
#endif

#ifdef __WIN32__
#include <windows.h>
#endif

#ifdef __GNUC__
#include <unistd.h>
#include <termios.h>
#include <sys/time.h>
#endif

#include "os_cpu.h"
#include "os_cfg.h"
#include "ucos_ii.h"
#include "pc.h"

//constants
# define MAP_SIZE 10
# define BUY_WOOD_DIGGER 10
# define BUY_SAND_DIGGER 20
# define BUY_COAL_DIGGER 30
# define BUY_IRON_DIGGER 40
# define BUY_GOLD_DIGGER 50
# define BUY_CONVEYOR 2
# define BUY_SEPERATOR 15
# define UPGRADE_DIGGERTIME 100

typedef enum LandInfo {
    EMPTY, // .
    WOOD_DIGGER,
    SAND_DIGGER,
    COAL_DIGGER,
    IRON_DIGGER,
    GOLD_DIGGER, //Digger END
    LEFT_CONVEYOR,
    RIGHT_CONVEYOR,
    UP_CONVEYOR,
    DOWN_CONVEYOR,
    SEPERATEOR,
    SHOP,
    ORIGIN_WOOD,
    ORIGIN_SAND,
    ORIGIN_MINE, //possible dig to coal, iron, gold
} land;
extern const char* landinfoString[15];

//자원이 몇개 있는지
typedef struct Resource {
    int have; //자원이 존재하는지
    int wood;
    int sand;
    int coal;
    int iron;
    int gold;
} Resource;

#endif