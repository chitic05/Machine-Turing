#include "Turing.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if(argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <config_file> <input_string>\n";
        return 1;
    }

    try {
        Turing machine(argv[1]);
        machine.initializeBand(argv[2]);  // Load input into band

        bool result = machine.run();

        
        if(result) {
            std::cout << "Turing machine accepted the input (reached final state)\n";
                    machine.printBand();
            std::cout << '\n';
            return 0;
        } else {
            std::cout << "Turing machine rejected the input (invalid transition or boundary violation)\n";
                    machine.printBand();
            std::cout << '\n';
            return 1;
        }
    } catch(const std::exception& e) {
        std::cerr << "Error: " << e.what();
        return 2;
    }
    
}
