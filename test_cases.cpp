#include "types.h"

struct TestNet {
  Network network;
  Row ratios;
};

// Set up a 17 node test network
TestNet testnet_A() {
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

  Network network = {s0, s1,  s2,  s3,  s4,  s5,  s6,  s7, s8,
                     s9, s10, s11, s12, s13, s14, s15, s16};

  Row ratios = {1,
                0.75641024112701416,
                0.5128205418586731,
                0.50549453496932983,
                0.25641027092933655,
                0.26923078298568726,
                0.12820513546466827,
                0.25274726748466492,
                0.31684982776641846,
                0.25457876920700073,
                0.12637363374233246,
                0.22161172330379486,
                0.380952388048172,
                0.380952388048172,
                0.22161172330379486,
                0.12820513546466827,
                0.26923078298568726};

  TestNet result = {network, ratios};
  return result;
}

// Set up a 8 node test network
TestNet testnet_B() {
  Node* s1 = new Node;
  Node* s2 = new Node;
  Node* s3 = new Node;
  Node* s4 = new Node;
  Node* s5 = new Node;
  Node* s6 = new Node;
  Node* s7 = new Node;
  Node* s8 = new Node;

  // inputs
  s3->inputs.push_back(s1);
  s3->inputs.push_back(s2);
  s4->inputs.push_back(s5);
  s5->inputs.push_back(s4);
  s5->inputs.push_back(s3);
  s6->inputs.push_back(s4);
  s6->inputs.push_back(s3);
  s7->inputs.push_back(s5);
  s8->inputs.push_back(s6);

  // outputs
  s1->outputs.push_back(s3);
  s2->outputs.push_back(s3);
  s3->outputs.push_back(s5);
  s3->outputs.push_back(s6);
  s4->outputs.push_back(s5);
  s4->outputs.push_back(s6);
  s5->outputs.push_back(s4);
  s5->outputs.push_back(s7);
  s6->outputs.push_back(s8);

  Network network = {s1, s2, s3, s4, s5, s6, s7, s8};
  Row ratios = {1,
                0,
                0.5,
                0.1666666716337204,
                0.3333333432674408,
                0.66666668653488159,
                0.3333333432674408,
                0.66666668653488159};

  TestNet result = {network, ratios};
  return result;
}