#ifndef GIAODIENLOP_H
#define GIAODIENLOP_H
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

const int Page_Size = 15;
const int mouse = 20;

void IntieuDeDSLop(){
    gotoxy(0,0);
        cout << char(218) << string(10, char(196)) << char(194)
            << string(20, char(196)) << char(194)
            << string(30, char(196)) << char(194)
            << string(20, char(196)) << char(191) << endl;

    // Vẽ dòng tiêu đề có khung dọc
    cout << char(179) << setw(10) << left << "STT"
            << char(179) << setw(20) << left << "Ma Lop"
            << char(179) << setw(30) << left << "Ten Lop"
            << char(179) << setw(20) << left << "So luong SV"
            << char(179) << endl;

    // Vẽ đường phân cách tiêu đề và dữ liệu
    cout << char(195) << string(10, char(196)) << char(197)
            << string(20, char(196)) << char(197)
            << string(30, char(196)) << char(197)
            << string(20, char(196)) << char(180) << endl;


        gotoxy(0, 18);
        cout << "A: Them | D: Xoa | E: Sua | C: Chon | S: luu | Q: Thoat" << endl;
}

void HienThiDanhSachSinhVien(DanhSachLop &dsLop, nodeSinhVien* &head, int &slSV){
	Normal();system("cls");
    char a;
    int currentPage = 1;
    int start;
    int end;
	int count;
    int numPage = (slSV + Page_Size - 1) / Page_Size;

	// Vẽ đường viền trên của bảng
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
			<< char(179) << setw(10) << left << "Gioi tinh"
            << char(179) << endl;

    // Vẽ đường phân cách tiêu đề và dữ liệu
    cout << char(195) << string(10, char(196)) << char(197)
            << string(20, char(196)) << char(197)
            << string(40, char(196)) << char(197)
			<< string(20, char(196)) << char(197)
            << string(10, char(196)) << char(180) << endl;
	while(true){
		start = (currentPage - 1) * Page_Size;
        end = min(start + Page_Size, slSV);

		for (int i = 0; i < Page_Size; i++) {
            gotoxy(0, 3 + i);
            clearCurrentLine(); 
        }

		gotoxy(0, 3);
		int index = 0;
        nodeSinhVien* p = head;
        while (p != NULL && index < end) {
            if (index >= start) {
                cout << char(179) << setw(10) << left << index + 1 
                    << char(179) << setw(20) << left << p->sv.MASV
                    << char(179) << setw(40) << left << p->sv.HO
                    << char(179) << setw(20) << left << p->sv.TEN
                    << char(179) << setw(10) << left << p->sv.PHAI
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
		gotoxy(49,18);
        cout<<"("<<currentPage<<"/"<<numPage<<")"<<endl;
        gotoxy(0, 19);
        cout << "A: Them | D: Xoa | E: Sua | C: Chon | S: luu | Q: Thoat" << endl;
        for(int i = mouse; i< mouse + 10; i++){
            gotoxy(0,i);
            clearCurrentLine();
        }
        a = toupper(getch());
        if (a == -32 || a == 0) { // phím đặc biệt
            a = getch();
            if (a == 75 && currentPage > 1) currentPage--; // mũi tên trái
            else if (a == 77 && currentPage < numPage) currentPage++; // mũi tên phải
        }
		else{
            switch (a) {
                case 'A':   // Thêm lớp
                    break;

                case 'D':   // Xóa lớp
                    break;
                case 'E':   // Sửa lớp
                    break;
                case 'C':   // Chọn lớp
                    break;
                case 'S':
                    GhiDanhSachLop(dsLop, "Lop.txt");
                    cout<<"Luu thanh cong";
                    Sleep(1000);
                    clearCurrentLine();
                    break;
                case 'Q':   // Thoát khỏi danh sách
                    system("cls");
                    IntieuDeDSLop();
                    return ;
                default:
                    break;
            }
        }
	}
}
#endif