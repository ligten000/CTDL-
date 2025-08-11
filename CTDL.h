#ifndef CTDL_H
#define CTDL_H
# include <iostream>
using namespace std;

struct CauHoi {
    char Id[16];
    string NoiDung;
    char A[128], B[128], C[128], D[128];
    char DapAn;
    int trangthai;
} ;

struct nodeCauhoi{
    CauHoi data;
    
    nodeCauhoi *left = NULL;
    nodeCauhoi *right = NULL;
};


struct MonHoc {
    char MAMH[16]; 
    char TENMH[50];
    nodeCauhoi * treeCauHoi = NULL;
};

// Môn học (Danh sách tuyến tính)
const int MAX_ListMH = 500;
struct ListMonHoc{
    int n = 0;
    MonHoc list[MAX_ListMH];
};

struct BaiThiChiTiets
{
    nodeCauhoi * cauhoi;
    char traloi;
};

struct Baithi{
    int n = 0;
    BaiThiChiTiets* dsBaithichitiet = NULL;
};


struct DiemThi {
    char MAMH[16];
    float Diem;
    Baithi *baithi = NULL;
};

// Điểm thi (Danh sách liên kết đơn)
struct nodeDiemThi{
    DiemThi diem;
    nodeDiemThi * next;
};

struct SinhVien {
    char MASV[10], HO[50], TEN[20], PHAI[4], Password[20];
    nodeDiemThi *dsDiemThi = NULL;

};

// Sinh viên (Danh sách liên kết đơn)
struct nodeSinhVien
{
   SinhVien sv;
   nodeSinhVien *next = NULL;
};


struct Lop {
    char MALOP[15], TENLOP[50];
    nodeSinhVien *listSV = NULL;
};

// Lớp (Mảng con trỏ)
const int Max_lop = 100;
struct DanhSachLop
{
    int n = 0;
    Lop* lop[Max_lop];
};

#endif