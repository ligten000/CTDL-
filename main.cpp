#include <iostream>
#include "CTDL.h"
#include "file_cauhoi.h"
#include "quanly_monhoc.h"
#include "quanly_cauhoi.h"
using namespace std;

// Khởi tạo dữ liệu mẫu
void KhoiTaoDuLieuMau(ListMonHoc &ds) {
    ds.n = 0;
    
    // Thêm một số môn học mẫu
    MonHoc mh1, mh2, mh3;
    
    strcpy(mh1.MAMH, "CS101");
    strcpy(mh1.TENMH, "Lap trinh C++");
    mh1.treeCauHoi = NULL;
    
    strcpy(mh2.MAMH, "CS102");
    strcpy(mh2.TENMH, "Cau truc du lieu");
    mh2.treeCauHoi = NULL;
    
    strcpy(mh3.MAMH, "CS103");
    strcpy(mh3.TENMH, "Toan roi rac");
    mh3.treeCauHoi = NULL;
    
    ds.list[ds.n++] = mh1;
    ds.list[ds.n++] = mh2;
    ds.list[ds.n++] = mh3;
    
    // Thêm một số câu hỏi mẫu cho môn học đầu tiên
    CauHoi ch1, ch2, ch3;
    
    strcpy(ch1.Id, "1");
    ch1.NoiDung = "Cau lenh nao dung de khai bao bien trong C++?";
    strcpy(ch1.A, "var");
    strcpy(ch1.B, "int");
    strcpy(ch1.C, "declare");
    strcpy(ch1.D, "variable");
    ch1.DapAn = 'B';
    
    strcpy(ch2.Id, "2");
    ch2.NoiDung = "Ham main() trong C++ co kieu tra ve la gi?";
    strcpy(ch2.A, "void");
    strcpy(ch2.B, "int");
    strcpy(ch2.C, "string");
    strcpy(ch2.D, "char");
    ch2.DapAn = 'B';
    
    strcpy(ch3.Id, "3");
    ch3.NoiDung = "Toan tu nao dung de lay dia chi cua bien?";
    strcpy(ch3.A, "&");
    strcpy(ch3.B, "*");
    strcpy(ch3.C, "@");
    strcpy(ch3.D, "#");
    ch3.DapAn = 'A';
    
    // Thêm câu hỏi vào cây của môn học đầu tiên
    ChenCauHoiVaoCay(ds.list[0].treeCauHoi, ch1);
    ChenCauHoiVaoCay(ds.list[0].treeCauHoi, ch2);
    ChenCauHoiVaoCay(ds.list[0].treeCauHoi, ch3);
    
    // Cập nhật nextId
    nextId = 4;
    
    cout << "Da khoi tao du lieu mau!" << endl;
}

// Menu chính
void MenuChinh(ListMonHoc &ds) {
    int choice;
    do {
        cout << "\n=== HE THONG QUAN LY THI TRAC NGHIEM ===" << endl;
        cout << "1. Quan ly mon hoc" << endl;
        cout << "2. Quan ly cau hoi thi" << endl;
        cout << "3. Ghi du lieu ra file" << endl;
        cout << "4. Doc du lieu tu file" << endl;
        cout << "5. Khoi tao du lieu mau" << endl;
        cout << "0. Thoat" << endl;
        cout << "Chon chuc nang: ";
        cin >> choice;
        cin.ignore();
        
        switch (choice) {
            case 1:
                MenuQuanLyMonHoc(ds);
                break;
            case 2:
                MenuQuanLyCauHoi(ds);
                break;
            case 3:
                GhiFileDSMonHoc(ds, "cauhoi.txt");
                cout << "Da ghi du lieu ra file cauhoi.txt!" << endl;
                break;
            case 4:
                DocFileDSMonHoc(ds, "cauhoi.txt");
                CapNhatNextId(ds);
                cout << "Da doc du lieu tu file cauhoi.txt!" << endl;
                break;
            case 5:
                KhoiTaoDuLieuMau(ds);
                break;
            case 0:
                cout << "Cam on ban da su dung chuong trinh!" << endl;
                break;
            default:
                cout << "Lua chon khong hop le!" << endl;
        }
    } while (choice != 0);
}

int main() {
    ListMonHoc ds;
    ds.n = 0;
    
    cout << "=== HE THONG QUAN LY THI TRAC NGHIEM ===" << endl;
    cout << "Chuong trinh quan ly danh sach mon hoc va cau hoi thi" << endl;
    
    // Thử đọc dữ liệu từ file trước
    DocFileDSMonHoc(ds, "cauhoi.txt");
    if (ds.n > 0) {
        CapNhatNextId(ds);
        cout << "Da doc du lieu tu file cauhoi.txt!" << endl;
    } else {
        cout << "Khong tim thay file du lieu. Ban co the khoi tao du lieu mau." << endl;
    }
    
    MenuChinh(ds);
    
    return 0;
} 