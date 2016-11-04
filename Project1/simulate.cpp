#include<simulator.h>
#include <iostream>

Simulator::Simulator(directGraph<gate> * circuit, unsigned int * PI){
	this->circuit = circuit;
	this->PI = circuit->getRootList();
	nodes = vector<DGNode<gate> *>(circuit->getIdVector());
	isVisitedOutput = vector<bool>(nodes.size(), false);
	output = vector<unsigned int>(nodes.size(), false);
	faultList = vector<vector<fault>>(nodes.size(), vector<fault>(nodes.size(), fault()));
	isVisitedFaultList = vector<bool>(nodes.size(), false);
	unsigned int crrtId;
	int i = 0;
	for (auto itPI = this->PI.begin(); itPI != this->PI.end(); ++itPI){
		crrtId = (*itPI)->getId();
		isVisitedOutput[crrtId] = true;
		output[crrtId] = PI[crrtId];

		isVisitedFaultList[crrtId] = true;
		faultList.at(crrtId)[crrtId] = fault(~PI[crrtId], PI[crrtId]);
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

vector<fault> * Simulator::getFaultList(unsigned int id)
{
	if (isVisitedFaultList.at(id)) {
		return &faultList.at(id);
	}
	
	vector<fault> newFaultList = vector<fault>(nodes.size(), fault());
	DGNode<gate> * crrntNode = nodes.at(id);
	switch (crrntNode->getValue()) {
	case AND:
		//for AND initial to 0 if ouput = 1 (Non-controlling) (to do or), else = 0 (controlling) (to do and)
		for (auto it = newFaultList.begin(); it != newFaultList.end(); ++it) {
			*it = ~getOutput(id);
		}

		for (auto itIE = crrntNode->getIE()->begin(); itIE != crrntNode->getIE()->end(); ++itIE) {
			auto itIEF = getFaultList((*itIE)->getId())->begin();
			auto it = newFaultList.begin();
			for (;
				itIEF != getFaultList((*itIE)->getId())->end(), it != newFaultList.end();
				++itIEF, ++it) {
				*it = *it | *itIEF & getOutput(id) //Non - controlling
					| *it & *itIEF & (~(getOutput((*itIE)->getId()))) & (~getOutput(id)) //controlling
					| *it & (~(*itIEF)) & getOutput((*itIE)->getId()) & (~getOutput(id)); //controlling
			}
		}
		break;
	case OR:
		//for OR initial to 0 if ouput = 0 (Non-controlling) (to do or), else = 1 (controlling) (to do and)
		for (auto it = newFaultList.begin(); it != newFaultList.end(); ++it) {
			*it = getOutput(id);
		}

		for (auto itIE = crrntNode->getIE()->begin(); itIE != crrntNode->getIE()->end(); ++itIE) {
			auto itIEF = getFaultList((*itIE)->getId())->begin();
			auto it = newFaultList.begin();
			for (;
				itIEF != getFaultList((*itIE)->getId())->end(), it != newFaultList.end();
				++itIEF, ++it) {
				*it = *it | *itIEF & (~getOutput(id)) //Non - controlling
					| *it & *itIEF & getOutput((*itIE)->getId()) & getOutput(id) //controlling
					| *it & (~(*itIEF)) & (~(getOutput((*itIE)->getId()))) & getOutput(id); //controlling
			}
		}
		break;
	case NAND:
		//for NAND initial to 0 if ouput = 0 (Non-controlling) (to do or), else = 1 (controlling) (to do and)
		for (auto it = newFaultList.begin(); it != newFaultList.end(); ++it) {
			*it = getOutput(id);
		}

		for (auto itIE = crrntNode->getIE()->begin(); itIE != crrntNode->getIE()->end(); ++itIE) {
			auto itIEF = getFaultList((*itIE)->getId())->begin();
			auto it = newFaultList.begin();
			for (;
				itIEF != getFaultList((*itIE)->getId())->end(), it != newFaultList.end();
				++itIEF, ++it) {
				*it = *it | *itIEF & (~getOutput(id)) //Non - controlling
					| *it & *itIEF & (~(getOutput((*itIE)->getId()))) & getOutput(id) //controlling
					| *it & (~(*itIEF)) & (getOutput((*itIE)->getId())) & getOutput(id); //controlling
			}
		}
		break;
	case NOR:
		//for NOR initial to 0 if ouput = 1 (Non-controlling) (to do or), else = 0 (controlling) (to do and)
		for (auto it = newFaultList.begin(); it != newFaultList.end(); ++it) {
			*it = ~getOutput(id);
		}

		for (auto itIE = crrntNode->getIE()->begin(); itIE != crrntNode->getIE()->end(); ++itIE) {
			auto itIEF = getFaultList((*itIE)->getId())->begin();
			auto it = newFaultList.begin();
			for (;
				itIEF != getFaultList((*itIE)->getId())->end(), it != newFaultList.end();
				++itIEF, ++it) {
				*it = *it | *itIEF & getOutput(id) //Non - controlling
					| *it & *itIEF & (getOutput((*itIE)->getId())) & (~getOutput(id)) //controlling
					| *it & (~(*itIEF)) & (~(getOutput((*itIE)->getId()))) & (~getOutput(id)); //controlling
			}
		}
		break;
	case NOT:
	case BUFFER:
		if (++(crrntNode->getIE()->begin()) != crrntNode->getIE()->end()) {
			cout << "more than one input!!" << endl;
		}
		newFaultList.assign(getFaultList((*(crrntNode->getIE()->begin()))->getId())->begin(), 
			getFaultList((*(crrntNode->getIE()->begin()))->getId())->end());
		break;
	}
	newFaultList[id] = fault(~getOutput(id), getOutput(id)); //locally generate fault

	isVisitedFaultList[id] = true;
	faultList[id] = newFaultList;
	return &(faultList[id]);
}

