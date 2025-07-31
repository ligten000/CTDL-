#ifndef FILE_LOP_H
#define FILE_LOP_H
#include <iostream>
#include <fstream>
#include "CTDL.h"
#include <cstring>
#include <sstream>
using namespace std;

void ThemSinhVienTheoThuTu(nodeSinhVien*& head, SinhVien sv) {
    nodeSinhVien* newNode = new nodeSinhVien;
    newNode->sv = sv;
    newNode->next = NULL;

    if (head == NULL || strcmp(sv.MASV, head->sv.MASV) < 0) {
        newNode->next = head;
        head = newNode;
        return;
    }

    nodeSinhVien* prev = NULL;
    nodeSinhVien* curr = head;

    while (curr != NULL && strcmp(sv.MASV, curr->sv.MASV) > 0) {
        prev = curr;
        curr = curr->next;
    }

    prev->next = newNode;
    newNode->next = curr;
}


// ====== Đọc file lop.txt ======
void DocDanhSachLop(DanhSachLop& dsLop, const char* filename) {
    ifstream in(filename);
    if (!in) return;

    in >> dsLop.n;
    in.ignore();

    for (int i = 0; i < dsLop.n; ++i) {
        dsLop.lop[i] = new Lop;
        in.getline(dsLop.lop[i]->MALOP, 15, '|');
        in.getline(dsLop.lop[i]->TENLOP, 50, '|');
        int soSV;
        in >> soSV;
        in.ignore();

        nodeSinhVien* tail = NULL;
        for (int j = 0; j < soSV; ++j) {
            SinhVien sv;
            in.getline(sv.MASV, 10, '|');
            in.getline(sv.HO, 50, '|');
            in.getline(sv.TEN, 20, '|');
            in.getline(sv.PHAI, 4, '|');
            in.getline(sv.Password, 20);

            int soMon;
            in >> soMon;
            in.ignore();

            for (int j = 0; j < soMon; ++j) {
                nodeDiemThi* diemNode = new nodeDiemThi;
                in.getline(diemNode->diem.MAMH, 16, '|');
                in >> diemNode->diem.Diem;
                int soCau;
                in.ignore();
                in >> soCau;
                in.ignore();

                diemNode->diem.baithi = NULL;
                nodeBaithi* tailCauHoi = NULL;
                for (int k = 0; k < soCau; ++k) {
                    nodeBaithi* cau = new nodeBaithi;
                    in.getline(cau->btct.idcauhoi, 16, '|');
                    in >> cau->btct.traloi;
                    in.ignore();
                    cau->next = NULL;
                    if (tailCauHoi == NULL) {
                        diemNode->diem.baithi = tailCauHoi = cau;
                    } else {
                        tailCauHoi->next = cau;
                        tailCauHoi = cau;
                    }
                }
                diemNode->next = sv.dsDiemThi;
                sv.dsDiemThi = diemNode;
            }

            ThemSinhVienTheoThuTu(dsLop.lop[i]->listSV, sv);
        }
    }
    in.close();
    cout<<"Load completed"<<endl;
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
        for (nodeSinhVien* sv = lop->listSV; sv != NULL; sv = sv->next) count++;
        
        out << count << '\n';

        for (nodeSinhVien* p = lop->listSV; p != NULL; p = p->next) {
            out << p->sv.MASV << '|' << p->sv.HO << '|' << p->sv.TEN << '|' 
            << p->sv.PHAI << '|' << p->sv.Password << '\n';

            // Ghi danh sách điểm thi
            int soMon = 0;
            for (nodeDiemThi* d = p->sv.dsDiemThi; d != NULL; d = d->next) soMon++;
            
            out << soMon << '\n';

            for (nodeDiemThi* d = p->sv.dsDiemThi; d != NULL; d = d->next) {
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
    }
    out.close();
}
#endif