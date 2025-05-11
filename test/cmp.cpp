#include <ctime>
#include <iostream>
#include <set>

#include "AVL.hpp"
#include "FHQtreap.hpp"
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

int main() {
    freopen("/home/starry/ESet/test/test0.in", "r", stdin);
    std::set<ll> s;
    ESet<ll> rbt;
    AVL_SET::ESet<ll> avl;
    FHQ_treap::ESet<ll> treap;
    int op;
    ll a, b, c;
    int cnt0 = 0, cnt1 = 0, cnt2 = 0, cnt3 = 0, cnt4 = 0;
    long double s_time0 = 0, s_time1 = 0, s_time2 = 0, s_time3 = 0, s_time4 = 0;
    double rbt_time0 = 0, rbt_time1 = 0, rbt_time2 = 0, rbt_time3 = 0,
           rbt_time4 = 0;
    double avl_time0 = 0, avl_time1 = 0, avl_time2 = 0, avl_time3 = 0,
           avl_time4 = 0;
    double treap_time0 = 0, treap_time1 = 0, treap_time2 = 0, treap_time3 = 0,
           treap_time4 = 0;
    // test emplace
    clock_t start = clock();
    cnt0 = 3000000;
    for (int i = 0; i < cnt0; i++) {
        a = rand() % 3000000;
        s.insert(a);
    }
    clock_t end = clock();
    s_time0 = (double)(end - start)/CLOCKS_PER_SEC;

    start = clock();
    for (int i = 0; i < cnt0; i++) {
        a = rand() % 3000000;
        rbt.emplace(a);
    }
    end = clock();
    rbt_time0 = (double)(end - start)/CLOCKS_PER_SEC;

    start = clock();
    for (int i = 0; i < cnt0; i++) {
        a = rand() % 3000000;
        avl.emplace(a);
    }
    end = clock();
    avl_time0 = (double)(end - start)/CLOCKS_PER_SEC;

    start = clock();
    for (int i = 0; i < cnt0; i++) {
        a = rand() % 3000000;
        treap.emplace(a);
    }
    end = clock();
    treap_time0 = (double)(end - start)/CLOCKS_PER_SEC;
    
    // test erase
    start = clock();
    cnt1 = 500000;
    for (int i = 0; i < cnt1; i++) {
        a = rand() % cnt1*3;
        s.erase(a);
    }
    end = clock();
    s_time1 = static_cast<double>(end - start);

    start = clock();
    for (int i = 0; i < cnt1; i++) {
        a = rand() % cnt1*3;
        rbt.erase(a);
    }
    end = clock();
    rbt_time1 = static_cast<double>(end - start);

    start = clock();
    for (int i = 0; i < cnt1; i++) {
        a = rand() % cnt1*3;
        avl.erase(a);
    }
    end = clock();
    avl_time1 = static_cast<double>(end - start);

    start = clock();
    for (int i = 0; i < cnt1; i++) {
        a = rand() % cnt1*3;
        treap.erase(a);
    }
    end = clock();
    treap_time1 = static_cast<double>(end - start);

    // test find
    start = clock();
    cnt2 = 1000000;
    for (int i = 0; i < cnt2; i++) {
        a = rand() % 3000000;
        s.find(a);
    }
    end = clock();
    s_time2 = static_cast<double>(end - start);
    start = clock();    
    for (int i = 0; i < cnt2; i++) {
        a = rand() % 3000000;
        rbt.find(a);
    }
    end = clock();
    rbt_time2 = static_cast<double>(end - start);
    start = clock();
    for (int i = 0; i < cnt2; i++) {
        a = rand() % 3000000;
        avl.find(a);
    }
    end = clock();
    avl_time2 = static_cast<double>(end - start);   
    start = clock();
    for (int i = 0; i < cnt2; i++) {
        a = rand() % 3000000;
        treap.find(a);
    }
    end = clock();
    treap_time2 = static_cast<double>(end - start); 

    // test copy
    start = clock();
    cnt3 = 30;
    for (int i = 0; i < cnt3; i++) {
        std::set<ll> s_copy = s;  
    }
    end = clock();
    s_time3 = static_cast<double>(end - start)/CLOCKS_PER_SEC;

    start = clock();
    for (int i = 0; i < cnt3; i++) {
        ESet<ll> eset_copy = rbt;  
    }
    end = clock();
    rbt_time3 = static_cast<double>(end - start)/CLOCKS_PER_SEC;

    start = clock();
    for (int i = 0; i < cnt3; i++) {
        AVL_SET::ESet<ll> avl_copy = avl;  
    }
    end = clock();
    avl_time3 = static_cast<double>(end - start)/CLOCKS_PER_SEC;
    
    start = clock();
    for (int i = 0; i < cnt3; i++) {
        FHQ_treap::ESet<ll> treap_copy = treap;  
    }
    end = clock();
    treap_time3 = static_cast<double>(end - start);

    // test traversal
    start = clock();
    cnt4 = 30;
    for (int i = 0; i < cnt4; i++) {
        for (auto it = s.begin(); it != s.end(); ++it) {
        }
    }
    end = clock();
    s_time4 = static_cast<double>(end - start)/CLOCKS_PER_SEC;

    start = clock();
    for (int i = 0; i < cnt4; i++) {
        for (auto it = rbt.begin(); it != rbt.end(); ++it) {
        }
    }
    end = clock();
    rbt_time4 = static_cast<double>(end - start)/CLOCKS_PER_SEC;

    start = clock();
    for (int i = 0; i < cnt4; i++) {
        for (auto it = avl.begin(); it != avl.end(); ++it) {
        }
    }
    end = clock();
    avl_time4 = static_cast<double>(end - start)/CLOCKS_PER_SEC;

    start = clock();
    for (int i = 0; i < cnt4; i++) {
        for (auto it = treap.begin(); it != treap.end(); ++it) {
        }
    }
    end = clock();
    treap_time4 = static_cast<double>(end - start)/CLOCKS_PER_SEC;

    std::cout << "Operation\tstd::set (us)\tRBT (us)\n";
    std::cout << "Insert totally: \t\t" << cnt0 << " times "
              << (cnt0 ? s_time0 / cnt0 : 0) << "\t\t"
              << (cnt0 ? rbt_time0 / cnt0 : 0) << "\t\t"
              << (cnt0 ? avl_time0 / cnt0 : 0) << "\t\t"
              << (cnt0 ? treap_time0 / cnt0 : 0) << "\n";
    std::cout << "Erase totally: \t\t" << cnt1 << " times "
              << (cnt1 ? s_time1 / cnt1 : 0) << "\t\t"
              << (cnt1 ? rbt_time1 / cnt1 : 0) << "\t\t"
              << (cnt1 ? avl_time1 / cnt1 : 0) << "\t\t"
              << (cnt1 ? treap_time1 / cnt1 : 0) << "\n";
    std::cout << "Find totally: \t\t" << cnt2 << " times"
              << (cnt2 ? s_time2 / cnt2 : 0) << "\t\t"
              << (cnt2 ? rbt_time2 / cnt2 : 0) << "\t\t"
              << (cnt2 ? avl_time2 / cnt2 : 0) << "\t\t"
              << (cnt2 ? treap_time2 / cnt2 : 0) << "\n";
    std::cout << "= operation totally: \t\t" << cnt3 << " times"
              << (cnt3 ? s_time3 / cnt3 : 0) << "\t\t"
              << (cnt3 ? rbt_time3 / cnt3 : 0) << "\t\t"
              << (cnt3 ? avl_time3 / cnt3 : 0) << "\t\t"
              << (cnt3 ? treap_time3 / cnt3 : 0) << "\n";
    std::cout << "Traverse totally: \t\t" << cnt4 << " times"
              << (cnt4 ? s_time4 / cnt4 : 0) << "\t\t"
              << (cnt4 ? rbt_time4 / cnt4 : 0) << "\t\t"
              << (cnt4 ? avl_time4 / cnt4 : 0) << "\t\t"
              << (cnt4 ? treap_time4 / cnt4 : 0) << "\n";
    return 0;
}