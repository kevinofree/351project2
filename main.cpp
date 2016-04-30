#include <iostream>
#include <math.h>
#include <vector>
#include <fstream>
#include <string>

struct Process
{
	int pid;
	int arrivalTime;
	int startTime;
	int timeToComplete;
	int numOfPieces;
	int totalAddressSpace;
	int turnAroundTime;
	int waitTime;
	int framesNeeded;
};

int main()
{
	// Input variables for the user
	Process* processArray;
	int memSize, pageSize, frameCount, arraySize, freeFrames;
	int completeCounter = 0;
	int time = 0;
	std::string iFileName, oFileName;
	std::ifstream input;
	std::ofstream output;

	// Ask for user input
	std::cout << "Enter the memory size (kB): ";
	std::cin >> memSize;
	std::cout << "Enter the page size (kB): ";
	std::cin >> pageSize;

	std::cin.ignore();
	std::cout << "Enter the input file name (including file extension): ";
	std::getline(std::cin, iFileName);

	std::cin.ignore();
	std::cout << "Enter the output file name (including file extension)";
	std::getline(std::cin, oFileName);

	// Open the input file and create the output file
	input.open(iFileName.c_str(), std::ifstream::in);
	output.open(oFileName.c_str(), std::ofstream::out);

	// Create the vectors for memArray and pageArray for printing
	arraySize = memSize / pageSize;
	freeFrames = arraySize;
	std::vector<int> memArray(arraySize, 0);
	std::vector<int> pageArray(arraySize, 0);

	// Begin inputting info from the input file and create an array of structures
	int numOfProcesses = 0;
	input >> numOfProcesses;
	processArray = new Process[numOfProcesses];

	int counter = 0;
	int memPiece = 0;
	while (!input.eof())
	{
		input >> processArray[counter].pid;
		input >> processArray[counter].arrivalTime;
		input >> processArray[counter].timeToComplete;
		input >> processArray[counter].numOfPieces;
		for (int i = 0; i < processArray[counter].numOfPieces; i++)
		{
			input >> memPiece;
			processArray[counter].totalAddressSpace += memPiece;
		}
		processArray[counter].turnAroundTime = 0;
		processArray[counter].waitTime = 0;
		processArray[counter].framesNeeded = int(ceil(double(processArray[counter].totalAddressSpace) / pageSize));
	}
	input.close(); // close the input file

	// BEGIN MEMORY MANAGEMENT
	do
	{	// Check starting times
		// Process arrives and is placed in input queue
		for (int i = 0; i < numOfProcesses - 1; i++)
		{
			if (processArray[i].arrivalTime <= time && freeFrames >= processArray[i].framesNeeded)
			{
				std::cout << "Process " << processArray[i].pid << "Arrives\n";
				// PLACE INTO INPUT QUEUE
				// ALLOCATE INTO MEMORY
			}
		}


		// Check for Process completions

		// Check for the full completion

		// Increment the global time
		time++;
	} while (completeCounter != numOfProcesses);

	// Close the output file
	output.close();
	return 0;
}
