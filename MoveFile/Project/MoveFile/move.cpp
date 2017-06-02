//move.cpp
//Simple program which moves specified file to specified localization.
//It should be executed from command line in order to provide required arguments.
//Return value of 0 means that file has been moved successfully and 1 otherwise.
//1st argument is a path (relative or absolute) to file we want to move and 2nd argument is a path to directory (relative or absolute).
//If in the destination already exists file with the same name as our source file, user will be asked if he wants to overwrite it or no.

#include <iostream>
#include <string>
#include <filesystem>

using namespace std;
using experimental::filesystem::is_regular_file;
using experimental::filesystem::is_directory;
using experimental::filesystem::exists;
using experimental::filesystem::rename;

string getBaseName(string name);

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		cout << argv[0] << ": Invalid number of arguments.";
		return 1;
	}

	string source = argv[1];
	string dest = argv[2];

	if (!is_regular_file(source))
	{
		cout << argv[0] << ": Cannot find specified file.";
		return 1;
	}

	if (!is_directory(dest))
	{
		cout << argv[0] << ": Cannot find specified directory.";
		return 1;
	}

	string file = getBaseName(source);
	string newFilePath = dest + "/" + file;

	if (exists(newFilePath))
	{
		cout << argv[0] << ": File named '" << file << "' already exists in specified directory. Overwrite?[y/n]: ";
		char input;
		cin >> input;
		if (input != 'y')
			return 1;
	}

	try
	{
		rename(source, newFilePath);
	}
	catch (exception e)
	{
		cout << argv[0] << ": " <<  e.what();
		return 1;
	}

	return 0;
}

string getBaseName(string name)
{
	auto index = name.find_last_of("/\\");
	if (index == string::npos)
		return name;
	else
		return name.substr(index + 1, name.size() - 1);
}