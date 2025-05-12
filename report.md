# Report for ESet's performance tuning
* by starrywiki for Johnclass project4
## 数据结构采用：AVL & RBTree & std::set & 无旋treap
### Experiment process

#### 使用cmp.cpp对AVL & RBTree & std::set进行性能测试
#### 使用fhqtreap_test_task2对无旋treap进行性能测试

#### 测试数目如下
* insert：3'000'000
* erase: 500'000
* find: 1'000'000
* = operation: 30
* iterator related operation: 30

测试结果
单位：秒
|            | std ::set | RBTree | AVL   | 无旋Treap |
| ---------- | --------- | ------ | ----- | --------- |
| insert     | 0.958     | 0.855  | 1.288 | 5.912     |
| erase      | 1.156     | 0.882  | 1.234 | 1.297     |
| find       | 1.059     | 0.925  | 0.781 | 0.939     |
| = opration | 0.462     | 0.673  | 0.563 | 0.124     |
| iterator   | 0.201     | 0.207  | 0.158 | 0.366     |

### 调研结果
由于数据具有随机性，测试结果具有一定误差，通过多次测量取平均仍可以从中得到普遍规律。

1. **std::set**
* 优势：整体表现优异，插入删除、赋值运算以及迭代器遍历上等都不错
2. **RBTree**
* 优势：手写红黑树实现在插入删除以及find操作上表现优异，快于std::set
* 可能原因：手写红黑树可能在平衡性上做得更好，减少内存分配、避免不必要的操作等。std::set 可能包含额外的功能（如异常处理、调试信息等），这些功能会增加运行时的开销。
* 劣势：在"="operation以及iterator相关操作上慢于std::set
* 可能原因: 手写的红黑树在复制操作中使用了深拷贝（即递归复制整棵树），而 std::set 可能使用了更高效的实现方式（如引用计数或延迟拷贝）。
深拷贝的时间复杂度为 O(n)，且在实际操作中可能会频繁分配和释放内存，导致性能下降。std::set 的迭代器经过了高度优化，能够快速地进行遍历和操作。
3. **AVL**
* 优势：在find、"= operation"表现优异，尤其是find操作。
* 可能原因：平衡性较红黑树更好，find二分查找更高效，拷贝时栈的递归深度更小
* 劣势：在 insert 和 erase 操作上略慢于手写红黑树，比std稍逊色
* 可能原因：为了保持更好的平衡性，AVL 树在插入和删除时更频繁地调整树的结构（进行更多的旋转操作）所以插入删除稍慢。
4. **FHQtreap**
* 优势："= operation"表现优异，明显快于红黑树和avl树
* 可能原因：我的无旋treap实现了可持久化，将copy操作平摊到每次erase和emplace能够达到接近O（1）的复杂度
* 劣势：整体表现不如另外红黑树以及avl树，尤其是插入删除操作耗时较长
* 可能原因：随机优先级的维护增加了额外的开销。相较于红黑树和AVL树，无旋Treap的平衡性稍弱，可能导致树的高度较大，影响性能。其次，由于实现了可持久化，copy操作平摊到每次erase和emplace导致插入删除时间变长。
## Conclusion
整体表现而言，红黑树优于std::set优于avl树优于无旋treap
单方面而言，红黑树更擅长频繁插入删除的场景avl平衡性更好find操作更占优，可持久化的无旋treap在copy操作上更优。

