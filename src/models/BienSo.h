#ifndef BIEN_SO_H
#define BIEN_SO_H

#include <cctype>
#include <string>

struct BienSoXe {
    std::string maTinh;   // 2 chữ số
    std::string maXa;     // 2 ký tự chữ/số
    std::string seri;     // 5 chữ số

    std::string toString() const {
        return maTinh + "-" + maXa + "-" + seri;
    }
};

inline std::string trimCopy(const std::string& input) {
    const std::string whitespace = " \t\r\n";
    const std::size_t first = input.find_first_not_of(whitespace);
    if (first == std::string::npos) return "";
    const std::size_t last = input.find_last_not_of(whitespace);
    return input.substr(first, last - first + 1);
}

inline bool isDigits(const std::string& value) {
    for (unsigned char ch : value) {
        if (!std::isdigit(ch)) return false;
    }
    return !value.empty();
}

inline bool parseMaTinh(const std::string& raw, std::string& maTinh) {
    const std::string value = trimCopy(raw);
    if (value.size() != 2 || !isDigits(value)) {
        return false;
    }
    maTinh = value;
    return true;
}

inline bool parseMaXa(const std::string& raw, std::string& maXa) {
    std::string value = trimCopy(raw);
    if (value.size() != 2) {
        return false;
    }
    for (unsigned char ch : value) {
        if (!std::isalnum(ch)) return false;
    }
    for (char& ch : value) {
        ch = static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
    }
    maXa = value;
    return true;
}

inline bool parseBienSo(const std::string& raw, BienSoXe& result) {
    const std::string value = trimCopy(raw);
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
