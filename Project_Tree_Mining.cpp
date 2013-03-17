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
	time_t start_time;
	time_t stop_time;
	string s = "database1000.txt";
	FrequentTreeList* ftl[30];
	ftl[0] = new FrequentTreeList();
	ftl[1] = new FrequentTreeList();
	start_time = time(0);
	ftl[0]->populate2(s);
	stop_time = time(0);
	cout << "number of 2-frequent tree: " << ftl[0]->returnSize() << endl;
	cout << "time spent: " << difftime(stop_time, start_time) << endl;
	cout << endl;

	start_time = time(0);
	ftl[1]->populate3(s,ftl[0]);
	stop_time = time(0);
	cout << "number of 3-frequent tree: " << ftl[1]->returnSize() << endl;
	cout << "time spent: " << difftime(stop_time, start_time) << endl;
	cout << endl;

	ftl[2] = new FrequentTreeList();
	start_time = time(0);
	ftl[2]->populate4(s,ftl[1]);
	stop_time = time(0);
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
		//pcl->test();
		pcl->writeTo(*ftl[i+1]);
		ftl[i+1]->countSupport(s);
		cout << "the size of canidata" << i+3 << " tree set is: " << ftl[i+1]->returnSize() << endl;
		ftl[i+1]->finalize(SUPPORT_THRESHOLD);
		cout << "the size of frequent" << i+3 << " tree set is: " << ftl[i+1]->returnSize() << endl;
		i++;
		//ftl[i]->test();
		delete pcl;
		stop_time = time(0);
		cout << "time spent: " << difftime(stop_time, start_time) << endl;
		cout << endl;
	}


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
