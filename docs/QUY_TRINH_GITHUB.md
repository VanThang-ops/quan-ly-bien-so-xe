# Quy trình GitHub cho nhóm

## Branch đề xuất
- `main`: phiên bản ổn định, chỉ nhận Pull Request.
- `feature/hash-table`
- `feature/linked-list`
- `feature/statistics`
- `feature/file-io-menu`
- `feature/testing`
- `feature/documentation`

Nhóm trưởng có thể làm việc trên `feature/hash-table` hoặc một branch tích hợp riêng; không phát triển trực tiếp trên `main`.

## Quy trình mỗi nhiệm vụ
1. Cập nhật `main` mới nhất.
2. Chuyển sang branch cá nhân.
3. Code trong phạm vi module đã nhận.
4. Biên dịch và chạy test liên quan.
5. Commit với thông điệp rõ nghĩa.
6. Push branch lên GitHub.
7. Tạo Pull Request vào `main`.
8. Thành viên khác review; sửa theo góp ý.
9. Nhóm trưởng merge khi kiểm tra đạt.

## Commit message gợi ý
- `feat: implement plate registration`
- `feat: add hash table search`
- `fix: validate plate format`
- `test: add invalid input cases`
- `docs: update project architecture`

## Thiết lập bảo vệ main
Trong Settings → Branches (hoặc Rules → Rulesets):
- Tạo rule áp dụng cho `main`.
- Bật yêu cầu Pull Request trước khi merge.
- Yêu cầu ít nhất 1 approval nếu gói/quyền repository hỗ trợ.
- Chặn force push và xóa nhánh.
- Không bật quy tắc bắt buộc status check nếu nhóm chưa cấu hình CI.

## Quy tắc phối hợp
- Không sửa file thuộc module người khác nếu chưa trao đổi.
- Khi cần đổi chữ ký hàm hoặc cấu trúc dữ liệu, mở Issue/thảo luận trước.
- Không commit mật khẩu, token, file cá nhân hoặc file thực thi.
