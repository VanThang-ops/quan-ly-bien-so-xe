#ifndef BIEN_SO_H
#define BIEN_SO_H

#include <string>
using namespace std;

struct BienSoXe {
    string maTinh;   // 2 chữ số (VD: "29")
    string maXa;     // 2 ký tự chữ hoặc số, in hoa (VD: "A1")
    string seri;     // 5 chữ số (VD: "12345")

    string toString() const {
        return maTinh + "-" + maXa + "-" + seri;
    }
};

// Cắt bỏ khoảng trắng thừa ở đầu và cuối chuỗi
inline string trimCopy(const string& input) {
    int left = 0;
    int right = (int)input.size() - 1;

    while (left <= right && (input[left] == ' ' || input[left] == '\t' ||
                             input[left] == '\r' || input[left] == '\n')) {
        left++;
    }
    while (right >= left && (input[right] == ' ' || input[right] == '\t' ||
                             input[right] == '\r' || input[right] == '\n')) {
        right--;
    }

    if (left > right) return "";
    return input.substr(left, right - left + 1);
}

// Kiểm tra chuỗi chỉ toàn chữ số từ '0' đến '9'
inline bool isDigits(const string& value) {
    if (value.empty()) return false;

    for (size_t i = 0; i < value.size(); i++) {
        char c = value[i];
        if (c < '0' || c > '9') {
            return false;
        }
    }
    return true;
}

// Kiểm tra mã tỉnh: đúng 2 chữ số (VD: "29", "51")
inline bool parseMaTinh(const string& raw, string& maTinh) {
    string value = trimCopy(raw);
    if (value.size() != 2 || !isDigits(value)) {
        return false;
    }
    maTinh = value;
    return true;
}

// Kiểm tra mã xã: đúng 2 ký tự chữ hoặc số, tự động đổi chữ thường sang chữ hoa
inline bool parseMaXa(const string& raw, string& maXa) {
    string value = trimCopy(raw);
    if (value.size() != 2) {
        return false;
    }

    for (size_t i = 0; i < value.size(); i++) {
        char c = value[i];

        // Nếu là chữ thường ('a' -> 'z') thì đổi thành chữ hoa ('A' -> 'Z')
        if (c >= 'a' && c <= 'z') {
            c = c - 'a' + 'A';
            value[i] = c;
        }

        // Kiểm tra ký tự sau khi đổi phải là chữ hoa ('A'-'Z') hoặc chữ số ('0'-'9')
        bool laChuHoa = (c >= 'A' && c <= 'Z');
        bool laChuSo  = (c >= '0' && c <= '9');
        if (!laChuHoa && !laChuSo) {
            return false;
        }
    }

    maXa = value;
    return true;
}

// Phân tích chuỗi biển số đầy đủ theo định dạng ab-cd-ghxyz (11 ký tự)
inline bool parseBienSo(const string& raw, BienSoXe& result) {
    string value = trimCopy(raw);

    // Chuỗi chuẩn "29-A1-12345" có đúng 11 ký tự, dấu '-' ở vị trí 2 và 5
    if (value.size() != 11 || value[2] != '-' || value[5] != '-') {
        return false;
    }

    BienSoXe parsed;
    if (!parseMaTinh(value.substr(0, 2), parsed.maTinh)) return false;
    if (!parseMaXa(value.substr(3, 2), parsed.maXa)) return false;

    parsed.seri = value.substr(6, 5);
    if (!isDigits(parsed.seri)) return false;

    result = parsed;
    return true;
}

#endif
