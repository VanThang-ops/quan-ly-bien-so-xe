#ifndef FILE_IO_H
#define FILE_IO_H

#include <fstream>
#include <string>
#include "../services/LicensePlateService.h"

class FileIO {
public:
    static bool loadFromFile(const std::string& path, LicensePlateService& service) {
        std::ifstream input(path);
        if (!input.is_open()) return false;

        std::string line;
        while (std::getline(input, line)) {
            if (trimCopy(line).empty()) continue;
            // Dòng sai định dạng hoặc trùng sẽ bị bỏ qua ở bước nạp.
            service.registerPlate(line);
        }
        return !input.bad();
    }

    static bool saveToFile(const std::string& path, const HashTable& table) {
        std::ofstream output(path);
        if (!output.is_open()) return false;

        for (std::size_t i = 0; i < HashTable::TABLE_SIZE; ++i) {
            for (const Node* current = table.bucketAt(i);
                 current != nullptr; current = current->next) {
                output << current->data.toString() << '\n';
            }
        }
        return output.good();
    }
};

#endif
