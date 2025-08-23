#ifndef CAU_HOI_H
#define CAU_HOI_H

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include "CTDL.h"
using namespace std;

// Forward declaration
nodeCauhoi* timCauHoiTheoId(nodeCauhoi* root, const char* id);


// Hàm kiểm tra dữ liệu câu hỏi có hợp lệ không
bool KiemTraHopLeCauHoi(const char* noiDung, const char* A, const char* B, const char* C, const char* D, char dapAn) {
    // Kiểm tra con trỏ NULL
    if (noiDung == NULL || A == NULL || B == NULL || C == NULL || D == NULL) {
        return false;
    }
    
    // Kiểm tra nội dung câu hỏi không rỗng
    if (strlen(noiDung) == 0) {
        return false;
    }
    
    // Kiểm tra các phương án không rỗng
    if (strlen(A) == 0 || strlen(B) == 0 || strlen(C) == 0 || strlen(D) == 0) {
        return false;
    }
    
    // Kiểm tra đáp án ∈ {A, B, C, D}
    char dapAnChuan = toupper(dapAn);
    if (dapAnChuan != 'A' && dapAnChuan != 'B' && dapAnChuan != 'C' && dapAnChuan != 'D') {
        return false;
    }
    
    return true;
}

// Hàm trích xuất MAMH từ ID câu hỏi
char* trichXuatMAMH(const char* id) {
    if (id == NULL) return NULL;
    
    static char maMon[16];
    int i = 0;
    
    // Trích xuất phần MAMH (trước khi gặp số)
    while (id[i] != '\0' && !isdigit(id[i])) {
        maMon[i] = id[i];
        i++;
    }
    maMon[i] = '\0';
    
    return maMon;
}

// Hàm tìm môn học theo MAMH
int timMonHocTheoMAMH(const ListMonHoc& dsMH, const char* maMon) {
    if (maMon == NULL) return -1;
    
    for (int i = 0; i < dsMH.n; i++) {
        if (strcmp(dsMH.list[i].MAMH, maMon) == 0) {
            return i; // Trả về index của môn học
        }
    }
    
    return -1; // Không tìm thấy
}

// Hàm kiểm tra trùng ID trong một cây câu hỏi (đệ quy)
bool KiemTraTrungIDTrongCay(nodeCauhoi* root, const char* id) {
    if (root == NULL) return false;
    
    // Kiểm tra node hiện tại
    if (strcmp(root->data.Id, id) == 0) {
        return true; // Tìm thấy ID trùng
    }
    
    // Tìm kiếm đệ quy trong cây con trái
    if (KiemTraTrungIDTrongCay(root->left, id)) {
        return true;
    }
    
    // Tìm kiếm đệ quy trong cây con phải
    if (KiemTraTrungIDTrongCay(root->right, id)) {
        return true;
    }
    
    return false; // Không tìm thấy ID trùng
}

// Hàm kiểm tra ID đã tồn tại trong toàn hệ thống chưa
bool KiemTraTrungID(const ListMonHoc& dsMH, const char* id) {
    if (id == NULL) return false;
    
    // Trích xuất MAMH từ ID
    char* maMon = trichXuatMAMH(id);
    if (maMon == NULL) return false;
    
    // Tìm môn học theo MAMH
    int indexMonHoc = timMonHocTheoMAMH(dsMH, maMon);
    if (indexMonHoc == -1) {
        return false; // Không tìm thấy môn học
    }
    
    // Kiểm tra trùng ID trong môn học này
    return KiemTraTrungIDTrongCay(dsMH.list[indexMonHoc].treeCauHoi, id);
}

// Sửa sinhID: không cần dsMH, giữ ngẫu nhiên
char* sinhID(const char* maMon, MonHoc &mh) {
    if (maMon == NULL || strlen(maMon) == 0) return NULL;
    static char idMoi[16];
    int attempts = 0;
    const int MAX_ATTEMPTS = 50; // Giới hạn thử vì n ≤ 1000
    int newNum;
    do {
        newNum = rand() % 1000; // Random 000-999
        sprintf(idMoi, "%s%03d", maMon, newNum); // Format 3 chữ số
        attempts++;
    } while (timCauHoiTheoId(mh.treeCauHoi, idMoi) != NULL && attempts < MAX_ATTEMPTS);
    if (attempts >= MAX_ATTEMPTS) return NULL; // Hết ID hoặc lỗi
    if (newNum > mh.maxID) mh.maxID = newNum; // Cập nhật maxID
    return idMoi;
}



// Dem so node trong cay (duyet in-order khong de quy)
inline int demNodeCauHoi(nodeCauhoi* root) {
    int count = 0;
    if (root == NULL) return 0;
    const int MAX_STACK = 8192;
    nodeCauhoi* stack[MAX_STACK]; int top = -1; nodeCauhoi* curr = root;
    while (curr != NULL || top >= 0) {
        while (curr != NULL) { if (top < MAX_STACK - 1) stack[++top] = curr; curr = curr->left; }
        curr = stack[top--];
        ++count;
        curr = curr->right;
    }
    return count;
}

