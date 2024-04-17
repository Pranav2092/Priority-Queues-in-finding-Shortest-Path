#include <bits/stdc++.h>
#include <chrono>
#include <utility>
using namespace std;
using namespace std::chrono;
#define limit 1000

// implementation of priority queue using leftist heap
template <typename T> struct Node {
    T key;
    int node_rank; // Renamed from rank to avoid conflict with std::rank
    Node* left;
    Node* right;

    Node(T key) : key(key), node_rank(1), left(nullptr), right(nullptr) {}
};

template <typename T> class PriorityQueue {
private:
    Node<T>* root;
    int size_;

    Node<T>* merge(Node<T>* a, Node<T>* b) {
        if (!a)
            return b;
        if (!b)
            return a;

        if (a->key > b->key)
            std::swap(a, b);

        a->right = merge(a->right, b);

        if (!a->left || a->left->node_rank <
                            a->right->node_rank) // Updated rank to node_rank
            std::swap(a->left, a->right);

        a->node_rank = (a->right ? a->right->node_rank + 1
                                 : 1); // Updated rank to node_rank
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
        if (!root)
            return;

        Node<T>* left_subtree = root->left;
        Node<T>* right_subtree = root->right;

        delete root;
        root = merge(left_subtree, right_subtree);
        size_--;
    }

    T extract_min() {
        if (!root)
            throw std::logic_error("Priority queue is empty");

        T min_key = root->key;
        pop();
        return min_key;
    }

    int size() const { return size_; }

    bool empty() const { return size_ == 0; }

    T top() const {
        if (!root)
            throw std::logic_error("Priority queue is empty");
        return root->key;
    }

    ~PriorityQueue() {
        while (root) {
            pop();
        }
    }
};

// implementation of priority queue using binomial heap
//  Node structure for Binomial Heap
struct BinomialNode {
    pair<int, int> key; // Pair of integers as the key
    int degree;         // Degree of the node
    BinomialNode* parent;
    BinomialNode* child;
    BinomialNode* sibling;
};

class BinomialHeap {
private:
    BinomialNode* head; // Pointer to the head of the binomial heap
    int heap_size;      // Size of the binomial heap

    // Link two binomial trees of the same degree
    BinomialNode* linkTrees(BinomialNode* tree1, BinomialNode* tree2) {
        if (tree1->key > tree2->key)
            swap(tree1, tree2);

        tree2->parent = tree1;
        tree2->sibling = tree1->child;
        tree1->child = tree2;
        tree1->degree++;

        return tree1;
    }
    // Merge two binomial heaps into one
    BinomialNode* mergeHeaps(BinomialNode* heap1, BinomialNode* heap2) {
        BinomialNode* head = nullptr;
        BinomialNode* tail = nullptr;
        BinomialNode* carry = nullptr;

        while (heap1 || heap2 || carry) {
            BinomialNode* sumNode = carry;

            if (heap1 && heap2) {
                if (heap1->degree <= heap2->degree) {
                    sumNode = heap1;
                    heap1 = heap1->sibling;
                } else {
                    sumNode = heap2;
                    heap2 = heap2->sibling;
                }
            } else if (heap1) {
                sumNode = heap1;
                heap1 = heap1->sibling;
            } else if (heap2) {
                sumNode = heap2;
                heap2 = heap2->sibling;
            }

            if (carry) {
                sumNode = linkTrees(carry, sumNode);
                carry = nullptr;
            }

            if (!head) {
                head = sumNode;
                tail = sumNode;
            } else {
                tail->sibling = sumNode;
                tail = sumNode;
            }

            if (sumNode->degree >= 2) {
                carry = sumNode;
                carry->sibling = nullptr;
            }
        }

        return head;
    }

    // Reverse the sibling pointers of a binomial tree
    BinomialNode* reverseTree(BinomialNode* root) {
        BinomialNode* prev = nullptr;
        BinomialNode* current = root;
        BinomialNode* next = nullptr;

        while (current) {
            next = current->sibling;
            current->sibling = prev;
            prev = current;
            current = next;
        }

        return prev;
    }

public:
    BinomialHeap() : head(nullptr), heap_size(0) {}

    // Insert a new element into the binomial heap
    void push(pair<int, int> key) {
        BinomialNode* newNode = new BinomialNode;
        newNode->key = key;
        newNode->degree = 0;
        newNode->parent = nullptr;
        newNode->child = nullptr;
        newNode->sibling = nullptr;

        head = mergeHeaps(head, newNode);
        heap_size++;
    }

