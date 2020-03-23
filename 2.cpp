#include <iostream>
#include <stack>
#include <vector>

using std::cin;
using std::cout;

class Graph {
public:
    Graph(int Size) {
        size = Size;
        for (int i = 0; i < size; ++i) {
            std::vector<int> VertexEdges;
            EdgesList.push_back(VertexEdges);
        }
    }
    void AddEdge(int from, int to);
    const std::vector<int>& GetNextVertexs(int from) const;
    int Size();
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

int Graph::Size() {
    return size;
}

bool TopSort(Graph& graph, int start_vertex, std::vector<int>& colors, std::vector<int>& vec_for_yes) {
    std::stack<int> stack;
    stack.push(start_vertex);
    colors[start_vertex] = 1;
    while (!stack.empty()) {
        int vertex = stack.top();
        bool is_go = false; // проверка на наличие белых соседей
        for (int i = graph.GetNextVertexs(vertex).size() - 1; i > -1; --i) {
            if (colors[graph.GetNextVertexs(vertex)[i]] == 1) {
                return false;
            }
            if (colors[graph.GetNextVertexs(vertex)[i]] == 0) {
                colors[graph.GetNextVertexs(vertex)[i]] = 1;
                stack.push(graph.GetNextVertexs(vertex)[i]);
                is_go = true;
                break;
            }
        }
        if (!is_go) {
            colors[vertex] = 2;
            vec_for_yes.push_back(vertex);
            stack.pop();
        }
    }
    return true;
}

std::string solution(std::vector<int>& vec_for_yes, Graph& graph) {
    std::vector<int> colors(graph.Size());
    for (int i = 0; i < graph.Size(); ++i) {
        colors[i] = 0;
    }
    bool check = true;
    for (int i = 0; i < graph.Size(); ++i) {
        if (colors[i] == 0) {
            check *= TopSort(graph, i, colors, vec_for_yes);
        }
    }
    if (check) {
        return "YES";
    }
    return "NO";
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
        graph.AddEdge(a, b);
    }
    std::vector<int> vec_for_yes;
    std::string ans = solution(vec_for_yes, graph);
    if (ans == "YES") {
        cout << ans << std::endl;
        for (int i = vec_for_yes.size() - 1; i > -1; --i) {
            cout << vec_for_yes[i] << " ";
        }
    }
    else {
        cout << ans;
    }
    return 0;
}
