// Tools for the Network/Matrix domain

#pragma once

#include "matrix.hpp"
#include "network.hpp"
#include "wiring.hpp"

//////////////////////////////
// Flow operations
//////////////////////////////

// Generate a balancer flow
Matrix balancerFlow(int input_num, int output_num);

// Calculate the flow of a splitter network.
//
// The [i][j] entry of is the amount of node i's output that depends  on node
// j's input. Eventually, we want to reduce everything to dependencies on the
// inputs. Initially, this will just be the trivial "this splitter outputs what
// it outputs" vector.
Matrix networkToFlow(Network network);

// Extend a flow with a new splitter.
//
// flow[i] is the flow from inputs to i-th output.
// splitter_inputs has an entry of -1 for a new input that's not already part of
// the network.
// splitter_outputs has an entry of -1 for a new output
// There must be at least one output.
// Network must have at least one input.
Matrix addSplitterToFlow(Matrix flow, Config splitter_config);