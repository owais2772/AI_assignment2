// 8 Puzzle Game using A Star
#include <iostream>
#include <string.h>
#include <vector>

#include "header_files/priority_queue.h"
#include "header_files/queue.h"

using namespace std;

struct State {
    int board[3][3];
    pair<int, int> zeroPosition;

    State() {
        memset(board, -1, sizeof(board));
    }

    State(int b[3][3]) {
        for (int i = 0; i < 3; i++) {
            memcpy(this->board[i], b[i], sizeof(int) * 3);
        }
        this->zeroPosition = find(0);
    }

    pair<int, int> find(int n) {
        pair<int, int> position;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (this->board[i][j] != n)
                    continue;
                position = make_pair(i, j);
                break;
            }
        }
        return position;
    }

    void print() {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                cout << board[i][j] << " ";
            }
            cout << endl;
        }
    }

    bool operator==(const State other) const {
        bool isEqual = true;
        for (int i = 0; i < 3 and isEqual; i++) {
            for (int j = 0; j < 3 and isEqual; j++) {
                if (this->board[i][j] != other.board[i][j]) {
                    isEqual = false;
                }
            }
        }
        return isEqual;
    }

    bool operator!=(const State other) const {
        bool isEqual = true;
        for (int i = 0; i < 3 and isEqual; i++) {
            for (int j = 0; j < 3 and isEqual; j++) {
                if (this->board[i][j] != other.board[i][j]) {
                    isEqual = false;
                }
            }
        }
        return !isEqual;
    }
};

class Node {
private:
    State state;
    Node* parent;
    int pathCost;
    int heuristicCost;
    int totalCost;

public:
    Node(State state, Node* parent, int heuristicCost, int pathCost = 1) {
        this->state = state;
        this->parent = parent;
        this->pathCost = pathCost;
        this->heuristicCost = heuristicCost;
        this->totalCost = pathCost + heuristicCost;
    }

    State getState() {
        return this->state;
    }

    int getTotalCost() {
        return this->totalCost;
    }

    int getPathCost() {
        return this->pathCost;
    }

    void setState(State state) {
        this->state = state;
    }

    bool operator<(const Node& other) const {
        return this->totalCost < other.totalCost;
    }

    bool operator>(const Node& other) const {
        return this->totalCost > other.totalCost;
    }

    bool operator==(const Node other) const {
        return this->state == other.state;
    }

    bool operator!=(const Node& other) const {
        return this->state != other.state;
    }
};

class Problem {
private:
    Node* intialNode;
    Node* goalNode;

    static bool priorityComparison(Node a, Node b) {
        return a.getTotalCost() < b.getTotalCost();
    }

    Queue<Node> exploredSet;
    CustomPriorityQueue<Node> frontier;


    int calculateHeuristicMisplaced(State* state) {
        int heuristicCost = 0;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (state->board[i][j] != 0 and state->board[i][j] != this->goalNode->getState().board[i][j]) {
                    heuristicCost++;
                }
            }
        }
        return heuristicCost;
    }

    int calculateHeuristicManhatnDistance(State* state) {
        int heuristicCost = 0;
        pair<int, int> goalPosition;
        pair<int, int> currentPosition;
        for (int i = 1; i < 9; i++) {
            goalPosition = this->goalNode->getState().find(i);
            currentPosition = state->find(i);
            heuristicCost += abs(goalPosition.first - currentPosition.first) + abs(
                goalPosition.second - currentPosition.second);
        }
        return heuristicCost;
    }

    int calculateHeuristic(State *state) {
        return calculateHeuristicMisplaced(state) + calculateHeuristicManhatnDistance(state);
    }

    // possible moves
    // return a list of states
    vector<Node> successors(Node* node) {
        State state = node->getState();
        vector<State> states;
        vector<Node> nodes;
        const int x = state.zeroPosition.first;
        const int y = state.zeroPosition.second;
        int tempBoard[3][3];
        // cout << "Zero Position: (x: " << x << ", y: " << y << ")" << endl;
        if (x > 0) {
            memcpy(tempBoard, state.board, sizeof(int) * 3 * 3);
            tempBoard[x][y] = tempBoard[x - 1][y];
            tempBoard[x - 1][y] = 0;
            states.push_back(State(tempBoard));
        }
        // move down
        if (x < 2) {
            memcpy(tempBoard, state.board, sizeof(int) * 3 * 3);
            tempBoard[x][y] = tempBoard[x + 1][y];
            tempBoard[x + 1][y] = 0;
            states.push_back(State(tempBoard));
        }
        // move left
        if (y > 0) {
            memcpy(tempBoard, state.board, sizeof(int) * 3 * 3);
            tempBoard[x][y] = tempBoard[x][y - 1];
            tempBoard[x][y - 1] = 0;
            states.push_back(State(tempBoard));
        }
        // move right
        if (y < 2) {
            memcpy(tempBoard, state.board, sizeof(int) * 3 * 3);
            tempBoard[x][y] = tempBoard[x][y + 1];
            tempBoard[x][y + 1] = 0;
            states.push_back(State(tempBoard));
        }
        for (int i = 0; i < states.size(); i++) {
            int hc = calculateHeuristicManhatnDistance(&states.at(i)) + calculateHeuristicMisplaced(&states.at(i));
            nodes.push_back(Node(states.at(i), node, hc, node->getPathCost() + 1));
        }
        this->exploredSet.insert(*node);
        return nodes;
    }

public:
    Problem(State intialState, State goalState) {
        frontier = CustomPriorityQueue<Node>(priorityComparison);
        this->goalNode = new Node(goalState, nullptr, 0);
        this->intialNode = new Node(intialState, nullptr, calculateHeuristic(&intialState));
    }

    void solve() {
        this->frontier.push(*this->intialNode);
        int iters = 0;
        while (!this->frontier.empty()) {
            iters++;
            Node node = this->frontier.pop();
            cout<< "Iteration Number: " << iters << endl;
            cout<< "Total Cost: " << node.getTotalCost() << endl;
            cout << "Zero Position: " << node.getState().zeroPosition.first << ", " << node.getState().zeroPosition.second << endl;
            node.getState().print();
            if (node.getState() == this->goalNode->getState()) {
                cout << "Goal Found in " << iters << " Iterations" << endl;
                return;
            }
            vector<Node> nodes = successors(&node);
            for (int i = 0; i < nodes.size(); i++) {
                if (this->exploredSet.contains(nodes.at(i))) {
                    continue;
                }
                this->frontier.push(nodes[i]);
            }
        }
    }
};

int main() {
    int initialBoard[3][3] = {{8, 0, 6}, {5, 4, 7}, {2, 3, 1}};
    // int initialBoard[3][3] = {{7, 2, 4}, {5, 0, 6}, {8, 3, 1}};
    int goalBoard[3][3] = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}};

    State initialState(initialBoard), goalState(goalBoard);

    Problem p(initialState, goalState);
    p.solve();
    return 0;
}
