#include <iostream>
#include <windows.h>
#include <ctime>
#include "mylib.h"
#include "CTDL.h"
#include "file_lop.h"
#include "file_cauhoi.h"
#include "GVSystem.h"
#include "SVSystem.h"
#include "GiaoDienMonHoc.h"
#include "Menu.h"
using namespace std;

const int dong = 5;
const int cot = 40 ;
const int Up = 72; // Extended code 
const int Down = 80;

char* menu[] = {"Quan ly danh sach lop",
                "Quan ly danh sach mon hoc",
                "Thoat chuong trinh"};

char * menuSV[] = { "Bat dau thi",
                    "Doi mat khau",
                    "Thoat chuong trinh"};                      


int UIMenu(char * menu[], int soItem ){
    Normal(); system("cls");
    int chon = 0;
    int i;
    for(i = 0; i<soItem; i++){
        RectangleNormal(menu[i],cot, i*5 + dong);
    }
    RectangleHighLight(menu[chon],cot,chon*5 + dong);
    char ch;
    do {
        ch = getch();
        if (ch==0) ch = getch();
        switch (ch) {
            case Up :if (chon > 0)
                    {
                        RectangleNormal(menu[chon], cot, chon*5+dong);
                        chon --;
                        RectangleHighLight(menu[chon], cot, chon*5+dong);
                    }
                    break;
            case Down :if (chon < soItem - 1)
                    {
                        RectangleNormal(menu[chon], cot, chon*5+dong);
                        chon ++;
                        RectangleHighLight(menu[chon], cot, chon*5+dong); 
                    }
                    break;
            case 13 : return chon+1;
        }  // end switch
    } while (1);
}



int xacminhTKSinhVien(char* user, char * pass,DanhSachLop&dsLop){
    
    if(dsLop.n == 0) return 0;
    for(int i = 0; i<dsLop.n;i++){
        for(nodeSinhVien* p = dsLop.lop[i]->listSV; p != NULL && strcmp(p->sv.MASV, user)<= 0; p = p->next){
            if(strcmp(p->sv.MASV, user) == 0){
                if(strcmp(p->sv.Password, pass) == 0) return 1;
                else return 0;
            }
        }
    }
    return 0;
}

nodeSinhVien* timSinhVien(DanhSachLop&dsLop, char* MSV){
    if(dsLop.n == 0) return NULL;
    for(int i = 0; i<dsLop.n;i++){
        for(nodeSinhVien* p = dsLop.lop[i]->listSV; p != NULL && strcmp(p->sv.MASV, MSV)<= 0; p = p->next){
            if(strcmp(p->sv.MASV, MSV) == 0){
                return p;
            }
        }
    }
    return NULL;
}

bool SystemGV(DanhSachLop &dsLop, ListMonHoc &dsMH, int chon, int item){
    while (true) {
        switch (chon){
            case 1:
                if (hienthidanhsachLop(dsLop, dsMH)) {
                    chon = UIMenu(menu, item);  // Gọi lại menu nếu người dùng thoát
                    continue;
                }
                break;
            case 2:
                if(hienthidanhsachMonHoc(dsMH, dsLop)){
                    chon = UIMenu(menu, item);  // Gọi lại menu nếu người dùng thoát
                    continue;
                }
                break;
            case 3:
                return true;  // Thoát luôn SystemGV (nếu muốn vậy)
            default:
                chon = UIMenu(menu, item);// gọi lại menu khi lựa chọn không hợp lệ
                break;
        }
    }
    return false;
}

bool SystemSV(nodeSinhVien* &sv, DanhSachLop& dsLop,ListMonHoc &dsMH, int chon, int item){
    while (true) {
        switch (chon){
            case 1:
                chuanBiThi(sv, dsLop, dsMH);
                chon = UIMenu(menuSV, item);
                break;
            case 2:
                DoiMatKhau(dsLop, sv);
                chon = UIMenu(menuSV, item);
                break;
            case 3:
                return true;  // Thoát luôn SystemGV (nếu muốn vậy)
            default:
                chon = UIMenu(menu, item);
                break;
        }
    }
    return false;
}

