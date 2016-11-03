#include <directGraph.h>
#include<gate.h>
#include <iostream>


//***************************************** DGNode *************************************************//
DGNode<gate>::DGNode(unsigned int id){
	this->id = id;
	value = AND;
	outEdge = forward_list<DGNode<gate> *>();
	inEdge = forward_list<DGNode<gate> *>();
}

DGNode<gate>::DGNode(unsigned int id, gate value){
	this->id = id;
	this->value = value;
	outEdge = forward_list<DGNode<gate> *>();
	inEdge = forward_list<DGNode<gate> *>();
}

void DGNode<gate>::addOE(DGNode<gate> * newNode){
	outEdge.push_front(newNode);
}

void DGNode<gate>::addIE(DGNode<gate> * newNode){
	inEdge.push_front(newNode);
}

void DGNode<gate>::cutIE(DGNode<gate> * deletNode){
	inEdge.remove(deletNode);
}

void DGNode<gate>::cutOE(DGNode<gate> * deletNode){
	outEdge.remove(deletNode);
}

//***************************************** directGraph *************************************************//
directGraph<gate>::directGraph(){
}

directGraph<gate>::~directGraph(){
	for (auto it = idVector.begin(); it != idVector.end(); ++it){
		delete *it;
	}
}

void directGraph<gate>::addRoot(gate Avalue){
	DGNode<gate> * newNode = new DGNode<gate>(idVector.size() + 1, Avalue);
	idVector.push_back(newNode);
	rootList.push_front(newNode);
}

void directGraph<gate>::add(gate Avalue, unsigned int Tid){
	DGNode<gate> * Node = idVector.at(Tid);
	DGNode<gate> * newNode = new DGNode<gate>(idVector.size() + 1, Avalue);
	idVector.push_back(newNode);

	Node->addOE(newNode);
	newNode->addIE(Node);
}

void directGraph<gate>::add(gate Avalue, gate Tvalue){
	cout << "don't use this\n";
}

void directGraph<gate>::addRoot(gate Avalue, unsigned int Aid){
	DGNode<gate> * newNode = new DGNode<gate>(Aid, Avalue);
	if (idVector.size() > Aid){
		DGNode<gate> * oldNode = idVector.at(Aid);
		for (auto it = oldNode->outEdge.begin(); it != oldNode->outEdge.end(); ++it){
			(*it)->cutIE(oldNode);
		}

		for (auto it = oldNode->inEdge.begin(); it != oldNode->inEdge.end(); ++it){
			(*it)->cutOE(oldNode);
		}
		idVector[Aid] = newNode;
		cout << "some gate has been replace!!" << '\n';
	}
	else{
		if (Aid - idVector.size() > 1) cout << "redundant!!" << endl;
		for (unsigned int i = idVector.size(); i <= Aid; i++){
			idVector.push_back(newNode);
		}
	}
	rootList.push_front(newNode);
}

//Aid is the id of new node, Tid is id of Target node. there will be an edge from Target node to new node
void directGraph<gate>::add(gate Avalue, unsigned int Aid, unsigned int Tid){
	DGNode<gate> * Node = idVector.at(Tid);
	DGNode<gate> * newNode = new DGNode<gate>(Aid, Avalue);
	if (idVector.size() > Aid){
		DGNode<gate> * oldNode = idVector.at(Aid);
		for (auto it = oldNode->outEdge.begin(); it != oldNode->outEdge.end(); ++it){
			(*it)->cutIE(oldNode);
		}
		
		for (auto it = oldNode->inEdge.begin(); it != oldNode->inEdge.end(); ++it){
			(*it)->cutOE(oldNode);
		}
		idVector[Aid] = newNode;
		cout << "some gate has been replace!!" << '\n';
	}
	else{
		if (Aid - idVector.size() > 1) cout << "redundant!!" << endl;
		for (unsigned int i = idVector.size(); i <= Aid; i++){
			idVector.push_back(newNode);
		}
	}
	Node->addOE(newNode);
	newNode->addIE(Node);
}

void directGraph<gate>::add(gate Avalue, unsigned int Aid, gate Tvalue){
	cout << "don't use this\n";
}

//edge direction is fome id1 to id2;
void directGraph<gate>::link(unsigned int id1, unsigned int id2){
	DGNode<gate> * Node1 = idVector.at(id1);
	DGNode<gate> * Node2 = idVector.at(id2);
	Node1->addOE(Node2);
	Node2->addIE(Node1);
}