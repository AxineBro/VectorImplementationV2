#include "Vector.h"

#include <iostream>
#include <string>

int main() {
    std::cout << "Testing with int:" << std::endl;
    Vector<int> v1;
    std::cout << "Empty vector: " << v1 << std::endl;

    Vector<int> v2(3);
    std::cout << "Vector of size 3: " << v2 << std::endl;

    v1.push_back(1);
    v1.push_back(2);
    v1.push_back(3);
    std::cout << "v1 after push_back: " << v1 << std::endl;

    v1.push_front(0);
    std::cout << "v1 after push_front: " << v1 << std::endl;

    v1.insert(2, 10);
    std::cout << "v1 after insert at 2: " << v1 << std::endl;

    v1.erase(1);
    std::cout << "v1 after erase at 1: " << v1 << std::endl;

    Vector<int> v3 = v1;
    std::cout << "v3 (copy of v1): " << v3 << std::endl;

    std::cout << "v1 == v3: " << (v1 == v3 ? "true" : "false") << std::endl;

    v3.push_back(4);
    std::cout << "v1 == v3 after change: " << (v1 == v3 ? "true" : "false") << std::endl;

    Vector<int> v4(4);
    v4[0] = 1; v4[1] = 2; v4[2] = 3; v4[3] = 4;

    Vector<int> v5(4);
    v5[0] = 5; v5[1] = 6; v5[2] = 7; v5[3] = 8;

    Vector<int> sum = v4 + v5;
    std::cout << "v4 + v5: " << sum << std::endl;

    Vector<int> prod = v4 * v5;
    std::cout << "v4 * v5: " << prod << std::endl;

    Vector<int> v6(3);
    Vector<int> invalid_sum = v4 + v6;
    std::cout << "Invalid sum (different sizes): " << invalid_sum << std::endl;

    std::cout << "\nTesting with double:" << std::endl;
    Vector<double> dv1(2);
    dv1[0] = 1.5; dv1[1] = 2.5;

    Vector<double> dv2(2);
    dv2[0] = 3.0; dv2[1] = 4.0;

    std::cout << "dv1: " << dv1 << std::endl;
    std::cout << "dv2: " << dv2 << std::endl;

    Vector<double> dsum = dv1 + dv2;
    std::cout << "dv1 + dv2: " << dsum << std::endl;

    Vector<double> dprod = dv1 * dv2;
    std::cout << "dv1 * dv2: " << dprod << std::endl;

    dv1.push_back(5.5);
    std::cout << "dv1 after push_back: " << dv1 << std::endl;

    std::cout << "\nTesting with std::string:" << std::endl;
    Vector<std::string> sv1;
    sv1.push_back("Hello");
    sv1.push_back(" ");
    sv1.push_back("World");

    std::cout << "sv1: " << sv1 << std::endl;

    Vector<std::string> sv2 = sv1;
    sv2.insert(3, "!");

    std::cout << "sv2 after insert: " << sv2 << std::endl;

    std::cout << "sv1 == sv2: " << (sv1 == sv2 ? "true" : "false") << std::endl;

    return 0;
}