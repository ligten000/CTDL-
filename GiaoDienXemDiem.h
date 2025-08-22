#ifndef GIAODIENXEMDIEM_H
#define GIAODIENXEMDIEM_H
#include <iostream>
#include <fstream>
#include "CTDL.h"
#include "ChuanHoa.h"
// #include "GiaoDienLop.h"
#include "mylib.h"
#include <cstring>
#include <iomanip>
#include <sstream>
using namespace std;

const int Page_Size = 15;
const int mouse = 21;

int demSinhVien (nodeSinhVien*& head){
    int dem = 0;
    for(nodeSinhVien * p = head; p != NULL; p = p->next)dem++;
    return dem;
}


int timMonHoc(ListMonHoc &dsMH, char* mmh){
    for(int i = 0; i < dsMH.n && strcmp(dsMH.list[i].MAMH, mmh) <= 0; i++){
        if(strcmp(dsMH.list[i].MAMH, mmh) == 0) return i;
    }
    return -1;
}

nodeDiemThi* timDiemThi(nodeDiemThi*& head, char* MMH) {
    for (nodeDiemThi* p = head; p != NULL && strcmp(p->diem.MAMH, MMH) <= 0; p = p->next) {
        if (strcmp(p->diem.MAMH, MMH) == 0)
            return p;
    }
    return NULL;
}

void ChiTietBaiThi(nodeSinhVien*&SV, nodeDiemThi* diem){
    Normal();system("cls");
    char a;
    int currentQ = 0;
    gotoxy(0,0);
    cout<<"Ma sinh vien : "<< SV->sv.MASV;
    gotoxy(25,0);
    cout<<"Ten sinh vien: "<< SV->sv.HO<<" "<< SV->sv.TEN;

    gotoxy(0,1);
    cout<<"Ma Mon: "<<diem->diem.MAMH;
    gotoxy(0,2);
    cout<<"Diem: "<<formatDiem(diem->diem.Diem);
    while(true){
        for(int i = 3; i<15; i++){
            gotoxy(0,i);
            clearCurrentLine();
        }
        gotoxy(0,3);
        cout<<"Cau: "<<currentQ + 1<<"/"<<diem->diem.baithi->n;
        gotoxy(0,4);
        cout<<diem->diem.baithi->dsBaithichitiet[currentQ].cauhoi->data.NoiDung<<endl;
        cout<<"A. "<<diem->diem.baithi->dsBaithichitiet[currentQ].cauhoi->data.A<<endl;
        cout<<"B. "<<diem->diem.baithi->dsBaithichitiet[currentQ].cauhoi->data.B<<endl;
        cout<<"C. "<<diem->diem.baithi->dsBaithichitiet[currentQ].cauhoi->data.C<<endl;
        cout<<"D. "<<diem->diem.baithi->dsBaithichitiet[currentQ].cauhoi->data.D<<endl;
        cout<<"Dap an : "<<diem->diem.baithi->dsBaithichitiet[currentQ].traloi<<endl;
        cout<<"Tra loi: "<<diem->diem.baithi->dsBaithichitiet[currentQ].cauhoi->data.DapAn<<endl;
         a = toupper(getch());
        if (a == -32 || a == 0) { // phím đặc biệt
            a = getch();
            if (a == 75 && currentQ > 0) currentQ --; // mũi tên trái
            else if (a == 77 && currentQ < diem->diem.baithi->n - 1) currentQ++; // mũi tên phải
        }
		else{
            if(a == 27)return;
        }
    }
}

