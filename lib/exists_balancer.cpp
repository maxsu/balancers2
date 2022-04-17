// Computes the list of all flows possible with a certain number of splitters

#include "exists_balancer.hpp"

#include <assert.h>

#include <algorithm>
#include <set>
#include <vector>

#include "network_tools.hpp"
#include "types.hpp"

using namespace std;

using Wiring = vector<int>;
using Config = vector<Wiring>;
using Configs = vector<Config>;

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

Config unwired_2_1_splitter = {{-1, -1}, {-1}};

void inline wire_2_inputs_1_fixedOutput(Configs &configs, int n) {
  for (int out = -1; out < n; ++out) {
    configs.push_back({{-1, -1}, {out, -1}});
  }
}

void inline wire_2_fixedInputs_1_output(Configs &configs, int in1, int in2,
                                        int n) {
  for (int out1 = -1; out1 < n; ++out1) {
    configs.push_back({{in1, in2}, {out1, -1}});
  }
}

void inline wire_1_fixedInput_1_output(Configs &configs, int in1, int n) {
  for (int out1 = -1; out1 < n; ++out1) {
    configs.push_back({{in1}, {out1, -1}});
  }
}

bool existsBalancer(int input_size, int output_size, int max_num_splitters) {
  set<Matrix> possible_networks;
  possible_networks.insert({{1}});

  // Note: I assume out1 and out2 aren't both looped back to inputs; check to
  // see if this is valid later
  for (int i = 0; i < max_num_splitters; ++i) {
    set<Matrix> new_possible_networks;

    // Expand on each possible network
    // Need to do this in a way so that there are no "infinite loops"
    for (auto it = possible_networks.begin(); it != possible_networks.end();
         ++it) {
      vector<Config> valid_configs;

      int n = (int)it->size();
      int m = (int)(*it)[0].size();

      // Add trivial unwired 2-1 splitter
      valid_configs.push_back(unwired_2_1_splitter);

      // Add 2-2 splitter with 1 wired output
      wire_2_inputs_1_fixedOutput(valid_configs, m);

      for (int in1 = -1; in1 < (int)it->size(); ++in1) {
        // Cases with one wired input
        valid_configs.push_back({{in1}, {-1}});
        wire_1_fixedInput_1_output(valid_configs, in1, m);

        // Cases with two wired inputs..
        for (int in2 = in1 + 1; in2 < n; ++in2) {
          // .. And one unwired output
          valid_configs.push_back({{in1, in2}, {-1}});

          // .. Or one wired output
          wire_2_fixedInputs_1_output(valid_configs, in1, in2, m);
        }
      }

      // Drop circular dependencies
      for (int j = valid_configs.size() - 1; j >= 0; --j) {
        // If there's a new input, we're fine
        if (valid_configs[j][0][0] == -1) {
          continue;
        }

        bool circular = true;
        for (int k = 0; k < valid_configs[j][0].size(); ++k) {
          // Check if this input is non-circular
          for (int l = 0; l < (*it)[0].size(); ++l) {
            // Check that this isn't a new output of the splitter
            bool true_input = true;
            for (int m = 0; m < valid_configs[j][1].size(); ++m) {
              if (l == valid_configs[j][1][m]) {
                true_input = false;
              }
            }

            if (true_input == true && (*it)[valid_configs[j][0][k]][l] != 0) {
              circular = false;
            }
          }
        }

        if (circular) {
          valid_configs.erase(valid_configs.begin() + j);
        }
      }

      for (int j = 0; j < valid_configs.size(); ++j) {
        new_possible_networks.insert(
            addSplitter(*it, valid_configs[j][0], valid_configs[j][1]));
      }
    }

    for (auto it = new_possible_networks.begin();
         it != new_possible_networks.end(); ++it) {
      possible_networks.insert(*it);
    }
  }

  // Check if it's a splitter
  Row balanced_output;
  for (int i = 0; i < input_size; ++i) {
    balanced_output.push_back(1.0 / output_size);
  }
  Matrix balancer;
  for (int i = 0; i < output_size; ++i) {
    balancer.push_back(balanced_output);
  }

  // it_num is for debugging purposes
  int it_num = 0;
  for (auto it = possible_networks.begin(); it != possible_networks.end();
       ++it, ++it_num) {
    if (*it == balancer) {
      return true;
    }
  }
  return false;
}
