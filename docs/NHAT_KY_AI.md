# Nhật ký khai thác công cụ AI

Đề tài 11: Hệ thống Quản lý Biển số Xe · Nhóm [số nhóm] · Người tổng hợp: Thành viên 6

*Bản mẫu để cả nhóm cùng ghi từ bây giờ. Mục đích: làm dữ liệu cho Mục 6 của báo cáo, và để mỗi người trả lời được câu hỏi vấn đáp về cách mình kiểm tra và tinh chỉnh code do AI gợi ý.*

## 1. Cách ghi

- Ghi ngay sau mỗi lần dùng AI cho dự án, đừng để đến cuối kỳ mới nhớ lại.
- Ghi cả những lần AI trả lời sai hoặc thiếu. Đó là bằng chứng nhóm có kiểm chứng chứ không tin AI một cách mù quáng.
- Hai ô quan trọng nhất là **Cách kiểm tra** và **Chỗ sai/thiếu và cách sửa**.
- Không dán mật khẩu, token hay dữ liệu cá nhân vào prompt hoặc vào log này.
- Mỗi lần dùng AI điền một phiếu ở Mục 4, rồi thêm một dòng vào bảng tổng hợp ở Mục 3.

## 2. Nguyên tắc dùng AI của nhóm (đề xuất, cả nhóm thống nhất lại)

- Đọc và hiểu từng dòng code AI đưa ra trước khi đưa vào dự án.
- Luôn biên dịch và chạy thử. Code chưa chạy thì chưa được coi là xong.
- Không đổi tên hàm hay tham số theo gợi ý của AI nếu chưa thống nhất với nhóm (xem `docs/THONG_NHAT_API.md`).
- Code đưa lên repo vẫn đi qua Pull Request và có người review như mọi phần khác.
- Mọi lần dùng AI đều được ghi vào log này.

## 3. Bảng tổng hợp

| STT | Ngày | Thành viên | Công cụ AI | Mục đích | Kết luận |
| --- | --- | --- | --- | --- | --- |
| 1 | | | | | |
| 2 | | | | | |
| 3 | | | | | |
| 4 | | | | | |
| 5 | | | | | |

## 4. Phiếu chi tiết (sao chép phiếu này cho mỗi lần dùng AI)

**Phiếu số: …**

| Mục | Nội dung |
| --- | --- |
| Ngày | |
| Thành viên | |
| Công cụ AI | |
| Mục đích | |
| Module liên quan | |
| Prompt đã gửi | |
| Kết quả AI trả về | |
| Cách kiểm tra | |
| Chỗ sai/thiếu và cách sửa | |
| Kết luận | Dùng nguyên / Dùng sau khi sửa / Bỏ |

## 5. Ví dụ minh họa (giả định, chỉ để tham khảo cách điền, xóa trước khi nộp)

**Phiếu số: 1 (ví dụ)**

| Mục | Nội dung |
| --- | --- |
| Ngày | [dd/mm] |
| Thành viên | Thành viên 1 |
| Công cụ AI | [tên công cụ] |
| Mục đích | Viết hàm băm DJB2 cho chuỗi biển số |
| Module liên quan | `structures/HashTable.h` |
| Prompt đã gửi | Viết hàm băm DJB2 cho `std::string` trong C++17, trả về chỉ số bucket trong khoảng từ 0 đến M-1. |
| Kết quả AI trả về | Hàm khởi tạo `hash = 5381`, mỗi ký tự `hash = hash * 33 + c`, nhưng trả về luôn `hash` mà chưa lấy phần dư theo M. |
| Cách kiểm tra | Đối chiếu với mô tả trong `THIET_KE_KIEN_TRUC.md`; chạy thử với các biển số mẫu trong `data_bienso.txt` và in ra chỉ số bucket. |
| Chỗ sai/thiếu và cách sửa | Có chỉ số vượt quá M-1 nên truy cập ngoài mảng. Sửa bằng cách thêm `% M` ở bước trả về. |
| Kết luận | Dùng sau khi sửa |
