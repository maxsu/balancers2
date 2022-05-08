// Tools for the Network/Matrix domain

#include "network_tools.hpp"

#include <assert.h>

#include <algorithm>

#include "network_tools.hpp"
#include "types.hpp"
#include "utils.hpp"

Row constRow(const int size, const double value) { return Row(size, value); }

Row zeroRow(int size) { return constRow(size, 0); }

Row oneRow(int size, int one_position) {
  Row one_row = zeroRow(size);
  vectorGuard(one_row, one_position);
  one_row[one_position] = 1;
  return one_row;
}

Row rowConcat(Row A, Row B) {
  A.insert(A.end(), B.begin(), B.end());
  return A;
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

Matrix constRowMatrix(const int rows, const Row base_row) {
  return Matrix(rows, base_row);
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

void sortMatrix(Matrix& matrix) {
  while (true) {
    Matrix old_matrix = matrix;

    // Sort rows
    sort(matrix.begin(), matrix.end());

    // Sort columns
    matrix = transpose(matrix);
    sort(matrix.begin(), matrix.end());
    matrix = transpose(matrix);

    // Finish when matrix is invariant
    if (matrix == old_matrix) {
      break;
    }
  }
}

using Wiring = std::vector<int>;

using Config = std::vector<Wiring>;

using Configs = std::vector<Config>;

bool isWired(int connection) { return (connection != -1); }

Wiring wiredConnections(Wiring wiring) {
  Wiring wired;
  std::copy_if(wiring.begin(), wiring.end(), std::back_inserter(wired),
               isWired);
  return wired;
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

Matrix balancerFlow(const int input_num, const int output_num) {
  const Row base_row = constRow(input_num, 1.0 / output_num);
  return constRowMatrix(output_num, base_row);
}

Matrix addSplitterToFlow(Matrix flow, const Wiring splitter_inputs,
                         const Wiring splitter_outputs) {
  const int num_flow_outputs = flow.size();
  const int num_flow_inputs = flow[0].size();

  const int num_splitter_outputs = splitter_outputs.size();
  const int num_splitter_inputs = splitter_inputs.size();

  const Wiring wired_inputs = wiredConnections(splitter_inputs);
  const Wiring wired_outputs = wiredConnections(splitter_outputs);

  const double output_fraction = 1.0 / num_splitter_outputs;

  // Find this splitter's output in terms of its input flows
  Row input_flow = zeroRow(num_flow_inputs);
  for (int input : wired_inputs) {
    input_flow = rowAdd(input_flow, flow[input]);
  }

  // Remove circular dependencies from new outputs to old inputs
  double loop_correction = output_fraction;
  for (int output : wired_outputs) {
    loop_correction /= 1 - input_flow[output] * output_fraction;
    input_flow[output] = 0;
  }
  assert(loop_correction != 1 / 0.0);  // Explode if self-flow is degenerate
  input_flow = rowMultiply(input_flow, loop_correction);
  const Row output_flow = constRow(num_splitter_inputs, loop_correction);
  const Row splitter_flow = rowConcat(input_flow, output_flow);

  // Remove other dependencies on the input (if any) that was just removed
  // Might be wonky when there are two self-loops added at once
  for (int i = 0; i < num_flow_outputs; ++i) {
    for (int j = 0; j < num_splitter_inputs; ++j) {
      flow[i].push_back(0);
    }
    for (int j = 0; j < num_splitter_outputs; ++j) {
      if (splitter_outputs[j] != -1) {
        // Flow that splitter wires back into network
        double back_flow = flow[i][splitter_outputs[j]];
        for (int k = 0; k < num_flow_inputs; ++k) {
          flow[i][k] += back_flow * splitter_flow[k];
        }
        // Add dependencies on inputs
        for (int k = 0; k < num_splitter_inputs; ++k) {
          // Flow to new outputs
          int output_index = num_flow_inputs + k;
          double forward_flow = splitter_flow[output_index];
          flow[i][output_index] += back_flow * forward_flow;
        }

        // Clear backflow
        flow[i][splitter_outputs[j]] = 0;
      }
    }
  }

  assert(isRectangular(flow));
  assert(flow[0].size() == num_flow_inputs + num_splitter_inputs);

  // Add new outputs
  for (int i = 0; i < num_splitter_outputs; ++i) {
    flow.push_back(splitter_flow);
  }

  // Now trim the new unused inputs/outputs; start from the back so that erasing
  // can work properly
  for (int i = num_flow_outputs + num_splitter_outputs - 1; i >= 0; --i) {
    // Erase this row if it's an output that is now used
    bool is_used = false;
    // Check if it's an output of the previous network that is now used in the
    // splitter
    for (int j = 0; j < num_splitter_inputs; ++j) {
      if (splitter_inputs[j] == i) {
        is_used = true;
      }
    }
    // Check if it's an output of the splitter that's used
    int output_index = i - num_flow_outputs;
    bool in_output_segment = output_index >= 0;
    bool output_wired = splitter_outputs[output_index] != -1;
    if (in_output_segment && output_wired) {
      is_used = true;
    }
    if (is_used) {
      flow.erase(flow.begin() + i);
      continue;
    }

    // Now trim columns (inputs)
    int old_row_size = flow[i].size();
    for (int j = old_row_size - 1; j >= 0; --j) {
      bool is_used = false;
      // Check if it's an input of the previous network that's now an output of
      // the splitter
      for (int k = 0; k < num_splitter_outputs; ++k) {
        if (splitter_outputs[k] == j) {
          is_used = true;
        }
      }
      // Check if it's a used input of the splitter that's used
      int input_index = num_splitter_inputs + j - old_row_size;
      bool in_input_segment = 0 <= input_index;
      bool input_wired = splitter_inputs[input_index] != -1;
      if (in_input_segment && input_wired) {
        is_used = true;
      }
      if (is_used) {
        flow[i].erase(flow[i].begin() + j);
      }
    }
  }

  // Transform flow into normal form
  sortMatrix(flow);

  return flow;
}