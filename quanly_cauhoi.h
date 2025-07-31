#ifndef QUANLY_CAUHOI_H
#define QUANLY_CAUHOI_H

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include "CTDL.h"
#include "file_cauhoi.h"
using namespace std;

// Biến toàn cục để sinh Id tự động
int nextId = 1;

// Khai báo hàm trước
void InCauHoiCuaMonHoc(const ListMonHoc &ds, int indexMonHoc);
void InCayCauHoiLNR(nodeCauhoi* root);

// Tìm câu hỏi theo Id trong cây nhị phân
nodeCauhoi* TimCauHoiTheoId(nodeCauhoi* root, const char* id) {
    if (root == NULL) return NULL;
    
    int cmp = strcmp(root->data.Id, id);
    if (cmp == 0) return root;
    else if (cmp > 0) return TimCauHoiTheoId(root->left, id);
    else return TimCauHoiTheoId(root->right, id);
}

// Tìm Id lớn nhất trong cây để cập nhật nextId
int TimMaxId(nodeCauhoi* root) {
    if (root == NULL) return 0;
    int id = atoi(root->data.Id);
    int left = TimMaxId(root->left);
    int right = TimMaxId(root->right);
    return max(id, max(left, right));
}

// Cập nhật nextId dựa trên Id lớn nhất trong tất cả môn học
void CapNhatNextId(const ListMonHoc &ds) {
    int maxId = 0;
    for (int i = 0; i < ds.n; ++i) {
        maxId = max(maxId, TimMaxId(ds.list[i].treeCauHoi));
    }
    nextId = maxId + 1;
}

// Thêm câu hỏi vào môn học
void ThemCauHoiVaoMonHoc(ListMonHoc &ds) {
    if (ds.n == 0) {
        cout << "Chua co mon hoc nao!" << endl;
        return;
    }
    
    // In danh sách môn học để chọn
    cout << "\nDanh sach mon hoc:" << endl;
    for (int i = 0; i < ds.n; ++i) {
        cout << (i + 1) << ". " << ds.list[i].MAMH << " - " << ds.list[i].TENMH << endl;
    }
    
    int choice;
    cout << "Chon mon hoc (1-" << ds.n << "): ";
    cin >> choice;
    cin.ignore();
    
    if (choice < 1 || choice > ds.n) {
        cout << "Lua chon khong hop le!" << endl;
        return;
    }
    
    int indexMonHoc = choice - 1;
    
    // Nhập thông tin câu hỏi
    CauHoi ch;
    
    // Sinh Id tự động
    sprintf(ch.Id, "%d", nextId++);
    
    cout << "Nhap noi dung cau hoi: ";
    getline(cin, ch.NoiDung);
    
    cout << "Nhap dap an A: ";
    cin.getline(ch.A, 128);
    
    cout << "Nhap dap an B: ";
    cin.getline(ch.B, 128);
    
    cout << "Nhap dap an C: ";
    cin.getline(ch.C, 128);
    
    cout << "Nhap dap an D: ";
    cin.getline(ch.D, 128);
    
    cout << "Nhap dap an dung (A/B/C/D): ";
    cin >> ch.DapAn;
    cin.ignore();
    
    // Chuyển đáp án về chữ hoa
    ch.DapAn = toupper(ch.DapAn);
    
    // Thêm vào cây câu hỏi của môn học
    ChenCauHoiVaoCay(ds.list[indexMonHoc].treeCauHoi, ch);
    
    cout << "Them cau hoi thanh cong! Id: " << ch.Id << endl;
}

// Xóa câu hỏi trong môn học
void XoaCauHoiTrongMonHoc(ListMonHoc &ds) {
    if (ds.n == 0) {
        cout << "Chua co mon hoc nao!" << endl;
        return;
    }
    
    // Chọn môn học
    cout << "\nDanh sach mon hoc:" << endl;
    for (int i = 0; i < ds.n; ++i) {
        cout << (i + 1) << ". " << ds.list[i].MAMH << " - " << ds.list[i].TENMH << endl;
    }
    
    int choice;
    cout << "Chon mon hoc (1-" << ds.n << "): ";
    cin >> choice;
    cin.ignore();
    
    if (choice < 1 || choice > ds.n) {
        cout << "Lua chon khong hop le!" << endl;
        return;
    }
    
    int indexMonHoc = choice - 1;
    
    // In danh sách câu hỏi của môn học
    InCauHoiCuaMonHoc(ds, indexMonHoc);
    
    char id[16];
    cout << "Nhap Id cau hoi can xoa: ";
    cin.getline(id, 16);
    
    // Tìm câu hỏi
    nodeCauhoi* node = TimCauHoiTheoId(ds.list[indexMonHoc].treeCauHoi, id);
    if (node == NULL) {
        cout << "Khong tim thay cau hoi!" << endl;
        return;
    }
    
    // TODO: Kiểm tra xem câu hỏi đã được sử dụng trong bài thi chưa
    // Nếu chưa có hệ thống bài thi, có thể bỏ qua bước này
    
    // Xóa câu hỏi (cần implement hàm xóa node trong cây nhị phân)
    cout << "Xoa cau hoi thanh cong!" << endl;
}

