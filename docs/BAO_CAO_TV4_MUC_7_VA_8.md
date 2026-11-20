# Báo Cáo Dự Án – Phần Của Thành Viên 4 (Kỹ Sư File I/O & UI Console)

> **Thành viên thực hiện:** Quốc Hữu (TV4 – `quochuu59-dot` / `huu_quoc`)  
> **Phạm vi phụ trách:** Mục 7 (Kết quả thực hiện và Minh họa giao diện) & Mục 8 (Bảng phân công và Đánh giá mức độ hoàn thành), cùng các module `src/io/FileIO.h`, `src/ui/Menu.h` và bộ dữ liệu mẫu `data/data_bienso.txt`.

---

## 7. KẾT QUẢ THỰC HIỆN VÀ MINH HỌA GIAO DIỆN

### 7.1. Xây dựng Module Quản lý Tệp tin (`src/io/FileIO.h`)
- **Chức năng đọc file (`loadFromFile`)**: Sử dụng thư viện `<fstream>` (`std::ifstream`) để mở file dữ liệu `data/data_bienso.txt`. Quá trình đọc duyệt qua từng dòng bằng `std::getline`, kết hợp hàm `trimCopy()` để lọc bỏ các khoảng trắng thừa ở hai đầu hoặc bỏ qua dòng trống. Dữ liệu sau đó được nạp trực tiếp vào tầng nghiệp vụ thông qua hàm `service.registerPlate(line)`. Các dòng sai định dạng hoặc biển số trùng lặp sẽ tự động bị bỏ qua một cách an toàn mà không làm gián đoạn quá trình nạp.
- **Chức năng ghi file (`saveToFile`)**: Sử dụng `std::ofstream` duyệt qua toàn bộ `101` bucket của bảng băm (`HashTable::TABLE_SIZE`) bằng hàm `table.bucketAt(i)`, lấy chuỗi chuẩn hóa của từng biển số xe (`current->data.toString()`) và ghi tuần tự ra tệp văn bản, đảm bảo dữ liệu được lưu trữ bền vững khi kết thúc chương trình.
- **Xây dựng bộ dữ liệu mẫu (`data/data_bienso.txt`)**: Đã thiết lập bộ dữ liệu mẫu gồm **50 biển số xe** hợp lệ phân bổ đa dạng trên nhiều tỉnh/thành phố (`29`, `30`, `43`, `50`, `51`, `59`, `60`, `61`, `63`, `65`, `68`, `71`, `72`, `75`, `79`, `84`, `92`, `99`), trong đó các tỉnh trọng điểm có nhiều mã xã khác nhau (ví dụ tỉnh `29` có `A1`, `B2`, `C3`, `D4`; tỉnh `51` có `F1`, `G2`, `H3`) để phục vụ kiểm thử đầy đủ cả 6 chức năng.

### 7.2. Xây dựng Giao diện Điều khiển Console (`src/ui/Menu.h`)
- **Tổ chức giao diện dòng lệnh (CLI)**: Hiển thị menu trực quan gồm đầy đủ 6 chức năng nghiệp vụ chính của hệ thống và lựa chọn thoát (`0`):
  1. Đăng ký biển số mới (`case 1`).
  2. Tra cứu biển số (`case 2`).
  3. Liệt kê danh sách mã xã theo tỉnh (`case 3` – kiểm tra mã tỉnh bằng `parseMaTinh` và gọi `service.getCommuneCodesByProvince`).
  4. Liệt kê danh sách biển số theo tỉnh và xã (`case 4` – kiểm tra mã tỉnh, mã xã bằng `parseMaTinh`, `parseMaXa` và gọi `service.getPlatesByCommune`).
  5. Nạp dữ liệu từ file `data/data_bienso.txt` (`case 5`).
  6. Lưu dữ liệu ra file `data/data_bienso.txt` (`case 6`).
- **Xử lý ngoại lệ nhập liệu (Input Validation)**:
  - Áp dụng kỹ thuật kiểm tra trạng thái luồng nhập `if (!(std::cin >> choice))`, kết hợp `std::cin.clear()` và `std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n')` để xử lý triệt để lỗi khi người dùng nhập chữ cái hoặc ký tự lạ thay vì số nguyên tại menu, ngăn chặn hoàn toàn tình trạng lặp vô tận (infinite loop).
  - Sau khi đọc `choice`, chương trình tiếp tục gọi `std::cin.ignore(..., '\n')` để dọn ký tự xuống dòng còn sót trong bộ đệm trước khi dùng `std::getline(std::cin, input)` ở từng chức năng con.

### 7.3. Minh họa kết quả chạy thực tế trên Console

#### a) Chức năng 5: Nạp dữ liệu từ file `data/data_bienso.txt`
```text
===== QUAN LY BIEN SO XE =====
1. Dang ky bien so
2. Tra cuu bien so
3. Danh sach ma xa theo tinh
4. Danh sach bien so theo xa
5. Nap du lieu tu file
6. Luu du lieu ra file
0. Thoat
Chon chuc nang: 5
Nap du lieu tu file thanh cong.
```

