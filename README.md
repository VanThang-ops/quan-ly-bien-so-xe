# Hệ thống quản lý biển số xe

Dự án học phần **Cấu trúc dữ liệu và Giải thuật** – Đề tài 11.

Hệ thống quản lý biển số xe bằng **Hash Table (Separate Chaining)** kết hợp **danh sách liên kết đơn**.

## 1. Công nghệ
- Ngôn ngữ: C++17
- Trình biên dịch: GCC / MinGW g++
- Giao diện: Console
- Dữ liệu: tệp TXT

## 2. Định dạng biển số
`ab-cd-ghxyz`, ví dụ: `29-A1-12345`
- `ab`: 2 chữ số mã tỉnh/thành phố.
- `cd`: 2 ký tự chữ hoặc số mã xã theo quy ước của đề tài.
- `ghxyz`: 5 chữ số seri.

> Nhóm cần thống nhất quy tắc chữ hoa/chữ thường và kiểm tra trùng trước khi triển khai. Bộ khung chuẩn hóa mã xã về chữ hoa.

## 3. Chức năng
1. Đăng ký biển số mới.
2. Tra cứu biển số.
3. Liệt kê mã xã thuộc một tỉnh.
4. Liệt kê biển số thuộc một xã của tỉnh.
5. Đọc/ghi dữ liệu `data/data_bienso.txt`.

## 4. Cấu trúc thư mục
```text
src/
  models/BienSo.h
  structures/LinkedList.h
  structures/HashTable.h
  services/LicensePlateService.h
  io/FileIO.h
  ui/Menu.h
  main.cpp
data/data_bienso.txt
tests/test_cases.md
docs/
```

## 5. Biên dịch và chạy
Từ thư mục gốc dự án, dùng GCC:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -Isrc src/main.cpp -o quan-ly-bien-so
```

Windows PowerShell:
```powershell
g++ -std=c++17 -Wall -Wextra -pedantic -Isrc src/main.cpp -o quan-ly-bien-so.exe
.\quan-ly-bien-so.exe
```

## 6. Quy ước làm việc nhóm
- Không push trực tiếp lên `main`.
- Mỗi thành viên làm trên branch riêng.
- Mọi thay đổi được gửi qua Pull Request (PR).
- Ít nhất một người khác kiểm tra trước khi merge.
- Không commit file thực thi, thư mục `.vscode/`, hoặc dữ liệu cá nhân.

Xem `docs/QUY_TRINH_GITHUB.md` và `docs/PHAN_CONG.md`.

## 7. Trạng thái
Đây là bộ khung ban đầu để nhóm thống nhất kiến trúc và giao diện module. Các thành viên cần đọc `docs/THIET_KE_KIEN_TRUC.md` trước khi bổ sung chức năng.
