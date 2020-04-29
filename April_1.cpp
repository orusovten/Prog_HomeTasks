//Требуется найти в связном графе остовное дерево минимального веса.
//Воспользуйтесь алгоритмом Прима.

#include <iostream>
#include <vector>
#include <set>

class Graph {
public:
    explicit Graph(int Size) {
        size = Size;
        for (int i = 0; i < size; ++i) {
            std::vector<std::pair<int, int>> VertexEdges;
            EdgesList.push_back(VertexEdges);
        }
    }
    void AddEdge(int from, int to, int weight);
    const std::vector<std::pair<int, int>>& GetNextVertexs(int from) const;
    int Size() const;
private:
    int size; // кол-во вершин
    std::vector<std::vector<std::pair<int, int>>> EdgesList; // вектор смежности
};

void Graph::AddEdge(int from, int to, int weight) {
    EdgesList[from].push_back(std::make_pair(to, weight));
}

const std::vector<std::pair<int, int>>& Graph::GetNextVertexs(int from) const {
    return EdgesList[from];
}

int Graph::Size() const {
    return size;
}

void Prim(const Graph& graph, int start, std::vector<int>& key) {
    std::vector<bool> visited(graph.Size(), false); // лежит ли в-на в остове
    std::set<std::pair<int, int>> priority; // левое значение - вес ребра, правое - в-на, е лежащая в остове
    std::vector<std::pair<int, int>> NextVertexs = graph.GetNextVertexs(start);
    for (auto vertex : NextVertexs) {
        priority.insert(std::make_pair(vertex.second, vertex.first));
        key[vertex.first] = vertex.second;
    }
    visited[start] = true;
    key[start] = 0;
    while (!priority.empty()) {
        auto current = *priority.begin();
        priority.erase(current);
        std::vector<std::pair<int, int>> NextVertexs = graph.GetNextVertexs(current.second);
        for (auto vertex : NextVertexs) {
            if (!visited[vertex.first] && vertex.second < key[vertex.first]) {
                priority.erase(std::make_pair(key[vertex.first], vertex.first));
                key[vertex.first] = vertex.second;
                priority.insert(std::make_pair(key[vertex.first], vertex.first));
            }
        }
        visited[current.second] = true;

    }
}

int solution(int N, int M) {
    Graph graph(N);
    for (int i = 0; i < M; ++i) {
        int a;
        int b;
        int weight;
        std::cin >> a >> b >> weight;
        graph.AddEdge(a - 1, b - 1, weight);
        graph.AddEdge(b - 1, a - 1, weight);
    }
    int MAX = 2147483647; // наиб int
    std::vector<int> key(N, MAX);
    Prim(graph, 0, key);
    int sum = 0;
    for (int ans : key) {
        sum += ans;
    }
    return sum;
}

int main()
{
    int N;
    int M;
    std::cin >> N >> M;
    std::cout << solution(N, M);
    return 0;
}