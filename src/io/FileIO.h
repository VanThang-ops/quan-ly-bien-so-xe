#ifndef FILE_IO_H
#define FILE_IO_H

#include <fstream>
#include <string>
#include "../services/LicensePlateService.h"

class FileIO {
public:
    // Đọc dữ liệu từ file văn bản vào bảng băm thông qua tầng service
    static bool loadFromFile(const std::string& path, LicensePlateService& service) {
        std::ifstream input(path);
        if (!input.is_open()) return false;

        std::string line;
        while (std::getline(input, line)) {
            if (trimCopy(line).empty()) continue;

            // Dòng sai định dạng hoặc biển số đã tồn tại sẽ bị bỏ qua ở bước nạp.
            service.registerPlate(line);
        }

        return !input.bad();
    }

    // Lưu toàn bộ dữ liệu từ bảng băm ra file văn bản
    static bool saveToFile(const std::string& path, const HashTable& table) {
        std::ofstream output(path);
        if (!output.is_open()) return false;

        for (std::size_t i = 0; i < HashTable::TABLE_SIZE; ++i) {
            for (const Node* current = table.bucketAt(i);
                 current != nullptr; current = current->next) {
                output << current->data.toString() << '\n';
            }
        }

        output.close();
        return !output.fail();
    }
};

#endif
