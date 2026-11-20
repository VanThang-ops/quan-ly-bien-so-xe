# Báo Cáo Dự Án – Phần Của Thành Viên 5 (Kỹ Sư QA/QC & Tối Ưu Hiệu Năng)

> **Thành viên thực hiện:** Phan Nhật Phi (TV5)  
> **Phạm vi phụ trách:** Mục 5 (Kiểm thử và Đánh giá hiệu năng) và bộ chương trình kiểm thử tự động trong thư mục `tests/` (`tests/test_bienso.cpp`, `tests/bench_bienso.cpp`, `tests/test_cases.md`).

---

## 5. KIỂM THỬ VÀ ĐÁNH GIÁ

### 5.1. Mục tiêu và phạm vi kiểm thử
Quá trình kiểm thử chương trình quản lý biển số xe được thiết kế và thực hiện nhằm bảo đảm 4 mục tiêu cốt lõi:
1. Tất cả các chức năng nghiệp vụ hoạt động đúng với đặc tả yêu cầu của Đề tài 11;
2. Chương trình xử lý chính xác các trường hợp biên (boundary cases), tự động chuẩn hóa dữ liệu hợp lệ và từ chối an toàn mọi dữ liệu sai định dạng;
3. Bộ nhớ động (`Node*`) trong bảng băm được cấp phát và thu hồi đúng cách, không xảy ra rò rỉ bộ nhớ (memory leak) hay giải phóng hai lần (double free);
4. Hiệu năng thực tế phù hợp với độ phức tạp lý thuyết $O(1 + \alpha)$ khi kích thước dữ liệu $N$ tăng lên.

**Phạm vi kiểm thử** bao phủ toàn bộ 5 nhóm chức năng của hệ thống:
- Đăng ký biển số mới (`registerPlate` / `HashTable::insert`);
- Tra cứu biển số (`searchPlate` / `HashTable::find`);
- Liệt kê danh sách mã Xã thuộc một Tỉnh (`getCommuneCodesByProvince`);
- Liệt kê danh sách biển số thuộc một Xã của Tỉnh (`getPlatesByCommune`);
- Đọc/Ghi dữ liệu bền vững qua file văn bản (`FileIO::loadFromFile` & `FileIO::saveToFile`).

---

### 5.2. Môi trường và công cụ kiểm thử

| Hạng mục | Thông tin cấu hình |
| :--- | :--- |
| **Hệ điều hành** | Windows 11 (64-bit) |
| **Trình biên dịch** | `g++` / `clang++` (x86_64-w64-windows-gnu) |
| **Chuẩn ngôn ngữ** | C++17 (`-std=c++17 -Wall -Wextra -pedantic`) |
| **Lệnh biên dịch kiểm thử** | `g++ -std=c++17 -Wall -Wextra -pedantic -O1 -Isrc tests/test_bienso.cpp -o test_bienso.exe` |
| **Lệnh biên dịch đo hiệu năng** | `g++ -std=c++17 -Wall -Wextra -pedantic -O2 -Isrc tests/bench_bienso.cpp -o bench_bienso.exe` |
| **Công cụ kiểm tra bộ nhớ** | Kiểm thử vòng đời `HashTable::clear()` / Destructor RAII & AddressSanitizer |
| **Thư viện đo thời gian** | `<chrono>` (`std::chrono::steady_clock` / `high_resolution_clock`) |
| **Cấu hình máy đo** | CPU: Intel(R) Core(TM) i7-8750H @ 2.20GHz · RAM: 16.0 GB DDR4 |

*Bảng 5.1. Môi trường và công cụ kiểm thử*

---