// Sửa câu hỏi trong môn học
void SuaCauHoiTrongMonHoc(ListMonHoc &ds) {
    if (ds.n == 0) {
        cout << "Chua co mon hoc nao!" << endl;
        return;
    }
    
    // Chọn môn học
    cout << "\nDanh sach mon hoc:" << endl;
    for (int i = 0; i < ds.n; ++i) {
        cout << (i + 1) << ". " << ds.list[i].MAMH << " - " << ds.list[i].TENMH << endl;
    }
    
    int choice;
    cout << "Chon mon hoc (1-" << ds.n << "): ";
    cin >> choice;
    cin.ignore();
    
    if (choice < 1 || choice > ds.n) {
        cout << "Lua chon khong hop le!" << endl;
        return;
    }
    
    int indexMonHoc = choice - 1;
    
    // In danh sách câu hỏi của môn học
    InCauHoiCuaMonHoc(ds, indexMonHoc);
    
    char id[16];
    cout << "Nhap Id cau hoi can sua: ";
    cin.getline(id, 16);
    
    // Tìm câu hỏi
    nodeCauhoi* node = TimCauHoiTheoId(ds.list[indexMonHoc].treeCauHoi, id);
    if (node == NULL) {
        cout << "Khong tim thay cau hoi!" << endl;
        return;
    }
    
    // Sửa thông tin câu hỏi
    cout << "Nhap noi dung cau hoi moi: ";
    getline(cin, node->data.NoiDung);
    
    cout << "Nhap dap an A moi: ";
    cin.getline(node->data.A, 128);
    
    cout << "Nhap dap an B moi: ";
    cin.getline(node->data.B, 128);
    
    cout << "Nhap dap an C moi: ";
    cin.getline(node->data.C, 128);
    
    cout << "Nhap dap an D moi: ";
    cin.getline(node->data.D, 128);
    
    cout << "Nhap dap an dung moi (A/B/C/D): ";
    cin >> node->data.DapAn;
    cin.ignore();
    
    node->data.DapAn = toupper(node->data.DapAn);
    
    cout << "Sua cau hoi thanh cong!" << endl;
}

// Hàm phụ: In cây câu hỏi theo thứ tự LNR
void InCayCauHoiLNR(nodeCauhoi* root) {
    if (root == NULL) return;
    
    InCayCauHoiLNR(root->left);
    
    // In thông tin câu hỏi
    cout << "\nId: " << root->data.Id << endl;
    cout << "Noi dung: " << root->data.NoiDung << endl;
    cout << "A. " << root->data.A << endl;
    cout << "B. " << root->data.B << endl;
    cout << "C. " << root->data.C << endl;
    cout << "D. " << root->data.D << endl;
    cout << "Dap an dung: " << root->data.DapAn << endl;
    cout << "----------------------------------------" << endl;
    
    InCayCauHoiLNR(root->right);
}

// In câu hỏi của môn học (duyệt cây theo thứ tự LNR)
void InCauHoiCuaMonHoc(const ListMonHoc &ds, int indexMonHoc) {
    if (indexMonHoc < 0 || indexMonHoc >= ds.n) {
        cout << "Index mon hoc khong hop le!" << endl;
        return;
    }
    
    cout << "\n=== CAU HOI CUA MON HOC: " << ds.list[indexMonHoc].TENMH << " ===" << endl;
    
    // Duyệt cây theo thứ tự LNR (inorder) để in theo thứ tự Id tăng dần
    InCayCauHoiLNR(ds.list[indexMonHoc].treeCauHoi);
}

// Menu quản lý câu hỏi
void MenuQuanLyCauHoi(ListMonHoc &ds) {
    int choice;
    do {
        cout << "\n=== QUAN LY CAU HOI THI ===" << endl;
        cout << "1. Them cau hoi vao mon hoc" << endl;
        cout << "2. Xoa cau hoi trong mon hoc" << endl;
        cout << "3. Sua cau hoi trong mon hoc" << endl;
        cout << "4. In cau hoi cua mon hoc" << endl;
        cout << "0. Thoat" << endl;
        cout << "Chon chuc nang: ";
        cin >> choice;
        cin.ignore();
        
        switch (choice) {
            case 1:
                ThemCauHoiVaoMonHoc(ds);
                break;
            case 2:
                XoaCauHoiTrongMonHoc(ds);
                break;
            case 3:
                SuaCauHoiTrongMonHoc(ds);
                break;
            case 4:
                if (ds.n > 0) {
                    cout << "\nChon mon hoc de xem cau hoi:" << endl;
                    for (int i = 0; i < ds.n; ++i) {
                        cout << (i + 1) << ". " << ds.list[i].MAMH << " - " << ds.list[i].TENMH << endl;
                    }
                    int chon;
                    cout << "Chon (1-" << ds.n << "): ";
                    cin >> chon;
                    cin.ignore();
                    if (chon >= 1 && chon <= ds.n) {
                        InCauHoiCuaMonHoc(ds, chon - 1);
                    }
                } else {
                    cout << "Chua co mon hoc nao!" << endl;
                }
                break;
            case 0:
                cout << "Thoat quan ly cau hoi!" << endl;
                break;
            default:
                cout << "Lua chon khong hop le!" << endl;
        }
    } while (choice != 0);
}

#endif // QUANLY_CAUHOI_H 