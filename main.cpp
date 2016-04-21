#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <fstream>
#include <string>

void MemoryManager();
struct Process
{
	int id;
	int arrivalTime;
	int completionTime;
	int numPieces;
	int totalAddressSpace;
};

int main()
{
	long memSize = 0;
	int pageSize = 0;
	Process* processArray;
	int memPiece = 0;
	std::string fileName;
	std::ifstream input;
	std::ofstream output;

	//Prompt user for both the memory size and page size
	std::cout << "Enter the memory size (kB): ";
	std::cin >> memSize;
	std::cout << "Enter the page size (kB): ";
	std::cin >> pageSize;

	if(pageSize > memSize)
	{
		std::cout << "Page size cannot be larger than the memory size. Restart the program.\n";
		exit(EXIT_FAILURE);
	}

	//Prompt user for the input file
	std::cout << "Enter the input file name (including file extension): ";
	std::cin >> fileName;

	//Begin reading the input file
	input.open(fileName, std::ifstream::in);

	//Grab first char to determine how many processes
	char c = input.get();
	std::cout << c << endl;
	int counter = 0;
	processArray = new Process[int(c)];

	while(!input.eof())
	{
		input >> processArray[counter].id;
		input >> processArray[counter].arrivalTime >> 
			processArray[counter].completionTime;
		input >> processArray[counter].numPieces;
		for(int i = 0; i < processArray[counter].numPieces; i++)
		{
			input >> memPiece;
			processArray[counter].totalAddressSpace += memPiece;
		}
		counter++;
	}

	for(int i = 0; i < int(c); i++)
	{
		std::cout << "ProcessID: " << processArray[i].id <<std:: endl;
		std::cout << "Arrival Time: " << processArray[i].arrivalTime <<std::endl;
		std::cout << "Completion Time: " << processArray[i].completionTime << std::endl;
		std::cout << "Total Address Space: " << processArray[i].totalAddressSpace << std::endl;
	}
	return 0;
}

void MemoryManager()
{
}
