# Nhật ký khai thác công cụ AI

**Đề tài 11:** Hệ thống Quản lý Biển số Xe · **Nhóm 5** · **Người tổng hợp:** Thành viên 6 (`karu638`)

---

## 1. Nguyên tắc sử dụng AI của nhóm
1. **Không sao chép mù quáng:** Đọc và hiểu từng dòng code do AI gợi ý trước khi đưa vào dự án; đối chiếu chặt chẽ với kiến trúc tại `docs/THIET_KE_KIEN_TRUC.md`.
2. **Luôn biên dịch và kiểm thử thực tế:** Mọi đoạn code AI sinh ra đều phải biên dịch qua `g++ -std=c++17 -Wall -Wextra -pedantic` và chạy qua bộ 25 Test Cases (`tests/test_bienso.cpp`).
3. **Tuân thủ hợp đồng tích hợp (`docs/THONG_NHAT_API.md`):** Không để AI tự ý đổi tên hàm, kiểu trả về hoặc tạo class mới lệch khỏi cấu trúc module của nhóm.
4. **Kiểm soát qua Pull Request:** Code có sự hỗ trợ của AI vẫn phải đẩy lên nhánh `feature/*` riêng của từng thành viên và được Nhóm trưởng (TV1) review trước khi merge vào `main`.

---

## 2. Bảng tổng hợp nhật ký sử dụng AI của 6 thành viên

| STT | Ngày | Thành viên | Công cụ AI | Mục đích sử dụng | Module liên quan | Kết luận |
| :---: | :---: | :--- | :---: | :--- | :--- | :--- |
| **1** | 13/11/2026 | **TV1** (Phan Văn Thắng) | Gemini / ChatGPT | Rà soát hàm kiểm tra định dạng `parseBienSo` và chuẩn hóa chuỗi biển số `ab-cd-ghxyz` | `src/models/BienSo.h` | **Dùng sau khi sửa** (Phát hiện và sửa lỗi độ dài `size() != 12` thành `11`) |
| **2** | 13/11/2026 | **TV1** (Phan Văn Thắng) | Gemini | Thiết kế hàm băm DJB2 với `TABLE_SIZE = 101` và các hàm đo độ phân tán bucket (`bucketSize`, `usedBuckets`, `maxBucketSize`) | `src/structures/HashTable.h` | **Dùng sau khi sửa** (Bổ sung `% TABLE_SIZE` và xóa Copy Constructor/Assignment theo RAII) |
| **3** | 15/11/2026 | **TV2** (Trầm Hoài Nghĩa) | ChatGPT / Claude | Cài đặt các thao tác danh sách liên kết đơn (`contains`, `insertFront`, `remove`, `clear`) và vẽ sơ đồ khối Flowchart Mục 2 | `src/structures/LinkedList.h` | **Dùng sau khi sửa** (Khôi phục hàm `print()` bị AI lược bỏ và kiểm tra con trỏ đầu `prev == nullptr`) |
| **4** | 14/11/2026 | **TV3** (Trần Võ Công Danh) | ChatGPT | Viết 2 hàm thống kê `getCommuneCodesByProvince` và `getPlatesByCommune` duyệt bảng băm $O(M + N)$ | `src/services/LicensePlateService.h` | **Dùng sau khi sửa** (Thay `std::unordered_set` do AI gợi ý bằng vòng lặp khử trùng trên `std::vector`) |
| **5** | 18/11/2026 | **TV4** (Quốc Hữu) | Gemini / ChatGPT | Xử lý chống trôi lệnh `std::cin` trong giao diện Menu Console, kết nối `case 3, 4` và sinh bộ 50 biển số mẫu | `src/ui/Menu.h`, `src/io/FileIO.h`, `data/data_bienso.txt` | **Dùng sau khi sửa** (Loại bỏ các dòng trùng lặp AI sinh ra và bổ sung nhiều mã xã cho cùng 1 tỉnh) |
| **6** | 14/11/2026 | **TV5** (Phan Nhật Phi) | Claude | Sinh chương trình kiểm thử tự động `test_bienso.cpp` (25 Test Cases) | `tests/test_bienso.cpp` | **Dùng sau khi sửa** (Xóa class giả lập của AI, `#include` trực tiếp `LicensePlateService.h` và sửa `TC06`, `TC20`) |
| **7** | 15/11/2026 | **TV5** (Phan Nhật Phi) | Claude / Gemini | Sinh chương trình đo hiệu năng `bench_bienso.cpp` với `<chrono>` và phân tích hệ số tải $\alpha = N/M$ | `tests/bench_bienso.cpp` | **Dùng sau khi sửa** (Sửa hàm sinh biển số đảm bảo không trùng lặp và sửa cờ biên dịch `-O2`) |
| **8** | 15/11/2026 | **TV6** (Chuyên viên AI & Báo cáo) | ChatGPT / Gemini | Thiết kế biểu mẫu thu thập AI Log, tổng hợp Mục 6 và chuẩn hóa định dạng báo cáo toàn nhóm | `docs/NHAT_KY_AI.md`, `docs/BAO_CAO_TV6_MUC_6.md` | **Dùng sau khi sửa** (Đối chiếu chéo với lịch sử commit và Pull Request thực tế trên GitHub) |

