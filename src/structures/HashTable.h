#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <cstddef>
#include <string>
#include "LinkedList.h"
using namespace std;

class HashTable {
public:
    static constexpr size_t TABLE_SIZE = 101;

    HashTable() : table{} {}

    ~HashTable() {
        clear();
    }

    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;

    // Hàm băm DJB2: trả về chỉ số bucket từ 0 đến 100
    size_t hashFunction(const string& key) const {
        unsigned long hash = 5381;
        for (size_t i = 0; i < key.size(); i++) {
            unsigned char ch = (unsigned char)key[i];
            hash = hash * 33 + ch; // Tương đương ((hash << 5) + hash) + ch
        }
        return hash % TABLE_SIZE;
    }

    // Thêm biển số vào bảng băm (Separate Chaining)
    bool insert(const BienSoXe& value) {
        string key = value.toString();
        size_t index = hashFunction(key);

        // Kiểm tra trùng trong bucket trước khi thêm
        if (LinkedList::contains(table[index], key)) {
            return false;
        }

        LinkedList::insertFront(table[index], value);
        size++;
        return true;
    }

    // Tìm kiếm biển số theo khóa chuẩn "ab-cd-ghxyz"
    const BienSoXe* find(const string& key) const {
        size_t index = hashFunction(key);
        const Node* current = table[index];

        while (current != nullptr) {
            if (current->data.toString() == key) {
                return &current->data;
            }
            current = current->next;
        }
        return nullptr;
    }

    // Xóa biển số khỏi bảng băm
    bool remove(const string& key) {
        size_t index = hashFunction(key);
        if (!LinkedList::remove(table[index], key)) {
            return false;
        }
        size--;
        return true;
    }

    // Lấy con trỏ đầu của bucket thứ index
    const Node* bucketAt(size_t index) const {
        if (index < TABLE_SIZE) {
            return table[index];
        }
        return nullptr;
    }

    size_t getSize() const { return size; }

    // Hệ số tải alpha = N / M
    double loadFactor() const {
        return (double)size / (double)TABLE_SIZE;
    }

    // Đếm số lượng phần tử trong bucket thứ index
    size_t bucketSize(size_t index) const {
        if (index >= TABLE_SIZE) return 0;

        size_t count = 0;
        const Node* current = table[index];
        while (current != nullptr) {
            count++;
            current = current->next;
        }
        return count;
    }

    // Đếm số bucket hiện đang có dữ liệu (khác nullptr)
    size_t usedBuckets() const {
        size_t used = 0;
        for (size_t i = 0; i < TABLE_SIZE; i++) {
            if (table[i] != nullptr) {
                used++;
            }
        }
        return used;
    }

    // Tìm độ dài của chuỗi liên kết dài nhất trong bảng băm
    size_t maxBucketSize() const {
        size_t maxCount = 0;
        for (size_t i = 0; i < TABLE_SIZE; i++) {
            size_t currentCount = bucketSize(i);
            if (currentCount > maxCount) {
                maxCount = currentCount;
            }
        }
        return maxCount;
    }

    // Giải phóng toàn bộ bộ nhớ động trong các bucket
    void clear() {
        for (size_t i = 0; i < TABLE_SIZE; i++) {
            LinkedList::clear(table[i]);
        }
        size = 0;
    }

private:
    Node* table[TABLE_SIZE];
    size_t size = 0;
};

#endif
