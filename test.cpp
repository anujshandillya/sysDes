#include <iostream>
#include <vector>
#include <stack>
#include <unordered_set>
#include <unordered_map>
using namespace std;

class Graph {
private:
    int V; // number of vertices
    vector<vector<int>> adj; // adjacency list

public:
    Graph(int vertices) : V(vertices) {
        adj.resize(V);
    }

    // Add an edge to the graph
    void addEdge(int u, int v) {
        adj[u].push_back(v);
    }

    // Recursive DFS implementation
    void DFSRecursive(int start) {
        vector<bool> visited(V, false);
        cout << "Recursive DFS starting from vertex " << start << ": ";
        DFSRecursiveUtil(start, visited);
        cout << endl;
    }

    // Recursive DFS utility function
    void DFSRecursiveUtil(int vertex, vector<bool>& visited) {
        visited[vertex] = true;
        cout << vertex << " ";

        // Visit all adjacent vertices
        for (int neighbor : adj[vertex]) {
            if (!visited[neighbor]) {
                DFSRecursiveUtil(neighbor, visited);
            }
        }
    }

    // Iterative DFS implementation using stack
    void DFSIterative(int start) {
        vector<bool> visited(V, false);
        stack<int> s;
        
        cout << "Iterative DFS starting from vertex " << start << ": ";
        
        s.push(start);
        visited[start] = true;
        
        while (!s.empty()) {
            int vertex = s.top();
            s.pop();
            cout << vertex << " ";
            
            // Push all unvisited neighbors to stack
            // Note: We push in reverse order to maintain DFS order
            for (int i = adj[vertex].size() - 1; i >= 0; i--) {
                int neighbor = adj[vertex][i];
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    s.push(neighbor);
                }
            }
        }
        cout << endl;
    }

    // DFS to find connected components
    void findConnectedComponents() {
        vector<bool> visited(V, false);
        int componentCount = 0;
        
        cout << "Connected Components:" << endl;
        
        for (int i = 0; i < V; i++) {
            if (!visited[i]) {
                componentCount++;
                cout << "Component " << componentCount << ": ";
                DFSRecursiveUtil(i, visited);
                cout << endl;
            }
        }
        cout << "Total components: " << componentCount << endl;
    }

    // DFS to detect cycle in directed graph
    bool hasCycle() {
        vector<bool> visited(V, false);
        vector<bool> recStack(V, false);
        
        for (int i = 0; i < V; i++) {
            if (!visited[i]) {
                if (hasCycleUtil(i, visited, recStack)) {
                    return true;
                }
            }
        }
        return false;
    }

    // Utility function for cycle detection
    bool hasCycleUtil(int vertex, vector<bool>& visited, vector<bool>& recStack) {
        visited[vertex] = true;
        recStack[vertex] = true;
        
        for (int neighbor : adj[vertex]) {
            if (!visited[neighbor]) {
                if (hasCycleUtil(neighbor, visited, recStack)) {
                    return true;
                }
            } else if (recStack[neighbor]) {
                return true; // Back edge found
            }
        }
        
        recStack[vertex] = false;
        return false;
    }

    // Print the graph
    void printGraph() {
        cout << "Graph representation (adjacency list):" << endl;
        for (int i = 0; i < V; i++) {
            cout << i << " -> ";
            for (int neighbor : adj[i]) {
                cout << neighbor << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
};

int main() {
    // Create a graph with 6 vertices
    Graph g(6);
    
    // Add edges to create a sample graph
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 3);
    g.addEdge(2, 4);
    g.addEdge(3, 4);
    g.addEdge(3, 5);
    g.addEdge(4, 5);
    g.addEdge(5, 0); // Adding a back edge to create a cycle
    
    // Print the graph
    g.printGraph();
    
    // Test recursive DFS
    g.DFSRecursive(0);
    
    // Test iterative DFS
    g.DFSIterative(0);
    
    // Test DFS from different starting points
    cout << "\nDFS from different starting vertices:" << endl;
    g.DFSRecursive(2);
    g.DFSRecursive(4);
    
    // Test connected components (create a disconnected graph)
    cout << "\n--- Testing Connected Components ---" << endl;
    Graph g2(8);
    g2.addEdge(0, 1);
    g2.addEdge(1, 2);
    g2.addEdge(2, 0);
    g2.addEdge(3, 4);
    g2.addEdge(4, 5);
    g2.addEdge(6, 7);
    
    g2.printGraph();
    g2.findConnectedComponents();
    
    // Test cycle detection
    cout << "\n--- Testing Cycle Detection ---" << endl;
    Graph g3(4);
    g3.addEdge(0, 1);
    g3.addEdge(1, 2);
    g3.addEdge(2, 3);
    g3.addEdge(3, 1); // This creates a cycle
    
    g3.printGraph();
    if (g3.hasCycle()) {
        cout << "Graph contains a cycle!" << endl;
    } else {
        cout << "Graph does not contain a cycle." << endl;
    }
    
    // Test acyclic graph
    Graph g4(4);
    g4.addEdge(0, 1);
    g4.addEdge(1, 2);
    g4.addEdge(2, 3);
    
    g4.printGraph();
    if (g4.hasCycle()) {
        cout << "Graph contains a cycle!" << endl;
    } else {
        cout << "Graph does not contain a cycle." << endl;
    }
    
    return 0;
}
