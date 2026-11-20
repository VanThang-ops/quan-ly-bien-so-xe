#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <iostream>
#include <string>
#include "../models/BienSo.h"

struct Node {
    BienSoXe data;
    Node* next;

    explicit Node(const BienSoXe& value) : data(value), next(nullptr) {}
};

// Các thao tác danh sách nhận Node*& để cập nhật được con trỏ đầu bucket.
class LinkedList {
public:
    static bool contains(const Node* head, const std::string& plate) {
        const Node* current = head;
        while (current != nullptr) {
            if (current->data.toString() == plate) return true;
            current = current->next;
        }
        return false;
    }

    static void insertFront(Node*& head, const BienSoXe& value) {
        Node* newNode = new Node(value);
        newNode->next = head;
        head = newNode;
    }

    static bool remove(Node*& head, const std::string& plate) {
        Node* prev = nullptr;
        Node* current = head;
        while (current != nullptr) {
            if (current->data.toString() == plate) {
                if (prev == nullptr) head = current->next;
                else prev->next = current->next;
                delete current;
                return true;
            }
            prev = current;
            current = current->next;
        }
        return false;
    }

    static void print(const Node* head) {
        for (const Node* current = head; current != nullptr; current = current->next) {
            std::cout << current->data.toString() << '\n';
        }
    }

    static void clear(Node*& head) {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }
};

#endif