// Tim ID lon nhat (phan so sau MAMH) trong cay
inline void inorderTimMax(nodeCauhoi* root, int &maxID) {
    if (root == NULL) return;
    const int MAX_STACK = 8192;
    nodeCauhoi* stack[MAX_STACK]; int top = -1; nodeCauhoi* curr = root;
    while (curr != NULL || top >= 0) {
        while (curr != NULL) { if (top < MAX_STACK - 1) stack[++top] = curr; curr = curr->left; }
        curr = stack[top--];
        // Tach phan so tu Id
        const char* id = curr->data.Id;
        int pos = 0; while (id[pos] && !isdigit(id[pos])) pos++;
        int num = 0; if (id[pos]) num = atoi(id + pos);
        if (num > maxID) maxID = num;
        curr = curr->right;
    }
}

// Sinh ID moi theo thu tu: lon nhat + 1
// Hàm này đã bị loại bỏ, thay thế bằng sinhID ngẫu nhiên
// inline char* sinhID_TuanTu(const char* maMon, nodeCauhoi* root) {
//     static char buffer[16];
//     if (maMon == NULL || strlen(maMon) == 0) return NULL;
//     int maxID = 0;
//     inorderTimMax(root, maxID);
//     sprintf(buffer, "%s%03d", maMon, maxID + 1);
//     return buffer;
// }

// Chuyen inorder sang mang (khong dung vector)
inline void inorderToArray(nodeCauhoi* root, CauHoi* outArr, int n) {
    if (root == NULL || outArr == NULL || n <= 0) return;
    const int MAX_STACK = 8192;
    nodeCauhoi* stack[MAX_STACK]; int top = -1; nodeCauhoi* curr = root; int idx = 0;
    while (curr != NULL || top >= 0) {
        while (curr != NULL) { if (top < MAX_STACK - 1) stack[++top] = curr; curr = curr->left; }
        curr = stack[top--];
        if (idx < n) outArr[idx++] = curr->data; else break;
        curr = curr->right;
    }
}

// Xay dung BST can bang tu mang CauHoi (da sap xep theo Id tang dan)
inline nodeCauhoi* buildBalancedBST(CauHoi* arr, int l, int r) {
    if (arr == NULL || l > r) return NULL;
    int mid = (l + r) / 2;
    nodeCauhoi* node = new nodeCauhoi;
    node->data = arr[mid];
    node->left = buildBalancedBST(arr, l, mid - 1);
    node->right = buildBalancedBST(arr, mid + 1, r);
    return node;
}

// Giai phong cay (hau tu)
inline void freeTree(nodeCauhoi* &root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
    root = NULL;
}

// Thêm rebuildTree: xóa sort dư thừa, dùng trung vị
void buildBalancedTree(nodeCauhoi* &root, nodeCauhoi** arr, int start, int end) {
    if (start > end) { root = NULL; return; }
    int mid = (start + end) / 2; // Trung vị
    root = arr[mid];
    root->left = NULL;
    root->right = NULL;
    buildBalancedTree(root->left, arr, start, mid - 1);
    buildBalancedTree(root->right, arr, mid + 1, end);
}

void rebuildTree(nodeCauhoi* &root) {
    // Sử dụng inorderToArray thay vì flattenCauHoi để tránh dependency
    int n = demNodeCauHoi(root);
    if (n <= 1) return;
    
    CauHoi* arr = new CauHoi[n];
    inorderToArray(root, arr, n);
    
    // Giải phóng cây cũ
    freeTree(root);
    
    // Xây dựng cây cân bằng từ mảng đã sorted
    root = buildBalancedBST(arr, 0, n - 1);
    
    // Giải phóng mảng tạm
    delete[] arr;
}

// Tinh chieu cao cay
inline int chieuCao(nodeCauhoi* root) {
    if (root == NULL) return 0;
    int h = 0;
    const int MAX_STACK = 8192;
    nodeCauhoi* stack[MAX_STACK]; int top = -1; nodeCauhoi* curr = root;
    int depthStack[MAX_STACK]; int dtop = -1; int depth = 0; nodeCauhoi* lastVisited = NULL;
    while (curr != NULL || top >= 0) {
        if (curr != NULL) {
            if (top < MAX_STACK - 1) { stack[++top] = curr; depthStack[++dtop] = depth; }
            curr = curr->left; depth++;
        } else {
            curr = stack[top]; int currDepth = depthStack[dtop];
            if (curr->right != NULL && lastVisited != curr->right) {
                curr = curr->right; depth = currDepth + 1; continue;
            }
            if (currDepth + 1 > h) h = currDepth + 1;
            lastVisited = curr; curr = NULL; top--; dtop--; depth = currDepth;
        }
    }
    return h;
}

