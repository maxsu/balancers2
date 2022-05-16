// Computes the list of all flows possible with a certain number of splitters

#include "exists_balancer.hpp"

#include <set>
#include <vector>

#include "flow.hpp"
#include "iters.hpp"
#include "network.hpp"
#include "wiring.hpp"

using std::set;

bool existsBalancer(const int input_size, const int output_size,
                    const int max_num_splitters) {
  set<Matrix> flows;
  flows.insert({{1}});

  for (auto i : range(max_num_splitters)) {
    set<Matrix> new_flows;

    for (auto flow : flows) {
      for (auto config : flowExtensions(flow)) {
        new_flows.insert(addSplitterToFlow(flow, config));
      }
    }
    flows.insert(new_flows.begin(), new_flows.end());
  }

  // Check if it's a balancer
  return contains(flows, balancerFlow(input_size, output_size));
}