---

## 3. Phiếu chi tiết từng lần khai thác và kiểm chứng AI

### Phiếu số 1 – Thành viên 1 (Nhóm trưởng & Kiến trúc)
| Mục | Nội dung |
| :--- | :--- |
| **Ngày** | 13/11/2026 |
| **Thành viên** | Thành viên 1 – Phan Văn Thắng (`VanThang-ops`) |
| **Công cụ AI** | Gemini / ChatGPT |
| **Mục đích** | Viết và rà soát hàm `parseBienSo` kiểm tra định dạng biển số `ab-cd-ghxyz` kèm chuẩn hóa mã xã in hoa |
| **Module liên quan** | `src/models/BienSo.h` |
| **Prompt đã gửi** | *"Viết hàm C++17 `bool parseBienSo(const std::string& raw, BienSoXe& result)` kiểm tra chuỗi biển số định dạng `ab-cd-ghxyz` (ab: 2 chữ số mã tỉnh, cd: 2 ký tự chữ/số mã xã chuẩn hóa in hoa, ghxyz: 5 chữ số seri)."* |
| **Kết quả AI trả về** | AI sinh hàm cắt chuỗi `trimCopy` và kiểm tra điều kiện độ dài: `if (value.size() != 12 \|\| value[2] != '-' \|\| value[5] != '-') return false;` |
| **Cách kiểm tra** | Đếm trực tiếp số lượng ký tự của chuỗi mẫu `"29-A1-12345"` ($2 + 1 + 2 + 1 + 5 = 11$ ký tự) và chạy thử `registerPlate("29-A1-12345")`. |
| **Chỗ sai/thiếu và cách sửa** | **Lỗi Off-by-one nghiêm trọng:** AI tính nhầm tổng độ dài chuỗi `ab-cd-ghxyz` là `12` thay vì `11`, khiến mọi biển số hợp lệ đều bị trả về `false`. Đã sửa điều kiện thành `value.size() != 11` và tối ưu hàm sang so sánh ký tự ASCII cơ bản (`'0' <= c && c <= '9'`, `'a' <= c && c <= 'z'`). |
| **Kết luận** | **Dùng sau khi sửa** |

### Phiếu số 2 – Thành viên 1 (Nhóm trưởng & Kiến trúc)
| Mục | Nội dung |
| :--- | :--- |
| **Ngày** | 13/11/2026 |
| **Thành viên** | Thành viên 1 – Phan Văn Thắng (`VanThang-ops`) |
| **Công cụ AI** | Gemini |
| **Mục đích** | Cài đặt bảng băm `HashTable` theo phương pháp Separate Chaining với hàm băm DJB2 (`TABLE_SIZE = 101`) và các hàm đo thống kê bucket |
| **Module liên quan** | `src/structures/HashTable.h` |
| **Prompt đã gửi** | *"Cài đặt lớp `HashTable` C++17 quản lý mảng tĩnh `Node* table[101]`, dùng hàm băm DJB2 cho khóa `std::string`, hỗ trợ `insert`, `find`, `remove`, `loadFactor`, `usedBuckets`, `maxBucketSize`."* |
| **Kết quả AI trả về** | Hàm `hashFunction` tính `hash = ((hash << 5) + hash) + ch` nhưng thiếu phép chia lấy dư `% TABLE_SIZE` ở một bản nháp và chưa vô hiệu hóa Copy Constructor. |
| **Cách kiểm tra** | Kiểm tra giới hạn chỉ số mảng `0 .. 100` và kiểm tra quy tắc quản lý bộ nhớ động (Rule of Three/Five). |
| **Chỗ sai/thiếu và cách sửa** | Thêm phép toán `% TABLE_SIZE` ở cuối hàm băm DJB2; bổ sung `HashTable(const HashTable&) = delete;` và `HashTable& operator=(const HashTable&) = delete;` để ngăn lỗi sao chép nông (Shallow Copy) gây giải phóng bộ nhớ 2 lần (Double Free). |
| **Kết luận** | **Dùng sau khi sửa** |

