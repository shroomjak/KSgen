#include "ks.h"
using namespace std;
void Parser::split_input(const string line, string & sv, string & sw) const{
	stringstream str(line);			//make artifical stream
	str >> sv >> sw;			 	//and read from it
}

void Parser::read(){
	ifstream file(filename);
	file.seekg(0, std::ios::end);
	file.seekg(0, std::ios::beg);
	string line;
	getline(file, line);
	string sn, sk;
	split_input(line, sn, sk);
	size = stoi(sn);
	wmax = stod(sk);
	while (getline(file, line) and !file.eof()){
		string sv, sw;
		split_input(line, sv, sw);
		items.push_back(make_pair(stod(sv), stod(sw)));
	}
	file.close();
}
vector<item> Parser::get_items() const{
	return items;
}

double Solver::fit(const vector<bool> & kit) const{
	vector<item> items = parser->get_items();
	double wmax = parser->get_max();
	double res = 0;
	double w = 0;
	for(const auto & i : items){
		res += i.first;
		w += i.second;
	}
	return (w <= wmax)?(res):(0);
}

void Solver::init_gen(int N){
	srand(time(nullptr));
	int size = parser->get_size();
	gen.resize(N);
	for(int i = 0; i < N; i++){
		gen[i].first.resize(size);
		for(int j = 0; j < size; j++){
			gen[i].first[j] = ((double(rand())/double(RAND_MAX) > 0.5)?(1):(0));
		}
		gen[i].second = fit(gen[i].first);
	}
}

void Solver::mate(const pair<vector<bool>,double> & father,
				  const pair<vector<bool>, double> & mother,
				  pair<vector<bool>, double> & ans) const
{
	srand(time(nullptr));
	int n = father.first.size();  //= mother.size()
	ans.first.resize(n);
	for(int i = 0; i < n; i++){
		ans.first[i] = ((double(rand())/double(RAND_MAX) > crossover_rate)?(father.first[i]):(mother.first[i]));
		ans.first[i] = ((double(rand())/double(RAND_MAX) > mutation_rate)?(ans.first[i]):(!ans.first[i]));
	}
	ans.second = fit(ans.first);
}


void Solver::regen(){
	struct{
		bool operator()(pair<vector<bool>, double> a, pair<vector<bool>, double> b) const{
			return a.second < b.second;
		}
	}cmp;
	int n = gen.size(); 
	int m = ceil(0.5*(1+sqrt(1+8*n)));
	sort(gen.begin(), gen.end(), cmp);
	gen.resize(m);
	vector<pair<vector<bool>, double>> newgen;
	for(int i = 0; i < m-1; i++){
		for(int j = i+1; j < m; j++){
			pair<vector<bool>, double> elem;
			mate(gen[i], gen[j], elem);
			newgen.push_back(elem);
		} 
	}
	gen.clear();
	gen.resize(n);
	int size = parser->get_size();
	for(int i = 0; i < n; i++){ 
		for(int j = 0; j < size; j++)
			gen[i].first[j] = newgen[i].first[j];
		gen[i].second = newgen[i].second;
	}
}

void Solver::print_gen(){
	int n = gen.size();
	int size = parser->get_size();
	for(int i = 0; i < n; i++){
		for(int j = 0; j < size; j++)
			cout << gen[i].first[j] << " ";
		cout << ":" << gen[i].second << endl;
	}
}

