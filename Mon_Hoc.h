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





#endif
