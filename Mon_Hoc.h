#ifndef MON_HOC_H
#define MON_HOC_H

#include <string>
#include <cctype> 
#include <cstring>
#include "CTDL.h"
using namespace std;

// Hàm chuẩn hóa tên môn học
void chuanHoaTenMonHoc(char* tenMon) {
    if (tenMon == NULL || strlen(tenMon) == 0) return;
    
    int len = strlen(tenMon);
    char* temp = new char[len + 1];
    strcpy(temp, tenMon);
    
    // Bước 1: Xóa khoảng trắng đầu và cuối
    int start = 0, end = len - 1;
    while (start < len && (temp[start] == ' ' || temp[start] == '\t')) start++;
    while (end >= 0 && (temp[end] == ' ' || temp[end] == '\t')) end--;
    
    if (start > end) {
        tenMon[0] = '\0';
        delete[] temp;
        return;
    }
    
    // Bước 2: Chuẩn hóa khoảng trắng giữa các từ và chữ cái
    int j = 0;
    bool prevSpace = false;
    bool prevLetter = false;
    
    for (int i = start; i <= end; i++) {
        char c = temp[i];
        
        // Nếu là khoảng trắng
        if (c == ' ' || c == '\t') {
            if (!prevSpace && prevLetter) {
                tenMon[j++] = ' ';
                prevSpace = true;
            }
        }
        // Nếu là chữ cái hoặc số
        else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) {
            if (prevLetter) {
                // Chữ cái thường giữa từ
                tenMon[j++] = tolower(c);
            } else {
                // Chữ cái đầu từ - viết hoa
                tenMon[j++] = toupper(c);
            }
            prevLetter = true;
            prevSpace = false;
        }
        // Bỏ qua các ký tự khác
    }
    
    // Xóa khoảng trắng cuối nếu có
    if (j > 0 && tenMon[j-1] == ' ') {
        j--;
    }
    
    tenMon[j] = '\0';
    delete[] temp;
}

// Hàm chuẩn hóa mã môn học
void chuanHoaMaMonHoc(char* maMon) {
    if (maMon == NULL || strlen(maMon) == 0) return;
    
    int len = strlen(maMon);
    char* temp = new char[len + 1];
    strcpy(temp, maMon);
    
    // Bước 1: Xóa khoảng trắng đầu và cuối
    int start = 0, end = len - 1;
    while (start < len && (temp[start] == ' ' || temp[start] == '\t')) start++;
    while (end >= 0 && (temp[end] == ' ' || temp[end] == '\t')) end--;
    
    if (start > end) {
        maMon[0] = '\0';
        delete[] temp;
        return;
    }
    
    // Bước 2: Chuyển thành chữ in hoa và loại bỏ khoảng trắng giữa
    int j = 0;
    for (int i = start; i <= end; i++) {
        char c = temp[i];
        
        // Nếu là chữ cái, số hoặc ký tự đặc biệt hợp lệ
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || 
            (c >= '0' && c <= '9') || c == '-' || c == '_') {
            // Chuyển thành chữ in hoa nếu là chữ cái
            if (c >= 'a' && c <= 'z') {
                maMon[j++] = toupper(c);
            } else {
                maMon[j++] = c;
            }
        }
        // Bỏ qua khoảng trắng và các ký tự khác
    }
    
    maMon[j] = '\0';
    delete[] temp;
}

// Hàm tìm vị trí môn học theo MAMH
int timViTriMonHoc(const ListMonHoc& dsMH, const char* maMon) {
    if (maMon == NULL || dsMH.n == 0) return -1;
    
    for (int i = 0; i < dsMH.n; i++) {
        if (strcmp(dsMH.list[i].MAMH, maMon) == 0) {
            return i; // Tìm thấy, trả về index
        }
    }
    
    return -1; // Không tìm thấy
}

// Hàm kiểm tra tên môn học hợp lệ
bool HopLeTenMon(const char* tenMon) {
    if (tenMon == NULL) return false;
    
    int len = strlen(tenMon);
    
    // Kiểm tra không rỗng
    if (len == 0) return false;
    
    // Kiểm tra không toàn khoảng trắng
    for (int i = 0; i < len; i++) {
        if (tenMon[i] != ' ' && tenMon[i] != '\t') {
            return true; // Có ít nhất 1 ký tự không phải khoảng trắng
        }
    }
    
    return false; // Toàn khoảng trắng
}

// Hàm kiểm tra mã môn học hợp lệ
bool HopLeMAMH(const char* maMon) {
    if (maMon == NULL) return false;
    
    int len = strlen(maMon);
    
    // Kiểm tra độ dài: 3-5 ký tự
    if (len < 3 || len > 5) return false;
    
    // Kiểm tra từng ký tự phải là A-Z
    for (int i = 0; i < len; i++) {
        if (maMon[i] < 'A' || maMon[i] > 'Z') {
            return false;
        }
    }
    
    return true;
}

// Hàm kiểm tra mã môn học đã tồn tại chưa
bool TonTaiMAMH(const ListMonHoc& dsMH, const char* maMon) {
    if (maMon == NULL || dsMH.n == 0) return false;
    
    for (int i = 0; i < dsMH.n; i++) {
        if (strcmp(dsMH.list[i].MAMH, maMon) == 0) {
            return true; // Đã tồn tại
        }
    }
    
    return false; // Chưa tồn tại
}

