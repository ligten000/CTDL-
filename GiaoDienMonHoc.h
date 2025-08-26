#ifndef GIAODIENMONHOC_H
#define GIAODIENMONHOC_H

#include <iostream>
#include <iomanip>
#include <cstring>
#include "mylib.h"      // ensure <windows.h> loads before using namespace std in other headers
#include "CTDL.h"
#include "Mon_Hoc.h"    // contains validation helpers; avoid including ChuanHoa.h to prevent redefinition
#include "file_cauhoi.h"
#include "GiaoDienCauHoi.h"

using namespace std;

static const int MH_PAGE_SIZE = 15;
static const int MH_MOUSE = 20;

// Đếm số câu hỏi trong cây (duyệt in-order không đệ quy)
static int demSoCauHoi(nodeCauhoi *root) {
	int count = 0;
	if (root == NULL) return count;
	// sử dụng ngăn xếp thủ công nhỏ
	const int MAX_STACK = 1024;
	nodeCauhoi* stack[MAX_STACK];
	int top = -1;
	nodeCauhoi* curr = root;
	while (curr != NULL || top >= 0) {
		while (curr != NULL) {
			if (top < MAX_STACK - 1) stack[++top] = curr; else break;
			curr = curr->left;
		}
		if (top >= 0) {
			curr = stack[top--];
			count++;
			curr = curr->right;
		} else break;
	}
	return count;
}

static void xapxepDanhSachMonHoc(ListMonHoc &dsMH) {
	for (int i = 0; i < dsMH.n - 1; ++i) {
		int minIdx = i;
		for (int j = i + 1; j < dsMH.n; ++j) {
			if (strcmp(dsMH.list[j].MAMH, dsMH.list[minIdx].MAMH) < 0) minIdx = j;
		}
		if (minIdx != i) {
			MonHoc tmp = dsMH.list[i];
			dsMH.list[i] = dsMH.list[minIdx];
			dsMH.list[minIdx] = tmp;
		}
	}
}

static bool timMonHocTheoMa(const ListMonHoc &dsMH, const char* mamh, int &outIdx) {
	for (int i = 0; i < dsMH.n; ++i) {
		int cmp = strcmp(dsMH.list[i].MAMH, mamh);
		if (cmp == 0) { outIdx = i; return true; }
		if (cmp > 0) break;
	}
	return false;
}

static void themMonHocUI(ListMonHoc &dsMH) {
	Normal();
	gotoxy(0, MH_MOUSE);
	cout << "Nhap thong tin mon hoc";
	gotoxy(0, MH_MOUSE + 1); cout << "MAMH"; gotoxy(12, MH_MOUSE + 1); cout << ":";
	gotoxy(0, MH_MOUSE + 2); cout << "TENMH"; gotoxy(12, MH_MOUSE + 2); cout << ":";

	char mamh[16];
	char tenmh[50];

	while (true) {
		gotoxy(14, MH_MOUSE + 1);
		NhapMa(mamh, 16);
		chuanHoaMaMonHoc(mamh);
		if (strlen(mamh) == 0) { thongBaoLoi("Ma mon khong duoc rong", 14, MH_MOUSE + 1); continue; }
		if (!HopLeMAMH(mamh)) { thongBaoLoi("Ma mon khong hop le (3-5 ky tu A-Z)", 14, MH_MOUSE + 1); continue; }
		if (TonTaiMAMH(dsMH, mamh)) { thongBaoLoi("Ma mon da ton tai", 14, MH_MOUSE + 1); continue; }
		break;
	}

	while (true) {
		gotoxy(14, MH_MOUSE + 2);
		NhapChuoi(tenmh, 50);
		chuanHoaTenMonHoc(tenmh);
		if (!HopLeTenMon(tenmh)) { thongBaoLoi("Ten mon khong hop le", 14, MH_MOUSE + 2); continue; }
		break;
	}

	gotoxy(0, MH_MOUSE + 3);
	cout << "Ban co chac chan muon them mon hoc? (Y/N): ";
	char ch;
	while (true) {
		ch = toupper(getch());
		if (ch == 'N') return;
		if (ch == 'Y') break;
	}

	if (dsMH.n >= MAX_ListMH) { thongBaoLoi("Danh sach mon hoc da day", 0, MH_MOUSE + 4); return; }
	// chèn vào theo vị trí đã sắp xếp
	int pos = dsMH.n;
	for (int i = 0; i < dsMH.n; ++i) { if (strcmp(mamh, dsMH.list[i].MAMH) < 0) { pos = i; break; } }
	for (int j = dsMH.n; j > pos; --j) dsMH.list[j] = dsMH.list[j-1];
	strcpy(dsMH.list[pos].MAMH, mamh);
	strcpy(dsMH.list[pos].TENMH, tenmh);
	dsMH.list[pos].treeCauHoi = NULL;
	dsMH.list[pos].insertsSinceRebuild = 0;
	//dsMH.list[pos].maxID = 0; // Khởi tạo maxID = 0 cho môn học mới
	initIDPool(dsMH.list[pos]); // Khởi tạo pool ID cho môn học mới
	dsMH.n++;
	thongBaoLoi("Them mon hoc thanh cong", 0, MH_MOUSE + 4);
}

