#include <iostream>
using namespace std;

int main(){
    int x = 0;
    int n;
    string input;
    cin >> n;

    for(int i = 0; i < n; i++){ 
        cin >> input;
        
        if(input == "X++" || input == "++X"){
            x += 1;
        }
        else if (input == "X--" || input == "--X"){
            x -= 1;
        }
    }
    cout<<x<<endl;
}