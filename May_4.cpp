// Решить задачу LCA

#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

using ull = unsigned long long;

class Graph {
public:
    explicit Graph(int size) {
        for (int i = 0; i < size; ++i) {
            std::vector<int> VertexEdges;
            EdgesList.push_back(VertexEdges);
        }
    }
    void AddEdge(int from, int to);
    const std::vector<int>& GetNextVertexs(int from) const;
    int Size() const;
private:
    std::vector<std::vector<int>> EdgesList; // вектор смежности
};

void Graph::AddEdge(int from, int to) {
    EdgesList[from].push_back(to);
}

const std::vector<int>& Graph::GetNextVertexs(int from) const {
    return EdgesList[from];
}

int Graph::Size() const {
    return EdgesList.size();
}

void DFS(const Graph& graph, std::vector<int>& vtx, std::vector<int>& index, 
         std::vector<int>& d, const std::vector<int>& parents) {
    std::vector<int> depth(graph.Size(), 0);
    std::vector<int> colors(graph.Size(), 0);
    std::vector<bool> isFirstEntry(graph.Size(), false); // первый ли раз мы вошли в в-ну
    std::stack<int> stack;
    stack.push(0);
    colors[0] = 1;
    depth[0] = 0;
    while (!stack.empty()) {
        int vertex = stack.top();
        if (!isFirstEntry[vertex]) {
            index[vertex] = d.size();
            d.push_back(depth[vertex]);
            vtx.push_back(vertex);
            isFirstEntry[vertex] = true;
        }
        bool is_go = false; // проверка на наличие белого ребенка
        auto nextVertexs = graph.GetNextVertexs(vertex);
        for (auto next : nextVertexs) {
            if (colors[next] == 0) {
                depth[next] = depth[vertex] + 1;
                colors[next] = 1;
                stack.push(next);
                is_go = true;
                break;
            }
        }
        if (!is_go) {
            if (vertex != 0) {
                d.push_back(depth[parents[vertex]]);
                vtx.push_back(parents[vertex]);
            }
            colors[vertex] = 2;
            stack.pop();
        }
    }
}

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
    return std::min(buffer[level][left], buffer[level][right - (int(1) << level) + 1]);
}

ull solution(const std::vector<int>& parents, ull a1, ull a2, int x, int y, int z, const Graph& graph, int M) {
    std::vector<int> vtx;
    std::vector<int> index(graph.Size());
    std::vector<int> d;
    DFS(graph, vtx, index, d, parents);
    SparseTable table(d);
    ull ans = 0;
    ull cur = 0;
    ull first_a = a1;
    ull second_a = a2;
    for (int i = 0; i < M; ++i) {
        int index1 = index[(first_a + cur) % graph.Size()];
        int index2 = index[second_a];
        if (index1 < index2) {
            cur = vtx[table.findMin(index1, index2).second];
        }
        else {
            cur = vtx[table.findMin(index2, index1).second];
        }
        ans += cur;
        first_a = (first_a * x + second_a * y + z) % graph.Size();
        second_a = (second_a * x + first_a * y + z) % graph.Size();
    }
    return ans;
}

int main() {
    int N;
    int M;
    std::cin >> N >> M;
    Graph graph(N);
    std::vector<int> parents(N);
    for (int i = 1; i < N; ++i) {
        int parent;
        std::cin >> parent;
        graph.AddEdge(parent, i);
        parents[i] = parent;
    }
    ull a1;
    ull a2;
    std::cin >> a1 >> a2;
    ull x;
    ull y;
    ull z;
    std::cin >> x >> y >> z;
    std::cout << solution(parents, a1, a2, x, y, z, graph, M) << std::endl;
}
