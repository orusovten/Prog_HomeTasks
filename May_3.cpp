// Построить дерево отрезков с двумя видами запросов:
// 1. Присвоение к x отрезка [l, r]
// 2. Узнать мин эл-т на отрезке [l, r]

#include <iostream>
#include <vector>

class SegmentTree {
private:
	void push(int vertex);
	void build(const std::vector<int>& vec, int vertex, int tl, int tr);
	int findMin(int vertex, int tl, int tr, int left, int right);
	void paint(int vertex, int tl, int tr, int left, int right, int value);
public:
	std::vector<int> buffer;
	std::vector<int> color;
	explicit SegmentTree(const std::vector<int>& vec) {
		buffer = std::vector<int>(4 * vec.size(), 2147483647);;
		color = std::vector<int>(4 * vec.size(), 2147483647);
		build(vec, 1, 0, vec.size() - 1);
	}
	int Min(int left, int right);
	void Paint(int left, int right, int value);
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
		buffer[vertex] = min(buffer[vertex * 2], buffer[vertex * 2 + 1]);
	}
}

int SegmentTree::findMin(int vertex, int tl, int tr, int left, int right) {
	if (left == tl && right == tr) {
		return buffer[vertex];
	}
	if (left > right) {
		return 2147483647;
	}
	push(vertex);
	int tm = (tl + tr) / 2;
	int minChildColor = min(findMin(vertex * 2, tl, tm, left, min(right, tm)), findMin(vertex * 2 + 1, tm + 1, tr, max(left, tm + 1), right));
	return minChildColor;
}

int SegmentTree::Min(int left, int right) {
	return findMin(1, 0, buffer.size() / 4 - 1, left, right);
}

void SegmentTree::push(int vertex) {
	if (color[vertex] != 2147483647) {
		color[vertex * 2] = color[vertex];
		color[vertex * 2 + 1] = color[vertex];
		buffer[vertex * 2] = color[vertex];
		buffer[vertex * 2 + 1] = color[vertex];
		color[vertex] = 2147483647;
	}
}

void SegmentTree::paint(int vertex, int tl, int tr, int left, int right, int value) {
	if (left == tl && right == tr) {
		color[vertex] = value;
		buffer[vertex] = value;
	}
	else if (left <= right) {
		push(vertex);
		int tm = (tl + tr) / 2;
		paint(vertex * 2, tl, tm, left, min(right, tm), value);
		paint(vertex * 2 + 1, tm + 1, tr, max(left, tm + 1), right, value);
		buffer[vertex] = min(buffer[vertex * 2], buffer[vertex * 2 + 1]);
	}
}

void SegmentTree::Paint(int left, int right, int value) {
	return paint(1, 0, buffer.size() / 4 - 1, left, right, value);
}

std::vector<int> solution(const std::vector<int>& road, int M) {
	SegmentTree tree(road);
	std::vector<int> ans;
	for (int i = 0; i < M; ++i) {
		int C; 
		int D;
		int R; 
		int G;
		int B;
		int E;
		int F;
		std::cin >> C >> D >> R >> G >> B >> E >> F;
		tree.Paint(C, D, R + G + B);
		ans.push_back(tree.Min(E, F));
	}
	return ans;
}

int main()
{
	int N;
	std::cin >> N;
	std::vector<int> tape(N);
	for (int i = 0; i < N; ++i) {
		int R;
		int G;
		int B;
		std::cin >> R >> G >> B;
		tape[i] = R + G + B;
	}
	int M;
	std::cin >> M;
	std::vector<int> ans = solution(tape, M);
	for (auto elem : ans) {
		std::cout << elem << " ";
	}
	std::cout << std::endl;
}
