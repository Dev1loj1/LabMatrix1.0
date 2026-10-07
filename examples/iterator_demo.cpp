#include "TVector.h"

#include <iostream>

int main() {
    TVector<int> my_vec(8);
    int value = 1;

    for (TVector<int>::iterator current = my_vec.begin(); current != my_vec.end(); ++current) {
        *current = value++;
    }

    for (TVector<int>::const_iterator current = my_vec.cbegin(); current != my_vec.cend(); ++current) {
        std::cout << *current << ' ';
    }
    std::cout << std::endl;
    return 0;
}
