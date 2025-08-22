#ifndef GVSYSTEM_H
#define GVSYSTEM_H
#include <iostream>
#include <fstream>
#include "CTDL.h"
#include "ChuanHoa.h"
#include "file_lop.h"
#include <cstring>
#include <iomanip>
#include <sstream>
#include "GiaoDienLop.h"
using namespace std;

void xapxepDanhSachLop(DanhSachLop & dsLop){
    for (int i = 0; i < dsLop.n - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < dsLop.n; j++) {
            if (strcmp(dsLop.lop[j]->MALOP, dsLop.lop[minIndex]->MALOP) < 0) {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            // Hoán đổi con trỏ lớp
            Lop* temp = dsLop.lop[i];
            dsLop.lop[i] = dsLop.lop[minIndex];
            dsLop.lop[minIndex] = temp;
        }
    }
}

void themLop(DanhSachLop &dsLop, Lop *& lop, char malop[], char tenlop[]){
    if (dsLop.n >= Max_lop) {
        cout << "\nKhong the them lop moi. Danh sach lop da day!\n";
        return;
    }
    gotoxy(0,mouse);
    cout << "Nhap ma lop";
    gotoxy(13,mouse); cout<<":";
    gotoxy(0,mouse +1);
    cout << "Nhap ten lop";
    gotoxy(13,mouse +1); cout<<":";
    int i;
    while (true)
    {   
        gotoxy(15,mouse);
        NhapMa(malop, 15);
        if (strlen(malop) == 0) {
            clearLine(15,mouse,15);
            thongBaoLoi("Ma lop khong duoc rong!", 15,mouse);
            continue;
        }
        

        bool trung = false;
            for (i = 0; i < dsLop.n; ++i) {
                int cmp = strcmp(dsLop.lop[i]->MALOP, malop);
                if (cmp == 0) {
                    trung = true;
                    break;
                }
                if (cmp > 0) break;
            }

            if (trung) {
                clearLine(15,mouse,15);
                thongBaoLoi("Ma lop da ton tai", 15,mouse);
            } else {
                break; 
            }   
    }
    
    // Kiểm tra trùng mã lớp
    while (true) {
        gotoxy(15,mouse +1);
        NhapChuoi(tenlop, 50);
        chuanHoaKhoangCach(tenlop);
        if (strlen(tenlop) == 0) {
            clearLine(15,mouse +1,50);
            thongBaoLoi("Ten lop khong duoc rong!",15,mouse +1);
        } else {
            break;
        }
    }

    // Xác nhận thêm
    gotoxy(0,mouse +2);
    cout << "Ban co chac chan muon them lop moi? (y/n): ";
    char confirm = toupper(getch());
    switch(confirm){
        case 'N':
            cout << "\nHuy them lop.\n";
            Sleep(1000);
            return;
        case 'Y':
            strcpy(lop->MALOP, malop);
            strcpy(lop->TENLOP, tenlop);
            for(int j = dsLop.n; j>i; j--){
                dsLop.lop[j] = dsLop.lop[j-1];
            }
            dsLop.lop[i] = lop;
            dsLop.n++;
            cout<<"Them thanh cong"<<endl;

            Sleep(1000);
            return ;
        default:
            break;
            
    }  
}

void xoaLop(DanhSachLop&dsLop){
    char malop[15];
    gotoxy(0,mouse);
    cout << "Nhap ma lop:";
    int i;
    while (true)
    {   
        gotoxy(15,mouse);
        NhapMa(malop, 15);
        if (strlen(malop) == 0) {
            clearLine(15,mouse,15);
            thongBaoLoi("Ma lop khong duoc rong!", 15,mouse);
            continue;
        }
        bool tontai = false;
        for (i = 0; i < dsLop.n; ++i) {
            int cmp = strcmp(dsLop.lop[i]->MALOP, malop);
            if (cmp == 0) {
                tontai = true;
                break;
            }
            if (cmp > 0) {
                break;
            }
        }

        if (tontai == false) {
            clearLine(15,mouse,15);
            thongBaoLoi("Ma lop khong ton tai!",15,mouse);
        } else {
            if(demSinhVien(dsLop.lop[i]->listSV) != 0){
                gotoxy(0,mouse +1);   
                thongBaoLoi("Lop da co sinh vien, khong the xoa!",0,mouse +1);
                clearLine(15,mouse,30);
                gotoxy(15,mouse);
            }
            else break;
        }
    }
    gotoxy(0,mouse +1);
    cout << "Ban co chac chan muon xoa lop khong? (y/n): ";
    char confirm;
    while(true){
        confirm = toupper(getch());
        switch(confirm){
            case 'N':
                cout << "\nHuy xoa lop.\n";
                Sleep(1000);
                return;
            case 'Y':
                delete dsLop.lop[i];
                for(int j = i; j <dsLop.n - 1;j++)
                    dsLop.lop[j] = dsLop.lop[j+1];
                dsLop.n--;
                cout<<"Xoa thanh cong"<<endl;
                Sleep(1000);
                return ;
            default:
                continue;  
        } 
    } 
}

