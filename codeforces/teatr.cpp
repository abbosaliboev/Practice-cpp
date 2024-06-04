#include <iostream>

using namespace std;

int main() {
    long long n, m, a;
    cin >> n >> m >> a;

    long long tiles_along_n = n / a;
    if (n % a != 0) {
        tiles_along_n++;
    }

    long long tiles_along_m = m / a;
    if (m % a != 0) {
        tiles_along_m++;
    }

    long long total_tiles = tiles_along_n * tiles_along_m;

    cout << total_tiles << endl;

    return 0;
}
