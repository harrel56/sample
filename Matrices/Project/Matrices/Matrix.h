//Matrix.h
//Because Matrix is a template class all function definition can be found in this file.

#pragma once

using namespace std;

template<typename T>
class Matrix
{
public:

   /**
	*Default constructor.
	*Sets both size values to 0 and data pointer to nullptr.
	*/
	Matrix();

   /**
	*Parameterized constructor.
	*@param x the x size of matrix (number of columns).
	*@param y the y size of matrix (number of rows).
	*@param initVal initialization value of each element.
	*/
	Matrix(unsigned int x, unsigned int y, const T& initVal = {});

   /**
	*Copy constructor.
	*/
	Matrix(const Matrix& matrix);

   /**
	*Move constructor.
	*/
	Matrix(Matrix&& matrix);

   /**
	*Destructor.
	*/
	~Matrix();

   /**
	*Access column.
	*@param index index of column.
	*@return A whole column which in fact is a pointer to the first element in column which can be treated as an array in order to access any element.
	*/
	T* const operator[](unsigned int index) const;

   /**
	*Copy assign operator.
	*@param matrix constant reference to matrix which will be copied and assigned.
	*@return Reference to a matrix which has been assigned (*this).
	*/
	Matrix& operator=(const Matrix& matrix);

   /**
	*Move assign operator.
	*@param matrix r-value matrix which will be moved.
	*@return Reference to a matrix which has been assigned (*this).
	*/
	Matrix& operator=(Matrix&& matrix);

   /**
	*Addition.
	*@pre Matrices have to have exactly the same size.
	*@param rhs right-hand side constant reference to an object of a Matrix type.
	*@return new matrix created by addition of two other matrices.
	*/
	Matrix operator+(const Matrix& rhs);

   /**
	*Substraction.
	*@pre Matrices have to have exactly the same size.
	*@param rhs right-hand side constant reference to an object of a Matrix type.
	*@return new matrix created by substraction of two other matrices.
	*/
	Matrix operator-(const Matrix& rhs);

   /**
	*Multiplication.
	*@pre Number of columns in the first matrix has to be equal to number of rows in the second matrix.
	*@param rhs right-hand side constant reference to an object of a Matrix type.
	*@return new matrix created by multiplication of two other matrices.
	*/
	Matrix operator*(const Matrix& rhs);

   /**
	*Get columns number.
	*@return number of columns.
	*/
	unsigned int getNumberOfColumns() const;

   /**
	*Get rows number.
	*@return number of rows.
	*/
	unsigned int getNumberOfRows() const;

   /**
	*Get number of columns and rows.
	*@return pair in which first element is number of columns and second element is number of rows.
	*/
	pair<unsigned int, unsigned int> getSize() const;

   /**
	*Print matrix to standard output.
	*@param os ostream object.
	*@param matrix matrix to be printed.
	*@return modified ostream object reference.
	*/
	template<typename T>
	friend ostream& operator<<(ostream& os, const Matrix<T>& matrix);
private:
   /**
	*Two-dimensional array.
	*Contains all elements.
	*/
	T** elements;

   /**
	*Number of columns.
	*/
	unsigned int xSize;

   /**
	*Number of rows.
	*/
	unsigned int ySize;
};

template<typename T>
Matrix<T>::Matrix() : elements(nullptr), xSize(0), ySize(0) {}


template<typename T>
Matrix<T>::Matrix(unsigned int x, unsigned int y, const T& initVal)
{
	if (x < 1 || y < 1)
		throw invalid_argument("Matrix Constructor: Dimensions of matrix have to be greater than 0!");
	else
	{
		xSize = x;
		ySize = y;
	}

	elements = new T*[xSize];
	for (unsigned int col = 0; col < xSize; col++)
	{
		elements[col] = new T[ySize];
		for (unsigned int row = 0; row < ySize; row++)
		{
			elements[col][row] = initVal;
		}
	}
}

template<typename T>
Matrix<T>::Matrix(const Matrix<T>& matrix) : xSize(matrix.xSize), ySize(matrix.ySize)
{
	elements = new T*[xSize];
	for (unsigned int col = 0; col < xSize; col++)
	{
		elements[col] = new T[ySize];
		for (unsigned int row = 0; row < ySize; row++)
		{
			elements[col][row] = matrix.elements[col][row];
		}
	}
}

template<typename T>
Matrix<T>::Matrix(Matrix<T>&& matrix) : xSize(matrix.xSize), ySize(matrix.ySize)
{
	elements = matrix.elements;
	matrix.xSize = 0;
	matrix.ySize = 0;
	matrix.elements = nullptr;
}

