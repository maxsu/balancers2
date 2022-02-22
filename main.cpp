//  main.cpp
//  beltbalancers2

#include <iostream>
#include <vector>
using namespace std;

struct Node {
  vector<Node*> inputs;
  vector<Node*> outputs;
};

// Find the ratios given by a certain splitter network (as a double)
vector<vector<double>> outputRatios(vector<Node*> nodes, int num_inputs,
                                    int num_splitters, int num_outputs) {
  // Construct the vector of ratios, which will hold what each splitter outputs
  // in terms of the outputs of the others The [i][j] entry is the amount of
  // node i's output depends on node j's output Eventually, we want to reduce
  // everything to dependencies on the inputs Initially, this will just be the
  // trivial "this splitter outputs what it outputs" vector
  vector<vector<double>> flow;

  for (int i = 0; i < num_inputs + num_splitters + num_outputs; ++i) {
    vector<double> temp;
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
    vector<double> new_flow;
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
  Node* s0 = new Node;
  Node* s1 = new Node;
  Node* s2 = new Node;
  Node* s3 = new Node;
  Node* s4 = new Node;
  Node* s5 = new Node;
  Node* s6 = new Node;
  Node* s7 = new Node;
  Node* s8 = new Node;
  Node* s9 = new Node;
  Node* s10 = new Node;
  Node* s11 = new Node;
  Node* s12 = new Node;
  Node* s13 = new Node;
  Node* s14 = new Node;
  Node* s15 = new Node;
  Node* s16 = new Node;

  // inputs
  s1->inputs.push_back(s0);
  s1->inputs.push_back(s2);
  s2->inputs.push_back(s1);
  s2->inputs.push_back(s5);
  s3->inputs.push_back(s1);
  s3->inputs.push_back(s9);
  s4->inputs.push_back(s2);
  s5->inputs.push_back(s8);
  s5->inputs.push_back(s11);
  s6->inputs.push_back(s4);
  s7->inputs.push_back(s3);
  s8->inputs.push_back(s3);
  s8->inputs.push_back(s6);
  s9->inputs.push_back(s4);
  s9->inputs.push_back(s7);
  s10->inputs.push_back(s7);
  s11->inputs.push_back(s10);
  s11->inputs.push_back(s8);
  s12->inputs.push_back(s9);
  s12->inputs.push_back(s10);
  s13->inputs.push_back(s12);
  s14->inputs.push_back(s11);
  s15->inputs.push_back(s6);
  s16->inputs.push_back(s5);

  // outputs
  s0->outputs.push_back(s1);
  s2->outputs.push_back(s1);
  s1->outputs.push_back(s2);
  s5->outputs.push_back(s2);
  s1->outputs.push_back(s3);
  s9->outputs.push_back(s3);
  s2->outputs.push_back(s4);
  s8->outputs.push_back(s5);
  s11->outputs.push_back(s5);
  s4->outputs.push_back(s6);
  s3->outputs.push_back(s7);
  s3->outputs.push_back(s8);
  s6->outputs.push_back(s8);
  s4->outputs.push_back(s9);
  s7->outputs.push_back(s9);
  s7->outputs.push_back(s10);
  s10->outputs.push_back(s11);
  s8->outputs.push_back(s11);
  s9->outputs.push_back(s12);
  s10->outputs.push_back(s12);
  s12->outputs.push_back(s13);
  s11->outputs.push_back(s14);
  s6->outputs.push_back(s15);
  s5->outputs.push_back(s16);

  vector<vector<double>> flow =
      outputRatios({s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12, s13,
                    s14, s15, s16},
                   1, 12, 4);

  cout << "done"
       << "\n";
}