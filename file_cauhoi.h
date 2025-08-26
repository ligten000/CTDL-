#ifndef FILE_CAUHOI_H
#define FILE_CAUHOI_H

#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <unordered_set>
#include "CTDL.h"
#include "Cau_Hoi.h"

using namespace std;
inline void freeAllMonHoc(ListMonHoc &dsMH);
// Ghi 1 môn học
static inline void GhiMonHoc(ofstream &fo, const MonHoc &mh) {
	fo << "MONHOC|" << mh.MAMH << "|" << mh.TENMH << '\n';
}

// Ghi 1 cây câu hỏi theo NLR (preorder)
static void GhiCayCauHoi(ofstream &fo, nodeCauhoi *root) {
    if (root == NULL) return;
	const CauHoi &ch = root->data;
	fo << "CAUHOI|"
	   << ch.Id << "|" << ch.NoiDung << "|"
	   << ch.A << "|" << ch.B << "|" << ch.C << "|" << ch.D << "|"
	   << ch.DapAn << "|" << ch.trangthai << '\n';
    GhiCayCauHoi(fo, root->left);
    GhiCayCauHoi(fo, root->right);
}

// API ghi file theo định dạng mới
inline void GhiFile(const ListMonHoc &dsMH, const char* filename) {
    ofstream fo(filename);
    if (!fo.is_open()) return;
	for (int i = 0; i < dsMH.n; ++i) {
		GhiMonHoc(fo, dsMH.list[i]);
		GhiCayCauHoi(fo, dsMH.list[i].treeCauHoi);
	}
    fo.close();
}

// Tạo node mới từ CauHoi
static nodeCauhoi* TaoNode(const CauHoi &ch) {
	nodeCauhoi* p = new nodeCauhoi;
	p->data = ch;
	p->left = NULL;
	p->right = NULL;
	return p;
}

// Sửa ChenCauHoiTheoId: ngưỡng 100, cập nhật maxID
static void ChenCauHoiTheoId(nodeCauhoi* &root, const CauHoi &ch, MonHoc &mh) {
    if (root == NULL) { 
        root = TaoNode(ch); 
        mh.insertsSinceRebuild++;
    } else {
        int cmp = strcmp(ch.Id, root->data.Id);
        if (cmp < 0) ChenCauHoiTheoId(root->left, ch, mh);
        else if (cmp > 0) ChenCauHoiTheoId(root->right, ch, mh);
        //else return; // Trùng ID, bỏ qua
        mh.insertsSinceRebuild++;
    }
    
   
    // Kiểm tra và rebuild (ngưỡng 100)
    int n = demNodeCauHoi(root);
    int h = chieuCao(root);
    if (mh.insertsSinceRebuild >= 100 || h > ilog2_int(max(1, n)) + 2) {
        rebuildTree(root);
        mh.insertsSinceRebuild = 0;
    }
}

// Đọc dòng MONHOC và thêm vào danh sách
static void DocMonHoc(const string &line, ListMonHoc &dsMH) {
	// line: MONHOC|MAMH|TENMH
	stringstream ss(line);
	string prefix, mamh, tenmh;
	getline(ss, prefix, '|');
	getline(ss, mamh, '|');
	getline(ss, tenmh);
	if (dsMH.n >= MAX_ListMH) return;
	MonHoc &mh = dsMH.list[dsMH.n++];
	strncpy(mh.MAMH, mamh.c_str(), sizeof(mh.MAMH)-1); mh.MAMH[sizeof(mh.MAMH)-1] = '\0';
	strncpy(mh.TENMH, tenmh.c_str(), sizeof(mh.TENMH)-1); mh.TENMH[sizeof(mh.TENMH)-1] = '\0';
	mh.treeCauHoi = NULL;
	mh.insertsSinceRebuild = 0;
	//mh.maxID = 0; // Khởi tạo maxID = 0
}

// Đọc dòng CAUHOI và chèn vào treeCauHoi của môn hiện tại
static void DocCauHoi(const string &line, MonHoc &monHienTai) {
	// line: CAUHOI|ID|NOIDUNG|A|B|C|D|DAPAN|TRANGTHAI
	// Không cần kiểm tra NULL vì đã là tham chiếu
	stringstream ss(line);
	string prefix, id, nd, A, B, C, D, dapAnStr, ttStr;
	getline(ss, prefix, '|');
	getline(ss, id, '|');
	getline(ss, nd, '|');
	getline(ss, A, '|');
	getline(ss, B, '|');
	getline(ss, C, '|');
	getline(ss, D, '|');
	getline(ss, dapAnStr, '|');
	getline(ss, ttStr);
	if (id.empty()) return;
	
	
	CauHoi ch{};
	strncpy(ch.Id, id.c_str(), sizeof(ch.Id)-1); ch.Id[sizeof(ch.Id)-1] = '\0';
	ch.NoiDung = nd;
	strncpy(ch.A, A.c_str(), sizeof(ch.A)-1); ch.A[sizeof(ch.A)-1] = '\0';
	strncpy(ch.B, B.c_str(), sizeof(ch.B)-1); ch.B[sizeof(ch.B)-1] = '\0';
	strncpy(ch.C, C.c_str(), sizeof(ch.C)-1); ch.C[sizeof(ch.C)-1] = '\0';
	strncpy(ch.D, D.c_str(), sizeof(ch.D)-1); ch.D[sizeof(ch.D)-1] = '\0';
	ch.DapAn = dapAnStr.empty() ? 'A' : static_cast<char>(toupper(dapAnStr[0]));
	ch.trangthai = ttStr.empty() ? 0 : atoi(ttStr.c_str());
	ChenCauHoiTheoId(monHienTai.treeCauHoi, ch, monHienTai);
}

// API đọc file theo định dạng mới
inline void DocFile(ListMonHoc &dsMH, const char* filename) {
    ifstream fi(filename);
    if (!fi.is_open()) return;
	// Khởi tạo lại danh sách (không giải phóng bộ nhớ cũ ở đây)
	// dsMH.n = 0;
	freeAllMonHoc(dsMH);
    string line;
	MonHoc *monHienTai = NULL;
	//unordered_set<string> idSet; // theo dõi Id duy nhất
	while (getline(fi, line)) {
		if (line.rfind("MONHOC|", 0) == 0) {
			DocMonHoc(line, dsMH);
			monHienTai = &dsMH.list[dsMH.n - 1];
		} else if (line.rfind("CAUHOI|", 0) == 0) {
			DocCauHoi(line, *monHienTai);
		} else {
			// dòng trống hoặc không hợp lệ -> bỏ qua
		}
	}
    fi.close();
	 // Rebuild ID pool sau khi đọc file xong
    for (int i = 0; i < dsMH.n; i++) {
        rebuildIDPool(dsMH.list[i]);
    }
    
    cout<<"read file complete!"<<endl;
}
// Giải phóng toàn bộ cây câu hỏi của tất cả môn học và reset danh sách
inline void freeAllMonHoc(ListMonHoc &dsMH) {
    for (int i = 0; i < dsMH.n; i++) {
        freeTree(dsMH.list[i].treeCauHoi);
        dsMH.list[i].treeCauHoi = NULL;
    }
    dsMH.n = 0;
}

#endif // FILE_CAUHOI_H 