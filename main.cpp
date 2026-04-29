#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include <string>
#include <chrono>

using namespace std;
using namespace chrono;

struct City {
    string name;
    double latitude;
    double longitude;
    int population;
};

struct Edge {
    int to;
    int distance;
    int roadType;
};

class Graph {
private:
    int n;
    vector<vector<Edge>> adj;

public:
    Graph(int n) : n(n), adj(n) {}

    void addEdge(int u, int v, int distance, int roadType) {
        adj[u].push_back({v, distance, roadType});
        adj[v].push_back({u, distance, roadType});
    }

    void dfsUtil(int current, vector<bool>& visited, const vector<City>& cities) {
        visited[current] = true;
        cout << cities[current].name << " ";

        for (const Edge& edge : adj[current]) {
            if (!visited[edge.to]) {
                dfsUtil(edge.to, visited, cities);
            }
        }
    }

    void dfs(int start, const vector<City>& cities) {
        vector<bool> visited(n, false);
        dfsUtil(start, visited, cities);
        cout << endl;
    }

    void bfsPath(int start, int finish, const vector<City>& cities) {
        vector<bool> visited(n, false);
        vector<int> parent(n, -1);
        queue<int> q;

        visited[start] = true;
        q.push(start);

        while (!q.empty()) {
            int current = q.front();
            q.pop();

            if (current == finish) break;

            for (const Edge& edge : adj[current]) {
                if (!visited[edge.to]) {
                    visited[edge.to] = true;
                    parent[edge.to] = current;
                    q.push(edge.to);
                }
            }
        }

        if (!visited[finish]) {
            cout << "Path not found.\n";
            return;
        }

        vector<int> path;
        for (int v = finish; v != -1; v = parent[v]) {
            path.push_back(v);
        }

        reverse(path.begin(), path.end());

        cout << "BFS path: ";
        for (int city : path) {
            cout << cities[city].name << " ";
        }
        cout << endl;
    }

    void dijkstra(int start, int finish, bool useDistance, const vector<City>& cities) {
        vector<int> dist(n, numeric_limits<int>::max());
        vector<int> parent(n, -1);

        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

        dist[start] = 0;
        pq.push({0, start});

        while (!pq.empty()) {
            int currentDist = pq.top().first;
            int current = pq.top().second;
            pq.pop();

            if (currentDist > dist[current]) continue;

            for (const Edge& edge : adj[current]) {
                int weight = useDistance ? edge.distance : edge.roadType;

                if (dist[current] + weight < dist[edge.to]) {
                    dist[edge.to] = dist[current] + weight;
                    parent[edge.to] = current;
                    pq.push({dist[edge.to], edge.to});
                }
            }
        }

        if (dist[finish] == numeric_limits<int>::max()) {
            cout << "Path not found.\n";
            return;
        }

        vector<int> path;
        for (int v = finish; v != -1; v = parent[v]) {
            path.push_back(v);
        }

        reverse(path.begin(), path.end());

        cout << "Total weight: " << dist[finish] << endl;
        cout << "Path: ";
        for (int city : path) {
            cout << cities[city].name << " ";
        }
        cout << endl;
    }
};

void sortByLongitude(vector<City> cities) {
    sort(cities.begin(), cities.end(), [](const City& a, const City& b) {
        return a.longitude < b.longitude;
    });

    cout << "\nCities sorted from west to east:\n";
    for (const City& city : cities) {
        cout << city.name << " | longitude: " << city.longitude << endl;
    }
}

void sortByPopulation(vector<City> cities) {
    sort(cities.begin(), cities.end(), [](const City& a, const City& b) {
        return a.population > b.population;
    });

    cout << "\nCities sorted by population descending:\n";
    for (const City& city : cities) {
        cout << city.name << " | population: " << city.population << endl;
    }
}

template <typename Func>
void measureTime(const string& title, Func func) {
    auto start = high_resolution_clock::now();
    func();
    auto finish = high_resolution_clock::now();

    auto duration = duration_cast<nanoseconds>(finish - start).count();
    cout << "Execution time: " << duration << " ns\n";
    cout << "----------------------------------------\n";
}

