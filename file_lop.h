#ifndef FILE_LOP_H
#define FILE_LOP_H
#include <iostream>
#include <fstream>
#include "CTDL.h"
#include <cstring>
#include <sstream>
using namespace std;

void ThemSinhVienTheoThuTu(nodeSinhVien*& head, nodeSinhVien* newNode) {
    if (head == NULL || strcmp(newNode->sv.MASV, head->sv.MASV) < 0) {
        newNode->next = head;
        head = newNode;
        return;
    }

    nodeSinhVien* prev = NULL;
    nodeSinhVien* curr = head;

    while (curr != NULL && strcmp(newNode->sv.MASV, curr->sv.MASV) > 0) {
        prev = curr;
        curr = curr->next;
    }

    prev->next = newNode;
    newNode->next = curr;
}

void ThemDiemThiTheoThuTu(SinhVien &sv, nodeDiemThi* diemMoi) {
    // TH1: Danh sách rỗng
    if (sv.dsDiemThi == nullptr) {
        sv.dsDiemThi = diemMoi;
        diemMoi->next = nullptr;
        return;
    }

    // TH2: Chèn vào đầu
    if (strcmp(diemMoi->diem.MAMH, sv.dsDiemThi->diem.MAMH) < 0) {
        diemMoi->next = sv.dsDiemThi;
        sv.dsDiemThi = diemMoi;
        return;
    }

    // TH3: Tìm vị trí chèn
    nodeDiemThi* prev = sv.dsDiemThi;
    nodeDiemThi* curr = sv.dsDiemThi->next;

    while (curr != nullptr && strcmp(diemMoi->diem.MAMH, curr->diem.MAMH) > 0) {
        prev = curr;
        curr = curr->next;
    }

    // Chèn vào giữa hoặc cuối
    prev->next = diemMoi;
    diemMoi->next = curr;
}


nodeCauhoi* TimCauHoiTrongCay(nodeCauhoi* root, const char* id) {
    if (root == nullptr) return nullptr;
    int cmp = strcmp(id, root->data.Id);
    if (cmp == 0) return root;
    else if (cmp < 0) return TimCauHoiTrongCay(root->left, id);
    else return TimCauHoiTrongCay(root->right, id);
}

MonHoc* TimMonHoc(ListMonHoc& dsMH, const char* mamh) {
    for (int i = 0; i < dsMH.n; i++) {
        if (strcmp(dsMH.list[i].MAMH, mamh) == 0) {
            return &dsMH.list[i];
        }
    }
    return nullptr;
}

void DocDanhSachLop(DanhSachLop& dsLop, ListMonHoc& dsMonHoc, const char* filename) {
    ifstream in(filename);
    if (!in) {
        cout << "Không thể mở file\n";
        return;
    }

    in >> dsLop.n;
    in.ignore();

    for (int i = 0; i < dsLop.n; ++i) {
        dsLop.lop[i] = new Lop;
        Lop* lop = dsLop.lop[i];
        lop->listSV = nullptr;

        in.getline(lop->MALOP, 15, '|');
        in.getline(lop->TENLOP, 50, '|');

        int soSV;
        in >> soSV;
        in.ignore();

        for (int j = 0; j < soSV; ++j) {
            nodeSinhVien* svNode = new nodeSinhVien;
            svNode->next = nullptr;
            svNode->sv.dsDiemThi = nullptr;

            in.getline(svNode->sv.MASV, 10, '|');
            in.getline(svNode->sv.HO, 50, '|');
            in.getline(svNode->sv.TEN, 20, '|');
            in.getline(svNode->sv.PHAI, 4, '|');
            in.getline(svNode->sv.Password, 20);

            int soMon;
            in >> soMon;
            in.ignore();

            for (int m = 0; m < soMon; ++m) {
                nodeDiemThi* diemNode = new nodeDiemThi;
                diemNode->next = nullptr;
                diemNode->diem.baithi = new Baithi;

                in.getline(diemNode->diem.MAMH, 16, '|');
                in >> diemNode->diem.Diem;
                in.ignore();

                int soCau;
                in >> soCau;
                in.ignore();

                diemNode->diem.baithi->n = soCau;
                diemNode->diem.baithi->dsBaithichitiet = new BaiThiChiTiets[soCau];

                MonHoc* mon = TimMonHoc(dsMonHoc, diemNode->diem.MAMH);

                for (int c = 0; c < soCau; ++c) {
                    char idCH[16];
                    char buffer[4];

                    in.getline(idCH, 16, '|');       // mã câu hỏi
                    in.getline(buffer, sizeof(buffer)); // đáp án (có thể trống)

                    char ans = buffer[0] ? buffer[0] : '\0'; // '\0' nghĩa là chưa chọn

                    nodeCauhoi* cau = nullptr;
                    if (mon != nullptr) {
                        cau = TimCauHoiTrongCay(mon->treeCauHoi, idCH);
                    }

                    diemNode->diem.baithi->dsBaithichitiet[c].cauhoi = cau;
                    diemNode->diem.baithi->dsBaithichitiet[c].traloi = ans;
                }

                ThemDiemThiTheoThuTu(svNode->sv, diemNode);
            }

            ThemSinhVienTheoThuTu(lop->listSV, svNode);
        }
    }

    in.close();
    cout << "Load completed\n";
}

