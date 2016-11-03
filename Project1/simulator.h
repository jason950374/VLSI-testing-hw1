#ifndef __SIMULATOR_H__
#define __SIMULATOR_H__

#include <directGraph.h>
#include<gate.h>
#include<vector>
#include<stack>

class Simulator{
	public:
		Simulator(directGraph<gate> * circuit, unsigned int * PI);
		directGraph<gate> * circuit;
		forward_list<DGNode<gate> *> PI;
		vector<DGNode<gate> *> nodes;
		vector<bool> isVisited;
		vector<unsigned int> output;
		stack<unsigned int> stackNodes;

		unsigned int getOutput(unsigned int id);
};

#endif