// compute the output of a splitter network

#pragma once
#include "exists_balancer.cpp"

vector<vector<double>> transpose(vector<vector<double>> matrix);

// network[i] is flow from inputs to i-th output
// splitter_inputs has an entry of -1 for a new input that's not already part of the network
// splitter_outputs has an entry of -1 for a new output
// There must be at least one output
// Network must have at least one input
vector<vector<double>> addSplitter(vector<vector<double>> network, vector<int> splitter_inputs, vector<int> splitter_outputs);

bool existsBalancer(int input_size, int output_size, int max_num_splitters);