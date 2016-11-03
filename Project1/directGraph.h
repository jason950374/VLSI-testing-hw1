#ifndef __DIRECTGRAPH_H__
#define __DIRECTGRAPH_H__

#include <forward_list>
#include <vector>

using namespace std;

template <typename T> class DGNode{
	template <typename T> friend class directGraph;
public:
	DGNode<T>(unsigned int id);
	DGNode<T>(unsigned int id, T value);
	unsigned int getId() { return id; };
	forward_list<DGNode<T> *> * getOE() { return &outEdge; };
	forward_list<DGNode<T> *> * getIE() { return &inEdge; };
	T getValue(){ return value; };
private:
	unsigned int id;
	T value;
	forward_list<DGNode<T> *>  outEdge;
	forward_list<DGNode<T> *>  inEdge;
	void addOE(DGNode<T> *);
	void addIE(DGNode<T> *);
	void cutIE(DGNode<T> *);
	void cutOE(DGNode<T> *);
};

template <typename T> class directGraph{
public:
	directGraph<typename T>();
	~directGraph<typename T>();
	void addRoot(T Avalue);
	void add(T Avalue, unsigned int Tid);
	void add(T Avalue, T Tvalue);
	void addRoot(T Avalue, unsigned int Aid);
	void add(T Avalue, unsigned int Aid, unsigned int Tid);
	void add(T Avalue, unsigned int Aid, T Tvalue);
	void link(unsigned int id1, unsigned int id2);
	forward_list<DGNode<T> *> getRootList() { return rootList; };
	vector<DGNode<T> *> getIdVector() { return idVector; };
private:
	forward_list<DGNode<T> *>  rootList;
	vector<DGNode<T> *> idVector;
};

#endif