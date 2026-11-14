#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <unordered_set>
#include <iomanip>

using namespace std;

struct Node {
    string data;
    Node* next;
    Node(const string& d) : data(d), next(nullptr) {}
};

class Sut {
private:
    int M;
    vector<Node*> table;

    int hashFunc(const string& s) const {
        unsigned long h = 0;
        for (char c : s) h = h * 31 + c;
        return h % M;
    }

public:
    Sut(int m = 1009) : M(m), table(m, nullptr) {}

    ~Sut() {
        for (int i = 0; i < M; ++i) {
            Node* cur = table[i];
            while (cur) {
                Node* tmp = cur;
                cur = cur->next;
                delete tmp;
            }
        }
    }

    bool insert(const string& s) {
        if (search(s)) return false;
        int idx = hashFunc(s);
        Node* n = new Node(s);
        n->next = table[idx];
        table[idx] = n;
        return true;
    }

    bool search(const string& s) const {
        int idx = hashFunc(s);
        Node* cur = table[idx];
        while (cur) {
            if (cur->data == s) return true;
            cur = cur->next;
        }
        return false;
    }
};

vector<string> bienDuyNhat(int N, uint32_t seed = 42) {
    mt19937 rng(seed);
    unordered_set<string> seen;
    seen.reserve(N * 2);
    vector<string> res;
    res.reserve(N);

    const string letters = "ABCDEFGHKLMNPSTUVXYZ";
    const string alnum = "0123456789ABCDEFGHKLMNPSTUVXYZ";

    while ((int)res.size() < N) {
        char buf[16];
        int tinh = rng() % 100;
        char c1 = letters[rng() % letters.size()];
        char c2 = alnum[rng() % alnum.size()];
        int seri = rng() % 100000;

        snprintf(buf, sizeof(buf), "%02d-%c%c-%05d", tinh, c1, c2, seri);
        string bs(buf);
        if (seen.insert(bs).second) {
            res.push_back(bs);
        }
    }
    return res;
}

void runBenchmark(int N, int M) {
    vector<string> data = bienDuyNhat(N);
    Sut ht(M);

    auto t1 = chrono::steady_clock::now();
    for (const auto& bs : data) {
        ht.insert(bs);
    }
    auto t2 = chrono::steady_clock::now();
    double insert_ms = chrono::duration<double, milli>(t2 - t1).count();

    // Đo thời gian Search
    int found = 0;
    auto t3 = chrono::steady_clock::now();
    for (const auto& bs : data) {
        if (ht.search(bs)) found++;
    }
    auto t4 = chrono::steady_clock::now();
    double search_ms = chrono::duration<double, milli>(t4 - t3).count();

    cout << "N = " << setw(7) << N
         << " | M = " << setw(6) << M
         << " | Insert: " << fixed << setprecision(3) << setw(8) << insert_ms << " ms"
         << " | Search: " << fixed << setprecision(3) << setw(8) << search_ms << " ms"
         << " | Found: " << found << "/" << N << "\n";
}

int main() {
    vector<int> sizes_N = {1000, 10000, 100000};
    vector<int> sizes_M = {1, 101, 1009, 10007};

    cout << "=== BENCHMARK QUAN LY BIEN SO XE ===\n";
    for (int N : sizes_N) {
        for (int M : sizes_M) {
            runBenchmark(N, M);
        }
        cout << "-----------------------------------------------------------------------\n";
    }
    return 0;
}