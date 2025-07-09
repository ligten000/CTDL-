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