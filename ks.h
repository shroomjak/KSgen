#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <utility>
#include <vector>

using namespace std;
using item = pair<double, double>;

using namespace std;
class Parser{
	int size;
	double wmax;
	vector<item> items;
	string filename;
	void split_input(const string line, string & sv, string & sw) const;
public:
	Parser(string fname = ""){
		size = 0;
		filename = fname;
	}
	~Parser(){}
	
	int get_size() const{return size; }
	int get_max() const{return wmax; }
	void read();
	vector<item> get_items() const;
	void show_items();
	
	Parser * operator= (Parser * p){
		for(auto & elem : p->get_items())
			items.push_back(elem);
		size = p->size;
		wmax = p->wmax;
		filename = p->filename;
		return this;
	}
};

class Solver{
	int max;
	vector<pair<vector<bool>, double>> gen;
	double mutation_rate, crossover_rate;
	Parser * parser;
	double fit(const vector<bool> & kit) const;
	struct{
		bool operator()(pair<vector<bool>, double> a, pair<vector<bool>, double> b) const{
			return a.second > b.second;
		}
	}cmp;
public:
	Solver(){}
	Solver(Parser * p, double m, double c){
		parser = p;
		mutation_rate = m;
		crossover_rate = c;
	}
	~Solver(){}
	void init_gen(int N);
	void mate(const pair<vector<bool>, double> & father, 
			  const pair<vector<bool>, double> & mother,
			  pair<vector<bool>, double> & ans) const;
	vector<pair<vector<bool>, double>> get_gen(){return gen;}
	void regen();
	double get_max(){return max;}
	void print_gen();
	Solver * operator= (Solver * s){
		//allocate memory
		Parser tmp(*(s->parser));
		parser = &tmp;
		return this;
	}
};
