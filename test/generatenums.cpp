#include <cstdlib>  
#include <ctime>   
#include <fstream>
#include <iostream>
#define ll long long

// 生成一个 [min, max] 范围内的随机数
ll rand_range(ll min, ll max) { return min + rand() % (max - min + 1); }

int main() {
    srand(time(0));  
    const int total_operations = 50000;  
    const int max_value = 10000;         
    const int range_size = 100;          
    std::ofstream out("test0.in");       

    for (int i = 0; i < total_operations; ++i) {
        int op = rand() % 5;  
        ll b, c;
        switch (op) {
            case 0:  // 插入
                b = rand_range(0, max_value);
                out << op << " " << b << "\n";
                break;
            case 1:  // 删除
                b = rand_range(0, max_value);
                out << op << " " << b << "\n";
                break;
            case 2:  // 查找
                b = rand_range(0, max_value);
                out << op << " " << b << "\n";
                break;
            case 3:  // 复制
                out << op << "\n";  
                break;
            case 4:  // 遍历集合
                out << op << "\n";  
                break;
        }
    }

    out.close();  
    std::cout << "finished!" << std::endl;
    return 0;
}