int main() {
    vector<City> cities = {
        {"Vinnytsia", 49.2331, 28.4682, 370000},
        {"Kyiv", 50.4501, 30.5234, 2950000},
        {"Uman", 48.7484, 30.2218, 82000},
        {"Zhytomyr", 50.2547, 28.6587, 260000},
        {"Berdychiv", 49.8993, 28.6024, 73000},
        {"Koziatyn", 49.7143, 28.8339, 23000},
        {"Kalynivka", 49.4539, 28.5261, 18000},
        {"Nemyriv", 48.9708, 28.8378, 12000},
        {"Haisyn", 48.8114, 29.3898, 25000},
        {"Ladyzhyn", 48.6849, 29.2368, 22000},
        {"Tulchyn", 48.6745, 28.8464, 15000},
        {"Khrystynivka", 48.8153, 29.9686, 10000},
        {"Bila Tserkva", 49.7989, 30.1153, 207000},
        {"Fastiv", 50.0787, 29.9177, 45000},
        {"Vasylkiv", 50.1779, 30.3192, 37000},
        {"Obukhiv", 50.1069, 30.6185, 33000},
        {"Skvyra", 49.7318, 29.6657, 15000},
        {"Pohrebyshche", 49.4864, 29.2636, 9500},
        {"Khmilnyk", 49.5598, 27.9575, 27000},
        {"Illintsi", 49.1044, 29.2174, 11000}
    };

    Graph g(20);

    g.addEdge(0, 6, 25, 2);
    g.addEdge(0, 7, 45, 2);
    g.addEdge(0, 5, 65, 2);
    g.addEdge(6, 5, 30, 2);
    g.addEdge(5, 4, 40, 2);
    g.addEdge(4, 3, 45, 2);
    g.addEdge(3, 1, 140, 1);
    g.addEdge(5, 17, 35, 2);
    g.addEdge(17, 19, 30, 2);
    g.addEdge(19, 8, 35, 2);
    g.addEdge(8, 2, 50, 2);
    g.addEdge(2, 11, 20, 3);
    g.addEdge(8, 9, 15, 3);
    g.addEdge(9, 10, 20, 3);
    g.addEdge(10, 7, 50, 2);
    g.addEdge(2, 12, 145, 1);
    g.addEdge(12, 1, 85, 1);
    g.addEdge(12, 13, 40, 2);
    g.addEdge(13, 1, 70, 2);
    g.addEdge(1, 14, 30, 1);
    g.addEdge(1, 15, 45, 1);
    g.addEdge(14, 13, 35, 2);
    g.addEdge(15, 12, 50, 2);
    g.addEdge(16, 12, 60, 2);
    g.addEdge(16, 2, 70, 2);
    g.addEdge(18, 0, 70, 2);
    g.addEdge(18, 17, 40, 2);
    g.addEdge(4, 16, 80, 2);
    g.addEdge(3, 13, 120, 2);
    g.addEdge(11, 8, 25, 3);
    g.addEdge(7, 8, 60, 2);

    int start = 0;   // Vinnytsia
    int finish = 1;  // Kyiv

    measureTime("DFS traversal", [&]() {
        cout << "DFS traversal from Vinnytsia:\n";
        g.dfs(start, cities);
    });

    measureTime("BFS path", [&]() {
        g.bfsPath(start, finish, cities);
    });

    measureTime("Dijkstra by distance", [&]() {
        cout << "Dijkstra by distance:\n";
        g.dijkstra(start, finish, true, cities);
    });

    measureTime("Dijkstra by road type", [&]() {
        cout << "Dijkstra by road type:\n";
        g.dijkstra(start, finish, false, cities);
    });

    measureTime("Sort by longitude", [&]() {
        sortByLongitude(cities);
    });

    measureTime("Sort by population", [&]() {
        sortByPopulation(cities);
    });

    return 0;
}
