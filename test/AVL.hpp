#include <iostream>
#include <stdexcept>
#include <utility>
// #include <functional>
#include <stack>
namespace AVL_SET {
template <class Key, class Compare = std::less<Key>>
class ESet {
   private:
    struct Node {
        Key key;
        Node* left;
        Node* right;
        Node* parent;
        int height;

        explicit Node(const Key& k)
            : key(k),
              left(nullptr),
              right(nullptr),
              parent(nullptr),
              height(1) {}
    };

    Node* root;
    size_t count;
    Compare comp;

    int height(Node* node) const { return node ? node->height : 0; }

    int balanceFactor(Node* node) const {
        return node ? height(node->left) - height(node->right) : 0;
    }

    void updateHeight(Node* node) {
        if (node) {
            node->height =
                std::max(height(node->left), height(node->right)) + 1;
        }
    }

    void rotateLeft(Node*& node) {
        Node* newRoot = node->right;
        node->right = newRoot->left;
        if (node->right) node->right->parent = node;
        newRoot->left = node;
        newRoot->parent = node->parent;
        node->parent = newRoot;
        node = newRoot;
        updateHeight(node->left);
        updateHeight(node);
    }

    void rotateRight(Node*& node) {
        Node* newRoot = node->left;
        node->left = newRoot->right;
        if (node->left) node->left->parent = node;
        newRoot->right = node;
        newRoot->parent = node->parent;
        node->parent = newRoot;
        node = newRoot;
        updateHeight(node->right);
        updateHeight(node);
    }

    void balance(Node*& node) {
        if (!node) return;
        updateHeight(node);
        int bf = balanceFactor(node);
        if (bf > 1) {
            if (balanceFactor(node->left) < 0) rotateLeft(node->left);
            rotateRight(node);
        } else if (bf < -1) {
            if (balanceFactor(node->right) > 0) rotateRight(node->right);
            rotateLeft(node);
        }
    }

    void insert(Node*& node, Node* parent, const Key& key) {
        if (!node) {
            node = new Node(key);
            node->parent = parent;
            ++count;
            return;
        }
        if (comp(key, node->key)) {
            insert(node->left, node, key);
        } else if (comp(node->key, key)) {
            insert(node->right, node, key);
        }
        balance(node);
    }

    Node* find(Node* node, const Key& key) const {
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

    void erase(Node*& node, const Key& key) {
        if (!node) return;
        if (comp(key, node->key)) {
            erase(node->left, key);
        } else if (comp(node->key, key)) {
            erase(node->right, key);
        } else {
            if (!node->left || !node->right) {
                Node* temp = node->left ? node->left : node->right;
                if (temp) temp->parent = node->parent;
                delete node;
                node = temp;
                --count;
            } else {
                Node* minNode = node->right;
                while (minNode->left) minNode = minNode->left;
                node->key = minNode->key;
                erase(node->right, minNode->key);
            }
        }
        balance(node);
    }

    void copyTree(Node* src, Node*& dest, Node* parent) {
        if (!src) return;
        std::stack<std::pair<Node*, Node**>> stack;
        dest = new Node(src->key);
        dest->parent = parent;
        stack.push({src, &dest});
        while (!stack.empty()) {
            auto [srcNode, destNode] = stack.top();
            stack.pop();
            if (srcNode->left) {
                *destNode = new Node(srcNode->left->key);
                (*destNode)->parent = srcNode;
                stack.push({srcNode->left, &(*destNode)->left});
            }
            if (srcNode->right) {
                *destNode = new Node(srcNode->right->key);
                (*destNode)->parent = srcNode;
                stack.push({srcNode->right, &(*destNode)->right});
            }
        }
    }
    void clear(Node* node) {
        if (!node) return;
        clear(node->left);
        clear(node->right);
        delete node;
    }

   public:
    class iterator {
       private:
        Node* ptr;
        const ESet* set;

       public:
        explicit iterator(Node* p = nullptr, const ESet* s = nullptr)
            : ptr(p), set(s) {}

        const Key& operator*() const {
            if (!ptr) throw std::out_of_range("Dereferencing end iterator");
            return ptr->key;
        }

        iterator& operator++() {
            if (!ptr) return *this;
            if (ptr->right) {
                ptr = ptr->right;
                while (ptr->left) ptr = ptr->left;
            } else {
                while (ptr->parent && ptr == ptr->parent->right)
                    ptr = ptr->parent;
                ptr = ptr->parent;
            }
            return *this;
        }

        iterator operator++(int) {
            iterator temp = *this;
            ++(*this);
            return temp;
        }

        iterator& operator--() {
            if (!ptr) {
                ptr = set->root;
                while (ptr && ptr->right) ptr = ptr->right;
                return *this;
            }
            if (ptr->left) {
                ptr = ptr->left;
                while (ptr->right) ptr = ptr->right;
            } else {
                while (ptr->parent && ptr == ptr->parent->left)
                    ptr = ptr->parent;
                ptr = ptr->parent;
            }
            return *this;
        }

        iterator operator--(int) {
            iterator temp = *this;
            --(*this);
            return temp;
        }

        bool operator==(const iterator& other) const {
            return ptr == other.ptr;
        }
        bool operator!=(const iterator& other) const {
            return ptr != other.ptr;
        }
    };

    ESet() : root(nullptr), count(0) {}
    ~ESet() { clear(root); }

    template <class... Args>
    std::pair<iterator, bool> emplace(Args&&... args) {
        Key key(std::forward<Args>(args)...);
        auto it = find(key);
        if (it != end()) return {it, false};
        insert(root, nullptr, key);
        return {find(key), true};
    }

    size_t erase(const Key& key) {
        if (!find(root, key)) return 0;
        erase(root, key);
        return 1;
    }

    iterator find(const Key& key) const {
        return iterator(find(root, key), this);
    }

    ESet(const ESet& other) : count(other.count) {
        copyTree(other.root, root, nullptr);
    }

    void swap(ESet& lhs, ESet& rhs) noexcept {
        using std::swap;
        swap(lhs.root, rhs.root);
        swap(lhs.count, rhs.count);
    }
    ESet& operator=(const ESet& other) {
        if (this != &other) {
            ESet temp(other);   
            swap(*this, temp); 
        }
        return *this;
    }

    ESet(ESet&& other) noexcept : root(other.root), count(other.count) {
        other.root = nullptr;
        other.count = 0;
    }

    ESet& operator=(ESet&& other) noexcept {
        if (this != &other) {
            clear(root);
            root = other.root;
            count = other.count;
            other.root = nullptr;
            other.count = 0;
        }
        return *this;
    }

    size_t range(const Key& l, const Key& r) const {
        size_t result = 0;
        for (auto it = lower_bound(l); it != end() && !comp(r, *it); ++it)
            ++result;
        return result;
    }

    size_t size() const noexcept { return count; }

    iterator lower_bound(const Key& key) const {
        Node* node = root;
        Node* result = nullptr;
        while (node) {
            if (!comp(node->key, key)) {
                result = node;
                node = node->left;
            } else {
                node = node->right;
            }
        }
        return iterator(result, this);
    }

    iterator upper_bound(const Key& key) const {
        Node* node = root;
        Node* result = nullptr;
        while (node) {
            if (comp(key, node->key)) {
                result = node;
                node = node->left;
            } else {
                node = node->right;
            }
        }
        return iterator(result, this);
    }

    iterator begin() const {
        Node* node = root;
        while (node && node->left) node = node->left;
        return iterator(node, this);
    }

    iterator end() const { return iterator(nullptr, this); }
};
}  // namespace AVL_SET