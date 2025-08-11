#ifndef MENU_H
#define MENU_H
#include <cstring>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <fstream>
#include <windows.h>
#include "mylib.h"
using namespace std;



// Vẽ hình chữ nhật có viền mỏng và nội dung căn giữa
void veHinhChuNhat(int x, int y, int width, int height,const char* noiDung, int textColor , int bgColor , int borderColor) {
    if (width < 3 || height < 3) return;

    // Vẽ viền trên
    gotoxy(x, y);
    SetColor(borderColor);
    SetBGColor(0);
    cout << char(218);
    for (int i = 0; i < width - 2; ++i) cout << char(196);
    cout << char(191);

    // Thân giữa
    for (int i = 1; i < height - 1; ++i) {
        gotoxy(x, y + i);
        SetColor(borderColor);
        SetBGColor(0);
        cout << char(179);
        SetColor(textColor);
        SetBGColor(bgColor);
        for (int j = 0; j < width - 2; ++j) cout << " ";
        SetColor(borderColor);
        SetBGColor(0);
        cout << char(179);
    }

    // Viền dưới
    gotoxy(x, y + height - 1);
    cout << char(192);
    for (int i = 0; i < width - 2; ++i) cout << char(196);
    cout << char(217);

    // Viết chữ vào giữa
    int textX = x + (width - strlen(noiDung)) / 2;
    int textY = y + (height - 1) / 2;
    gotoxy(textX, textY);
    SetColor(textColor);
    SetBGColor(bgColor);
    cout << noiDung;

    Normal();
}

void RectangleNormal(const char* noidung, int x, int y){
    veHinhChuNhat(x, y, 30, 5, noidung, 15, 0, 15);
}

void RectangleHighLight(const char* noidung, int x, int y){
    veHinhChuNhat(x, y, 30, 5, noidung, 15, 14, 15);
}


#endif