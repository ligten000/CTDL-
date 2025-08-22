#ifndef CHUANHOATEN_H
#define CHUANHOATEN_H

#include <string>
#include <cctype> 
using namespace std;

inline int strim(string &a, string &result){
    if (a.empty()) return 0;
    int n = a.length();
    while (isspace(a[n -1]))
    {
        a.pop_back();
        n--;
    }
    int i = 0;
    while (i < n && isspace(a[i])) i++;
    if (i == n) return 0;

    while (i<n)
    {
        result += a[i];
    } 
    return 1;
};

#include <cstring>


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

// void chuanHoaTenMonHoc(char* tenMon) {
//     if (tenMon == NULL || strlen(tenMon) == 0) return;
    
//     int len = strlen(tenMon);
//     char* temp = new char[len + 1];
//     strcpy(temp, tenMon);
    
//     // Bước 1: Xóa khoảng trắng đầu và cuối
//     int start = 0, end = len - 1;
//     while (start < len && (temp[start] == ' ' || temp[start] == '\t')) start++;
//     while (end >= 0 && (temp[end] == ' ' || temp[end] == '\t')) end--;
    
//     if (start > end) {
//         tenMon[0] = '\0';
//         delete[] temp;
//         return;
//     }
    
//     // Bước 2: Chuẩn hóa khoảng trắng giữa các từ và chữ cái
//     int j = 0;
//     bool prevSpace = false;
//     bool prevLetter = false;
    
//     for (int i = start; i <= end; i++) {
//         char c = temp[i];
        
//         // Nếu là khoảng trắng
//         if (c == ' ' || c == '\t') {
//             if (!prevSpace && prevLetter) {
//                 tenMon[j++] = ' ';
//                 prevSpace = true;
//             }
//         }
//         // Nếu là chữ cái hoặc số
//         else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) {
//             if (prevLetter) {
//                 // Chữ cái thường giữa từ
//                 tenMon[j++] = tolower(c);
//             } else {
//                 // Chữ cái đầu từ - viết hoa
//                 tenMon[j++] = toupper(c);
//             }
//             prevLetter = true;
//             prevSpace = false;
//         }
//         // Bỏ qua các ký tự khác
//     }
    
//     // Xóa khoảng trắng cuối nếu có
//     if (j > 0 && tenMon[j-1] == ' ') {
//         j--;
//     }
    
//     tenMon[j] = '\0';
//     delete[] temp;
// }

// // Hàm chuẩn hóa mã môn học
// void chuanHoaMaMonHoc(char* maMon) {
//     if (maMon == NULL || strlen(maMon) == 0) return;
    
//     int len = strlen(maMon);
//     char* temp = new char[len + 1];
//     strcpy(temp, maMon);
    
//     // Bước 1: Xóa khoảng trắng đầu và cuối
//     int start = 0, end = len - 1;
//     while (start < len && (temp[start] == ' ' || temp[start] == '\t')) start++;
//     while (end >= 0 && (temp[end] == ' ' || temp[end] == '\t')) end--;
    
//     if (start > end) {
//         maMon[0] = '\0';
//         delete[] temp;
//         return;
//     }
    
//     // Bước 2: Chuyển thành chữ in hoa và loại bỏ khoảng trắng giữa
//     int j = 0;
//     for (int i = start; i <= end; i++) {
//         char c = temp[i];
        
//         // Nếu là chữ cái, số hoặc ký tự đặc biệt hợp lệ
//         if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || 
//             (c >= '0' && c <= '9') || c == '-' || c == '_') {
//             // Chuyển thành chữ in hoa nếu là chữ cái
//             if (c >= 'a' && c <= 'z') {
//                 maMon[j++] = toupper(c);
//             } else {
//                 maMon[j++] = c;
//             }
//         }
//         // Bỏ qua khoảng trắng và các ký tự khác
//     }
    
//     maMon[j] = '\0';
//     delete[] temp;
// }

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

// Hàm chuẩn hóa và kiểm tra đáp án
bool chuanHoaVaKiemTraDapAn(char& dapAn) {
    // Chuẩn hóa trước
    chuanHoaDapAn(dapAn);
    
    // Kiểm tra tính hợp lệ
    if (!HopLeDapAn(dapAn)) {
        return false; // Không hợp lệ
    }
    
    return true; // Hợp lệ
}
#endif