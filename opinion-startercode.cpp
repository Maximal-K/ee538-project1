#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

/********************DO NOT EDIT**********************/
// Function prototype. Defined later.
void read_opinions(const string &filename); // reads file into opinions vector and updates total_nodes as needed
void read_edges(const string &filename); // reads file into edge_list, defined later
void build_adj_matrix(); // convert edge_list to adjacency matrix

int total_nodes = 0; // We keep track of the total number of nodes based on largest node id.


/****************************************************************/

/******** Create adjacency matrix and vector of opinions */
// simple vector to hold each node's opinion (0 or 1)
std::vector<int> opinions;

// global adjacency matrix initialized later
std::vector<std::vector<int> > adj;

// edge list: each row contains {source, target}
std::vector<std::vector<int> > edge_list;

void build_adj_matrix() {
    // Initialize matrix
    adj.resize(total_nodes, std::vector<int>(total_nodes, 0));
    // Build matrix from edge list
    for (const auto &edge: edge_list) {
        const int source = edge[0];
        const int target = edge[1];
        adj[source][target] = 1;
    }
}

double calculate_fraction_of_ones() {
    int count = 0;
    for (int i = 0; i < total_nodes; i++) {
        if (opinions[i] == 1) {
            count++;
        }
    }
    return static_cast<double>(count) / total_nodes;
}

// For a given node, count majority opinion among its neighbors. Tie -> 0.
int get_majority_friend_opinions(const int node) {
    // read in all friend opinions
    int count0 = 0;
    int count1 = 0;
    for (int i = 0; i < total_nodes; i++) {
        // if friend, then look up on their opinion
        if (adj[i][node]) {
            if (opinions[i]) count1++;
            else count0++;
        }
    }

    // compare and get majority
    return count1 > count0 ? 1 : 0;
}

// Calculate new opinions for all voters and return if anyone's opinion changed
bool update_opinions() {
    bool changed = false;
    vector<int> new_opinions = opinions;
    for (int i = 0; i < total_nodes; i++) {
        if (const int new_opinion = get_majority_friend_opinions(i); opinions[i] != new_opinion) {
            changed = true;
            new_opinions[i] = new_opinion;
        }
    }
    opinions = new_opinions;
    return changed;
}

int main() {
    // no preallocation; vectors grow on demand

    // Read input files
    read_opinions("opinions.txt");
    read_edges("edge_list.txt");

    // convert edge list into adjacency matrix once we know total_nodes
    build_adj_matrix();

    cout << "Total nodes: " << total_nodes << endl;

    // Run simulation
    constexpr int max_iterations = 30;
    int iteration = 0;
    bool opinions_changed = true;

    // Print initial state
    cout << "Iteration " << iteration << ": fraction of 1's = "
            << calculate_fraction_of_ones() << endl;

    /// (6)  //////////////////////////////////////////////
    while ((iteration < max_iterations) && (opinions_changed)) {
        opinions_changed = update_opinions();
        iteration++;
        if (iteration % 1 == 0)
            cout << "Iteration " << iteration << ": fraction of 1's = "
                    << calculate_fraction_of_ones() << endl;
    }

    ////////////////////////////////////////////////////////
    // Print final result
    const double final_fraction = calculate_fraction_of_ones();
    cout << "Iteration " << iteration << ": fraction of 1's = "
            << final_fraction << endl;

    if (final_fraction == 1.0)
        cout << "Consensus reached: all 1's" << endl;
    else if (final_fraction == 0.0)
        cout << "Consensus reached: all 0's" << endl;
    else
        cout << "No consensus reached after " << iteration << " iterations" << endl;

    return 0;
}


/*********** Functions to read files **************************/

// Read opinion vector from file.
void read_opinions(const string &filename) {
    ifstream file(filename);
    int id, opinion;
    while (file >> id >> opinion) {
        opinions.push_back(opinion);
        if (id >= total_nodes) total_nodes = id + 1;
    }
    file.close();
}

// Read edge list from file and update total nodes as needed.
void read_edges(const string &filename) {
    ifstream file(filename);
    int source, target;

    while (file >> source >> target) {
        edge_list.push_back({source, target});
        if (source >= total_nodes) total_nodes = source + 1;
        if (target >= total_nodes) total_nodes = target + 1;
    }
    file.close();
}

/********************************************************************** */
