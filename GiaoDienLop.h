#ifndef GIAODIENLOP_H
#define GIAODIENLOP_H
#include <iostream>
#include <fstream>
#include "CTDL.h"
#include "ChuanHoa.h"
#include "file_lop.h"
#include "mylib.h"
#include "GiaoDienXemDiem.h"
#include <cstring>
#include <iomanip>
#include <sstream>
using namespace std;

int timSV(DanhSachLop& dsLop,char* MSV){
    if(dsLop.n == 0) return 0;
    for(int i = 0; i<dsLop.n;i++){
        for(nodeSinhVien* p = dsLop.lop[i]->listSV; p != NULL && strcmp(p->sv.MASV, MSV)<= 0; p = p->next){
            if(strcmp(p->sv.MASV, MSV) == 0){
                return 1;
            }
        }
    }
    return 0;
}

void inDanhSachSV(nodeSinhVien* head, int start, int end){
    gotoxy(0, 4);
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
}

void chenSV(nodeSinhVien*& head, nodeSinhVien* a) {
    a->next = NULL;

    if (head == NULL || strcmp(a->sv.MASV, head->sv.MASV) < 0) {
        a->next = head;
        head = a;
        return;
    }

    nodeSinhVien* p = head;
    while (p->next != NULL && strcmp(a->sv.MASV, p->next->sv.MASV) > 0) {
        p = p->next;
    }
    a->next = p->next;
    p->next = a;
}

void themSV(DanhSachLop & dsLop, nodeSinhVien *& head){
    gotoxy(0, mouse);
    cout<<"Nhap thong tin sinh vien";
    gotoxy(0,mouse + 1);
    cout<<"MSV";gotoxy(12, mouse + 1); cout<<":";
    gotoxy(0,mouse + 2);
    cout<<"Ho & ten dem";gotoxy(12, mouse + 2); cout<<":";
    gotoxy(0,mouse + 3);
    cout<<"Ten";gotoxy(12, mouse + 3); cout<<":";
    gotoxy(0,mouse + 4);
    cout<<"Gioi tinh";gotoxy(12, mouse + 4); cout<<":";
    SinhVien them;
    strcpy(them.Password, "123");
    // Nhap ma sinh vien
    while(true){
        gotoxy(14, mouse + 1);
        NhapMa(them.MASV, 10);
        if(strlen(them.MASV) == 0){
            thongBaoLoi("Ma sinh vien khong duoc de trong", 14, mouse + 1);
            continue;
        }
        if(timSV(dsLop, them.MASV) == 1){
            thongBaoLoi("Ma sinh vien da ton tai", 14, mouse + 1);
            continue;
        }
        break;
    }
    //Nhap Ho
    while(true){
        gotoxy(14,mouse + 2);
        NhapTen(them.HO, 50);
        chuanHoaKhoangCach(them.HO);
        if(strlen(them.HO) == 0){
            thongBaoLoi(" Khong duoc de trong thong tin SV", 14, mouse + 2);
            continue;
        }
        gotoxy(14,mouse + 2);
        clearLine(14,mouse + 2, 50);
        gotoxy(14,mouse + 2);
        cout<<them.HO;
        break;
    }
    // nhap ten
    while(true){
        gotoxy(14,mouse + 3);
        NhapTen(them.TEN, 20);
        chuanHoaKhoangCach(them.TEN);
        if(strlen(them.TEN) == 0){
            thongBaoLoi(" Khong duoc de trong thong tin SV", 14, mouse + 3);
            continue;
        }
        gotoxy(14,mouse + 3);
        clearLine(14,mouse + 3, 20);
        gotoxy(14,mouse + 3);
        cout<<them.TEN;
        break;
    }
    // nhap phai
    while(true){
        gotoxy(14,mouse + 4);
        NhapTen(them.PHAI, 4);
        chuanHoaKhoangCach(them.PHAI);
        if(strlen(them.PHAI) == 0){
            thongBaoLoi(" Khong duoc de trong thong tin SV", 14, mouse + 4);
            continue;
        }
        if(strcmp(them.PHAI, "NAM") == 0 || strcmp(them.PHAI, "NU") == 0){
            clearLine(14, mouse + 4, 4);
            gotoxy(14,mouse + 4);cout<<them.PHAI;
            break;
        }
        else{
            thongBaoLoi("Chi co the dien NAM hoac NU", 14, mouse + 4);
            continue;
        }
    }
    gotoxy(0, mouse + 5);
    cout<<"Ban co chac muon them sinh vien moi?(Y/N)";
    char ch;
    while(true){
        ch = toupper(getch());
        if(ch == 'Y'){
            nodeSinhVien* newSV = new nodeSinhVien;
            newSV->sv = them;
            chenSV(head, newSV);
            thongBaoLoi("Them thanh cong", 0, mouse + 6);
            return;
        }
        if(ch == 'N'){
            return;
        }
    }
}

