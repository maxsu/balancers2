//  main.cpp
//  beltbalancers2

#include <iostream>
#include <vector>
using namespace std;

#include "network_tools.h"
#include "test_cases.h"
#include "types.h"
#include "utils.h"

// Find the ratios given by a certain splitter network (as a double)
Flow outputRatios(Network nodes, int inputs, int splitters, int outputs) {
  // Construct the vector of ratios, which will hold what each splitter outputs
  // in terms of the outputs of the others The [i][j] entry is the amount of
  // node i's output depends on node j's output Eventually, we want to reduce
  // everything to dependencies on the inputs Initially, this will just be the
  // trivial "this splitter outputs what it outputs" vector

  int network_size = inputs + splitters + outputs;

  Flow flow = identityFlow(network_size);

  // Solve the nodes, starting at the splitters, in terms of others one by one
  for (int i = inputs; i < network_size; ++i) {
    Node* current_node = nodes[i];

    int node_inputs = current_node->inputs.size();
    int node_outputs = current_node->outputs.size();
    bool node_has_outputs = node_outputs > 0;

    Row new_row = zeroRow(network_size);

    // Iterate through node's inputs
    for (auto input_node : current_node->inputs) {
      // Find which node this one is
      int node_num = nodeNum(nodes, input_node);

      for (int k = 0; k < network_size; ++k) {
        // Check if this is an output node; if so, then don't divide output by
        // the number of belts coming out
        if (node_has_outputs) {
          new_row[k] += flow[node_num][k] / node_outputs;
        } else {
          new_row[k] += flow[node_num][k];
        }
      }
    }

    // Update self-dependencies on this splitter
    double multiplier = 1 / (1 - new_row[i]);
    new_row = rowMultiply(new_row, multiplier);
    new_row[i] = 0;

    // Update flow
    flow[i] = new_row;

    // Update any other splitters with this one as its flow
    for (int j = 0; j < network_size; ++j) {
      for (int k = 0; k < network_size; ++k) {
        flow[j][k] += flow[j][i] * flow[i][k];
      }
      flow[j][i] = 0;
    }
  }

  return flow;
}

int main() {
  TestNet testnet = testnet_A();  // 17 node testcase

  Flow flow = outputRatios(testnet.network, 1, 12, 4);
  Row final_ratios = getColumn(flow, 0);

  log("Done!");
  log("Results:");
  log(final_ratios);

  if (final_ratios == testnet.ratios) {
    log("Results are valid!");

  } else {
    log("Failed results test.");
    log("Expected:");
    log(testnet.ratios);
  }
}