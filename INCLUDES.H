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
# define MAP_SIZE 8

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
} land;

//자원이 몇개 있는지
typedef struct {
    int have = 0; //자원이 존재하는지
    int wood = 0;
    int sand = 0;
    int coal = 0;
    int iron = 0;
    int gold = 0;
} Resource;

#endif