// log2 integer (approx)
inline int ilog2_int(int n) {
    int r = 0; while (n > 1) { n >>= 1; r++; } return r;
}

// Hàm tạo một node câu hỏi mới
// Cấp phát động, gán dữ liệu
nodeCauhoi* taoNodeCauHoi(const char* id, const char* noiDung, const char* A, const char* B, const char* C, const char* D, char dapAn) {
    // Cấp phát động cho node mới
    nodeCauhoi* newNode = new nodeCauhoi;
    
    // Gán ID
    strncpy(newNode->data.Id, id, 15);
    newNode->data.Id[15] = '\0';
    
    // Gán nội dung câu hỏi
    newNode->data.NoiDung = string(noiDung);
    
    // Gán các phương án trả lời
    strncpy(newNode->data.A, A, 127);
    newNode->data.A[127] = '\0';
    
    strncpy(newNode->data.B, B, 127);
    newNode->data.B[127] = '\0';
    
    strncpy(newNode->data.C, C, 127);
    newNode->data.C[127] = '\0';
    
    strncpy(newNode->data.D, D, 127);
    newNode->data.D[127] = '\0';
    
    // Gán đáp án
    newNode->data.DapAn = dapAn;
    
    // Khởi tạo trạng thái (0: chưa sử dụng, 1: đã sử dụng)
    newNode->data.trangthai = 0;
    
    // Khởi tạo con trỏ left và right
    newNode->left = NULL;
    newNode->right = NULL;
    
    return newNode;
}

// Hàm tạo node câu hỏi với ID tự động (overload)
nodeCauhoi* taoNodeCauHoi(const char* maMon, const char* noiDung, const char* A, const char* B, const char* C, const char* D, char dapAn, const ListMonHoc& dsMH) {
    // Sinh ID duy nhất - cần tìm môn học đúng
    int indexMonHoc = timMonHocTheoMAMH(dsMH, maMon);
    if (indexMonHoc == -1) {
        cout << "Khong tim thay mon hoc co ma: " << maMon << endl;
        return NULL;
    }
    char* id = sinhID(maMon, const_cast<MonHoc&>(dsMH.list[indexMonHoc]));
    if (id == NULL) {
        cout << "Khong the sinh ID duy nhat cho cau hoi!\n";
        return NULL;
    }
    
    // Tạo node với ID đã sinh
    nodeCauhoi* newNode = taoNodeCauHoi(id, noiDung, A, B, C, D, dapAn);
    
    return newNode;
}

// Hàm tạo node câu hỏi từ struct CauHoi có sẵn (overload)
nodeCauhoi* taoNodeCauHoi(const CauHoi& cauHoi) {
    // Cấp phát động cho node mới
    nodeCauhoi* newNode = new nodeCauhoi;
    
    // Gán toàn bộ dữ liệu từ struct CauHoi
    newNode->data = cauHoi;
    
    // Khởi tạo con trỏ left và right
    newNode->left = NULL;
    newNode->right = NULL;
    
    return newNode;
}

// Hàm tìm câu hỏi trong cây theo Id
// Trả về con trỏ node hoặc NULL nếu không tìm thấy
nodeCauhoi* timCauHoiTheoId(nodeCauhoi* root, const char* id) {
    // Nếu cây rỗng hoặc ID không hợp lệ
    if (root == NULL || id == NULL) {
        return NULL;
    }
    
    // So sánh ID với node hiện tại
    int compareResult = strcmp(id, root->data.Id);
    
    if (compareResult == 0) {
        // Tìm thấy câu hỏi
        return root;
    } else if (compareResult < 0) {
        // ID cần tìm nhỏ hơn, tìm trong cây con trái
        return timCauHoiTheoId(root->left, id);
    } else {
        // ID cần tìm lớn hơn, tìm trong cây con phải
        return timCauHoiTheoId(root->right, id);
    }
}

// Hàm tìm câu hỏi trong toàn hệ thống theo Id
// Trả về con trỏ node hoặc NULL nếu không tìm thấy
nodeCauhoi* timCauHoiTheoIdTrongHeThong(const ListMonHoc& dsMH, const char* id) {
    if (id == NULL || dsMH.n == 0) {
        return NULL;
    }
    
    // Trích xuất MAMH từ ID
    char* maMon = trichXuatMAMH(id);
    if (maMon == NULL) {
        return NULL;
    }
    
    // Tìm môn học theo MAMH
    int indexMonHoc = timMonHocTheoMAMH(dsMH, maMon);
    if (indexMonHoc == -1) {
        return NULL; // Không tìm thấy môn học
    }
    
    // Tìm câu hỏi trong cây của môn học
    return timCauHoiTheoId(dsMH.list[indexMonHoc].treeCauHoi, id);
}

