// tests/test_cases.cpp
// Examples of networks and their flow ratios
#pragma once
#include "test_cases.cpp"

// A 2 node trivial link
TestNet trivialLink();

// A 1 -> 2 splitter; 4 nodes
TestNet splitter1_2();

// A 2 -> 1 splitter; 4 nodes
TestNet splitter2_1();

// A 2 -> 2 splitter; 5 nodes
TestNet splitter2_2();

// A 10 node test network; 3 -> 3 Balancer
TestNet balancer3_3();

// An 8 node test network
TestNet testnetA();

// A 17 node test network
TestNet testnetB();