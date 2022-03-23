// test_exists_balancer.cpp

#include <iostream>

#include "lib/exists_balancers.h"

int main() {
    //vector<vector<double>> my_network = {{0.1, 0.2, 0.3, 0.4}, {0.5, 0.6, 0.7, 0.8}};
    
    //vector<vector<double>> new_network = addSplitter(my_network, {0}, {1, -1});
    
    bool balancerExists = existsBalancer(5, 5, 6);
    
    cout << "\n";
}
