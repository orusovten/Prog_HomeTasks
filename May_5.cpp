#include <iostream>
#include <vector>
#include <stack>
#include <cstdlib>

struct Node {
    int Size = 1; // размер поддерева
    int Priority;
    std::string Value;
    Node* Left = nullptr;
    Node* Right = nullptr;
    bool Flag = false; // для обхода
    Node() : Priority(), Value() {};
    Node(int priority, const std::string& value) : Priority(priority), Value(value) {};
};

// Декартово дерево по неявному ключу
class ImplicitTreap {
public:
    ImplicitTreap() {
        root = nullptr;
    }
    ~ImplicitTreap() {
        // при вызове деструктора флаги у всех узлов одинаковы, поэтому достаточно знать флаг корня
        bool root_flag = root->Flag;
        std::stack<Node*> stack;
        stack.push(root);
        while (!stack.empty()) {
            Node* node = stack.top();
            if (node->Left && (node->Left)->Flag == root_flag) {
                stack.push(node->Left);
            }
            else if (node->Right && (node->Right)->Flag == root_flag) {
                stack.push(node->Right);
            }
            else {
                node->Flag = !root_flag;
                delete node;
                stack.pop();
            }
        }
    }
    void InsertAt(int position, const std::string& value);
    void DeleteAt(int position, int second_position);
    std::string GetAt(int position);
private:
    int size(Node*& node);
    void fixSize(Node*& node);
    void split(Node* node, int position, Node*& T1, Node*& T2);
    Node* merge(Node*& T1, Node*& T2);
    Node* root;
};

int ImplicitTreap::size(Node*& node) {
    if (!node) {
        return 0;
    }
    return node->Size;
}

int max(int a, int b) {
    if (a > b)
    {
        return a;
    }
    return b;
}

void ImplicitTreap::fixSize(Node*& node) {
    node->Size = size(node->Left) + size(node->Right) + 1;
}

void ImplicitTreap::split(Node* node, int position, Node*& L, Node*& R) {
    if (!node)
    {
        L = nullptr;
        R = nullptr;
        return;
    }
    if (position <= size(node->Left))
    {
        R = node;
        split(node->Left, position, L, R->Left);
    }
    else
    {
        L = node;
        split(node->Right, position - size(node->Left) - 1, L->Right, R);
    }
    fixSize(node);
}

Node* ImplicitTreap::merge(Node*& L, Node*& R){
    if (!R)
    {
        return L;
    }
    if (!L)
    {
        return R;
    }
    Node* node;
    if (R->Priority < L->Priority) {
        node = L;
        node->Left = L->Left;
        node->Right = merge(L->Right, R);
    }
    else {
        node = R;
        node->Right = R->Right;
        node->Left = merge(L, R->Left);
    }
    fixSize(node);
    return node;
}

void ImplicitTreap::InsertAt(int position, const std::string& value)
{
    int priority = std::rand();
    if (!root) {
        root = new Node(priority, value);
        return;
    }
    Node* L = new Node;
    Node* R = new Node;
    Node* newNode = new Node(priority, value);
    split(root, position, L, R);
    Node* tmp = merge(L, newNode);
    root = merge(tmp, R);
    fixSize(root);
}

void ImplicitTreap::DeleteAt(int position, int second_position) {
    for (int i = position; i <= second_position; ++i) {
        Node* L = new Node;
        Node* R = new Node;
        split(root, i, L, R);
        Node* delNode = new Node;
        split(R, 1, delNode, R);
        delete delNode;
        root = merge(L, R);
    }
}

std::string ImplicitTreap::GetAt(int position) {
    Node* L = new Node;
    Node* R = new Node;
    split(root, position, L, R);
    Node* getNode = new Node;
    split(R, 1, getNode, R);
    std::string ans = getNode->Value;
    R = merge(getNode, R);
    root = merge(L, R);
    return ans;
}

int main() {
    int K;
    std::cin >> K;
    ImplicitTreap treap;
    std::vector<std::string> ans;
    for (int i = 0; i < K; ++i) {
        std::string operation;
        int position;
        std::cin >> operation >> position;
        if (operation == "+") {
            std::string word;
            std::cin >> word;
            treap.InsertAt(position, word);
        }
        else if (operation == "-") {
            int second_position;
            std::cin >> second_position;
            treap.DeleteAt(position, second_position);
        }
        else if (operation == "?") {
            ans.push_back(treap.GetAt(position));
        }
    }
    for (auto elem : ans) {
        std::cout << elem << std::endl;
    }
    return 0;
}
