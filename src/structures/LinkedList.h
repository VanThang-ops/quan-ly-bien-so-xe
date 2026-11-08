#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <iostream>
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
        for (const Node* current = head; current != nullptr; current = current->next) {
            if (current->data.toString() == plate) return true;
        }
        return false;
    }

    static void insertFront(Node*& head, const BienSoXe& value) {
        Node* node = new Node(value);
        node->next = head;
        head = node;
    }

    static bool remove(Node*& head, const std::string& plate) {
        Node* current = head;
        Node* previous = nullptr;

        while (current != nullptr) {
            if (current->data.toString() == plate) {
                if (previous == nullptr) head = current->next;
                else previous->next = current->next;
                delete current;
                return true;
            }
            previous = current;
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
            Node* old = head;
            head = head->next;
            delete old;
        }
    }
};

#endif
