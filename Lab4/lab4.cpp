#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <time.h>
using namespace std;

random_device device;
mt19937 rng(time(nullptr));

vector<int> generateGen(int n){
    vector<int> gen;
    uniform_int_distribution<> distrib(0, 1);
    for(int i=0; i<n; i++){
        gen.push_back(distrib(rng));
    }
    return gen;
}

pair<unsigned long long int,unsigned long long int> genToFen(vector<int> binary){
    unsigned long long int x = 0;
    unsigned long long int y = 0;
    unsigned long long int j=0;
    for(int i=63; i>=0; i--){
        x += binary.at(i)*(unsigned long long int)(1ULL<<j);
        y += binary.at(i+64)*(unsigned long long int)(1ULL<<j);
        j++;
    } 
    return {x,y};
}

vector<int> fenToGen(unsigned long long int x,unsigned long long int y){
    vector<int> gen(128);
    for(int i=63; i>=0; i--){
        gen[i] = x%2;
        x = x/2;
        gen[i+64] = y%2;
        y = y/2;
    }
    return gen;
}

double bukin (double x, double y){
    return 100*sqrt(abs(y-(0.01*pow(x,2))))+0.01*abs(x+10);
};

double long_to_double(unsigned long long int num) {
    return num / 1.8446744e+18;
}

double fitness(vector<int> gen){
    auto fen = genToFen(gen);
    double x = long_to_double(fen.first);
    double y = long_to_double(fen.second);
    return 1/(bukin(x,y)+1);
}

int main(){
    vector<int> gen = generateGen(128);
    for(int i=0;i<64;i++){
        cout << gen[i];
    }
    cout << endl;
    for(int i=64;i<128;i++){
        cout << gen[i];
    }
    cout << endl;
    auto fen = genToFen(gen);
    cout << fen.first << endl << fen.second << endl;
    vector<int> gen1 = fenToGen(fen.first, fen.second);
    for(int i=0;i<64;i++){
        cout << gen1[i];
    }
    cout << endl;
    for(int i=64;i<128;i++){
        cout << gen1[i];
    }
    cout << endl;

    double result = fitness(gen);
    cout << result;
    return 0;
}