void xoaSV(nodeSinhVien* &head){
    if(head == NULL){
        thongBaoLoi("Lop chua co sinh vien", 0, mouse );
        return;
    }
    char MSV[10];
    gotoxy(0, mouse);
    cout<<"Nhap ma sinh vien muon xoa";
    gotoxy(0,mouse + 1);
    cout<<"MSV:";
    char ch;
    while(true){
        gotoxy(5, mouse + 1);
        NhapMa(MSV, 10);
        if(strlen(MSV) == 0){
            thongBaoLoi("Ma sinh vien trong!", 5, mouse + 1);
            continue;
        }
        nodeSinhVien* p;
        nodeSinhVien* pre = NULL;
        for( p = head; p != NULL && strcmp(MSV, p->sv.MASV) >=0; p = p->next){
            if(strcmp(MSV, p->sv.MASV) ==0) break;
            pre = p;
        }
        if(p == NULL || strcmp(MSV, p->sv.MASV) != 0){
            thongBaoLoi("Khong tim thay sinh vien trong lop nay", 5, mouse + 1);
            continue;
        }
        if(p->sv.dsDiemThi != NULL){
            thongBaoLoi("Sinh vien nay da co diem thi, khong the xoa", 0, mouse + 2);
            gotoxy(0,mouse +2);   
            cout<<"Ban co muon thuc hien lai thao tac khong?(Y/N)";
            while(true){
                ch = toupper(getch());
                if(ch == 'Y'){
                        clearLine(0,mouse + 1,10);
                        clearLine(0,mouse + 2,100);
                        break;
                    }
                if(ch == 'N'){
                    thongBaoLoi("Huy thao tac!", 0,mouse +3);
                    return;
                }
            }
            continue;
        }
        gotoxy(0, mouse + 2);
        cout<<"Ban co chac muon xoa sinh vien nay?(Y/N)";
        while(true){
            ch = toupper(getch());
            if(ch == 'Y'){
                if(pre == NULL){
                    head = head->next;
                    delete(p);
                }
                else{
                    pre->next = p->next;
                    delete(p);
                }
                thongBaoLoi("Xoa thanh cong!", 0, mouse + 3);
                return;
            }
            if(ch == 'N'){
                return;
            }
        }
    }
}

void PrintRow(int row, const string &label, const char* value, bool highlight) {
    if (highlight) HighLight();
    else Normal();

    gotoxy(0, row);
    cout << setw(12) << left << label << ": " << value;
    Normal();
}


// Hàm nhập 1 trường, cho phép để trống (để giữ nguyên)
bool NhapTruong(char* field, int maxLen, int x, int y) {
    char* buffer = new char[maxLen];
    clearLine(x, y, maxLen);
    gotoxy(x, y);
    NhapTen(buffer, maxLen); // hoặc NhapMa nếu là mã sinh viên
    chuanHoaKhoangCach(buffer);

    if (strlen(buffer) == 0) {
        return false;
    }

    strcpy(field, buffer);
    gotoxy(x, y);
    cout << field;
    return true;
}


struct Field {
    string label;
    char value[50];
};

