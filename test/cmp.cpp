#include <ctime>  
#include <iostream>
#include <set>

#include "src.hpp"
#define ll long long

inline ll read() {
    ll x = 0;
    bool f = 1;
    char c;
    while ((c = getchar()) < '0' || c > '9') {
        if (c == '-') f = 0;
        if (c == EOF) return 0;
    }
    while (c >= '0' && c <= '9') {
        x = (x << 3) + (x << 1) + (c ^ 48);
        c = getchar();
        if (c == EOF) break;
    }
    return f ? x : -x;
}

int count_elements_in_range(const std::set<ll>& my_set, ll low, ll high) {
    auto it_low = my_set.lower_bound(low);
    auto it_high = my_set.upper_bound(high);
    return std::distance(it_low, it_high);
}

int main() {
    freopen("/home/starry/ESet/test/test0.in", "r", stdin);
    std::set<ll> s;
    ESet<ll> rbt;
    int op;
    ll a, b, c;
    int cnt0 = 0, cnt1 = 0, cnt2 = 0, cnt3 = 0, cnt4 = 0; 
    double s_time0 = 0, s_time1 = 0, s_time2 = 0, s_time3 = 0,
           s_time4 = 0;  
    double rbt_time0 = 0, rbt_time1 = 0, rbt_time2 = 0, rbt_time3 = 0,
           rbt_time4 = 0;  
    ll numinsert, numerase, numfind, numrange, numtraverse;
    while (scanf("%d", &op) != EOF) {
        switch (op) {
            case 0: {  // 插入
                a = read();
                cnt0++;
                clock_t start = clock();
                s.insert(a);
                clock_t end = clock();
                s_time0 += static_cast<double>(end - start) / CLOCKS_PER_SEC *
                           1e6;  
                start = clock();
                rbt.emplace(a);
                end = clock();
                rbt_time0 +=
                    static_cast<double>(end - start) / CLOCKS_PER_SEC * 1e6;
                break;
            }
            case 1: {  // 删除
                a = read();
                cnt1++;
                clock_t start = clock();
                s.erase(a);
                clock_t end = clock();
                s_time1 +=
                    static_cast<double>(end - start) / CLOCKS_PER_SEC * 1e6;
                start = clock();
                rbt.erase(a);
                end = clock();
                rbt_time1 +=
                    static_cast<double>(end - start) / CLOCKS_PER_SEC * 1e6;
                break;
            }
            case 2: {  // 查找
                a = read();
                cnt2++;
                clock_t start = clock();
                s.find(a);
                clock_t end = clock();
                s_time2 +=
                    static_cast<double>(end - start) / CLOCKS_PER_SEC * 1e6;
                start = clock();
                rbt.find(a);
                end = clock();
                rbt_time2 +=
                    static_cast<double>(end - start) / CLOCKS_PER_SEC * 1e6;
                break;
            }
            case 3: {  // “=” operation
                cnt3++;
                clock_t start = clock();
                std::set<ll> s_copy = s;  // 复制 std::set
                clock_t end = clock();
                s_time3 +=
                    static_cast<double>(end - start) / CLOCKS_PER_SEC * 1e6;
                start = clock();
                ESet<ll> rbt_copy = rbt;  // 复制 ESet
                end = clock();
                rbt_time3 +=
                    static_cast<double>(end - start) / CLOCKS_PER_SEC * 1e6;
                break;
            }
            case 4: {  // 遍历
                cnt4++;
                // 测试 std::set
                clock_t start = clock();
                for (auto it = s.begin(); it != s.end(); ++it) {
                    // 遍历操作
                }
                clock_t end = clock();
                s_time4 +=
                    static_cast<double>(end - start) / CLOCKS_PER_SEC * 1e6;
                // 测试 ESet
                start = clock();
                for (auto it = rbt.begin(); it != rbt.end(); ++it) {
                    // 遍历操作
                }
                end = clock();
                rbt_time4 +=
                    static_cast<double>(end - start) / CLOCKS_PER_SEC * 1e6;
                break;
            }
        }
    }

    std::cout << "Operation\tstd::set (us)\tRBT (us)\n";
    std::cout << "Insert totally: \t\t" <<cnt0<<" times " << (cnt0 ? s_time0 / cnt0 : 0) << "\t\t"
              << (cnt0 ? rbt_time0 / cnt0 : 0) << "\n";
    std::cout << "Erase totally: \t\t" <<cnt1<<" times "  << (cnt1 ? s_time1 / cnt1 : 0) << "\t\t"
              << (cnt1 ? rbt_time1 / cnt1 : 0) << "\n";
    std::cout << "Find totally: \t\t" <<cnt2<<" times" << (cnt2 ? s_time2 / cnt2 : 0) << "\t\t"
              << (cnt2 ? rbt_time2 / cnt2 : 0) << "\n";
    std::cout << "= operation totally: \t\t" <<cnt3<<" times"<< (cnt3 ? s_time3 / cnt3 : 0) << "\t\t"
              << (cnt3 ? rbt_time3 / cnt3 : 0) << "\n";
    std::cout << "Traverse totally: \t\t" <<cnt2<<" times" << (cnt4 ? s_time4 / cnt4 : 0) << "\t\t"
              << (cnt4 ? rbt_time4 / cnt4 : 0) << "\n";

    return 0;
}