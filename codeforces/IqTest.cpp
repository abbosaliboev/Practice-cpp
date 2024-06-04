#include <iostream>
#include <time.h>
using namespace std;

int main(){

    int n;
    int arr[]={};
    cin>>n;

    for(int i=0;i<n;i++){
        cin>>arr[i];
    }

    for(int i=0;i<n;i++){
        cout<<arr[i]<<" - ";
    }

return 0;
}