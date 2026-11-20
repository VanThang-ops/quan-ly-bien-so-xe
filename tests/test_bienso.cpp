#include <algorithm>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "../src/io/FileIO.h"
#include "../src/services/LicensePlateService.h"

struct TestResult {
    std::string id;
    std::string category;
    std::string desc;
    bool passed;
};

static std::vector<TestResult> results;

static void record(const std::string& id, const std::string& cat,
                   const std::string& desc, bool cond) {
    results.push_back({id, cat, desc, cond});
    std::cout << (cond ? "[PASS] " : "[FAIL] ") << id << " (" << cat
              << "): " << desc << "\n";
}

int main() {
    std::cout << "=== CHAY BO KIEM THU TU DONG HE THONG QUAN LY BIEN SO XE ===\n";

    // 1. Nhom Normal (Chuc nang co ban va chuan hoa)
    {
        LicensePlateService service;
        record("TC01", "Normal", "Dang ky bien so hop le (59-B1-12345)",
               service.registerPlate("59-B1-12345") && service.data().getSize() == 1);

        const BienSoXe* found = service.searchPlate("59-B1-12345");
        record("TC02", "Normal", "Tra cuu bien so ton tai (59-B1-12345)",
               found != nullptr && found->toString() == "59-B1-12345");

        record("TC03", "Normal", "Tra cuu bien so khong ton tai (59-B1-99999)",
               service.searchPlate("59-B1-99999") == nullptr);

        service.registerPlate("59-C2-11111");
        service.registerPlate("59-B1-22222");
        std::vector<std::string> xas = service.getCommuneCodesByProvince("59");
        std::sort(xas.begin(), xas.end());
        record("TC04", "Normal", "Lay danh sach ma xa theo tinh (khu trung B1)",
               xas.size() == 2 && xas[0] == "B1" && xas[1] == "C2");

        std::vector<BienSoXe> bss = service.getPlatesByCommune("59", "B1");
        record("TC05", "Normal", "Lay danh sach bien so theo tinh 59 va xa B1",
               bss.size() == 2);

        record("TC06", "Normal", "Chuan hoa ma xa chu thuong (29-a1-12345 -> 29-A1-12345)",
               service.registerPlate("29-a1-12345") &&
               service.searchPlate("29-A1-12345") != nullptr &&
               service.searchPlate("29-a1-12345") != nullptr);

        record("TC07", "Normal", "Cat khoang trang thua hai dau ('  29-B2-12345  ')",
               service.registerPlate("  29-B2-12345  ") &&
               service.searchPlate("29-B2-12345") != nullptr);
    }

    // 2. Nhom Boundary (Truong hop bien)
    {
        LicensePlateService service;
        record("TC08", "Boundary", "Tra cuu khi bang bam dang rong",
               service.searchPlate("59-B1-12345") == nullptr);

        record("TC09", "Boundary", "Bien so bien duoi 00-A0-00000",
               service.registerPlate("00-A0-00000") &&
               service.searchPlate("00-A0-00000") != nullptr);

        record("TC10", "Boundary", "Bien so bien tren 99-ZZ-99999",
               service.registerPlate("99-ZZ-99999") &&
               service.searchPlate("99-ZZ-99999") != nullptr);

        record("TC11", "Boundary", "getCommuneCodesByProvince voi tinh khong ton tai (88)",
               service.getCommuneCodesByProvince("88").empty());

        record("TC12", "Boundary", "getPlatesByCommune voi xa khong ton tai (99, AA)",
               service.getPlatesByCommune("99", "AA").empty());
    }

    // 3. Nhom Collision & Duplicate (Va cham Separate Chaining & Chong trung)
    {
        LicensePlateService service;
        for (int i = 0; i < 300; ++i) {
            char buf[16];
            std::snprintf(buf, sizeof(buf), "51-F1-%05d", i);
            service.registerPlate(buf);
        }
        record("TC13", "Collision",
               "Va cham Separate Chaining (300 bien so tren M=101 bucket)",
               service.data().getSize() == 300 &&
               service.data().maxBucketSize() > 1 &&
               service.searchPlate("51-F1-00000") != nullptr &&
               service.searchPlate("51-F1-00299") != nullptr);

        record("TC14", "Duplicate",
               "Chan dang ky trung lap (ke ca khac hoa/thuong 51-f1-00050)",
               !service.registerPlate("51-F1-00050") &&
               !service.registerPlate("51-f1-00050") &&
               service.data().getSize() == 300);
    }

    // 4. Nhom Invalid (Du lieu khong hop le)
    {
        LicensePlateService service;
        record("TC15", "Invalid", "Chuoi rong hoac toan khoang trang",
               !service.registerPlate("") && !service.registerPlate("   "));
        record("TC16", "Invalid", "Thieu ky tu so seri (59-B1-1234)",
               !service.registerPlate("59-B1-1234"));
        record("TC17", "Invalid", "Thua ky tu so seri (59-B1-123456)",
               !service.registerPlate("59-B1-123456"));
        record("TC18", "Invalid", "Sai ky tu ma tinh (AB-B1-12345)",
               !service.registerPlate("AB-B1-12345"));
        record("TC19", "Invalid", "Thieu dau gach ngang (59B112345)",
               !service.registerPlate("59B112345"));
        record("TC20", "Invalid", "Ma xa chua ky tu dac biet (59-B!-12345)",
               !service.registerPlate("59-B!-12345"));
        record("TC21", "Invalid", "So seri chua chu cai (59-B1-12B45)",
               !service.registerPlate("59-B1-12B45"));
    }

    // 5. Nhom Structure & File I/O & Memory
    {
        HashTable ht;
        BienSoXe p1{"29", "A1", "12345"};
        BienSoXe p2{"29", "A1", "23456"};
        ht.insert(p1);
        ht.insert(p2);
        bool removed = ht.remove("29-A1-12345");
        record("TC22", "Structure",
               "Xoa node trong HashTable::remove va kiem tra kich thuoc",
               removed && ht.getSize() == 1 &&
               ht.find("29-A1-12345") == nullptr &&
               ht.find("29-A1-23456") != nullptr);
    }
    {
        LicensePlateService s1;
        s1.registerPlate("29-A1-00001");
        s1.registerPlate("30-E2-00002");
        const std::string tempPath = "temp_test_bienso.txt";
        bool wOk = FileIO::saveToFile(tempPath, s1.data());

        LicensePlateService s2;
        record("TC23", "File", "Doc file khong ton tai tra ve false",
               !FileIO::loadFromFile("khong_ton_tai_xyz.txt", s2));

        bool rOk = FileIO::loadFromFile(tempPath, s2);
        record("TC24", "File", "Ghi va doc lai file (Round-trip 100% du lieu)",
               wOk && rOk && s2.data().getSize() == 2 &&
               s2.searchPlate("29-A1-00001") != nullptr &&
               s2.searchPlate("30-E2-00002") != nullptr);
        std::remove(tempPath.c_str());
    }
    {
        HashTable ht;
        for (int i = 0; i < 5000; ++i) {
            char seri[16];
            std::snprintf(seri, sizeof(seri), "%05d", i);
            ht.insert(BienSoXe{"43", "D1", seri});
        }
        ht.clear();
        record("ML01", "Memory",
               "Giai phong 5000 Node bang HashTable::clear() va tai su dung",
               ht.getSize() == 0 && ht.usedBuckets() == 0 &&
               ht.insert(BienSoXe{"43", "D1", "00000"}));
    }

    int passCount = 0;
    for (const auto& r : results) {
        if (r.passed) ++passCount;
    }

    std::cout << "\nTong ket: " << passCount << "/" << results.size()
              << " Test Cases PASS.\n";
    return passCount == static_cast<int>(results.size()) ? 0 : 1;
}