// Hàm chèn câu hỏi vào cây theo khóa Id
void ChenCauHoi(ListMonHoc& dsMH, const CauHoi& cauHoi) {
    // Kiểm tra đầu vào
    if (dsMH.n == 0) {
        cout << "Danh sach mon hoc rong! Khong the chen cau hoi.\n";
        return;
    }
    
    // Trích xuất MAMH từ Id câu hỏi
    char* maMon = trichXuatMAMH(cauHoi.Id);
    if (maMon == NULL) {
        cout << "Id cau hoi khong hop le! Khong the trich xuat ma mon hoc.\n";
        return;
    }
    
    // Tìm vị trí môn học trong danh sách
    int indexMonHoc = timMonHocTheoMAMH(dsMH, maMon);
    if (indexMonHoc == -1) {
        cout << "Khong tim thay mon hoc co ma: " << maMon << endl;
        cout << "Khong the chen cau hoi vao mon hoc khong ton tai.\n";
        return;
    }
    
    // Kiểm tra Id câu hỏi đã tồn tại chưa
    if (KiemTraTrungIDTrongCay(dsMH.list[indexMonHoc].treeCauHoi, cauHoi.Id)) {
        cout << "Id cau hoi " << cauHoi.Id << " da ton tai trong mon hoc " << maMon << "!\n";
        cout << "Khong the chen cau hoi trung lap.\n";
        return;
    }
    
    // Tạo node mới cho câu hỏi
    nodeCauhoi* newNode = taoNodeCauHoi(cauHoi);
    if (newNode == NULL) {
        cout << "Loi: Khong the tao node cau hoi!\n";
        return;
    }
    
    // Chèn vào cây câu hỏi của môn học
    if (dsMH.list[indexMonHoc].treeCauHoi == NULL) {
        // Cây rỗng, gán node mới làm gốc
        dsMH.list[indexMonHoc].treeCauHoi = newNode;
        cout << "Chen cau hoi thanh cong vao mon hoc " << maMon << "!\n";
        cout << "Id: " << cauHoi.Id << endl;
        cout << "Noi dung: " << cauHoi.NoiDung << endl;
        return;
    }
    
    // Cây không rỗng, tìm vị trí chèn phù hợp
    nodeCauhoi* current = dsMH.list[indexMonHoc].treeCauHoi;
    nodeCauhoi* parent = NULL;
    
    while (current != NULL) {
        parent = current;
        
        // So sánh Id dạng chuỗi
        int compareResult = strcmp(cauHoi.Id, current->data.Id);
        
        if (compareResult < 0) {
            // Id mới nhỏ hơn, đi sang cây con trái
            current = current->left;
        } else if (compareResult > 0) {
            // Id mới lớn hơn, đi sang cây con phải
            current = current->right;
        } else {
            // Id trùng nhau (không nên xảy ra do đã kiểm tra trước)
            cout << "Loi: Id cau hoi trung lap khi chen!\n";
            delete newNode;
            return;
        }
    }
    
    // Chèn node mới vào vị trí phù hợp
    if (strcmp(cauHoi.Id, parent->data.Id) < 0) {
        parent->left = newNode;
    } else {
        parent->right = newNode;
    }
    
    cout << "Chen cau hoi thanh cong vao mon hoc " << maMon << "!\n";
    cout << "Id: " << cauHoi.Id << endl;
    cout << "Noi dung: " << cauHoi.NoiDung << endl;
    cout << "Dap an: " << cauHoi.DapAn << endl;
}

