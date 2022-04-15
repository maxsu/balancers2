// Call the tests

#include <iostream>
#include <string>

#include "lib/exists_balancer.hpp"
#include "lib/output_ratios.hpp"
#include "lib/utils.hpp"
#include "tests/test_cases.hpp"
#include "tests/test_utils.hpp"

void test_output_ratios() {
    log("Running output ratios test cases:");

    test_outputRatio_first_column(trivialLink());
    test_outputRatio_first_column(splitter1_2());
    test_outputRatio_first_column(splitter2_1());
    test_outputRatio_first_column(splitter2_2());
    test_outputRatio_first_column(balancer3_3());
    test_outputRatio_first_column(testnetA());  // 8 node test case
    test_outputRatio_first_column(testnetB());  // 17 node test case
}

void test_balancer_exists() {
    log("Running balancer existence checks:");
    
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

// Returns whether ther is a test with this index
bool run_test_by_number(int test_number) {
    switch (test_number) {
        case 0:
            test_output_ratios();
            return true;
        case 1:
            test_balancer_exists();
            return true;
    }
    
    return false;
}

int main(int argc, char **argv) {
    if (argc == 1) {
        int test_to_run = 0;
        while (run_test_by_number(test_to_run)) {
            ++test_to_run;
        }
    }
    else if (argc >= 2) {
        run_test_by_number(std::stoi(std::string(argv[1])));
    }
}
