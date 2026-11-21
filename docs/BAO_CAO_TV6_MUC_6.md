# Báo Cáo Dự Án – Phần Của Thành Viên 6 (Chuyên Viên AI & Biên Tập Báo Cáo)

> **Thành viên thực hiện:** Thành viên 6 (`karu638`)  
> **Phạm vi phụ trách:** Mục 6 (Khai thác và Sử dụng AI trong dự án), quản lý hồ sơ `docs/NHAT_KY_AI.md` và biên tập đồng bộ báo cáo toàn nhóm.

---

## 6. KHAI THÁC VÀ SỬ DỤNG CÔNG CỤ AI TRONG DỰ ÁN

### 6.1. Định hướng và nguyên tắc khai thác AI của nhóm
Trong quá trình thực hiện **Đề tài 11 – Hệ thống quản lý biển số xe**, nhóm sử dụng các công cụ Trí tuệ nhân tạo tạo sinh (**Gemini**, **ChatGPT**, **Claude**) với vai trò là **trợ lý gợi ý, đối chiếu giải thuật và rà soát lỗi**, tuyệt đối không để AI làm thay hoàn toàn hoặc phá vỡ kiến trúc tự cài đặt của nhóm.

Cả 6 thành viên tuân thủ quy trình 4 bước khi làm việc với AI:
1. **Xác định rõ hợp đồng API trước khi hỏi AI:** Mọi prompt gửi cho AI đều phải bám sát chữ ký hàm đã khóa tại `docs/THONG_NHAT_API.md` và cấu trúc `HashTable` (`TABLE_SIZE = 101`, DJB2, Separate Chaining) tại `docs/THIET_KE_KIEN_TRUC.md`.
2. **Đọc hiểu và kiểm chứng độc lập:** Mọi đoạn code AI phản hồi đều được thành viên phụ trách đọc kiểm tra từng dòng, biên dịch với cờ nghiêm ngặt `g++ -std=c++17 -Wall -Wextra -pedantic`.
3. **Bắt lỗi và tinh chỉnh (Verification & Refinement):** Ghi nhận lại toàn bộ các trường hợp AI sinh code sai logic, sai độ dài chuỗi hoặc lệch kiến trúc để tự tay sửa lại.
4. **Review chéo qua Pull Request trên GitHub:** Mọi nhánh `feature/*` đều được Nhóm trưởng (TV1) kiểm tra chéo trước khi merge vào `main`.

---

### 6.2. Bảng tổng hợp quá trình sử dụng AI của 6 thành viên