static void xoaMonHocUI(ListMonHoc &dsMH) {
	if (dsMH.n == 0) { thongBaoLoi("Danh sach mon hoc rong", 0, MH_MOUSE); return; }
	char mamh[16];
	int idx = -1;
	gotoxy(0, MH_MOUSE); cout << "Nhap ma mon muon xoa";
	gotoxy(0, MH_MOUSE + 1); cout << "MAMH:";
	while (true) {
		gotoxy(6, MH_MOUSE + 1);
		NhapMa(mamh, 16);
		chuanHoaMaMonHoc(mamh);
		if (strlen(mamh) == 0) { thongBaoLoi("Ma mon trong", 6, MH_MOUSE + 1); continue; }
		if (!timMonHocTheoMa(dsMH, mamh, idx)) { thongBaoLoi("Khong tim thay mon hoc", 6, MH_MOUSE + 1); continue; }
		if (dsMH.list[idx].treeCauHoi != NULL) {
			thongBaoLoi("Mon da co cau hoi, khong the xoa", 0, MH_MOUSE + 2);
			gotoxy(0, MH_MOUSE + 3); cout << "Ban co muon thuc hien lai thao tac? (Y/N): ";
			char cxy;
			while (true) {
				cxy = toupper(getch());
				if (cxy == 'Y') { for (int i = MH_MOUSE; i <= MH_MOUSE + 4; ++i) { gotoxy(0, i); clearCurrentLine(); } idx = -1; break; }
				if (cxy == 'N') { for (int i = MH_MOUSE; i <= MH_MOUSE + 4; ++i) { gotoxy(0, i); clearCurrentLine(); } return; }
			}
			gotoxy(0, MH_MOUSE); cout << "Nhap ma mon muon xoa";
			gotoxy(0, MH_MOUSE + 1); cout << "MAMH:";
			continue;
		}
		break;
	}
	gotoxy(0, MH_MOUSE + 3); cout << "Ban co chac chan muon xoa? (Y/N): ";
	char ch;
	while (true) { ch = toupper(getch()); if (ch == 'N') return; if (ch == 'Y') break; }
	// Giai phong cay cau hoi cua mon truoc khi xoa khoi danh sach
	freeTree(dsMH.list[idx].treeCauHoi);
	for (int j = idx; j < dsMH.n - 1; ++j) dsMH.list[j] = dsMH.list[j+1];
	dsMH.n--;
	thongBaoLoi("Xoa thanh cong", 0, MH_MOUSE + 4);
}

