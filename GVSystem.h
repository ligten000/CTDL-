#ifndef GVSYSTEM_H
#define GVSYSTEM_H
#include <iostream>
#include <fstream>
#include "CTDL.h"
#include <cstring>
#include <iomanip>
#include <sstream>
using namespace std;

int demSinhVien (Lop*& lop){
    int dem = 0;
    for(nodeSinhVien * p = lop->listSV; p != NULL; p = p->next)dem++;
    return dem;
}

void NhapMa(char ma[], int n) {
    int i = 0;
    char c;
    while (true) {
        c = getch();

        // Enter
        if (c == 13) break;

        // Backspace
        if (c == 8) {
            if (i > 0) {
                i--;
                cout << "\b \b";
            }
        }
        // Ký tự hợp lệ
        else {
            c = toupper(c);
            if ((c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '-' || c == '_' || c== ' ') {
                if (i < n) {
                    ma[i++] = c;
                    cout << c;
                }
            }
        }
    }

    ma[i] = '\0'; 
    cout << endl;
}

void themLop(DanhSachLop &dsLop, Lop *& lop, char malop[], char tenlop[]){
    if (dsLop.n >= Max_lop) {
        cout << "\nKhong the them lop moi. Danh sach lop da day!\n";
        return;
    }

    cout << "\nNhap ma lop: ";
    NhapMa(malop, 15);

    // Kiểm tra trùng mã lớp
    int i = 0;
    for (; i < dsLop.n; ++i) {
        int cmp = strcmp(dsLop.lop[i]->MALOP, malop);
        if (cmp == 0){
            cout<< "Ma lop da ton tai";
            Sleep(1000);
            return;  
        } 
        if (cmp > 0) break;
    }

    cout << "Nhap ten lop: ";
    NhapMa(tenlop, 50);

    // Xác nhận thêm
    cout << "Ban co chac chan muon them lop moi? (y/n): ";
    char confirm = toupper(getch());
    switch(confirm){
        case 'N':
            cout << "\nHuy them lop.\n";
            return;
        case 'Y':
            strcpy(lop->MALOP, malop);
            strcpy(lop->TENLOP, tenlop);
            for(int j = dsLop.n; j>i; j--){
                dsLop.lop[j] = dsLop.lop[j-1];
            }
            dsLop.lop[i] = lop;
            dsLop.n++;
            cout<<"them thanh cong"<<endl;

            Sleep(1000);
            return ;
        default:
            break;
            
    }  
}

bool hienthidanhsachLop(DanhSachLop &dsLop) {
    char a;
    while (true) {
        system("cls");
        cout << setw(10) << left << "STT"
             << setw(20) << left << "Ma Lop"
             << setw(30) << left << "Ten Lop"
             << setw(20) << left << "So luong SV" << endl;
        cout << string(80, '-') << endl;
        for (int i = 0; i < dsLop.n; i++) {
            cout << setw(10) << left << i + 1
                 << setw(20) << left << dsLop.lop[i]->MALOP
                 << setw(30) << left << dsLop.lop[i]->TENLOP
                 << setw(20) << left << demSinhVien(dsLop.lop[i])
                 << endl;
        }

        gotoxy(0, 12);
        cout << string(80, '-') << endl;
        cout << "A: Them | D: Xoa | E: Sua | S: Chon | Q: Thoat" << endl;

        a = toupper(getch());
        switch (a) {
            case 'A':   // Thêm lớp
                {Lop* lop = new Lop;
                char malop[15],tenlop[50];
                themLop(dsLop, lop, malop, tenlop);
                break;
                }
            case 'D':   // Xóa lớp
                // xoaLop(dsLop);
                break;
            case 'E':   // Sửa lớp
                break;
            case 'S':   // Chọn lớp
                break;
            case 'Q':   // Thoát khỏi danh sách
                return true;
            default:
                break;
        }
        // Sau mỗi thao tác -> quay lại đầu vòng lặp và in lại danh sách
    }

    return false;
}

#endif