### 5.3. Phương pháp thiết kế Test Case
Bộ test case được thiết kế kết hợp giữa **kiểm thử hộp đen (Black-box Testing)** dựa trên đặc tả định dạng `ab-cd-ghxyz` và **kiểm thử hộp trắng (White-box Testing)** dựa trên cấu trúc `HashTable` (`M = 101`, Separate Chaining):
- **Phân lớp tương đương (Equivalence Partitioning):** Chia dữ liệu đầu vào thành các lớp tương đương (biển số hợp lệ chuẩn, biển số hợp lệ có chữ thường/khoảng trắng thừa, và từng loại lỗi định dạng riêng biệt). Mỗi test case không hợp lệ (`Invalid`) chỉ cài đúng một lỗi duy nhất để xác định chính xác từng nhánh kiểm tra trong `parseBienSo`, `parseMaTinh`, `parseMaXa`.
- **Phân tích giá trị biên (Boundary Value Analysis):** Kiểm thử các giá trị biên dưới và biên trên của bộ mã (`00-A0-00000`, `99-ZZ-99999`), độ dài chuỗi sát biên (`10` ký tự, `11` ký tự chuẩn, `12` ký tự), và trạng thái biên của cấu trúc dữ liệu (bảng băm rỗng `size == 0`, bucket có 1 node hoặc nhiều node).
- **Kiểm thử đụng độ (Collision Testing):** Nạp số lượng biển số $N = 300 > M = 101$ để chắc chắn xảy ra đụng độ theo nguyên lý Dirichlet (Chuồng bồ câu), sau đó tra cứu lại các phần tử ở đầu và cuối chuỗi liên kết để xác nhận cơ chế **Separate Chaining** không làm mất dữ liệu.
- **Kiểm thử chống trùng lặp (Duplicate Prevention):** Đăng ký cùng một biển số hai lần (bao gồm cả trường hợp khác biệt chữ hoa/chữ thường như `51-F1-00050` và `51-f1-00050`), lần thứ hai bắt buộc bị từ chối và `getSize()` không thay đổi.
- **Kiểm thử khứ hồi (Round-trip File I/O):** Lưu dữ liệu từ `HashTable` ra file văn bản bằng `FileIO::saveToFile` rồi nạp lại vào một `LicensePlateService` mới bằng `FileIO::loadFromFile`, đối chiếu bảo toàn 100% dữ liệu.
- **Kiểm thử chức năng lọc thống kê:** Dùng bộ dữ liệu có kiểm soát gồm nhiều biển số trùng mã Xã trong cùng Tỉnh và cùng mã Xã nhưng khác Tỉnh để kiểm chứng tính đúng đắn của `getCommuneCodesByProvince` và `getPlatesByCommune`.

---

### 5.4. Bộ dữ liệu kiểm thử
Bộ dữ liệu mẫu cơ sở dùng để kiểm tra nhanh các chức năng lọc thống kê theo Tỉnh/Xã:

| STT | Biển số | Mã Tỉnh | Mã Xã | Số Seri |
| :---: | :---: | :---: | :---: | :---: |
| 1 | `51-A1-00001` | `51` | `A1` | `00001` |
| 2 | `51-A1-00002` | `51` | `A1` | `00002` |
| 3 | `51-B2-00003` | `51` | `B2` | `00003` |
| 4 | `29-H3-00004` | `29` | `H3` | `00004` |
| 5 | `29-H3-00005` | `29` | `H3` | `00005` |
| 6 | `29-K9-00006` | `29` | `K9` | `00006` |

*Bảng 5.2. Bộ dữ liệu mẫu 6 biển số kiểm thử chức năng lọc*

Ngoài ra, quá trình kiểm thử còn sử dụng:
- Bộ dữ liệu thực tế `data/data_bienso.txt` gồm **50 biển số mẫu** đa dạng do Thành viên 4 xây dựng;
- Bộ dữ liệu sinh tự động không trùng lặp với hạt giống cố định (`seed = 42`) ở các mức quy mô $N \in \{50, 100, 500, 1.000, 5.000, 10.000, 100.000\}$ để đảm bảo tính khách quan và khả năng tái lập (reproducibility) khi đo hiệu năng.

---

### 5.5. Bảng Test Case và kết quả kiểm thử thực tế
Bảng 5.3 liệt kê toàn bộ **25 Test Cases** được thực thi tự động qua chương trình `tests/test_bienso.cpp` (trực tiếp kiểm thử các module trong `src/`):

