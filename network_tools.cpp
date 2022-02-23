// network_tools.cpp
// Tools for the Network/Flow domain
#include "types.h"

Row zeroRow(int size) {
  Row zero_row;
  for (int i = 0; i < size; ++i) {
    zero_row.push_back(0);
  }
  return zero_row;
}

Row oneRow(int size, int one_position) {
  Row one_row = zeroRow(size);
  one_row[one_position] = 1;
  return one_row;
}

Row rowMultiply(Row row, double multiplier) {
  for (int i = 0; i < row.size(); ++i) {
    row[i] *= multiplier;
  }
  return row;
}

Flow identityFlow(int size) {
  Flow identity_flow;
  for (int i = 0; i < size; ++i) {
    identity_flow.push_back(oneRow(size, i));
  }
  return identity_flow;
}

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

int nodeNum(Network nodes, Node* node) {
  int node_num;
  for (int k = 0; k < nodes.size(); ++k) {
    if (nodes[k] == node) {
      node_num = k;
    }
  }
  return node_num;
}