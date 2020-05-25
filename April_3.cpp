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

std::set<std::pair<long long, std::pair<int, int>>> Kruskal(int N, std::set<std::pair<long long, std::pair<int, int>>>& priority) {
	std::vector<int> parent(N);
	for (int i = 0; i < N; ++i) {
		parent[i] = i;
	}
	std::vector<int> size(N, 1);
	std::set<std::pair<long long, std::pair<int, int>>> skeleton; // остов
	while (!priority.empty()) {
		auto current = *priority.begin();
		priority.erase(current);
		if (get(current.second.first, parent) != get(current.second.second, parent)) {
			unite(current.second.first, current.second.second, parent, size);
			skeleton.insert(current);
		}
	}
	return skeleton;
}

long long solution(int N, int M) {
	std::vector<long long> self_weights(N);
	int min_a = 0; // в-на с мин собств весом
	long long min_self_weight = 10000000000000;
	for (int i = 0; i < N; ++i) {
		long long self_weight;
		std::cin >> self_weight;
		if (self_weight < min_self_weight) {
			min_a = i;
			min_self_weight = self_weight;
		}
		self_weights[i] = self_weight;
	}
	std::set<std::pair<long long, std::pair<int, int>>> priority; // левый аргумент - вес ребра, остальные 2 - его концы
	for (int i = 0; i < M; ++i) {
		int a;
		int b;
		long long weight;
		std::cin >> a >> b >> weight;
		priority.insert(std::make_pair(weight, std::make_pair(a - 1, b - 1)));
	}
	for (int i = 0; i < min_a; ++i) {
		priority.insert(std::make_pair(self_weights[i] + min_self_weight, std::make_pair(min_a, i)));
	}
	for (int i = min_a + 1; i < N; ++i) {
		priority.insert(std::make_pair(self_weights[i] + min_self_weight, std::make_pair(min_a, i)));
	}
	priority = Kruskal(N, priority);
	long long sum = 0;
	for (auto ans : priority) {
		sum += ans.first;
	}
	return sum;
}

int main() {
	int N;
	int M;
	std::cin >> N >> M;
	std::cout << solution(N, M) << std::endl;
	return 0;
}