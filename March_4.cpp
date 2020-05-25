// Рик отправляет Морти в путешествие по N вселенным. У него есть список всех существующих однонаправленных телепортов. 
// Чтобы Морти не потерялся, Рику необходимо узнать, между какими вселенными существуют пути, а между какими нет. Помогите ему в этом!

#include <bitset>
#include <iostream>
#include <vector>
#include <string>

void solution(std::vector<std::vector<std::bitset<32>>>& vec) {
    for (int k = 0; k < vec.size(); ++k) {
        for (int i = 0; i < vec.size(); ++i) {
            if (vec[i][k / 32][k % 32]) {
                for (int j = 0; j < vec.size() / 32 + 1; ++j) {
                    vec[i][j] = vec[i][j] | vec[k][j];
                }
            }     
        }
    }
}

int main()
{
    int n;
    std::cin >> n;
    std::vector<std::vector<std::bitset<32>>> vec(n);
    for (int i = 0; i < n; ++i) {
        std::string line;
        std::cin >> line;
        for (int j = 0; j < n / 32 + 1; ++j) {
            std::bitset<32> newBitset;
            vec[i].push_back(newBitset);
            for (int k = 0; k < 32 && j * 32 + k < n; ++k) {
                if (line[j * 32 + k] == '1') {
                    vec[i][j][k] = true;
                }
                else {
                    vec[i][j][k] = false;
                }
            }
        }
    }
    solution(vec);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n / 32 + 1; ++j) {
            for (int k = 0; k < 32 && j * 32 + k < n; ++k) {
                std::cout << vec[i][j][k];
            }
        }
        std::cout << std::endl;
    }
    return 0;
}
