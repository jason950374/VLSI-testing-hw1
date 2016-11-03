#include <directGraph.h>
#include<gate.h>
#include<simulator.h>
#include<stdio.h>
#include<vector>
#include<stack>
#include <iostream>

using namespace std;

void initial(directGraph<gate> * circuit);
void encode(unsigned int * x, unsigned char size);
void printTrTable(unsigned int * x, unsigned char size, unsigned int z);

enum gatelist{
	X1,
	X2,
	X3,
	Y1,
	Y2,
	A,
	B,
	D
};

int main(void){
	directGraph<gate>  * circuit = new directGraph<gate>();
	initial(circuit);
	unsigned int x[3] = { 0 };
	encode(x, 3);
	Simulator simulator (circuit, x);
	printTrTable(x, 3, simulator.getOutput(D));

	system("pause");
	return 0;
}

void initial(directGraph<gate> * circuit){
	circuit->addRoot(BUFFER, X1);
	circuit->addRoot(BUFFER, X2);
	circuit->addRoot(BUFFER, X3);
	circuit->add(BUFFER, Y1, X2);
	circuit->add(BUFFER, Y2, X2);
	circuit->add(NOR, A, X1);
	circuit->link(Y1, A);
	circuit->add(NAND, B, Y2);
	circuit->link(X3, B);
	circuit->add(NOR, D, A);
	circuit->link(B, D);
}

void encode(unsigned int * x, unsigned char size){
	int i, j;
	unsigned int trigger;
	for(i = 0; i < size; i++){
		trigger = (1<<(1<<i)) -1;
		x[i] = 0;
		// 32 / (2<<i) == (1<<5)/(1<<(i+1)) == 1 << (4 - i)
		for(j = 0; j < (1 << (4 - i)); j++){
			x[i] = x[i] | (trigger << (j * (2<<i)));
		}
	}
}

void printTrTable(unsigned int * x, unsigned char size, unsigned int z){
	int i, j;
	unsigned int mask;
	//mask = (mask << 1) | (mask >> 31) is circular shift
	for (i = 0, mask = 1; i < (1 << size); i++, mask = (mask << 1) | (mask >> 31)){
		for (j = 0; j < size; j++){
			printf("%d  ", (x[j] & mask) != 0);
		}
		printf("|  %d\n", (z & mask) != 0);
	}
}