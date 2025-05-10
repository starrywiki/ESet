#ifndef ESET_HPP
#define ESET_HPP

#include <iostream>
#include <stdexcept>
#include <utility>
template <class Key, class Compare = std::less<Key>>
class ESet {
   public:
    // 红黑树节点定义
    struct Node {
        Key key;
        bool is_red;  // 是否为红色节点
        Node* left;
        Node* right;
        Node* parent;
        int siz = 1;
        // bool isdele = false;
        Node(const Key& k, bool red = true)
            : key(k),
              is_red(red),
              left(nullptr),
              right(nullptr),
              parent(nullptr) {}
    };

    Node* root;           // 根节点
    Compare comp;         // 比较函数
    size_t num_elements;  // 元素个数

    // 外侧靠左
    void LL(Node* pnode) {
        Node* subL = pnode->left;
        Node* subLR = subL->right;
        Node* ppnode = pnode->parent;

        pnode->siz = (pnode->right ? pnode->right->siz : 0) +
                     (subLR ? subLR->siz : 0) + 1;
        subL->siz = (subL->left ? subL->left->siz : 0) + pnode->siz + 1;

        pnode->left = subLR;
        if (subLR) subLR->parent = pnode;

        subL->right = pnode;
        pnode->parent = subL;

        if (root == pnode) {
            root = subL;
            subL->parent = nullptr;
        } else {
            if (ppnode->left == pnode)
                ppnode->left = subL;
            else
                ppnode->right = subL;
            subL->parent = ppnode;
        }
        // print_siz(root);
    }

    // 右旋
    void RR(Node* pnode) {
        Node* subR = pnode->right;
        Node* subRL = subR->left;
        Node* ppnode = pnode->parent;

        pnode->siz =
            (pnode->left ? pnode->left->siz : 0) + (subRL ? subRL->siz : 0) + 1;
        subR->siz = (subR->right ? subR->right->siz : 0) + pnode->siz + 1;

        pnode->right = subRL;
        if (subRL) subRL->parent = pnode;

        subR->left = pnode;
        pnode->parent = subR;

        if (root == pnode) {
            root = subR;
            subR->parent = nullptr;
        } else {
            if (ppnode->left == pnode)
                ppnode->left = subR;
            else
                ppnode->right = subR;
            subR->parent = ppnode;
        }
        // print_siz(root);
    }
    void LR(Node* pnode) {
        RR(pnode->left);
        LL(pnode);
    }
    void RL(Node* pnode) {
        LL(pnode->right);
        RR(pnode);
    }
    // 插入修复
    void fix_insert(Node* cur) {
        while (cur != root && cur->parent->is_red) {
            Node* pnode = cur->parent;
            Node* gpnode = pnode->parent;

            if (gpnode->left == pnode) {
                Node* uncle = gpnode->right;
                if (uncle && uncle->is_red) {
                    pnode->is_red = false;
                    uncle->is_red = false;
                    gpnode->is_red = true;
                    cur = gpnode;
                } else {
                    if (pnode->right == cur) {
                        RR(pnode);
                        cur = pnode;
                        pnode = cur->parent;
                    }
                    pnode->is_red = false;
                    gpnode->is_red = true;
                    LL(gpnode);
                }
            } else {
                Node* uncle = gpnode->left;
                if (uncle && uncle->is_red) {
                    pnode->is_red = false;
                    uncle->is_red = false;
                    gpnode->is_red = true;
                    cur = gpnode;
                } else {
                    if (pnode->left == cur) {
                        LL(pnode);
                        cur = pnode;
                        pnode = cur->parent;
                    }
                    pnode->is_red = false;
                    gpnode->is_red = true;
                    RR(gpnode);
                }
            }
        }
        root->is_red = false;
    }
    void fix_erase(Node* node) {
        if (node == nullptr) return;
        while (node != root && !node->is_red) {
            Node* parent = node->parent;
            Node* sibling =
                (node == parent->left) ? parent->right : parent->left;

            if (sibling->is_red) {
                // Case 1: 兄弟节点为红色
                sibling->is_red = false;
                parent->is_red = true;
                if (node == parent->left)
                    RR(parent);
                else
                    LL(parent);
                sibling = (node == parent->left) ? parent->right : parent->left;
            }

            if ((!sibling->left || !sibling->left->is_red) &&
                (!sibling->right || !sibling->right->is_red)) {
                // Case 2: 兄弟节点为黑色且兄弟节点的子节点为黑色
                sibling->is_red = true;
                node = parent;
            } else {
                if (node == parent->left &&
                    (!sibling->right || !sibling->right->is_red)) {
                    // Case 3:
                    // 兄弟节点为黑色，兄弟节点的左子节点为红色，右子节点为黑色
                    sibling->left->is_red = false;
                    sibling->is_red = true;
                    LL(sibling);
                    sibling = parent->right;
                } else if (node == parent->right &&
                           (!sibling->left || !sibling->left->is_red)) {
                    // Case 3:
                    // 兄弟节点为黑色，兄弟节点的右子节点为红色，左子节点为黑色
                    sibling->right->is_red = false;
                    sibling->is_red = true;
                    RR(sibling);
                    sibling = parent->left;
                }

                // Case 4: 兄弟节点为黑色，兄弟节点的右子节点为红色
                sibling->is_red = parent->is_red;
                parent->is_red = false;
                if (node == parent->left) {
                    sibling->right->is_red = false;
                    RR(parent);
                } else {
                    sibling->left->is_red = false;
                    LL(parent);
                }
                node = root;
            }
        }
        node->is_red = false;
    }