    // Extracts the minimum element from the binomial heap
    pair<int, int> extract_min() {
        if (!head)
            throw runtime_error("Heap is empty.");

        BinomialNode* minNode = head;
        BinomialNode* prevMin = nullptr;
        BinomialNode* current = head->sibling;
        BinomialNode* prev = head;

        while (current) {
            if (current->key < minNode->key) {
                minNode = current;
                prevMin = prev;
            }
            prev = current;
            current = current->sibling;
        }

        if (prevMin)
            prevMin->sibling = minNode->sibling;
        else
            head = minNode->sibling;

        minNode->child = reverseTree(
            minNode->child); // Reverse the child list for consolidation
        BinomialNode* extractedNode = minNode;
        pair<int, int> minValue = extractedNode->key;

        delete extractedNode;

        heap_size--;
        head = mergeHeaps(head,
                          minNode->child); // Merge the child list with the heap
        return minValue;
    }

    // Removes the minimum element from the binomial heap
    void pop() { extract_min(); }

    // Returns the minimum element from the binomial heap without removing it
    pair<int, int> top() const {
        if (!head)
            throw runtime_error("Heap is empty.");

        BinomialNode* minNode = head;
        BinomialNode* current = head->sibling;

        while (current) {
            if (current->key < minNode->key) {
                minNode = current;
            }
            current = current->sibling;
        }

        return minNode->key;
    }

    // Returns the size of the binomial heap
    int size() const { return heap_size; }

    // Checks if the binomial heap is empty
    bool empty() const { return heap_size == 0; }

    // Destructor to free memory
    ~BinomialHeap() {
        while (head) {
            BinomialNode* temp = head;
            head = head->sibling;
            delete temp;
        }
    }
};

// implementation of priority queue using min-max heap
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
        if (index >= heap.size())
            return;
        int minChild = getMinChild(index);
        if (minChild != -1 && heap[minChild] < heap[index]) {
            swap(heap[minChild], heap[index]);
            trickleDown(minChild);
        }
    }

    int getMinChild(int index) {
        int leftChild = 2 * index + 1;
        int rightChild = 2 * index + 2;
        if (leftChild >= heap.size())
            return -1;
        if (rightChild >= heap.size())
            return leftChild;
        return heap[leftChild] < heap[rightChild] ? leftChild : rightChild;
    }

public:
    void push(pair<int, int> key) {
        heap.push_back(key);
        trickleUp(heap.size() - 1);
    }

    void pop() {
        if (heap.empty())
            return;
        heap[0] = heap.back();
        heap.pop_back();
        trickleDown(0);
    }

    pair<int, int> extract_min() {
        if (heap.empty())
            return make_pair(INT_MAX, INT_MAX);
        pair<int, int> minKey = heap[0];
        pop();
        return minKey;
    }

    int size() const { return heap.size(); }

    bool empty() const { return heap.empty(); }

    pair<int, int> top() const {
        if (heap.empty())
            return make_pair(INT_MAX, INT_MAX);
        return heap[0];
    }
};

// implementation of graph generator function
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

// function to calculate heuristic
vector<int> calHeuristic(vector<pair<int, int>> coordinates, int end) {
    vector<int> ans(coordinates.size());
    pair<int, int> e = coordinates[end];
    for (int i = 0; i < coordinates.size(); i++) {
        ans[i] = sqrt(pow(coordinates[i].first - e.first, 2) +
                      pow(coordinates[i].second - e.second, 2));
    }
    return ans;
}

// Implementation of A* algorithm using leftist heap
vector<pair<int, int>> astar1(vector<int>& h, vector<vector<int>>& graph,
                              int start, int end,int w) {
    vector<int> visited(graph.size(), 0);
    vector<pair<int, int>> path;
    vector<int> f(graph.size());
    vector<int> weight(graph.size(), INT_MAX);
    vector<int> parent(graph.size(), -1);
    // Set the starting node weight to 0
    weight[start] = 0;
    parent[start] = -1;

    f[start] = weight[start] + w*h[start];

    // Priority queue to store nodes with the lowest f value
    PriorityQueue<pair<int, int>> pq;

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
                f[i] = weight[i] + w*h[i];
                parent[i] = current_node;

                // Push the updated neighbor node to the priority queue
                pq.push({f[i], i});
            }
        }
    }

    return path;
}

