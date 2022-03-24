// compute the output of a splitter network

#include <algorithm>

#include "network_tools.h"
#include "types.h"

Flow outputRatios(Network nodes) {
  int network_size = nodes.size();

  Flow flow = identityFlow(network_size);

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