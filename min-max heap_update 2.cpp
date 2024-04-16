#include <bits/stdc++.h>
#include <chrono>
#include <utility>
using namespace std;
using namespace std::chrono;
#define limit 1000

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

void generator(vector<vector<int>>& graph, vector<pair<int, int>>& cord) {
    int n = graph.size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                graph[i][j] = -1;
                continue;
            }

            if (graph[i][j] == -1) {
                int dist = sqrt(pow(abs(cord[i].first - cord[j].first), 2) +
                                pow(abs(cord[i].second - cord[j].second), 2));
                graph[i][j] = dist;
                graph[j][i] = dist;
            } else {
                continue;
            }
        }
    }
    return;
}

int fun(vector<vector<int>>& graph, int index) {
    int n = graph.size();
    int count = 0;
    for (int i = 0; i < n; i++) {
        if (graph[index][i] == -1 && i != index) {
            count++;
        }
    }
    return count;
}

vector<vector<int>> updating(vector<vector<int>>& graph) {
    vector<vector<int>> copy = graph;
    int n = graph.size();

    for (int i = 0; i < n; i++) {
        int count = fun(graph, i);
        if (count >= (n - 1) / 2) {
            continue;
        }

        else {
            count = ((n - 1) / 2) - count;
            unordered_set<int> st;
            while (count > 0) {
                int index = rand() % n;
                if (index != i && st.find(index) == st.end()) {

                    st.insert(index);

                    graph[i][index] = -1;
                    graph[index][i] = -1; // Set the opposite edge to -1
                    count--;
                }
            }
        }
    }

    for (int i = 0; i < n; i++) {
        int count = fun(graph, i);
        if (count == n - 1) {
            int index = rand() % n;
            if (index != i) {
                graph[i][index] = copy[i][index];
                graph[index][i] = copy[i][index];
            } else {
                graph[index - 1][i] = copy[i][index - 1];
                graph[index - 1][i] = copy[i][index - 1];
            }
        }
    }

    return graph;
}

void graphGenerator(int n, vector<vector<int>>& graph,
                    vector<pair<int, int>>& coordinates) {
    srand(time(0));

    // we will generate a symmetrix matrix only
    //  to generate coordinates
    // will ensure 2 nodes don't get the same  coordinates
    set<pair<int, int>> st;
    for (int i = 0; i < n; i++) {
        int x = rand() % limit;
        int y = rand() % limit;
        if (st.find({x, y}) == st.end()) {
            st.insert({x, y});
            coordinates.push_back({x, y});
        } else {
            i--; // Skip the duplicate coordinate
        }
    }

    // generating a complete graph
    generator(graph, coordinates);

    // now deleting randomly n/2 edges for each node randomly so that always
    // there is a connected graph which I get but during deleting ensuring that
    // always atleast there is 1 edge present and graph remain connected
    vector<vector<int>> g = updating(graph);
}

vector<int> calHeuristic(vector<pair<int, int>> coordinates, int end) {
    vector<int> ans(coordinates.size());
    pair<int, int> e = coordinates[end];
    for (int i = 0; i < coordinates.size(); i++) {
        ans[i] = sqrt(pow(coordinates[i].first - e.first, 2) +
                      pow(coordinates[i].second - e.second, 2));
    }
    return ans;
}

vector<pair<int, int>> astar(vector<int>& h, vector<vector<int>>& graph,
                             int start, int end) {
    vector<int> visited(graph.size(), 0);
    vector<pair<int, int>> path;
    vector<int> f(graph.size());
    vector<int> weight(graph.size(), INT_MAX);
    vector<int> parent(graph.size(), -1);
    // Set the starting node weight to 0
    weight[start] = 0;
    parent[start] = -1;

    f[start] = weight[start] + h[start];

    // Priority queue to store nodes with the lowest f value
    MinMaxHeap pq;

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
    cout << "Enter number of nodes: ";
    cin >> n;
    vector<pair<int, int>> coordinates;
    vector<vector<int>> graph(n, vector<int>(n, -1));
    graphGenerator(n, graph, coordinates);

    // printing the generated graph

    cout << "The graph is \n";
    for (int i = 0; i < graph.size(); i++) {
        for (int j = 0; j < graph.size(); j++) {
            cout << graph[i][j] << " ";
        }
        cout << endl;
    }
    cout << "The coordinates of the nodes are \n";
    for (int i = 0; i < coordinates.size(); i++) {
        cout << i << " " << coordinates[i].first << " " << coordinates[i].second
             << endl;
    }

    int start, end;
    cout << "Enter start & end node: ";
    cin >> start >> end;
    vector<int> h;
    h = calHeuristic(coordinates, end);

    // Get starting timepoint
    auto sta = high_resolution_clock::now();

    vector<pair<int, int>> path = astar(h, graph, start, end);

    // Get ending timepoint
    auto stop = high_resolution_clock::now();

    cout << " the starting node is   " << start << " the end node is  " << end
         << endl;

    cout << "the path followed is" << endl;
    for (auto i : path) {
        cout << i.first << " ->" << i.second << "   ";
    }
    cout << endl;
    if (path[path.size()].second == end) {
        cout << "there does not exist a path from start to end " << endl;
    } else {
        cout << "so the above is path " << endl;
    }

    auto duration = duration_cast<microseconds>(stop - sta);

    cout << "Time taken by function: " << duration.count() << " microseconds"
         << endl;

    return 0;
}