#include <iostream>
#include <vector>
#include <set>

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

int Graph::Size() const {
    return size;
}

std::vector<bool> solution(Graph& graph) {
    int best_ans = 2147483647;
    std::vector<bool> best_cut;
    std::vector<std::set<int>> Union(graph.Size()); // с какими в-нами объединена
    std::vector<bool> exist(graph.Size(), true); // существует ли в-на(не объединена ли с другой)
    for (int first = 0; first < graph.Size() - 1; ++first) {
        std::vector<int> weight(graph.Size(), 0);
        std::set<std::pair<int, int>> notTaken;
        std::vector<bool> taken(graph.Size(), false);
        taken[first] = true;
        std::vector<int> first_neighs = graph.GetNextVertexs(first);
        for (int vertex = 0; vertex < graph.Size(); ++vertex) {
            if (exist[vertex] && vertex != first) {
                notTaken.insert(std::make_pair(weight[vertex], vertex));
            }
        }
        for (auto neigh : first_neighs) {
            if (exist[neigh] && !taken[neigh]) {
                notTaken.erase(std::make_pair(weight[neigh], neigh));
                weight[neigh] += 1;
                notTaken.insert(std::make_pair(weight[neigh], neigh));
            }
        }
        int prev = 0;
        while (!notTaken.empty()) {
            auto last_pair = *notTaken.rbegin();
            notTaken.erase(last_pair);
            int last = last_pair.second;
            taken[last] = true;
            std::vector<int> neighs = graph.GetNextVertexs(last);
            if (notTaken.size() == 0) {
                if (weight[last] < best_ans) {
                    best_ans = weight[last];
                    best_cut = std::vector<bool>(graph.Size(), false);
                    for (auto vertex : Union[last]) {
                        best_cut[vertex] = true;
                    }
                    best_cut[last] = true;
                }
                for (auto vertex : Union[last]) {
                    Union[prev].insert(vertex);
                }
                Union[prev].insert(last);
                for (auto neigh : neighs) {
                    if (exist[neigh]) {
                        graph.AddEdge(neigh, prev);
                        graph.AddEdge(prev, neigh);
                    }
                }
                exist[last] = false;
            }
            else {
                for (auto neigh : neighs) {
                    if (!taken[neigh] && exist[neigh]) {
                        notTaken.erase(std::make_pair(weight[neigh], neigh));
                        weight[neigh] += 1;
                        notTaken.insert(std::make_pair(weight[neigh], neigh));
                    }
                }
                prev = last;
            }
        }
    }
    return best_cut;
}


int main()
{
    int N;
    std::cin >> N;
    Graph graph(N);
    for (int i = 0; i < N; ++i) {
        std::string line;
        std::cin >> line;
        for (int j = 0; j < N; ++j) {
            if (line[j] == '1') {
                graph.AddEdge(i, j);
            }
        }
    }
    std::vector<bool> two_parts = solution(graph);
    for (size_t i = 0; i < two_parts.size(); ++i) {
        if (!two_parts[i])
            std::cout << i + 1 << " ";
    }
    std::cout << std::endl;
    for (size_t i = 0; i < two_parts.size(); ++i) {
        if (two_parts[i])
            std::cout << i + 1 << " ";
    }
    return 0;
}
