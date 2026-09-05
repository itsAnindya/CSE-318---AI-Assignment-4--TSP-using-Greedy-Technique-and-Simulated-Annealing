#include <bits/stdc++.h>

using namespace std;

constexpr const double INF = numeric_limits<double>::infinity();
constexpr const char INPUT_PATH[] = "input.txt";
constexpr const char OUTPUT_PATH[] = "output.txt";

constexpr const double INITIAL_TEMPERATURE = 1000.0;
constexpr const double COOLING_RATE = 0.995;
constexpr const double MIN_TEMPERATURE = 0.001;
constexpr const size_t ITERATION_PER_TEMPERATURE = 100;
constexpr const size_t MAX_ITERATIONS = 100000;
constexpr bool SOLUTION_INITIALIZE_GREEDY = true;
constexpr unsigned NEIGHBOUR_SELECTION_METHOD = 0; // 0: swap, 1: reverse segment

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
// Dumb structure, does not have access to any graph instance. Must be handled by graph class.
struct Tour {
    double cost;
    vector<size_t> path;

    Tour(const vector<size_t> &p = {}, double c = 0.0) : cost(c), path(p) {}

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
        for (auto &v : adjacencyMatrix) {
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

    Graph(const vector<vector<double>> &adj) : size(adj.size()), start(0), visitedCount(0), adjacencyMatrix(adj) {
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

    const vector<double> &getAdjacencyList(size_t id) const {
        return adjacencyMatrix[id];
    }

    const vector<vector<double>> &getAdjacencyMatrix() const {
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

    double calculateCost(const Tour &tour) const {
        double totalCost = 0.0;
        for (size_t i = 0; i < tour.path.size(); ++i) {
            size_t from = tour.path[i];
            size_t to = tour.path[(i + 1) % tour.path.size()];
            if (from < size && to < size) {
                totalCost += adjacencyMatrix[from][to];
            } else {
                return INF; // Invalid node index
            }
        }
        return totalCost;
    }
};

class SimulatedAnnealing {
  private:
    Graph &graph;
    double temperature = INITIAL_TEMPERATURE;
    double cooling_rate = COOLING_RATE;
    Tour solution;
    bool solution_init_greedy = SOLUTION_INITIALIZE_GREEDY;
    size_t iterations = MAX_ITERATIONS;

    bool randomDecision(double probability) const {
        static random_device rd;
        static mt19937 gen(rd());
        uniform_real_distribution<double> dis(0.0, 1.0);
        return dis(gen) < probability;
    }

    void solutionInitialize_greedy() {
        solution = graph.visit_greedy();
    }

    void solutionInitialize_random() {
        size_t n = graph.getSize();
        vector<size_t> path(n);
        iota(path.begin(), path.end(), 0);
        shuffle(path.begin() + 1, path.end(), mt19937(random_device()())); // city 0 should stay fixed in its position
        solution = Tour(path);
        double cost = graph.calculateCost(solution);
        solution.cost = cost;
    }

    void initialize() {
        temperature = INITIAL_TEMPERATURE;
        if (solution_init_greedy) {
            solutionInitialize_greedy();
        } else {
            solutionInitialize_random();
        }
    }

    Tour reverseSegment(size_t start, size_t end) {
        Tour newSolution = solution;
        if (0 < start && start < end && end < newSolution.path.size()) { // city 0 should stay fixed in its position
            while (start < end) {
                swap(newSolution.path[start], newSolution.path[end]);
                start++;
                end--;
            }
            newSolution.cost = graph.calculateCost(newSolution);
        }
        return newSolution;
    }

    Tour swapNodes(size_t i, size_t j) {
        Tour newSolution = solution;
        if (0 < i && i < solution.path.size() && 0 < j && j < solution.path.size()) { // city 0 should stay fixed in its position
            swap(newSolution.path[i], newSolution.path[j]);
            newSolution.cost = graph.calculateCost(newSolution);
        }
        return newSolution;
    }

    Tour neighbour(unsigned neighbour_selection_method = NEIGHBOUR_SELECTION_METHOD) {
        Tour neighbourSolution = solution;

        size_t n = graph.getSize();
        if (n < 3) {
            return neighbourSolution; // Not enough cities to generate a neighbour
        }

        static random_device rd;
        static mt19937 gen(rd());
        uniform_int_distribution<size_t> dis1(1, n - 2); // city 0 should stay fixed in its position
        size_t i = dis1(gen);
        uniform_int_distribution<size_t> dis2(1 + 1, n - 1);
        size_t j = dis2(gen);

        if (neighbour_selection_method == 0) {
            neighbourSolution = reverseSegment(i, j);
        } else if (neighbour_selection_method == 1) {
            neighbourSolution = swapNodes(i, j);
        }
        return neighbourSolution;
    }

    double calculateCurrentCost() const {
        return graph.calculateCost(solution);
    }

    void cool() {
        temperature *= cooling_rate;
    }

    Tour simulatedAnnealing() {
        initialize();
        Tour current = solution;
        Tour best = solution;
        double delta;
        for (size_t i = 0; i < iterations; i++) {
            Tour neighbourSolution = neighbour();
            delta = neighbourSolution.cost - current.cost;

            if (delta < 0 || randomDecision(exp(-delta / temperature))) {
                current = neighbourSolution;
                if (current.cost < best.cost) {
                    best = current;
                }
            }

            if ((i + 1) % ITERATION_PER_TEMPERATURE == 0)
                cool();

            if (temperature < MIN_TEMPERATURE) {
                break;
            }
        }

        return best;
    }

  public:
    SimulatedAnnealing(Graph &g) : graph(g) {}
};

int main(int argc, char *argv[]) {
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
        while (inputFile.is_open())
            inputFile.close();
        return 1;
    }

    while (inputFile.is_open())
        inputFile.close();
    while (outputFile.is_open())
        outputFile.close();

    return 0;
}