void suaSV(nodeSinhVien*& head, DanhSachLop &dsLop, int start, int end){
    if(head == NULL){
        thongBaoLoi("Lop chua co sinh vien", 0, mouse );
        return;
    }
    char MSV[10];
    char HO[50];
    char TEN[20];
    char PHAI[4];
    gotoxy(0, mouse);
    cout<<"Nhap ma sinh vien muon sua";
    gotoxy(0,mouse + 1);
    cout<<"MSV:";
    nodeSinhVien* sua;
    nodeSinhVien* pre = nullptr;
    int currentLine = 1;
    while(true){
        gotoxy(5, mouse + 1);
        NhapMa(MSV, 10);
        if(strlen(MSV) == 0){
            thongBaoLoi("Ma sinh vien trong!", 5, mouse + 1);
            continue;
        }
        for( sua = head; sua != NULL && strcmp(MSV, sua->sv.MASV) > 0; sua = sua->next){
            pre = sua;
        }
        if(sua == NULL || strcmp(MSV, sua->sv.MASV) != 0){
            thongBaoLoi("Khong tim thay sinh vien trong lop nay", 5, mouse + 1);
            continue;
        }
        break;
    }
    Field fields[4] = { {"MSV", ""},
                        {"Ho & ten dem", ""},
                        {"Ten", ""},
                        {"Gioi tinh", ""}
                    };
    strcpy(fields[0].value, sua->sv.MASV);
    strcpy(fields[1].value, sua->sv.HO);
    strcpy(fields[2].value, sua->sv.TEN);
    strcpy(fields[3].value, sua->sv.PHAI);
    gotoxy(0, mouse);
    cout<<"Thong tin sinh vien (ENTER de sua, ESC de thoat, S de luu thay doi)";
    for (int i = 0; i < 4; i++) {
        PrintRow(mouse + 1 + i, fields[i].label, fields[i].value, (i + 1 == currentLine));
    }
    char a;
    bool notSaved = false;
    while(true){
        PrintRow(mouse + currentLine, fields[currentLine - 1].label, fields[currentLine - 1].value, true);
        a = toupper(getch());
        
        if (a == -32 || a == 0) { // phím đặc biệt
            a = getch();
            if (a == 80 && currentLine < 4) {// mũi tên xuống
                PrintRow(mouse + currentLine, fields[currentLine - 1].label, fields[currentLine - 1].value, false);
                currentLine ++;
                PrintRow(mouse + currentLine, fields[currentLine - 1].label, fields[currentLine - 1].value, true);
            } 
            else if (a == 72 && currentLine > 1) {// mũi tên Lên
                PrintRow(mouse + currentLine, fields[currentLine - 1].label, fields[currentLine - 1].value, false);
                currentLine --;
                PrintRow(mouse + currentLine, fields[currentLine - 1].label, fields[currentLine - 1].value, true);
            } 
        }
        else{
            if(a == 13){
                notSaved = true;
                PrintRow(mouse + currentLine, fields[currentLine - 1].label, fields[currentLine - 1].value, false);
                if(currentLine == 1){
                    while(true){
                        clearLine(14,mouse + currentLine, 10);
                        gotoxy(14, mouse + currentLine);
                        NhapMa(MSV, 10);
                        if(strlen(MSV) != 0){
                            if(strcmp(MSV, sua->sv.MASV) == 0){
                                strcpy(fields[currentLine - 1].value, MSV);
                                break;
                            }
                            else{
                                if(timSV(dsLop, MSV)){
                                    thongBaoLoi("Ma sinh vien da ton tai!", 14, mouse + currentLine);
                                    continue;
                                }
                                else{
                                    strcpy(fields[currentLine - 1].value, MSV);
                                    break;
                                }
                            }
                        }
                        else break;
                    }   
                }
                if(currentLine == 4){
                    while(true){
                        clearLine(14,mouse + currentLine, 10);
                        gotoxy(14, mouse + currentLine);
                        NhapTen(PHAI, 4);
                        chuanHoaKhoangCach(PHAI);
                        if(strlen(PHAI) == 0)break;
                        if(strcmp(PHAI, "NAM") == 0 || strcmp(PHAI, "NU") == 0){
                            clearLine(14,mouse + currentLine, 10);
                            gotoxy(14,mouse + currentLine);cout<<PHAI;
                            strcpy(fields[currentLine - 1].value,PHAI);
                            break;
                        }
                        else{
                            thongBaoLoi("Chi co the dien NAM hoac NU", 14, mouse + currentLine);
                            continue;
                        }
                    }
                }

                if(currentLine == 2){
                    if(NhapTruong(HO, 50, 14, mouse + currentLine)){
                        strcpy(fields[currentLine - 1].value,HO);
                    }
                }

                if(currentLine == 3){
                    if(NhapTruong(TEN, 20, 14, mouse + currentLine)){
                        strcpy(fields[currentLine - 1].value, TEN);
                    }
                }
                
            }
            if(a == 27){
                while(notSaved){
                    gotoxy(0, mouse + 5);
                    cout<< "Co thay doi chua luu, co chac muon thoat khong? (Y/N)";
                    a = toupper(getch());
                    if(a == 'Y') return;
                    if(a == 'N') {
                        clearLine(0, mouse + 5, 60);
                        break;
                    }
                }
                if(notSaved == false) return;
            }
            if(a == 'S'){
                gotoxy(0,mouse +5);
                cout << "Ban co chac muon luu thay doi khong? (Y/N): ";
                while (true) {
                    a = toupper(getch());
                    if (a == 'Y') {
                        notSaved = false;
                        strcpy(sua->sv.HO, fields[1].value);
                        strcpy(sua->sv.TEN, fields[2].value);
                        strcpy(sua->sv.PHAI, fields[3].value);
                        if(strcmp(sua->sv.MASV, fields[0].value) != 0){
                            strcpy(sua->sv.MASV, fields[0].value);
                            if (sua == head) {
                            head = sua->next;
                            } else {
                                pre->next = sua->next;
                            }
                            chenSV(head, sua);
                        }
                        inDanhSachSV(head, start, end);
                        GhiDanhSachLop(dsLop, "Lop.txt");
                        thongBaoLoi("Luu thanh cong", 0, mouse + 6);
                        clearLine(0,mouse + 5, 100);
                        break;
                    } else if (a == 'N') {
                        clearLine(0,mouse + 5, 100);
                        break;
                    }
                }  
            }
        }
    }
}

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