#### b) Chức năng 1: Đăng ký biển số mới (Hợp lệ, Trùng lặp và Sai định dạng)
```text
Chon chuc nang: 1
Nhap bien so (29-A1-12345): 29-k1-77777
Dang ky thanh cong.

Chon chuc nang: 1
Nhap bien so (29-A1-12345): 29-A1-12345
Du lieu sai hoac bien so da ton tai.

Chon chuc nang: 1
Nhap bien so (29-A1-12345): 29A1-12345
Du lieu sai hoac bien so da ton tai.
```

#### c) Chức năng 2: Tra cứu biển số (Hỗ trợ tự động chuẩn hóa chữ thường sang chữ hoa)
```text
Chon chuc nang: 2
Nhap bien so can tim: 29-k1-77777
Tim thay: 29-K1-77777

Chon chuc nang: 2
Nhap bien so can tim: 99-ZZ-00000
Khong tim thay bien so.
```

#### d) Chức năng 3: Liệt kê danh sách mã xã thuộc một tỉnh
```text
Chon chuc nang: 3
Nhap ma tinh (2 chu so, VD: 29): 29
Danh sach ma xa thuoc tinh 29 (5 xa):
 - A1
 - B2
 - C3
 - D4
 - K1

Chon chuc nang: 3
Nhap ma tinh (2 chu so, VD: 29): 15
Khong co ma xa nao thuoc tinh 15.
```

#### e) Chức năng 4: Liệt kê danh sách biển số theo tỉnh và xã
```text
Chon chuc nang: 4
Nhap ma tinh (2 chu so, VD: 29): 51
Nhap ma xa (2 ky tu chu/so, VD: A1): h3
Danh sach bien so thuoc 51-H3 (3 bien so):
 - 51-H3-22222
 - 51-H3-11111
 - 51-H3-98765
```

#### f) Chức năng 6 & Kiểm tra chống trôi lệnh khi nhập sai kiểu ở Menu
```text
Chon chuc nang: abc
Lua chon khong hop le.

Chon chuc nang: 6
Luu du lieu thanh cong.

Chon chuc nang: 0
Ket thuc chuong trinh.
```

### 7.4. Đánh giá kết quả tích hợp
- Module I/O (`FileIO.h`) và UI (`Menu.h`) đã hoạt động trơn tru, khớp nối chính xác với các cấu trúc dữ liệu (`LinkedList`, `HashTable`) và tầng xử lý nghiệp vụ (`LicensePlateService`).
- Các kịch bản kiểm thử đọc/ghi file mẫu 50 biển số, chuẩn hóa dữ liệu nhập và tương tác qua menu đều cho kết quả chính xác, ổn định và không xảy ra lỗi bộ đệm bàn phím.

---

## 8. BẢNG PHÂN CÔNG VÀ ĐÁNH GIÁ MỨC ĐỘ HOÀN THÀNH

| Thành viên | Vai trò | Module / Đầu việc chính | Tài liệu / Báo cáo phụ trách | Tỷ lệ đóng góp | Đánh giá hoàn thành |
| :---: | :--- | :--- | :--- | :---: | :---: |
| **1** | Nhóm trưởng kiêm Kiến trúc | Điều phối; thống nhất API; tích hợp; rà soát `BienSo.h`, `HashTable.h`; quản lý tiến độ | Mục 1 (Giới thiệu đề tài) & Mục 3 (Thiết kế giải pháp, kiến trúc) | **16.7%** | 100% (Hoàn thành tốt) |
| **2** | Kỹ sư Thuật toán Core | `LinkedList.h`; đăng ký và tra cứu (`registerPlate`, `searchPlate`); kiểm tra trùng; quản lý `Node` | Mục 2 (Phân tích bài toán, sơ đồ khối thuật toán Chèn/Tìm) | **16.7%** | 100% (Hoàn thành tốt) |
| **3** | Lập trình viên Thống kê | Hai hàm lọc theo tỉnh/xã trong `LicensePlateService.h`; loại trùng mã xã | Mục 4 (Cài đặt & Hiện thực, phân tích thuật toán duyệt bảng) | **16.7%** | 100% (Hoàn thành tốt) |
| **4** | Kỹ sư File I/O & UI | `FileIO.h`, `Menu.h`, bộ dữ liệu mẫu `data/data_bienso.txt`, kiểm tra nhập liệu | Mục 7 (Kết quả thực hiện & Giao diện) & Mục 8 (Bảng phân công) | **16.7%** | 100% (Hoàn thành tốt) |
| **5** | Kỹ sư QA/QC & Tối ưu | `tests/`; kiểm thử biên/không hợp lệ; kiểm tra memory leak; đo hiệu năng | Mục 5 (Kiểm thử và Đánh giá hiệu năng) | **16.6%** | 100% (Hoàn thành tốt) |
| **6** | Chuyên viên AI & Báo cáo | Nhật ký AI (`NHAT_KY_AI.md`); biên tập tổng hợp báo cáo; slide; kịch bản demo | Mục 6 (Khai thác và Sử dụng AI) & Biên tập tổng thể | **16.6%** | 100% (Hoàn thành tốt) |
| **Tổng** | **6 thành viên** | **Toàn bộ hệ thống Quản lý Biển số Xe (Đề tài 11)** | **Đầy đủ 8 mục Báo cáo + Source Code + Test Suite** | **100%** | **Hoàn thành 100%** |
