#include "ks.h"
using namespace std;
int main(void){
	string input_file = "data.txt";
	Parser parser(input_file);
	parser.read();
	
	Solver solver(&parser, 0.5, 0.001);
	solver.init_gen(10);
	for(int i = 0; i < 100; i++)
		solver.regen();
	solver.print_gen();
	return 0;
}
