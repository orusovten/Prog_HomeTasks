// Мост представляет собой поле из n × m клеток, причем некоторые клетки ещё остались целыми. У Шрека есть только дощечки размера 1 × 2, 
// установка которых занимает у него a секунд, и 1 × 1, установка которых занимает b секунд. Ему необходимо закрыть все пустые клетки, 
// причем только их, не накладывая дощечки друг на друга.
// Определите, через какое минимальное количество секунд Шрек и Осёл смогут продолжить свой путь дальше.

#include <iostream>
#include <vector>

int count_whole(const std::vector<std::vector<int>>& field) { // подсчет кол-ва целых клеток
    int ans = 0;
    for (int i = 0; i < field.size(); ++i) {
        for (int j = 0; j < field[i].size(); ++j) {
            ans += field[i][j];
        }
    }
    return ans;
}

std::vector<std::pair<int, int>> get_neighs(int i, int j, int max_i, int max_j) {
    std::vector<std::pair<int, int>> ans;
    if (i > 0) {
        ans.push_back(std::make_pair(i - 1, j));
    }
    if (i < max_i) {
        ans.push_back(std::make_pair(i + 1, j));
    }
    if (j > 0) {
        ans.push_back(std::make_pair(i, j - 1));
    }
    if (j < max_j) {
        ans.push_back(std::make_pair(i, j + 1));
    }
    return ans;
}

bool Kuhn(int i, int j, const std::vector<std::vector<int>>& field, std::vector<std::vector<bool>>& used, 
    std::vector<std::vector<std::pair<int, int>>>& note) { // (i, j) - координаты в-ны
    if (used[i][j]) {
        return false;
    }
    used[i][j] = true;
    std::vector<std::pair<int, int>> neighs = get_neighs(i, j, field.size() - 1, field[0].size() - 1);
    for (auto current : neighs) {
        if (field[current.first][current.second] == 1) {
            if (note[current.first][current.second].first == -1 || 
                Kuhn(note[current.first][current.second].first, note[current.first][current.second].second, field, used, note)) {
                note[current.first][current.second] = std::make_pair(i, j);
                return true;
            }
        }
    }
    return false;
}

int solution(int a, int b, const std::vector<std::vector<int>>& field) {
    int whole = count_whole(field); // кол-во целых клеток
    if (b * 2 <= a) {
        return b * whole;
    }
    std::vector<std::vector<bool>> used(field.size());
    for (int i = 0; i < field.size(); ++i) {
        used[i] = std::vector<bool>(field[0].size(), false);
    }
    std::vector<std::vector<std::pair<int, int>>> note(field.size()); // насыщена ли в-на, (-1, -1) - нет, иначе - координаты соседа
    for (int i = 0; i < field.size(); ++i) {
        note[i] = std::vector<std::pair<int, int>>(field[0].size(), std::make_pair(-1, -1));
    }
    for (int i = 0; i < field.size(); ++i) {
        for (int j = 0; j < field[0].size(); ++j) {
            if (field[i][j] == 1 && (i + j) % 2 == 0) {
                if (Kuhn(i, j, field, used, note)) {
                    for (int k = 0; k < field.size(); ++k) {
                        used[k] = std::vector<bool>(field[0].size(), false);
                    }
                }
            }
        }
    }
    int domino = 0;
    for (int i = 0; i < field.size(); ++i) {
        for (int j = 0; j < field[0].size(); ++j) {
            if (note[i][j].first != -1) {
                domino += 1;
            }   
        }
    }
    return (whole - domino * 2) * b + domino * a;
}

int main()
{
    int n;
    int m;
    int a;
    int b;
    std::cin >> n >> m >> a >> b;
    std::vector<std::vector<int>> field(n);
    for (int i = 0; i < n; ++i) {
        std::string line;
        std::cin >> line;
        for (int j = 0; j < m; ++j) {
            if (line[j] == '*') {
                field[i].push_back(1);
            }
            else{
                field[i].push_back(0);
            }
        }
    }
    std::cout << solution(a, b, field) << std::endl;
}
