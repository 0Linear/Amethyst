#include "engine/master.h"
#include <iostream>

void Engine::Warning(std::string text) {
    std::cout << "The engine issued the following warning:\n\n" << text << std::endl;
}

void Engine::Error(std::string text) {
    std::cout << "The engine encountered an unrecoverable error!\n\n" << text << std::endl;
    quick_exit(-1);
}