void login(DanhSachLop& dsLop, ListMonHoc& dsMH) {
    char user[32], pass[32];
    char ch;
    int x = 35, y = 8;
    nodeSinhVien* currentLogin = NULL;

    while (true) {
        memset(user, 0, sizeof(user));
        memset(pass, 0, sizeof(pass));
        Normal();
        system("cls");

        // Giao diện
        gotoxy(x, y);     cout << char(218)<<string(27,char(196))<<char(191);
        gotoxy(x, y + 1); cout << char(179)<<"    DANG NHAP HE THONG     "<<char(179);
        gotoxy(x, y + 2); cout << char(195)<<string(27,char(196))<<char(180);
        gotoxy(x, y + 3); cout << char(179);gotoxy(x + 28, y + 3);cout<<char(179);
        gotoxy(x, y + 4); cout << char(179)<<"User     : ";gotoxy(x + 28, y + 4);cout<<char(179);
        gotoxy(x, y + 5); cout << char(179)<<"Password : ";gotoxy(x + 28, y + 5);cout<<char(179);
        gotoxy(x, y + 6); cout << char(192)<<string(27,char(196))<<char(217);

        // Nhập user
        gotoxy(x + 12, y + 4);
        int len_user = 0;
        while (true) {
            ch = getch();
            if (ch == 13) break;
            if (ch == 8 && len_user > 0) {
                len_user--;
                user[len_user] = '\0';
                cout << "\b \b";
            } else if (isprint(ch) && len_user < 31) {
                user[len_user++] = ch;
                cout << ch;
            }
        }
        user[len_user] = '\0';

        // Nhập pass
        gotoxy(x + 12, y + 5);
        cout << string(15, ' ');
        gotoxy(x + 12, y + 5);
        int len_pass = 0;
        while (true) {
            ch = getch();
            if (ch == 13) break;
            if (ch == 8 && len_pass > 0) {
                len_pass--;
                pass[len_pass] = '\0';
                cout << "\b \b";
            } else if (isprint(ch) && len_pass < 31) {
                pass[len_pass++] = ch;
                cout << '*';
            }
        }
        pass[len_pass] = '\0';

        // Đăng nhập GV
        if (strcmp(user, "GV") == 0) {
            if (strcmp(pass, "GV") == 0) {
                gotoxy(x, y + 7); cout << "Dang nhap thanh cong!";
                Sleep(1000);
                while (true) {
                    int item = sizeof(menu)/sizeof(menu[0]);
                    int chon = UIMenu(menu,item);
                    // int chon = MenuDong(thucdon);
                    bool quayLaiLogin = SystemGV(dsLop, dsMH, chon, item);
                    if (quayLaiLogin) break; // quay lại giao diện đăng nhập
                }
                continue; // tiếp tục vòng login từ đầu
            }
            else {
                gotoxy(x, y + 7); cout << "Mat khau sai!";
                Sleep(1500);
                continue;
            }
        }

        // Đăng nhập sinh viên
        if (xacminhTKSinhVien(user, pass, dsLop) == 1) {
            currentLogin = timSinhVien(dsLop, user);
            gotoxy(x, y + 7); cout << "Dang nhap thanh cong!";
            Sleep(1000);
            int item = sizeof(menuSV)/sizeof(menuSV[0]);
            int chon = UIMenu(menuSV, item);
            bool quayLaiLogin = SystemSV(currentLogin, dsLop,dsMH, chon, item);
            if (quayLaiLogin) continue;
        }

        // Sai tài khoản
        gotoxy(x, y + 7); 
        cout << "Ten dang nhap hoac mat khau khong hop le!";
        Sleep(1500);
    }
}

int main(int argc, char const *argv[])
{
    // Khởi tạo seed cho hàm rand() để sinh ID ngẫu nhiên
    srand(time(0));
    
    DanhSachLop dsLop;
    ListMonHoc dsMonHoc;

    DocFile(dsMonHoc, "MonHoc.txt");
    DocDanhSachLop(dsLop, dsMonHoc, "Lop.txt"); 

    login(dsLop, dsMonHoc);
    return 0;
}
