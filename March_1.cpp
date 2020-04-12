// Рик и Морти снова бороздят просторы вселенных, но решили ограничиться только теми, номера которых меньше M.
// Они могут телепортироваться из вселенной с номером z во вселенную (z+1) mod M за a бутылок лимонада или
// во вселенную (z2+1) mod M за b бутылок лимонада. Рик и Морти хотят добраться из вселенной с номером x во вселенную с номером y. 
// Сколько бутылок лимонада отдаст Рик за такое путешествие, если он хочет потратить их как можно меньше?

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

void Dijkstra(const Graph& graph, int start, std::vector<int>& distance) {
    std::vector<bool> visited(graph.Size(), false);
    std::set<std::pair<int, int>> priority;
    distance[start] = 0;
    priority.insert(std::make_pair(0, start));
    while (!priority.empty()) {
        auto current = *priority.begin();
        priority.erase(current);
        std::vector<std::pair<int, int>> NextVertexs = graph.GetNextVertexs(current.second);
        for (auto vertex : NextVertexs) {
            if (!visited[vertex.first] && current.first + vertex.second < distance[vertex.first]) {
                priority.erase(std::make_pair(distance[vertex.first], vertex.first));
                distance[vertex.first] = current.first + vertex.second;
                priority.insert(std::make_pair(distance[vertex.first], vertex.first));
            }
        }
        visited[current.second] = true;
    }
}

int solution(int a, int b, long long M, int start, int end) {
    Graph graph(M);
    for (long long i = 0; i < M; ++i) {
        graph.AddEdge(i, (i + 1) % M, a);
        graph.AddEdge(i, (i * i + 1) % M, b);
    }
    int MAX = 2147483647; // наиб int
    std::vector<int> distance(M, MAX);
    Dijkstra(graph, start, distance);
    return distance[end];
}

int main()
{
	int a;
	int b;
	long long M;
	int start;
	int end;
	std::cin >> a >> b >> M >> start >> end;
    std::cout << solution(a, b, M, start, end);
    return 0;
}