// Hàm nhập dữ liệu cho câu hỏi mới và thêm vào cây
void NhapCauHoi(ListMonHoc& dsMH) {
    if (dsMH.n == 0) {
        cout << "Danh sach mon hoc rong! Khong the them cau hoi.\n";
        return;
    }
    
    cout << "\n=== NHAP CAU HOI MOI ===\n";
    
    // Bước 1: Chọn môn học
    cout << "Danh sach mon hoc hien co:\n";
    for (int i = 0; i < dsMH.n; i++) {
        cout << (i + 1) << ". " << dsMH.list[i].MAMH << " - " << dsMH.list[i].TENMH << endl;
    }
    
    int choice;
    cout << "Chon mon hoc (1-" << dsMH.n << "): ";
    cin >> choice;
    
    if (choice < 1 || choice > dsMH.n) {
        cout << "Lua chon khong hop le!\n";
        return;
    }
    
    const char* maMon = dsMH.list[choice - 1].MAMH;
    cout << "Ban da chon mon: " << maMon << " - " << dsMH.list[choice - 1].TENMH << endl;
    
    // Bước 2: Sinh ID tự động
    char* id = sinhID(maMon, dsMH.list[choice - 1]); // Sử dụng môn học đã chọn
    if (id == NULL) {
        cout << "Khong the sinh ID duy nhat cho cau hoi!\n";
        return;
    }
    
    cout << "ID duoc sinh: " << id << endl;
    
    // Bước 3: Nhập dữ liệu câu hỏi
    cin.ignore(); // Xóa buffer
    
    string noiDung;
    char A[128], B[128], C[128], D[128];
    char dapAn;
    
    cout << "\nNhap noi dung cau hoi: ";
    getline(cin, noiDung);
    
    cout << "Nhap phuong an A: ";
    cin.getline(A, 128);
    
    cout << "Nhap phuong an B: ";
    cin.getline(B, 128);
    
    cout << "Nhap phuong an C: ";
    cin.getline(C, 128);
    
    cout << "Nhap phuong an D: ";
    cin.getline(D, 128);
    
    do {
        cout << "Nhap dap an (A/B/C/D): ";
        cin >> dapAn;
        dapAn = toupper(dapAn);
        if (dapAn != 'A' && dapAn != 'B' && dapAn != 'C' && dapAn != 'D') {
            cout << "Dap an khong hop le! Hay nhap A, B, C hoac D.\n";
        }
    } while (dapAn != 'A' && dapAn != 'B' && dapAn != 'C' && dapAn != 'D');
    
    // Bước 4: Chuẩn hóa dữ liệu
    // Chuẩn hóa nội dung câu hỏi
    if (!noiDung.empty()) {
        // Xóa khoảng trắng đầu và cuối
        noiDung.erase(0, noiDung.find_first_not_of(" \t"));
        noiDung.erase(noiDung.find_last_not_of(" \t") + 1);
    }
    
    // Chuẩn hóa các phương án
    char* ptrA = A, *ptrB = B, *ptrC = C, *ptrD = D;
    
    // Xóa khoảng trắng đầu
    while (*ptrA == ' ' || *ptrA == '\t') ptrA++;
    while (*ptrB == ' ' || *ptrB == '\t') ptrB++;
    while (*ptrC == ' ' || *ptrC == '\t') ptrC++;
    while (*ptrD == ' ' || *ptrD == '\t') ptrD++;
    
    // Xóa khoảng trắng cuối
    int lenA = strlen(ptrA);
    while (lenA > 0 && (ptrA[lenA-1] == ' ' || ptrA[lenA-1] == '\t')) {
        ptrA[--lenA] = '\0';
    }
    
    int lenB = strlen(ptrB);
    while (lenB > 0 && (ptrB[lenB-1] == ' ' || ptrB[lenB-1] == '\t')) {
        ptrB[--lenB] = '\0';
    }
    
    int lenC = strlen(ptrC);
    while (lenC > 0 && (ptrC[lenC-1] == ' ' || ptrC[lenC-1] == '\t')) {
        ptrC[--lenC] = '\0';
    }
    
    int lenD = strlen(ptrD);
    while (lenD > 0 && (ptrD[lenD-1] == ' ' || ptrD[lenD-1] == '\t')) {
        ptrD[--lenD] = '\0';
    }
    
    // Bước 5: Kiểm tra hợp lệ
    if (!KiemTraHopLeCauHoi(noiDung.c_str(), ptrA, ptrB, ptrC, ptrD, dapAn)) {
        cout << "Du lieu cau hoi khong hop le! Kiem tra lai:\n";
        cout << "- Noi dung khong duoc rong\n";
        cout << "- Cac phuong an khong duoc rong\n";
        cout << "- Dap an phai la A, B, C hoac D\n";
        return;
    }
    
    // Bước 6: Tạo câu hỏi và thêm vào cây
    CauHoi cauHoiMoi;
    strcpy(cauHoiMoi.Id, id);
    cauHoiMoi.NoiDung = noiDung;
    strcpy(cauHoiMoi.A, ptrA);
    strcpy(cauHoiMoi.B, ptrB);
    strcpy(cauHoiMoi.C, ptrC);
    strcpy(cauHoiMoi.D, ptrD);
    cauHoiMoi.DapAn = dapAn;
    cauHoiMoi.trangthai = 0;
    
    // Thêm vào cây
    ChenCauHoi(dsMH, cauHoiMoi);
    
    cout << "\n=== NHAP CAU HOI THANH CONG ===\n";
    cout << "ID: " << cauHoiMoi.Id << endl;
    cout << "Noi dung: " << cauHoiMoi.NoiDung << endl;
    cout << "A. " << cauHoiMoi.A << endl;
    cout << "B. " << cauHoiMoi.B << endl;
    cout << "C. " << cauHoiMoi.C << endl;
    cout << "D. " << cauHoiMoi.D << endl;
    cout << "Dap an: " << cauHoiMoi.DapAn << endl;
}

