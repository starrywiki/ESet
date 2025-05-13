#include <bits/stdc++.h>

#include "src.hpp"  

template <class Key, class Compare>
bool isRedBlackTreeValid(typename ESet<Key, Compare>::Node* root) {
    // 性质 2: 根节点是黑色的
    if (root && root->is_red) {
        std::cout << "违反性质 2: 根节点是红色的！" << std::endl;
        return false;
    }

    // 性质 4: 如果一个节点是红色的，那么它的两个子节点都是黑色的
    auto checkRedProperty =
        [](typename ESet<Key, Compare>::Node* node) -> bool {
        if (!node) return true;
        if (node->is_red) {
            if ((node->left && node->left->is_red) ||
                (node->right && node->right->is_red)) {
                std::cout << "违反性质 4: 红色节点的子节点也是红色的！"
                          << std::endl;
                return false;
            }
        }
        return true;
    };

    // 性质 5: 从任一节点到其每个叶子的所有路径都包含相同数目的黑色节点
    std::function<bool(typename ESet<Key, Compare>::Node*, int, int&)>
        checkBlackHeight;
    checkBlackHeight = [&](typename ESet<Key, Compare>::Node* node,
                           int blackCount, int& targetCount) -> bool {
        if (!node) {
            if (targetCount == -1) {
                targetCount = blackCount;  // 第一次到达叶子节点，设置目标值
            } else if (blackCount != targetCount) {
                std::cout << "违反性质 5: 黑色节点数量不一致！" << std::endl;
                return false;
            }
            return true;
        }
        if (!node->is_red) blackCount++;
        return checkBlackHeight(node->left, blackCount, targetCount) &&
               checkBlackHeight(node->right, blackCount, targetCount);
    };

    // 检查性质 4
    std::function<bool(typename ESet<Key, Compare>::Node*)>
        checkRedPropertyRecursive;
    checkRedPropertyRecursive =
        [&](typename ESet<Key, Compare>::Node* node) -> bool {
        if (!node) return true;
        if (!checkRedProperty(node)) return false;
        return checkRedPropertyRecursive(node->left) &&
               checkRedPropertyRecursive(node->right);
    };

    // 检查性质 5
    int targetCount = -1;
    if (!checkRedPropertyRecursive(root) ||
        !checkBlackHeight(root, 0, targetCount)) {
        return false;
    }

    return true;
}

// 测试函数
void testEmplace() {
    ESet<int> rbTree;  // 创建一个 ESet 对象

    // 插入大量元素
    const int N = 100000;  // 插入 10 万个元素
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(1, N * 10);

    for (int i = 0; i < N; ++i) {
        int key = dis(gen);
        rbTree.emplace(key);
    }

    // 检查红黑树性质
    if (isRedBlackTreeValid<int, std::less<int>>(rbTree.getRoot())) {
        std::cout << "插入 " << N << " 个元素后，红黑树性质仍然满足！"
                  << std::endl;
    } else {
        std::cout << "插入 " << N << " 个元素后，红黑树性质被破坏！"
                  << std::endl;
    }
}

int main() {
    testEmplace();
    ESet<int> set;
    set.emplace(10);
    set.emplace(20);
    set.emplace(30);
    set.emplace(40);
    set.emplace(50);

    std::cout << "Range [20, 40]: " << set.range(20, 40) << std::endl; // 应输出 3
    std::cout << "Range [10, 50]: " << set.range(10, 50) << std::endl; // 应输出 5
    std::cout << "Range [15, 45]: " << set.range(15, 45) << std::endl; // 应输出 3
    std::cout << "Range [60, 70]: " << set.range(60, 70) << std::endl; // 应输出 0

    ESet<int> s2;
    auto it = s2.emplace(1).first;
    for (int i = 0; i <= 2; i++) {
        s2.emplace(i);
    }
    while (it != s2.end()) s2.erase(*(it++));
    return 0;
}