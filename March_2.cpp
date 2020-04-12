// Рику необходимо попасть на межвселенную конференцию. За каждую телепортацию он платит бутылками лимонада,
// поэтому хочет потратить их на дорогу как можно меньше (он же всё-таки на конференцию едет!).
// Однако после K перелетов подряд Рика начинает сильно тошнить, и он ложится спать на день. Ему известны все существующие телепортации. 
// Теперь Рик хочет найти путь (наименьший по стоимости в бутылках лимонада), учитывая, что телепортация не занимает времени,
// а до конференции осталось 10 минут (то есть он может совершить не более K перелетов)!

#include <iostream>
#include <vector>

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
    int cost(int from, int to);
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

int Graph::cost(int from, int to) {
    return EdgesList[from][to].second;
}

void Ford_Bellman(const Graph& graph, int k, int start, std::vector<int>& dp) {
    dp[start] = 0;
    for (int l = 1; l < k; ++l) {
        for (int i = 0; i < graph.Size(); ++i) {
            std::vector<std::pair<int, int>> NextVertexs = graph.GetNextVertexs(i);
            for (auto vertex : NextVertexs) {
                if (vertex.second < dp[vertex.first] - dp[i]) {
                    dp[vertex.first] = vertex.second + dp[i];
                }
            }
        }
    }
}

int solution(const Graph& graph, int k, int start, int end) {
    int MAX = 2147483647; // наиб int
    std::vector<int> dp(graph.Size(), MAX);
    Ford_Bellman(graph, k, start, dp);
    return dp[end] == MAX ? -1 : dp[end];
}

int main()
{ 
    int N;
    int M;
    int k;
    int start;
    int end;
    std::cin >> N >> M >> k >> start >> end;
    Graph graph(N);
    for (int i = 0; i < M; ++i) {
        int a;
        int b;
        int weight;
        std::cin >> a >> b >> weight;
        graph.AddEdge(a - 1, b - 1, weight);
    }
    std::cout << solution(graph, k, start - 1, end - 1) << std::endl;
    return 0;
}
