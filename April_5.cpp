#include <iostream>
#include <vector>
#include <set>
#include <stack>

class Graph {
public:
    explicit Graph(int Size) {
        size = Size;
        for (int i = 0; i < size; ++i) {
            std::multiset<int> VertexEdges;
            EdgesList.push_back(VertexEdges);
        }
    }
    void AddEdge(int from, int to);
    void DelEdge(int from, int to);
    bool HasEdge(int from, int to) const;
    const std::multiset<int>& GetNextVertexs(int from) const;
    int Size() const;
private:
    int size; // кол-во вершин
    std::vector<std::multiset<int>> EdgesList; // вектор смежности
};

void Graph::AddEdge(int from, int to) {
    EdgesList[from].insert(to);
}

void Graph::DelEdge(int from, int to) {
    auto iterTo = EdgesList[from].find(to);
    if (iterTo != EdgesList[from].end())
        EdgesList[from].erase(iterTo);
}

bool Graph::HasEdge(int from, int to) const {
    return EdgesList[from].find(to) != EdgesList[from].end();
}

const std::multiset<int>& Graph::GetNextVertexs(int from) const {
    return EdgesList[from];
}

int Graph::Size() const {
    return size;
}

void Get_and_InvertRoad(Graph& graph, std::vector<int>& road, int s, int t, const std::vector<int>& parent) {
    int current = t;
    road.push_back(t);
    while (parent[current] != s) {
        graph.DelEdge(parent[current], current);
        graph.AddEdge(current, parent[current]);
        road.push_back(parent[current]);
        current = parent[current];
    }
    graph.DelEdge(s, current);
    graph.AddEdge(current, s);
    road.push_back(s);
}

bool DFS(Graph& graph, int s, int t, std::vector<int>& road) {
    std::vector<int> flags(graph.Size(), 0);
    std::vector<int> parent(graph.Size(), -1);
    std::stack<int> stack;
    stack.push(s);
    flags[s] = 1;
    while (!stack.empty()) {
        int vertex = stack.top();
        if (vertex == t) {
            break;
        }
        bool is_go = false; // проверка на наличие белых соседей
        auto neighs = graph.GetNextVertexs(vertex);
        for (auto neigh : neighs) {
            if (flags[neigh] == 0) {
                stack.push(neigh);
                flags[neigh] = 1;
                parent[neigh] = vertex;
                is_go = true;
                break;
            }
        }
        if (!is_go) {
            flags[vertex] = 2;
            stack.pop();
        }
    }
    if (parent[t] == -1) {
        return false;
    }
    Get_and_InvertRoad(graph, road, s, t, parent);
    return true;
}

std::vector<std::vector<int>> Edmonds_Carp(Graph& graph, int s, int t) {
    std::vector<int> firstRoad;
    std::vector<int> secondRoad;
    if (DFS(graph, s, t, firstRoad) && DFS(graph, s, t, secondRoad)) {
        Graph restGraph(graph.Size());
        for (int i = 0; i < firstRoad.size() - 1; ++i) {
            restGraph.AddEdge(firstRoad[i], firstRoad[i + 1]);
        }
        for (int i = 0; i < secondRoad.size() - 1; ++i) {
            if (restGraph.HasEdge(secondRoad[i + 1], secondRoad[i])) {
                restGraph.DelEdge(secondRoad[i + 1], secondRoad[i]);
            }
            else {
                restGraph.AddEdge(secondRoad[i], secondRoad[i + 1]);
            }
        }
        std::vector<int> restFirstRoad;
        DFS(restGraph, t, s, restFirstRoad);
        std::vector<int> restSecondRoad;
        DFS(restGraph, t, s, restSecondRoad);
        std::vector<std::vector<int>> ans;
        ans.push_back(restFirstRoad);
        ans.push_back(restSecondRoad);
        return ans;
    }
    std::vector<std::vector<int>> badAns;
    return badAns;
}


int main()
{
    int N;
    int M;
    int s;
    int t;
    std::cin >> N >> M >> s >> t;
    Graph graph(N);
    for (int i = 0; i < M; ++i) {
        int a;
        int b;
        std::cin >> a >> b;
        graph.AddEdge(a - 1, b - 1);
    }
    std::vector<std::vector<int>> two_roads = Edmonds_Carp(graph, s - 1, t - 1);
    if (two_roads.size() < 2) {
        std::cout << "NO" << std::endl;
    }
    else {
        std::cout << "YES" << std::endl;
        for (auto vertex : two_roads[0]) {
            std::cout << vertex + 1 << " ";
        }
        std::cout << std::endl;
        for (auto vertex : two_roads[1]) {
            std::cout << vertex + 1 << " ";
        }
    }
}
