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
void processQueuePrint(std::ofstream&, std::vector<int>);
void timeStampPrint(std::ofstream&, Process*, int, int);
void memMapPrint(std::ofstream&, Process, int, int, std::vector<int>, std::vector<int>&);

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
	std::vector<Process> runningProcessQueue;

	// Ask for user input
	std::cout << "Enter the memory size (kB): ";
	std::cin >> memSize;
	std::cout << "Enter the page size (kB): ";
	std::cin >> pageSize;

	std::cin.ignore();
	std::cout << "Enter the input file name (including file extension): ";
	std::getline(std::cin, iFileName);
	std::cout << "Enter the output file name (including file extension): ";
	std::getline(std::cin, oFileName);

	// Open the input file and create the output file
	input.open(iFileName.c_str(), std::ifstream::in);
	output.open(oFileName.c_str(), std::ofstream::out);

	// Create the vectors for memArray and pageArray for printing
	arraySize = memSize/pageSize;
	freeFrames = arraySize;
	std::vector<int> memArray (arraySize,0); // Used for printing memory map
	std::vector<int> pageArray (arraySize,0); // Used for printing pages in the memory
	std::vector<int> outputProcessQueue; // Used to print input queue

	// Begin inputting info from the input file and create an array of structures
	int numOfProcesses = 0;
	input >> numOfProcesses;
	processArray = new Process[numOfProcesses];

	int counter = 0;
	int memPiece = 0;
	for(counter; counter < numOfProcesses; counter++)
	{
		input >> processArray[counter].pid;
		input >> processArray[counter].arrivalTime;
		input >> processArray[counter].timeToComplete;
		input >> processArray[counter].numOfPieces;
		for(int i = 0; i < processArray[counter].numOfPieces; i++)
		{
			input >> memPiece;
			processArray[counter].totalAddressSpace += memPiece;
		}
		processArray[counter].turnAroundTime = 0;
		processArray[counter].waitTime = 0;
		processArray[counter].startTime = -1;
		processArray[counter].framesNeeded = int(ceil(double(processArray[counter].totalAddressSpace) / pageSize));
	}
	input.close(); // close the input file

