#ifndef GIAODIENCAUHOI_H
#define GIAODIENCAUHOI_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <cstring>
#include "CTDL.h"
#include "mylib.h"
#include "ChuanHoa.h"
#include "Cau_Hoi.h"
#include "file_cauhoi.h"

using namespace std;

static const int CH_PAGE_SIZE = 10;
static const int CH_MOUSE = 20;

// Duyệt cây in-order và lưu vào danh sách
static void flattenCauHoi(nodeCauhoi *root, vector<nodeCauhoi*> &out) {
	if (root == NULL) return;
	const int MAX_STACK = 4096;
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
			out.push_back(curr);
			curr = curr->right;
		} else break;
	}
}

static void themCauHoiUI(ListMonHoc &dsMH, MonHoc &mh) {
	Normal();
	gotoxy(0, CH_MOUSE);
	cout << "Nhap cau hoi moi";
	char A[128] = {0}, B[128] = {0}, C[128] = {0}, D[128] = {0};
	char dapAn = '\0';
	char noiDungBuf[256] = {0};

	// Noi dung
	while (true) {
		gotoxy(0, CH_MOUSE + 1); clearCurrentLine(); cout << "Noi dung:"; gotoxy(10, CH_MOUSE + 1);
		NhapChuoi(noiDungBuf, 255);
		chuanHoaNoiDungCauHoi(noiDungBuf);
		if (strlen(noiDungBuf) == 0) { thongBaoLoi("Noi dung rong", 10, CH_MOUSE + 1); continue; }
		break;
	}
	// Lua chon A-D
	while (true) { gotoxy(0, CH_MOUSE + 2); clearCurrentLine(); cout << "A:"; gotoxy(3, CH_MOUSE + 2); NhapChuoi(A, 127); chuanHoaNoiDungCauHoi(A); if (strlen(A)==0){ thongBaoLoi("Rong",3,CH_MOUSE+2); continue;} break; }
	while (true) { gotoxy(0, CH_MOUSE + 3); clearCurrentLine(); cout << "B:"; gotoxy(3, CH_MOUSE + 3); NhapChuoi(B, 127); chuanHoaNoiDungCauHoi(B); if (strlen(B)==0){ thongBaoLoi("Rong",3,CH_MOUSE+3); continue;} break; }
	while (true) { gotoxy(0, CH_MOUSE + 4); clearCurrentLine(); cout << "C:"; gotoxy(3, CH_MOUSE + 4); NhapChuoi(C, 127); chuanHoaNoiDungCauHoi(C); if (strlen(C)==0){ thongBaoLoi("Rong",3,CH_MOUSE+4); continue;} break; }
	while (true) { gotoxy(0, CH_MOUSE + 5); clearCurrentLine(); cout << "D:"; gotoxy(3, CH_MOUSE + 5); NhapChuoi(D, 127); chuanHoaNoiDungCauHoi(D); if (strlen(D)==0){ thongBaoLoi("Rong",3,CH_MOUSE+5); continue;} break; }
	while (true) {
		gotoxy(0, CH_MOUSE + 6); clearCurrentLine(); cout << "Dap an (A/B/C/D):"; gotoxy(18, CH_MOUSE + 6);
		dapAn = toupper(getch()); cout << dapAn;
		if (dapAn!='A' && dapAn!='B' && dapAn!='C' && dapAn!='D') { thongBaoLoi("Khong hop le", 18, CH_MOUSE + 6); continue; }
		break;
	}

	if (!KiemTraHopLeCauHoi(noiDungBuf, A, B, C, D, dapAn)) { thongBaoLoi("Cau hoi khong hop le", 0, CH_MOUSE + 7); return; }
	char *id = sinhID(mh.MAMH, dsMH);
	if (id == NULL) { thongBaoLoi("Khong sinh duoc ID", 0, CH_MOUSE + 7); return; }
	CauHoi ch{};
	strncpy(ch.Id, id, sizeof(ch.Id)-1);
	ch.NoiDung = string(noiDungBuf);
	strncpy(ch.A, A, sizeof(ch.A)-1);
	strncpy(ch.B, B, sizeof(ch.B)-1);
	strncpy(ch.C, C, sizeof(ch.C)-1);
	strncpy(ch.D, D, sizeof(ch.D)-1);
	ch.DapAn = dapAn;
	ch.trangthai = 0;
	ChenCauHoiTheoId(mh.treeCauHoi, ch);
	thongBaoLoi("Them cau hoi thanh cong", 0, CH_MOUSE + 8);
}

