#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <cstddef>
#include <string>
#include "LinkedList.h"

class HashTable {
public:
    static constexpr std::size_t TABLE_SIZE = 101;

    HashTable() : table{} {}

    ~HashTable() {
        clear();
    }

    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;

    std::size_t hashFunction(const std::string& key) const {
        // DJB2: nhóm có thể phân tích và so sánh với Polynomial Rolling.
        unsigned long hash = 5381;
        for (unsigned char ch : key) {
            hash = ((hash << 5) + hash) + ch;
        }
        return hash % TABLE_SIZE;
    }

    bool insert(const BienSoXe& value) {
        const std::string key = value.toString();
        const std::size_t index = hashFunction(key);
        if (LinkedList::contains(table[index], key)) return false;
        LinkedList::insertFront(table[index], value);
        ++size;
        return true;
    }

    const BienSoXe* find(const std::string& key) const {
        const std::size_t index = hashFunction(key);
        for (const Node* current = table[index]; current != nullptr; current = current->next) {
            if (current->data.toString() == key) return &current->data;
        }
        return nullptr;
    }

    bool remove(const std::string& key) {
        const std::size_t index = hashFunction(key);
        if (!LinkedList::remove(table[index], key)) return false;
        --size;
        return true;
    }

    const Node* bucketAt(std::size_t index) const {
        return index < TABLE_SIZE ? table[index] : nullptr;
    }

    std::size_t getSize() const { return size; }

    void clear() {
        for (std::size_t i = 0; i < TABLE_SIZE; ++i) {
            LinkedList::clear(table[i]);
        }
        size = 0;
    }

private:
    Node* table[TABLE_SIZE];
    std::size_t size = 0;
};

#endif
