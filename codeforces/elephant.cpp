#include <iostream>
using namespace std;

int main() {
    int p = 0; // the starting point where the elephant is located
    int x; // the point where the friend is located
    int max_steps = 5; // the maximum number of steps the elephant can take
    int count = 1; // the number of attempts (tries)
    cin >> x; // input the point where the elephant's friend is located

    // if the maximum steps are greater than or equal to the friend's location
    if(x <= max_steps) {
        cout << count << endl; // print the number of tries (count)
    }
    else {
        while(x > max_steps) { // while the friend's location is greater than max_steps, keep adding 5
            max_steps += 5; // increase the maximum steps by 5
            count++; // increment the number of tries (count)
        }
        cout << count << endl; // print the final result
    }
}