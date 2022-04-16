// Tools for the Network/Matrix domain

#include "network_tools.hpp"

#include <assert.h>

#include <algorithm>

#include "network_tools.hpp"
#include "types.hpp"
#include "utils.hpp"

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

bool isRectangular(Matrix network) {
  int M = network[0].size();
  bool is_rectangular = true;

  for (auto row : network) {
    if (row.size() != M) {
      is_rectangular = false;
    }
  }

  return is_rectangular;
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

using Wiring = vector<int>;

using Config = vector<Wiring>;

using Configs = vector<Config>;

Matrix addSplitter(Matrix network, const Wiring splitter_inputs,
                   const Wiring splitter_outputs) {
  const int N = network.size();
  const int M = network[0].size();

  const int n = splitter_inputs.size();
  const int m = splitter_outputs.size();

  Row splitter_flow = zeroRow(M);
  for (int i = 0; i < n; ++i) {
    splitter_flow.push_back(1.0 / m);
  }

  // Find this splitter's output in terms of its input flows
  for (int i = 0; i < n; ++i) {
    if (splitter_inputs[i] != -1) {
      for (int j = 0; j < M; ++j) {
        splitter_flow[j] += network[splitter_inputs[i]][j] / m;
      }
    }
  }

  // Remove circular dependencies of the new outputs on any inputs that they
  // lead to
  Row new_splitter_flow = splitter_flow;
  for (int i = 0; i < m; ++i) {
    if (splitter_outputs[i] != -1) {
      for (int j = 0; j < n + M; ++j) {
        new_splitter_flow[j] *= 1 / (1 - splitter_flow[splitter_outputs[i]]);
      }

      new_splitter_flow[splitter_outputs[i]] = 0;
    }
  }
  splitter_flow = new_splitter_flow;

  // Remove other dependencies on the input (if any) that was just removed
  // Might be wonky when there are two self-loops added at once
  for (int i = 0; i < N; ++i) {
    bool added_inputs = false;

    for (int j = 0; j < m; ++j) {
      if (splitter_outputs[j] != -1) {
        for (int k = 0; k < M; ++k) {
          network[i][k] += network[i][splitter_outputs[j]] * splitter_flow[k];
        }
        // Add dependencies on inputs
        for (int k = 0; k < n; ++k) {
          network[i].push_back(network[i][splitter_outputs[j]] *
                               splitter_flow[M + k]);

          added_inputs = true;
        }

        network[i][splitter_outputs[j]] = 0;
      }
    }

    if (!added_inputs) {
      for (int j = 0; j < n; ++j) {
        network[i].push_back(0);
      }
    }
  }

  assert(isRectangular(network));
  assert(network[0].size() == M + n);

  // Add new outputs
  for (int i = 0; i < m; ++i) {
    network.push_back(splitter_flow);
  }

  // Now trim the new unused inputs/outputs; start from the back so that erasing
  // can work properly
  for (int i = N + m - 1; i >= 0; --i) {
    // Erase this row if it's an output that is now used
    bool is_used = false;
    // Check if it's an output of the previous network that is now used in the
    // splitter
    for (int j = 0; j < n; ++j) {
      if (splitter_inputs[j] == i) {
        is_used = true;
      }
    }
    // Check if it's an output of the splitter that's used
    int output_index = i - N;
    bool in_output_segment = output_index >= 0;
    bool output_wired = splitter_outputs[output_index] != -1;
    if (in_output_segment && output_wired) {
      is_used = true;
    }
    if (is_used) {
      network.erase(network.begin() + i);
      continue;
    }

    // Now trim columns (inputs)
    int old_row_size = network[i].size();
    for (int j = old_row_size - 1; j >= 0; --j) {
      bool is_used = false;
      // Check if it's an input of the previous network that's now an output of
      // the splitter
      for (int k = 0; k < m; ++k) {
        if (splitter_outputs[k] == j) {
          is_used = true;
        }
      }
      // Check if it's a used input of the splitter that's used
      int input_index = n + j - old_row_size;
      bool in_input_segment = 0 <= input_index;
      bool input_wired = splitter_inputs[input_index] != -1;
      if (in_input_segment && input_wired) {
        is_used = true;
      }
      if (is_used) {
        network[i].erase(network[i].begin() + j);
      }
    }
  }

  // Keep sorting rows and columns until nothing further happens (to transform
  // into normal form)
  bool sorted = false;
  while (!sorted) {
    Matrix old_network = network;

    sort(network.begin(), network.end());
    network = transpose(network);
    sort(network.begin(), network.end());
    network = transpose(network);

    if (network == old_network) {
      sorted = true;
    }
  }

  return network;
}