static void suaMonHocUI(ListMonHoc &dsMH, DanhSachLop &dsLop) {
	if (dsMH.n == 0) { thongBaoLoi("Danh sach mon hoc rong", 0, MH_MOUSE); return; }
	char mamh[16];
	int idx = -1;
	gotoxy(0, MH_MOUSE); cout << "Nhap ma mon can sua";
	gotoxy(0, MH_MOUSE + 1); cout << "MAMH:";
	while (true) {
		gotoxy(6, MH_MOUSE + 1);
		NhapMa(mamh, 16);
		chuanHoaMaMonHoc(mamh);
		if (strlen(mamh) == 0) { thongBaoLoi("Ma mon trong", 6, MH_MOUSE + 1); continue; }
		if (!timMonHocTheoMa(dsMH, mamh, idx)) { thongBaoLoi("Khong tim thay mon hoc", 6, MH_MOUSE + 1); continue; }
		break;
	}
	// Cho phep sua ca MAMH va TENMH, cap nhat cac lien ket phu thuoc
	char mamhMoi[16]; strcpy(mamhMoi, dsMH.list[idx].MAMH);
	char tenmh[50]; strcpy(tenmh, dsMH.list[idx].TENMH);
	gotoxy(0, MH_MOUSE + 2); cout << "MAMH moi:";
	while (true) {
		gotoxy(10, MH_MOUSE + 2);
		NhapMa(mamhMoi, 16);
		chuanHoaMaMonHoc(mamhMoi);
		if (strlen(mamhMoi) == 0) { strcpy(mamhMoi, dsMH.list[idx].MAMH); break; }
		if (!HopLeMAMH(mamhMoi)) { thongBaoLoi("Ma mon khong hop le (3-5 ky tu A-Z)", 10, MH_MOUSE + 2); continue; }
		int tmpIdx; if (strcmp(mamhMoi, dsMH.list[idx].MAMH)!=0 && timMonHocTheoMa(dsMH, mamhMoi, tmpIdx)) { thongBaoLoi("Ma mon da ton tai", 10, MH_MOUSE + 2); continue; }
		break;
	}
	gotoxy(0, MH_MOUSE + 3); cout << "TENMH:";
	while (true) {
		gotoxy(7, MH_MOUSE + 3);
		NhapChuoi(tenmh, 50);
		chuanHoaTenMonHoc(tenmh);
		if (!HopLeTenMon(tenmh)) { thongBaoLoi("Ten mon khong hop le", 7, MH_MOUSE + 3); continue; }
		break;
	}
	gotoxy(0, MH_MOUSE + 4); cout << "Luu thay doi? (Y/N): ";
	char ch;
	while (true) { ch = toupper(getch()); if (ch == 'N') return; if (ch == 'Y') break; }

	// Cap nhat TENMH
	strcpy(dsMH.list[idx].TENMH, tenmh);

	// Neu thay doi MAMH, cap nhat toan bo he thong lien quan
	if (strcmp(mamhMoi, dsMH.list[idx].MAMH) != 0) {
		char mamhCu[16]; strcpy(mamhCu, dsMH.list[idx].MAMH);
		// 1) Cap nhat MAMH trong danh sach mon hoc
		strcpy(dsMH.list[idx].MAMH, mamhMoi);
		// 2) Cap nhat MAMH trong danh sach diem thi cua tat ca sinh vien
		for (int iL = 0; iL < dsLop.n; ++iL) {
			if (dsLop.lop[iL] == NULL) continue;
			for (nodeSinhVien* pSV = dsLop.lop[iL]->listSV; pSV != NULL; pSV = pSV->next) {
				nodeDiemThi* pD = pSV->sv.dsDiemThi;
				for (; pD != NULL; pD = pD->next) {
					if (strcmp(pD->diem.MAMH, mamhCu) == 0) {
						strcpy(pD->diem.MAMH, mamhMoi);
					}
				}
			}
		}
		
		{
			nodeCauhoi* oldRoot = dsMH.list[idx].treeCauHoi;
			nodeCauhoi* newRoot = NULL;
			const int MAX_STACK = 4096;
			nodeCauhoi* stack[MAX_STACK]; int top = -1; nodeCauhoi* cur = oldRoot;
			while (cur != NULL || top >= 0) {
				while (cur != NULL) { if (top < MAX_STACK - 1) stack[++top] = cur; cur = cur->left; }
				cur = stack[top--];
				CauHoi ch = cur->data;
				char so[16] = {0};
				int len = (int)strlen(ch.Id);
				int pos = 0; while (ch.Id[pos] && !isdigit(ch.Id[pos])) pos++;
				if (pos < len) { strncpy(so, ch.Id + pos, sizeof(so)-1); }
				char idMoi[16]; idMoi[0] = '\0';
				sprintf(idMoi, "%s%s", mamhMoi, so);
				strncpy(ch.Id, idMoi, sizeof(ch.Id)-1);
				ch.Id[sizeof(ch.Id)-1] = '\0';
				ChenCauHoiTheoId(newRoot, ch, dsMH.list[idx]); // Truyền tham chiếu monHoc để cập nhật maxID
				cur = cur->right;
			}
			// Giai phong cay cu truoc khi gan moi
			freeTree(dsMH.list[idx].treeCauHoi);
			dsMH.list[idx].treeCauHoi = newRoot;
			rebuildIDPool(dsMH.list[idx]);
		}
		xapxepDanhSachMonHoc(dsMH);
	}

	thongBaoLoi("Sua thanh cong", 0, MH_MOUSE + 5);
}

