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
using namespace std;

const int Page_Size = 15;

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
            if ((c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '-' || c == '_') {
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
void NhapChuoi(char ma[], int n) {
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
    gotoxy(0,19);
    cout << "Nhap ma lop";
    gotoxy(13,19); cout<<":";
    gotoxy(0,20);
    cout << "Nhap ten lop";
    gotoxy(13,20); cout<<":";
    int i;
    while (true)
    {   
        gotoxy(15,19);
        NhapMa(malop, 15);
        if (strlen(malop) == 0) {
            clearLine(15,19,15);
            thongBaoLoi("Ma lop khong duoc rong!", 15,19);
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
                clearLine(15,19,15);
                thongBaoLoi("Ma lop da ton tai", 15,19);
            } else {
                break; 
            }   
    }
    
    // Kiểm tra trùng mã lớp
    while (true) {
        gotoxy(15,20);
        NhapChuoi(tenlop, 50);
        chuanHoaKhoangCach(tenlop);
        if (strlen(tenlop) == 0) {
            clearLine(15,20,50);
            thongBaoLoi("Ten lop khong duoc rong!",15,20);
        } else {
            break;
        }
    }

    // Xác nhận thêm
    gotoxy(0,21);
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
    gotoxy(0,19);
    cout << "Nhap ma lop:";
    int i;
    while (true)
    {   
        gotoxy(15,19);
        NhapMa(malop, 15);
        if (strlen(malop) == 0) {
            clearLine(15,19,15);
            thongBaoLoi("Ma lop khong duoc rong!", 15,19);
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
            clearLine(15,19,15);
            thongBaoLoi("Ma lop khong ton tai!",15,19);
        } else {
            if(demSinhVien(dsLop.lop[i]) != 0){
                gotoxy(0,20);   
                thongBaoLoi("Lop da co sinh vien, khong the xoa!",0,20);
                clearLine(15,19,30);
                gotoxy(15,19);
            }
            else break;
        }
    }
    gotoxy(0,20);
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
    gotoxy(0,19);
    cout << "Nhap ma lop can sua:";
    int i;

    // Bước 1: Tìm lớp theo mã
    while (true) {
        gotoxy(21,19);
        NhapMa(malop, 15);
        if (strlen(malop) == 0) {
            clearLine(21,19,15);
            thongBaoLoi("Ma lop khong duoc rong!", 21, 19);
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
            clearLine(21,19,15);
            thongBaoLoi("Ma lop khong ton tai", 21, 19);
        } else break;
    }

    // Bước 2: Hiển thị để sửa
    strcpy(malop, ds.lop[i]->MALOP);
    strcpy(tenlop, ds.lop[i]->TENLOP);

    clearLine(0,19,50);
    gotoxy(0,19);
    cout << "Thong tin lop (ENTER de sua, ESC de thoat, S de luu thay doi)";

    int currentRow = 20; // 20: MALOP, 21: TENLOP
    char ch, confirm;
    bool saved = true;

    while (true) {
        // Vẽ lại giao diện
        for (int row = 20; row <= 21; row++) {
            gotoxy(0, row);
            cout << (row == currentRow ? "->" : "  ");
            if (row == 20) {
                cout << " Ma lop : "; gotoxy(15, 20); cout << setw(15) << left << malop;
            } else {
                cout << " Ten lop: "; gotoxy(15, 21); cout << setw(50) << left << tenlop;
            }
        }

        ch = toupper(getch());

        switch (ch) {
        case 13: // ENTER để chỉnh sửa dòng hiện tại
            saved = false;
            if (currentRow == 20) {
                clearLine(15, 20, 15);
                gotoxy(15, 20);
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
                        thongBaoLoi("Ma lop da ton tai", 15, 20);
                        strcpy(malop, ds.lop[i]->MALOP);
                    }
                }
            } else if (currentRow == 21) {
                clearLine(15, 21, 50);
                gotoxy(15, 21);
                NhapChuoi(tenlop, 50);
                chuanHoaKhoangCach(tenlop);
                if (strlen(tenlop) == 0)
                    strcpy(tenlop, ds.lop[i]->TENLOP);
            }
            break;

        case 'S': // Lưu thay đổi
            if(saved == false){
                gotoxy(0,22);
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
                        gotoxy(0,22);
                        clearCurrentLine();
                        break;
                    } else if (confirm == 'N') {
                        thongBaoLoi("Huy luu", 0,23);
                        gotoxy(0,22);
                        clearCurrentLine();
                        break;
                    }
                }
            }
            break;

        case 72: // Mũi tên lên
            if (currentRow > 20) currentRow--;
            break;
        case 80: // Mũi tên xuống
            if (currentRow < 21) currentRow++;
            break;

        case 27: // ESC - Thoát nếu chưa lưu
            if (saved) return;
            gotoxy(0,22);
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


bool hienthidanhsachLop(DanhSachLop &dsLop) {
    SetColor(15);
	SetBGColor(0);
    char a;
    int currentPage = 1;
    int start;
    int end;
    int numPage = (dsLop.n/Page_Size)+1;
    while (true) {
        system("cls");
        cout << setw(10) << left << "STT"
             << setw(20) << left << "Ma Lop"
             << setw(30) << left << "Ten Lop"
             << setw(20) << left << "So luong SV" << endl;
        cout << string(80, '-') << endl;
        start = (currentPage - 1) * Page_Size;
        end = min(start + Page_Size, dsLop.n);
        for (int i = start; i < end; i++) {
            cout << setw(10) << left << i + 1
                 << setw(20) << left << dsLop.lop[i]->MALOP
                 << setw(30) << left << dsLop.lop[i]->TENLOP
                 << setw(20) << left << demSinhVien(dsLop.lop[i])
                 << endl;
        }

        gotoxy(0, 17);
        cout << string(80, '-') << endl;
        gotoxy(38,17);
        cout<<"("<<currentPage<<"/"<<numPage<<")"<<endl;
        cout << "A: Them | D: Xoa | E: Sua | C: Chon | S: luu | Q: Thoat" << endl;

        a = toupper(getch());
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
                break;
            case 'S':
                GhiDanhSachLop(dsLop, "Lop.txt");
                cout<<"Luu thanh cong";
                Sleep(1000);
                clearCurrentLine();
                break;
            case 'Q':   // Thoát khỏi danh sách
                return true;
            case 75:
                if(currentPage > 1)currentPage--;
                break;
            case 77:
                if(currentPage < numPage)currentPage++;
                break;
            default:
                break;
        }
        // Sau mỗi thao tác -> quay lại đầu vòng lặp và in lại danh sách
    }

    return false;
}

#endif