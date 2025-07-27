#ifndef QUANGREAD_H
#define QUANGREAD_H
#include <iostream>
#include <fstream>
#include "CTDL.h"
#include <cstring>
#include <sstream>
using namespace std;

void ThemSinhVienTheoThuTu(nodeSinhVien*& head, SinhVien* sv) {
    nodeSinhVien* newNode = new nodeSinhVien;
    newNode->sv = sv;
    newNode->next = NULL;

    if (head == NULL || strcmp(sv->MASV, head->sv->MASV) < 0) {
        newNode->next = head;
        head = newNode;
        return;
    }

    nodeSinhVien* prev = NULL;
    nodeSinhVien* curr = head;

    while (curr != NULL && strcmp(sv->MASV, curr->sv->MASV) > 0) {
        prev = curr;
        curr = curr->next;
    }

    prev->next = newNode;
    newNode->next = curr;
}

SinhVien* TimSinhVien(nodeSinhVien* head, const char* masv) {
    while (head != NULL) {
        if(strcmp(head->sv->MASV, masv) > 0) return NULL;
        if (strcmp(head->sv->MASV, masv) == 0) {
            return head->sv;
        }
        head = head->next;
    }
    return NULL;
}

// ====== Đọc file sinhvien.txt ======
void DocDanhSachSinhVien(nodeSinhVien*& head, const char* filename) {
    ifstream in(filename);
    if (!in) return;

    int n;
    in >> n;
    in.ignore();

    for (int i = 0; i < n; ++i) {
        SinhVien* sv = new SinhVien;
        in.getline(sv->MASV, 10, '|');
        in.getline(sv->HO, 50, '|');
        in.getline(sv->TEN, 20, '|');
        in.getline(sv->PHAI, 4, '|');
        in.getline(sv->Password, 20);

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
            nodeBaithi* tail = NULL;
            for (int k = 0; k < soCau; ++k) {
                nodeBaithi* cau = new nodeBaithi;
                in.getline(cau->btct.idcauhoi, 16, '|');
                in >> cau->btct.traloi;
                in.ignore();
                cau->next = NULL;
                if (tail == NULL) {
                    diemNode->diem.baithi = tail = cau;
                } else {
                    tail->next = cau;
                    tail = cau;
                }
            }
            diemNode->next = sv->dsDiemThi;
            sv->dsDiemThi = diemNode;
        }

        ThemSinhVienTheoThuTu(head, sv);
    }
    in.close();
}

// ====== Đọc file lop.txt ======
void DocDanhSachLop(DanhSachLop& dsLop, nodeSinhVien* &dsSV, const char* filename) {
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
            char masv[10];
            in.getline(masv, 10);

            SinhVien* sv = TimSinhVien(dsSV, masv);
            if (sv != NULL) {
                nodeSinhVien* node = new nodeSinhVien;
                node->sv = sv;
                node->next = NULL;
                if (tail == NULL) {
                    dsLop.lop[i]->listSV = tail = node;
                } else {
                    tail->next = node;
                    tail = node;
                }
            }
        }
    }
    in.close();
}

void quangRead(DanhSachLop& dsLop, nodeSinhVien*& dsSV, const char* fileSV, const char* fileLop){
    DocDanhSachSinhVien(dsSV,fileSV);
    DocDanhSachLop(dsLop,dsSV,fileLop);
    cout<<"Load completed"<<endl;
}
#endif