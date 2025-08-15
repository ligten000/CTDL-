#ifndef SVSYSTEM_H
#define SVSYSTEM_H
#include <iostream>
#include <fstream>
#include "CTDL.h"
#include "ChuanHoa.h"
#include "file_lop.h"
#include "mylib.h"
#include <cstring>
#include <iomanip>
#include <sstream>
using namespace std;

const int MAX_STACK = 1000;

struct StackNode {
    nodeCauhoi* data[MAX_STACK];
    int top = -1;

    bool isEmpty() {
        return top == -1;
    }

    bool isFull() {
        return top == MAX_STACK - 1;
    }

    void push(nodeCauhoi* p) {
        if (!isFull()) {
            data[++top] = p;
        }
    }

    nodeCauhoi* pop() {
        if (!isEmpty()) {
            return data[top--];
        }
        return nullptr;
    }

    nodeCauhoi* peek() {
        if (!isEmpty()) return data[top];
        return nullptr;
    }
};

int demCay(nodeCauhoi *root) {
    StackNode stack;
    nodeCauhoi* current = root;
    int count = 0;
    if(current == NULL) return count;
    while (current != nullptr || !stack.isEmpty()) {
        while (current != nullptr) {
            stack.push(current);
            current = current->left;
        }

        current = stack.pop();
        count++;
        current = current->right;
    }
    return count;
}

void xapXep(int X[],int n){
    int a;
    for (int i = 0; i < n - 1; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (X[i] > X[j]) {
                a = X[i];
                X[i] = X[j];
                X[j] = a;
            }
        }
    }
}

bool timSo(int X[], int n, int a){
    for(int i = 0; i< n; i++){
        if(X[i] == a) return true;
        if(X[i]>a) break;
    }
    return false;
}

nodeDiemThi* themNodeBaiThi(nodeDiemThi*& head, char* mmh){
    // Trường hợp danh sách rỗng
    if (head == NULL) {
        head = new nodeDiemThi();
        head->diem.baithi = new Baithi;
        strcpy(head->diem.MAMH, mmh);
        return head;
    }

    // Nếu cần chèn trước head
    if (strcmp(mmh, head->diem.MAMH) < 0) {
        nodeDiemThi* a = new nodeDiemThi();
        strcpy(a->diem.MAMH, mmh);
        a->diem.Diem = 0;
        a->diem.baithi = new Baithi;
        a->next = head;
        head = a;
        return a;
    }

    // Duyệt để tìm chỗ chèn
    nodeDiemThi* p;
    for(p = head; p != NULL && strcmp(p->diem.MAMH, mmh) <= 0; p = p->next){
        if(strcmp(p->diem.MAMH, mmh) == 0){
            return p;
        }
        if (p->next == NULL) {
            nodeDiemThi* a = new nodeDiemThi();
            strcpy(a->diem.MAMH, mmh);
            a->diem.Diem = 0;
            a->diem.baithi = new Baithi;
            a->next = NULL;
            p->next = a;
            return a;
        }

        if (strcmp(p->next->diem.MAMH, mmh) > 0) {
            nodeDiemThi* a = new nodeDiemThi();
            strcpy(a->diem.MAMH, mmh);
            a->diem.Diem = 0;
            a->diem.baithi = new Baithi;
            a->next = p->next;
            p->next = a;
            return a;
        }
    }
    return NULL;
}

void layDeThi(Baithi*&bt, nodeCauhoi*&tree, int n){
    int X[n];
    bt->dsBaithichitiet= new BaiThiChiTiets[n];
    bt->n = 0;
    int a = demCay(tree);
    srand(time(0)); 
    int wi = 0;
    while(wi < n){
        int r = rand() % a; 
        if(!timSo(X, wi, r)){
            X[wi++] = r;
        }
    }
    xapXep(X, n);

    StackNode stack;
    nodeCauhoi* current = tree;
    int count = 0;
    while (current != nullptr || !stack.isEmpty()) {
        while (current != nullptr) {
            stack.push(current);
            current = current->left;
        }

        current = stack.pop(); 
        if(timSo(X,n,count)){
            bt->dsBaithichitiet[bt->n].cauhoi = current;
            bt->dsBaithichitiet[bt->n].traloi = '\0';
            bt->n++;
            
            if(bt->n == n)break;
        }
        
        count++;
        current = current->right;
    }
}

