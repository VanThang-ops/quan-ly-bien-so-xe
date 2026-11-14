#ifndef LINKED_LIST_H
#define LINKED_LIST_H
#include <string>
#include "../models/BienSo.h"
struct Node {
    BienSoXe data;
    Node* next;
};
struct LinkedList {
    static bool contains(const Node* head, const std::string& plate) {
        const Node* current = head;
        while (current != nullptr) {
            if (current->data.toString() == plate) return true;
            current = current->next;
        }
        return false;
    }
    static void insertFront(Node*& head, const BienSoXe& value) {
        Node* newNode = new Node;
        newNode->data = value;
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
    static void clear(Node*& head) {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }
};
#endif
