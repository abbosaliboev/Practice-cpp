#include <iostream>
#include <vector>
using namespace std;

int main() {
    string sonlar;
    cin>>sonlar;
    int count = 1;
    int max = 7;

    char left = sonlar[0];

    for(int i=1;i<sonlar.length();i++){
        if(sonlar[i]==left){
            count++;
            if(count>=max){
                cout<<"YES"<<endl;
                return 0;
            }
        }
        else{
            left = sonlar[i];
            count=1;
        }
    }

    cout<<"NO"<<endl;
    return 0;
}
