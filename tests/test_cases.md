# Danh sách Test Cases Hệ thống Quản lý Biển số Xe

| ID | Nhóm | Dữ liệu / Thao tác | Kết quả mong đợi | Kết quả thực tế | Trạng thái |
| :--- | :--- | :--- | :--- | :--- | :---: |
| **TC01** | Normal | Đăng ký `59-B1-12345` | Đăng ký thành công (`true`), `size == 1` | Trả về `true`, `size == 1` | **PASS** |
| **TC02** | Normal | Tra cứu `59-B1-12345` | Tìm thấy bản ghi `59-B1-12345` | Trả về con trỏ tới `59-B1-12345` | **PASS** |
| **TC03** | Normal | Tra cứu `59-B1-99999` (chưa đăng ký) | Không tìm thấy (`nullptr`) | Trả về `nullptr` | **PASS** |
| **TC04** | Normal | `getCommuneCodesByProvince("59")` với 2 biển `B1`, 1 biển `C2` | Danh sách mã xã `{"B1", "C2"}` không trùng lặp | Trả về `{"B1", "C2"}` | **PASS** |
| **TC05** | Normal | `getPlatesByCommune("59", "B1")` | Trả về đúng 2 biển số thuộc `59-B1` | Trả về 2 biển số | **PASS** |
| **TC06** | Normal | Chuẩn hóa chữ thường `29-a1-12345` | Chuẩn hóa thành `29-A1-12345` và tra cứu được | Chuẩn hóa thành `29-A1-12345` | **PASS** |
| **TC07** | Normal | Khoảng trắng thừa `"  29-B2-12345  "` | Tự động cắt khoảng trắng và đăng ký thành công | Đăng ký và tra cứu thành công | **PASS** |
| **TC08** | Boundary | Tra cứu khi bảng băm rỗng | Trả về `nullptr`, không lỗi | Trả về `nullptr` | **PASS** |
| **TC09** | Boundary | Biển số biên dưới `00-A0-00000` | Đăng ký và tra cứu thành công | Thành công | **PASS** |
| **TC10** | Boundary | Biển số biên trên `99-ZZ-99999` | Đăng ký và tra cứu thành công | Thành công | **PASS** |
| **TC11** | Boundary | `getCommuneCodesByProvince("88")` (tỉnh không có dữ liệu) | Trả về `vector` rỗng | `vector` rỗng | **PASS** |
| **TC12** | Boundary | `getPlatesByCommune("99", "AA")` (xã không có dữ liệu) | Trả về `vector` rỗng | `vector` rỗng | **PASS** |
| **TC13** | Collision | Chèn 300 biển số vào bảng `M = 101` (`alpha = 2.97`) | Xử lý va chạm bằng DSLK đơn, tra cứu đủ 300 biển | `size == 300`, tra cứu chính xác | **PASS** |
| **TC14** | Duplicate | Đăng ký trùng `51-F1-00050` và `51-f1-00050` | Từ chối (`false`), không tăng `size` | Trả về `false`, `size` giữ nguyên | **PASS** |
| **TC15** | Invalid | Chuỗi rỗng `""` hoặc toàn khoảng trắng `"   "` | Từ chối (`false`) | Trả về `false` | **PASS** |
| **TC16** | Invalid | Thiếu chữ số seri `59-B1-1234` (10 ký tự) | Từ chối (`false`) | Trả về `false` | **PASS** |
| **TC17** | Invalid | Thừa chữ số seri `59-B1-123456` (12 ký tự) | Từ chối (`false`) | Trả về `false` | **PASS** |
| **TC18** | Invalid | Sai mã tỉnh chứa chữ `AB-B1-12345` | Từ chối (`false`) | Trả về `false` | **PASS** |
| **TC19** | Invalid | Thiếu dấu gạch ngang `59B112345` | Từ chối (`false`) | Trả về `false` | **PASS** |
| **TC20** | Invalid | Mã xã chứa ký tự đặc biệt `59-B!-12345` | Từ chối (`false`) | Trả về `false` | **PASS** |
| **TC21** | Invalid | Số seri chứa chữ cái `59-B1-12B45` | Từ chối (`false`) | Trả về `false` | **PASS** |
| **TC22** | Structure | `HashTable::remove("29-A1-12345")` | Xóa thành công, giảm `size`, các node còn lại giữ nguyên | Xóa đúng node, `size == 1` | **PASS** |
| **TC23** | File I/O | `FileIO::loadFromFile` với đường dẫn không tồn tại | Trả về `false`, không crash | Trả về `false` | **PASS** |
| **TC24** | File I/O | Lưu file và nạp lại (`saveToFile` -> `loadFromFile`) | Bảo toàn 100% các biển số đã lưu | Khớp toàn bộ bản ghi | **PASS** |
| **ML01** | Memory | Chèn 5.000 biển số rồi gọi `HashTable::clear()` | Giải phóng sạch `5.000 Node`, `size == 0`, `usedBuckets == 0` | Giải phóng sạch và tái sử dụng tốt | **PASS** |

## Hướng dẫn biên dịch và chạy kiểm thử tự động

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic -Isrc tests/test_bienso.cpp -o test_bienso.exe
.\test_bienso.exe

g++ -std=c++17 -Wall -Wextra -pedantic -O2 -Isrc tests/bench_bienso.cpp -o bench_bienso.exe
.\bench_bienso.exe
```
