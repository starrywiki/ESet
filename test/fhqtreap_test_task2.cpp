//可持久化无旋treap 每次split变化时都要开辟新的节点
#pragma GCC optimize("Ofast")
#include <cassert>
#include <ctime>
#include <iostream>
#include <random>
#include <stack>
#include <stdexcept>
#include <unordered_map>
#include <vector>
#define ll long long

std::mt19937 rng(std::time(nullptr));

template <typename T>
class MemoryPool {
   private:
    std::vector<T*> pool;
    std::vector<T*> freeList;
    size_t blockSize;

   public:
    MemoryPool(size_t initialSize = 1024, size_t blockSize = 1024)
        : blockSize(blockSize) {
        expandPool(initialSize);
    }

    ~MemoryPool() {
        for (auto ptr : pool) {
            delete[] ptr;
        }
    }

    T* allocate() {
        if (freeList.empty()) {
            expandPool(blockSize);
        }
        T* obj = freeList.back();
        freeList.pop_back();
        return obj;
    }

    void deallocate(T* obj) { freeList.push_back(obj); }

   private:
    void expandPool(size_t size) {
        T* block = new T[size];
        pool.push_back(block);
        for (size_t i = 0; i < size; ++i) {
            freeList.push_back(&block[i]);
        }
    }
};
inline void read(long long& x) {
    char ch = getchar();
    long long f = 1;
    x = 0;
    while (!isdigit(ch) && ch ^ '-') ch = getchar();
    if (ch == '-') f = -1, ch = getchar();
    while (isdigit(ch)) x = x * 10 + ch - '0', ch = getchar();
    x *= f;
}
inline void write(long long x) {
    static int sta[35];
    int top = 0;
    do {
        sta[top++] = x % 10, x /= 10;
    } while (x);
    while (top) putchar(sta[--top] + 48);
    putchar('\n');
}
template <class key>
class ESet {
   public:
    struct Node {
        Node *ls, *rs;
        key val;
        int lev, siz;
        int ref_count = 1;
        Node() : ls(nullptr), rs(nullptr), val(0), lev(0), siz(0) {}
        Node(key v) {
            val = v;
            lev = rng();
            siz = 1;
            ls = rs = nullptr;
        }
        Node(key v, int lev, Node* ls, Node* rs)
            : val(v), lev(lev), ls(ls), rs(rs) {
            siz = 1 + (ls ? ls->siz : 0) + (rs ? rs->siz : 0);
            if (ls) ls->ref_count++;
            if (rs) rs->ref_count++;
        }
        void update_siz() {
            siz = 1;
            if (ls) siz += ls->siz;
            if (rs) siz += rs->siz;
        }
    };
    Node* root;
    size_t num_elements;
    static MemoryPool<Node> nodePool;  // 内存池
    //按值分裂
    void split(Node* cur, const key& val, Node*& left, Node*& right) {
        if (!cur) {
            left = right = nullptr;
            return;
        }
        if (cur->val < val) {
            left = nodePool.allocate();
            *left = Node(cur->val, cur->lev, cur->ls, nullptr);
            split(cur->rs, val, left->rs, right);
            left->update_siz();
        } else {
            right = nodePool.allocate();
            *right = Node(cur->val, cur->lev, nullptr, cur->rs);
            split(cur->ls, val, left, right->ls);
            right->update_siz();
        }
    }

    Node* merge(Node* left, Node* right) {
        if (!left) return right;
        if (!right) return left;
        if (left->lev < right->lev) {
            left->rs = merge(left->rs, right);
            left->update_siz();
            return left;
        } else {
            right->ls = merge(left, right->ls);
            right->update_siz();
            return right;
        }
    }

