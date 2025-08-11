#include <iostream>
#include <windows.h>
#include "mylib.h"
#include "CTDL.h"
#include "file_cauhoi.h"
using namespace std;

const int MAX_STACK = 100;

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
    int cout = 0;
    while (current != nullptr || !stack.isEmpty()) {
        while (current != nullptr) {
            stack.push(current);
            current = current->left;
        }

        current = stack.pop();
        cout++;
        current = current->right;
    }
    return cout;
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

void layDeThi(Baithi& bt, nodeCauhoi*&tree, int n){
    int X[n];
    bt.dsBaithichitiet = new BaiThiChiTiets[n];
    bt.n = 0;
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
    int cout = 0;
    while (current != nullptr || !stack.isEmpty()) {
        while (current != nullptr) {
            stack.push(current);
            current = current->left;
        }

        current = stack.pop(); 
        if(timSo(X,n,cout)){
            bt.dsBaithichitiet[bt.n].cauhoi = current;
            bt.dsBaithichitiet[bt.n].traloi = '\0';
            bt.n++;
            
            if(bt.n == n)break;
        }
        
        cout++;
        current = current->right;
    }
}

void BatDauThi(Baithi &bt, int minute){
    Normal(); system("cls");
    int index = 0;
    time_t startTime = time(0);
    const int duration = minute*60;
    bool ketThuc = false;
    char traloi;
    for (int i = 0; i < bt.n; ++i) {
        bt.dsBaithichitiet[i].traloi = '\0';
    }
    while (!ketThuc) {
        int elapsed = time(0) - startTime;
        int remaining = duration - elapsed;
        if (remaining <= 0) {
            ketThuc = true;
            break;
        }
        gotoxy(60, 0); cout << "Thoi gian con lai: " << remaining / 60 << "m " << remaining % 60 << "s   ";
        gotoxy(2, 1); cout << "Cau " << index + 1 << "/" << bt.n << ":";
        gotoxy(2, 2); clearCurrentLine();
        gotoxy(2, 2);cout << bt.dsBaithichitiet[index].cauhoi->data.NoiDung;

        gotoxy(4, 3); clearCurrentLine();
        gotoxy(4, 3);cout << "A. " << bt.dsBaithichitiet[index].cauhoi->data.A;
        gotoxy(4, 4); clearCurrentLine();
        gotoxy(4, 4);cout << "B. " << bt.dsBaithichitiet[index].cauhoi->data.B;
        gotoxy(4, 5); clearCurrentLine();
        gotoxy(4, 5);cout << "C. " << bt.dsBaithichitiet[index].cauhoi->data.C;
        gotoxy(4, 6); clearCurrentLine();
        gotoxy(4, 6);cout << "D. " << bt.dsBaithichitiet[index].cauhoi->data.D;
        gotoxy(4, 7); clearCurrentLine();
        gotoxy(4, 7);cout << "Cau tra loi: "<<bt.dsBaithichitiet[index].traloi;
        if (_kbhit()) {
            char key = _getch();
            if (key == 0 || key == -32) {
                key = _getch(); // get extended key
                switch (key) {
                    case 75: // left
                        if (index > 0) index--;
                        break;
                    case 77: // right
                        if (index < bt.n - 1) index++;
                        break;
                
                }
            } else {
                key = toupper(key);
                if (key >= 'A' && key <= 'D') {
                    bt.dsBaithichitiet[index].traloi = key;

                } else if (key == 13) { // Enter
                    
                    if(index == bt.n - 1){
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
    cout << "=== Ket thuc bai thi ===\n";
    int dapanDung = 0;
    for (int i = 0; i < bt.n; ++i) {
        bt.dsBaithichitiet[i].cauhoi->data.trangthai = 1;
        if (bt.dsBaithichitiet[i].traloi == bt.dsBaithichitiet[i].cauhoi->data.DapAn)
            dapanDung++;
    }
    cout<<dapanDung<<"/"<<bt.n;
};

int main(int argc, char const *argv[]){
    ListMonHoc ds;
    Baithi bt;
    DocFileDSMonHoc(ds,"MonHoc.txt");
    layDeThi(bt,ds.list[0].treeCauHoi,3);
    BatDauThi(bt, 3);
    return 0;
}
