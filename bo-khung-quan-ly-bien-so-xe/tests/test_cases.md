# Bộ test dự kiến

| ID | Nhóm | Dữ liệu / thao tác | Kết quả mong đợi |
|---|---|---|---|
| TC01 | Hợp lệ | `29-A1-12345` | Đăng ký thành công |
| TC02 | Trùng | Đăng ký `29-A1-12345` lần 2 | Từ chối, không tăng số lượng |
| TC03 | Sai định dạng | `29A112345` | Từ chối |
| TC04 | Sai mã tỉnh | `2A-A1-12345` | Từ chối |
| TC05 | Sai mã xã | `29-A!-12345` | Từ chối |
| TC06 | Sai seri | `29-A1-12B45` | Từ chối |
| TC07 | Sai độ dài | `29-A1-1234` | Từ chối |
| TC08 | Chuẩn hóa | `29-a1-12345` | Chuẩn hóa thành `29-A1-12345` |
| TC09 | Tra cứu có tồn tại | Tìm `51-H3-98765` | Trả về bản ghi |
| TC10 | Tra cứu không tồn tại | Tìm `99-Z9-00000` | Không tìm thấy |
| TC11 | Va chạm | Hai khóa có cùng bucket | Cả hai vẫn tra cứu được |
| TC12 | Xóa đầu danh sách | Xóa node đầu bucket | Danh sách còn hợp lệ |
| TC13 | File không tồn tại | Nạp đường dẫn sai | Báo lỗi mở file |
| TC14 | File có dòng rỗng/sai | Nạp file hỗn hợp | Bỏ qua dòng rỗng/sai, không crash |
| TC15 | Lưu và nạp lại | Lưu rồi nạp lại file | Dữ liệu được giữ nguyên |

## Quy tắc ghi nhận
Mỗi test ghi: ID, dữ liệu đầu vào, kết quả mong đợi, kết quả thực tế, Pass/Fail, người kiểm thử, ghi chú.