// Hàm sửa nội dung/đáp án câu hỏi có Id cho trước
// Không được sửa Id
void SuaCauHoi(ListMonHoc& dsMH) {
    if (dsMH.n == 0) {
        cout << "Danh sach mon hoc rong! Khong co cau hoi nao de sua.\n";
        return;
    }
    
    cout << "\n=== SUA CAU HOI ===\n";
    
    // Nhập ID câu hỏi cần sửa
    char id[16];
    cout << "Nhap ID cau hoi can sua: ";
    cin >> id;
    
    // Tìm câu hỏi trong hệ thống
    nodeCauhoi* nodeCauHoi = timCauHoiTheoIdTrongHeThong(dsMH, id);
    if (nodeCauHoi == NULL) {
        cout << "Khong tim thay cau hoi co ID: " << id << endl;
        return;
    }
    
    // Hiển thị thông tin câu hỏi hiện tại
    cout << "\nThong tin cau hoi hien tai:\n";
    cout << "ID: " << nodeCauHoi->data.Id << " (khong the sua)\n";
    cout << "Noi dung: " << nodeCauHoi->data.NoiDung << endl;
    cout << "A. " << nodeCauHoi->data.A << endl;
    cout << "B. " << nodeCauHoi->data.B << endl;
    cout << "C. " << nodeCauHoi->data.C << endl;
    cout << "D. " << nodeCauHoi->data.D << endl;
    cout << "Dap an: " << nodeCauHoi->data.DapAn << endl;
    
    cout << "\n=== NHAP THONG TIN MOI ===\n";
    cout << "Nhan Enter de giu nguyen, nhap noi dung moi de thay doi\n";
    
    cin.ignore(); // Xóa buffer
    
    // Nhập nội dung mới
    string noiDungMoi;
    cout << "Noi dung moi (Enter de giu nguyen): ";
    getline(cin, noiDungMoi);
    
    // Nếu không nhập gì thì giữ nguyên
    if (!noiDungMoi.empty()) {
        // Chuẩn hóa nội dung mới
        noiDungMoi.erase(0, noiDungMoi.find_first_not_of(" \t"));
        noiDungMoi.erase(noiDungMoi.find_last_not_of(" \t") + 1);
        
        if (!noiDungMoi.empty()) {
            nodeCauHoi->data.NoiDung = noiDungMoi;
            cout << "Da cap nhat noi dung!\n";
        }
    }
    
    // Nhập phương án A mới
    char A[128];
    cout << "Phuong an A moi (Enter de giu nguyen): ";
    cin.getline(A, 128);
    
    if (strlen(A) > 0) {
        // Chuẩn hóa phương án A
        char* ptrA = A;
        while (*ptrA == ' ' || *ptrA == '\t') ptrA++;
        int lenA = strlen(ptrA);
        while (lenA > 0 && (ptrA[lenA-1] == ' ' || ptrA[lenA-1] == '\t')) {
            ptrA[--lenA] = '\0';
        }
        
        if (lenA > 0) {
            strcpy(nodeCauHoi->data.A, ptrA);
            cout << "Da cap nhat phuong an A!\n";
        }
    }
    
    // Nhập phương án B mới
    char B[128];
    cout << "Phuong an B moi (Enter de giu nguyen): ";
    cin.getline(B, 128);
    
    if (strlen(B) > 0) {
        // Chuẩn hóa phương án B
        char* ptrB = B;
        while (*ptrB == ' ' || *ptrB == '\t') ptrB++;
        int lenB = strlen(ptrB);
        while (lenB > 0 && (ptrB[lenB-1] == ' ' || ptrB[lenB-1] == '\t')) {
            ptrB[--lenB] = '\0';
        }
        
        if (lenB > 0) {
            strcpy(nodeCauHoi->data.B, ptrB);
            cout << "Da cap nhat phuong an B!\n";
        }
    }
    
    // Nhập phương án C mới
    char C[128];
    cout << "Phuong an C moi (Enter de giu nguyen): ";
    cin.getline(C, 128);
    
    if (strlen(C) > 0) {
        // Chuẩn hóa phương án C
        char* ptrC = C;
        while (*ptrC == ' ' || *ptrC == '\t') ptrC++;
        int lenC = strlen(ptrC);
        while (lenC > 0 && (ptrC[lenC-1] == ' ' || ptrC[lenC-1] == '\t')) {
            ptrC[--lenC] = '\0';
        }
        
        if (lenC > 0) {
            strcpy(nodeCauHoi->data.C, ptrC);
            cout << "Da cap nhat phuong an C!\n";
        }
    }
    
    // Nhập phương án D mới
    char D[128];
    cout << "Phuong an D moi (Enter de giu nguyen): ";
    cin.getline(D, 128);
    
    if (strlen(D) > 0) {
        // Chuẩn hóa phương án D
        char* ptrD = D;
        while (*ptrD == ' ' || *ptrD == '\t') ptrD++;
        int lenD = strlen(ptrD);
        while (lenD > 0 && (ptrD[lenD-1] == ' ' || ptrD[lenD-1] == '\t')) {
            ptrD[--lenD] = '\0';
        }
        
        if (lenD > 0) {
            strcpy(nodeCauHoi->data.D, ptrD);
            cout << "Da cap nhat phuong an D!\n";
        }
    }
    
    // Nhập đáp án mới
    char dapAnMoi;
    cout << "Dap an moi (Enter de giu nguyen, A/B/C/D de thay doi): ";
    cin >> dapAnMoi;
    
    if (dapAnMoi != '\n' && dapAnMoi != '\0') {
        dapAnMoi = toupper(dapAnMoi);
        if (dapAnMoi == 'A' || dapAnMoi == 'B' || dapAnMoi == 'C' || dapAnMoi == 'D') {
            nodeCauHoi->data.DapAn = dapAnMoi;
            cout << "Da cap nhat dap an!\n";
        } else {
            cout << "Dap an khong hop le! Giu nguyen dap an cu.\n";
        }
    }
    
    // Kiểm tra tính hợp lệ của câu hỏi sau khi sửa
    if (!KiemTraHopLeCauHoi(nodeCauHoi->data.NoiDung.c_str(), 
                             nodeCauHoi->data.A, 
                             nodeCauHoi->data.B, 
                             nodeCauHoi->data.C, 
                             nodeCauHoi->data.D, 
                             nodeCauHoi->data.DapAn)) {
        cout << "\nCanh bao: Cau hoi sau khi sua khong hop le!\n";
        cout << "Hay kiem tra lai noi dung va cac phuong an.\n";
        return;
    }
    
    // Hiển thị thông tin câu hỏi sau khi sửa
    cout << "\n=== SUA CAU HOI THANH CONG ===\n";
    cout << "ID: " << nodeCauHoi->data.Id << endl;
    cout << "Noi dung: " << nodeCauHoi->data.NoiDung << endl;
    cout << "A. " << nodeCauHoi->data.A << endl;
    cout << "B. " << nodeCauHoi->data.B << endl;
    cout << "C. " << nodeCauHoi->data.C << endl;
    cout << "D. " << nodeCauHoi->data.D << endl;
    cout << "Dap an: " << nodeCauHoi->data.DapAn << endl;
}