| Test ID | Nhóm (Category) | Mô tả kịch bản kiểm thử (Description) | Kết quả mong đợi | Kết quả thực tế | Status |
| :---: | :---: | :--- | :--- | :--- | :---: |
| **TC01** | Normal | Đăng ký biển số hợp lệ (`59-B1-12345`) | Trả về `true`, `size == 1` | Trả về `true`, `size == 1` | **PASS** |
| **TC02** | Normal | Tra cứu biển số tồn tại (`59-B1-12345`) | Trả về con trỏ khác `nullptr` | Tìm thấy `59-B1-12345` | **PASS** |
| **TC03** | Normal | Tra cứu biển số không tồn tại (`59-B1-99999`) | Trả về `nullptr` | Trả về `nullptr` | **PASS** |
| **TC04** | Normal | Lấy danh sách mã Xã theo Tỉnh `59` (khử trùng `B1`) | Trả về `{"B1", "C2"}` | Trả về `{"B1", "C2"}` | **PASS** |
| **TC05** | Normal | Lấy danh sách biển số theo Tỉnh `59` và Xã `B1` | Trả về 2 biển số của `59-B1` | Trả về đúng 2 biển số | **PASS** |
| **TC06** | Normal | Chuẩn hóa mã Xã chữ thường (`29-a1-12345` $\rightarrow$ `29-A1-12345`) | Đăng ký & tra cứu được ở cả chữ hoa/thường | Chuẩn hóa thành `29-A1-12345` | **PASS** |
| **TC07** | Normal | Cắt khoảng trắng thừa hai đầu (`"  29-B2-12345  "`) | Tự động `trimCopy` và đăng ký thành công | Tìm thấy `29-B2-12345` | **PASS** |
| **TC08** | Boundary | Tra cứu khi bảng băm đang rỗng (`size == 0`) | Trả về `nullptr`, không lỗi | Trả về `nullptr` | **PASS** |
| **TC09** | Boundary | Biển số biên dưới `00-A0-00000` | Đăng ký và tra cứu thành công | Thành công | **PASS** |
| **TC10** | Boundary | Biển số biên trên `99-ZZ-99999` | Đăng ký và tra cứu thành công | Thành công | **PASS** |
| **TC11** | Boundary | `getCommuneCodesByProvince("88")` với Tỉnh không tồn tại | Trả về `vector` rỗng | `vector` rỗng | **PASS** |
| **TC12** | Boundary | `getPlatesByCommune("99", "AA")` với Xã không tồn tại | Trả về `vector` rỗng | `vector` rỗng | **PASS** |
| **TC13** | Collision | Va chạm Separate Chaining (300 biển số trên `M = 101` bucket) | `size == 300`, tra cứu được mọi phần tử va chạm | `size == 300`, `maxBucketSize > 1` | **PASS** |
| **TC14** | Duplicate | Chặn đăng ký trùng lặp (`51-F1-00050` và `51-f1-00050`) | Trả về `false`, `size` giữ nguyên `300` | Trả về `false`, `size == 300` | **PASS** |
| **TC15** | Invalid | Chuỗi rỗng `""` hoặc toàn khoảng trắng `"   "` | Từ chối (`false`) | Trả về `false` | **PASS** |
| **TC16** | Invalid | Thiếu ký tự số seri (`59-B1-1234` – 10 ký tự) | Từ chối (`false`) | Trả về `false` | **PASS** |
| **TC17** | Invalid | Thừa ký tự số seri (`59-B1-123456` – 12 ký tự) | Từ chối (`false`) | Trả về `false` | **PASS** |
| **TC18** | Invalid | Sai ký tự mã Tỉnh (`AB-B1-12345`) | Từ chối (`false`) | Trả về `false` | **PASS** |
| **TC19** | Invalid | Thiếu dấu gạch ngang (`59B112345`) | Từ chối (`false`) | Trả về `false` | **PASS** |
| **TC20** | Invalid | Mã Xã chứa ký tự đặc biệt (`59-B!-12345`) | Từ chối (`false`) | Trả về `false` | **PASS** |
| **TC21** | Invalid | Số seri chứa chữ cái (`59-B1-12B45`) | Từ chối (`false`) | Trả về `false` | **PASS** |
| **TC22** | Structure | Xóa node trong `HashTable::remove` và kiểm tra kích thước | Xóa thành công, giảm `size` đi 1 | `remove == true`, `size == 1` | **PASS** |
| **TC23** | File I/O | Đọc file không tồn tại (`khong_ton_tai_xyz.txt`) | Trả về `false`, không crash | Trả về `false` | **PASS** |
| **TC24** | File I/O | Ghi và đọc lại file (Round-trip `saveToFile` $\rightarrow$ `loadFromFile`) | Bảo toàn 100% số lượng và nội dung biển số | Đọc lại đủ 100% bản ghi | **PASS** |
| **ML01** | Memory | Giải phóng 5.000 `Node` bằng `HashTable::clear()` và tái sử dụng | `size == 0`, `usedBuckets == 0`, chèn lại bình thường | Không rò rỉ bộ nhớ | **PASS** |

