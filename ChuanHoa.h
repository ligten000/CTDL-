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
#endif