void suaLop(DanhSachLop& ds) {
    char malop[15], tenlop[50];
    gotoxy(0,mouse);
    cout << "Nhap ma lop can sua:";
    int i;

    // Bước 1: Tìm lớp theo mã
    while (true) {
        gotoxy(21,mouse);
        NhapMa(malop, 15);
        if (strlen(malop) == 0) {
            clearLine(21,mouse,15);
            thongBaoLoi("Ma lop khong duoc rong!", 21, mouse);
            continue;
        }

        bool found = false;
        for (i = 0; i < ds.n; ++i) {
            int cmp = strcmp(ds.lop[i]->MALOP, malop);
            if (cmp == 0) {
                found = true;
                break;
            }
            if (cmp > 0) break;
        }

        if (!found) {
            clearLine(21,mouse,15);
            thongBaoLoi("Ma lop khong ton tai", 21, mouse);
        } else break;
    }

    // Bước 2: Hiển thị để sửa
    strcpy(malop, ds.lop[i]->MALOP);
    strcpy(tenlop, ds.lop[i]->TENLOP);

    clearLine(0,mouse,50);
    gotoxy(0,mouse);
    cout << "Thong tin lop (ENTER de sua, ESC de thoat, S de luu thay doi)";

    int currentRow = mouse+1; 
    char ch, confirm;
    bool saved = true;

    while (true) {
        // Vẽ lại giao diện
        for (int row = mouse +1; row <= mouse +2; row++) {
            gotoxy(0, row);
            cout << (row == currentRow ? "->" : "  ");
            if (row == mouse +1) {
                cout << " Ma lop : "; gotoxy(15, mouse +1); cout << setw(15) << left << malop;
            } else {
                cout << " Ten lop: "; gotoxy(15, mouse +2); cout << setw(50) << left << tenlop;
            }
        }

        ch = toupper(getch());

        switch (ch) {
        case 13: // ENTER để chỉnh sửa dòng hiện tại
            saved = false;
            if (currentRow == mouse +1) {
                clearLine(15, mouse +1, 15);
                gotoxy(15, mouse +1);
                NhapMa(malop, 15);

                if (strlen(malop) == 0) {
                    strcpy(malop, ds.lop[i]->MALOP); // giữ nguyên nếu bỏ trống
                } else if (strcmp(malop, ds.lop[i]->MALOP) != 0) {
                    // Kiểm tra trùng mã
                    bool trung = false;
                    for (int j = 0; j < ds.n; j++) {
                        if (j == i) continue;
                        if (strcmp(ds.lop[j]->MALOP, malop) == 0) {
                            trung = true;
                            break;
                        }
                    }
                    if (trung) {
                        thongBaoLoi("Ma lop da ton tai", 15, mouse +1);
                        strcpy(malop, ds.lop[i]->MALOP);
                    }
                }
            } else if (currentRow == mouse +2) {
                clearLine(15, mouse +2, 50);
                gotoxy(15, mouse +2);
                NhapChuoi(tenlop, 50);
                chuanHoaKhoangCach(tenlop);
                if (strlen(tenlop) == 0)
                    strcpy(tenlop, ds.lop[i]->TENLOP);
            }
            break;

        case 'S': // Lưu thay đổi
            if(saved == false){
                gotoxy(0,mouse +3);
                cout << "Ban co chac muon luu thay doi khong? (Y/N): ";
                while (true) {
                    confirm = toupper(getch());
                    if (confirm == 'Y') {
                        saved = true;
                        strcpy(ds.lop[i]->MALOP, malop);
                        strcpy(ds.lop[i]->TENLOP, tenlop);
                        xapxepDanhSachLop(ds);
                        GhiDanhSachLop(ds, "Lop.txt");
                        thongBaoLoi("Luu thanh cong", 0,23);
                        gotoxy(0,mouse +3);
                        clearCurrentLine();
                        break;
                    } else if (confirm == 'N') {
                        thongBaoLoi("Huy luu", 0,23);
                        gotoxy(0,mouse +3);
                        clearCurrentLine();
                        break;
                    }
                }
            }
            break;

        case 72: // Mũi tên lên
            if (currentRow > mouse +1) currentRow--;
            break;
        case 80: // Mũi tên xuống
            if (currentRow < mouse +2) currentRow++;
            break;

        case 27: // ESC - Thoát nếu chưa lưu
            if (saved) return;
            gotoxy(0,mouse +3);
            cout << "Chua luu thay doi, co chac muon thoat khong (Y/N): ";
            while (true) {
                confirm = toupper(getch());
                if (confirm == 'Y') return;
                else if (confirm == 'N') {
                    clearCurrentLine();
                    break;
                }
            }
            break;

        default:
            break;
        }
    }
}

