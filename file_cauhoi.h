#ifndef FILE_CAUHOI_H
#define FILE_CAUHOI_H

#include <fstream>
#include <sstream>
#include <string>
#include "CTDL.h"
#include <cstring>

using namespace std;

// Ghi 1 cây câu hỏi theo thứ tự NLR (preorder)
void GhiCayCauHoi(ofstream &fo, nodeCauhoi *root) {
    if (root == NULL) return;
    // Ghi 1 câu hỏi trên 1 dòng
    fo << root->data.Id << "|" << root->data.NoiDung << "|"
       << root->data.A << "|" << root->data.B << "|" << root->data.C << "|" << root->data.D << "|" << root->data.DapAn << endl;
    GhiCayCauHoi(fo, root->left);
    GhiCayCauHoi(fo, root->right);
}

// Đếm số lượng node trong cây
int DemSoCauHoi(nodeCauhoi *root) {
    if (root == NULL) return 0;
    return 1 + DemSoCauHoi(root->left) + DemSoCauHoi(root->right);
}

// Ghi toàn bộ danh sách môn học và cây câu hỏi ra file TXT
void GhiFileDSMonHoc(const ListMonHoc &ds, const char* filename) {
    ofstream fo(filename);
    if (!fo.is_open()) return;
    fo << ds.n << endl;
    for (int i = 0; i < ds.n; ++i) {
        int socau = DemSoCauHoi(ds.list[i].treeCauHoi);
        fo << ds.list[i].MAMH << "|" << ds.list[i].TENMH << "|" << socau << endl;
        GhiCayCauHoi(fo, ds.list[i].treeCauHoi);
    }
    fo << "END" << endl;
    fo.close();
}

// Chèn 1 câu hỏi vào cây nhị phân tìm kiếm theo Id (dạng số nguyên)
void ChenCauHoiVaoCay(nodeCauhoi* &root, const CauHoi &ch) {
    if (root == NULL) {
        root = new nodeCauhoi{ch, true, NULL, NULL};
        return;
    }
    int id_new = atoi(ch.Id);
    int id_cur = atoi(root->data.Id);
    if (id_new < id_cur) ChenCauHoiVaoCay(root->left, ch);
    else ChenCauHoiVaoCay(root->right, ch);
}

// Đọc 1 cây câu hỏi từ file (số lượng node đã biết)
void DocCayCauHoi(ifstream &fi, nodeCauhoi* &root, int socau) {
    for (int i = 0; i < socau; ++i) {
        string line;
        if (!getline(fi, line)) break;
        stringstream ss(line);
        CauHoi ch;
        string tmp;
        getline(ss, tmp, '|'); strncpy(ch.Id, tmp.c_str(), 15); ch.Id[15] = '\0';
        getline(ss, ch.NoiDung, '|');
        getline(ss, tmp, '|'); strncpy(ch.A, tmp.c_str(), 127); ch.A[127] = '\0';
        getline(ss, tmp, '|'); strncpy(ch.B, tmp.c_str(), 127); ch.B[127] = '\0';
        getline(ss, tmp, '|'); strncpy(ch.C, tmp.c_str(), 127); ch.C[127] = '\0';
        getline(ss, tmp, '|'); strncpy(ch.D, tmp.c_str(), 127); ch.D[127] = '\0';
        getline(ss, tmp, '|'); ch.DapAn = tmp.empty() ? 'A' : tmp[0];
        ChenCauHoiVaoCay(root, ch);
    }
}

// Đọc toàn bộ danh sách môn học và cây câu hỏi từ file TXT
void DocFileDSMonHoc(ListMonHoc &ds, const char* filename) {
    ifstream fi(filename);
    if (!fi.is_open()) return;
    string line;
    getline(fi, line);
    ds.n = stoi(line);
    for (int i = 0; i < ds.n; ++i) {
        if (!getline(fi, line)) break;
        stringstream ss(line);
        string tmp;
        getline(ss, tmp, '|'); strncpy(ds.list[i].MAMH, tmp.c_str(), 15); ds.list[i].MAMH[15] = '\0';
        getline(ss, tmp, '|'); strncpy(ds.list[i].TENMH, tmp.c_str(), 49); ds.list[i].TENMH[49] = '\0';
        getline(ss, tmp, '|'); int socau = stoi(tmp);
        ds.list[i].treeCauHoi = NULL;
        DocCayCauHoi(fi, ds.list[i].treeCauHoi, socau);
    }
    // Đọc END
    getline(fi, line);
    fi.close();
}


#endif // FILE_CAUHOI_H 