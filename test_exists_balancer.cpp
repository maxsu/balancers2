// Test Runner for exists_balancers

#include <iostream>

#include "lib/exists_balancer.hpp"

int main() {
    //vector<vector<double>> my_network = {{0.1, 0.2, 0.3, 0.4}, {0.5, 0.6, 0.7, 0.8}};
    
    //vector<vector<double>> new_network = addSplitter(my_network, {0}, {1, -1});
    
    // bool balancerExists = existsBalancer(5, 5, 6);
    bool balancerExists = existsBalancer(4, 4, 4);

    if (balancerExists) {
        cout << "Balancer Exists!";
    } else {
        cout << "Balancer does not exist!";
    }
    
    cout << "\n";
}
