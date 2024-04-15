#include "openai.hpp"

#include <iostream>

int main() {
    openai::start();



    std::cout << "Response is:\n" << completion.dump(2) << '\n'; 
}