    // 查找最小节点
    Node* find_min(Node* node) const {
        if (!node) return nullptr;
        while (node->left) node = node->left;
        return node;
    }

    // 查找最大节点
    Node* find_max(Node* node) const {
        if (!node) return nullptr;
        while (node->right) node = node->right;
        return node;
    }

    // 查找节点
    Node* find_node(const Key& key) const {
        Node* cur = root;
        while (cur) {
            if (comp(key, cur->key))
                cur = cur->left;
            else if (comp(cur->key, key))
                cur = cur->right;
            else
                return cur;
        }
        return nullptr;
    }

    // 清空树
    void clear_tree(Node* node) {
        if (node) {
            clear_tree(node->left);
            clear_tree(node->right);
            delete node;
        }
    }
    void clear() {
        clear_tree(root);
        root = nullptr;
        num_elements = 0;
    }
    // 迭代器定义
    Node* getRoot() const { return root; }
    class iterator {
       private:
        Node* current;
        const ESet* curset;

       public:
        iterator(Node* node = nullptr, const ESet* nowset = nullptr)
            : current(node), curset(nowset) {}
        ~iterator() = default;
        const Key& operator*() const {
            if (!current) throw std::out_of_range("Dereferencing end iterator");
            return current->key;
        }

        // ++iter
        iterator& operator++() {
            if (current) {
                if (current->right) {
                    current = curset->find_min(current->right);
                } else {
                    while (current->parent &&
                           current == current->parent->right) {
                        current = current->parent;
                    }
                    current = current->parent;
                }
            }
            return *this;
        }

        // iter++
        iterator operator++(int) {
            iterator temp = *this;
            ++(*this);
            return temp;
        }

        // --iter
        iterator& operator--() {
            if (current == nullptr) {
                current = curset->find_max(curset->root);
                return *this;
            }
            Node* min_node = curset->find_min(curset->root);
            if (!curset->comp(min_node->key, current->key) &&
                !curset->comp(current->key, min_node->key))
                return *this;
            if (current) {
                if (current->left) {
                    current = curset->find_max(current->left);
                } else {
                    while (current->parent &&
                           current == current->parent->left) {
                        current = current->parent;
                    }
                    current = current->parent;
                }
            }
            return *this;
        }

        // iter--
        iterator operator--(int) {
            iterator temp = *this;
            --(*this);
            return temp;
        }

