#include "types.h"

struct TestNet {
  Network network;
  Row ratios;
};

// Test case utility functions

// Generate an empty network
Network emptyNetwork(int size) {
  Network nodes;
  for (int i = 0; i < size; ++i) {
    Node* node = new Node;
    nodes.push_back(node);
  }
  return nodes;
}

// Link two nodes in a network
void link(Network& nodes, int source, int target) {
  Node* target_node = nodes[target];
  Node* source_node = nodes[source];
  source_node->outputs.push_back(target_node);
  target_node->inputs.push_back(source_node);
}

// Test cases

// Set up a 17 node test network
TestNet testnet_A() {
  Network nodes = emptyNetwork(17);

  link(nodes, 0, 1);
  link(nodes, 2, 1);
  link(nodes, 1, 2);
  link(nodes, 5, 2);
  link(nodes, 1, 3);
  link(nodes, 9, 3);
  link(nodes, 2, 4);
  link(nodes, 8, 5);
  link(nodes, 11, 5);
  link(nodes, 4, 6);
  link(nodes, 3, 7);
  link(nodes, 3, 8);
  link(nodes, 6, 8);
  link(nodes, 4, 9);
  link(nodes, 7, 9);
  link(nodes, 7, 10);
  link(nodes, 10, 11);
  link(nodes, 8, 11);
  link(nodes, 9, 12);
  link(nodes, 10, 12);
  link(nodes, 12, 13);
  link(nodes, 11, 14);
  link(nodes, 6, 15);
  link(nodes, 5, 16);

  Row ratios = {1,
                0.75641024112701416,
                0.5128205418586731,
                0.50549453496932983,
                0.25641027092933655,
                0.26923078298568726,
                0.12820513546466827,
                0.25274726748466492,
                0.31684982776641846,
                0.25457876920700073,
                0.12637363374233246,
                0.22161172330379486,
                0.380952388048172,
                0.380952388048172,
                0.22161172330379486,
                0.12820513546466827,
                0.26923078298568726};

  TestNet A = {nodes, ratios};
  return A;
}

// Set up a 8 node test network
TestNet testnet_B() {
  Network nodes = emptyNetwork(8);

  link(nodes, 0, 2);
  link(nodes, 1, 2);
  link(nodes, 2, 4);
  link(nodes, 2, 5);
  link(nodes, 3, 4);
  link(nodes, 3, 5);
  link(nodes, 4, 3);
  link(nodes, 4, 6);
  link(nodes, 5, 7);

  Row ratios = {1,
                0,
                0.5,
                0.1666666716337204,
                0.3333333432674408,
                0.66666668653488159,
                0.3333333432674408,
                0.66666668653488159};

  TestNet B = {nodes, ratios};
  return B;
}