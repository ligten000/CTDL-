#ifndef QUANGREAD_H
#define QUANGREAD_H
#include <iostream>
#include <fstream>
#include "CTDL.h"
#include <cstring>
#include <sstream>
using namespace std;

void LoadLop(DanhSachLop &dsLop, const char* filename) {
    ifstream in(filename);
    if (!in.is_open()) {
        cerr << "Không thể mở file: " << filename << '\n';
        return;
    }

    in >> dsLop.n;
    in.ignore(); // Bỏ dòng mới

    for (int i = 0; i < dsLop.n; ++i) {
        dsLop.lop[i] = new Lop;
        in.getline(dsLop.lop[i]->MALOP, 15, '|');
        in.getline(dsLop.lop[i]->TENLOP, 50);
        dsLop.lop[i]->listSV = nullptr;
    }

    in.close();
}


void LoadDanhSachSinhVien(DanhSachSinhVien &dsSV, const char* filename) {
    ifstream in(filename);
    if (!in.is_open()) {
        cerr << "Không thể mở file: " << filename << '\n';
        return;
    }

    in >> dsSV.n;
    in.ignore();

    for (int i = 0; i < dsSV.n; ++i) {
        in.getline(dsSV.ds[i].MASV, 10, '|');
        in.getline(dsSV.ds[i].HO, 50, '|');
        in.getline(dsSV.ds[i].TEN, 20, '|');
        in.getline(dsSV.ds[i].PHAI, 4, '|');
        in.getline(dsSV.ds[i].Password, 20);
        dsSV.ds[i].dsDiemThi = nullptr;
    }

    in.close();
}


void LoadLop_SinhVien(DanhSachLop &dsLop, const DanhSachSinhVien &dsSV, const char* filename) {
    ifstream in(filename);
    if (!in.is_open()) {
        cerr << "Không thể mở file: " << filename << '\n';
        return;
    }

    char malop[15], masv[10];
    while (in >> malop >> masv) {
        for (int i = 0; i < dsLop.n; ++i) {
            if (strcmp(dsLop.lop[i]->MALOP, malop) == 0) {
                for (int j = 0; j < dsSV.n; ++j) {
                    if (strcmp(dsSV.ds[j].MASV, masv) == 0) {
                        nodeSinhvien *node = new nodeSinhvien;
                        node->sv = dsSV.ds[j];
                        node->next = dsLop.lop[i]->listSV;
                        dsLop.lop[i]->listSV = node;
                        break;
                    }
                }
                break;
            }
        }
    }

    in.close();
}



void LoadDiemVaBaiThi(DanhSachSinhVien &dsSV, const char* filename) {
    ifstream in(filename);
    if (!in.is_open()) {
        cerr << "Không thể mở file: " << filename << '\n';
        return;
    }

    int n;
    in >> n;
    in.ignore();

    for (int i = 0; i < n; ++i) {
        char masv[10];
        in.getline(masv, 10);
        int index = -1;
        for (int j = 0; j < dsSV.n; ++j) {
            if (strcmp(dsSV.ds[j].MASV, masv) == 0) {
                index = j;
                break;
            }
        }

        if (index == -1) continue;

        int slDiem;
        in >> slDiem;
        in.ignore();
        for (int k = 0; k < slDiem; ++k) {
            nodeDiemThi *ndt = new nodeDiemThi;
            in.getline(ndt->diem.MAMH, 16, '|');
            in >> ndt->diem.Diem;
            in.ignore();
            ndt->diem.baithi = nullptr;

            int soCau;
            in >> soCau;
            in.ignore();
            for (int h = 0; h < soCau; ++h) {
                nodeBaithi *nbt = new nodeBaithi;
                in.getline(nbt->btct.idcauhoi, 16, '|');
                in >> nbt->btct.traloi;
                in.ignore();
                nbt->next = ndt->diem.baithi;
                ndt->diem.baithi = nbt;
            }

            ndt->next = dsSV.ds[index].dsDiemThi;
            dsSV.ds[index].dsDiemThi = ndt;
        }
    }

    in.close();
}


