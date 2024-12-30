#include<bits/stdc++.h>
using namespace std;

class DroneOperations {
public:
    virtual void execute () = 0;
    virtual ~DroneOperations () {}
};
class TakeOff : public DroneOperations {
public:
    void execute () override {
        cout << "Drone is taking off." << endl;
    }
};
class ComeBackToSource : public DroneOperations {
public:
    void execute () override {
        cout << "Drone getting ready to come back to source." << endl;
    }
};
class Land : public DroneOperations {
public:
    void execute () override {
        cout << "The drone is Landing." << endl;
    }
};
class Failure : public DroneOperations {
public:
    void execute () override {
        cout << "The drone system has encountered an error." << endl;
    }
};

class MissionPlanning {
public:
    // Time Complexity : O(E|logV|)
    vector<int>constructPath(map<int, map<int, int>>&graph, int start, int end, int V) {
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>>pq;
        vector<int>distance(V, INT_MAX);
        vector<int>parent(V, -1);
        distance[start] = 0;
        pq.push({0, start});
        
        while (!pq.empty()) {
            int currDist = pq.top().first;
            int node = pq.top().second;
            pq.pop();

            for (const auto& neighbor : graph[node]) {
                auto [nextNode, edgeWeight] = neighbor;
                int nextDist = currDist + edgeWeight;
                if (distance[nextNode] > nextDist) {
                    distance[nextNode] = currDist + edgeWeight;
                    parent[nextNode] = node;
                    pq.push({currDist + edgeWeight, nextNode});
                }
            }
        }
        
        vector<int> path;
        for (int curr = end; curr != -1; curr = parent[curr]) 
            path.push_back(curr);
        reverse(path.begin(), path.end());
        cout << "The distance between Node - "<< start << " and Node - " << end << " is: " << distance[end] << " units." << endl;
        return path;
    }
};

class Survey {
public:
    vector<int>path;
    Survey(vector<int>path){
        this->path = path;
        cout << "Drone is leaving the location." << endl;
    }
    void print(vector<int>arr) {
        for(int i = 0; i < arr.size(); i++) {
            cout << arr[i];
            if(i != arr.size() - 1)
                cout << "->" ;
        }
        cout << endl;
    }
    void FollowPath(){
        cout << "The drone is following the path : ";
        print(path);
        cout << "Drone has reached the Destination" << endl;
    }
};

void readGraphFromFile(string filename, map<int, map<int, int>> &graph) {
    ifstream inFile(filename);
    graph.clear();
    int u, v, weight;
    while (inFile >> u >> v >> weight) {
        graph[u][v] = weight;
        graph[v][u] = weight;
    }
    inFile.close();
}

int main() {
    const int V = 100;
    map<int, map<int, int>>graph;
    // source   dest, dist
    readGraphFromFile("graph.txt", graph);
    int source, destination;
    cout << "Input Source Location Integer: ";
    cin >> source;
    cout << "Input Destination Location Integer: ";
    cin >> destination;
    Failure failure;
    if (source < 1 || source > 100 || destination < 1 || destination > 100) {
        cerr << "Error: Positions must be within 1 - 100, inclusive." << endl;    
        failure.execute();
        return 1;
    }
    vector<int>path;
    TakeOff takeOffOperation;
    MissionPlanning getPath;
    thread t1([&takeOffOperation]() { takeOffOperation.execute(); });
    thread t2([&]() { path = getPath.constructPath(graph, source, destination, V); });
    t1.join();
    t2.join();

    // I am not 100% sure of what "survey" does, I am assuming it means the drone is going to follow the path provided to it.
    Survey s(path);
    s.FollowPath();
    thread t3([](vector<int>& p) { reverse(p.begin(), p.end()); }, ref(path));
    ComeBackToSource returnJourney;
    returnJourney.execute();
    t3.join();
    Survey s2(path);
    s2.FollowPath();
    Land landing;
    landing.execute();
    return 0;
}


