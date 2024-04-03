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
	double res = 0.0;
	double w = 0.0;
	for(int i = 0; i < parser->get_size(); i++){
		res += items[i].first*int(kit[i]);
		w += items[i].second*int(kit[i]);
	}
	return (w <= wmax)?(res):(0.0);
}

void Solver::init_gen(int N){
	srand(time(nullptr));
	int size = parser->get_size();
	gen.resize(N);
	int i = 0;
	while(i < N){
		gen[i].first.resize(size);
		for(int j = 0; j < size; j++){
			gen[i].first[j] = ((double(rand())/double(RAND_MAX) > 0.5)?(1):(0));
		}
		gen[i].second = fit(gen[i].first);
		if(gen[i].second != 0) i++;
	}
	sort(gen.begin(), gen.end(), cmp);
}

void Solver::mate(const pair<vector<bool>,double> & father,
				  const pair<vector<bool>, double> & mother,
				  pair<vector<bool>, double> & ans) const
{
	srand(time(nullptr));
	int n = parser->get_size();  //= mother.size()
	ans.first.resize(n);
	for(int i = 0; i < n; i++){
		ans.first[i] = ((double(rand())/double(RAND_MAX) > crossover_rate)?(father.first[i]):(mother.first[i]));
		ans.first[i] = ((double(rand())/double(RAND_MAX) > mutation_rate)?(ans.first[i]):(!ans.first[i]));
	}
	ans.second = fit(ans.first);
}


void Solver::print_gen(){
	int n = gen.size();
	int size = parser->get_size();
	for(int i = 0; i < n; i++){
		for(int j = 0; j < size; j++)
			cout << gen[i].first[j] << " ";
		cout << ":" << gen[i].second << endl;
	}
	cout << endl;
}

void Solver::regen(){
	int n = gen.size(); 
	vector<pair<vector<bool>, double>> newgen(n);
	for(int i = 0; i < n-1; i++){
		for(int j = i+1; j < n; j++){
			auto elem = new pair<vector<bool>, double>;
			mate(gen[i], gen[j], *elem);
			newgen.push_back(*elem);
		} 
	}
	sort(newgen.begin(), newgen.end(), cmp);
	newgen.resize(n);
	int size = parser->get_size();
	for(int i = 0; i < n; i++){ 
		for(int j = 0; j < size; j++)
			gen[i].first[j] = newgen[i].first[j];
		gen[i].second = newgen[i].second;
	}
	max = gen[0].second;
}