*Bảng 5.3. Bảng kết quả thực thi 25/25 Test Cases tự động (`PASS 100%`)*

---

### 5.6. Đánh giá hiệu năng (Performance Benchmark)

Hiệu năng được đo bằng `std::chrono::steady_clock` trên bản biên dịch tối ưu hóa `-O2`, sử dụng dữ liệu biển số sinh ngẫu nhiên với hạt giống cố định (`seed = 42`). Không thực hiện lệnh in `std::cout` bên trong vòng lặp đo thời gian để tránh nhiễu I/O.

#### a) Kết quả đo trên cấu hình chính thức của hệ thống (`HashTable` DJB2, `M = 101` bucket)
Thực thi trực tiếp `tests/bench_bienso.cpp` với lớp `LicensePlateService` và `HashTable` (`TABLE_SIZE = 101`):

| Quy mô $N$ | Số bucket $M$ | Hệ số tải $\alpha = N/M$ | Số bucket có dữ liệu (`usedBuckets`) | Độ dài bucket lớn nhất (`maxBucketSize`) | Thời gian Insert toàn bộ $N$ khóa (ms) | Thời gian Search toàn bộ $N$ khóa (ms) | Tỷ lệ tìm thấy |
| ---: | :---: | ---: | :---: | :---: | ---: | ---: | :---: |
| **50** | 101 | 0.50 | 47 / 101 | 2 | `0.022 ms` | `0.010 ms` | 50 / 50 |
| **100** | 101 | 0.99 | 69 / 101 | 3 | `0.032 ms` | `0.020 ms` | 100 / 100 |
| **500** | 101 | 4.95 | 101 / 101 | 7 | `0.156 ms` | `0.127 ms` | 500 / 500 |
| **1.000** | 101 | 9.90 | 101 / 101 | 13 | `0.354 ms` | `0.329 ms` | 1.000 / 1.000 |
| **5.000** | 101 | 49.50 | 101 / 101 | 56 | `4.864 ms` | `4.764 ms` | 5.000 / 5.000 |
| **10.000** | 101 | 99.01 | 101 / 101 | 110 | `17.817 ms` | `17.625 ms` | 10.000 / 10.000 |

*Bảng 5.4. Kết quả đo hiệu năng và độ phân tán bucket của hàm băm DJB2 trên bảng băm $M = 101$*

#### b) Khảo sát mở rộng: Ảnh hưởng của kích thước bảng băm $M$ khi $N$ tăng lớn ($1.000 \rightarrow 100.000$)
Để đánh giá sâu hơn mối quan hệ giữa số lượng phần tử $N$ và kích thước bảng $M \in \{1, 101, 1009, 10007\}$ (với $M = 1$ tương đương với một danh sách liên kết đơn thuần không dùng bảng băm):

