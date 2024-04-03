#include "ks.h"
using namespace std;
int main(void){
	string input_file = "data.txt";
	Parser parser(input_file);
	parser.read();
	
	Solver solver(&parser, 0.001, 0.75);
	solver.init_gen(50);
	for(int i = 0; i < 10000; i++){
		cout << solver.get_max() << endl;
		solver.regen();
	}
	solver.print_gen();
	return 0;
}
