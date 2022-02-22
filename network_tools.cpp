#include "types.h"

// Generate an empty row
Row zeroRow(int size) {
  Row zero_row;
  for (int i = 0; i < size; ++i) {
    zero_row.push_back(0);
  }
  return zero_row;
}

// Generate a row with a single 1 entry
Row oneRow(int size, int one_position) {
  Row one_row = zeroRow(size);
  one_row[one_position] = 1;
  return one_row;
}

// Multiply a row by a doublee
Row rowMultiply(Row row, double multiplier) {
  for (int i = 0; i < row.size(); ++i) {
    row[i] *= multiplier;
  }
  return row;
}

// Generate an identity flow
Flow identityFlow(int size) {
  Flow identity_flow;
  for (int i = 0; i < size; ++i) {
    identity_flow.push_back(oneRow(size, i));
  }
  return identity_flow;
}

// Find a node's index in a network
int nodeNum(Network nodes, Node* node) {
  int node_num;
  for (int k = 0; k < nodes.size(); ++k) {
    if (nodes[k] == node) {
      node_num = k;
    }
  }
  return node_num;
}