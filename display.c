#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>

# define GRID_SIZE 8

void DrawGrid(int cursor_x, int cursor_y) {
    system("cls"); // 화면 지우기
    for (int y = 0; y < GRID_SIZE; y++) {
        for (int x = 0; x < GRID_SIZE; x++) {
            if (x == cursor_x && y == cursor_y)
                printf("@ ");
            else
                printf(". ");
        }
        printf("\n");
    }
    fflush(stdout);
}

void Display_select() {
    system("cls"); //화면 지우기
    printf("What you want to build?\n");
    printf("a : Digger\n");
    printf("b : Conveyor\n");
    printf("c : Seperator\n");
    printf("esc : back to main\n");
}