# Báo Cáo Dự Án – Phần Của Thành Viên 3 (Lập Trình Viên Thống Kê)

> **Thành viên thực hiện:** Trần Võ Công Danh (TV3 – `tranvocongdanhkt-glitch`)  
> **Phạm vi phụ trách:** Mục 4 (Cài đặt và Hiện thực) và hai hàm thống kê theo Tỉnh/Xã trong `src/services/LicensePlateService.h`.

---

## 4. CÀI ĐẶT VÀ HIỆN THỰC

### 4.1. Phạm vi cài đặt
Phần việc của Thành viên 3 tập trung vào hai chức năng thống kê của hệ thống quản lý biển số xe:
1. Lấy danh sách mã Xã thuộc một Tỉnh (`getCommuneCodesByProvince`);
2. Lấy danh sách biển số thuộc một Xã của Tỉnh được chọn (`getPlatesByCommune`).

Hai chức năng này được cài đặt trong lớp `LicensePlateService` tại file `src/services/LicensePlateService.h`.

Khác với chức năng đăng ký hoặc tra cứu một biển số cụ thể (chỉ cần băm khóa rồi truy cập đúng 1 bucket), hai chức năng thống kê cần duyệt dữ liệu trên toàn bộ bảng băm để tìm các phần tử thỏa mãn điều kiện hành chính (vì hàm băm DJB2 băm trên toàn bộ chuỗi khóa `ab-cd-ghxyz` nên các biển số cùng một Tỉnh nằm phân tán ở nhiều bucket khác nhau). Dữ liệu được lưu trong `HashTable` có `101` bucket, mỗi bucket quản lý các `Node` bằng danh sách liên kết đơn theo phương pháp **Separate Chaining**.

### 4.2. Dữ liệu sử dụng và thư viện hỗ trợ
Mỗi `Node` trong danh sách liên kết chứa một đối tượng `BienSoXe`. Đối tượng này có ba trường chính: `maTinh` (mã Tỉnh), `maXa` (mã Xã) và `seri` (số seri biển số). Hai hàm thống kê truy cập các trường này để so sánh với dữ liệu người dùng nhập.

| Thành phần | Kiểu dữ liệu / Hàm | Vai trò trong chức năng |
| :--- | :--- | :--- |
| `provinceCode` | `std::string` | Mã Tỉnh do người dùng nhập. |
| `communeCode` | `std::string` | Mã Xã do người dùng nhập ở chức năng thứ hai. |
| `BienSoXe` | `struct` | Lưu mã Tỉnh, mã Xã và số seri của một biển số. |
| `Node` | `struct` | Lưu `BienSoXe` và con trỏ `next` để liên kết các phần tử trong cùng bucket. |
| `HashTable::bucketAt(i)` | `const Node*` | Trả về `Node` đầu tiên của bucket thứ `i` để bắt đầu duyệt. |
| `parseMaTinh()` | `bool` | Kiểm tra mã Tỉnh gồm đúng 2 chữ số và chuẩn hóa dữ liệu. |
| `parseMaXa()` | `bool` | Kiểm tra mã Xã gồm 2 ký tự chữ/số và chuẩn hóa chữ cái thành in hoa. |
| `std::vector` | `std::vector<std::string>` / `std::vector<BienSoXe>` | Lưu danh sách kết quả trả về và hỗ trợ kiểm tra trùng mã Xã mà không phụ thuộc vào bảng băm có sẵn của STL. |

### 4.3. Chức năng lấy danh sách mã Xã theo Tỉnh (`getCommuneCodesByProvince`)

#### 4.3.1. Mục đích và dữ liệu vào/ra
Hàm `getCommuneCodesByProvince` nhận vào mã Tỉnh `provinceCode` và trả về danh sách các mã Xã đang có biển số thuộc Tỉnh đó. Nếu nhiều biển số cùng thuộc một Xã thì mã Xã chỉ được xuất hiện **đúng một lần** trong danh sách kết quả.

| Hạng mục | Mô tả |
| :--- | :--- |
| **Dữ liệu vào** | `provinceCode` – chuỗi mã Tỉnh, yêu cầu đúng 2 chữ số. |
| **Dữ liệu ra** | `std::vector<std::string>` – danh sách mã Xã không trùng lặp. |
| **Trường hợp đầu vào sai** | Trả về `vector` rỗng. |
| **Điều kiện lọc** | `current->data.maTinh == maTinh`. |

#### 4.3.2. Cách cài đặt
Đầu tiên, hàm tạo `std::vector<std::string> result` để lưu kết quả. Mã Tỉnh đầu vào được kiểm tra và cắt khoảng trắng bằng `parseMaTinh`. Nếu không hợp lệ, hàm kết thúc sớm và trả về danh sách rỗng.

Nếu mã Tỉnh hợp lệ, chương trình duyệt lần lượt các bucket từ `0` đến `HashTable::TABLE_SIZE - 1` (`100`). Tại mỗi bucket, hàm `bucketAt(i)` lấy `Node` đầu tiên, sau đó vòng lặp `while` tiếp tục duyệt từng `Node` thông qua con trỏ `next`.

