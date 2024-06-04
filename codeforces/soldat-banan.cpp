#include <iostream>
using namespace std;

int main (){

    int k,n,w;

    cin>>k>>n>>w;

    int total=0;
    for(int i=1; i<=w; i++){
        total+=k*i;
    }

    int totalmoney =total - n;

    if(totalmoney<0){
        totalmoney=0;
    }

    cout<<totalmoney<<endl;


}