// Hàm thêm môn học mới vào danh sách
bool ThemMonHoc(ListMonHoc& dsMH, const char* maMon, const char* tenMon) {
    // Kiểm tra đầu vào
    if (maMon == NULL || tenMon == NULL) return false;
    
    // Kiểm tra danh sách đã đầy chưa
    if (dsMH.n >= MAX_ListMH) {
        cout << "Danh sach mon hoc da day! Khong the them mon moi.\n";
        return false;
    }
    
    // Tạo bản sao để chuẩn hóa
    char maChuan[16], tenChuan[50];
    strcpy(maChuan, maMon);
    strcpy(tenChuan, tenMon);
    
    // Bước 1: Chuẩn hóa dữ liệu
    chuanHoaMaMonHoc(maChuan);
    chuanHoaTenMonHoc(tenChuan);
    
    // Bước 2: Kiểm tra tính hợp lệ
    if (!HopLeMAMH(maChuan)) {
        cout << "Ma mon hoc khong hop le! (3-5 ky tu A-Z)\n";
        return false;
    }
    
    if (!HopLeTenMon(tenChuan)) {
        cout << "Ten mon hoc khong hop le! (khong duoc rong hoac toan khoang trang)\n";
        return false;
    }
    
    // Bước 3: Kiểm tra trùng lặp
    if (TonTaiMAMH(dsMH, maChuan)) {
        cout << "Ma mon hoc " << maChuan << " da ton tai!\n";
        return false;
    }
    
    // Bước 4: Thêm môn học mới vào danh sách
    int viTri = dsMH.n;
    strcpy(dsMH.list[viTri].MAMH, maChuan);
    strcpy(dsMH.list[viTri].TENMH, tenChuan);
    dsMH.list[viTri].treeCauHoi = NULL; // Khởi tạo cây câu hỏi rỗng
    
    dsMH.n++; // Tăng số lượng môn học
    
    cout << "Them mon hoc thanh cong!\n";
    cout << "Ma mon: " << maChuan << endl;
    cout << "Ten mon: " << tenChuan << endl;
    
    return true;
}

// Hàm sửa tên môn học theo mã
bool SuaMonHoc(ListMonHoc& dsMH, const char* maMon, const char* tenMoi) {
    // Kiểm tra đầu vào
    if (maMon == NULL || tenMoi == NULL) return false;
    
    // Kiểm tra danh sách rỗng
    if (dsMH.n == 0) {
        cout << "Danh sach mon hoc rong! Khong co gi de sua.\n";
        return false;
    }
    
    // Bước 1: Tìm vị trí môn học cần sửa
    int viTri = timViTriMonHoc(dsMH, maMon);
    if (viTri == -1) {
        cout << "Khong tim thay mon hoc co ma: " << maMon << endl;
        return false;
    }
    
    // Bước 2: Chuẩn hóa tên mới
    char tenChuan[50];
    strcpy(tenChuan, tenMoi);
    chuanHoaTenMonHoc(tenChuan);
    
    // Bước 3: Kiểm tra tính hợp lệ của tên mới
    if (!HopLeTenMon(tenChuan)) {
        cout << "Ten mon hoc moi khong hop le! (khong duoc rong hoac toan khoang trang)\n";
        return false;
    }
    
    // Bước 4: Kiểm tra tên mới có khác tên cũ không
    if (strcmp(dsMH.list[viTri].TENMH, tenChuan) == 0) {
        cout << "Ten mon hoc moi giong ten cu! Khong can sua.\n";
        return false;
    }
    
    // Bước 5: Lưu tên cũ để thông báo
    char tenCu[50];
    strcpy(tenCu, dsMH.list[viTri].TENMH);
    
    // Bước 6: Cập nhật tên mới
    strcpy(dsMH.list[viTri].TENMH, tenChuan);
    
    cout << "Sua mon hoc thanh cong!\n";
    cout << "Ma mon: " << maMon << endl;
    cout << "Ten cu: " << tenCu << endl;
    cout << "Ten moi: " << tenChuan << endl;
    
    return true;
}

// Hàm xóa môn học theo mã
bool XoaMonHoc(ListMonHoc& dsMH, const char* maMon) {
    // Kiểm tra đầu vào
    if (maMon == NULL) return false;
    
    // Kiểm tra danh sách rỗng
    if (dsMH.n == 0) {
        cout << "Danh sach mon hoc rong! Khong co gi de xoa.\n";
        return false;
    }
    
    // Bước 1: Tìm vị trí môn học cần xóa
    int viTri = timViTriMonHoc(dsMH, maMon);
    if (viTri == -1) {
        cout << "Khong tim thay mon hoc co ma: " << maMon << endl;
        return false;
    }
    
    // Bước 2: Kiểm tra có thể xóa không (treeCauHoi == NULL)
    if (dsMH.list[viTri].treeCauHoi != NULL) {
        cout << "Khong the xoa mon hoc " << maMon << " vi da co cau hoi thi!\n";
        cout << "Hay xoa tat ca cau hoi truoc khi xoa mon hoc.\n";
        return false;
    }
    
    // Bước 3: Lưu thông tin môn học để thông báo
    char tenMon[50];
    strcpy(tenMon, dsMH.list[viTri].TENMH);
    
    // Bước 4: Xóa môn học bằng cách dồn các phần tử phía sau lên
    for (int i = viTri; i < dsMH.n - 1; i++) {
        strcpy(dsMH.list[i].MAMH, dsMH.list[i + 1].MAMH);
        strcpy(dsMH.list[i].TENMH, dsMH.list[i + 1].TENMH);
        dsMH.list[i].treeCauHoi = dsMH.list[i + 1].treeCauHoi;
    }
    
    // Bước 5: Giảm số lượng môn học
    dsMH.n--;
    
    cout << "Xoa mon hoc thanh cong!\n";
    cout << "Ma mon: " << maMon << endl;
    cout << "Ten mon: " << tenMon << endl;
    
    return true;
}

#endif
