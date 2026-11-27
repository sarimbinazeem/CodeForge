#include <iostream>
#include <vector>
using namespace std;

void binaryString(int size, int index, vector<int>& current) {
    if (index == size) {
        for (int i = 0; i < size; i++) {
            cout << current[i];
        }
        cout << " ";
        return;
    }
    current[index] = 0;
    binaryString(size, index + 1, current);

    current[index] = 1;
    binaryString(size, index + 1, current);
}

int main() {
    int n;
    if (cin >> n) {
        vector<int> current(n);
        binaryString(n, 0, current);
    }
    return 0;
}