    bool find(const key& curkey) {
        Node* cur = root;
        while (cur) {
            if (cur->val == curkey) return true;
            if (cur->val < curkey)
                cur = cur->rs;
            else
                cur = cur->ls;
        }
        return false;
    }
    Node* nxt(const key& curkey) {
        Node* cur = root;
        Node* ans = nullptr;
        while (cur) {
            if (cur->val > curkey) {
                ans = cur;
                cur = cur->ls;
            } else
                cur = cur->rs;
        }
        return ans;
    }
    Node* prev(const key& curkey) {
        Node* cur = root;
        Node* ans = nullptr;
        while (cur) {
            if (cur->val < curkey) {
                ans = cur;
                cur = cur->rs;
            } else
                cur = cur->ls;
        }
        return ans;
    }
    void clear(Node* cur) {
        if (cur) {
            if (--cur->ref_count == 0) {
                clear(cur->ls);
                clear(cur->rs);
                cur->ls = cur->rs = nullptr;
                if (cur) nodePool.deallocate(cur);
                cur = nullptr;
            }
        }
    }
    template <class... Args>
    bool emplace(Args&&... args) {
        key curkey(std::forward<Args>(args)...);
        if (find(curkey)) return false;
        Node* new_node = nodePool.allocate();
        *new_node = Node(curkey);
        Node *left = nullptr, *right = nullptr;
        split(root, curkey, left, right);
        clear(root);
        root = merge(merge(left, new_node), right);
        num_elements++;
        return true;
    }

    size_t erase(const key& curkey) {
        if (!find(curkey)) return 0;
        Node *left = nullptr, *mid = nullptr, *right = nullptr, *tmpright;
        split(root, curkey, left, right);
        clear(root);
        split(right, curkey + 1, mid, tmpright);
        clear(right);
        root = merge(left, tmpright);
        if (mid) {
            clear(mid);
            --num_elements;
        }
        return 1;
    }
    size_t range(const key& l, const key& r) {
        if (l > r) return 0;
        Node* cur = root;
        size_t cnt1 = 0;
        size_t cnt2 = 0;
        while (cur) {
            if (cur->val < l) {
                cnt1 += (cur->ls ? cur->ls->siz : 0) + 1;
                cur = cur->rs;
            } else {
                cur = cur->ls;
            }
        }
        Node* cur2 = root;
        while (cur2) {
            if (r >= cur2->val) {
                cnt2 += (cur2->ls ? cur2->ls->siz : 0) + 1;
                cur2 = cur2->rs;
            } else {
                cur2 = cur2->ls;
            }
        }
        return cnt2 - cnt1;
    }

    ESet() : root(nullptr), num_elements(0) {}
    ~ESet() {
        clear(root);
        num_elements = 0;
    }

    ESet& operator=(const ESet& other) {
        if (this != &other) {
            clear(root);
            root = other.root;
            num_elements = other.num_elements;
            if (root) ++root->ref_count;
        }
        return *this;
    }
};

template <class key>
MemoryPool<typename ESet<key>::Node> ESet<key>::nodePool;
int main() {
    struct IntHash {
        size_t operator()(int key) const { return static_cast<size_t>(key); }
    };
    std::unordered_map<int, ESet<long long>, IntHash> s;
    double treap_time0 = 0, treap_time1 = 0, treap_time2 = 0, treap_time3 = 0,
           treap_time4 = 0;

    clock_t start = clock();
    int cnt0 = 3000000;
    for (int i = 0; i < cnt0; i++) {
        long long a = rand() % 3000000;
        s[0].emplace(a);
    }
    clock_t end = clock();
    treap_time0 = static_cast<double>(end - start) / CLOCKS_PER_SEC;

    start = clock();
    for (int i = 0; i < 500000; i++) {
        long long a = rand() % (3 * 500000);
        s[0].erase(a);
    }
    end = clock();
    treap_time1 = static_cast<double>(end - start) / CLOCKS_PER_SEC;

    start = clock();
    int cnt2 = 1000000;
    for (int i = 0; i < cnt2; i++) {
        long long a = rand() % 3000000;
        s[0].find(a);
    }
    end = clock();
    treap_time2 = static_cast<double>(end - start) / CLOCKS_PER_SEC;

    start = clock();
    for (int i = 0; i < 30; i++) {
        ESet<long long> copy_eset = s[0];
    }
    end = clock();
    treap_time3 = static_cast<double>(end - start) / CLOCKS_PER_SEC;

    start = clock();
    for (int j = 0; j < 15; ++j) {
        for (int i = 0; i < cnt2; i++) {
            long long a = rand() % 3000000;
            s[0].nxt(a);
        }
    }
    end = clock();
    treap_time4 = static_cast<double>(end - start) / CLOCKS_PER_SEC;

    std::cout << "Time for emplace: " << treap_time0 << " seconds\n";
    std::cout << "Time for erase: " << treap_time1 << " seconds\n";
    std::cout << "Time for find: " << treap_time2 << " seconds\n";
    std::cout << "Time for copy: " << treap_time3 << " seconds\n";
    std::cout << "Time for traversal: " << treap_time4 << " seconds\n";
}