template<typename T>
Matrix<T>::~Matrix()
{
	for (unsigned int col = 0; col < xSize; col++)
	{
		delete[] elements[col];
	}
	delete[] elements;
}

template<typename T>
T* const Matrix<T>::operator[](unsigned int index) const
{
	return elements[index];
}

template<typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& matrix)
{
	//Check if we are assigning matrix to itself - if yes - don't waste time on copy.
	if (this != &matrix)
	{
		if (elements != nullptr)
		{
			for (unsigned int col = 0; col < xSize; col++)
			{
				delete[] elements[col];
			}
			delete[] elements;
		}
		xSize = matrix.xSize;
		ySize = matrix.ySize;
		elements = new T*[xSize];
		for (unsigned int col = 0; col < xSize; col++)
		{
			elements[col] = new T[ySize];
			for (unsigned int row = 0; row < ySize; row++)
			{
				elements[col][row] = matrix.elements[col][row];
			}
		}
	}
	return *this;
}

template<typename T>
Matrix<T>& Matrix<T>::operator=(Matrix<T>&& matrix)
{
	//Check if we are assigning matrix to itself - if yes - don't waste time on move.
	if (this != &matrix)
	{
		if (elements != nullptr)
		{
			for (unsigned int col = 0; col < xSize; col++)
			{
				delete[] elements[col];
			}
			delete[] elements;
		}
		xSize = matrix.xSize;
		ySize = matrix.ySize;
		elements = matrix.elements;
		matrix.xSize = 0;
		matrix.ySize = 0;
		matrix.elements = nullptr;
	}
	return *this;
}

template<typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T>& rhs)
{
	if(elements == nullptr || rhs.elements == nullptr)
		throw invalid_argument("Matrix addition: Empty matrices!");
	if (xSize != rhs.xSize || ySize != rhs.ySize)
		throw invalid_argument("Matrix addition: Matrices have different size!");

	Matrix<T> result(xSize, ySize);
	for (unsigned int col = 0; col < xSize; col++)
	{
		for (unsigned int row = 0; row < ySize; row++)
		{
			result.elements[col][row] = elements[col][row] + rhs.elements[col][row];
		}
	}
	return result;
}

template<typename T>
Matrix<T> Matrix<T>::operator-(const Matrix<T>& rhs)
{
	if (elements == nullptr || rhs.elements == nullptr)
		throw invalid_argument("Matrix substraction: Empty matrices!");
	if (xSize != rhs.xSize || ySize != rhs.ySize)
		throw invalid_argument("Matrix substraction: Matrices have different size!");

	Matrix<T> result(xSize, ySize);
	for (unsigned int col = 0; col < xSize; col++)
	{
		for (unsigned int row = 0; row < ySize; row++)
		{
			result.elements[col][row] = elements[col][row] - rhs.elements[col][row];
		}
	}
	return result;
}

template<typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T>& rhs)
{
	if (elements == nullptr || rhs.elements == nullptr)
		throw invalid_argument("Matrix multiplication: Empty matrices!");
	if (xSize != rhs.ySize)
		throw invalid_argument("Matrix multiplication: Number of columns in the first matrix is not equal to the number of rows in the second matrix!");

	Matrix<T> result(rhs.xSize, ySize);
	for (unsigned int row = 0; row < ySize; row++)
	{
		for (unsigned int col1 = 0; col1 < rhs.xSize; col1++)
		{
			for (unsigned int col2 = 0; col2 < xSize; col2++)
			{
				result.elements[col1][row] += elements[col2][row] * rhs.elements[col1][col2];
			}
		}
	}
	return result;
}

template<typename T>
unsigned int Matrix<T>::getNumberOfColumns() const
{
	return xSize;
}

template<typename T>
unsigned int Matrix<T>::getNumberOfRows() const
{
	return ySize;
}

template<typename T>
pair<unsigned int, unsigned int> Matrix<T>::getSize() const
{
	return make_pair(xSize, ySize);
}

template<typename T>
ostream& operator<<(ostream& os, const Matrix<T>& matrix)
{
	if (matrix.elements != nullptr)
	{
		for (int row = 0; row < matrix.ySize; row++)
		{
			for (int col = 0; col < matrix.xSize; col++)
			{
				os << left << setw(3) << matrix.elements[col][row] << " ";
			}
			os << endl;
		}
	}
	else
		os << "Matrix is empty!" << endl;
	return os;
}