| STT | Thành viên | Công cụ AI | Mục đích sử dụng | Kết quả AI sinh ra | Cách kiểm tra & chỉnh sửa của sinh viên |
| :---: | :--- | :---: | :--- | :--- | :--- |
| **1** | **TV1** (Phan Văn Thắng) | Gemini, ChatGPT | Viết hàm `parseBienSo` kiểm tra định dạng `ab-cd-ghxyz` và thiết kế bảng băm `HashTable.h` (DJB2, `M = 101`) | AI sinh điều kiện kiểm tra độ dài chuỗi `value.size() != 12` và chưa khóa Copy Constructor của `HashTable`. | Đếm thực tế chuỗi `"29-A1-12345"` chỉ có **11 ký tự** ($2+1+2+1+5$), sửa `12` thành `11`; bổ sung `= delete` cho Copy Constructor/Assignment để chống lỗi Double Free. |
| **2** | **TV2** (Trầm Hoài Nghĩa) | ChatGPT, Claude | Cài đặt các hàm `LinkedList` (`contains`, `insertFront`, `remove`, `clear`) và vẽ Flowchart Mục 2 | AI viết lại các hàm bằng vòng lặp `while` nhưng tự ý lược bỏ hàm `LinkedList::print()` có sẵn trong bộ khung. | So sánh `git diff` trên Pull Request với `THONG_NHAT_API.md`, khôi phục đầy đủ hàm `print()` và kiểm tra kỹ thao tác xóa nút đầu bucket (`prev == nullptr`). |
| **3** | **TV3** (Trần Võ Công Danh) | ChatGPT | Viết 2 hàm thống kê `getCommuneCodesByProvince` và `getPlatesByCommune` trong `LicensePlateService.h` | AI gợi ý dùng `std::unordered_set<std::string>` của thư viện chuẩn STL để lọc trùng mã xã. | Nhận thấy học phần yêu cầu tự cài đặt cấu trúc dữ liệu, đã loại bỏ `#include <unordered_set>` và thay bằng vòng lặp kiểm tra trùng trực tiếp trên `std::vector<std::string> result`. |
| **4** | **TV4** (Quốc Hữu) | Gemini, ChatGPT | Xử lý chống lặp vô hạn khi nhập `std::cin` ở `Menu.h`, nối `case 3, 4` và tạo 50 biển số mẫu trong `data_bienso.txt` | Bộ dữ liệu 50 biển số AI sinh ra bị trùng lặp 4 dòng đầu và mỗi tỉnh chỉ có đúng 1 mã xã duy nhất. | Loại bỏ các dòng trùng lặp, thiết kế lại dữ liệu để mỗi tỉnh trọng điểm (`29, 30, 43, 51, 65`) có 2–4 mã xã khác nhau; kết nối `case 3, 4` gọi trực tiếp 2 hàm thống kê của TV3. |
| **5** | **TV5** (Phan Nhật Phi) | Claude, Gemini | Viết chương trình kiểm thử tự động `test_bienso.cpp` và đo hiệu năng `bench_bienso.cpp` | AI tự khai báo `class QuanLyBienSo` mới bên trong file test (dùng hàm băm `* 31`, `M = 1009`) và coi mã xã chữ thường ở `TC20` là không hợp lệ. | Xóa toàn bộ class giả lập của AI, sửa code để `#include "../src/services/LicensePlateService.h"` và `"../src/io/FileIO.h"`, sửa `TC06` và `TC20` theo đúng chuẩn hóa chữ hoa, chạy đạt `25/25 PASS`. |
| **6** | **TV6** (`karu638`) | ChatGPT, Gemini | Thiết kế mẫu `NHAT_KY_AI.md`, tổng hợp lỗi AI toàn nhóm và đồng bộ thuật ngữ 8 mục báo cáo | AI gợi ý mẫu bảng nhật ký chung nhưng chưa có cột ghi nhận lỗi sai cụ thể của AI. | Thiết kế bổ sung 2 mục bắt buộc trong `NHAT_KY_AI.md`: **"Cách kiểm tra"** và **"Chỗ sai/thiếu và cách sửa"** để làm bằng chứng xác thực quá trình làm chủ mã nguồn của nhóm. |

---

### 6.3. Phân tích 3 tình huống điển hình nhóm phát hiện và sửa lỗi do AI sinh ra

Để chứng minh nhóm làm chủ hoàn toàn mã nguồn chứ không phụ thuộc thụ động vào AI, dưới đây là 3 lỗi thực tế điển hình do AI tạo ra đã được nhóm phát hiện và khắc phục:

#### Tình huống 1: Lỗi đếm sai độ dài chuỗi (Off-by-one Bug) trong `src/models/BienSo.h` (TV1 phát hiện)
- **Code do AI gợi ý ban đầu:**
  ```cpp
  if (value.size() != 12 || value[2] != '-' || value[5] != '-') {
      return false;
  }
  ```