### Phiếu số 3 – Thành viên 2 (Kỹ sư Thuật toán Core)
| Mục | Nội dung |
| :--- | :--- |
| **Ngày** | 15/11/2026 |
| **Thành viên** | Thành viên 2 – Trầm Hoài Nghĩa (`HOAINGHIA1`) |
| **Công cụ AI** | ChatGPT / Claude |
| **Mục đích** | Hoàn thiện các thao tác trên danh sách liên kết đơn (`LinkedList.h`) và thiết kế sơ đồ khối Flowchart cho `registerPlate` & `searchPlate` |
| **Module liên quan** | `src/structures/LinkedList.h`, `docs/BAO_CAO_TV2_MUC_2.md` |
| **Prompt đã gửi** | *"Tối ưu lại class `LinkedList` dùng vòng lặp `while` cho các hàm `contains`, `insertFront`, `remove`, `clear` nhận tham chiếu `Node*& head`."* |
| **Kết quả AI trả về** | AI viết lại 4 hàm bằng vòng lặp `while` gọn gàng nhưng tự ý xóa mất hàm `static void print(const Node* head)` và `#include <iostream>` của bộ khung. |
| **Cách kiểm tra** | So sánh `git diff` trên Pull Request #3 với file `docs/THONG_NHAT_API.md` và bộ khung gốc. |
| **Chỗ sai/thiếu và cách sửa** | Khôi phục lại đầy đủ hàm `LinkedList::print(const Node* head)` và kiểm tra kỹ nhánh xóa phần tử đầu danh sách (`if (prev == nullptr) head = current->next;`). |
| **Kết luận** | **Dùng sau khi sửa** |

### Phiếu số 4 – Thành viên 3 (Lập trình viên Thống kê)
| Mục | Nội dung |
| :--- | :--- |
| **Ngày** | 14/11/2026 |
| **Thành viên** | Thành viên 3 – Trần Võ Công Danh (`tranvocongdanhkt-glitch`) |
| **Công cụ AI** | ChatGPT |
| **Mục đích** | Cài đặt 2 hàm thống kê `getCommuneCodesByProvince` (lọc mã xã không trùng) và `getPlatesByCommune` (lọc biển số theo tỉnh + xã) |
| **Module liên quan** | `src/services/LicensePlateService.h` |
| **Prompt đã gửi** | *"Viết hàm `getCommuneCodesByProvince` và `getPlatesByCommune` duyệt qua 101 bucket của `HashTable` để lấy danh sách mã xã duy nhất thuộc 1 tỉnh và danh sách biển số thuộc 1 xã."* |
| **Kết quả AI trả về** | AI gợi ý `#include <unordered_set>` và dùng `std::unordered_set<std::string> seen` để kiểm tra trùng mã xã. |
| **Cách kiểm tra** | Rà soát lại yêu cầu học phần CTDL&GT (không phụ thuộc vào cấu trúc bảng băm có sẵn của STL). |
| **Chỗ sai/thiếu và cách sửa** | Loại bỏ `#include <unordered_set>`, thay bằng vòng lặp `for (const std::string& code : result)` kiểm tra trùng trực tiếp trên `std::vector<std::string> result`, đồng thời đảm bảo gọi `parseMaTinh` và `parseMaXa` để chuẩn hóa chữ hoa trước khi so sánh. |
| **Kết luận** | **Dùng sau khi sửa** |

