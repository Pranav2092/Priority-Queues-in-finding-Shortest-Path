#include <bits/stdc++.h>
using namespace std;

vector<int> calHeuristic(vector<pair<int, int>> coordinates, int end)
{
    vector<int> ans(coordinates.size());
    pair<int, int> e = coordinates[end];
    for (int i = 0; i < coordinates.size(); i++)
    {
        ans[i] = sqrt(pow(coordinates[i].first - e.first, 2) + pow(coordinates[i].second - e.second, 2));
    }
    return ans;
}

vector<pair<int,int>> astar(vector<int> &h, vector<vector<int>> &graph, int start, int end)
{
    vector<int> visited(graph.size(), 0);
    vector<pair<int,int>> path;
    vector<int> f(graph.size());
    vector<int> weight(graph.size(), INT_MAX);
    vector<int> parent(graph.size(),-1);
    // Set the starting node weight to 0
    weight[start] = 0;
    parent[start]=-1;

    f[start] = weight[start] + h[start];

    // Priority queue to store nodes with the lowest f value
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    // Push the starting node to the priority queue
    pq.push({f[start], start});

    while (!pq.empty())
    {
        auto k = pq.top();
        int current_f_weight = k.first;
        int current_node = k.second;

        pq.pop();

        // Terminate if the current node is the end node
        if (current_node == end)
        {
            path.push_back({parent[end],end});
            break;
        }

        // Skip visited nodes
        if (visited[current_node] == 1)
        {
            continue;
        }

        // Mark the current node as visited
        visited[current_node] = 1;
        path.push_back({parent[current_node],current_node});

        // Explore neighbors of the current node
        for (int i = 0; i < graph.size(); i++)
        {
            // Update neighbor nodes if a shorter path is found
            if (visited[i] == 0 && graph[current_node][i] != -1 &&
                weight[current_node] + graph[current_node][i] < weight[i])
            {

                weight[i] = graph[current_node][i] + weight[current_node];
                f[i] = weight[i] + h[i];
                parent[i]=current_node;

                // Push the updated neighbor node to the priority queue
                pq.push({f[i], i});
            }
        }
    }

    return path;
}


int main(){
    int n;
    cout<<"Enter number of nodes: ";
    cin>> n;
    vector<pair<int,int>> coordinates(n);
    cout<<"Enter the coordinates (x,y): \n";
    for(int i = 0;i<n;i++){
        int x,y;
        cin>>x>>y;
        coordinates[i] = make_pair(x,y);
    }
    int start,end;
    cout<<"Enter start & end node: ";
    cin>>start>>end;
    vector<int> h;
    h = calHeuristic(coordinates,end);
 


    vector<vector<int>> graph(n,vector<int>(n));
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            int x;
            cin>>x;
            graph[i][j]=x;
        }
    }
    
    vector<pair<int,int>> path=astar(h,graph,start,end);

    cout<<" the starting node is   "<<start<<" the end node is  "<<end<<endl;

    cout<<"the path followed is"<<endl;
    for(auto i:path){
        cout<<i.first<<" ->"<<i.second<<"   ";
    }
    cout<<endl;
    if(path[path.size()].second==end){
        cout<<"there does not exist a path from start to end "<<endl;
    }
    else{
        cout<<"so the above is path "<<endl;
    }

    return 0;
}
