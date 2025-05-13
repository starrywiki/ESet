// original passed version for eset with range not fixed
#ifndef FASTESTESET_HPP
#define FASTESTESET_HPP

#include <stdexcept>
#include <utility>
namespace RBTree {
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
    }

    // 右旋
    void RR(Node* pnode) {
        Node* subR = pnode->right;
        Node* subRL = subR->left;
        Node* ppnode = pnode->parent;

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
        // void advance() {
        //     do {
        //         if (current->right) {
        //             current = curset->find_min(current->right);
        //         } else {
        //             while (current->parent &&
        //                    current == current->parent->right) {
        //                 current = current->parent;
        //             }
        //             current = current->parent;
        //         }
        //     } while (current && current->isdele);
        // }
        // void retreat() {
        //     do {
        //         if (current == nullptr) {
        //             current = curset->find_max(curset->root);
        //             return;
        //         }
        //         Node* min_node = curset->find_min(curset->root);
        //         if (!curset->comp(min_node->key, current->key) &&
        //             !curset->comp(current->key, min_node->key))
        //             return;
        //         if (current->left) {
        //             current = curset->find_max(current->left);
        //         } else {
        //             while (current->parent &&
        //                    current == current->parent->left) {
        //                 current = current->parent;
        //             }
        //             current = current->parent;
        //         }
        //     } while (current && current->isdele);
        // }

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

        new_node->parent = pa;
        if (comp(key, pa->key)) {
            pa->left = new_node;
        } else {
            pa->right = new_node;
        }

        fix_insert(new_node);
        num_elements++;
        return {iterator(new_node, this), true};
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
    void swapNodes(Node*& a, Node*& b) {
        if (a == b) return;

        Node* bParent = b->parent;
        Node* bLeft = b->left;
        Node* bRight = b->right;
        bool isBLeftChild = (bParent && bParent->left == b);

        if (a->parent == nullptr) {
            root = b;
        } else if (a->parent->left == a) {
            a->parent->left = b;
        } else {
            a->parent->right = b;
        }
        b->parent = a->parent;
        b->left = a->left;
        b->right = a->right;
        if (a->left) a->left->parent = b;
        if (a->right) a->right->parent = b;
        b->is_red = a->is_red;

        // 将 a 替换为 b
        a->left = bLeft;
        a->right = bRight;
        if (bLeft) bLeft->parent = a;
        if (bRight) bRight->parent = a;
        if (isBLeftChild) {
            bParent->left = a;
        } else {
            bParent->right = a;
        }
        a->parent = bParent;
    }

    size_t erase(const Key& key) {
        Node* node = find_node(key);
        if (!node) return 0;

        Node* y = node;     // y 是要删除的节点或其后继节点
        Node* x = nullptr;  // x 是 y 的子节点
        bool y_original_color = y->is_red;

        if (!node->left) {
            x = node->right;
            transplant(node, node->right);
        } else if (!node->right) {
            x = node->left;
            transplant(node, node->left);
        } else {
            y = find_min(node->right);
            y_original_color = y->is_red;
            x = y->right;

            if (y->parent != node) {
                transplant(y, y->right);
                y->right = node->right;
                y->right->parent = y;
            }

            transplant(node, y);
            y->left = node->left;
            y->left->parent = y;
            y->is_red = node->is_red;
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
        size_t count = 0;
        iterator it = lower_bound(l);
        iterator end_it = upper_bound(r);
        while (it != end_it) {
            count++;
            ++it;
        }
        return count;
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
}  // namespace RBTree
#endif