static void suaCauHoiUI(ListMonHoc &dsMH, MonHoc &mh) {
	if (mh.treeCauHoi == NULL) { thongBaoLoi("Mon chua co cau hoi", 0, CH_MOUSE); return; }
	char id[16];
	gotoxy(0, CH_MOUSE); cout << "Nhap ID cau hoi can sua:";
	gotoxy(0, CH_MOUSE + 1); cout << "ID:"; gotoxy(4, CH_MOUSE + 1); NhapMa(id, 16);
	if (strlen(id) == 0) { thongBaoLoi("ID rong", 4, CH_MOUSE + 1); return; }
	nodeCauhoi* node = timCauHoiTheoId(mh.treeCauHoi, id);
	if (node == NULL) { thongBaoLoi("Khong tim thay ID", 4, CH_MOUSE + 1); return; }
	// Hien thi va sua
	char A[128], B[128], C[128], D[128]; char noiDungBuf[256];
	strncpy(A, node->data.A, 127); A[127]='\0';
	strncpy(B, node->data.B, 127); B[127]='\0';
	strncpy(C, node->data.C, 127); C[127]='\0';
	strncpy(D, node->data.D, 127); D[127]='\0';
	strncpy(noiDungBuf, node->data.NoiDung.c_str(), 255); noiDungBuf[255]='\0';
	char dapAn = node->data.DapAn;

	gotoxy(0, CH_MOUSE + 2); cout << "ENTER de sua, de trong de giu nguyen";
	gotoxy(0, CH_MOUSE + 3); cout << "Noi dung:"; gotoxy(10, CH_MOUSE + 3); NhapChuoi(noiDungBuf, 255); chuanHoaNoiDungCauHoi(noiDungBuf);
	gotoxy(0, CH_MOUSE + 4); cout << "A:"; gotoxy(3, CH_MOUSE + 4); NhapChuoi(A, 127); chuanHoaNoiDungCauHoi(A);
	gotoxy(0, CH_MOUSE + 5); cout << "B:"; gotoxy(3, CH_MOUSE + 5); NhapChuoi(B, 127); chuanHoaNoiDungCauHoi(B);
	gotoxy(0, CH_MOUSE + 6); cout << "C:"; gotoxy(3, CH_MOUSE + 6); NhapChuoi(C, 127); chuanHoaNoiDungCauHoi(C);
	gotoxy(0, CH_MOUSE + 7); cout << "D:"; gotoxy(3, CH_MOUSE + 7); NhapChuoi(D, 127); chuanHoaNoiDungCauHoi(D);
	gotoxy(0, CH_MOUSE + 8); cout << "Dap an (A/B/C/D):"; gotoxy(18, CH_MOUSE + 8); {
		char c = toupper(getch()); if (c=='A'||c=='B'||c=='C'||c=='D') { dapAn=c; cout<<dapAn; }
	}
	if (!KiemTraHopLeCauHoi(noiDungBuf, A, B, C, D, dapAn)) { thongBaoLoi("Khong hop le", 0, CH_MOUSE + 9); return; }
	// Cap nhat
	node->data.NoiDung = string(noiDungBuf);
	strcpy(node->data.A, A);
	strcpy(node->data.B, B);
	strcpy(node->data.C, C);
	strcpy(node->data.D, D);
	node->data.DapAn = dapAn;
	thongBaoLoi("Sua thanh cong", 0, CH_MOUSE + 10);
}

