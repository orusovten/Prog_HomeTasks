// Требуется найти в связном графе остовное дерево минимального веса.
// Воспользуйтесь алгоритмом Крускала.

#include <iostream>
#include <tuple>
#include <vector>
#include <set>

class DSU {
private:
	std::vector<int> parent;
	std::vector<int> size;
public:
	explicit DSU(int n) {
		parent = std::vector<int>(n);
		for (int i = 0; i < n; ++i) {
			parent[i] = i;
		}
		size = std::vector<int>(n, 1);
	}
	int get(int vertex);
	void unite(int leftVertex, int rightVertex);
};

int DSU::get(int vertex) { // получить корень
	if (parent[vertex] == vertex) {
		return vertex;
	}
	int x = get(parent[vertex]);
	parent[vertex] = x;
	return x;
}

void DSU::unite(int leftVertex, int rightVertex) {
	int left = get(leftVertex);
	int right = get(rightVertex);
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
	DSU dsu(N);
	int sum = 0;
	while (!priority.empty()) {
		auto current = *priority.begin();
		priority.erase(current);
		if (dsu.get(current.second.first) != dsu.get(current.second.second)) {
			dsu.unite(current.second.first, current.second.second);
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