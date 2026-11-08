#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <limits>
#include <string>
#include "../io/FileIO.h"

class Menu {
public:
    explicit Menu(LicensePlateService& service) : service(service) {}

    void run() {
        int choice = -1;
        do {
            show();
            std::cout << "Chon chuc nang: ";
            if (!(std::cin >> choice)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Lua chon khong hop le.\n";
                continue;
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            handle(choice);
        } while (choice != 0);
    }

private:
    LicensePlateService& service;

    void show() const {
        std::cout << "\n===== QUAN LY BIEN SO XE =====\n"
                  << "1. Dang ky bien so\n"
                  << "2. Tra cuu bien so\n"
                  << "3. Danh sach ma xa theo tinh (TODO)\n"
                  << "4. Danh sach bien so theo xa (TODO)\n"
                  << "5. Nap du lieu tu file\n"
                  << "6. Luu du lieu ra file\n"
                  << "0. Thoat\n";
    }

    void handle(int choice) {
        std::string input;
        switch (choice) {
        case 1:
            std::cout << "Nhap bien so (29-A1-12345): ";
            std::getline(std::cin, input);
            std::cout << (service.registerPlate(input) ? "Dang ky thanh cong.\n"
                                                       : "Du lieu sai hoac bien so da ton tai.\n");
            break;
        case 2: {
            std::cout << "Nhap bien so can tim: ";
            std::getline(std::cin, input);
            const BienSoXe* found = service.searchPlate(input);
            std::cout << (found ? "Tim thay: " + found->toString() + "\n"
                                : "Khong tim thay.\n");
            break;
        }
        case 5:
            std::cout << (FileIO::loadFromFile("data/data_bienso.txt", service)
                              ? "Nap file xong.\n" : "Khong mo duoc file.\n");
            break;
        case 6:
            std::cout << (FileIO::saveToFile("data/data_bienso.txt", service.data())
                              ? "Luu file thanh cong.\n" : "Luu file that bai.\n");
            break;
        case 0:
            std::cout << "Ket thuc chuong trinh.\n";
            break;
        default:
            std::cout << "Chuc nang chua hop le hoac chua duoc hien thuc.\n";
        }
    }
};

#endif
