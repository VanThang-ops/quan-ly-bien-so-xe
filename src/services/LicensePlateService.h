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
        std::unordered_set<std::string> seen;

        std::string maTinh;
        if (!parseMaTinh(provinceCode, maTinh)) {
            return result;
        }

        for (std::size_t i = 0; i < HashTable::TABLE_SIZE; ++i) {
            const Node* current = hashTable.bucketAt(i);

            while (current != nullptr) {
                if (current->data.maTinh == maTinh) {
                    std::string maXa = current->data.maXa;

                    if (seen.insert(maXa).second) {
                        result.push_back(maXa);
                    }
                }

                current = current->next;
            }
        }
        return result;
    }

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
        return result;
    }

    const HashTable& data() const { return hashTable; }

private:
    HashTable hashTable;
};

#endif