        bool operator==(const iterator& other) const {
            return current == other.current;
        }

        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }
    };

    ESet() : root(nullptr), num_elements(0) {}
    ~ESet() { clear(); }

    template <class... Args>
    std::pair<iterator, bool> emplace(Args&&... args) {
        Key key(std::forward<Args>(args)...);
        Node* new_node = new Node(key);

        if (!root) {
            root = new_node;
            root->is_red = false;
            num_elements++;
            return {iterator(root, this), true};
        }

        Node* pa = root;
        Node* current = root;
        while (current) {
            pa = current;
            if (comp(key, current->key)) {
                current = current->left;
            } else if (comp(current->key, key)) {
                current = current->right;
            } else {
                delete new_node;
                return {iterator(current, this), false};
            }
        }
        Node* tmp = pa;
        while (tmp) {
            tmp->siz++;
            tmp = tmp->parent;
        }
        new_node->parent = pa;
        if (comp(key, pa->key)) {
            pa->left = new_node;
        } else {
            pa->right = new_node;
        }

        fix_insert(new_node);
        num_elements++;
        return {iterator(new_node, this), true};
        // print_siz(root);
    }
    void transplant(Node* u, Node* v) {
        if (u->parent == nullptr) {
            root = v;
        } else if (u == u->parent->left) {
            u->parent->left = v;
        } else {
            u->parent->right = v;
        }
        if (v != nullptr) {
            v->parent = u->parent;
        }
    }

    size_t erase(const Key& key) {
        Node* node = find_node(key);
        if (!node) return 0;

        // print_siz(root);
        Node* y = node;     // y 是要删除的节点或其后继节点
        Node* x = nullptr;  // x 是 y 的子节点
        bool y_original_color = y->is_red;

        if (!node->left) {
            // std::cout<<"enter11?"<<std::endl;
            Node* cur = node->parent;
            while (cur) {
                cur->siz--;
                cur = cur->parent;
            }
            x = node->right;
            transplant(node, node->right);
        } else if (!node->right) {
            // std::cout<<"enter22?"<<std::endl;
            Node* cur = node->parent;
            while (cur) {
                cur->siz--;
                cur = cur->parent;
            }
            x = node->left;
            transplant(node, node->left);
        } else {
            // std::cout<<"enter?"<<std::endl;
            y = find_min(node->right);
            y_original_color = y->is_red;
            x = y->right;
            Node* pa = y;
            while (pa) {
                pa->siz--;
                pa = pa->parent;
            }
            if (y->parent != node) {
                transplant(y, y->right);
                y->right = node->right;
                y->right->parent = y;
            }
            transplant(node, y);
            y->left = node->left;
            y->left->parent = y;
            y->is_red = node->is_red;
            y->siz = (y->left ? y->left->siz : 0) +
                     (y->right ? y->right->siz : 0) + 1;
        }

        delete node;
        num_elements--;

        if (!y_original_color) {
            fix_erase(x);
        }

        return 1;
    }

    iterator find(const Key& key) const {
        Node* node = find_node(key);
        return iterator(node, this);
    }

    // 复制构造函数和赋值运算符
    ESet(const ESet& other) : root(nullptr), num_elements(0) {
        for (const auto& key : other) {
            emplace(key);
        }
    }

    ESet& operator=(const ESet& other) {
        if (this != &other) {
            clear();
            for (const auto& key : other) {
                emplace(key);
            }
        }
        return *this;
    }

    // 移动构造函数和赋值运算符
    ESet(ESet&& other) noexcept
        : root(other.root), num_elements(other.num_elements) {
        other.root = nullptr;
        other.num_elements = 0;
    }

    ESet& operator=(ESet&& other) noexcept {
        if (this != &other) {
            clear();
            root = other.root;
            num_elements = other.num_elements;
            other.root = nullptr;
            other.num_elements = 0;
        }
        return *this;
    }

    // 范围查询
    size_t range(const Key& l, const Key& r) const {
        if (comp(r, l)) return 0;
        Node* cur = root;
        size_t cnt1 = 0;
        size_t cnt2 = 0;
        while (cur) {
            if (comp(cur->key, l)) {
                cnt1 += (cur->left ? cur->left->siz : 0) + 1;
                cur = cur->right;
            } else {
                cur = cur->left;
            }
        }
        Node* cur2 = root;
        while (cur2) {
            if (!comp(r, cur2->key)) {
                cnt2 += (cur2->left ? cur2->left->siz : 0) + 1;
                cur2 = cur2->right;
            } else {
                cur2 = cur2->left;
            }
        }
        return cnt2 - cnt1;
    }

    // 获取元素个数
    size_t size() const noexcept { return num_elements; }

    // 迭代器相关
    iterator begin() const { return iterator(find_min(root), this); }
    iterator end() const { return iterator(nullptr, this); }

    // 下界和上界查询
    iterator lower_bound(const Key& key) const {
        Node* cur = root;
        Node* result = nullptr;
        while (cur) {
            if (!comp(cur->key, key)) {
                result = cur;
                cur = cur->left;
            } else {
                cur = cur->right;
            }
        }
        return iterator(result, this);
    }

    iterator upper_bound(const Key& key) const {
        iterator it = lower_bound(key);
        if (it != end() && *it == key) {
            ++it;
        }
        return it;
    }
};

#endif  // ESET_HPP