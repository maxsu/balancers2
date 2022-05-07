// Tools for the Network/Matrix domain

#include "matrix.hpp"

#include <algorithm>

#include "vector_guard.hpp"

Row constRow(const int size, const double value) { return Row(size, value); }

Row zeroRow(int size) { return constRow(size, 0); }

Row oneRow(int size, int one_position) {
  Row one_row = zeroRow(size);
  vectorGuard(one_row, one_position);
  one_row[one_position] = 1;
  return one_row;
}

Row rowConcat(Row A, Row B) {
  A.insert(A.end(), B.begin(), B.end());
  return A;
}

Row rowAdd(Row A, Row B) {
  if (A.size() != B.size()) {
    throw "Row sizes mismatch";
  }

  Row C = zeroRow(A.size());
  for (int i = 0; i < A.size(); i++) {
    C[i] = A[i] + B[i];
  }
  return C;
}

Row rowMultiply(Row row, double multiplier) {
  for (int i = 0; i < row.size(); ++i) {
    row[i] *= multiplier;
  }
  return row;
}

Matrix identityMatrix(int size) {
  Matrix identity_matrix;
  for (int i = 0; i < size; ++i) {
    identity_matrix.push_back(oneRow(size, i));
  }
  return identity_matrix;
}

Matrix constRowMatrix(const int rows, const Row base_row) {
  return Matrix(rows, base_row);
}

bool isRectangular(Matrix network) {
  int M = network[0].size();
  bool is_rectangular = true;

  for (auto row : network) {
    if (row.size() != M) {
      is_rectangular = false;
    }
  }

  return is_rectangular;
}

Row getColumn(Matrix matrix, int column_position) {
  vectorGuard(matrix[0], column_position);
  Row column;
  int m = matrix.size();
  for (int j = 0; j < m; j++) {
    column.push_back(matrix[j][column_position]);
  }
  return column;
}

Matrix transpose(Matrix matrix) {
  Matrix transpose_matrix;

  int m = matrix.size();

  if (m == 0) {
    return matrix;
  }

  int n = matrix[0].size();

  for (int i = 0; i < n; ++i) {
    Row column = getColumn(matrix, i);
    transpose_matrix.push_back(column);
  }

  return transpose_matrix;
}

void sortMatrix(Matrix& matrix) {
  while (true) {
    Matrix old_matrix = matrix;

    // Sort rows
    sort(matrix.begin(), matrix.end());

    // Sort columns
    matrix = transpose(matrix);
    sort(matrix.begin(), matrix.end());
    matrix = transpose(matrix);

    // Finish when matrix is invariant
    if (matrix == old_matrix) {
      break;
    }
  }
}