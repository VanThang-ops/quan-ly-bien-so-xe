# Thiết kế kiến trúc

## 1. Luồng phụ thuộc
`Menu -> LicensePlateService -> HashTable -> LinkedList/Node`

`FileIO -> LicensePlateService / HashTable`

`BienSo.h` định nghĩa dữ liệu và kiểm tra định dạng.

## 2. Trách nhiệm module
- `models/BienSo.h`: struct `BienSoXe`, chuẩn hóa và phân tích chuỗi biển số.
- `structures/LinkedList.h`: `Node` và thao tác danh sách liên kết đơn.
- `structures/HashTable.h`: mảng `Node* table[M]`, hàm băm DJB2, Separate Chaining, thêm/tìm/xóa.
- `services/LicensePlateService.h`: các nghiệp vụ đăng ký, tra cứu, lọc theo tỉnh/xã.
- `io/FileIO.h`: đọc/ghi TXT.
- `ui/Menu.h`: giao tiếp Console, nhận lựa chọn và gọi service.
- `main.cpp`: khởi tạo đối tượng và chạy menu.

## 3. Cấu trúc dữ liệu cốt lõi
Bảng băm dùng `Node* table[M]`. Mỗi bucket là đầu một danh sách liên kết đơn. Khi hai biển số có cùng chỉ số băm, node mới được nối vào danh sách tại bucket đó (Separate Chaining).

## 4. Hàm băm
Bộ khung dùng DJB2:
- Khởi tạo `hash = 5381`.
- Với mỗi ký tự: `hash = hash * 33 + ký_tự`.
- Chỉ số bucket: `hash % M`.
- `M` ban đầu là 101; nhóm cần giải thích đây là lựa chọn cấu hình ban đầu và có thể đánh giá lại qua kiểm thử.

## 5. Độ phức tạp dự kiến
Gọi `N` là số biển số, `M` là số bucket:
- Băm một khóa: O(L), với L là độ dài chuỗi biển số (ở đây độ dài cố định).
- Thêm/tìm/xóa: trung bình O(1 + alpha), với alpha = N/M; xấu nhất O(N) nếu dồn vào một bucket.
- Duyệt toàn bộ bảng: O(M + N).
- Bộ nhớ: O(M + N).

## 6. Quy tắc dữ liệu
Định dạng theo đề tài: `ab-cd-ghxyz`.
- `ab`: đúng 2 chữ số.
- `cd`: đúng 2 ký tự chữ hoặc số.
- `ghxyz`: đúng 5 chữ số.
- Mã xã được chuẩn hóa chữ hoa để tránh `a1` và `A1` bị xem là hai mã khác nhau.
- Khi lọc biển số theo xã phải so khớp cả mã tỉnh và mã xã.

## 7. Quyền sở hữu bộ nhớ
`HashTable` sở hữu các node trong mọi bucket và giải phóng chúng trong destructor/`clear()`. Không sao chép `HashTable` để tránh shallow copy và double-free; bộ khung đã vô hiệu hóa copy constructor và phép gán.
