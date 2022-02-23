// network_tools.cpp
// Tools for the Network/Flow domain
#include "types.h"
#include "utils.h"

Row zeroRow(int size) {
  Row zero_row;
  for (int i = 0; i < size; ++i) {
    zero_row.push_back(0);
  }
  return zero_row;
}

Row oneRow(int size, int one_position) {
  Row one_row = zeroRow(size);
  vectorGuard(one_row, one_position);
  one_row[one_position] = 1;
  return one_row;
}

Row rowAdd(Row A, Row B) {
  if (A.size() != B.size()) {
    throw "Row sizes mismatch";
  }

  Row C = zeroRow(A.size());
  for (int i = 0; i < A.size(); i++) {
    C[i] = A[i] + B[i];
  }
  return C;
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

Row getColumn(Flow flow, int column_position) {
  vectorGuard(flow, column_position);
  Row column;
  for (Row row : flow) {
    column.push_back(row[column_position]);
  }
  return column;
}

Network emptyNetwork(int size) {
  Network nodes;
  for (int i = 0; i < size; ++i) {
    Node* node = new Node;
    nodes.push_back(node);
  }
  return nodes;
}

void link(Network& nodes, int source, int target) {
  vectorGuard(nodes, source);
  vectorGuard(nodes, target);
  Node* target_node = nodes[target];
  Node* source_node = nodes[source];
  source_node->outputs.push_back(target_node);
  target_node->inputs.push_back(source_node);
}

int nodeNum(Network nodes, Node* node) {
  for (int node_num = 0; node_num < nodes.size(); ++node_num) {
    if (node == nodes[node_num]) {
      return node_num;
    }
  }
  throw "Node not found";
}