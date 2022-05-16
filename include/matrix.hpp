// Tools for the Matrix domain

#pragma once

#include <vector>

/////////////////////////////
// Data Types
/////////////////////////////

using Row = std::vector<double>;

using Matrix = std::vector<Row>;

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

// Generate a matrix with identical rows
Matrix constRowMatrix(int rows, Row base_row);

// Tell whether a matrix has rows of identical length
bool isRectangular(Matrix network);

// Extract a column from a matrix
Row getColumn(Matrix matrix, int column_position);

// Transpose a matrix
Matrix transpose(Matrix matrix);

// Put matrix into lexicographic normal form
void sortMatrix(Matrix& matrix);