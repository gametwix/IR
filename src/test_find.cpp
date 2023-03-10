#include <iostream>
#include "parser.hpp"
#include <chrono>

int main(){
    std::cout << "What yot want find? : ";
    std::string find;
    std::getline(std::cin, find);
    auto begin = std::chrono::steady_clock::now();
    auto tockens = parse_request(find);
    // for(size_t i = 0; i < tockens.size(); ++i){
    //     std::cout << "type: " << tockens[i].type << "\ttocken: " << tockens[i].tocken << std::endl;
    // }
    auto polsk_tockens = to_reverse_polish(tockens);
    // for(size_t i = 0; i < polsk_tockens.size(); ++i){
    //     std::cout << "type: " << polsk_tockens[i].type << "\ttocken: " << polsk_tockens[i].tocken << std::endl;
    // }
    auto ans = run_queue(polsk_tockens);
    auto end = std::chrono::steady_clock::now();
    std::cout << "Size: " << ans.size() << std::endl;
    for(size_t i = 0; i < ans.size(); ++i){
        std::cout << ans[i] << " ";
    }
    std::cout << std::endl;
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
  std::cout << "The time: " << elapsed_ms.count() << " ms\n";

    return 0;
}