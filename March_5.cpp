#include <iostream>
#include <vector>

class Graph {
public:
    explicit Graph(int Size) {
        size = Size;
        for (int i = 0; i < size; ++i) {
            std::vector<std::pair<int, std::pair<double, double>>> VertexEdges;
            EdgesList.push_back(VertexEdges);
        }
    }
    void AddEdge(int from, int to, double weight, double commision);
    const std::vector<std::pair<int, std::pair<double, double>>>& GetNextVertexs(int from) const;
    int Size() const;
private:
    int size; // кол-во вершин
    std::vector<std::vector<std::pair<int, std::pair<double, double>>>> EdgesList; // вектор смежности
};

void Graph::AddEdge(int from, int to, double weight, double commision) {
    EdgesList[from].push_back(std::make_pair(to, std::make_pair(weight, commision)));
}

const std::vector<std::pair<int, std::pair<double, double>>>& Graph::GetNextVertexs(int from) const {
    return EdgesList[from];
}

int Graph::Size() const {
    return size;
}

bool Ford_Bellman(const Graph& graph, int start, double svalue, std::vector<double>& dp) {
    dp[start] = svalue;
    for (int l = 0; l < graph.Size() - 1; ++l) {
        for (int i = 0; i < graph.Size(); ++i) {
            std::vector<std::pair<int, std::pair<double, double>>> NextVertexs = graph.GetNextVertexs(i);
            for (auto vertex : NextVertexs) {
                if (dp[vertex.first] < vertex.second.first * (dp[i] - vertex.second.second)) {
                    dp[vertex.first] = vertex.second.first * (dp[i] - vertex.second.second);
                }
            }
        }
    }
    for (int i = 0; i < graph.Size(); ++i) {
        std::vector<std::pair<int, std::pair<double, double>>> NextVertexs = graph.GetNextVertexs(i);
        for (auto vertex : NextVertexs) {
            if (dp[vertex.first] < vertex.second.first * (dp[i] - vertex.second.second)) {
                dp[vertex.first] = vertex.second.first * (dp[i] - vertex.second.second);
                return true;
            }
        }
    }
    return false;
}

std::string solution(const Graph& graph, int start, double svalue) {
    std::vector<double> dp(graph.Size(), 0);
    if (Ford_Bellman(graph, start, svalue, dp)) {
        return "YES";
    }
    return "NO";
}

int main()
{
    int n; // кол-во валют
    int m; // кол-во обменников(рёбер)
    int start; // изначальная валюта
    double value_of_start;  // кол-во изнач.валюты
    std::cin >> n >> m >> start >> value_of_start;
    Graph graph(n);
    for (int i = 0; i < m; ++i) {
        int a;
        int b;
        double rate_ab;
        double commision_ab;
        double rate_ba;
        double commision_ba;
        std::cin >> a >> b >> rate_ab >> commision_ab >> rate_ba >> commision_ba;
        graph.AddEdge(a - 1, b - 1, rate_ab, commision_ab);
        graph.AddEdge(b - 1, a - 1, rate_ba, commision_ba);
    }
    std::cout << solution(graph, start - 1, value_of_start) << std::endl;
    return 0;
}
