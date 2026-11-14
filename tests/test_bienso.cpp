#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

using namespace std;

class QuanLyBienSo {
private:
    struct Node {
        string bs;
        Node* next;
        Node(const string& b) : bs(b), next(nullptr) {}
    };

    int M;
    vector<Node*> table;

    int hashFunc(const string& s) const {
        unsigned long h = 0;
        for (char c : s) h = h * 31 + c;
        return h % M;
    }

    bool hopLe(const string& bs) const {
        if (bs.size() != 11) return false;
        if (bs[2] != '-' || bs[5] != '-') return false;
        if (!isdigit(bs[0]) || !isdigit(bs[1])) return false;
        if (!isupper(bs[3]) || (!isupper(bs[4]) && !isdigit(bs[4]))) return false;
        for (int i = 6; i < 11; ++i) {
            if (!isdigit(bs[i])) return false;
        }
        return true;
    }

public:
    QuanLyBienSo(int m = 1009) : M(m), table(m, nullptr) {}

    ~QuanLyBienSo() {
        clear();
    }

    void clear() {
        for (int i = 0; i < M; ++i) {
            Node* cur = table[i];
            while (cur) {
                Node* tmp = cur;
                cur = cur->next;
                delete tmp;
            }
            table[i] = nullptr;
        }
    }

    bool dangKy(const string& bs) {
        if (!hopLe(bs) || traCuu(bs)) return false;
        int idx = hashFunc(bs);
        Node* n = new Node(bs);
        n->next = table[idx];
        table[idx] = n;
        return true;
    }

    bool traCuu(const string& bs) const {
        if (!hopLe(bs)) return false;
        int idx = hashFunc(bs);
        Node* cur = table[idx];
        while (cur) {
            if (cur->bs == bs) return true;
            cur = cur->next;
        }
        return false;
    }

    vector<string> dsXa(const string& maTinh) const {
        vector<string> res;
        for (int i = 0; i < M; ++i) {
            Node* cur = table[i];
            while (cur) {
                if (cur->bs.substr(0, 2) == maTinh) {
                    string xa = cur->bs.substr(3, 2);
                    if (find(res.begin(), res.end(), xa) == res.end()) {
                        res.push_back(xa);
                    }
                }
                cur = cur->next;
            }
        }
        sort(res.begin(), res.end());
        return res;
    }

    vector<string> dsBienSo(const string& maTinh, const string& maXa) const {
        vector<string> res;
        for (int i = 0; i < M; ++i) {
            Node* cur = table[i];
            while (cur) {
                if (cur->bs.substr(0, 2) == maTinh && cur->bs.substr(3, 2) == maXa) {
                    res.push_back(cur->bs);
                }
                cur = cur->next;
            }
        }
        sort(res.begin(), res.end());
        return res;
    }

    bool docFile(const string& path) {
        ifstream f(path);
        if (!f.is_open()) return false;
        string line;
        while (getline(f, line)) {
            while (!line.empty() && (line.back() == '\r' || line.back() == '\n' || line.back() == ' ')) {
                line.pop_back();
            }
            if (!line.empty()) dangKy(line);
        }
        return true;
    }

    bool ghiFile(const string& path) const {
        ofstream f(path);
        if (!f.is_open()) return false;
        for (int i = 0; i < M; ++i) {
            Node* cur = table[i];
            while (cur) {
                f << cur->bs << "\n";
                cur = cur->next;
            }
        }
        return true;
    }
};

struct TestResult {
    string id;
    string category;
    string desc;
    bool passed;
};

vector<TestResult> results;

void record(const string& id, const string& cat, const string& desc, bool cond) {
    results.push_back({id, cat, desc, cond});
    cout << (cond ? "[PASS] " : "[FAIL] ") << id << " (" << cat << "): " << desc << "\n";
}

