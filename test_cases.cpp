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

  Row ratios = {
      1,
      0.75641025641025639,
      0.51282051282051277,
      0.50549450549450547,
      0.25641025641025639,
      0.26923076923076927,
      0.12820512820512819,
      0.25274725274725274,
      0.31684981684981689,
      0.25457875457875456,
      0.12637362637362637,
      0.22161172161172166,
      0.38095238095238093,
      0.38095238095238093,
      0.22161172161172166,
      0.12820512820512819,
      0.26923076923076927,
  };

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

  Row ratios = {
      1,
      0,
      0.5,
      0.16666666666666666,
      0.33333333333333331,
      0.66666666666666663,
      0.33333333333333331,
      0.66666666666666663,
  };

  TestNet B = {nodes, ratios};
  return B;
}