// BEGIN MEMORY MANAGEMENT
	int currProcessID;
	int numOfFrames;
	do
	{	// Check time and print time stamp of major events
		timeStampPrint(output, processArray, numOfProcesses, time);

		// Process arrives and is placed in input queue
		for(int b = 0; b < numOfProcesses; b++)
			if(processArray[b].arrivalTime == time)
			{
				output << "Process " << processArray[b].pid << " Arrives\n";
				outputProcessQueue.push_back(processArray[b].pid);
				runningProcessQueue.push_back(processArray[b]);
				processQueuePrint(output, outputProcessQueue);
			}

		// Allocate in the memory if there is enough frames for the process
		int c = 0;
		while(!runningProcessQueue.empty())
		{
			if(runningProcessQueue[c].framesNeeded <= freeFrames)
			{
				// ALLOCATE THE PROCESS TO MEMORY IF THERE IS ENOUGH FRAMES
				currProcessID = runningProcessQueue[c].pid;
				output << "\tMM moves Process " << currProcessID << " to memory\n";
				freeFrames -= runningProcessQueue[c].framesNeeded;
				for(int d = 0; d < runningProcessQueue[c].framesNeeded; d++)
				{
					for(int e = 0; e < arraySize; e++)
						if(memArray[e] == 0)
						{
							memArray[e] = runningProcessQueue[c].pid;
							break;
						}
				}
				// UPDATE THE START TIME OF THE PROCESS
				runningProcessQueue[c].startTime = time;
				processArray[currProcessID-1].startTime = time;
				// REMOVE PROCESS FROM THE QUEUE
				runningProcessQueue.erase(runningProcessQueue.begin() + c);
				outputProcessQueue.erase(outputProcessQueue.begin() + c);
				// PRINT OUT THE QUEUE AND MEMORY MAP
				processQueuePrint(output, outputProcessQueue);
				memMapPrint(output, processArray[currProcessID-1], arraySize, pageSize, memArray, pageArray);
			}
			else if(runningProcessQueue[c].framesNeeded > freeFrames)
			{
				c++;
				int qS = runningProcessQueue.size();
				if(runningProcessQueue.begin() + qS == runningProcessQueue.end())
					break;
			}
		}

		// Check for Process completions
		for(int f = 0; f < numOfProcesses; f++)
		{
			if(processArray[f].startTime != -1 && processArray[f].timeToComplete + processArray[f].startTime == time)
			{
				// INCREMENT THE COMPLETE COUNTER
				completeCounter++;
				/* LOOP THROUGH THE MEMORY AND EMPTY THE ENTRIES WHERE
				 THE ENTRY IS EQUAL TO THE COMPLETE PROCESS ID */
				/* INCREMENT THE NUMBER OF FREE FRAMES FOR EVERY ENTRY
				REMOVED FROM MEMORY */
				for(int g = 0; g < arraySize; g++)
				{
					if(memArray[g] == processArray[f].pid)
					{
						memArray[g] = 0;
						freeFrames++;
					}
				}
				// PRINT OUT MEMORY MAP
				output << "Process " << processArray[f].pid << " Completes\n";
				memMapPrint(output, processArray[f], arraySize, pageSize, memArray, pageArray);
				// CALCULATE THE TURNAROUND TIME FOR THE PROCESS
				processArray[f].turnAroundTime = processArray[f].timeToComplete
				+ (processArray[f].startTime - processArray[f].arrivalTime);
			}
		}

		// Increment the global time
		time++;

	}while(completeCounter != numOfProcesses);

	// Calculate the total and average turn around time
	int totalTurnAroundTime = 0;
	double averageTurnAroundTime = 0.0;
	for(int h = 0; h < numOfProcesses; h++)
		totalTurnAroundTime += processArray[h].turnAroundTime;
	averageTurnAroundTime = double(totalTurnAroundTime)/numOfProcesses;
	output << "Total Turn Around Time: " << totalTurnAroundTime << std::endl;
	output << "Average Turn Around Time: " << averageTurnAroundTime << std::endl;

	// Close the output file
	output.close();

	delete[] processArray;
	return 0;
}

void processQueuePrint(std::ofstream& output, std::vector<int> queue)
{
	output << "\tInput Queue: [";
	if(queue.empty())
		output << "]\n";
	else
		for(int i = 0; i < queue.size(); i++)
		{
			if(i == queue.size() - 1)
				output << queue[i] << "]\n";
			else
				output << queue[i] << " ";
		}
}

void timeStampPrint(std::ofstream& output, Process* p, int n, int t)
{
	for(int a = 0; a < n; a++)
	{
		// PROCESS ARRIVES, PROCESS BEGINS, PROCESS COMPLETES
		if(p[a].arrivalTime == t ||
		(p[a].startTime + p[a].timeToComplete == t && p[a].startTime != -1))
		{
			output << "t = " << t << ":\n";
			break;
		}
	}
}

void memMapPrint(std::ofstream& output, Process p, int size, int pageSz, std::vector<int> mem, std::vector<int>& pg)
{
	int pageCounter = 0;
	output << "Memory Map: \n";
	for(int j = 0; j < size; j++)
	{
		if(mem[j] != 0)
		{
			if(p.pid == mem[j])
			{
				pageCounter++;
				pg[j] = pageCounter;
			}
			output << "\t" << pageSz*j << "-" << pageSz*(j+1)-1 << ": Process "
			<< mem[j] << " , Page " << pg[j] << std::endl;
		}
		else
			output << "\t" << pageSz*j << "-" << pageSz*(j+1)-1 << ": "
			<< "Free Frame\n";
	}
}
