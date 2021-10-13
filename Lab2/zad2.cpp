#include <iostream>
#include <vector>
#include <random>
#include <iomanip>
#include <functional>

using namespace std;

random_device rd;
mt19937 gen(rd());

auto bukin = [](vector<double> v){
    double x = v.at(0), y = v.at(1);
    return 100*sqrt(abs(y-(0.01*pow(x,2))))+0.01*abs(x+10);
};

auto bukin_domain = [](vector<double> v) {
    return (v[0] >= -15) && (v[0] <= -5) && (abs(v[1]) <= 3);
};

uniform_real_distribution<> distrib_x(-15, -5);
uniform_real_distribution<> distrib_y(-3, 3);
vector<double> ackley_p0 = {
        distrib_x(gen),
        distrib_y(gen),
};

ostream& operator<<(ostream& o, vector<double> v)
{
    for (auto e : v) {
        o << std::fixed << std::setprecision(5) << "\t" << e;
    }
    return o;
}


vector<double> hill_climbing(function<double(vector<double>)> f, function<bool(vector<double>)> f_domain, vector<double> p0, int iterations)
{
    auto p = p0;
    uniform_int_distribution<> distrib(0, p.size() - 1);
    uniform_real_distribution<> distrib_r(-0.1, 0.1);

    if (!f_domain(p)) throw std::invalid_argument("The p0 point must be in domain");
    for (int i = 0; i < iterations; i++) {
        auto p2 = p;

        p[distrib(gen)] += distrib_r(gen);
        double y2 = f(p2);
        if (y2 < f(p)) {
            p = p2;
        }
    }
    return p;
}

int main() {

    auto result = hill_climbing(bukin, bukin_domain, ackley_p0, 10000);
    cout << result << " -> " << bukin(result) << endl;

    return 0;
}
