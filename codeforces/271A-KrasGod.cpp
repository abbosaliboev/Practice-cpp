#include <iostream>
using namespace std;

int main (){

    int n = 1978;
    int n1, n10, n100, n1000;

    

    n1= n%10;
    n10= n%100;
    n10=n10/10;
    n100=n%1000;
    n100=n100/100;
    n1000=n/1000;
    cout<<n1000<<" "<<n100<<" "<<n10<<" "<<n1;



    return 0;
}