// Implementation of A* algorithm using binomial heap
vector<pair<int, int>> astar2(vector<int>& h, vector<vector<int>>& graph,
                              int start, int end,int w) {
    vector<int> visited(graph.size(), 0);
    vector<pair<int, int>> path;
    vector<int> f(graph.size());
    vector<int> weight(graph.size(), INT_MAX);
    vector<int> parent(graph.size(), -1);
    // Set the starting node weight to 0
    weight[start] = 0;
    parent[start] = -1;

    f[start] = weight[start] + w*h[start];

    // Priority queue to store nodes with the lowest f value
    BinomialHeap pq;

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
                f[i] = weight[i] + w*h[i];
                parent[i] = current_node;

                // Push the updated neighbor node to the priority queue
                pq.push({f[i], i});
            }
        }
    }

    return path;
}

// Implementation of A* algorithm using min-max heap
vector<pair<int, int>> astar3(vector<int>& h, vector<vector<int>>& graph,
                              int start, int end,int w) {
    vector<int> visited(graph.size(), 0);
    vector<pair<int, int>> path;
    vector<int> f(graph.size());
    vector<int> weight(graph.size(), INT_MAX);
    vector<int> parent(graph.size(), -1);
    // Set the starting node weight to 0
    weight[start] = 0;
    parent[start] = -1;

    f[start] = weight[start] + w*h[start];

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
                f[i] = weight[i] +w* h[i];
                parent[i] = current_node;

                // Push the updated neighbor node to the priority queue
                pq.push({f[i], i});
            }
        }
    }

    return path;
}

// Driver function
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
    cout<<endl;
    cout<<"enter the weight factor of heuristic yoou want to take : ";
    int w;
    cin>>w;
    vector<int> h;
    // pre-processing of heuristics
    h = calHeuristic(coordinates, end);

    // Time for leftist heap implementation

    // Get starting timepoint
    auto sta1 = high_resolution_clock::now();

    vector<pair<int, int>> path1 = astar1(h, graph, start, end,w);

    // Get ending timepoint
    auto stop1 = high_resolution_clock::now();

    cout << " the starting node is   " << start << " the end node is  " << end
         << endl;

    cout << "the path followed is" << endl;
    for (auto i : path1) {
        cout << i.first << " ->" << i.second << "   ";
    }
    cout << endl;
    if (path1.empty() || path1.back().second != end) {
        cout << "there does not exist a path from start to end " << endl;
    } else {
        cout << "so the above is path " << endl;
    }

    auto duration1 = duration_cast<microseconds>(stop1 - sta1);

    cout << "Time taken by function using Leftist heap: " << duration1.count()
         << " microseconds" << endl;

    // Time for binomial heap implementation

    // Get starting timepoint
    auto sta2 = high_resolution_clock::now();

    vector<pair<int, int>> path2 = astar2(h, graph, start, end,w);

    // Get ending timepoint
    auto stop2 = high_resolution_clock::now();

    cout << " the starting node is   " << start << " the end node is  " << end
         << endl;

    cout << "the path followed is" << endl;
    for (auto i : path2) {
        cout << i.first << " ->" << i.second << "   ";
    }
    cout << endl;
    if (path2.empty() || path2.back().second != end) {
        cout << "there does not exist a path from start to end " << endl;
    } else {
        cout << "so the above is path " << endl;
    }

    auto duration2 = duration_cast<microseconds>(stop2 - sta2);

    cout << "Time taken by function using Binomial heap: " << duration2.count()
         << " microseconds" << endl;

    // Time for min-max heap implementation

    // Get starting timepoint
    auto sta3 = high_resolution_clock::now();

    vector<pair<int, int>> path3 = astar3(h, graph, start, end,w);

    // Get ending timepoint
    auto stop3 = high_resolution_clock::now();

    cout << " the starting node is   " << start << " the end node is  " << end
         << endl;

    cout << "the path followed is" << endl;
    for (auto i : path3) {
        cout << i.first << " ->" << i.second << "   ";
    }
    cout << endl;
    if (path3.empty() || path3.back().second != end) {
        cout << "there does not exist a path from start to end " << endl;
    } else {
        cout << "so the above is path " << endl;
    }

    auto duration3 = duration_cast<microseconds>(stop3 - sta3);

    cout << "Time taken by function using min-max heap: " << duration3.count()
         << " microseconds" << endl;

    return 0;
}