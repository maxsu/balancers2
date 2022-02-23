// types.h
// Datatypes and aliases
#pragma once
#include <vector>
using namespace std;

struct Node {
  vector<Node *> inputs;
  vector<Node *> outputs;
};

using Row = vector<double>;
using Flow = vector<Row>;
using Network = vector<Node *>;

struct TestNet {
  Network network;
  Row ratios;
};