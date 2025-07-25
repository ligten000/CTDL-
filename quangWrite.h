#ifndef QUANGWRITE_H
#define QUANGWRITE_H
#include <iostream>
#include <fstream>
#include "CTDL.h"
#include <cstring>
#include <sstream>

using namespace std;



void SaveLop(const DanhSachLop& dsLop, const char* filename) {
    ofstream out(filename);
    if (!out.is_open()) {
        cerr << "Không thể mở file: " << filename << '\n';
        return;
    }
    out << dsLop.n << '\n';
    for (int i = 0; i < dsLop.n; ++i) 
        out << dsLop.lop[i]->MALOP << '|'<<dsLop.lop[i]->TENLOP << '\n';
    
    out.close();
}

void SaveDanhSachSinhVien(const DanhSachSinhVien& dsSV, const char* filename) {
    ofstream out(filename);
    if (!out.is_open()) {
        cerr << "Không thể mở file: " << filename << '\n';
        return;
    }
    out << dsSV.n << '\n';
    for (int i = 0; i < dsSV.n; ++i) {
        out << dsSV.ds[i].MASV << '|' << dsSV.ds[i].HO << '|' << dsSV.ds[i].TEN << '|' << dsSV.ds[i].PHAI<< '|' <<dsSV.ds[i].Password<<'\n';
    }
    out.close();
}

void SaveLop_SinhVien(const DanhSachLop& dsLop, const char* filename) {
    ofstream out(filename);
    if (!out.is_open()) {
        cerr << "Không thể mở file: " << filename << '\n';
        return;
    }
    for (int i = 0; i < dsLop.n; ++i) {
        nodeSinhvien* p = dsLop.lop[i]->listSV;
        while (p != nullptr) {
            out << dsLop.lop[i]->MALOP << " " << p->sv.MASV << '\n';
            p = p->next;
        }
    }
    out.close();
}

void SaveDiemVaBaiThi(const DanhSachSinhVien& dsSV, const char* filename) {
    ofstream out(filename);
    if (!out.is_open()) {
        cerr << "Không thể mở file: " << filename << '\n';
        return;
    }
    out << dsSV.n << '\n';
    for (int i = 0; i < dsSV.n; ++i) {
        SinhVien sv = dsSV.ds[i];
        out << sv.MASV << '\n';
        // Ghi danh sách điểm thi
        nodeDiemThi* diem = sv.dsDiemThi;
        int dem = 0;
        for (nodeDiemThi* p = diem; p != nullptr; p = p->next) dem++;
        out << dem << '\n';
        for (nodeDiemThi* p = diem; p != nullptr; p = p->next) {
            out << p->diem.MAMH << '|'<< p->diem.Diem << '\n';
            // Ghi danh sách bài thi chi tiết
            nodeBaithi* bt = p->diem.baithi;
            int sobt = 0;
            for (nodeBaithi* q = bt; q != nullptr; q = q->next) sobt++;
            out << sobt << '\n';
            for (nodeBaithi* q = bt; q != nullptr; q = q->next) {
                out << q->btct.idcauhoi << '|'<< q->btct.traloi << '\n';
            }
        }
    }
    out.close();
}

void quangWrite(DanhSachLop &dsLop, DanhSachSinhVien &dsSV){
    SaveLop(dsLop, "Lop.txt");
    SaveDanhSachSinhVien(dsSV, "SinhVien.txt");
    SaveLop_SinhVien(dsLop, "Lop_SinhVien.txt");
    SaveDiemVaBaiThi(dsSV, "DiemThi.txt");

    cout << "save completed.\n";
};
#endif