#include <vector>
using namespace std;

struct Node {
  vector<Node*> inputs;
  vector<Node*> outputs;
};