#ifndef __SIMULATOR_H__
#define __SIMULATOR_H__

#include <directGraph.h>
#include<gate.h>
#include<vector>
#include<stack>

struct fault {
	unsigned int stuckAtOne;
	unsigned int stuckAtZero;

	fault(int stuckAtOne = 0, int stuckAtZero = 0)
		: stuckAtOne(stuckAtOne), stuckAtZero(stuckAtZero)
	{
	}

	fault operator&(const fault& f2) const {
		return fault(stuckAtOne & f2.stuckAtOne, stuckAtZero & f2.stuckAtZero);
	}

	fault operator&(const unsigned int i) const {
		return fault(stuckAtOne & i, stuckAtZero & i);
	}

	fault operator|(const fault& f2) const {
		return fault(stuckAtOne | f2.stuckAtOne, stuckAtZero | f2.stuckAtZero);
	}

	fault operator|(const unsigned int i) const {
		return fault(stuckAtOne | i, stuckAtZero | i);
	}

	fault operator~() const {
		return fault(~stuckAtOne, ~stuckAtZero);
	}
};

class Simulator{
	public:
		Simulator(directGraph<gate> * circuit, unsigned int * PI);
		directGraph<gate> * circuit;
		forward_list<DGNode<gate> *> PI;
		vector<DGNode<gate> *> nodes;
		vector<bool> isVisitedOutput;
		vector<unsigned int> output;
		stack<unsigned int> stackNodes;
		vector<bool> isVisitedFaultList;
		vector<vector<fault>> faultList; // Actually ,it's list of fault list

		unsigned int getOutput(unsigned int id);
		vector<fault> * getFaultList(unsigned int id);
};

#endif