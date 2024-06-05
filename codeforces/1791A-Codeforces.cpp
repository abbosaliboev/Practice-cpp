#include <iostream>
using namespace std;

int main(){
    int n;
    cin>>n;
    char arr[n];
    char code[]={'c','o','d','e','f','o','r','c','e','s'};

    for(int i=0;i<n;i++){
        cin>>arr[i];
    }
    
    for(int i=0;i<n;i++){
        bool found = false;
        for(int k=0;k<10;k++){
            if(arr[i]==code[k]){
                found = true;
                break;
            }
        }
        if(found){
            cout<<"YES"<<endl;
        }
        else{
            cout<<"NO"<<endl;
        }
    }
    return 0;
}