inline bool hienthidanhsachMonHoc(ListMonHoc &dsMH, DanhSachLop &dsLop) {
	
	char a;
	int currentPage = 1;
	int start;
	int end;
	int numPage = (dsMH.n + MH_PAGE_SIZE - 1) / MH_PAGE_SIZE;

	// Khung tiêu đề
	
	while (true) {
		Normal(); system("cls");
		cout << char(218) << string(10, char(196)) << char(194)
	     << string(20, char(196)) << char(194)
	     << string(40, char(196)) << char(194)
	     << string(15, char(196)) << char(191) << endl;
		cout << char(179) << setw(10) << left << "STT"
			<< char(179) << setw(20) << left << "Ma Mon"
			<< char(179) << setw(40) << left << "Ten Mon"
			<< char(179) << setw(15) << left << "So cau"
			<< char(179) << endl;
		cout << char(195) << string(10, char(196)) << char(197)
			<< string(20, char(196)) << char(197)
			<< string(40, char(196)) << char(197)
			<< string(15, char(196)) << char(180) << endl;

		gotoxy(0, 18);
		cout << "A: Them | D: Xoa | E: Sua | B: Chon mon | S: Luu | Q: Thoat" << endl;
			start = (currentPage - 1) * MH_PAGE_SIZE;
			end = min(start + MH_PAGE_SIZE, dsMH.n);
			// clear vung du lieu
		
		gotoxy(0, 3);
		for (int i = start; i < end; ++i) {
			cout << char(179) << setw(10) << left << i + 1
			     << char(179) << setw(20) << left << dsMH.list[i].MAMH
			     << char(179) << setw(40) << left << dsMH.list[i].TENMH
			     << char(179) << setw(15) << left << demSoCauHoi(dsMH.list[i].treeCauHoi)
			     << char(179) << endl;
		}
		for (int i = end + 1; i <= start + MH_PAGE_SIZE; ++i) {
			cout << char(179) << setw(10) << left << i
			     << char(179) << setw(20) << left << ""
			     << char(179) << setw(40) << left << ""
			     << char(179) << setw(15) << left << ""
			     << char(179) << endl;
		}
		cout << char(192) << string(10, char(196)) << char(193)
		     << string(20, char(196)) << char(193)
		     << string(40, char(196)) << char(193)
		     << string(15, char(196)) << char(217) << endl;
		gotoxy(36, 18); cout << "(" << currentPage << "/" << max(1, numPage) << ")";
		gotoxy(0, 19); cout << "A: Them | D: Xoa | E: Sua | B: Chon mon | S: Luu | Q: Thoat" << endl;
		for (int i = MH_MOUSE; i < MH_MOUSE + 10; ++i) { gotoxy(0, i); clearCurrentLine(); }
		a = toupper(getch());
		if (a == -32 || a == 0) {
			a = getch();
			if (a == 75 && currentPage > 1) currentPage--;
			else if (a == 77 && currentPage < max(1, numPage)) currentPage++;
		} else {
			switch (a) {
				case 'A':
					themMonHocUI(dsMH); //xapxepDanhSachMonHoc(dsMH); 
					numPage = (dsMH.n + MH_PAGE_SIZE - 1) / MH_PAGE_SIZE; break;
				case 'D':
					xoaMonHocUI(dsMH); numPage = (dsMH.n + MH_PAGE_SIZE - 1) / MH_PAGE_SIZE; break;
				case 'E':
					suaMonHocUI(dsMH, dsLop); break;
				case 'B':
				{
					char mamh[16]; int idx=-1;
					while (true) {
						
						gotoxy(0, MH_MOUSE); cout << "Nhap ma mon de quan ly cau hoi:";
						gotoxy(0, MH_MOUSE+1); cout << "MAMH:"; gotoxy(6, MH_MOUSE+1);
						NhapMa(mamh, 16);
						chuanHoaMaMonHoc(mamh);
						if (strlen(mamh) == 0) {
							thongBaoLoi("Ma mon rong", 6, MH_MOUSE+1);
							gotoxy(0, MH_MOUSE+2); cout << "Nhan ESC de huy, phim bat ky de nhap lai";
							char c = getch();
							for (int i = MH_MOUSE; i < MH_MOUSE + 3; ++i) { gotoxy(0, i); clearCurrentLine(); }
							if (c == 27) break; else continue;
						}
						if (!timMonHocTheoMa(dsMH, mamh, idx)) {
							thongBaoLoi("Khong tim thay mon", 6, MH_MOUSE+1);
							gotoxy(0, MH_MOUSE+2); cout << "Nhan ESC de huy, phim bat ky de nhap lai";
							char c = getch();
							for (int i = MH_MOUSE; i < MH_MOUSE + 3; ++i) { gotoxy(0, i); clearCurrentLine(); }
							if (c == 27) break; else continue;
						}
						
						break;
					}
					hienthiDanhSachCauHoi(dsMH, dsMH.list[idx]);
					break;
				}
				case 'S':
					GhiFile(dsMH, "MonHoc.txt"); cout << "Luu thanh cong"; Sleep(1000); clearCurrentLine(); break;
				case 'Q':
					return true;
				default:
					break;
			}
		}
	}
	return false;
}

#endif

