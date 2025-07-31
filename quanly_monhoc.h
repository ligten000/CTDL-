#ifndef QUANLY_MONHOC_H
#define QUANLY_MONHOC_H

#include <iostream>
#include <cstring>
#include "CTDL.h"
using namespace std;

// Tìm môn học theo mã môn học, trả về index hoặc -1 nếu không tìm thấy
int TimMonHocTheoMa(const ListMonHoc &ds, const char* maMH) {
    cout << "Tim mon hoc voi ma: [" << maMH << "]" << endl;
    for (int i = 0; i < ds.n; ++i) {
        cout << "So sanh voi: [" << ds.list[i].MAMH << "]" << endl;
        if (strcmp(ds.list[i].MAMH, maMH) == 0) {
            cout << "Tim thay tai vi tri: " << i << endl;
            return i;
        }
    }
    cout << "Khong tim thay!" << endl;
    return -1;
}

// Kiểm tra mã môn học có trùng không
bool KiemTraTrungMaMonHoc(const ListMonHoc &ds, const char* maMH) {
    return TimMonHocTheoMa(ds, maMH) != -1;
}

// Chuẩn hóa mã môn học: chỉ giữ chữ in hoa, số và dấu "-"
void ChuanHoaMaMonHoc(char* maMH) {
    char temp[16];
    int j = 0;
    
    for (int i = 0; maMH[i] != '\0' && j < 15; ++i) {
        if (isupper(maMH[i]) || isdigit(maMH[i]) || maMH[i] == '-') {
            temp[j++] = maMH[i];
        }
    }
    temp[j] = '\0';
    strcpy(maMH, temp);
}

// Thêm môn học mới
void ThemMonHoc(ListMonHoc &ds) {
    if (ds.n >= MAX_ListMH) {
        cout << "Danh sach mon hoc da day!" << endl;
        return;
    }
    
    MonHoc mh;
    cout << "Nhap ma mon hoc (toi da 15 ky tu): ";
    cin.getline(mh.MAMH, 16);
    
    // Chuẩn hóa mã môn học
    ChuanHoaMaMonHoc(mh.MAMH);
    
    // Kiểm tra mã rỗng sau khi chuẩn hóa
    if (strlen(mh.MAMH) == 0) {
        cout << "Ma mon hoc khong hop le!" << endl;
        return;
    }
    
    // Kiểm tra trùng mã
    cout << "Ma mon hoc sau chuan hoa: [" << mh.MAMH << "]" << endl;
    if (KiemTraTrungMaMonHoc(ds, mh.MAMH)) {
        cout << "Ma mon hoc da ton tai!" << endl;
        return;
    }
    
    cout << "Nhap ten mon hoc (toi da 49 ky tu): ";
    cin.getline(mh.TENMH, 50);
    
    // Khởi tạo cây câu hỏi rỗng
    mh.treeCauHoi = NULL;
    
    // Thêm vào danh sách
    ds.list[ds.n] = mh;
    ds.n++;
    
    cout << "Them mon hoc thanh cong!" << endl;
}

// Xóa môn học
void XoaMonHoc(ListMonHoc &ds) {
    char maMH[16];
    cout << "Nhap ma mon hoc can xoa: ";
    cin.getline(maMH, 16);
    
    // Chuẩn hóa mã môn học
    ChuanHoaMaMonHoc(maMH);
    
    int index = TimMonHocTheoMa(ds, maMH);
    if (index == -1) {
        cout << "Khong tim thay mon hoc!" << endl;
        return;
    }
    
    // Kiểm tra xem môn học có câu hỏi không
    if (ds.list[index].treeCauHoi != NULL) {
        cout << "Khong the xoa mon hoc da co cau hoi!" << endl;
        return;
    }
    
    // Xóa môn học bằng cách dồn các phần tử phía sau lên
    for (int i = index; i < ds.n - 1; ++i) {
        ds.list[i] = ds.list[i + 1];
    }
    ds.n--;
    
    cout << "Xoa mon hoc thanh cong!" << endl;
}

// Sửa môn học
void SuaMonHoc(ListMonHoc &ds) {
    char maMH[16];
    cout << "Nhap ma mon hoc can sua: ";
    cin.getline(maMH, 16);
    
    // Chuẩn hóa mã môn học
    ChuanHoaMaMonHoc(maMH);
    
    int index = TimMonHocTheoMa(ds, maMH);
    if (index == -1) {
        cout << "Khong tim thay mon hoc!" << endl;
        return;
    }
    
    cout << "Mon hoc hien tai: " << ds.list[index].TENMH << endl;
    cout << "Nhap ten mon hoc moi: ";
    cin.getline(ds.list[index].TENMH, 50);
    
    cout << "Sua mon hoc thanh cong!" << endl;
}

// In danh sách môn học
void InDanhSachMonHoc(const ListMonHoc &ds) {
    if (ds.n == 0) {
        cout << "Danh sach mon hoc trong!" << endl;
        return;
    }
    
    cout << "\n=== DANH SACH MON HOC ===" << endl;
    cout << "STT\tMa MH\t\tTen Mon Hoc\t\tSo cau hoi" << endl;
    cout << "------------------------------------------------" << endl;
    
    for (int i = 0; i < ds.n; ++i) {
        int soCauHoi = 0;
        // Đếm số câu hỏi trong cây (có thể dùng hàm DemSoCauHoi từ file_cauhoi.h)
        nodeCauhoi* p = ds.list[i].treeCauHoi;
        while (p != NULL) {
            soCauHoi++;
            if (p->left != NULL) p = p->left;
            else p = p->right;
        }
        
        cout << (i + 1) << "\t" << ds.list[i].MAMH << "\t\t" 
             << ds.list[i].TENMH << "\t\t" << soCauHoi << endl;
    }
    cout << "------------------------------------------------" << endl;
}

// Menu quản lý môn học
void MenuQuanLyMonHoc(ListMonHoc &ds) {
    int choice;
    do {
        cout << "\n=== QUAN LY MON HOC ===" << endl;
        cout << "1. Them mon hoc" << endl;
        cout << "2. Xoa mon hoc" << endl;
        cout << "3. Sua mon hoc" << endl;
        cout << "4. In danh sach mon hoc" << endl;
        cout << "0. Thoat" << endl;
        cout << "Chon chuc nang: ";
        cin >> choice;
        cin.ignore(); // Xóa buffer
        
        switch (choice) {
            case 1:
                ThemMonHoc(ds);
                break;
            case 2:
                XoaMonHoc(ds);
                break;
            case 3:
                SuaMonHoc(ds);
                break;
            case 4:
                InDanhSachMonHoc(ds);
                break;
            case 0:
                cout << "Thoat quan ly mon hoc!" << endl;
                break;
            default:
                cout << "Lua chon khong hop le!" << endl;
        }
    } while (choice != 0);
}

#endif // QUANLY_MONHOC_H 