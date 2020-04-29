// Требуется найти в связном графе остовное дерево минимального веса.
// Воспользуйтесь алгоритмом Крускала.

#include <iostream>
#include <tuple>
#include <vector>
#include <set>

int get(int vertex, std::vector<int>& parent) { // получить корень
	if (parent[vertex] == vertex) {
		return vertex;
	}
	int x = get(parent[vertex], parent);
	parent[vertex] = x;
	return x;
}

void unite(int leftVertex, int rightVertex, std::vector<int>& parent, std::vector<int>& size) {
	int left = get(leftVertex, parent);
	int right = get(rightVertex, parent);
	if (size[left] < size[right]) {
		parent[left] = right;
		size[right] += size[left];
	}
	else {
		parent[right] = left;
		size[left] += size[right];
	}
}

int Kruskal(int N, int M) {
	std::set<std::pair<int, std::pair<int, int>>> priority; // левый аргумент - вес ребра, остальные 2 - его концы
	for (int i = 0; i < M; ++i) {
		int a;
		int b;
		int weight;
		std::cin >> a >> b >> weight;
		priority.insert(std::make_pair(weight, std::make_pair(a - 1, b - 1)));
	}
	std::vector<int> parent(N);
	for (int i = 0; i < N; ++i) {
		parent[i] = i;
	}
	std::vector<int> size(N, 1);
	int sum = 0;
	while (!priority.empty()) {
		auto current = *priority.begin();
		priority.erase(current);
		if (get(current.second.first, parent) != get(current.second.second, parent)) {
			unite(current.second.first, current.second.second, parent, size);
			sum += current.first;
		}
	}
	return sum;
}

int main() { 
	int N;
	int M;
	std::cin >> N >> M;
	std::cout << Kruskal(N, M) << std::endl;
	return 0;
}