| Quy mô dữ liệu $N$ | Kích thước bảng $M$ | Hệ số tải $\alpha = N/M$ | Thời gian Insert (ms) | Thời gian Search (ms) | Tỷ lệ tìm thấy |
| ---: | ---: | ---: | ---: | ---: | :---: |
| **1.000** | `1` *(DSLK đơn)* | 1000.00 | `6.336 ms` | `5.987 ms` | 1000 / 1000 |
| **1.000** | **`101`** *(Mặc định)* | **9.90** | **`0.464 ms`** | **`0.192 ms`** | 1000 / 1000 |
| **1.000** | `1009` | 0.99 | `0.349 ms` | `0.107 ms` | 1000 / 1000 |
| **1.000** | `10007` | 0.10 | `0.351 ms` | `0.069 ms` | 1000 / 1000 |
| **10.000** | `1` *(DSLK đơn)* | 10000.00 | `338.270 ms` | `281.997 ms` | 10000 / 10000 |
| **10.000** | **`101`** *(Mặc định)* | **99.01** | **`7.549 ms`** | **`7.920 ms`** | 10000 / 10000 |
| **10.000** | `1009` | 9.91 | `1.958 ms` | `1.020 ms` | 10000 / 10000 |
| **10.000** | `10007` | 1.00 | `1.995 ms` | `0.605 ms` | 10000 / 10000 |
| **100.000** | `1` *(DSLK đơn)* | 100000.00 | `31188.891 ms` | `29901.298 ms` | 100000 / 100000 |
| **100.000** | **`101`** *(Mặc định)* | **990.10** | **`868.732 ms`** | **`849.766 ms`** | 100000 / 100000 |
| **100.000** | `1009` | 99.11 | `127.420 ms` | `96.796 ms` | 100000 / 100000 |
| **100.000** | `10007` | 9.99 | `39.145 ms` | `16.363 ms` | 100000 / 100000 |

*Bảng 5.5. So sánh hiệu năng thực thi theo kích thước bảng băm $M \in \{1, 101, 1009, 10007\}$*

**Nhận xét và đánh giá:**
1. **Phản ánh chính xác độ phức tạp lý thuyết $O(1 + \alpha)$ với $\alpha = \frac{N}{M}$:**
   - Khi $M = 1$ (bảng băm suy biến thành 1 danh sách liên kết đơn duy nhất), ở mức $N = 100.000$ biển số, thời gian chèn mất tới `31.188 ms` ($\approx 31,2$ giây) và tra cứu mất `29.901 ms` ($\approx 29,9$ giây) do chi phí duyệt tuyến tính $O(N)$ cho mỗi thao tác.
   - Khi sử dụng bảng băm Separate Chaining với $M = 101$ bucket, thời gian tra cứu $100.000$ biển số giảm mạnh xuống chỉ còn `849,766 ms` (**nhanh gấp ~35 lần** so với danh sách liên kết đơn), và ở mức quy mô vừa ($N = 1.000 \div 10.000$ biển số), toàn bộ thao tác chèn và tra cứu chỉ tốn từ `0,35 ms` đến `17,8 ms` (tức trung bình chỉ vài micro-giây cho mỗi biển số).
2. **Đánh giá độ phân tán của hàm băm DJB2 với số nguyên tố $M = 101$:**
   - Từ Bảng 5.4, tại $N = 1.000$ ($\alpha = 9,90$), cả `101/101` bucket đều được lấp đầy đồng đều và bucket dài nhất (`MaxChain`) chỉ chứa `13` phần tử (rất sát với kỳ vọng lý thuyết $\alpha \approx 10$). Tại $N = 10.000$ ($\alpha = 99,01$), bucket dài nhất chỉ là `110` phần tử. Điều này chứng minh hàm băm **DJB2** kết hợp kích thước số nguyên tố $M = 101$ phân phối khóa cực kỳ đồng đều, không gây hiện tượng gom cụm (clustering).
3. **Tác động của hệ số tải $\alpha$:** Khi $N$ tăng lên hàng trăm nghìn bản ghi ($N = 100.000$), nếu giữ cố định $M = 101$ thì $\alpha \approx 990$ làm chuỗi liên kết tại mỗi bucket dài ra; nếu nâng $M$ lên số nguyên tố tương ứng ($M = 10007$, giữ $\alpha \approx 10$), thời gian tra cứu $100.000$ biển số giảm xuống chỉ còn `16,363 ms`.

