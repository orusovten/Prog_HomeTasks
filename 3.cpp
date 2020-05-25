#include <iostream>
#include <stack>
#include <vector>

using std::cin;
using std::cout;

class Graph {
public:
    explicit Graph(int Size) {
        size = Size;
        for (int i = 0; i < size; ++i) {
            std::vector<int> VertexEdges;
            EdgesList.push_back(VertexEdges);
        }
    }
    void AddEdge(int from, int to);
    const std::vector<int>& GetNextVertexs(int from) const;
    void Invert();
    int Size() const;
private:
    int size; // кол-во вершин
    std::vector<std::vector<int>> EdgesList; // вектор смежности
};

void Graph::AddEdge(int from, int to) {
    EdgesList[from].push_back(to);
}

const std::vector<int>& Graph::GetNextVertexs(int from) const {
    return EdgesList[from];
}

void Graph::Invert() {
    std::vector<std::vector<int>> InvertingEdgesList(size);
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < EdgesList[i].size(); ++j) {
            InvertingEdgesList[EdgesList[i][j]].push_back(i);
        }
    }
    EdgesList = InvertingEdgesList;
}

int Graph::Size() const {
    return size;
}

void TopSort(const Graph& graph, int start_vertex, std::vector<int>& sorted_vertex, int& t, std::vector<int>& flags) {
    std::stack<int> stack;
    stack.push(start_vertex);
    flags[start_vertex] = 1;
    while (!stack.empty()) {
        int vertex = stack.top();
        bool is_go = false; // проверка на наличие белых соседей
        for (int i = 0; i < graph.GetNextVertexs(vertex).size(); ++i) {
            if (flags[graph.GetNextVertexs(vertex)[i]] == 0) {
                stack.push(graph.GetNextVertexs(vertex)[i]);
                flags[graph.GetNextVertexs(vertex)[i]] = 1;
                is_go = true;
                break;
            }
        }
        if (!is_go) {
            flags[vertex] = 2;
            sorted_vertex.push_back(vertex);
            //std::cout << vertex << " ";
            stack.pop();
        }
    }
}

// поиск компонент сильной связности(strong connectivity components)
std::vector<int> SCC_Search(const Graph& graph, int start_vertex, std::vector<int>& flags, std::vector<int>& colors, int color) {
    std::stack<int> stack;
    std::vector<int> scc;
    stack.push(start_vertex);
    flags[start_vertex] = 1;
    while (!stack.empty()) {
        int vertex = stack.top();
        bool is_go = false; // проверка на наличие белых соседей
        for (int i = 0; i < graph.GetNextVertexs(vertex).size(); ++i) {
            if (flags[graph.GetNextVertexs(vertex)[i]] == 0) {
                stack.push(graph.GetNextVertexs(vertex)[i]);
                flags[graph.GetNextVertexs(vertex)[i]] = 1;
                is_go = true;
                break;
            }
        }
        if (!is_go) {
            flags[vertex] = 2;
            scc.push_back(vertex);
            colors[vertex] = color;
            //std::cout << vertex << " ";
            stack.pop();
        }
    }
    return scc;
}

int max(int a, int b) {
    if (a > b) {
        return a;
    }
    return b;
}

int solution(Graph& graph) {
    std::vector<int> sorted_vertexs; // вершины в порядке убывания времени выхода
    int t = 0; // время выхода из последней обработанной вершины
    std::vector<int> flags(graph.Size(), 0); // флаги для DFS
    for (int i = 0; i < graph.Size(); ++i) {
        if (flags[i] == 0) {
            TopSort(graph, i, sorted_vertexs, t, flags);
        }
    }
    graph.Invert(); // инвертирование графа
    for (int i = 0; i < graph.Size(); ++i) {
        flags[i] = 0;
    }
    std::vector<std::vector<int>> vec_scc; // компоненты связности
    std::vector<int> colors(graph.Size(), -1); // цвет вершины определяет её КСС
    int color = 0;
    for (int i = graph.Size() - 1; i > -1; --i) {
        if (flags[sorted_vertexs[i]] == 0) {
            std::vector<int> new_scc = (SCC_Search(graph, sorted_vertexs[i], flags, colors, color));
            vec_scc.push_back(new_scc);
            color++;
        }
    }
    if (vec_scc.size() == 1) {
        return 0;
    }
    std::vector<bool> sources(vec_scc.size(), true);
    std::vector<bool> stocks(vec_scc.size(), true);
    for (int i = 0; i < vec_scc.size(); ++i) {
        for (int j = 0; j < vec_scc[i].size(); ++j) {
            for (int v = 0; v < graph.GetNextVertexs(vec_scc[i][j]).size(); ++v) {
                if (colors[graph.GetNextVertexs(vec_scc[i][j])[v]] != i) {
                    stocks[i] = false;
                    sources[colors[graph.GetNextVertexs(vec_scc[i][j])[v]]] = false;
                }
            }
        }
    }
    int stock = 0;
    for (int i = 0; i < vec_scc.size(); ++i) {
        if (stocks[i]) {
            stock++;
        }
    }
    int source = 0;
    for (int i = 0; i < vec_scc.size(); ++i) {
        if (sources[i]) {
            source++;
        }
    }
    return max(source, stock);
}

int main()
{
    int n;
    int m;
    cin >> n >> m;
    Graph graph(n);
    for (int i = 0; i < m; ++i) {
        int a;
        int b;
        cin >> a >> b;
        graph.AddEdge(a - 1, b - 1);
    }
    cout << solution(graph);
    return 0;
}