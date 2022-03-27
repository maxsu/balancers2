// Tools for the Network/Matrix domain

#include "types.hpp"
#include "utils.hpp"
#include "network_tools.hpp"

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

Matrix identityMatrix(int size) {
  Matrix identity_matrix;
  for (int i = 0; i < size; ++i) {
    identity_matrix.push_back(oneRow(size, i));
  }
  return identity_matrix;
}

Row getColumn(Matrix matrix, int column_position) {
  vectorGuard(matrix[0], column_position);
  Row column;
  int m = matrix.size();
  for (int j = 0; j < m; j++) {
    column.push_back(matrix[j][column_position]);
  }
  return column;
}

Matrix transpose(Matrix matrix) {
    Matrix transpose_matrix;
    
    int m = matrix.size();

    if (m == 0) {
        return matrix;
    }
    
    int n = matrix[0].size();
    
    for (int i = 0; i < n; ++i) {
        Row column = getColumn(matrix, i);
        transpose_matrix.push_back(column);
    }
    
    return transpose_matrix;
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