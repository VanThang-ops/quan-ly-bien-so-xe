#include <chrono>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "../src/services/LicensePlateService.h"

// Sinh danh sách N biển số xe ngẫu nhiên không trùng lặp theo chuẩn ab-cd-ghxyz
static std::vector<std::string> generateUniquePlates(int N, unsigned int seed = 42) {
    std::mt19937 rng(seed);
    std::vector<std::string> res;
    res.reserve(static_cast<std::size_t>(N));

    const std::string letters = "ABCDEFGHKLMNPSTUVXYZ";
    const std::string digits = "0123456789";

    for (int i = 0; i < N; ++i) {
        int tinh = 10 + (i % 90);
        char c1 = letters[(i / 90) % letters.size()];
        char c2 = digits[(i / (90 * static_cast<int>(letters.size()))) % 10];
        int seri = (i * 73 + static_cast<int>(rng() % 97)) % 100000;

        // Đảm bảo tính duy nhất tuyệt đối theo chỉ số i khi N <= 100000
        seri = i % 100000;

        char buf[16];
        std::snprintf(buf, sizeof(buf), "%02d-%c%c-%05d", tinh, c1, c2, seri);
        res.emplace_back(buf);
    }
    return res;
}

static void runBenchmark(int N) {
    const std::vector<std::string> data = generateUniquePlates(N);
    LicensePlateService service;

    const auto t1 = std::chrono::steady_clock::now();
    for (const auto& bs : data) {
        service.registerPlate(bs);
    }
    const auto t2 = std::chrono::steady_clock::now();
    const double insertMs =
        std::chrono::duration<double, std::milli>(t2 - t1).count();

    int found = 0;
    const auto t3 = std::chrono::steady_clock::now();
    for (const auto& bs : data) {
        if (service.searchPlate(bs) != nullptr) ++found;
    }
    const auto t4 = std::chrono::steady_clock::now();
    const double searchMs =
        std::chrono::duration<double, std::milli>(t4 - t3).count();

    const HashTable& ht = service.data();
    std::cout << "N = " << std::setw(6) << N
              << " | M = " << std::setw(3) << HashTable::TABLE_SIZE
              << " | Alpha = " << std::fixed << std::setprecision(2) << std::setw(6) << ht.loadFactor()
              << " | Used = " << std::setw(3) << ht.usedBuckets() << "/101"
              << " | MaxChain = " << std::setw(3) << ht.maxBucketSize()
              << " | Insert: " << std::setprecision(3) << std::setw(7) << insertMs << " ms"
              << " | Search: " << std::setprecision(3) << std::setw(7) << searchMs << " ms"
              << " | Found: " << found << "/" << N << "\n";
}

int main() {
    const std::vector<int> sizes = {50, 100, 500, 1000, 5000, 10000};

    std::cout << "=== BENCHMARK HIEU NANG HASH TABLE (DJB2, M = 101, SEPARATE CHAINING) ===\n";
    for (int N : sizes) {
        runBenchmark(N);
    }
    return 0;
}
