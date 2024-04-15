#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <queue>
#include <utility>

template <typename T>
struct Node {
    T key;
    int rank;
    Node* left;
    Node* right;

    Node(T key) : key(key), rank(1), left(nullptr), right(nullptr) {}
};

template <typename T>
class PriorityQueue {
private:
    Node<T>* root;
    int size_;

    Node<T>* merge(Node<T>* a, Node<T>* b) {
        if (!a) return b;
        if (!b) return a;

        if (a->key > b->key)
            std::swap(a, b);

        a->right = merge(a->right, b);

        if (!a->left || a->left->rank < a->right->rank)
            std::swap(a->left, a->right);

        a->rank = (a->right ? a->right->rank + 1 : 1);
        return a;
    }

public:
    PriorityQueue() : root(nullptr), size_(0) {}

    void push(const T& key) {
        Node<T>* new_node = new Node<T>(key);
        root = merge(root, new_node);
        size_++;
    }

    void pop() {
        if (!root) return;

        Node<T>* left_subtree = root->left;
        Node<T>* right_subtree = root->right;

        delete root;
        root = merge(left_subtree, right_subtree);
        size_--;
    }

    T extract_min() {
        if (!root) throw std::logic_error("Priority queue is empty");

        T min_key = root->key;
        pop();
        return min_key;
    }

    int size() const {
        return size_;
    }

    bool empty() const {
        return size_ == 0;
    }

    T top() const {
        if (!root) throw std::logic_error("Priority queue is empty");
        return root->key;
    }

    ~PriorityQueue() {
        while (root) {
            pop();
        }
    }
};

std::vector<int> calHeuristic(std::vector<std::pair<int, int>> coordinates, int end) {
    std::vector<int> ans(coordinates.size());
    std::pair<int, int> e = coordinates[end];
    for (int i = 0; i < coordinates.size(); i++) {
        ans[i] = sqrt(pow(coordinates[i].first - e.first, 2) + pow(coordinates[i].second - e.second, 2));
    }
    return ans;
}

std::vector<std::pair<int,int>> astar(std::vector<int> &h, std::vector<std::vector<int>> &graph, int start, int end) {
    std::vector<int> visited(graph.size(), 0);
    std::vector<std::pair<int,int>> path;
    std::vector<int> f(graph.size());
    std::vector<int> weight(graph.size(), std::numeric_limits<int>::max());
    std::vector<int> parent(graph.size(),-1);
    // Set the starting node weight to 0
    weight[start] = 0;
    parent[start] = -1;

    f[start] = weight[start] + h[start];

    // Priority queue to store nodes with the lowest f value
    PriorityQueue<std::pair<int, int>> pq;
    // Push the starting node to the priority queue
    pq.push({f[start], start});

    while (!pq.empty()) {
        auto k = pq.top();
        int current_f_weight = k.first;
        int current_node = k.second;

        pq.pop();

        // Terminate if the current node is the end node
        if (current_node == end) {
            path.push_back({parent[end], end});
            break;
        }

        // Skip visited nodes
        if (visited[current_node] == 1) {
            continue;
        }

        // Mark the current node as visited
        visited[current_node] = 1;
        path.push_back({parent[current_node], current_node});

        // Explore neighbors of the current node
        for (int i = 0; i < graph.size(); i++) {
            // Update neighbor nodes if a shorter path is found
            if (visited[i] == 0 && graph[current_node][i] != -1 &&
                weight[current_node] + graph[current_node][i] < weight[i]) {

                weight[i] = graph[current_node][i] + weight[current_node];
                f[i] = weight[i] + h[i];
                parent[i] = current_node;

                // Push the updated neighbor node to the priority queue
                pq.push({f[i], i});
            }
        }
    }

    return path;
}

int main() {
    int n;
    std::cout << "Enter number of nodes: ";
    std::cin >> n;
    std::vector<std::pair<int,int>> coordinates(n);
    std::cout << "Enter the coordinates (x,y): \n";
    for(int i = 0; i < n; i++) {
        int x, y;
        std::cin >> x >> y;
        coordinates[i] = std::make_pair(x, y);
    }
    int start, end;
    std::cout << "Enter start & end node: ";
    std::cin >> start >> end;
    std::vector<int> h;
    h = calHeuristic(coordinates, end);

    std::vector<std::vector<int>> graph(n, std::vector<int>(n));
    std::cout << "Enter the graph adjacency matrix:\n";
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            int x;
            std::cin >> x;
            graph[i][j] = x;
        }
    }
    
    std::vector<std::pair<int,int>> path = astar(h, graph, start, end);

    std::cout << "The starting node is " << start << " and the end node is " << end << std::endl;
    std::cout << "The path followed is:\n";
    for(auto i : path) {
        std::cout << i.first << " -> " << i.second << "   ";
    }
    std::cout << std::endl;

    if (path.empty() || path.back().second != end) {
        std::cout << "There does not exist a path from start to end.\n";
    } else {
        std::cout << "So the above is the path.\n";
    }

    return 0;
}
