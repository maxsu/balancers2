//  main.cpp
//  beltbalancers2

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

#include "network_tools.h"
#include "test_cases.h"
#include "types.h"
#include "utils.h"

// Find the ratios given by a certain splitter network (as a double).
// Construct the vector of ratios, which will hold what each splitter outputs
// in terms of the outputs of the others. The [i][j] entry is the amount of
// node i's output that depends  on node j's output. Eventually, we want to reduce
// everything to dependencies on the inputs. Initially, this will just be the
// trivial "this splitter outputs what it outputs" vector.
Flow outputRatios(Network nodes, int inputs, int splitters, int outputs) {

  int network_size = inputs + splitters + outputs;

  Flow flow = identityFlow(network_size);

  // Solve the nodes in terms of others
  for (int i = inputs; i < network_size; ++i) {
    Node* current_node = nodes[i];
    int node_outputs = current_node->outputs.size();

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
