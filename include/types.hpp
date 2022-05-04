// Datatypes and aliases

#pragma once
#include <string>
#include <vector>

struct Node {
  std::vector<Node *> inputs;
  std::vector<Node *> outputs;
};

using Row = std::vector<double>;
using Matrix = std::vector<Row>;
using Network = std::vector<Node *>;

struct TestNet {
  std::string name;
  Network network;
  Row ratios;
};