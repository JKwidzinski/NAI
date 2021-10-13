#include <iostream>
#include <tuple>

auto f2 = [](int i){
    return (2*(i%2)-1)*i/2;
};

auto f1 = [](auto x){
    return 40-abs(x);
};

auto obliczenia = [](auto f1, auto f2, int n){
    auto max_x = f2(1);
    auto max_y = f1(max_x);
    for(int i=1;i<n;i++){
        int x=f2(i);
        int y=f1(x);
        if(y > max_y){
            max_x=x;
            max_y=y;
        }
    }
    return std::make_pair(max_x,max_y);
};
int main(){
    using namespace std;
    cout << obliczenia(f1,f2,10).first << " " << obliczenia(f1,f2,10).second;
}
