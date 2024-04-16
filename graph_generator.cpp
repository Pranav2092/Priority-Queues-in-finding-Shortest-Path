#include <bits/stdc++.h>
using namespace std;
#define limit 1000


void generator(vector<vector<int>>& graph,vector<pair<int,int>>& cord){
        int n=graph.size();
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                if(i==j){
                    graph[i][j]=-1;
                    continue;
                }
                
                if(graph[i][j]==-1){
                        int dist=sqrt(pow(abs(cord[i].first - cord[j].first),2)+pow(abs(cord[i].second - cord[j].second),2));
                        graph[i][j]=dist;
                        graph[j][i]=dist;
                }
                else{
                    continue;
                }
            }
        }
        return ;
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
    
    for(int i = 0; i < n; i++) {
        int count = fun(graph, i);
       if(count>=(n-1)/2){
            continue;
       }
       
        

        else{
            count=((n-1)/2)-count;
            unordered_set<int> st;
            while(count > 0) {
            int index = rand() % n;
            if(index != i && st.find(index) == st.end()) {
                
                st.insert(index);
                
                graph[i][index] = -1;
                graph[index][i] = -1; // Set the opposite edge to -1
                count--;
            }
        }}
    }

    for(int i = 0; i < n; i++) {
        int count = fun(graph, i);
        if(count == n - 1) {
            int index = rand() % n;
            if(index != i) {
                graph[i][index] = copy[i][index];
                graph[index][i] = copy[i][index];
            }
            else{
                graph[index-1][i]=copy[i][index-1];
                graph[index-1][i] = copy[i][index-1];
                
            }
        }
    }

    return graph;
}

int main(){
    srand(time(0));
   //we will generate a symmetrix matrix only 
    int n;
    cout<<"how many nodes needed :"<<endl;
    cin>>n;
    vector<pair<int,int>> coordinate;
    
   //will ensure 2 nodes don't get the same  coordinates
    set<pair<int,int>> st;
    for(int i = 0; i < n; i++) {
    int x = rand() % limit;
    int y = rand() % limit;
    if(st.find({x,y}) == st.end()) {
        st.insert({x,y});
        coordinate.push_back({x,y});
    } else {
        i--; // Skip the duplicate coordinate
    }
}
    //generating a complete graph
    vector<vector<int>> graph(n,vector<int>(n,-1));

    generator(graph,coordinate);

    //now deleting randomly n/2 edges for each node randomly so that always there is a connected graph which I get
    //but during deleting ensuring that always atleast there is 1 edge present and graph remain connected
    vector<vector<int>> g=updating(graph);

    cout<<"the resultant adjeceny matrix looks like"<<endl;
    for(int i=0;i<g.size();i++){
        for(int j=0;j<g.size();j++){
            cout<<g[i][j]<<" ";
        }
        cout<<endl;
    }

    cout<<"the coordinates of nodes are "<<endl;
    for(int i=0;i<coordinate.size();i++){
        cout<<i<<" "<<coordinate[i].first<<" "<<coordinate[i].second<<endl;
    }

    return 0;
}