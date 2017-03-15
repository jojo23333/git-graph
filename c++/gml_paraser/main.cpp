#include <iostream>
#include "readgml.h"
/* run this program using the console pauser or add your own getch, system("pause") or input loop */
using namespace std;
int main(int argc, char** argv) {
	NETWORK x;
	cout<<read_network(&x,fopen("polblogs.gml","r"));
	return 0;
}