- **Nguyên nhân lỗi của AI:** Mô hình ngôn ngữ lớn (LLM) thường ước lượng sai độ dài chuỗi có dấu phân cách (cộng nhầm $2 + 2 + 5 + 3 = 12$). Trên thực tế, chuỗi chuẩn `ab-cd-ghxyz` (ví dụ `"29-A1-12345"`) có độ dài chính xác là $2 + 1 + 2 + 1 + 5 = \mathbf{11}$ ký tự.
- **Cách nhóm kiểm tra và khắc phục:** Ngay khi chạy thử `TC01` với `"29-A1-12345"`, hàm `parseBienSo` luôn trả về `false`. Nhóm trưởng (TV1) đã rà soát lại chỉ số mảng và sửa điều kiện thành `value.size() != 11` (commit `6272f86`), giúp toàn bộ chức năng đăng ký hoạt động chính xác.

#### Tình huống 2: Lỗi AI tự tạo class độc lập trong file kiểm thử `tests/test_bienso.cpp` (TV1 & TV5 phát hiện)
- **Vấn đề từ code AI sinh ra:** Khi TV5 yêu cầu Claude viết file kiểm thử `test_bienso.cpp`, vì không đính kèm toàn bộ cây thư mục `src/` vào prompt nên AI đã tự bịa ra một `class QuanLyBienSo` độc lập ngay bên trong file `test_bienso.cpp` (dùng hàm băm nhân `31`, kích thước bảng `M = 1009`, và từ chối biển số viết chữ thường `"59-b1-12345"`).
- **Cách nhóm kiểm tra và khắc phục:** Khi review nhánh `feature/testing`, Nhóm trưởng phát hiện file test không hề `#include` bất kỳ file header nào trong `src/`, dẫn tới việc test pass nhưng thực chất không kiểm thử code thật của nhóm. Nhóm đã xóa bỏ hoàn toàn class tự chế của AI, thay bằng `#include "../src/services/LicensePlateService.h"` và `"../src/io/FileIO.h"`, đồng thời chuẩn hóa lại 25 Test Cases (`TC01` $\rightarrow$ `TC24`, `ML01`) bám sát 100% kiến trúc `HashTable` (`DJB2`, `M = 101`).

#### Tình huống 3: Lỗi lạm dụng thư viện `std::unordered_set` trong `LicensePlateService.h` (TV1 & TV3 phát hiện)
- **Vấn đề từ code AI sinh ra:** Khi viết hàm `getCommuneCodesByProvince`, AI mặc định sử dụng `std::unordered_set<std::string> seen` của C++ STL để lọc trùng mã xã.
- **Cách nhóm kiểm tra và khắc phục:** Dù kết quả chạy đúng, việc sử dụng một cấu trúc bảng băm có sẵn của STL (`std::unordered_set`) trong đồ án yêu cầu tự cài đặt Bảng băm là không phù hợp với tiêu chí đánh giá của môn Cấu trúc dữ liệu và Giải thuật. Nhóm đã thay thế bằng thuật toán duyệt kiểm tra trùng trực tiếp trên `std::vector<std::string> result`, vừa đảm bảo tính nguyên bản của đồ án vừa không làm tăng độ phức tạp tiệm cận (do số mã xã $K$ trong một tỉnh rất nhỏ).

---

### 6.4. Đánh giá hiệu quả và bài học kinh nghiệm khi sử dụng AI
- **Lợi ích đạt được:** AI giúp nhóm rút ngắn thời gian tra cứu cú pháp C++17 (`std::chrono`, xử lý luồng `std::cin`, định dạng bảng Markdown và vẽ sơ đồ Mermaid), cũng như gợi ý nhanh danh sách các trường hợp biên (Boundary/Invalid cases) để kiểm thử.
- **Bài học cốt lõi:** AI rất dễ mắc lỗi **Off-by-one** (đếm sai độ dài chuỗi), **lệch hợp đồng giao diện (API Drift)** khi không có đủ ngữ cảnh toàn dự án, hoặc **lạm dụng thư viện STL có sẵn**. Do đó, quy trình **Review Pull Request trên GitHub** kết hợp **Kiểm thử tự động (`test_bienso.cpp`)** là chốt chặn quyết định giúp nhóm đảm bảo chất lượng sản phẩm đạt mức hoàn thiện 100%.
