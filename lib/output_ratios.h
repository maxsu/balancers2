// compute the output of a splitter network

#pragma once
#include "output_ratios.cpp"

// Find the ratios given by a certain splitter network (as a double).
// Construct the vector of ratios, which will hold what each splitter outputs
// in terms of the outputs of the others. The [i][j] entry is the amount of
// node i's output that depends  on node j's output. Eventually, we want to
// reduce everything to dependencies on the inputs. Initially, this will just be
// the trivial "this splitter outputs what it outputs" vector.
Matrix outputRatios(Network nodes, int inputs, int splitters, int outputs);