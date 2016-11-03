#include<simulator.h>
#include <iostream>

Simulator::Simulator(directGraph<gate> * circuit, unsigned int * PI){
	this->circuit = circuit;
	this->PI = circuit->getRootList();
	nodes = vector<DGNode<gate> *>(circuit->getIdVector());
	isVisitedOutput = vector<bool>(nodes.size(), false);
	output = vector<unsigned int>(nodes.size(), false);
	faultList = vector<vector<fault>>(nodes.size(), vector<fault>(nodes.size(), fault()));
	unsigned int crrtId;
	int i = 0;
	for (auto itPI = this->PI.begin(); itPI != this->PI.end(); ++itPI){
		crrtId = (*itPI)->getId();
		isVisitedOutput[crrtId] = true;
		output[crrtId] = PI[crrtId];
	}
}

unsigned int Simulator::getOutput(unsigned int id){
	if (isVisitedOutput.at(id)){
		return output.at(id);
	}
	unsigned int newOutput;
	DGNode<gate> * crrntNode = nodes.at(id);
	switch (crrntNode->getValue()){
		case AND:
			newOutput = 0xffffffff;
			for (auto it = crrntNode->getIE()->begin(); it != crrntNode->getIE()->end(); ++it){
				newOutput = newOutput & getOutput((*it)->getId());
			}
			break;
		case OR:
			newOutput = 0;
			for (auto it = crrntNode->getIE()->begin(); it != crrntNode->getIE()->end(); ++it){
				newOutput = newOutput | getOutput((*it)->getId());
			}
			break;
		case NAND:
			newOutput = 0xffffffff;
			for (auto it = crrntNode->getIE()->begin(); it != crrntNode->getIE()->end(); ++it){
				newOutput = newOutput & getOutput((*it)->getId());
			}
			newOutput = ~newOutput;
			break;
		case NOR:
			newOutput = 0;
			for (auto it = crrntNode->getIE()->begin(); it != crrntNode->getIE()->end(); ++it){
				newOutput = newOutput | getOutput((*it)->getId());
			}
			newOutput = ~newOutput;
			break;
		case NOT:
			if (++(crrntNode->getIE()->begin()) != crrntNode->getIE()->end()){
				cout << "NOT more than one input!!" << endl;
			}
			newOutput = ~getOutput((*crrntNode->getIE()->begin())->getId());
			break;
		case BUFFER:
			if (++(crrntNode->getIE()->begin()) != crrntNode->getIE()->end()){
				cout << "BUFFER more than one input!!" << endl;
			}
			newOutput = getOutput((*crrntNode->getIE()->begin())->getId());
			break;
	}
	isVisitedOutput[id] = true;
	output[id] = newOutput;
	return newOutput;
}

vector<fault> Simulator::getFaultList(unsigned int id)
{
	if (isVisitedFaultList.at(id)) {
		return faultList.at(id);
	}
	
	unsigned int newFaultList;
	/*DGNode<gate> * crrntNode = nodes.at(id);
	switch (crrntNode->getValue()) {
	case AND:
		newOutput = 0xffffffff;
		for (auto it = crrntNode->getIE()->begin(); it != crrntNode->getIE()->end(); ++it) {
			newOutput = newOutput & getOutput((*it)->getId());
		}
		break;
	case OR:
		newOutput = 0;
		for (auto it = crrntNode->getIE()->begin(); it != crrntNode->getIE()->end(); ++it) {
			newOutput = newOutput | getOutput((*it)->getId());
		}
		break;
	case NAND:
		newOutput = 0xffffffff;
		for (auto it = crrntNode->getIE()->begin(); it != crrntNode->getIE()->end(); ++it) {
			newOutput = newOutput & getOutput((*it)->getId());
		}
		newOutput = ~newOutput;
		break;
	case NOR:
		newOutput = 0;
		for (auto it = crrntNode->getIE()->begin(); it != crrntNode->getIE()->end(); ++it) {
			newOutput = newOutput | getOutput((*it)->getId());
		}
		newOutput = ~newOutput;
		break;
	case NOT:
		if (++(crrntNode->getIE()->begin()) != crrntNode->getIE()->end()) {
			cout << "NOT more than one input!!" << endl;
		}
		newOutput = ~getOutput((*crrntNode->getIE()->begin())->getId());
		break;
	case BUFFER:
		if (++(crrntNode->getIE()->begin()) != crrntNode->getIE()->end()) {
			cout << "BUFFER more than one input!!" << endl;
		}
		newOutput = getOutput((*crrntNode->getIE()->begin())->getId());
		break;
	}
	isVisitedOutput[id] = true;
	output[id] = newOutput;
	return newOutput;*/
}