### Phiếu số 5 – Thành viên 4 (Kỹ sư File I/O & UI Console)
| Mục | Nội dung |
| :--- | :--- |
| **Ngày** | 18/11/2026 |
| **Thành viên** | Thành viên 4 – Quốc Hữu (`quochuu59-dot`) |
| **Công cụ AI** | Gemini / ChatGPT |
| **Mục đích** | Xử lý chống lặp vô hạn khi nhập sai kiểu ở Menu Console, hoàn thiện `case 3, 4` và sinh bộ dữ liệu 50 biển số mẫu |
| **Module liên quan** | `src/ui/Menu.h`, `src/io/FileIO.h`, `data/data_bienso.txt` |
| **Prompt đã gửi** | *"Sinh 50 biển số xe mẫu định dạng `ab-cd-ghxyz` và viết giao diện `Menu.h` xử lý lỗi khi người dùng nhập chữ vào `std::cin >> choice`."* |
| **Kết quả AI trả về** | Bộ 50 biển số do AI sinh lần đầu bị lặp lại 4 dòng đầu tiên và mỗi tỉnh từ `50` đến `99` chỉ có đúng 1 mã xã duy nhất. |
| **Cách kiểm tra** | Chạy chức năng 5 (Nạp file) và chức năng 3 (Liệt kê mã xã theo tỉnh) trên Console để kiểm tra kết quả hiển thị. |
| **Chỗ sai/thiếu và cách sửa** | Sửa lại `data/data_bienso.txt` để loại bỏ 4 dòng trùng và phân bổ mỗi tỉnh trọng điểm (`29`, `30`, `43`, `51`, `59`, `65`) có từ 2–4 mã xã khác nhau; kết nối hoàn chỉnh `case 3` và `case 4` trong `Menu.h` vào 2 hàm thống kê của `LicensePlateService`. |
| **Kết luận** | **Dùng sau khi sửa** |

### Phiếu số 6 – Thành viên 5 (Kỹ sư QA/QC & Tối ưu)
| Mục | Nội dung |
| :--- | :--- |
| **Ngày** | 14/11/2026 – 15/11/2026 |
| **Thành viên** | Thành viên 5 – Phan Nhật Phi (`Phan Nhật Phi`) |
| **Công cụ AI** | Claude / Gemini |
| **Mục đích** | Xây dựng bộ kiểm thử tự động `tests/test_bienso.cpp` và chương trình đo hiệu năng `tests/bench_bienso.cpp` |
| **Module liên quan** | `tests/test_bienso.cpp`, `tests/bench_bienso.cpp`, `docs/BAO_CAO_TV5_MUC_5.md` |
| **Prompt đã gửi** | *"Viết file C++17 `test_bienso.cpp` kiểm thử tự động các trường hợp Normal, Boundary, Collision, Duplicate, Invalid, File I/O, Memory và file `bench_bienso.cpp` đo thời gian Insert/Search bằng `std::chrono`."* |
| **Kết quả AI trả về** | AI tự tạo một `class QuanLyBienSo` và `class Sut` giả lập ngay trong file test (dùng hàm băm nhân `31`, `M = 1009`) thay vì gọi code của nhóm, và ở `TC20` lại coi biển số chữ thường `"59-b1-12345"` là không hợp lệ (`Invalid`). |
| **Cách kiểm tra** | Nhóm trưởng và TV5 review code trên nhánh `feature/testing`, đối chiếu với `src/services/LicensePlateService.h` và quy tắc chuẩn hóa chữ hoa trong `src/models/BienSo.h`. |
| **Chỗ sai/thiếu và cách sửa** | Xóa bỏ hoàn toàn class `QuanLyBienSo` và `Sut` tự chế của AI; thay bằng `#include "../src/services/LicensePlateService.h"` và `"../src/io/FileIO.h"`; sửa `TC06` kiểm tra chuẩn hóa chữ thường `29-a1-12345 -> 29-A1-12345` và `TC20` kiểm tra ký tự đặc biệt `59-B!-12345`. Biên dịch và chạy thực tế đạt `25/25 PASS`. |
| **Kết luận** | **Dùng sau khi sửa** |