void quangRead(DanhSachLop& dsLop,DanhSachSinhVien& dsSV){
    LoadLop(dsLop, "Lop.txt");
    LoadDanhSachSinhVien(dsSV, "SinhVien.txt");
    LoadDiemVaBaiThi(dsSV, "DiemThi.txt");
    LoadLop_SinhVien(dsLop, dsSV, "Lop_SinhVien.txt");

    cout << "load completed.\n";
};

void TaoDuLieuMau(DanhSachLop &dsLop, DanhSachSinhVien &dsSV) {
    // Tạo lớp mẫu
    dsLop.n = 2;
    dsLop.lop[0] = new Lop;
    strcpy(dsLop.lop[0]->MALOP, "CTK44");
    strcpy(dsLop.lop[0]->TENLOP, "CNTT K44");
    dsLop.lop[0]->listSV = nullptr;

    dsLop.lop[1] = new Lop;
    strcpy(dsLop.lop[1]->MALOP, "DLH45");
    strcpy(dsLop.lop[1]->TENLOP, "Du Lich H45");
    dsLop.lop[1]->listSV = nullptr;

    // Tạo sinh viên mẫu
    dsSV.n = 3;

    strcpy(dsSV.ds[0].MASV, "SV001");
    strcpy(dsSV.ds[0].HO, "Nguyen Van");
    strcpy(dsSV.ds[0].TEN, "An");
    strcpy(dsSV.ds[0].PHAI, "NAM");
    strcpy(dsSV.ds[0].Password, "123");
    dsSV.ds[0].dsDiemThi = nullptr;

    strcpy(dsSV.ds[1].MASV, "SV002");
    strcpy(dsSV.ds[1].HO, "Tran Thi");
    strcpy(dsSV.ds[1].TEN, "Binh");
    strcpy(dsSV.ds[1].PHAI, "NU");
    strcpy(dsSV.ds[1].Password, "abc");
    dsSV.ds[1].dsDiemThi = nullptr;

    strcpy(dsSV.ds[2].MASV, "SV003");
    strcpy(dsSV.ds[2].HO, "Le");
    strcpy(dsSV.ds[2].TEN, "Cuong");
    strcpy(dsSV.ds[2].PHAI, "NAM");
    strcpy(dsSV.ds[2].Password, "pass");
    dsSV.ds[2].dsDiemThi = nullptr;

    // Gắn sinh viên vào lớp
    nodeSinhvien* sv1 = new nodeSinhvien{dsSV.ds[0], nullptr};
    nodeSinhvien* sv2 = new nodeSinhvien{dsSV.ds[1], nullptr};
    sv1->next = sv2;
    dsLop.lop[0]->listSV = sv1;

    nodeSinhvien* sv3 = new nodeSinhvien{dsSV.ds[2], nullptr};
    dsLop.lop[1]->listSV = sv3;

    // Gắn điểm và bài thi mẫu cho SV001
    nodeDiemThi* dt = new nodeDiemThi;
    strcpy(dt->diem.MAMH, "CTDL");
    dt->diem.Diem = 8.5;

    nodeBaithi* bt1 = new nodeBaithi{{"C01", 'A'}, nullptr};
    nodeBaithi* bt2 = new nodeBaithi{{"C02", 'C'}, nullptr};
    bt1->next = bt2;
    dt->diem.baithi = bt1;
    dt->next = nullptr;
    dsSV.ds[0].dsDiemThi = dt;

    // Gắn điểm cho SV003
    nodeDiemThi* dt2 = new nodeDiemThi;
    strcpy(dt2->diem.MAMH, "LTNC");
    dt2->diem.Diem = 7.0;
    dt2->diem.baithi = nullptr;
    dt2->next = nullptr;
    dsSV.ds[2].dsDiemThi = dt2;

    cout << "create completed.\n";
}
#endif