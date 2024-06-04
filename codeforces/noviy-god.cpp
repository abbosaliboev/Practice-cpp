#include <iostream>
using namespace std;

int main(){

    int x1,x2,x3;
    int min,med,max,maxim,medium;
    int n1,n2;

    cin>>x1>>x2>>x3;

    if(x1<x2){
        max=x2;
        min=x1;
    }
    else{
        max=x1;
        min=x2;
    }

    if(x3>max){
        maxim=x3;
        medium=max;
    }
    else{
        maxim=max;
        medium=x3;
    }

    if(medium<min){
        med=medium;
        medium=min;
        min=med;
    }

    n1=maxim-medium;
    n2=medium-min;

    cout<<n1+n2<<endl;

    return 0;
}