// ====== Đọc file lop.txt ======
// void DocDanhSachLop(DanhSachLop& dsLop, const char* filename) {
//     ifstream in(filename);
//     if (!in) return;

//     in >> dsLop.n;
//     in.ignore();

//     for (int i = 0; i < dsLop.n; ++i) {
//         dsLop.lop[i] = new Lop;
//         in.getline(dsLop.lop[i]->MALOP, 15, '|');
//         in.getline(dsLop.lop[i]->TENLOP, 50, '|');
//         int soSV;
//         in >> soSV;
//         in.ignore();

//         nodeSinhVien* tail = NULL;
//         for (int j = 0; j < soSV; ++j) {
//             SinhVien sv;
//             in.getline(sv.MASV, 10, '|');
//             in.getline(sv.HO, 50, '|');
//             in.getline(sv.TEN, 20, '|');
//             in.getline(sv.PHAI, 4, '|');
//             in.getline(sv.Password, 20);

//             int soMon;
//             in >> soMon;
//             in.ignore();

//             for (int j = 0; j < soMon; ++j) {
//                 nodeDiemThi* diemNode = new nodeDiemThi;
//                 in.getline(diemNode->diem.MAMH, 16, '|');
//                 in >> diemNode->diem.Diem;
//                 int soCau;
//                 in.ignore();
//                 in >> soCau;
//                 in.ignore();

//                 diemNode->diem.baithi = NULL;
//                 nodeBaithi* tailCauHoi = NULL;
//                 for (int k = 0; k < soCau; ++k) {
//                     nodeBaithi* cau = new nodeBaithi;
//                     in.getline(cau->btct.idcauhoi, 16, '|');
//                     in >> cau->btct.traloi;
//                     in.ignore();
//                     cau->next = NULL;
//                     if (tailCauHoi == NULL) {
//                         diemNode->diem.baithi = tailCauHoi = cau;
//                     } else {
//                         tailCauHoi->next = cau;
//                         tailCauHoi = cau;
//                     }
//                 }
//                 diemNode->next = sv.dsDiemThi;
//                 sv.dsDiemThi = diemNode;
//             }

//             ThemSinhVienTheoThuTu(dsLop.lop[i]->listSV, sv);
//         }
//     }
//     in.close();
//     cout<<"Load completed"<<endl;
// }

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
                out << d->diem.baithi->n << '\n';

                for (int i = 0; i < d->diem.baithi->n; i++) {
                    out << d->diem.baithi->dsBaithichitiet[i].cauhoi->data.Id << '|';
                    if(d->diem.baithi->dsBaithichitiet[i].traloi == '\0')
                        out<<'\n'; 
                    else out<< d->diem.baithi->dsBaithichitiet[i].traloi << '\n';
                }
            }
        }
    }
    out.close();
    cout << "Saved completed\n";
}
#endif