#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

/********************DO NOT EDIT**********************/
void read_opinions(const string &filename);
void read_edges(const string &filename);

int total_nodes = 0;
/****************************************************************/

// opinion of each node
vector<int> opinions;

// for each node, store who influences it
vector<vector<int>> graph;

double calculate_fraction_of_ones() {
    int ones = 0;
    for (int i = 0; i < opinions.size(); i++) {
        if (opinions[i] == 1) {
            ones++;
        }
    }
    return (double)ones / total_nodes;
}

// get the new opinion of one node based on its incoming neighbors
int get_majority_friend_opinions(int node) {
    int zero_count = 0;
    int one_count = 0;

    for (int i = 0; i < graph[node].size(); i++) {
        int nei = graph[node][i];
        if (opinions[nei] == 1) {
            one_count++;
        } else {
            zero_count++;
        }
    }

    if (one_count > zero_count) {
        return 1;
    }
    return 0;   // tie also returns 0
}

// update all nodes at the same time
bool update_opinions() {
    vector<int> new_opinions = opinions;
    bool changed = false;

    for (int i = 0; i < total_nodes; i++) {
        int new_op = get_majority_friend_opinions(i);
        if (new_op != opinions[i]) {
            new_opinions[i] = new_op;
            changed = true;
        }
    }

    opinions = new_opinions;
    return changed;
}

int main() {
    read_opinions("opinions.txt");
    read_edges("edge_list.txt");

    cout << "Total nodes: " << total_nodes << endl;

    int max_iterations = 30;
    int iteration = 0;
    bool opinions_changed = true;

    cout << "Iteration " << iteration << ": fraction of 1's = "
         << calculate_fraction_of_ones() << endl;

    while (iteration < max_iterations && opinions_changed) {
        opinions_changed = update_opinions();
        iteration++;

        cout << "Iteration " << iteration << ": fraction of 1's = "
             << calculate_fraction_of_ones() << endl;
    }

    double final_fraction = calculate_fraction_of_ones();

    if (final_fraction == 1.0) {
        cout << "Consensus reached: all 1's" << endl;
    } else if (final_fraction == 0.0) {
        cout << "Consensus reached: all 0's" << endl;
    } else {
        cout << "No consensus reached after " << iteration << " iterations" << endl;
    }

    return 0;
}

/*********** Functions to read files **************************/

void read_opinions(const string &filename) {
    ifstream file(filename);
    int id, op;

    vector<pair<int, int>> temp;

    while (file >> id >> op) {
        temp.push_back({id, op});
        if (id >= total_nodes) {
            total_nodes = id + 1;
        }
    }

    file.close();

    opinions.resize(total_nodes, 0);

    for (int i = 0; i < temp.size(); i++) {
        int id = temp[i].first;
        int op = temp[i].second;
        opinions[id] = op;
    }
}

void read_edges(const string &filename) {
    ifstream file(filename);
    int source, target;

    vector<pair<int, int>> edges;

    while (file >> source >> target) {
        edges.push_back({source, target});

        if (source >= total_nodes) {
            total_nodes = source + 1;
        }
        if (target >= total_nodes) {
            total_nodes = target + 1;
        }
    }

    file.close();

    if (opinions.size() < total_nodes) {
        opinions.resize(total_nodes, 0);
    }

    graph.resize(total_nodes);

    for (int i = 0; i < edges.size(); i++) {
        int source = edges[i].first;
        int target = edges[i].second;
        graph[target].push_back(source);
    }
}
/*
cd /d "d:\Users\LinJiangfan\Downloads\ee538-project1-main"
.\Project_2.exe
*/