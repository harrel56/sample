//main.cpp
//For presentation purposes the Matrix class template has been parameterized with int type.

#include <iostream>
#include <stdlib.h> //Clear console - it may not work properly on other OS from Windows
#include <iomanip> //Output manipulation like setw(int)
#include "Matrix.h"

using namespace std;

/**
*Print all of the menu options.
*/
void printMenu();

/**
*Construct new matrix from user input.
*@return constructed matrix.
*/
template<typename T>
Matrix<T> createNewMatrix();

int main()
{
	Matrix<int> m1; //First Matrix variable
	Matrix<int> m2; //Second Matrix variable
	Matrix<int> mResult; //Matrix variable which will store operation outcomes
	char inputVariable; //Variable used to get user input
	bool exit = false;

	printMenu();
	while (!exit)
	{
		cin >> inputVariable;
		system("cls");
		printMenu();

		switch (inputVariable)
		{
		case '1':
			m1 = createNewMatrix<int>();
			system("cls");
			printMenu();
			cout << "Successfully created new matrix!" << endl;
			break;
		case '2':
			m2 = createNewMatrix<int>();
			system("cls");
			printMenu();
			cout << "Successfully created new matrix!" << endl;
			break;
		case '3':
			cout << m1;
			break;
		case '4':
			cout << m2;
			break;
		case '5':
			cout << mResult;
			break;
		case '6':
			try
			{
				mResult = m1 + m2;
				cout << "Operation successful!";
			}
			catch (exception e)
			{
				cout << e.what() << endl;
			}
			break;
		case '7':
			try
			{
				mResult = m1 - m2;
				cout << "Operation successful!";
			}
			catch (exception e)
			{
				cout << e.what() << endl;
			}
			break;
		case '8':
			try
			{
				mResult = m2 - m1;
				cout << "Operation successful!";
			}
			catch (exception e)
			{
				cout << e.what() << endl;
			}
			break;
		case '9':
			try
			{
				mResult = m1 * m2;
				cout << "Operation successful!";
			}
			catch (exception e)
			{
				cout << e.what() << endl;
			}
			break;
		case '0':
			try
			{
				mResult = m2 * m1;
				cout << "Operation successful!";
			}
			catch (exception e)
			{
				cout << e.what() << endl;
			}
			break;
		case 'q':
			exit = true;
			break;
		default:
			cout << "Wrong input! Try again." << endl;
			break;
		}

		//In case if more chars were provided - ignore them
		cin.clear();
		cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
	}

	
	return 0;
}

void printMenu()
{
	cout << "There are 3 Matrix variables - M1, M2 and MResult" << endl
		<< "(1) Create new matrix and assign it to M1" << endl
		<< "(2) Create new matrix and assign it to M2" << endl
		<< "(3) Print M1" << endl
		<< "(4) Print M2" << endl
		<< "(5) Print MResult" << endl
		<< "(6) MResult = M1 + M2" << endl
		<< "(7) MResult = M1 - M2" << endl
		<< "(8) MResult = M2 - M1" << endl
		<< "(9) MResult = M1 * M2" << endl
		<< "(0) MResult = M2 * M1" << endl
		<< "(q) Exit" << endl << endl;
}

template<typename T>
Matrix<T> createNewMatrix()
{
	unsigned int cols;
	unsigned int rows;

	bool readSuccess = false;
	while(!readSuccess)
	{
		cin.clear();
		cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
		cout << "Specify the number of columns: ";
		cin >> cols;
		if (cin.good())
			readSuccess = true;
	}

	readSuccess = false;
	while(!readSuccess)
	{
		cin.clear();
		cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
		cout << "Specify the number of rows: ";
		cin >> rows;
		if (cin.good())
			readSuccess = true;
	}

	Matrix<T> result(cols, rows);
	for (unsigned int row = 0; row < rows; row++)
	{
		readSuccess = false;
		while(!readSuccess)
		{
			cin.clear();
			cin.ignore(numeric_limits<std::streamsize>::max(), '\n');
			cout << "Insert the row number " << row + 1 << " (elements separated by spaces): ";
			for (unsigned int col = 0; col < cols; col++)
			{
				cin >> result[col][row];
			}
			if (cin.good())
				readSuccess = true;
		}
	}
	return result;
}