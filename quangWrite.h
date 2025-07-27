#ifndef QUANGWRITE_H
#define QUANGWRITE_H
#include <iostream>
#include <fstream>
#include "CTDL.h"
#include <cstring>
#include <sstream>

using namespace std;


void GhiDanhSachSinhVien(nodeSinhVien* head, const char* filename) {
    ofstream out(filename);
    if (!out) return;

    // Đếm số lượng sinh viên
    int count = 0;
    for (nodeSinhVien* p = head; p != NULL; p = p->next) {
        count++;
    }
    out << count << '\n';

    for (nodeSinhVien* p = head; p != NULL; p = p->next) {
        SinhVien* sv = p->sv;
        out << sv->MASV << '|' << sv->HO << '|' << sv->TEN << '|' 
            << sv->PHAI << '|' << sv->Password << '\n';

        // Ghi danh sách điểm thi
        int soMon = 0;
        for (nodeDiemThi* d = sv->dsDiemThi; d != NULL; d = d->next) {
            soMon++;
        }
        out << soMon << '\n';

        for (nodeDiemThi* d = sv->dsDiemThi; d != NULL; d = d->next) {
            out << d->diem.MAMH << '|' << d->diem.Diem << '|';

            // Ghi số câu hỏi đã trả lời
            int soCau = 0;
            for (nodeBaithi* c = d->diem.baithi; c != NULL; c = c->next) {
                soCau++;
            }
            out << soCau << '\n';

            for (nodeBaithi* c = d->diem.baithi; c != NULL; c = c->next) {
                out << c->btct.idcauhoi << '|' << c->btct.traloi << '\n';
            }
        }
    }
    out.close();
}

void GhiDanhSachLop(const DanhSachLop& dsLop, const char* filename) {
    ofstream out(filename);
    if (!out) return;

    out << dsLop.n << '\n';
    for (int i = 0; i < dsLop.n; ++i) {
        Lop* lop = dsLop.lop[i];
        out << lop->MALOP << '|' << lop->TENLOP << '|';

        // Đếm số sinh viên
        int count = 0;
        for (nodeSinhVien* sv = lop->listSV; sv != NULL; sv = sv->next) {
            count++;
        }
        out << count << '\n';

        for (nodeSinhVien* sv = lop->listSV; sv != NULL; sv = sv->next) {
            out << sv->sv->MASV << '\n';
        }
    }
    out.close();
}


void quangWrite(const DanhSachLop& dsLop, nodeSinhVien* dsSV, const char* fileSV, const char* fileLop) {
    GhiDanhSachSinhVien(dsSV, fileSV);
    GhiDanhSachLop(dsLop, fileLop);
    cout<<"Save completed"<<endl;
}
#endif