static void xoaCauHoiUI(MonHoc &mh) {
	if (mh.treeCauHoi == NULL) { thongBaoLoi("Mon chua co cau hoi", 0, CH_MOUSE); return; }
	char id[16];
	gotoxy(0, CH_MOUSE); cout << "Nhap ID cau hoi muon xoa:";
	gotoxy(0, CH_MOUSE + 1); cout << "ID:"; gotoxy(4, CH_MOUSE + 1); NhapMa(id, 16);
	if (strlen(id) == 0) { thongBaoLoi("ID rong", 4, CH_MOUSE + 1); return; }
	nodeCauhoi* node = timCauHoiTheoId(mh.treeCauHoi, id);
	if (node == NULL) { thongBaoLoi("Khong tim thay ID", 4, CH_MOUSE + 1); return; }
	if (node->data.trangthai == 1) { thongBaoLoi("Cau hoi da thi, khong the xoa", 0, CH_MOUSE + 2); return; }
	gotoxy(0, CH_MOUSE + 3); cout << "Ban co chac muon xoa? (Y/N): ";
	char c; while (true) { c=toupper(getch()); if (c=='N') return; if (c=='Y') break; }
	if (!xoaNodeCauHoiKhoiCay(mh.treeCauHoi, id)) { thongBaoLoi("Xoa that bai", 0, CH_MOUSE + 4); return; }
	thongBaoLoi("Xoa thanh cong", 0, CH_MOUSE + 4);
}

inline bool hienthiDanhSachCauHoi(ListMonHoc &dsMH, MonHoc &mh) {
	Normal(); system("cls");
	char a;
	int currentPage = 1;
	int numPage = 1;
	while (true) {
		vector<nodeCauhoi*> rows; rows.reserve(256);
		flattenCauHoi(mh.treeCauHoi, rows);
		int total = static_cast<int>(rows.size());
		numPage = (total + CH_PAGE_SIZE - 1) / CH_PAGE_SIZE; if (numPage <= 0) numPage = 1;
		int start = (currentPage - 1) * CH_PAGE_SIZE;
		int end = min(start + CH_PAGE_SIZE, total);

		// Khung tieu de
		for (int i = 0; i < 20; ++i) { gotoxy(0, i); clearCurrentLine(); }
		gotoxy(0,0);
		cout << char(218) << string(10, char(196)) << char(194)
		     << string(15, char(196)) << char(194)
		     << string(40, char(196)) << char(194)
		     << string(7, char(196)) << char(194)
		     << string(10, char(196)) << char(191) << endl;
		cout << char(179) << setw(10) << left << "STT"
		     << char(179) << setw(15) << left << "ID"
		     << char(179) << setw(40) << left << "Noi dung"
		     << char(179) << setw(7) << left << "DapAn"
		     << char(179) << setw(10) << left << "TrangThai"
		     << char(179) << endl;
		cout << char(195) << string(10, char(196)) << char(197)
		     << string(15, char(196)) << char(197)
		     << string(40, char(196)) << char(197)
		     << string(7, char(196)) << char(197)
		     << string(10, char(196)) << char(180) << endl;

		gotoxy(0,3);
		for (int i = start; i < end; ++i) {
			const CauHoi &ch = rows[i]->data;
			cout << char(179) << setw(10) << left << i + 1
			     << char(179) << setw(15) << left << ch.Id
			     << char(179) << setw(40) << left << ch.NoiDung.substr(0,38)
			     << char(179) << setw(7) << left << ch.DapAn
			     << char(179) << setw(10) << left << (ch.trangthai==0?"Chua":"Da")
			     << char(179) << endl;
		}
		cout << char(192) << string(10, char(196)) << char(193)
		     << string(15, char(196)) << char(193)
		     << string(40, char(196)) << char(193)
		     << string(7, char(196)) << char(193)
		     << string(10, char(196)) << char(217) << endl;

		gotoxy(0, 18); cout << "A: Them | D: Xoa | E: Sua | S: Luu | <-/-> Trang | Q: Thoat";
		gotoxy(45, 18); cout << "(" << currentPage << "/" << numPage << ")";
		for (int i = CH_MOUSE; i < CH_MOUSE + 10; ++i) { gotoxy(0, i); clearCurrentLine(); }
		a = toupper(getch());
		if (a == -32 || a == 0) { a = getch(); if (a==75 && currentPage>1) currentPage--; else if (a==77 && currentPage<numPage) currentPage++; }
		else {
			switch (a) {
				case 'A': themCauHoiUI(dsMH, mh); break;
				case 'E': suaCauHoiUI(dsMH, mh); break;
				case 'D': xoaCauHoiUI(mh); break;
				case 'S': GhiFile(dsMH, "MonHoc.txt"); cout << "\nLuu thanh cong"; Sleep(800); break;
				case 'Q': return true;
				default: break;
			}
		}
	}
	return false;
}

#endif


