#ifndef LICENSE_PLATE_SERVICE_H
#define LICENSE_PLATE_SERVICE_H

#include <string>
#include <vector>
#include "../structures/HashTable.h"

class LicensePlateService {
public:
    bool registerPlate(const std::string& rawPlate) {
        BienSoXe plate;
        if (!parseBienSo(rawPlate, plate)) return false;
        return hashTable.insert(plate);
    }

    const BienSoXe* searchPlate(const std::string& rawPlate) const {
        BienSoXe plate;
        if (!parseBienSo(rawPlate, plate)) return nullptr;
        return hashTable.find(plate.toString());
    }

    std::vector<std::string> getCommuneCodesByProvince(const std::string& provinceCode) const {
        std::vector<std::string> result;
        // TODO: Duyệt toàn bộ bucket, thu thập mã xã thuộc tỉnh,
        // tránh trùng mã xã. Độ phức tạp dự kiến O(M + N).
        (void)provinceCode;
        return result;
    }

    std::vector<BienSoXe> getPlatesByCommune(
        const std::string& provinceCode,
        const std::string& communeCode) const {
        std::vector<BienSoXe> result;
        // TODO: Duyệt toàn bộ bucket và lọc theo cả mã tỉnh + mã xã.
        (void)provinceCode;
        (void)communeCode;
        return result;
    }

    const HashTable& data() const { return hashTable; }

private:
    HashTable hashTable;
};

#endif
