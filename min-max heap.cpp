#include <iostream>
#include <vector>
#include <cmath>
#include <climits>
#include <utility>
using namespace std;

class MinMaxHeap {
private:
    vector<pair<int, int>> heap;

    void trickleUp(int index) {
        int parent = (index - 1) / 2;
        if (index > 0 && heap[index] < heap[parent]) {
            swap(heap[index], heap[parent]);
            trickleUp(parent);
        } else {
            trickleDown(index); // Changed to trickleDown
        }
    }

    void trickleDown(int index) {
        if (index >= heap.size()) return;
        int minChild = getMinChild(index);
        if (minChild != -1 && heap[minChild] < heap[index]) {
            swap(heap[minChild], heap[index]);
            trickleDown(minChild);
        }
    }

    int getMinChild(int index) {
        int leftChild = 2 * index + 1;
        int rightChild = 2 * index + 2;
        if (leftChild >= heap.size()) return -1;
        if (rightChild >= heap.size()) return leftChild;
        return heap[leftChild] < heap[rightChild] ? leftChild : rightChild;
    }

public:
    void push(pair<int, int> key) {
        heap.push_back(key);
        trickleUp(heap.size() - 1);
    }

    void pop() {
        if (heap.empty()) return;
        heap[0] = heap.back();
        heap.pop_back();
        trickleDown(0);
    }

    pair<int, int> extract_min() {
        if (heap.empty()) return make_pair(INT_MAX, INT_MAX);
        pair<int, int> minKey = heap[0];
        pop();
        return minKey;
    }

    int size() const {
        return heap.size();
    }

    bool empty() const {
        return heap.empty();
    }

    pair<int, int> top() const {
        if (heap.empty()) return make_pair(INT_MAX, INT_MAX);
        return heap[0];
    }
};

vector<int> calHeuristic(vector<pair<int, int>> coordinates, int end) {
    vector<int> ans(coordinates.size());
    pair<int, int> e = coordinates[end];
    for (int i = 0; i < coordinates.size(); i++) {
        ans[i] = sqrt(pow(coordinates[i].first - e.first, 2) + pow(coordinates[i].second - e.second, 2));
    }
    return ans;
}

vector<pair<int, int>> astar(vector<int> &h, vector<vector<int>> &graph, int start, int end) {
    vector<int> visited(graph.size(), 0);
    vector<pair<int, int>> path;
    vector<int> f(graph.size());
    vector<int> weight(graph.size(), INT_MAX);
    vector<int> parent(graph.size(), -1);
    weight[start] = 0;
    parent[start] = -1;
    f[start] = weight[start] + h[start];
    MinMaxHeap pq;
    pq.push({f[start], start});

    while (!pq.empty()) {
        auto k = pq.top();
        int current_f_weight = k.first;
        int current_node = k.second;

        pq.pop();

        if (current_node == end) {
            path.push_back({parent[end], end}); // Changed to push end directly
            break;
        }

        if (visited[current_node] == 1) {
            continue;
        }

        visited[current_node] = 1;
        path.push_back({parent[current_node], current_node});

        for (int i = 0; i < graph.size(); i++) {
            if (visited[i] == 0 && graph[current_node][i] != -1 &&
                weight[current_node] + graph[current_node][i] < weight[i]) {

                weight[i] = graph[current_node][i] + weight[current_node];
                f[i] = weight[i] + h[i];
                parent[i] = current_node;

                pq.push({f[i], i});
            }
        }
    }

    return path;
}

int main() {
    int n;
    cout << "Enter number of nodes: ";
    cin >> n;
    vector<pair<int, int>> coordinates(n);
    cout << "Enter the coordinates (x,y): \n";
    for (int i = 0; i < n; i++) {
        int x, y;
        cin >> x >> y;
        coordinates[i] = make_pair(x, y);
    }
    int start, end;
    cout << "Enter start & end node: ";
    cin >> start >> end;
    vector<int> h;
    h = calHeuristic(coordinates, end);

    vector<vector<int>> graph(n, vector<int>(n));
    cout << "Enter the graph adjacency matrix:\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int x;
            cin >> x;
            graph[i][j] = x;
        }
    }

    vector<pair<int, int>> path = astar(h, graph, start, end);

    cout << "The starting node is " << start << " and the end node is " << end << endl;

    cout << "The path followed is:\n";
    for (auto i : path) {
        cout << i.first << " -> " << i.second << "   ";
    }
    cout << endl;

    if (path.empty() || path.back().second != end) {
        cout << "There does not exist a path from start to end.\n";
    } else {
        cout << "So, the above is the path.\n";
    }

    return 0;
}
