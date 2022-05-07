// Wiring Stuff

#include "wiring.hpp"

#include <algorithm>

#include "flow.hpp"
#include "iters.hpp"
#include "vector_guard.hpp"

bool isWired(int connection) { return (connection != -1); }

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

Wiring wiredConnections(Wiring wiring) {
  Wiring wired;
  std::copy_if(wiring.begin(), wiring.end(), std::back_inserter(wired),
               isWired);
  return wired;
}

Configs flowExtensions(const Matrix &flow) {
  Configs valid_configs;
  const int flow_inputs = flow.size();
  const int flow_outputs = flow[0].size();

  // Note: I assume out1 and out2 aren't both looped back to inputs; check to
  // see if this is valid later

  valid_configs.push_back(unwired_2_1_splitter);
  wire_2_inputs_1_fixedOutput(valid_configs, flow_outputs);
  for (int in1 = -1; in1 < flow_inputs; ++in1) {
    valid_configs.push_back({{in1}, {-1}});
    wire_1_fixedInput_1_output(valid_configs, in1, flow_outputs);
    for (int in2 = in1 + 1; in2 < flow_inputs; ++in2) {
      valid_configs.push_back({{in1, in2}, {-1}});
      wire_2_fixedInputs_1_output(valid_configs, in1, in2, flow_outputs);
    }
  }

  // Drop circular dependencies
  for (int j = valid_configs.size() - 1; j >= 0; --j) {
    const auto [input_wiring, output_wiring] = valid_configs[j];
    // If there's a new input, we're fine
    if (!isWired(input_wiring[0])) {
      continue;
    }
    bool circular = true;
    for (auto current_input : input_wiring) {
      for (auto test_output : range(flow_outputs)) {
        // Check that this isn't a new output of the splitter
        if (contains(output_wiring, test_output)) {
          continue;
          // Check if this input is non-circular
        } else if (flow[current_input][test_output] != 0) {
          circular = false;
        }
      }
    }

    if (circular) {
      valid_configs.erase(valid_configs.begin() + j);
    }
  }

  return valid_configs;
}