Với mỗi `Node`, chương trình so sánh `current->data.maTinh` với mã Tỉnh đã chuẩn hóa. Nếu trùng khớp, chương trình kiểm tra xem `current->data.maXa` đã tồn tại trong `result` hay chưa thông qua biến cờ `bool exists`. Nếu chưa tồn tại (`!exists`), mã Xã đó mới được đẩy vào `result.push_back(current->data.maXa)`.

```cpp
std::vector<std::string> getCommuneCodesByProvince(const std::string& provinceCode) const {
    std::vector<std::string> result;

    std::string maTinh;
    if (!parseMaTinh(provinceCode, maTinh)) {
        return result;
    }

    for (std::size_t i = 0; i < HashTable::TABLE_SIZE; ++i) {
        const Node* current = hashTable.bucketAt(i);

        while (current != nullptr) {
            if (current->data.maTinh == maTinh) {
                bool exists = false;
                for (const std::string& code : result) {
                    if (code == current->data.maXa) {
                        exists = true;
                        break;
                    }
                }
                if (!exists) {
                    result.push_back(current->data.maXa);
                }
            }

            current = current->next;
        }
    }
    return result;
}
```

> **Điểm cần chú ý:** Việc khử trùng mã Xã được cài đặt trực tiếp bằng vòng lặp duyệt trên `result` thay vì dùng `std::unordered_set`, giúp hệ thống tuân thủ tuyệt đối yêu cầu của học phần là tự hiện thực cấu trúc bảng băm mà không sử dụng bảng băm có sẵn của thư viện chuẩn STL.

### 4.4. Chức năng lấy danh sách biển số theo Tỉnh và Xã (`getPlatesByCommune`)

#### 4.4.1. Mục đích và dữ liệu vào/ra
Hàm `getPlatesByCommune` nhận vào mã Tỉnh `provinceCode` và mã Xã `communeCode`, sau đó trả về danh sách tất cả các biển số có đồng thời mã Tỉnh và mã Xã khớp với hai giá trị đầu vào.

| Hạng mục | Mô tả |
| :--- | :--- |
| **Dữ liệu vào** | `provinceCode` – mã Tỉnh; `communeCode` – mã Xã. |
| **Dữ liệu ra** | `std::vector<BienSoXe>` – danh sách các biển số tìm được. |
| **Trường hợp đầu vào sai** | Trả về `vector` rỗng nếu mã Tỉnh hoặc mã Xã không hợp lệ. |
| **Điều kiện lọc** | `current->data.maTinh == maTinh && current->data.maXa == maXa`. |

#### 4.4.2. Cách cài đặt
Hàm tạo `std::vector<BienSoXe> result` để lưu các biển số tìm được. Trước khi duyệt bảng băm, chương trình lần lượt kiểm tra mã Tỉnh bằng `parseMaTinh` và mã Xã bằng `parseMaXa`. Hàm `parseMaXa` vừa kiểm tra mã Xã gồm đúng 2 ký tự chữ hoặc số, vừa chuẩn hóa chữ cái thành chữ in hoa (ví dụ người dùng nhập `"a1"` sẽ được chuẩn hóa thành `"A1"`).

Sau khi hai mã đều hợp lệ, chương trình duyệt toàn bộ `101` bucket và các `Node` tương tự chức năng thứ nhất. Tại mỗi `Node`, chương trình kiểm tra **đồng thời hai điều kiện**: mã Tỉnh phải trùng và mã Xã phải trùng. Chỉ khi cả hai điều kiện cùng đúng, đối tượng `BienSoXe` mới được đưa vào `result`. Việc kiểm tra đồng thời hai trường giúp ngăn chặn triệt để lỗi trả về nhầm biển số thuộc một Tỉnh khác nhưng có cùng mã Xã (ví dụ `29-A1-...` và `51-A1-...`).

```cpp
std::vector<BienSoXe> getPlatesByCommune(
    const std::string& provinceCode,
    const std::string& communeCode) const {
    std::vector<BienSoXe> result;

    std::string maTinh;
    std::string maXa;

    if (!parseMaTinh(provinceCode, maTinh)) {
        return result;
    }

    if (!parseMaXa(communeCode, maXa)) {
        return result;
    }

    for (std::size_t i = 0; i < HashTable::TABLE_SIZE; ++i) {
        const Node* current = hashTable.bucketAt(i);

        while (current != nullptr) {
            if (current->data.maTinh == maTinh &&
                current->data.maXa == maXa) {

                result.push_back(current->data);
            }

            current = current->next;
        }
    }

    return result;
}
```

### 4.5. Phân tích thuật toán và độ phức tạp
Gọi $M = 101$ là số bucket của bảng băm, $N$ là tổng số `Node` (tổng số biển số xe) đang được lưu trong hệ thống, $K$ là số mã Xã khác nhau thuộc một Tỉnh ($K \le N$), và $P$ là số biển số thỏa mãn điều kiện lọc theo Tỉnh + Xã.

