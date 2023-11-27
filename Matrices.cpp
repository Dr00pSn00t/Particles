#include "Matrices.h"

namespace Matrices
{
	Matrix::Matrix(int _rows, int _cols)
	{
		rows = _rows;
		cols = _cols;

		a.resize(rows, vector<double>(cols, 0));
	}

	// usage: c = a + b;
	Matrix operator+(const Matrix& a, const Matrix& b)
	{
		int rows = a.getRows();
		int cols = a.getCols();

		//if (!(rows == b.getRows() && cols == b.getCols())) throw runtime_error("Error: dimensions must agree");

		Matrix out(rows, cols);

		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				out(i, j) = a(i, j) + b(i, j);
			}
		}

		return out;
	}

	// usage: c = a * b;
	Matrix operator*(const Matrix& a, const Matrix& b)
	{
		int colsA = a.getCols();
		int rowsB = b.getRows();

		//if (!(colsA == rowsB)) throw runtime_error("Error: dimensions must agree");

		int rowsA = a.getRows();
		int colsB = b.getCols();

		Matrix out(rowsA, colsB);
		for (int i = 0; i < rowsA; i++)
		{
			for (int j = 0; j < colsB; j++)
			{
				double total = 0.0;
				for (int k = 0; k < colsA; k++)
				{
					total += a(i, k) * b(k, j);
				}
				out(i, j) = total;
			}
		}
		return out;
	}

	// usage: a == b
	bool operator==(const Matrix& a, const Matrix& b)
	{
		int rowA = a.getRows();
		int rowB = b.getRows();
		if (!(rowA == rowB)) return false;

		int colA = a.getCols();
		int colB = b.getCols();
		if (!(colA == colB)) return false;

		for (int i = 0; i < rowA; i++)
		{
			for (int j = 0; j < rowB; j++)
			{
				if (!(abs(a(i, j) - b(i, j)) < 0.001)) return false;
			}
		}

		return true;
	}

	// usage: a != b
	bool operator!=(const Matrix& a, const Matrix& b)
	{
		return !(a == b);
	}

	// Output matrix.
	// Separate columns by ' ' and rows by '\n'
	ostream& operator<<(ostream& os, const Matrix& a)
	{
		int rows = a.getRows();
		int cols = a.getCols();

		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				if (j != 0) os << " ";
				os << setw(10) << a(i, j);
			}
			os << endl;
		}

		return os;
	}

	RotationMatrix::RotationMatrix(double theta) : Matrix(2, 2)
	{
		// cos(theta) - sin(theta)
		// sin(theta)   cos(theta)
		a[0][0] = cos(theta);
		a[0][1] = -sin(theta);
		a[1][0] = sin(theta);
		a[1][1] = cos(theta);
	}

	ScalingMatrix::ScalingMatrix(double scale) : Matrix(2, 2)
	{
		// scale   0
		// 0       scale
		a[0][0] = scale;
		a[0][1] = 0;
		a[1][0] = scale;
		a[1][1] = 0;
	}

	TranslationMatrix::TranslationMatrix(double xShift, double yShift, int nCols) : Matrix(2, nCols)
	{
		// xShift  xShift  xShift  ...
		// yShift  yShift  yShift  ...
		for (int i = 0 i < nCols; i++)
		{
			a[0][i] = xShift;
			a[1][i] = yShift;
		}
	}
}