#include <iostream>
#include <windows.h>
#include "mylib.h"
#include "CTDL.h"
#include "quangRead.h"
#include "quangWrite.h"
#include "GVSystem.h"
using namespace std;

const int so_item = 4;
const int dong =10;
const int cot = 30 ;
const int Up = 72; // Extended code 
const int Down = 80;
const int item_menuSinhvien = 4;
char thucdon [so_item][50] = {"1. Quan ly danh sach lop  ",
			                  "2. Quan ly sinh vien      ",
			                  "3. Quan ly mon hoc        ",
			                  "4. Ket thuc chuong trinh  "};

char menuSinhvien [item_menuSinhvien][50] = {"1. Bat dau thi            ",
                                             "2. Xem Diem               ",
                                             "3. Doi mat khau           ",   
                                             "4. Ket thuc chuong trinh  "};                          

void Normal () {
	SetColor(15);
	SetBGColor(0);
}
void HighLight () {
	SetColor(15);
	SetBGColor(1);
}
int MenuDong(char td [so_item][50]){

  Normal(); system("cls");
  int chon =0;
  int i; 
  for ( i=0; i< so_item ; i++)
  { gotoxy(cot, dong +i);
    cout << td[i];
  }
  HighLight();
  gotoxy(cot,dong+chon);
  cout << td[chon];
  char kytu;
do {
  kytu = getch();
  if (kytu==0) kytu = getch();
  switch (kytu) {
    case Up :if (chon+1 >1)
  			  {
  		        Normal();
              	gotoxy(cot,dong+chon);
              	cout << td[chon];
              	chon --;
              	HighLight();
              	gotoxy(cot,dong+chon);
              	cout << td[chon];
  				
  			  }
  			  break;
  	case Down :if (chon+1 <so_item)
  			  {
  		        Normal();
              	gotoxy(cot,dong+chon);
              	cout << td[chon];
              	chon ++;
              	HighLight();
              	gotoxy(cot,dong+chon);
              	cout << td[chon];
  				
  			  }
  			  break;
  	case 13 : return chon+1;
  }  // end switch
  } while (1);
};

int inMenuSinhVien(char td[item_menuSinhvien][50]){
    Normal(); system("cls");
    int chon =0;
    int i; 
    for ( i=0; i< item_menuSinhvien ; i++)
    { gotoxy(cot, dong +i);
        cout << td[i];
    }
    HighLight();
    gotoxy(cot,dong+chon);
    cout << td[chon];
    char kytu;
    do {
    kytu = getch();
    if (kytu==0) kytu = getch();
    switch (kytu) {
        case Up :if (chon >0)
                {
                    Normal();
                    gotoxy(cot,dong+chon);
                    cout << td[chon];
                    chon --;
                    HighLight();
                    gotoxy(cot,dong+chon);
                    cout << td[chon];
                    
                }
                break;
        case Down :if (chon <item_menuSinhvien - 1)
                {
                    Normal();
                    gotoxy(cot,dong+chon);
                    cout << td[chon];
                    chon ++;
                    HighLight();
                    gotoxy(cot,dong+chon);
                    cout << td[chon];
                    
                }
                break;
        case 13 : return chon+1;
    }  // end switch
    } while (1);
}

int xacminhTKSinhVien(char* user, char * pass,nodeSinhVien* dsSV){
    if(dsSV == NULL) return 0;
    for(nodeSinhVien* p = dsSV; p->next != NULL && strcmp(p->sv->MASV, user)<= 0; p = p->next){
        if(strcmp(p->sv->MASV, user) == 0){
            if(strcmp(p->sv->Password, pass) == 0) return 1;
            else return 0;
        }
    }
    return 0;
}

SinhVien* timSinhVien(nodeSinhVien* &dsSV, char* MSV){
    if(dsSV == NULL) return NULL;
    for(nodeSinhVien* p; p->next != NULL && strcmp(p->sv->MASV, MSV)>0; p = p->next){
        if(strcmp(p->sv->MASV, MSV) == 0) return p->sv;
    }
    return NULL;
}

bool SystemGV(DanhSachLop &dsLop, nodeSinhVien* &dsSV, int chon){
    while (true) {
        switch (chon){
            case 1:
                if (hienthidanhsachLop(dsLop)) {
                    chon = MenuDong(thucdon);  // Gọi lại menu nếu người dùng thoát
                    continue;
                }
                break;
            case 2:
                // các chức năng khác
                break;
            case 3:
                break;
            case 4:
                return true;  // Thoát luôn SystemGV (nếu muốn vậy)
            default:
                chon = MenuDong(thucdon); // gọi lại menu khi lựa chọn không hợp lệ
                break;
        }
    }
    return false;
}

void login(nodeSinhVien* dsSV, DanhSachLop dsLop) {
    char user[32], pass[32];
    char ch;
    int x = 35, y = 8;
    SinhVien* currentLogin;

    while (true) {
        memset(user, 0, sizeof(user));
        memset(pass, 0, sizeof(pass));
        system("cls");

        // Giao diện
        gotoxy(x, y);     cout << "============================";
        gotoxy(x, y + 1); cout << "|      DANG NHAP HE THONG     |";
        gotoxy(x, y + 2); cout << "============================";
        gotoxy(x, y + 4); cout << "User     : ";
        gotoxy(x, y + 5); cout << "Password : ";

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
        cout << string(30, ' ');
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
                    int chon = MenuDong(thucdon);
                    bool quayLaiLogin = SystemGV(dsLop, dsSV, chon);
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
        if (xacminhTKSinhVien(user, pass, dsSV) == 1) {
            currentLogin = timSinhVien(dsSV, user);
            gotoxy(x, y + 7); cout << "Dang nhap thanh cong!";
            Sleep(1000);
            int chon = inMenuSinhVien(menuSinhvien);
            break;
        }

        // Sai tài khoản
        gotoxy(x, y + 7); 
        cout << "Ten dang nhap hoac mat khau khong hop le!";
        Sleep(1500);
    }
}

int main(int argc, char const *argv[])
{
    DanhSachLop dsLop;
    nodeSinhVien *dsSV = NULL;

    quangRead(dsLop, dsSV, "SinhVien.txt","Lop.txt"); 
    quangWrite(dsLop, dsSV, "SinhVien.txt","Lop.txt");
    login(dsSV, dsLop);
    return 0;
}
