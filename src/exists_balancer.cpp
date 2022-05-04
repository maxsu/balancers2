// Computes the list of all flows possible with a certain number of splitters

#include "exists_balancer.hpp"

#include <set>
#include <vector>

#include "network_tools.hpp"
#include "types.hpp"

using std::set;
using std::vector;

Config unwired_2_1_splitter = {{-1, -1}, {-1}};

void wire_2_inputs_1_fixedOutput(Configs &configs, int n) {
  for (int out = -1; out < n; ++out) {
    configs.push_back({{-1, -1}, {out, -1}});
  }
}

void wire_2_fixedInputs_1_output(Configs &configs, int in1, int in2, int n) {
  for (int out1 = -1; out1 < n; ++out1) {
    configs.push_back({{in1, in2}, {out1, -1}});
  }
}

void wire_1_fixedInput_1_output(Configs &configs, int in1, int n) {
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
            addSplitterToFlow(*it, valid_configs[j][0], valid_configs[j][1]));
      }
    }

    for (auto it = new_possible_networks.begin();
         it != new_possible_networks.end(); ++it) {
      possible_networks.insert(*it);
    }
  }

  // Check if it's a splitter
  Matrix balancer = balancerFlow(input_size, output_size);
  int it_num = 0;  // it_num is for debugging purposes
  for (auto it = possible_networks.begin(); it != possible_networks.end();
       ++it, ++it_num) {
    if (*it == balancer) {
      return true;
    }
  }
  return false;
}
