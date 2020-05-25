// Реализовать Sparse Table для 2-ой порядковой статистики
// В первой строке заданы 2 числа: размер последовательности N и количество диапазонов M.
// Следующие N целых чисел задают последовательность.Далее вводятся M пар чисел - границ диапазонов.
// Гарантируется, что каждый диапазон содержит как минимум 2 элемента.

#include <iostream>
#include <algorithm>
#include <vector>

class SparseTable {
private:
    std::vector<std::vector<std::pair<int, int>>> buffer; // (число, индекс в посл-ти)
    std::vector<int> logVector; // в i-ом эл-те лежит log2(i + 1)
    void preCount(); // предподсчет logVector-а и twoInPowsVector-а
public:
    SparseTable(const std::vector<int>& vec) {
        logVector.resize(vec.size());
        preCount();
        buffer.resize(logVector[vec.size() - 1] + 1);
        for (int i = 0; i < vec.size(); ++i) {
            buffer[0].push_back(std::make_pair(vec[i], i));
        }
        int powOfTwo = 1;
        for (int i = 1; i < logVector[vec.size() - 1] + 1; ++i) {
            for (int j = 0; j < buffer[i - 1].size() - powOfTwo; ++j)
                buffer[i].push_back(std::min(buffer[i - 1][j], buffer[i - 1][j + powOfTwo]));
            powOfTwo *= 2;
        }
    }
    std::pair<int, int> findMin(int left, int right);
};

void SparseTable::preCount() {
    int twoInPow = 1;
    int pow = 0;
    for (int i = 0; i < logVector.size(); ++i) {
        if (i + 1 >= twoInPow * 2) {
            twoInPow *= 2;
            pow++;
        }
        logVector[i] = pow;
    }
}

std::pair<int, int> SparseTable::findMin(int left, int right) { // left и right включительно
    int level = logVector[right - left];
    return std::min(buffer[level][left], buffer[level][right - (1 << level) + 1]);
}

int findSecondMin(const std::pair<int, int>& middle, int left, int right, SparseTable& table) {
    if (middle.second == left) {
        return (table.findMin(middle.second + 1, right)).first;
    }
    if (middle.second == right) {
        return (table.findMin(left, middle.second - 1)).first;
    }
    return std::min((table.findMin(left, middle.second - 1)).first, (table.findMin(middle.second + 1, right)).first);
}

std::vector<int> solution(const std::vector<int>& sequence, int M) {
    SparseTable table(sequence);
    std::vector<int> ans;
    for (int i = 0; i < M; ++i) {
        int left;
        int right;
        std::cin >> left >> right;
        auto middle = table.findMin(left - 1, right - 1);
        ans.push_back(findSecondMin(middle, left - 1, right - 1, table));
    }
    return ans;
}

int main()
{
    int N;
    int M;
    std::cin >> N >> M;
    std::vector<int> sequence;
    for (int i = 0; i < N; ++i) {
        int elem;
        std::cin >> elem;
        sequence.push_back(elem);
    }
    std::vector<int> ans = solution(sequence, M);
    for (auto elem : ans) {
        std::cout << elem << std::endl;
    }
}
