#include <bits/stdc++.h>

using namespace std;

constexpr double INF = numeric_limits<double>::infinity();
constexpr const char INPUT_PATH[] = "input.txt";
constexpr const char OUTPUT_PATH[] = "output.txt";

// class Node {
//   private:
//     size_t id;
//     bool isVisited;

//   public:
//     void markVisited() {
//         isVisited = true;
//     }

//     void unmarkVisited() {
//         isVisited = false;
//     }

//     //----------------------- Constructors
//     Node() : id(0), isVisited(false) {}
//     Node(size_t i) : id(i), isVisited(false) {}

//     //----------------------- Getters
//     size_t getId() const {
//         return id;
//     }

//     bool visited() const {
//         return isVisited;
//     }

//     //----------------------- Setters
//     void setID(const size_t id) {
//         this->id = id;
//     }

//     void setVisited(bool visited) {
//         isVisited = visited;
//     }
// };

struct Tour {
    double cost;
    vector<size_t> path;

    Tour(double c = 0.0, const vector<size_t>& p = {}) : cost(c), path(p) {}

    void addNode(size_t id, double weight) {
        path.push_back(id);
        cost += weight;
    }

    void reset() {
        cost = 0.0;
        path.clear();
    }
};

class Graph {
   private:
    size_t size;
    size_t start = 0;
    size_t visitedCount = 0;
    // vector<Node> nodes;
    vector<bool> visited;
    vector<vector<double>> adjacencyMatrix;

    void markVisited(size_t id) {
        if (id < size) {
            if (visited[id] == false && visitedCount < size) {
                visitedCount++;
            }
            // nodes[id].markVisited();
            visited[id] = true;
        }
    }

    void unmarkVisited(size_t id) {
        if (id < size) {
            if (visited[id] == true && visitedCount > 0) {
                visitedCount--;
            }
            // nodes[id].unmarkVisited();
            visited[id] = false;
        }
    }

    void resetVisited() {
        visitedCount = 0;
        for (size_t i = 0; i < visited.size(); ++i) {
            // nodes[i].unmarkVisited();
            visited[i] = false;
        }
    }

    bool isUnvisitedNeighbour(size_t from, size_t to) const {
        if (from < size && to < size) {
            return adjacencyMatrix[from][to] < INF && visited[to] == false;
        }
        return false;
    }

   public:
    //----------------------- Constructors
    Graph(size_t size) : size(size), start(0), visitedCount(0) {
        // this->size = size;
        // this->start = 0;
        // this->visitedCount = 0;
        // nodes.resize(size);
        // for (size_t i = 0; i < size; ++i) {
        //     nodes[i].setID(i);
        // }
        adjacencyMatrix.resize(size);
        for (auto& v : adjacencyMatrix) {
            v.resize(size, INF);
        }
        visited.resize(size, false);
    }

    // Graph(const vector<Node> &nodes) {
    //     this->size = nodes.size();
    //     this->nodes = nodes;
    //     adjacencyMatrix.resize(size);
    //     for (auto &v : adjacencyMatrix) {
    //         v.resize(size, INF);
    //     }
    //    // visited.resize(size, false);
    // }

    Graph(const vector<vector<double>>& adj) : size(adj.size()), start(0), visitedCount(0), adjacencyMatrix(adj) {
        // this->size = adjacencyMatrix.size();
        // this->adjacencyMatrix = adj;
        // this->start = 0;
        // nodes.resize(size);
        // for (size_t i = 0; i < size; ++i) {
        //     nodes[i].setID(i);
        // }
        visited.resize(adj.size(), false);
    }

    //----------------------- Methods
    void addEdge(double weight, size_t from, size_t to) {
        if (from < size && to < size) {
            adjacencyMatrix[from][to] = weight;
        }
    }

    Tour visit_greedy() {
        resetVisited();
        Tour tour;
        // double cost = 0.0;
        size_t current = start;
        markVisited(current);
        tour.addNode(current, 0.0);

        while (visitedCount < size) {
            // Loop finds the best next neighbour to select.
            double minCost = INF;
            size_t next = current;
            for (size_t i = 0; i < size; ++i) {
                if (isUnvisitedNeighbour(current, i) && adjacencyMatrix[current][i] < minCost) {
                    minCost = adjacencyMatrix[current][i];
                    next = i;
                }
            }
            if (next == current) {
                break;
            }
            // cost += minCost;
            tour.addNode(next, minCost);
            current = next;

            markVisited(current);
        }

        // Accounting for the return cost to start node
        if (visitedCount == size) {
            // cost += adjacencyMatrix[current][start];
            // tour.addNode(start, adjacencyMatrix[current][start]);
            tour.cost += adjacencyMatrix[current][start];
        } else {
            // cost = INF;
            tour.reset();
            tour.cost = INF;
        }

        return tour;
    }

    //------------------------ Getters
    size_t getSize() const {
        return size;
    }

    size_t getStart() const {
        return start;
    }

    size_t getVisitedCount() const {
        return visitedCount;
    }

    // const vector<Node> &getNodes() const {
    //     return nodes;
    // }

    const vector<double>& getAdjacencyList(size_t id) const {
        return adjacencyMatrix[id];
    }

    const vector<vector<double>>& getAdjacencyMatrix() const {
        return adjacencyMatrix;
    }

    bool isVisited(size_t id) const {
        if (id < size) {
            return visited[id];
            // return nodes[id].visited();
        }
        return false;
    }

    //------------------------ Setters
    void setStart(size_t start) {
        this->start = start;
    }
};

int main(int argc, char* argv[]) {
    string input_path = INPUT_PATH;
    string output_path = OUTPUT_PATH;

    if (argc > 1) {
        input_path = argv[1];
    }
    if (argc > 2) {
        output_path = argv[2];
    }

    ifstream inputFile(input_path);
    if (!inputFile.is_open()) {
        cerr << "Error: Unable to open input file: " << input_path << endl;
        return 1;
    }

    ofstream outputFile(output_path);
    if (!outputFile.is_open()) {
        cerr << "Error: Unable to open output file: " << output_path << endl;
        while (inputFile.is_open()) inputFile.close();
        return 1;
    }

    while (inputFile.is_open()) inputFile.close();
    while (outputFile.is_open()) outputFile.close();

    return 0;
}