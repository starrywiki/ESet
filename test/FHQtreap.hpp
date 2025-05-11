#ifndef FHQtreap_hpp
#define FHQtreap_hpp
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <utility>

namespace FHQ_treap {
template <class Key, class Compare = std::less<Key>>
class ESet {
   private:
    struct TreeNode {
        Key key;
        int priority;  // Treap 的堆性质
        size_t size;   // 子树大小
        TreeNode* left;
        TreeNode* right;

        TreeNode(const Key& k)
            : key(k),
              priority(rand()),
              size(1),
              left(nullptr),
              right(nullptr) {}
    };

    TreeNode* root;
    Compare comp;

    // 更新子树大小
    void update_size(TreeNode* node) {
        if (node) {
            node->size = 1;
            if (node->left) node->size += node->left->size;
            if (node->right) node->size += node->right->size;
        }
    }

    // 分裂操作
    void split(TreeNode* node, const Key& key, TreeNode*& left,
               TreeNode*& right) {
        if (!node) {
            left = right = nullptr;
            return;
        }
        if (comp(node->key, key)) {
            left = node;
            split(node->right, key, node->right, right);
        } else {
            right = node;
            split(node->left, key, left, node->left);
        }
        update_size(node);
    }

    // 合并操作
    TreeNode* merge(TreeNode* left, TreeNode* right) {
        if (!left) return right;
        if (!right) return left;
        if (left->priority > right->priority) {
            left->right = merge(left->right, right);
            update_size(left);
            return left;
        } else {
            right->left = merge(left, right->left);
            update_size(right);
            return right;
        }
    }

   public:
    // 构造函数
    ESet() : root(nullptr) {}

    // 析构函数
    ~ESet() {
        if (root) clear(root);
    }

    ESet& operator=(const ESet& other) {
        if (this != &other) {
            clear(root);
            if (other.root) {
                root = copy_tree(other.root);
            } else {
                root = nullptr;
            }
        }
        return *this;
    }
    TreeNode* copy_tree(TreeNode* node) {
        if (!node) return nullptr;
        TreeNode* new_node = new TreeNode(node->key);
        new_node->priority = node->priority;
        new_node->size = node->size;
        new_node->left = copy_tree(node->left);
        new_node->right = copy_tree(node->right);
        return new_node;
    }

    void clear(TreeNode* node) {
        if (node) {
            clear(node->left);
            clear(node->right);
            if (node) delete node;
        }
    }

    class iterator {
       private:
        TreeNode* current;
        const ESet* curset;

       public:
        iterator(TreeNode* node = nullptr, const ESet* s = nullptr)
            : current(node), curset(s) {}
        const Key& operator*() const {
            if (!current) throw std::out_of_range("Iterator is out of range");
            return current->key;
        }
        iterator& operator++() {
            if (current)
                current = curset->find_successor(
                    current);  // 调用 ESet 的 find_successor
            return *this;
        }
        iterator operator++(int) {
            iterator temp = *this;
            ++(*this);
            return temp;
        }
        bool operator==(const iterator& other) const {
            return current == other.current;
        }
        bool operator!=(const iterator& other) const {
            return current != other.current;
        }
    };

    template <class... Args>
    std::pair<iterator, bool> emplace(Args&&... args) {
        Key key(std::forward<Args>(args)...);
        TreeNode* left = nullptr;
        TreeNode* right = nullptr;
        split(root, key, left, right);

        TreeNode* existing = find_node(right, key);
        if (existing) {
            root = merge(left, right);
            return {iterator(existing), false};
        }

        TreeNode* new_node = new TreeNode(key);
        root = merge(merge(left, new_node), right);
        return {iterator(new_node), true};
    }

    iterator find(const Key& key) const {
        TreeNode* node = find_node(root, key);
        return iterator(node);
    }

    size_t erase(const Key& key) {
        TreeNode* left = nullptr;
        TreeNode* mid = nullptr;
        TreeNode* right = nullptr;
        Key successor_key = key;
        TreeNode* successor_node = find_successor(find_node(root, key));
        if (successor_node) {
            successor_key = successor_node->key;
        }
        split(root, key, left, right);
        split(right, successor_key, mid, right);
        if (mid) {
            delete mid;
            root = merge(left, right);
            return 1;
        } else {
            root = merge(left, right);
            return 0;
        }
    }

    size_t size() const { return root ? root->size : 0; }

    iterator begin() const {
        TreeNode* node = root;
        while (node && node->left) node = node->left;
        return iterator(node, this);
    }

    iterator end() const { return iterator(nullptr, this); }

   private:
    TreeNode* find_node(TreeNode* node, const Key& key) const {
        while (node) {
            if (comp(key, node->key)) {
                node = node->left;
            } else if (comp(node->key, key)) {
                node = node->right;
            } else {
                return node;
            }
        }
        return nullptr;
    }

    // 查找后继节点
    TreeNode* find_successor(TreeNode* node) const {
        if (!node) return nullptr;
        if (node->right) {
            node = node->right;
            while (node->left) node = node->left;
            return node;
        }
        TreeNode* successor = nullptr;
        TreeNode* current = root;
        while (current) {
            if (comp(node->key, current->key)) {
                successor = current;
                current = current->left;
            } else if (comp(current->key, node->key)) {
                current = current->right;
            } else {
                break;
            }
        }
        return successor;
    }
};
}  // namespace FHQ_treap
#endif  // FHQtreap_hpp