// Wiring Stuff

#pragma once

#include "matrix.hpp"
#include "network.hpp"

//////////////////////////////
// Data Types
//////////////////////////////

// Represents a splitter's connection to an existing network
using Wiring = std::vector<int>;

// Represent's a splitter's input and output wirings
using Config = std::pair<Wiring, Wiring>;

// A collection of wirings building a network
using Configs = std::vector<Config>;

//////////////////////////////
// Constants
//////////////////////////////

const Config unwired_2_1_splitter = {{-1, -1}, {-1}};

//////////////////////////////
// Wiring Functions
//////////////////////////////

// Tells if a wiring connection is wired or not
bool isWired(int connection);

// Generate simple wiring runs
void wire_2_inputs_1_fixedOutput(Configs &configs, int n);
void wire_2_fixedInputs_1_output(Configs &configs, int in1, int in2, int n);
void wire_1_fixedInput_1_output(Configs &configs, int in1, int n);

// Filter the unwired connections from a wiring
Wiring wiredConnections(Wiring wiring);

// Get all feasible splitter-extensions of a flow
Configs flowExtensions(const Matrix &flow);