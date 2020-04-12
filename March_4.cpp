// Рик отправляет Морти в путешествие по N вселенным. У него есть список всех существующих однонаправленных телепортов. 
// Чтобы Морти не потерялся, Рику необходимо узнать, между какими вселенными существуют пути, а между какими нет. Помогите ему в этом!

#include <bitset>
#include <iostream>
#include <vector>

void solution(std::vector<std::bitset<1000>>& vec) {
    for (int k = vec.size() - 1; k >= 0; --k) {
        for (int i = vec.size() - 1; i >= 0; --i) {
            if ((vec[i][vec.size() - 1 - k])) {
                vec[i] = vec[i] | vec[k];
            }

        }
    }
}

int main()
{
    int n;
    std::cin >> n;
    std::vector<std::bitset<1000>> vec(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> vec[i];
    }
    solution(vec);
    for (int i = 0; i < n; ++i) {
        for (int j = n - 1; j >= 0; --j) {
            std::cout << vec[i][j];
        }
        std::cout << std::endl;
    }
    return 0;
}
