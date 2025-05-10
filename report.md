# Report 
## 数据结构采用：AVL & RBTree & std::set & ?
### experiment
使用generatenums.cpp生成test0.cpp实验数据，使用cmp.cpp进行性能测试
* 总共测试个数：50000
* insert：10106
* erase: 9983
* find: 9926
* = operation: 9999
* iterator related operation: 9926

测试结果
单位：μs
|            | std::set | RBTree  | AVL | 无旋Treap |
| ---------- | -------- | ------- | --- | --------- |
| insert     | 1.1418   | 0.6686  |     |           |
| erase      | 2.4790   | 0.5087  |     |           |
| find       | 0.6588   | 0.4401  |     |           |
| = opration | 189.028  | 677.038 |     |           |
| iterator   | 52.3289  | 72.8599 |     |           |

### 调研结果
1. RBTree 
* 优势：手写红黑树实现在插入删除以及find操作上表现优异，快于std::set
* 可能原因：手写红黑树可能在平衡性上做得更好，减少内存分配、避免不必要的操作等。std::set 可能包含额外的功能（如异常处理、调试信息等），这些功能会增加运行时的开销。
* 劣势：在"="operation以及iterator相关操作上慢于std::set
* 可能原因: 手写的红黑树在复制操作中使用了深拷贝（即递归复制整棵树），而 std::set 可能使用了更高效的实现方式（如引用计数或延迟拷贝）。
深拷贝的时间复杂度为 O(n)，且在实际操作中可能会频繁分配和释放内存，导致性能下降。std::set 的迭代器经过了高度优化，能够快速地进行遍历和操作。

2. AVL