void BatDauThi(nodeDiemThi* &bt, int minute){
    Normal(); system("cls");
    int index = 0;
    time_t startTime = time(0);
    const int duration = minute*60;
    bool ketThuc = false;
    char traloi;
    for (int i = 0; i < bt->diem.baithi->n; ++i) {
        bt->diem.baithi->dsBaithichitiet[i].traloi = '\0';
    }
    while (!ketThuc) {
        int elapsed = time(0) - startTime;
        int remaining = duration - elapsed;
        if (remaining <= 0) {
            ketThuc = true;
            break;
        }
        gotoxy(60, 0); cout << "Thoi gian con lai: " << remaining / 60 << "m " << remaining % 60 << "s   ";
        gotoxy(2, 1); cout << "Cau " << index + 1 << "/" << bt->diem.baithi->n << ":";
        gotoxy(2, 2); clearCurrentLine();
        gotoxy(2, 2);cout << bt->diem.baithi->dsBaithichitiet[index].cauhoi->data.NoiDung;

        gotoxy(4, 3); clearCurrentLine();
        gotoxy(4, 3);cout << "A. " << bt->diem.baithi->dsBaithichitiet[index].cauhoi->data.A;
        gotoxy(4, 4); clearCurrentLine();
        gotoxy(4, 4);cout << "B. " << bt->diem.baithi->dsBaithichitiet[index].cauhoi->data.B;
        gotoxy(4, 5); clearCurrentLine();
        gotoxy(4, 5);cout << "C. " << bt->diem.baithi->dsBaithichitiet[index].cauhoi->data.C;
        gotoxy(4, 6); clearCurrentLine();
        gotoxy(4, 6);cout << "D. " << bt->diem.baithi->dsBaithichitiet[index].cauhoi->data.D;
        gotoxy(4, 7); clearCurrentLine();
        gotoxy(4, 7);cout << "Cau tra loi: "<<bt->diem.baithi->dsBaithichitiet[index].traloi;
        if (_kbhit()) {
            char key = _getch();
            if (key == 0 || key == -32) {
                key = _getch(); // get extended key
                switch (key) {
                    case 75: // left
                        if (index > 0) index--;
                        break;
                    case 77: // right
                        if (index < bt->diem.baithi->n - 1) index++;
                        break;
                
                }
            } else {
                key = toupper(key);
                if (key >= 'A' && key <= 'D') {
                    bt->diem.baithi->dsBaithichitiet[index].traloi = key;

                } else if (key == 13) { // Enter
                    
                    if(index == bt->diem.baithi->n - 1){
                        ketThuc = true;
                    }
                    else index++;
                    break;
                }
            }
        }

        Sleep(100); // giảm CPU
    }
    system("cls");
    cout << "=== Ket thuc bai thi ===";
    int dapanDung = 0;
    float diem = 0;
    for (int i = 0; i < bt->diem.baithi->n; ++i) {
        bt->diem.baithi->dsBaithichitiet[i].cauhoi->data.trangthai = 1;
        if (bt->diem.baithi->dsBaithichitiet[i].traloi == bt->diem.baithi->dsBaithichitiet[i].cauhoi->data.DapAn)
            dapanDung++;
    }
    diem = float(dapanDung)/float(bt->diem.baithi->n);
    gotoxy(10,1); cout<<diem;
    gotoxy(7,2); cout<<dapanDung<<"/"<<bt->diem.baithi->n;
    bt->diem.Diem = diem;
};

// ham tim mon hoc, neu tim duoc -> tra ve vi tri, neu khong -> tra ve -1;
int timMonHoc(ListMonHoc &dsMH, char* mmh){
    for(int i = 0; i < dsMH.n && strcmp(dsMH.list[i].MAMH, mmh) <= 0; i++){
        if(strcmp(dsMH.list[i].MAMH, mmh) == 0) return i;
    }
    return -1;
}

void chuanBiThi(nodeSinhVien* &sv, DanhSachLop &dsLop, ListMonHoc &dsMH){
    Normal();clrscr();
    char mmh[16];
    int thoigianthi = 0;
    int socau = 0;
    int i;
    gotoxy(0,0);cout<<"Nhap ma mon:";
    gotoxy(0,1);cout<<"So cau hoi"; gotoxy(13,1); cout<<":";
    gotoxy(20,1); cout<<"Cau";
    gotoxy(0,2);cout<<"Thoi gian thi:"; gotoxy(20,2); cout<<"Phut";
    
    while(true){
        gotoxy(14, 0);
        NhapMa(mmh, 16);
        chuanHoaKhoangCach(mmh);
        if(strlen(mmh) == 0){
            thongBaoLoi("Ma mon hoc khong duoc rong", 14,0);
            continue;
        }
        i = timMonHoc(dsMH, mmh);
        if(i < 0){
            thongBaoLoi("Ma mon hoc khong ton tai", 14, 0);
            continue;
        }
        if(demCay(dsMH.list[i].treeCauHoi) == 0){
            thongBaoLoi("Mon hoc tam thoi chua co cau hoi", 14, 0);
            continue;
        }
        char CHsocau[4];
        char CHthoigianthi [4];
        while(socau <= 0){
            gotoxy(14, 1);
            NhapSo(CHsocau, 3);
            if(strlen(CHsocau) == 0) continue;
            socau = ChuoiThanhSo(CHsocau);
            if(socau <= 0){
                thongBaoLoi("So cau khong hop le", 14, 1);
                gotoxy(20,1); cout<<"Cau";
            }
            if(socau > demCay(dsMH.list[i].treeCauHoi)){
                thongBaoLoi("Khong co du cau hoi", 14, 1);
                socau = 0;
                gotoxy(20,1); cout<<"Cau";
            }
        }
        while(thoigianthi <= 0){
            gotoxy(14, 2);
            NhapSo(CHthoigianthi, 3);
            if(strlen(CHthoigianthi) == 0) continue;
            thoigianthi = ChuoiThanhSo(CHthoigianthi);
            if(thoigianthi <= 0){
                thongBaoLoi("Thoi gian khong hop le", 14, 2);
                gotoxy(20,2); cout<<"Phut";
            }
        }
        break;
    }
    nodeDiemThi* a = themNodeBaiThi(sv->sv.dsDiemThi, mmh);
    layDeThi(a->diem.baithi,dsMH.list[i].treeCauHoi,socau);
    BatDauThi(a, thoigianthi);
    
    char ch;
    while(true){
        ch = toupper(getch());
        if(ch == 13 || ch == 27){
            cout<<endl;
            GhiDanhSachLop(dsLop, "Lop.txt");
            clrscr();
            return;
        }
    }
}

#endif