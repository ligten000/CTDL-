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


// Hàm khởi tạo pool khi tạo môn học mới
void initIDPool(MonHoc &mh) {
    for (int i = 0; i < 1000; i++) mh.idPool[i] = i;
    mh.remaining = 1000;
}

// Hàm đánh dấu ID đã sử dụng trong cây
void markUsedIDs(nodeCauhoi* root, MonHoc &mh) {
    if (root == NULL) return;

    // Giả sử ID dạng "MAMH123"
    int lenMAMH = strlen(mh.MAMH);
    int num = atoi(root->data.Id + lenMAMH);

    // loại khỏi pool nếu tồn tại
    for (int i = 0; i < mh.remaining; i++) {
        if (mh.idPool[i] == num) {
            mh.idPool[i] = mh.idPool[mh.remaining - 1];
            mh.remaining--;
            break;
        }
    }

    markUsedIDs(root->left, mh);
    markUsedIDs(root->right, mh);
}

// Hàm rebuild pool sau khi load từ file
void rebuildIDPool(MonHoc &mh) {
    // reset lại pool
    for (int i = 0; i < 1000; i++) mh.idPool[i] = i;
    mh.remaining = 1000;

    // loại bỏ những số đã dùng trong cây câu hỏi
    markUsedIDs(mh.treeCauHoi, mh);
}

// Sửa sinhID: sử dụng Fisher-Yates với pool ID
char* sinhID(const char* maMon, MonHoc &mh) {
    if (maMon == NULL || strlen(maMon) == 0) return NULL;
    if (mh.remaining <= 0) return NULL; // hết ID

    int index = rand() % mh.remaining;
    int num = mh.idPool[index];

    // swap với phần tử cuối rồi giảm remaining
    mh.idPool[index] = mh.idPool[mh.remaining - 1];
    mh.remaining--;

    // ghép lại ID: MAMH + số (3 chữ số)
    static char buffer[16];
    sprintf(buffer, "%s%03d", maMon, num);

    return buffer;
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

// // Thêm rebuildTree: xóa sort dư thừa, dùng trung vị
// void buildBalancedTree(nodeCauhoi* &root, nodeCauhoi** arr, int start, int end) {
//     if (start > end) { root = NULL; return; }
//     int mid = (start + end) / 2; // Trung vị
//     root = arr[mid];
//     root->left = NULL;
//     root->right = NULL;
//     buildBalancedTree(root->left, arr, start, mid - 1);
//     buildBalancedTree(root->right, arr, mid + 1, end);
// }

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

// log2 integer 
inline int ilog2_int(int n) {
    int r = 0; while (n > 1) { n >>= 1; r++; } return r;
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



nodeCauhoi* timNodeTheoId(nodeCauhoi* root, const char* id) {
    if (root == NULL) {
        return NULL;
    }

    // So sánh ID với node hiện tại
    int compareResult = strcmp(id, root->data.Id);

    if (compareResult < 0) {
        // ID cần tìm nhỏ hơn, tìm trong cây con trái
        return timNodeTheoId(root->left, id);
    } else if (compareResult > 0) {
        // ID cần tìm lớn hơn, tìm trong cây con phải
        return timNodeTheoId(root->right, id);
    } else {
        // Tìm thấy node cần tìm
        return root;
    }
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


// Hoàn trả ID vào pool khi xóa câu hỏi
void returnIDtoPool(MonHoc &mh, const string &id) {
    int lenMAMH = strlen(mh.MAMH);
    int num = atoi(id.c_str() + lenMAMH);

    // đảm bảo còn slot
    if (mh.remaining < 1000) {
        mh.idPool[mh.remaining] = num;
        mh.remaining++;
    }
}

#endif