void HienThiDanhSachSinhVien(DanhSachLop &dsLop, Lop* &lop, ListMonHoc &dsMonHoc){
    char a;
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
        cout<<"Ma lop: "<<lop->MALOP<<setw(10)<<"Ten lop"<<lop->TENLOP;
        // Vẽ đường viền trên của bảng
        gotoxy(0,1);
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

		// gotoxy(0, 4);
		// int index = 0;
        // nodeSinhVien* p = head;
        // while (p != NULL && index < end) {
        //     if (index >= start) {
        //         cout << char(179) << setw(10) << left << index + 1 
        //             << char(179) << setw(20) << left << p->sv.MASV
        //             << char(179) << setw(40) << left << p->sv.HO
        //             << char(179) << setw(20) << left << p->sv.TEN
        //             << char(179) << setw(10) << left << p->sv.PHAI
        //             << char(179) << endl;
        //     }
        //     p = p->next;
        //     index++;
        // }
        // for(int i = end+1; i<= start + Page_Size; i++){
		// 	cout 	<< char(179) << setw(10) << left << i
		// 			<< char(179) << setw(20) << left << ""
		// 			<< char(179) << setw(40) << left << ""
		// 			<< char(179) << setw(20) << left << ""
		// 			<< char(179) << setw(10) << left << ""
		// 			<< char(179) << endl;
        // }
		// // Vẽ đường phân cách tiêu đề và dữ liệu
		// cout << char(192) << string(10, char(196)) << char(193)
		// 		<< string(20, char(196)) << char(193)
		// 		<< string(40, char(196)) << char(193)
		// 		<< string(20, char(196)) << char(193)
		// 		<< string(10, char(196)) << char(217) << endl;
        inDanhSachSV(head,start, end);
		gotoxy(49,19);
        cout<<"("<<currentPage<<"/"<<numPage<<")"<<endl;
        gotoxy(0, 20);
        cout << "A: Them | D: Xoa | E: Sua | C: Xem Diem | S: luu | Q: Thoat" << endl;

        a = toupper(getch());
        if (a == -32 || a == 0) { // phím đặc biệt
            a = getch();
            if (a == 75 && currentPage > 1) currentPage--; // mũi tên trái
            else if (a == 77 && currentPage < numPage) currentPage++; // mũi tên phải
        }
		else{
            switch (a) {
                case 'A':   // Thêm SV
                    themSV(dsLop, head);
                    slSV = demSinhVien(head);
                    break;
                case 'D':   // Xóa SV
                    xoaSV(head);
                    slSV = demSinhVien(head);
                    break;
                case 'E':   // Sửa SV
                    suaSV(head, dsLop, start, end);
                    break;
                case 'C':   //Xem Diem
                    xemDiem(lop, dsMonHoc);
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