| Hàm | Độ phức tạp Thời gian | Độ phức tạp Bộ nhớ phụ | Giải thích |
| :--- | :---: | :---: | :--- |
| `getCommuneCodesByProvince` | $O(M + N + K^2)$ | $O(K)$ | Duyệt toàn bộ $M$ bucket và $N$ nút; kiểm tra trùng mã Xã trên vector `result` có tối đa $K$ mã Xã của Tỉnh đó (do số mã Xã $K$ trong 1 Tỉnh nhỏ nên chi phí thực tế xấp xỉ $O(M + N)$). |
| `getPlatesByCommune` | $O(M + N)$ | $O(P)$ | Duyệt toàn bộ $M$ bucket và $N$ nút; so sánh chuỗi độ dài cố định $O(1)$ và lưu $P$ biển số thỏa mãn vào `vector`. |

Về bộ nhớ phụ, hàm thứ nhất chỉ cần lưu tối đa $K$ mã Xã trong `vector` kết quả; hàm thứ hai cần `vector` chứa $P$ biển số tìm thấy. Cả hai hàm đều được đánh dấu `const`, không cấp phát hay xóa bất kỳ `Node` nào trong `HashTable`, mà chỉ đọc dữ liệu an toàn thông qua con trỏ hằng `const Node*`.

### 4.6. Các trường hợp kiểm tra chức năng
Để kiểm tra hai chức năng thống kê, sử dụng bộ dữ liệu mẫu gồm 6 biển số đại diện:

| STT | Biển số | Mã Tỉnh | Mã Xã | Số Seri |
| :---: | :---: | :---: | :---: | :---: |
| 1 | `51-A1-00001` | `51` | `A1` | `00001` |
| 2 | `51-A1-00002` | `51` | `A1` | `00002` |
| 3 | `51-B2-00003` | `51` | `B2` | `00003` |
| 4 | `29-H3-00004` | `29` | `H3` | `00004` |
| 5 | `29-H3-00005` | `29` | `H3` | `00005` |
| 6 | `29-K9-00006` | `29` | `K9` | `00006` |

Từ bộ dữ liệu trên, kết quả kiểm tra của hai chức năng thống kê như sau:

| Mã Test | Lời gọi hàm | Kết quả trả về | Mục đích kiểm tra |
| :--- | :--- | :--- | :--- |
| **TC04-1** | `getCommuneCodesByProvince("51")` | `{"A1", "B2"}` | Lấy các mã Xã thuộc Tỉnh `51`; mã `A1` xuất hiện 2 biển số nhưng chỉ trả về 1 lần. |
| **TC04-2** | `getCommuneCodesByProvince("29")` | `{"H3", "K9"}` | Lấy các mã Xã thuộc Tỉnh `29`. |
| **TC05-1** | `getPlatesByCommune("51", "A1")` | `{"51-A1-00001", "51-A1-00002"}` | Lấy đúng danh sách biển số thuộc Tỉnh `51` và Xã `A1`. |
| **TC05-2** | `getPlatesByCommune("29", "h3")` | `{"29-H3-00004", "29-H3-00005"}` | Kiểm tra tự động chuẩn hóa mã Xã chữ thường `"h3"` thành `"H3"`. |
| **TC12** | `getPlatesByCommune("51", "H3")` | Danh sách rỗng `{}` | Kiểm tra không lấy nhầm xã `H3` của Tỉnh `29` khi truy vấn Tỉnh `51`. |
| **TC11** | `getCommuneCodesByProvince("xx")` | Danh sách rỗng `{}` | Kiểm tra từ chối mã Tỉnh sai định dạng. |

### 4.7. Kết nối với các thành phần khác của hệ thống
Hai hàm thống kê được đặt trong `LicensePlateService` và truy xuất dữ liệu từ `HashTable` thông qua `hashTable.bucketAt(i)`. Khi người dùng chọn chức năng `3` hoặc `4` trên `Menu`, tầng giao diện (`src/ui/Menu.h` do Thành viên 4 phụ trách) sẽ nhận chuỗi mã Tỉnh / mã Xã từ bàn phím và gọi hai hàm dịch vụ này để nhận `std::vector` kết quả hiển thị ra màn hình Console.

### 4.8. Tổng kết phần cài đặt
Thành viên 3 đã hoàn thiện hai hàm thống kê trong `src/services/LicensePlateService.h`:
- `getCommuneCodesByProvince`: Duyệt toàn bộ `HashTable` ($O(M + N)$) để thu thập danh sách mã Xã duy nhất thuộc một Tỉnh, tự khử trùng trên `std::vector<std::string>` mà không phụ thuộc vào bảng băm STL.
- `getPlatesByCommune`: Lọc và trả về toàn bộ các biển số thỏa mãn đồng thời cả mã Tỉnh và mã Xã sau khi đã chuẩn hóa đầu vào qua `parseMaTinh` và `parseMaXa`.
