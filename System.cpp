#include <iostream>
#include <windows.h>
#include "mylib.h"
#include "CTDL.h"
#include "quangRead.h"
#include "quangWrite.h"

using namespace std;

const int so_item = 8;
const int dong =10;
const int cot = 30 ;
const int Up = 72; // Extended code 
const int Down = 80;
const int item_menuSinhvien = 4;
char thucdon [so_item][50] = {"1. Nhap danh sach hoc vien",
			                  "2. Liet ke danh sach      ",
			                  "3. Tim kiem hoc vien      ",
			                  "4. Xoa hoc vien theo ten  ",
			                  "5. Ghi danh sach          ",
			                  "6. Mo  danh sach          ",
			                  "7. Them sv co thu tu      ",
			                  "8. Ket thuc chuong trinh  "};

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

int xacminhTKSinhVien(char* user, char * pass, DanhSachSinhVien& dsSV){
    for(int i =0; i< dsSV.n; i++){
        if(strcmp(user,dsSV.ds[i].MASV)>0){
            return 0;
        }
        if(strcmp(user, dsSV.ds[i].MASV)==0){
            if(strcmp(pass, dsSV.ds[i].Password)==0) return 1;
            else return 0;
        }
    }
}

SinhVien timSinhVien(DanhSachSinhVien& dsSV, char* MSV){
    for(int i =0; i< dsSV.n; i++){
        if(strcmp(MSV,dsSV.ds[i].MASV)>0){
            SinhVien rong;
            return rong;
        }
        if(strcmp(MSV, dsSV.ds[i].MASV)==0){
            return dsSV.ds[i];
        }
    }
}

void login(DanhSachSinhVien& ds) {
    char user[32], pass[32];
    char ch;
    int x = 35, y = 8;
    SinhVien currentLogin;

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
                int chon = MenuDong(thucdon);
                break;
            } else {
                gotoxy(x, y + 7); cout << "Mat khau sai!";
                Sleep(1500);
                continue;
            }
        }

        // Đăng nhập sinh viên
        if (xacminhTKSinhVien(user, pass, ds) == 1) {
            currentLogin = timSinhVien(ds, user);
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
    DanhSachSinhVien dsSV;

    quangRead(dsLop, dsSV); 
    quangWrite(dsLop, dsSV);
    login(dsSV);
    return 0;
}