// Hàm tìm node nhỏ nhất trong cây (hỗ trợ)
nodeCauhoi* timNodeNhoNhat(nodeCauhoi* root) {
    if (root == NULL) {
        return NULL;
    }
    
    // Node nhỏ nhất là node ngoài cùng bên trái
    while (root->left != NULL) {
        root = root->left;
    }
    
    return root;
}
bool xoaNodeCauHoiKhoiCay(nodeCauhoi*& root, const char* id);

// Hàm xóa node cụ thể (hỗ trợ)
bool xoaNode(nodeCauhoi*& node) {
    if (node == NULL) {
        return false;
    }
    
    // Trường hợp 1: Node lá (không có con)
    if (node->left == NULL && node->right == NULL) {
        delete node;
        node = NULL;
        return true;
    }
    // Trường hợp 2: Node có 1 con
    else if (node->left == NULL) {
        nodeCauhoi* temp = node;
        node = node->right;
        delete temp;
        return true;
    } else if (node->right == NULL) {
        nodeCauhoi* temp = node;
        node = node->left;
        delete temp;
        return true;
    }
    // Trường hợp 3: Node có 2 con
    else {
        // Tìm node nhỏ nhất trong cây con phải (hoặc lớn nhất trong cây con trái)
        nodeCauhoi* temp = timNodeNhoNhat(node->right);
        
        // Sao chép dữ liệu từ node thay thế
        node->data = temp->data;
        
        // Xóa node thay thế
        return xoaNodeCauHoiKhoiCay(node->right, temp->data.Id);
    }
}
// Hàm xóa node câu hỏi khỏi cây (hỗ trợ)
bool xoaNodeCauHoiKhoiCay(nodeCauhoi*& root, const char* id) {
    if (root == NULL) {
        return false; // Không tìm thấy câu hỏi
    }
    
    // So sánh ID với node hiện tại
    int compareResult = strcmp(id, root->data.Id);
    
    if (compareResult < 0) {
        // ID cần xóa nhỏ hơn, tìm trong cây con trái
        return xoaNodeCauHoiKhoiCay(root->left, id);
    } else if (compareResult > 0) {
        // ID cần xóa lớn hơn, tìm trong cây con phải
        return xoaNodeCauHoiKhoiCay(root->right, id);
    } else {
        // Tìm thấy node cần xóa
        return xoaNode(root);
    }
}
// Hàm xóa câu hỏi theo Id
// Chỉ xóa được câu hỏi chưa thi (trangthai == 0)
void XoaCauHoi(ListMonHoc& dsMH) {
    if (dsMH.n == 0) {
        cout << "Danh sach mon hoc rong! Khong co cau hoi nao de xoa.\n";
        return;
    }
    
    cout << "\n=== XOA CAU HOI ===\n";
    
    // Nhập ID câu hỏi cần xóa
    char id[16];
    cout << "Nhap ID cau hoi can xoa: ";
    cin >> id;
    
    // Tìm câu hỏi trong hệ thống
    nodeCauhoi* nodeCauHoi = timCauHoiTheoIdTrongHeThong(dsMH, id);
    if (nodeCauHoi == NULL) {
        cout << "Khong tim thay cau hoi co ID: " << id << endl;
        return;
    }
    
    // Hiển thị thông tin câu hỏi cần xóa
    cout << "\nThong tin cau hoi can xoa:\n";
    cout << "ID: " << nodeCauHoi->data.Id << endl;
    cout << "Noi dung: " << nodeCauHoi->data.NoiDung << endl;
    cout << "A. " << nodeCauHoi->data.A << endl;
    cout << "B. " << nodeCauHoi->data.B << endl;
    cout << "C. " << nodeCauHoi->data.C << endl;
    cout << "D. " << nodeCauHoi->data.D << endl;
    cout << "Dap an: " << nodeCauHoi->data.DapAn << endl;
    cout << "Trang thai: " << (nodeCauHoi->data.trangthai == 0 ? "Chua thi" : "Da thi") << endl;
    
    // Kiểm tra trạng thái câu hỏi
    if (nodeCauHoi->data.trangthai == 1) {
        cout << "\nKHONG THE XOA CAU HOI NAY!\n";
        cout << "Ly do: Cau hoi da duoc su dung trong bai thi.\n";
        cout << "De bao ve tinh toan ven cua bai thi, khong the xoa cau hoi da thi.\n";
        return;
    }
    
    // Xác nhận xóa
    char xacNhan;
    cout << "\nBan co chac chan muon xoa cau hoi nay? (y/n): ";
    cin >> xacNhan;
    
    if (toupper(xacNhan) != 'Y') {
        cout << "Huy xoa cau hoi.\n";
        return;
    }
    
    // Tìm môn học chứa câu hỏi để xóa
    char* maMon = trichXuatMAMH(id);
    if (maMon == NULL) {
        cout << "Loi: Khong the trich xuat ma mon hoc!\n";
        return;
    }
    
    int indexMonHoc = timMonHocTheoMAMH(dsMH, maMon);
    if (indexMonHoc == -1) {
        cout << "Loi: Khong tim thay mon hoc!\n";
        return;
    }
    
    // Xóa câu hỏi khỏi cây
    if (!xoaNodeCauHoiKhoiCay(dsMH.list[indexMonHoc].treeCauHoi, id)) {
        cout << "Loi: Khong the xoa cau hoi khoi cay!\n";
        return;
    }
    
    // Cảnh báo nếu maxID gần đạt giới hạn
    if (dsMH.list[indexMonHoc].maxID >= 999) {
        cout << "\n=== CANH BAO ===\n";
        cout << "Mon hoc " << maMon << " da dat gan gioi han ID (999)!\n";
        cout << "Hay xem xet xoa bot cau hoi (trangthai=0) de tiep tuc them cau moi.\n";
    }
    
    cout << "\n=== XOA CAU HOI THANH CONG ===\n";
    cout << "Da xoa cau hoi co ID: " << id << endl;
    cout << "Thuoc mon: " << maMon << endl;
    cout << "Luu y: maxID khong giam khi xoa de tranh tai su dung ID.\n";
}

