//  main.cpp
//  beltbalancers2

#include <iostream>
#include <vector>
using namespace std;

#include "test_cases.cpp"
#include "types.h"
#include "utils.cpp"

// Find the ratios given by a certain splitter network (as a double)
Flow outputRatios(Network nodes, int num_inputs, int num_splitters,
                  int num_outputs) {
  // Construct the vector of ratios, which will hold what each splitter outputs
  // in terms of the outputs of the others The [i][j] entry is the amount of
  // node i's output depends on node j's output Eventually, we want to reduce
  // everything to dependencies on the inputs Initially, this will just be the
  // trivial "this splitter outputs what it outputs" vector
  Flow flow;

  for (int i = 0; i < num_inputs + num_splitters + num_outputs; ++i) {
    Row temp;
    for (int j = 0; j < num_inputs + num_splitters + num_outputs; ++j) {
      if (i == j) {
        temp.push_back(1);
      } else {
        temp.push_back(0);
      }
    }

    flow.push_back(temp);
  }

  // Solve the nodes, starting at the splitters, in terms of others one by one
  for (int i = num_inputs; i < num_inputs + num_splitters + num_outputs; ++i) {
    Row new_flow;
    for (int j = 0; j < num_inputs + num_splitters + num_outputs; ++j) {
      new_flow.push_back(0);
    }

    for (int j = 0; j < nodes[i]->inputs.size(); ++j) {
      // Find which node this one is
      int node_num;
      for (int k = 0; k < num_inputs + num_splitters + num_outputs; ++k) {
        if (nodes[k] == nodes[i]->inputs[j]) {
          node_num = k;
        }
      }

      for (int k = 0; k < num_inputs + num_splitters + num_outputs; ++k) {
        // Check if this is an output node; if so, then don't divide output by
        // the number of belts coming out
        if (i < num_inputs + num_splitters) {
          new_flow[k] += flow[node_num][k] / nodes[i]->outputs.size();
        } else {
          new_flow[k] += flow[node_num][k];
        }
      }
    }

    // Update self-dependencies on this splitter
    double multiplier = 1 / (1 - new_flow[i]);
    for (int k = 0; k < num_inputs + num_splitters + num_outputs; ++k) {
      new_flow[k] *= multiplier;
    }
    new_flow[i] = 0;

    for (int j = 0; j < num_inputs + num_splitters + num_outputs; ++j) {
      flow[i][j] = new_flow[j];
    }

    // Update any other splitters with this one as its flow
    for (int j = 0; j < num_inputs + num_splitters + num_outputs; ++j) {
      for (int k = 0; k < num_inputs + num_splitters + num_outputs; ++k) {
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