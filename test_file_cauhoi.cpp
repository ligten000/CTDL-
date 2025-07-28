#include <iostream>
#include "file_cauhoi.h"

using namespace std;

int main() {
    ListMonHoc ds;
    
    // Đọc file danh sách môn học
    cout << "=== DOC FILE DANH SACH MON HOC ===" << endl;
    DocFileDSMonHoc(ds, "danhsach_monhoc.txt");
    
    cout << "So mon hoc: " << ds.n << endl;
    
    // In thông tin các môn học
    for (int i = 0; i < ds.n; i++) {
        cout << "\nMon hoc " << i + 1 << ":" << endl;
        cout << "Ma mon hoc: " << ds.list[i].MAMH << endl;
        cout << "Ten mon hoc: " << ds.list[i].TENMH << endl;
        cout << "So cau hoi: " << DemSoCauHoi(ds.list[i].treeCauHoi) << endl;
    }
    
    // Ghi lại file để kiểm tra
    cout << "\n=== GHI LAI FILE ===" << endl;
    GhiFileDSMonHoc(ds, "output_monhoc.txt");
    cout << "Da ghi file output_monhoc.txt" << endl;
    
    return 0;
} 