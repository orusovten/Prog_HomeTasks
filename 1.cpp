// Леон и Матильда собрались пойти в магазин за молоком. Их хочет поймать Стэнсфилд,
// поэтому нашим товарищам нужно сделать это как можно быстрее. Каково минимальное количество улиц,
// по которым пройдёт хотя бы один из ребят (либо Матильда, либо Леон, либо оба вместе)? 
#include <iostream>
#include <vector>
#include <queue>

using std::cout;
using std::cin;

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

void BFS(Graph& graph, int from, int*& ans_count) {
    std::queue<int> queue;
    queue.push(from);
    int* colors = new int[graph.Size()];
    for (int i = 0; i < graph.Size(); ++i) {
        colors[i] = 0;
    }
    ans_count[from] = 0;
    colors[from] = 1;
    while (!queue.empty()) {
        int vertex = queue.front();
        for (int i = 0; i < graph.GetNextVertexs(vertex).size(); ++i) {
            if (colors[graph.GetNextVertexs(vertex)[i]] == 0)
            {
                queue.push(graph.GetNextVertexs(vertex)[i]);
                colors[graph.GetNextVertexs(vertex)[i]] = 1;
                ans_count[graph.GetNextVertexs(vertex)[i]] = ans_count[vertex] + 1;
            }
        }
        queue.pop();
    }
    delete[] colors;
}

int solution(Graph& graph, int leon, int mat, int milk) {
    int* ans_count1 = new int[graph.Size()];
    for (int i = 0; i < graph.Size(); ++i) {
        ans_count1[i] = 0;
    }
    BFS(graph, leon, ans_count1);
    int* ans_count2 = new int[graph.Size()];
    for (int i = 0; i < graph.Size(); ++i) {
        ans_count2[i] = 0;
    }
    BFS(graph, mat, ans_count2);
    int* ans_count3 = new int[graph.Size()];
    for (int i = 0; i < graph.Size(); ++i) {
        ans_count3[i] = 0;
    }
    BFS(graph, milk, ans_count3);
    int ans = ans_count1[0] + ans_count2[0] + ans_count3[0];
    for (int i = 1; i < graph.Size(); ++i) {
        if (ans_count1[i] + ans_count2[i] + ans_count3[i] < ans) {
            ans = ans_count1[i] + ans_count2[i] + ans_count3[i];
        }
    }
    delete[] ans_count1;
    delete[] ans_count2;
    delete[] ans_count3;
    return ans;
}

int main()
{
    int n;
    int m;
    int leon;
    int mat;
    int milk;
    cin >> n >> m >> leon >> mat >> milk;
    Graph graph(n);
    for (int i = 0; i < m; ++i) {
        int a;
        int b;
        cin >> a >> b;
        graph.AddEdge(a - 1, b - 1);
        graph.AddEdge(b - 1, a - 1);
    }
    cout << solution(graph, leon - 1, mat - 1, milk - 1);
    return 0;
}