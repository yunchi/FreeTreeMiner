// HorizontalTreeMiner.cpp : Defines the entry point for the console application.

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <functional>
#include <vector>
#include <list>
#include <iterator>
#include <cmath>
#include <ctime>
#include <utility>
#include "misc.h"
#include "FreeTree.h"
#include "CanonicalTree.h"
#include "TupleTree.h"
#include "FrequentTreeList.h"
#include "CoreList.h"

using namespace std;

long SUPPORT_THRESHOLD = 0;
bool NEED_MAXIMUM = true;

int main(int argc, char* argv[])
{
	if ( argc != 4 )
	{	
		cout << "Usage: FreeTreeMiner support input_file output_file" << endl;
		exit (1);
	}
	
    istringstream iss(argv[1]);
    iss >> SUPPORT_THRESHOLD;
    if(!iss)
    {
        cout << "invalid argument, not an integer value" << endl;
        exit (1);
    }

	time_t start_time;
	time_t stop_time;
	long totalTime = 0, totalCheckTime=0, totalGenTime=0, totalCheckMaxTime = 0;
	long totalFrequent = 0;
	long totalFrequentMaximum = 0;
	long temp;

	string inputFile = argv[2];
	string outputFile = argv[3];

	ofstream dataFile(outputFile.c_str());
	if(!dataFile)
	{
		cerr << "cannot open file!" << endl;
		exit(1);
	}

	ifstream inFile(inputFile.c_str());
	if(!inFile)
	{
		cerr << "cannot open INPUT file!" << endl;
		exit(1);
	}

	//read in the database
	ptrFreeTree pft;
	vector<ptrFreeTree> database;
	char c;
	long counter = 1;
	database.push_back(new FreeTree()); //a dummy tree
	while ( !inFile.eof() )
	{
		c = inFile.get();
		if ( inFile.eof() ) break;
		else inFile.putback(c);
		pft = new FreeTree();
		inFile >> *pft;
		pft->tid = counter++; //force the tid list to be
		database.push_back(pft);
		c = inFile.get(); //to eat the last "end of line"
	}
	inFile.close();


	dataFile << "#Output Statistics for " << argv[2] << endl;
        dataFile << "#with support " << argv[1] << endl << endl;	
	
	FrequentTreeList* ftl[500]; //at most 500 vertices
	ftl[0] = new FrequentTreeList();
	start_time = time(0);
	ftl[0]->populate2(database);
	stop_time = time(0);
	totalFrequent += ftl[0]->returnSize();
	totalCheckTime += static_cast<long>(difftime(stop_time, start_time));
	dataFile << "number of 2-frequent tree: " << ftl[0]->returnSize() << endl;
	dataFile << "time spent: " << difftime(stop_time, start_time) << endl;
	dataFile << endl;
	cout << "number of 2-frequent tree: " << ftl[0]->returnSize() << endl;
	cout << "time spent: " << difftime(stop_time, start_time) << endl;
	cout << endl;

	CoreList* pcl;
	short i = 0;
	while ( ftl[i]->returnSize() != 0 )
	{
		start_time = time(0);
		pcl = new CoreList();
		ftl[i+1] = new FrequentTreeList();
		pcl->readFrom(*ftl[i]);
		pcl->writeTo(*ftl[i+1]);
		ftl[i+1]->checkDownward(*ftl[i]);
		stop_time = time(0);

		totalGenTime += static_cast<long>(difftime(stop_time, start_time));

		dataFile << "time for generating candidate" << i+3 << " tree set is: " << difftime(stop_time, start_time) << endl;
		cout << "time for generating candidate" << i+3 << " tree set is: " << difftime(stop_time, start_time) << endl;
		
		start_time = time(0);	

		dataFile << "before couting support, the candidate" << i+3 << " tree set is: " << ftl[i+1]->returnSize() << endl;
		cout << "before couting support, the candidate" << i+3 << " tree set is: " << ftl[i+1]->returnSize() << endl;

		ftl[i+1]->countSupport(database);
		ftl[i+1]->finalize(SUPPORT_THRESHOLD);
				
		dataFile << "the size of frequent" << i+3 << " tree set is: " << ftl[i+1]->returnSize() << endl;
		cout << "the size of frequent" << i+3 << " tree set is: " << ftl[i+1]->returnSize() << endl;
		
		delete pcl;
		stop_time = time(0);

		totalFrequent += ftl[i+1]->returnSize();
		totalCheckTime += static_cast<long>(difftime(stop_time, start_time));

		dataFile << "time for checking candidate: " << difftime(stop_time, start_time) << endl;
		dataFile << endl;
		cout << "time for checking candidate: " << difftime(stop_time, start_time) << endl;
		cout << endl;

		if ( NEED_MAXIMUM )
		{
			start_time = time(0);
			temp = ftl[i+1]->countMaximum(*ftl[i]);
			totalFrequentMaximum += temp;
			stop_time = time(0);
			totalCheckMaxTime += static_cast<long>(difftime(stop_time, start_time));
			dataFile << "number of MAXIMUM " << i+2 << "-frequent tree: " << temp << endl;
			dataFile << "time spent for Maximum checking: " << difftime(stop_time, start_time) << endl;
			dataFile << endl;
			cout << "number of MAXIMUM " << i+2 << "-frequent tree: " << temp << endl;
			cout << "time spent for Maximum checking: " << difftime(stop_time, start_time) << endl;
			cout << endl;
		}
		delete ftl[i];
		i++;
	}

	//remove the database from the memory and output the results
	for ( long s = 0; s < database.size(); s++ )
	{
		delete database[s];
	}


	totalTime = totalCheckTime + totalGenTime;

	dataFile << "*****Total Generating Time Spent: " << totalGenTime << endl;
	dataFile << "*****Total Checking Time Spent: " << totalCheckTime << endl;
	dataFile << "*****Total Time for Maximum Checking: " << totalCheckMaxTime << endl;
	dataFile << "*****Total Time Spent: " << totalTime << endl;
	dataFile << "*****Number of Frequent Subtrees: " << totalFrequent << endl;
	dataFile << "*****Number of Maximum Frequent Subtrees: " << totalFrequentMaximum << endl;

	cout << "*****Total Generating Time Spent: " << totalGenTime << endl;
	cout << "*****Total Checking Time Spent: " << totalCheckTime << endl;
	cout << "*****Total Time for Maximum Checking: " << totalCheckMaxTime << endl;
	cout << "*****Total Time Spent: " << totalTime << endl;
	cout << "*****Number of Frequent Subtrees: " << totalFrequent << endl;
	cout << "*****Number of Maximum Frequent Subtrees: " << totalFrequentMaximum << endl;

	dataFile.close();

	return 0;
}
