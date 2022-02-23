// network_tools.h
// Tools for the Network/Flow domain
#pragma once
#include "network_tools.cpp"

// Generate a row of zeros
Row zeroRow(int size);

// Generate a row with a single 1 entry
Row oneRow(int size, int one_position);

// Add two rows together
Row rowAdd(Row new_row, Row input_node_row);

// Multiply a row by a double
Row rowMultiply(Row row, double multiplier);

// Generate an identity matrix flow
Flow identityFlow(int size);

// Extract a column from a flow
Row getColumn(Flow flow, int column_position);

// Generate an empty network
Network emptyNetwork(int size);

// Link two nodes in a network
void link(Network& nodes, int source, int target);

// Find a node's index in a network
int nodeNum(Network nodes, Node* node);
