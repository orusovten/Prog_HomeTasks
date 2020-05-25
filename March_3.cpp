// Сейчас Рику надо попасть из вселенной с номером S во вселенную с номером F.Он знает все существующие телепорты,
// и казалось бы нет никакой проблемы.Но, далеко не секрет, что за свою долгую жизнь Рик поссорился много с кем.
// Из своего личного опыта он знает, что при телепортациях есть вероятность, что его заставят ответить за свои слова.
// Если Рик знает вероятности быть прижатым к стенке на всех существующих телепортациях, помогите ему посчитать минимальную вероятность, 
// что он всё - таки столкнется с неприятностями.

#include <iostream>
#include <vector>
#include <set>

class Graph {
public:
    explicit Graph(int size) {
        for (int i = 0; i < size; ++i) {
            std::vector<std::pair<int, double>> VertexEdges;
            EdgesList.push_back(VertexEdges);
        }
    }
    void AddEdge(int from, int to, double weight);
    const std::vector<std::pair<int, double>>& GetNextVertexs(int from) const;
    int Size() const;
private:
    std::vector<std::vector<std::pair<int, double>>> EdgesList; // вектор смежности
};

void Graph::AddEdge(int from, int to, double weight) {
    EdgesList[from].push_back(std::make_pair(to, weight));
}

const std::vector<std::pair<int, double>>& Graph::GetNextVertexs(int from) const {
    return EdgesList[from];
}

int Graph::Size() const{
    return EdgesList.size();
}

void Dijkstra(const Graph& graph, int start, std::vector<double>& dp) {
    std::vector<bool> visited(graph.Size(), false);
    std::set<std::pair<double, int>, std::greater<std::pair<double, int>>> priority;
    dp[start] = 1;
    priority.insert(std::make_pair(1., start));
    while (!priority.empty()) {
        auto current = *priority.begin();
        priority.erase(current);
        auto NextVertexs = graph.GetNextVertexs(current.second);
        for (auto vertex : NextVertexs) {
            if (!visited[vertex.first] && current.first * vertex.second > dp[vertex.first]) {
                priority.erase(std::make_pair(dp[vertex.first], vertex.first));
                dp[vertex.first] = current.first * vertex.second;
                priority.insert(std::make_pair(dp[vertex.first], vertex.first));
            }
        }
        visited[current.second] = true;
    }
}

double solution(const Graph& graph, int start, int end) {
    // сведем задачу к поиску пути с наиб произведением
    std::vector<double> dp(graph.Size(), 0);
    Dijkstra(graph, start, dp);
    return 1 - dp[end];
}

int main()
{
    int N;
    int M;
    int start;
    int end;
    std::cin >> N >> M >> start >> end;
    Graph graph(N);
    for (int i = 0; i < M; ++i) {
        int a;
        int b;
        double weight;
        std::cin >> a >> b >> weight;
        graph.AddEdge(a - 1, b - 1, 1 - (weight / 100));
        graph.AddEdge(b - 1, a - 1, 1 - (weight / 100));
    }
    std::cout << solution(graph, start - 1, end - 1) << std::endl;
    return 0;
}
