// Построить дерево отрезков с двумя видами запросов:
// 1. Прибавить x к отрезку [l, r]
// 2. Узнать макс эл-т на отрезке [l, r]

#include <iostream>
#include <vector>

class SegmentTree {
private:
    void build(const std::vector<int>& vec, int vertex, int tl, int tr);
	int findMax(int vertex, int tl, int tr, int left, int right);
	void update(int vertex, int tl, int tr, int left, int right, int value);
	std::vector<int> buffer;
	std::vector<int> add;
public:
    explicit SegmentTree(const std::vector<int>& vec) {
        buffer = std::vector<int>(4 * vec.size(), 0);;
		add = std::vector<int>(4 * vec.size(), 0);
		build(vec, 1, 0, vec.size() - 1);
    }
	int Max(int left, int right);
	void Update(int left, int right, int value);
};

int max(int a, int b) {
	return a > b ? a : b;
}

int min(int a, int b) {
	return a < b ? a : b;
}

void SegmentTree::build(const std::vector<int>& vec, int vertex, int tl, int tr) {
	if (tl == tr) {
		buffer[vertex] = vec[tl];
	}
	else {
		int tm = (tl + tr) / 2;
		build(vec, vertex * 2, tl, tm);
		build(vec, vertex * 2 + 1, tm + 1, tr);
		buffer[vertex] = max(buffer[vertex * 2], buffer[vertex * 2 + 1]);
	}
}

int SegmentTree::findMax(int vertex, int tl, int tr, int left, int right) {
	if (left == tl && right == tr) {
		return buffer[vertex];
	}
	int tm = (tl + tr) / 2;
	if (left > right) {
		return 0;
	}
	return max(findMax(vertex * 2, tl, tm, left, min(right, tm)), findMax(vertex * 2 + 1, tm + 1, tr, max(left, tm + 1), right)) + add[vertex];
}

int SegmentTree::Max(int left, int right) {
	return findMax(1, 0, buffer.size() / 4 - 1, left, right);
}

void SegmentTree::update(int vertex, int tl, int tr, int left, int right, int value) {
	if (left == tl && right == tr) {
		add[vertex] += value;
		buffer[vertex] += value;
	}
	else if (left <= right) {
		int tm = (tl + tr) / 2;
		update(vertex * 2, tl, tm, left, min(right, tm), value);
		update(vertex * 2 + 1, tm + 1, tr, max(left, tm + 1), right, value);
		buffer[vertex] = max(buffer[vertex * 2], buffer[vertex * 2 + 1]) + add[vertex];
	}
}

void SegmentTree::Update(int left, int right, int value) {
	return update(1, 0, buffer.size() / 4 - 1, left, right, value);
}

std::vector<int> solution(const std::vector<int>& road, int M, int maxValue) {
	SegmentTree tree(road);
	std::vector<int> ans;
	for (int i = 0; i < M; ++i) {
		int left;
		int right;
		int value;
		std::cin >> left >> right >> value;
		if (tree.Max(left, right - 1) + value <= maxValue) {
			tree.Update(left, right - 1, value);
		}
		else {
			ans.push_back(i);
		}
	}
	return ans;
}

int main()
{
	int N;
	std::cin >> N;
	std::vector<int> road(N - 1);
	for (int i = 0; i < N - 1; ++i) {
		int elem;
		std::cin >> elem;
		road[i] = elem;
	}
	int maxValue;
	std::cin >> maxValue;
	int M;
	std::cin >> M;
	std::vector<int> ans = solution(road, M, maxValue);
	for (auto elem : ans) {
		std::cout << elem << " ";
	}
	std::cout << std::endl;
}
