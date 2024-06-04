#include <iostream>
using namespace std;

int main (){

    int a,b,c,t;
    cin>>t;

    if(t<163 && t>0){
    string arr[t];

    
        for(int i=0;i<t;i++){
            cin>>a>>b>>c;

                if(a<=9 && a>=0){
                if(b<=9 && b>=0){
                if(c<=18 && c>=-8){

                    if(c==a+b){
                        arr[i]='+';
                    }
                    else{
                        arr[i]='-';
                    }

                }
                }
                }

        }

        for(int i=0;i<t;i++){
            cout<<arr[i]<<endl;
        }

    }

    return 0;
}