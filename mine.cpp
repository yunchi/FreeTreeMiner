// Project_Tree_Mining.cpp : Defines the entry point for the console application.

#include <iostream>
#include <fstream>
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

int main(int argc, char* argv[])
{
	if ( argc != 3 )
	{	
		cout << "Usage: mine input_file output_file" << endl;
		exit (1);
	}
	
	time_t start_time;
	time_t stop_time;

	string inputFile = argv[1];
	string outputFile = argv[2];

	ofstream dataFile(outputFile.c_str());
	if(!dataFile)
	{
		cerr << "cannot open file!" << endl;
		exit(1);
	}
	
	FrequentTreeList* ftl[30];
	ftl[0] = new FrequentTreeList();
	ftl[1] = new FrequentTreeList();
	start_time = time(0);
	ftl[0]->populate2(inputFile);
	stop_time = time(0);
	dataFile << "number of 2-frequent tree: " << ftl[0]->returnSize() << endl;
	dataFile << "time spent: " << difftime(stop_time, start_time) << endl;
	dataFile << endl;
	cout << "number of 2-frequent tree: " << ftl[0]->returnSize() << endl;
	cout << "time spent: " << difftime(stop_time, start_time) << endl;
	cout << endl;


	start_time = time(0);
	ftl[1]->populate3(inputFile,ftl[0]);
	stop_time = time(0);
	dataFile << "number of 3-frequent tree: " << ftl[1]->returnSize() << endl;
	dataFile << "time spent: " << difftime(stop_time, start_time) << endl;
	dataFile << endl;
	cout << "number of 3-frequent tree: " << ftl[1]->returnSize() << endl;
	cout << "time spent: " << difftime(stop_time, start_time) << endl;
	cout << endl;


	ftl[2] = new FrequentTreeList();
	start_time = time(0);
	ftl[2]->populate4(inputFile,ftl[1]);
	stop_time = time(0);
	dataFile << "number of 4-frequent tree: " << ftl[2]->returnSize() << endl;
	dataFile << "time spent: " << difftime(stop_time, start_time) << endl;
	dataFile << endl;
	cout << "number of 4-frequent tree: " << ftl[2]->returnSize() << endl;
	cout << "time spent: " << difftime(stop_time, start_time) << endl;
	cout << endl;


	CoreList* pcl;
	short i = 2;
	while ( ftl[i]->returnSize() != 0 )
	{
		start_time = time(0);
		pcl = new CoreList();
		ftl[i+1] = new FrequentTreeList();
		pcl->readFrom(*ftl[i]);
		pcl->writeTo(*ftl[i+1]);
		stop_time = time(0);
		
		dataFile << "time for generating candidate" << i+3 << " tree set is: " << difftime(stop_time, start_time) << endl;
		cout << "time for generating candidate" << i+3 << " tree set is: " << difftime(stop_time, start_time) << endl;
		
		start_time = time(0);	
		ftl[i+1]->countSupport(inputFile);
		
		dataFile << "the size of canidata" << i+3 << " tree set is: " << ftl[i+1]->returnSize() << endl;
		cout << "the size of canidata" << i+3 << " tree set is: " << ftl[i+1]->returnSize() << endl;
		
		ftl[i+1]->finalize(SUPPORT_THRESHOLD);
		
		dataFile << "the size of frequent" << i+3 << " tree set is: " << ftl[i+1]->returnSize() << endl;
		cout << "the size of frequent" << i+3 << " tree set is: " << ftl[i+1]->returnSize() << endl;
		
		i++;
		delete pcl;
		stop_time = time(0);

		dataFile << "time for checking candidate: " << difftime(stop_time, start_time) << endl;
		dataFile << endl;
		cout << "time for checking candidate: " << difftime(stop_time, start_time) << endl;
		cout << endl;
	}

	dataFile.close();

///////////////////////////////////////////////////////////////////
	//TupleTree* pct1;
	//TupleTree* pct2;

	//ifstream dataFile("database_test.txt");
	//if(!dataFile)
	//{
	//	cerr << "cannot open file!" << endl;
	//	exit(1);
	//}

	//pct1 = new TupleTree();
	//pct2 = new TupleTree();
	//dataFile >> *pct1;
	//dataFile >> *pct2;
	//pct1->rootIt();
	//cout << pct1->support(*pct2) << endl;
	////pct1->test();

	//delete pct1;
	//delete pct2;

	//dataFile.close();

	return 0;
}
