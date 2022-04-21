// Tools for the Network/Matrix domain

#pragma once

#include "types.hpp"

//////////////////////////////
// Row operations
//////////////////////////////

// Generate a row with constant values
Row constRow(int size, double value);

// Generate a row of zeros
Row zeroRow(int size);

// Generate a row with a single 1 entry
Row oneRow(int size, int one_position);

// Concatenate two rows
Row rowConcat(Row A, Row B);

// Add two rows together
Row rowAdd(Row new_row, Row input_node_row);

// Multiply a row by a double
Row rowMultiply(Row row, double multiplier);

//////////////////////////////
// Matrix operations
//////////////////////////////

// Generate an identity matrix matrix
Matrix identityMatrix(int size);

bool isRectangular(Matrix network);

// Extract a column from a matrix
Row getColumn(Matrix matrix, int column_position);

// Transpose a matrix
Matrix transpose(Matrix matrix);

// Put matrix into lexicographic normal form
inline void sortMatrix(Matrix& matrix);

//////////////////////////////
// Network operations
//////////////////////////////

// Represents a splitter's connection to an existing network
using Wiring = vector<int>;

// A Pair of wirings for a splitter's input and output
using Config = vector<Wiring>;

// A collection of wirings building a network
using Configs = vector<Config>;

// Determine if a wiring value is a connected or unconnected (-1)
bool isWired(int connection);

// Filter the unwired connections from a wiring
Wiring wiredConnections(Wiring wiring);

// Generate an empty network
Network emptyNetwork(int size);

// Link two nodes in a network
void link(Network& nodes, int source, int target);

// Find a node's index in a network
int nodeNum(Network nodes, Node* node);

// network[i] is the flow from inputs to i-th output.
// splitter_inputs has an entry of -1 for a new input that's not already part of
// the network. splitter_outputs has an entry of -1 for a new output There must
// be at least one output Network must have at least one input
Matrix addSplitterToFlow(Matrix network, Wiring splitter_inputs,
                         Wiring splitter_outputs);