void xemLop(DanhSachLop & ds, ListMonHoc &dsMonHoc){
    char malop[15];
    int i;
    gotoxy(0,mouse);
    cout << "Nhap ma lop: ";
    
    while (true) {
        gotoxy(13,mouse);
        NhapMa(malop, 15);
        if (strlen(malop) == 0) {
            clearLine(13,mouse,15);
            thongBaoLoi("Ma lop khong duoc rong!", 13, mouse);
            continue;
        }

        bool found = false;
        for (i = 0; i < ds.n; ++i) {
            int cmp = strcmp(ds.lop[i]->MALOP, malop);
            if (cmp == 0) {
                found = true;
                break;
            }
            if (cmp > 0) break;
        }

        if (!found) {
            clearLine(21,mouse,15);
            thongBaoLoi("Ma lop khong ton tai", 13, mouse);
        } else break;
    }
    gotoxy(0,mouse +1);
    cout << "Ban co chac chan muon xem lop khong? (y/n): ";
    char confirm;
    while(true){
        confirm = toupper(getch());
        switch(confirm){
            case 'N':
                return;
            case 'Y': 
                HienThiDanhSachSinhVien(ds,ds.lop[i], dsMonHoc);
                return ;
            default:
                continue;  
        } 
    } 
}


bool hienthidanhsachLop(DanhSachLop &dsLop, ListMonHoc &dsMonHoc) {
    Normal();system("cls");
    char a;
    int currentPage = 1;
    int start;
    int end;
    int numPage = (dsLop.n + Page_Size - 1/Page_Size);

    // Vẽ đường viền trên của bảng
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
    while (true) {
        start = (currentPage - 1) * Page_Size;
        end = min(start + Page_Size, dsLop.n);

        // Xoá vùng dữ liệu cũ (ghi đè bằng khoảng trắng)
        for (int i = 0; i <= Page_Size; i++) {
            gotoxy(0, 3 + i);
            cout << string(83, ' ');
        }

        // In dữ liệu từng dòng
        gotoxy(0, 3);
        for (int i = start; i < end; i++) {
            cout << char(179) << setw(10) << left << i + 1
                 << char(179) << setw(20) << left << dsLop.lop[i]->MALOP
                 << char(179) << setw(30) << left << dsLop.lop[i]->TENLOP
                 << char(179) << setw(20) << left << demSinhVien(dsLop.lop[i]->listSV)
                 << char(179) << endl;
        }
        for(int i = end+1; i<= start + Page_Size; i++){
            cout << char(179) << setw(10) << left << i
                 << char(179) << setw(20) << left << ""
                 << char(179) << setw(30) << left << ""
                 << char(179) << setw(20) << left << ""
                 << char(179) << endl;
        }
        cout << char(192) << string(10, char(196)) << char(193)
             << string(20, char(196)) << char(193)
             << string(30, char(196)) << char(193)
             << string(20, char(196)) << char(217) << endl;
        gotoxy(36,18);
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
                    {Lop* lop = new Lop;
                    char malop[15],tenlop[50];
                    themLop(dsLop, lop, malop, tenlop);
                    break;
                    }
                case 'D':   // Xóa lớp
                    xoaLop(dsLop);
                    break;
                case 'E':   // Sửa lớp
                    suaLop(dsLop);
                    break;
                case 'C':   // Chọn lớp
                    xemLop(dsLop, dsMonHoc);
                    break;
                case 'S':
                    GhiDanhSachLop(dsLop, "Lop.txt");
                    cout<<"Luu thanh cong";
                    Sleep(1000);
                    clearCurrentLine();
                    break;
                case 'Q':   // Thoát khỏi danh sách
                    return true;
                default:
                    break;
            }
        }// Sau mỗi thao tác -> quay lại đầu vòng lặp và in lại danh sách
    }

    return false;
}

#endif