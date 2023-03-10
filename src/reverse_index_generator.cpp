#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <filesystem>

#include "article.hpp"
#include "reverse_index.hpp"
#include "dictionary.hpp"
#include "config.h"

namespace fs = std::filesystem;


int main(){
    std::cout << "Generate reverse index" << std::endl;
    auto dicionary = load_dicionary();
    std::vector<std::string> paths;
    for (const auto & entry : fs::directory_iterator(CORPUS_DIR_PATH)){
        paths.push_back(entry.path());
    }
    std::vector<std::vector<size_t>> reverse_index(dicionary.size(), std::vector<size_t>());
    std::ifstream file;
    for(const auto &path: paths){
        file.open(path,std::ios_base::binary);
        article wiki_article;
        while(file.peek() != EOF){
            wiki_article = read_article(file);
            reverse_index_add(reverse_index, dicionary, wiki_article.title, wiki_article.id);
            reverse_index_add(reverse_index, dicionary, wiki_article.text, wiki_article.id);
        }
        file.close();
        break;
    }
    std::cout << "Save reverse index" << std::endl;
    save_reverse_index(reverse_index);
    return 0;
}