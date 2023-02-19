#include <iostream>
#include <string>
#include <vector>
#include "parser.hpp"


int main(){
    std::string request;
    std::getline(std::cin, request);
    auto tockens = parse_request(request);
    for(size_t i = 0; i < tockens.size(); ++i){
        std::cout << "type: " << tockens[i].type << "\ttocken: " << tockens[i].tocken << std::endl;
    }
    auto queue = to_reverse_polish(tockens);
    std::cout << queue.size() << std::endl;
    for(size_t i = 0; i < queue.size(); ++i){
        std::cout << "type: " << queue[i].type << "\ttocken: " << queue[i].tocken << std::endl;
    }
    return 0;
}