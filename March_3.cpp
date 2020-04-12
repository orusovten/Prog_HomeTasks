// Сейчас Рику надо попасть из вселенной с номером S во вселенную с номером F.Он знает все существующие телепорты,
// и казалось бы нет никакой проблемы.Но, далеко не секрет, что за свою долгую жизнь Рик поссорился много с кем.
// Из своего личного опыта он знает, что при телепортациях есть вероятность, что его заставят ответить за свои слова.
// Если Рик знает вероятности быть прижатым к стенке на всех существующих телепортациях, помогите ему посчитать минимальную вероятность, 
// что он всё - таки столкнется с неприятностями.

#include <iostream>
#include <vector>

class Graph {
public:
    explicit Graph(int Size) {
        size = Size;
        for (int i = 0; i < size; ++i) {
            std::vector<std::pair<int, double>> VertexEdges;
            EdgesList.push_back(VertexEdges);
        }
    }
    void AddEdge(int from, int to, double weight);
    const std::vector<std::pair<int, double>>& GetNextVertexs(int from) const;
    int Size() const;
private:
    int size; // кол-во вершин
    std::vector<std::vector<std::pair<int, double>>> EdgesList; // вектор смежности
};

void Graph::AddEdge(int from, int to, double weight) {
    EdgesList[from].push_back(std::make_pair(to, weight));
}

const std::vector<std::pair<int, double>>& Graph::GetNextVertexs(int from) const {
    return EdgesList[from];
}

int Graph::Size() const{
    return size;
}

void Ford_Bellman(const Graph& graph, int start, std::vector<double>& dp) {
    dp[start] = 1;
    for (int l = 0; l < graph.Size() - 1; ++l) {
        for (int i = 0; i < graph.Size(); ++i) {
            std::vector<std::pair<int, double>> NextVertexs = graph.GetNextVertexs(i);
            for (auto vertex : NextVertexs) {
                if (dp[vertex.first] < vertex.second * dp[i]) {
                    dp[vertex.first] = vertex.second * dp[i];
                }
            }
        }
    }
}

double solution(const Graph& graph, int start, int end) {
    // сведем задачу к поиску пути с наиб произведением
    std::vector<double> dp(graph.Size(), 0);
    Ford_Bellman(graph, start, dp);
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
