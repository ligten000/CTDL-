#ifndef CHUANHOATEN_H
#define CHUANHOATEN_H

#include <string>
#include <cctype> 
#include <cstring>
using namespace std;






void chuanHoaKhoangCach(char S[]) {
    int len = strlen(S);
    int i = 0, j = 0;

    // Bỏ khoảng trắng đầu
    while (isspace(S[i])) i++;

    bool spaceFound = false;
    for (; i < len; i++) {
        if (isspace(S[i])) {
            if (!spaceFound) {
                S[j++] = ' ';
                spaceFound = true;
            }
        } else {
            S[j++] = S[i];
            spaceFound = false;
        }
    }

    // Xóa khoảng trắng cuối
    if (j > 0 && S[j - 1] == ' ')
        j--;

    S[j] = '\0';
}

inline int ChuanHoaTen(string &a,string &result) {
    result.clear();
    if (a.empty()) return 0;

    int n = a.length();
    int i = 0;

    // Bỏ ký tự đầu không phải chữ cái hoặc khoảng trắng
    while (i < n && (isspace(a[i]) || !isalpha(a[i]))) i++;
    if (i == n) return 0;

    // Ghi chữ cái đầu tiên viết hoa
    result += toupper(a[i]);
    i++;
    while (i < n) {
        if (isalpha(a[i])) {
            result += tolower(a[i]);
        } else if (isspace(a[i])) {
            result += ' ';
            while (i < n && (isspace(a[i]) || !isalpha(a[i]))) i++;
            if (i < n) result += toupper(a[i]);
        }
        i++;
    }

    if (!result.empty() && result.back() == ' ')
        result.pop_back();

    return !result.empty();
}



// Hàm chuẩn hóa nội dung câu hỏi thi
void chuanHoaNoiDungCauHoi(char* noiDung) {
    if (noiDung == NULL || strlen(noiDung) == 0) return;
    
    int len = strlen(noiDung);
    char* temp = new char[len + 1];
    strcpy(temp, noiDung);
    
    // Bước 1: Xóa khoảng trắng đầu và cuối
    int start = 0, end = len - 1;
    while (start < len && (temp[start] == ' ' || temp[start] == '\t')) start++;
    while (end >= 0 && (temp[end] == ' ' || temp[end] == '\t')) end--;
    
    if (start > end) {
        noiDung[0] = '\0';
        delete[] temp;
        return;
    }
    
    // Bước 2: Chuẩn hóa khoảng trắng giữa các từ
    int j = 0;
    bool prevSpace = false;
    
    for (int i = start; i <= end; i++) {
        char c = temp[i];
        
        // Nếu là khoảng trắng
        if (c == ' ' || c == '\t') {
            if (!prevSpace) {
                noiDung[j++] = ' ';
                prevSpace = true;
            }
        }
        // Nếu là ký tự khác
        else {
            noiDung[j++] = c;
            prevSpace = false;
        }
    }
    
    // Xóa khoảng trắng cuối nếu có
    if (j > 0 && noiDung[j-1] == ' ') {
        j--;
    }
    
    noiDung[j] = '\0';
    delete[] temp;
}

// Hàm chuẩn hóa tất cả nội dung câu hỏi (nội dung + các lựa chọn)
void chuanHoaToanBoCauHoi(CauHoi& cauHoi) {
    // NoiDung là string, cần chuyển về char* để chuẩn hóa
    char noiDungTemp[128];
    strncpy(noiDungTemp, cauHoi.NoiDung.c_str(), 127);
    noiDungTemp[127] = '\0';
    chuanHoaNoiDungCauHoi(noiDungTemp);
    cauHoi.NoiDung = string(noiDungTemp);
    
    // Các lựa chọn A, B, C, D là char[]
    chuanHoaNoiDungCauHoi(cauHoi.A);
    chuanHoaNoiDungCauHoi(cauHoi.B);
    chuanHoaNoiDungCauHoi(cauHoi.C);
    chuanHoaNoiDungCauHoi(cauHoi.D);
}

// Hàm chuẩn hóa đáp án câu hỏi
void chuanHoaDapAn(char& dapAn) {
    // Chuyển về chữ in hoa
    dapAn = toupper(dapAn);
}

// Hàm kiểm tra đáp án hợp lệ
bool HopLeDapAn(char dapAn) {
    // Chỉ chấp nhận A, B, C, D (đã in hoa)
    return (dapAn == 'A' || dapAn == 'B' || dapAn == 'C' || dapAn == 'D');
}


#endif