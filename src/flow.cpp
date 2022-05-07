// Tools for the Network/Matrix domain

#include "flow.hpp"

#include <assert.h>

#include <algorithm>

#include "vector_guard.hpp"
#include "wiring.hpp"

using std::max;

Matrix balancerFlow(const int input_num, const int output_num) {
  const Row base_row = constRow(input_num, 1.0 / output_num);
  return constRowMatrix(output_num, base_row);
}

Matrix networkToFlow(Network nodes) {
  int network_size = nodes.size();

  Matrix flow = identityMatrix(network_size);

  // Solve the nodes in terms of others
  for (int i = 0; i < network_size; ++i) {
    Node* current_node = nodes[i];

    int node_inputs = current_node->inputs.size();
    int node_outputs = current_node->outputs.size();

    // Do nothing for input nodes
    if (node_inputs == 0) {
      continue;
    }

    // Sum input node rows
    Row new_row = zeroRow(network_size);
    for (auto input_node : current_node->inputs) {
      Row input_row = flow[nodeNum(nodes, input_node)];
      new_row = rowAdd(new_row, input_row);
    }

    // Divide by number of belt outputs, if any
    double belt_normalizer = 1.0 / max(node_outputs, 1);
    new_row = rowMultiply(new_row, belt_normalizer);

    // Update self-dependencies on this node
    double self_flow = 1 / (1 - new_row[i]);
    new_row = rowMultiply(new_row, self_flow);
    new_row[i] = 0;

    // Update other nodes that flow to this one
    for (int j = 0; j < network_size; ++j) {
      // backflow is the share of i's flow that comes from j???
      double backflow = flow[j][i];
      flow[j] = rowAdd(flow[j], rowMultiply(new_row, backflow));
      // clear back-pressure
      flow[j][i] = 0;
    }

    // Update flow
    flow[i] = new_row;
  }

  return flow;
}

Matrix addSplitterToFlow(Matrix flow, const Config splitter_config) {
  const auto [splitter_inputs, splitter_outputs] = splitter_config;
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
