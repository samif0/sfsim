#include <SFML/Graphics.hpp>
#include <iostream>

namespace SimpleExamples {
    void runExample(int choice);
}

int main() {
    std::cout << "SFSim - Select an example to run:" << std::endl;
    std::cout << "1. DNA Helix (rotating double helix structure)" << std::endl;
    std::cout << "2. Particle System (bouncing particles)" << std::endl;
    std::cout << "3. Mandala (geometric mandala pattern)" << std::endl;
    std::cout << "Enter choice (1-3, default=1): ";
    
    int choice = 1;
    std::string input;
    std::getline(std::cin, input);
    
    if (!input.empty()) {
        try {
            choice = std::stoi(input);
            if (choice < 1 || choice > 3) {
                choice = 1;
            }
        } catch (const std::exception&) {
            choice = 1;
        }
    }
    
    SimpleExamples::runExample(choice);
    return 0;
}
