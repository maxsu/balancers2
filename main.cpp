//  main.cpp
//  beltbalancers2

#include <iostream>
#include <vector>
using namespace std;

#include "test_cases.cpp"
#include "types.h"
#include "utils.cpp"

// Find the ratios given by a certain splitter network (as a double)
Flow outputRatios(Network nodes, int inputs, int splitters, int outputs) {
  // Construct the vector of ratios, which will hold what each splitter outputs
  // in terms of the outputs of the others The [i][j] entry is the amount of
  // node i's output depends on node j's output Eventually, we want to reduce
  // everything to dependencies on the inputs Initially, this will just be the
  // trivial "this splitter outputs what it outputs" vector

  int network_size = inputs + splitters + outputs;

  // Generate an identity flow
  Flow flow;
  for (int i = 0; i < network_size; ++i) {
    Row temp;
    for (int j = 0; j < network_size; ++j) {
      if (i == j) {
        temp.push_back(1);
      } else {
        temp.push_back(0);
      }
    }

    flow.push_back(temp);
  }

  // Solve the nodes, starting at the splitters, in terms of others one by one
  for (int i = inputs; i < network_size; ++i) {
    int node_inputs = nodes[i]->inputs.size();
    int node_outputs = nodes[i]->outputs.size();
    bool node_has_outputs = node_outputs > 0;

    // Generate an empty row
    Row new_row;
    for (int j = 0; j < network_size; ++j) {
      new_row.push_back(0);
    }

    // Iterate through node's inputs
    for (int j = 0; j < node_inputs; ++j) {
      // Find which node this one is
      int node_num;
      for (int k = 0; k < network_size; ++k) {
        if (nodes[k] == nodes[i]->inputs[j]) {
          node_num = k;
        }
      }

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
    for (int k = 0; k < network_size; ++k) {
      new_row[k] *= multiplier;
    }
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
  // Load testcase
  TestNet testnet = testnet_A();

  // Compute ratios
  Flow flow = outputRatios(testnet.network, 1, 12, 4);

  log("Done!");

  // Improvised tests!

  // Collect significant output
  Row final_ratios;
  for (Row row : flow) {
    float ratio = row[0];
    final_ratios.push_back(ratio);
  }

  // Report
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