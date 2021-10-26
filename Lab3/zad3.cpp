#include <iostream>
#include <vector>
#include <random>
#include <iomanip>
#include <functional>
#include <string>

using namespace std;

random_device rd;
mt19937 generator(rd());

auto eggholder = [](vector<double> v){
    double x = v.at(0), y = v.at(1);
    return (-1)*(y+47)*sin(sqrt(abs(x/2 + y + 47))) - x*sin(sqrt(abs(x - y - 47)));
};

auto eggholder_domain = [](vector<double> v) {
    return (abs(v[0]) <= 512) && (abs(v[1]) <= 512);
};

uniform_real_distribution<> distrib_xy(-512, 512);
vector<double> eggholder_p0 = {
        distrib_xy(generator),
        distrib_xy(generator),
};

auto bukin = [](vector<double> v){
    double x = v.at(0), y = v.at(1);
    return 100*sqrt(abs(y-(0.01*pow(x,2))))+0.01*abs(x+10);
};

auto bukin_domain = [](vector<double> v) {
    return (v[0] >= -15) && (v[0] <= -5) && (abs(v[1]) <= 3);
};

uniform_real_distribution<> distrib_x(-15, -5);
uniform_real_distribution<> distrib_y(-3, 3);
vector<double> bukin_p0 = {
        distrib_x(generator),
        distrib_y(generator),
};

ostream& operator<<(ostream& o, vector<double> v)
{
    for (auto e : v) {
        o << std::fixed << std::setprecision(5) << "\t" << e;
    }
    return o;
}


vector<double> hill_climbing(function<double(vector<double>)> func, function<bool(vector<double>)> func_domain, vector<double> p0, int iterations)
{
    auto p1 = p0;
    uniform_int_distribution<> distrib(0, p1.size() - 1);
    uniform_real_distribution<> distrib_r(-0.1, 0.1);

    if (!func_domain(p1)) throw std::invalid_argument("The p0 point must be in domain");
    for (int i = 0; i < iterations; i++) {
        auto p2 = p1;

        p2[distrib(generator)] += distrib_r(generator);
        if (func_domain(p2)) {
            double val2 = func(p2);
            double val1 = func(p1);
            if (val2 < val1) {
                p1 = p2;
            }
        }
    }
    return p1;
}

vector<double> simulated_annealing(
    function<double(vector<double>)> f,
    function<bool(vector<double>)> f_domain,
    vector<double> p0,
    int iterations,
    function<vector<double>(vector<double>)> N,
    function<double(int)> T)
{
    auto s_current = p0;
    auto s_global_best = p0;

    uniform_real_distribution<> u_k(0.0, 1.0);

    if (!f_domain(s_current)) throw std::invalid_argument("The s_current point must be in domain");

    for (int k = 0; k < iterations; k++) {
        auto s_next = N(s_current);
        if(f_domain(s_next)){
            if (f(s_next) < f(s_current)) {
                s_current = s_next;
            } else {
                double u = u_k(generator);
                if (u < exp(-abs(f(s_next) - f(s_current)) / T(k))) {
                    s_current = s_next;
                }
            }
        }
        if (f(s_current) < f(s_global_best)) {
            s_global_best = s_current;
        }
    }
        
    return s_global_best;
}


int main() {
    vector<double> result;
    int input;
    cout<<"Choose:"<<endl<<"1.Hill + bukin"<<endl<<"2.Hill + eggholder"<<endl<<"3.Annealing + bukin"<<endl<<"4.Annealing + eggholder"<<endl;
    cin>>input;
    switch(input){
        case 1:
            result = hill_climbing(bukin, bukin_domain, bukin_p0, 10000);
            cout << result << " -> " << bukin(result) << endl;
            break;

        case 2:
            result = hill_climbing(eggholder, eggholder_domain, eggholder_p0, 10000);
            cout << result << " -> " << eggholder(result) << endl;
            break;

        case 3:
            result = simulated_annealing(
                bukin, bukin_domain, bukin_p0, 10000,
                [](auto p) {
                    normal_distribution<double> n(0.0, 0.3);
                    for (auto& e : p) {
                        e = e + n(generator);
                    }
                    return p;
                },
            [](int k) { return 1000.0 / k; }
            );
            cout << result << " -> " << bukin(result) << endl;
            break;

        case 4:
            result = simulated_annealing(
                eggholder, eggholder_domain, eggholder_p0, 10000,
                [](auto p) {
                    normal_distribution<double> n(0.0, 0.3);
                    for (auto& e : p) {
                        e = e + n(generator);
                    }
                    return p;
                },
            [](int k) { return 1000.0 / k; }
            );
            cout << result << " -> " << eggholder(result) << endl;
            break;

        default:
            cout<<"Invalid argument!"<<endl;
            break;

    }

    return 0;
}
