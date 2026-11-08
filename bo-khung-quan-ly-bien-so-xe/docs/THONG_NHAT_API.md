# API module cần thống nhất

Các chữ ký trong bộ khung là hợp đồng tích hợp ban đầu. Không tự ý đổi tên hàm/tham số nếu chưa thống nhất.

## Biển số
- `bool parseBienSo(const std::string& raw, BienSoXe& result)`
- `std::string BienSoXe::toString() const`

## LinkedList
- `static bool contains(const Node* head, const std::string& plate)`
- `static void insertFront(Node*& head, const BienSoXe& value)`
- `static bool remove(Node*& head, const std::string& plate)`
- `static void clear(Node*& head)`

## HashTable
- `bool insert(const BienSoXe& value)`
- `const BienSoXe* find(const std::string& key) const`
- `bool remove(const std::string& key)`
- `std::size_t getSize() const`

## LicensePlateService
- `bool registerPlate(const std::string& rawPlate)`
- `const BienSoXe* searchPlate(const std::string& rawPlate) const`
- `getCommuneCodesByProvince(...)` – thành viên 3 hoàn thiện
- `getPlatesByCommune(...)` – thành viên 3 hoàn thiện

## FileIO
- `static bool loadFromFile(const std::string& path, LicensePlateService& service)`
- `static bool saveToFile(const std::string& path, const HashTable& table)`

Lưu ý: con trỏ do `searchPlate` trả về chỉ dùng để đọc; không được `delete` hoặc giữ lại sau khi bảng băm bị xóa/thay đổi.