---

### 5.7. Tổng kết kiểm thử và hướng cải tiến
- **Kết quả đạt được:** Hệ thống Quản lý Biển số Xe đã vượt qua toàn bộ **25/25 ca kiểm thử tự động (`100% PASS`)** trên mọi nhóm chức năng (Normal, Boundary, Collision, Duplicate, Invalid, Structure, File I/O và Memory Management), vận hành ổn định và không rò rỉ bộ nhớ.
- **Hạn chế hiện tại & Hướng phát triển:** Trong phạm vi đồ án hiện tại, kích thước bảng băm đang được cấu hình cố định là hằng số `TABLE_SIZE = 101` (rất tối ưu cho quy mô dữ liệu vừa và nhỏ từ vài chục đến vài nghìn biển số). Trong tương lai, nếu áp dụng cho quy mô cấp quốc gia (hàng triệu phương tiện), hệ thống có thể nâng cấp thêm cơ chế **Rehashing động (Dynamic Resizing)**: tự động nhân đôi kích thước bảng lên số nguyên tố kế tiếp (ví dụ $101 \rightarrow 211 \rightarrow 431 \rightarrow 1009 \rightarrow 10007$) và băm lại khi hệ số tải $\alpha = \frac{N}{M}$ vượt ngưỡng $\alpha_{\max} = 2.0$.

---

### Phụ lục Mục 5: Nhật ký sử dụng AI của Thành viên 5 (Dữ liệu tổng hợp cho Mục 6)

| Ngày | Công cụ AI | Mục đích sử dụng | Kết quả AI trả về | Cách kiểm tra, phát hiện lỗi và chỉnh sửa của TV5 |
| :---: | :---: | :--- | :--- | :--- |
| `13/11/2026` | **Claude** | Đọc đề và giải thích cấu trúc yêu cầu phần Kiểm thử & Đánh giá (Mục 5) | Cung cấp định hướng các kỹ thuật thiết kế test case (phân lớp tương đương, giá trị biên, đụng độ) và khung bảng số liệu báo cáo. | Đối chiếu trực tiếp với đề cương Đề tài 11 và tài liệu `docs/THIET_KE_KIEN_TRUC.md` của nhóm. |
| `14/11/2026` | **Claude** | Sinh khung code kiểm thử tự động `tests/test_bienso.cpp` | AI tự định nghĩa một `class QuanLyBienSo` riêng bên trong file test và đánh giá mã xã chữ thường (`TC20: 59-b1-12345`) là không hợp lệ (`Invalid`). | Phát hiện code AI không `#include` code thật của nhóm và sai quy ước chuẩn hóa chữ hoa ở `BienSo.h`. Đã xóa class giả lập của AI, `#include "../src/services/LicensePlateService.h"` và `"../src/io/FileIO.h"`, sửa lại `TC06` và `TC20`, chạy đạt `25/25 PASS`. |
| `14/11/2026` | **Claude** | Sinh code đo hiệu năng `tests/bench_bienso.cpp` | Cung cấp hàm sinh biển số ngẫu nhiên và đo thời gian `Insert` / `Search` bằng `std::chrono`. | Tích hợp trực tiếp với `LicensePlateService` và bổ sung in các chỉ số nội tại của `HashTable` (`loadFactor()`, `usedBuckets()`, `maxBucketSize()`), biên dịch với cờ `-O2`. |
| `15/11/2026` | **Gemini** | Phân tích bảng số liệu benchmark và nhận xét biến động hiệu năng theo hệ số tải $\alpha = N/M$ | Xuất đoạn nhận xét đánh giá độ phức tạp $O(1 + N/M)$ và gợi ý hướng cải tiến Rehash động. | Kiểm tra lại số liệu trên Terminal, sửa lỗi định dạng công thức toán học bị lặp chữ (`O(1+N/M)`) và cờ biên dịch `-O1`, `-O2` trước khi đưa vào báo cáo. |
