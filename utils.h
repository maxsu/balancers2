// utils.h
// Utilities in the system domain (like logging)
#pragma once
#include "utils.cpp"

// Log a string to console
void log(string message);

// Log a vector<double> to console
void log(vector<double> row);

// Throw exception if index is out of bounds of vector
template <class T>
void vectorGuard(vector<T> v, int index);