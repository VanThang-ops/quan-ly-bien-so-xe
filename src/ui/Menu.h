#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <limits>
#include <string>
#include <vector>
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
                std::cin.ignore(
                    std::numeric_limits<std::streamsize>::max(), '\n'
                );

                std::cout << "Lua chon khong hop le.\n";
                continue;
            }

            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(), '\n'
            );

            handle(choice);

        } while (choice != 0);
    }

private:
    LicensePlateService& service;
    const std::string filePath = "data/data_bienso.txt";

    void show() const {
        std::cout << "\n===== QUAN LY BIEN SO XE =====\n"
                  << "1. Dang ky bien so\n"
                  << "2. Tra cuu bien so\n"
                  << "3. Danh sach ma xa theo tinh\n"
                  << "4. Danh sach bien so theo xa\n"
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

            std::cout << (
                service.registerPlate(input)
                ? "Dang ky thanh cong.\n"
                : "Du lieu sai hoac bien so da ton tai.\n"
            );
            break;

        case 2: {
            std::cout << "Nhap bien so can tim: ";
            std::getline(std::cin, input);

            const BienSoXe* found = service.searchPlate(input);

            if (found != nullptr) {
                std::cout << "Tim thay: " << found->toString() << '\n';
            } else {
                std::cout << "Khong tim thay bien so.\n";
            }
            break;
        }

        case 3: {
            std::cout << "Nhap ma tinh (2 chu so, VD: 29): ";
            std::getline(std::cin, input);

            std::string maTinh;
            if (!parseMaTinh(input, maTinh)) {
                std::cout << "Ma tinh khong hop le (phai gom 2 chu so).\n";
                break;
            }

            const std::vector<std::string> communes =
                service.getCommuneCodesByProvince(maTinh);

            if (communes.empty()) {
                std::cout << "Khong co ma xa nao thuoc tinh " << maTinh << ".\n";
            } else {
                std::cout << "Danh sach ma xa thuoc tinh " << maTinh
                          << " (" << communes.size() << " xa):\n";
                for (const std::string& xa : communes) {
                    std::cout << " - " << xa << '\n';
                }
            }
            break;
        }

        case 4: {
            std::string rawTinh, rawXa, maTinh, maXa;
            std::cout << "Nhap ma tinh (2 chu so, VD: 29): ";
            std::getline(std::cin, rawTinh);
            if (!parseMaTinh(rawTinh, maTinh)) {
                std::cout << "Ma tinh khong hop le (phai gom 2 chu so).\n";
                break;
            }

            std::cout << "Nhap ma xa (2 ky tu chu/so, VD: A1): ";
            std::getline(std::cin, rawXa);
            if (!parseMaXa(rawXa, maXa)) {
                std::cout << "Ma xa khong hop le (phai gom 2 ky tu chu hoac so).\n";
                break;
            }

            const std::vector<BienSoXe> plates =
                service.getPlatesByCommune(maTinh, maXa);

            if (plates.empty()) {
                std::cout << "Khong co bien so nao thuoc tinh "
                          << maTinh << ", xa " << maXa << ".\n";
            } else {
                std::cout << "Danh sach bien so thuoc " << maTinh << "-" << maXa
                          << " (" << plates.size() << " bien so):\n";
                for (const BienSoXe& p : plates) {
                    std::cout << " - " << p.toString() << '\n';
                }
            }
            break;
        }

        case 5:
            if (FileIO::loadFromFile(filePath, service)) {
                std::cout << "Nap du lieu tu file thanh cong.\n";
            } else {
                std::cout << "Khong the doc file du lieu.\n";
            }
            break;

        case 6:
            if (FileIO::saveToFile(filePath, service.data())) {
                std::cout << "Luu du lieu thanh cong.\n";
            } else {
                std::cout << "Luu du lieu that bai.\n";
            }
            break;

        case 0:
            std::cout << "Ket thuc chuong trinh.\n";
            break;

        default:
            std::cout << "Vui long chon lai chuc nang.\n";
        }
    }
};

#endif