void hienThiDanhSachDiem(Lop*& lop, MonHoc& mon){    
    char a;
    char MSV[10];
    nodeSinhVien* head = lop->listSV;
    int slSV = demSinhVien(head);
    int currentPage = 1;
    int start;
    int end;
	int count;
    int numPage = (slSV + Page_Size - 1) / Page_Size;
    while(true){
        start = (currentPage - 1) * Page_Size;
        end = min(start + Page_Size, slSV);

		Normal();system("cls");
        gotoxy(0,0);
        cout<<"Ma lop: "<<setw(30)<<lop->MALOP<<"Ten lop"<<lop->TENLOP<<endl;
        cout<<"Ma mon hoc: "<<setw(30)<<mon.MAMH<<"Ten Mon hoc"<<mon.TENMH<<endl;
        // Vẽ đường viền trên của bảng
        gotoxy(0,2);
        cout 	<< char(218) << string(10, char(196)) << char(194)
                << string(20, char(196)) << char(194)
                << string(40, char(196)) << char(194)
                << string(20, char(196)) << char(194)
                << string(10, char(196)) << char(191) << endl;

        // Vẽ dòng tiêu đề có khung dọc
        cout 	<< char(179) << setw(10) << left << "STT"
                << char(179) << setw(20) << left << "MSV"
                << char(179) << setw(40) << left << "Ho"
                << char(179) << setw(20) << left << "Ten"
                << char(179) << setw(10) << left << "Diem"
                << char(179) << endl;

        // Vẽ đường phân cách tiêu đề và dữ liệu
        cout << char(195) << string(10, char(196)) << char(197)
                << string(20, char(196)) << char(197)
                << string(40, char(196)) << char(197)
                << string(20, char(196)) << char(197)
                << string(10, char(196)) << char(180) << endl;

		gotoxy(0, 5);
		int index = 0;
        nodeSinhVien* p = head;
        nodeDiemThi* diem;
        while (p != NULL && index < end) {
            if (index >= start) {
                diem = timDiemThi(p->sv.dsDiemThi, mon.MAMH);
                cout << char(179) << setw(10) << left << index + 1 
                    << char(179) << setw(20) << left << p->sv.MASV
                    << char(179) << setw(40) << left << p->sv.HO
                    << char(179) << setw(20) << left << p->sv.TEN
                    << char(179) << setw(10) << left << ((diem == NULL) ? "Chua thi" : formatDiem(diem->diem.Diem))
                    << char(179) << endl;
            }
            p = p->next;
            index++;
        }
        for(int i = end+1; i<= start + Page_Size; i++){
			cout 	<< char(179) << setw(10) << left << i
					<< char(179) << setw(20) << left << ""
					<< char(179) << setw(40) << left << ""
					<< char(179) << setw(20) << left << ""
					<< char(179) << setw(10) << left << ""
					<< char(179) << endl;
        }
		// Vẽ đường phân cách tiêu đề và dữ liệu
		cout << char(192) << string(10, char(196)) << char(193)
				<< string(20, char(196)) << char(193)
				<< string(40, char(196)) << char(193)
				<< string(20, char(196)) << char(193)
				<< string(10, char(196)) << char(217) << endl;
		gotoxy(49,20);
        cout<<"("<<currentPage<<"/"<<numPage<<")"<<endl;
        gotoxy(0, 21);
        cout<<"Nhan ENTER de nhap ma sinh vien, ESC de thoat";
        a = toupper(getch());
        if (a == -32 || a == 0) { // phím đặc biệt
            a = getch();
            if (a == 75 && currentPage > 1) currentPage--; // mũi tên trái
            else if (a == 77 && currentPage < numPage) currentPage++; // mũi tên phải
        }
		else{
            int sttMon;
            if(a == 13){
                gotoxy(0,22);
                cout<<"Ma Sinh Vien :";
                while(true){
                    gotoxy(15, 22);
                    NhapMa(MSV, 10);
                    if(strlen(MSV) == 0){
                        thongBaoLoi("Ma sinh vien khong duoc trong!", 15, 22);
                        continue;
                    }
                    for( p = head; p != NULL && strcmp(MSV, p->sv.MASV) >=0; p = p->next){
                        if(strcmp(MSV, p->sv.MASV) ==0) break;
                    }
                    if(p == NULL || strcmp(MSV, p->sv.MASV) != 0){
                        thongBaoLoi("Khong tim thay sinh vien trong lop nay", 15, 22);
                        continue;
                    }
                    nodeDiemThi * bt = timDiemThi(p->sv.dsDiemThi, mon.MAMH);
                    if(bt != NULL)
                        ChiTietBaiThi(p, bt);
                    else thongBaoLoi("Sinh vien chua thi mon nay!",15, 22);
                    break;

                }
            }

            if(a == 27) return;
        };
    }
}

void xemDiem(Lop*&lop, ListMonHoc& dsMH){
    
    char a;
    int currentPage = 1;
    int start;
    int end;
    char MAMH[16];
    int numPage = (dsMH.n + Page_Size - 1) / Page_Size;
    while(true){
        Normal();system("cls");
        start = (currentPage - 1) * Page_Size;
        end = min(start + Page_Size, dsMH.n);
        gotoxy(0,0);
        cout<<"Danh sach mon hoc";
        gotoxy(0,1);
        cout << char(218) << string(10, char(196)) << char(194)
            << string(20, char(196)) << char(194)
            << string(30, char(196)) << char(194) << endl;

        cout << char(179) << setw(10) << left << "STT"
            << char(179) << setw(20) << left << "Ma mon hoc"
            << char(179) << setw(30) << left << "Ten mon hoc"
            << char(179) << endl;
        cout << char(195) << string(10, char(196)) << char(197)
            << string(20, char(196)) << char(197)
            << string(30, char(196)) << char(180) << endl;
        gotoxy(0,4);

        for(int i = start; i < end; i++){
            cout << char(179) << setw(10) << left << i + 1
                 << char(179) << setw(20) << left << dsMH.list[i].MAMH
                 << char(179) << setw(30) << left << dsMH.list[i].TENMH
                 << char(179) << endl;
        }

        for(int i = end+1; i<= start + Page_Size; i++){
            cout << char(179) << setw(10) << left << i
                 << char(179) << setw(20) << left << ""
                 << char(179) << setw(30) << left << ""
                 << char(179) << endl;
        }

        cout << char(192) << string(10, char(196)) << char(193)
             << string(20, char(196)) << char(193)
             << string(30, char(196)) << char(193)
             << char(217) << endl;
        gotoxy(26,19);
        cout<<"("<<currentPage<<"/"<<numPage<<")"<<endl;
        gotoxy(0,20);
        cout<<"Nhap phim ENTER de bat dau nhap ma mon, ESC de thoat";

        a = toupper(getch());
        if (a == -32 || a == 0) { // phím đặc biệt
            a = getch();
            if (a == 75 && currentPage > 1) currentPage--; // mũi tên trái
            else if (a == 77 && currentPage < numPage) currentPage++; // mũi tên phải
        }
		else{
            int sttMon;
            if(a == 13){
                gotoxy(0, mouse);
                cout<<"Ma mon hoc :";
                while(true){
                    gotoxy(12, mouse);
                    NhapMa(MAMH, 16);
                    if(strlen(MAMH) == 0){
                        thongBaoLoi("Ma mon hoc khong duoc trong!", 12, mouse);
                        continue;
                    }
                    sttMon = timMonHoc(dsMH, MAMH) ;
                    if(sttMon < 0){
                        thongBaoLoi("Ma mon hoc khong ton tai!", 12, mouse);
                        continue;
                    }
                    break;
                }
                hienThiDanhSachDiem(lop, dsMH.list[sttMon]);
            }

            if(a == 27) return;
        };
    }
}
#endif