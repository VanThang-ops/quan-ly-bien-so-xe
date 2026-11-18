#ifndef FILE_IO_H
#define FILE_IO_H

#include <fstream>
#include <string>
#include "../services/LicensePlateService.h"

class FileIO {
public:
    // Doc du lieu tu file
    static bool loadFromFile(const std::string& path, LicensePlateService& service) {
        std::ifstream input(path);
        if (!input.is_open()) return false;

        std::string line;
        while (std::getline(input, line)) {
            if (trimCopy(line).empty()) continue;

            // Bo qua bien so sai dinh dang hoac da ton tai
            service.registerPlate(line);
        }

        return !input.bad();
    }

    // Luu du lieu vao file
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