int main() {
    {
        QuanLyBienSo ql;
        record("TC01", "Normal", "Dang ky bien so hop le", ql.dangKy("59-B1-12345"));
        record("TC02", "Normal", "Tra cuu bien so ton tai", ql.traCuu("59-B1-12345"));
        record("TC03", "Normal", "Tra cuu bien so khong ton tai", !ql.traCuu("59-B1-99999"));

        ql.dangKy("59-C2-11111");
        ql.dangKy("59-B1-22222");
        vector<string> xas = ql.dsXa("59");
        record("TC04", "Normal", "Lay danh sach ma xa theo tinh", xas.size() == 2 && xas[0] == "B1" && xas[1] == "C2");

        vector<string> bss = ql.dsBienSo("59", "B1");
        record("TC05", "Normal", "Lay danh sach bien so theo tinh va xa", bss.size() == 2 && bss[0] == "59-B1-12345");
    }
    {
        QuanLyBienSo ql;
        record("TC08", "Boundary", "Tra cuu khi bang rong", !ql.traCuu("59-B1-12345"));
        record("TC09", "Boundary", "Bien so bien duoi 00-A0-00000", ql.dangKy("00-A0-00000") && ql.traCuu("00-A0-00000"));
        record("TC10", "Boundary", "Bien so bien tren 99-ZZ-99999", ql.dangKy("99-ZZ-99999") && ql.traCuu("99-ZZ-99999"));
        record("TC11", "Boundary", "dsXa tinh khong ton tai", ql.dsXa("88").empty());
        record("TC12", "Boundary", "dsBienSo xa khong ton tai", ql.dsBienSo("99", "AA").empty());
    }
    {
        QuanLyBienSo ql(5);
        for (int i = 0; i < 100; ++i) {
            char buf[16];
            snprintf(buf, sizeof(buf), "51-F1-%05d", i);
            ql.dangKy(buf);
        }
        record("TC13", "Collision", "Xu ly va cham nhieu phan tu tren bang nho", ql.traCuu("51-F1-00000") && ql.traCuu("51-F1-00099"));
        record("TC14", "Duplicate", "Chan dang ky trung lap", !ql.dangKy("51-F1-00050"));
    }
    {
        QuanLyBienSo ql;
        record("TC15", "Invalid", "Chuoi rong", !ql.dangKy(""));
        record("TC16", "Invalid", "Thieu ky tu so", !ql.dangKy("59-B1-1234"));
        record("TC17", "Invalid", "Thua ky tu so", !ql.dangKy("59-B1-123456"));
        record("TC18", "Invalid", "Sai ky tu tinh", !ql.dangKy("AB-B1-12345"));
        record("TC19", "Invalid", "Thieu dau gach ngang", !ql.dangKy("59B112345"));
        record("TC20", "Invalid", "Chu thuong khong hop le", !ql.dangKy("59-b1-12345"));
    }
    {
        QuanLyBienSo ql1;
        ql1.dangKy("29-A1-00001");
        ql1.dangKy("30-E2-00002");
        bool wOk = ql1.ghiFile("temp_test.txt");

        QuanLyBienSo ql2;
        bool rOk = ql2.docFile("temp_test.txt");
        record("TC23", "File", "Doc file khong ton tai", !ql2.docFile("khong_ton_tai_xyz.txt"));
        record("TC24", "File", "Ghi va doc lai file (Round-trip)", wOk && rOk && ql2.traCuu("29-A1-00001") && ql2.traCuu("30-E2-00002"));
        remove("temp_test.txt");
    }
    {
        QuanLyBienSo ql;
        for (int i = 0; i < 5000; ++i) {
            char buf[16];
            snprintf(buf, sizeof(buf), "43-D1-%05d", i);
            ql.dangKy(buf);
        }
        ql.clear();
        record("ML01", "Memory", "Xoa sach bang bam va tai su dung", !ql.traCuu("43-D1-00000") && ql.dangKy("43-D1-00000"));
    }

    ofstream csv("ketqua_test.csv");
    csv << "TestID,Category,Description,Status\n";
    int passCount = 0;
    for (const auto& r : results) {
        if (r.passed) passCount++;
        csv << r.id << "," << r.category << ",\"" << r.desc << "\"," << (r.passed ? "PASS" : "FAIL") << "\n";
    }
    csv.close();

    cout << "\nTong ket: " << passCount << "/" << results.size() << " PASS. Da xuat ketqua_test.csv\n";
    return 0;
}