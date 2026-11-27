#include <iostream>
using namespace std;

int main() {
    int r, c;
    if (!(cin >> r >> c) || r <= 0 || c <= 0) return 0;

    // Dynamically allocate an array of row pointers
    int** matrix = new int*[r];

    // Allocate memory for each row
    for (int i = 0; i < r; i++) {
        *(matrix + i) = new int[c];
    }

    int sum = 0;

    // Read elements and compute the total sum
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            cin >> *(*(matrix + i) + j);
            sum += *(*(matrix + i) + j);
        }
    }

    // Display the matrix row by row
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            cout << *(*(matrix + i) + j);
            if (j < c - 1) cout << " ";
        }
        cout << "\n";
    }

    // Print the total sum
    cout << "Sum: " << sum << "\n";

    // Deallocate memory for each row, then the array of pointers
    for (int i = 0; i < r; i++) {
        delete[] *(matrix + i);
    }
    delete[] matrix;

    return 0;
}
