//#include <stdio.h>
//#include <forward_list>
//#include <directGraph.h>
//
//using namespace std;
//
//unsigned int circuit(unsigned int * x);
//unsigned int circuitFA0(unsigned int * x);
//unsigned int circuitFA1(unsigned int * x);
//unsigned int circuitFB0(unsigned int * x);
//unsigned int circuitFB1(unsigned int * x);
//unsigned int circuitFZ0(unsigned int * x);
//unsigned int circuitFZ1(unsigned int * x);
//void encode(unsigned int * x, unsigned char size);
//void printTrTable(unsigned int * x, unsigned char size, unsigned int z);
//forward_list<unsigned int> TPG(unsigned int * x, unsigned char size, unsigned int z, unsigned int zf);
//void printTP(forward_list<unsigned int> TP, unsigned char size);
//
//int main(void){
//	unsigned int x[3] = {0};
//	unsigned int z_Ffree, z_FA0, z_FA1, z_FB0, z_FB1, z_FZ0, z_FZ1;
//	
//	encode(x, 3);
//	
//	z_Ffree = circuit(x);
//	z_FA0 = circuitFA0(x);
//	z_FA1 = circuitFA1(x);
//	z_FB0 = circuitFB0(x);
//	z_FB1 = circuitFB1(x);
//	z_FZ0 = circuitFZ0(x);
//	z_FZ1 = circuitFZ1(x);
//
//	forward_list<unsigned int> tp_FA0 = TPG(x, 3, z_Ffree, z_FA0);
//	forward_list<unsigned int> tp_FA1 = TPG(x, 3, z_Ffree, z_FA1);
//	forward_list<unsigned int> tp_FB0 = TPG(x, 3, z_Ffree, z_FB0);
//	forward_list<unsigned int> tp_FB1 = TPG(x, 3, z_Ffree, z_FB1);
//	forward_list<unsigned int> tp_FZ0 = TPG(x, 3, z_Ffree, z_FZ0);
//	forward_list<unsigned int> tp_FZ1 = TPG(x, 3, z_Ffree, z_FZ1);
//
//	printf("-------------\n"); printTP(tp_FA0, 3); printf("-------------\n\n");
//	printf("-------------\n"); printTP(tp_FA1, 3); printf("-------------\n\n");
//	printf("-------------\n"); printTP(tp_FB0, 3); printf("-------------\n\n");
//	printf("-------------\n"); printTP(tp_FB1, 3); printf("-------------\n\n");
//	printf("-------------\n"); printTP(tp_FZ0, 3); printf("-------------\n\n");
//	printf("-------------\n"); printTP(tp_FZ1, 3); printf("-------------\n\n");
//	
//	system("pause");
//	return 0;
//}
//
///*********************************************
//      |<-    32bit     ->|
//x[0]  ... 0101010101010101
//x[1]  ... 0011001100110011
//x[2]  ... 0000111100001111
//x[3]  ... 0000000011111111
//	...
//	...
//*********************************************/
//void encode(unsigned int * x, unsigned char size){
//	int i, j;
//	unsigned int trigger;
//	for(i = 0; i < size; i++){
//		trigger = (1<<(1<<i)) -1;
//		x[i] = 0;
//		// 32 / (2<<i) == (1<<5)/(1<<(i+1)) == 1 << (4 - i)
//		for(j = 0; j < (1 << (4 - i)); j++){
//			x[i] = x[i] | (trigger << (j * (2<<i)));
//		}
//	}
//}
//
//void printTrTable(unsigned int * x, unsigned char size, unsigned int z){
//	int i, j;
//	unsigned int mask;
//	//mask = (mask << 1) | (mask >> 31) is circular shift
//	for(i = 0, mask = 1; i < (1<<size); i++, mask = (mask << 1) | (mask >> 31)){
//		for(j = 0; j < size; j++){
//			printf("%d  ", (x[j] & mask) !=0);
//		}
//		printf("|  %d\n", (z & mask) !=0);
//	}
//}
//
///*********************************************
//           |<-       32bit      ->|
//pattern 1  ... x[4]x[3]x[2]x[1]x[0]
//pattern 2  ... x[4]x[3]x[2]x[1]x[0]
//pattern 3  ... x[4]x[3]x[2]x[1]x[0]
//pattern 4  ... x[4]x[3]x[2]x[1]x[0]
//...
//...
//*********************************************/
//forward_list<unsigned int> TPG(unsigned int * x, unsigned char size, unsigned int z, unsigned int zf){
//	forward_list<unsigned int> TP = forward_list<unsigned int>();
//	unsigned int feffect = z ^zf;
//	unsigned int pattern_temp;
//	unsigned int mask = 1;
//	for (int i = 0; i < (1 << size); i++, feffect = feffect >> 1){
//		if (feffect & mask){
//			pattern_temp = 0;
//			for (int j = 0; j < size; j++){
//				pattern_temp = pattern_temp | (((x[j]>>i) & mask) << j);
//			}
//			TP.push_front(pattern_temp);
//		}
//	}
//	return TP;
//}
//
//void printTP(forward_list<unsigned int> TP, unsigned char size){
//	unsigned int mask = 1;
//	for (auto it = TP.begin(); it != TP.end(); ++it){
//		for (int j = 0; j < size; j++){
//			printf("%u ", (*it >> j) & mask);
//		}
//		printf("\n");
//	}
//}
//
//unsigned int circuit(unsigned int * x){
//	unsigned int a, b, z;
//	a = ~(x[0] | x[1]);
//	b = ~(x[1] & x[2]);
//	z = ~(a | b); 
//	return z; 
//}
//
//unsigned int circuitFA0(unsigned int * x){
//	unsigned int a, b, z;
//	//a = ~(x[0] | x[1]);
//	a = 0;
//	b = ~(x[1] & x[2]);
//	z = ~(a | b); 
//	return z; 
//}
//
//unsigned int circuitFA1(unsigned int * x){
//	unsigned int a, b, z;
//	//a = ~(x[0] | x[1]);
//	a = 0xffffffff;
//	b = ~(x[1] & x[2]);
//	z = ~(a | b); 
//	return z; 
//}
//
//unsigned int circuitFB0(unsigned int * x){
//	unsigned int a, b, z;
//	a = ~(x[0] | x[1]);
//	//b = ~(x[1] & x[2]);
//	b = 0;
//	z = ~(a | b); 
//	return z; 
//}
//
//unsigned int circuitFB1(unsigned int * x){
//	unsigned int a, b, z;
//	a = ~(x[0] | x[1]);
//	//b = ~(x[1] & x[2]);
//	b = 0xffffffff;
//	z = ~(a | b); 
//	return z; 
//}
//
//unsigned int circuitFZ0(unsigned int * x){
//	return 0; 
//}
//
//unsigned int circuitFZ1(unsigned int * x){
//	return 0xffffffff; 
//}
