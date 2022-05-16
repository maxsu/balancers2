// Tools for the Network/Matrix domain

#pragma once

#include <vector>

#include "matrix.hpp"

/////////////////////////////
// Data Types
/////////////////////////////

// A splitter network node
struct Node {
  std::vector<Node*> inputs;
  std::vector<Node*> outputs;
};

// Represents a network of splitters
using Network = std::vector<Node*>;

// Represents network links
using Links = std::vector<std::pair<int, int>>;

//////////////////////////////
// Network operations
//////////////////////////////

// Generate an empty network
Network emptyNetwork(int size);

// Link two nodes in a network
void link(Network& nodes, int source, int target);

// Make a network with given links
Network makeNetwork(const int size, const Links link);

// Find a node's index in a network
int nodeNum(Network nodes, Node* node);