// Hàm lấy ngẫu nhiên câu hỏi cho thi trắc nghiệm
// Trả về mảng câu hỏi ngẫu nhiên, O(n) thời gian
void layCauHoiNgauNhien(const MonHoc& mh, CauHoi* outArr, int soCauHoi) {
    if (mh.treeCauHoi == NULL || outArr == NULL || soCauHoi <= 0) {
        return;
    }
    
    int tongCauHoi = demNodeCauHoi(mh.treeCauHoi);
    if (soCauHoi > tongCauHoi) {
        soCauHoi = tongCauHoi; // Không thể lấy nhiều hơn số câu hỏi có sẵn
    }
    
    // Tạo mảng tạm chứa tất cả câu hỏi
    CauHoi* tempArr = new CauHoi[tongCauHoi];
    inorderToArray(mh.treeCauHoi, tempArr, tongCauHoi);
    
    // Lấy ngẫu nhiên soCauHoi câu hỏi
    for (int i = 0; i < soCauHoi; i++) {
        int randomIndex = rand() % tongCauHoi;
        outArr[i] = tempArr[randomIndex];
        
        // Đổi chỗ để tránh lấy trùng
        if (randomIndex != tongCauHoi - 1) {
            tempArr[randomIndex] = tempArr[tongCauHoi - 1];
        }
        tongCauHoi--;
    }
    
    delete[] tempArr;
}

#endif
