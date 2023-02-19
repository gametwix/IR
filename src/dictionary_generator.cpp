#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <filesystem>
#include "article.hpp"
#include "config.h"

namespace fs = std::filesystem;

void add_words_to_dictionary(std::set<std::string> &dictionary, std::string text){
    std::string word;
    for(const auto &c: text){
        if(isalnum(c)){
            word += tolower(c);
        }else if(word.length() > 0){
            dictionary.insert(word);
            word.clear();
        }
    }
    if(word.length() > 0){
        dictionary.insert(word);
    }
}

void save_dictionary(std::ofstream &file, std::set<std::string> &dictionary){
    size_t len_word;
    size_t i = 1;
    for(const auto &word: dictionary){
        len_word = word.size();
        file.write(reinterpret_cast<char *>(&i), sizeof(size_t));
        file.write(reinterpret_cast<char *>(&len_word), sizeof(size_t));
        file.write(word.c_str(), len_word);
        i++;
    }
}

int main(){
    std::cout << "Generate dictionary" << std::endl;
    std::vector<std::string> paths;
    for (const auto & entry : fs::directory_iterator(CORPUS_DIR_PATH)){
        paths.push_back(entry.path());
    }
    std::set<std::string> dictionary;
    for(const auto &path: paths){
        std::ifstream file;
        file.open(path,std::ios_base::binary);
        article wiki_article;
        while(file.peek() != EOF){
            wiki_article = read_article(file);
            add_words_to_dictionary(dictionary, wiki_article.title);
            add_words_to_dictionary(dictionary, wiki_article.text);
            
        }
        
        file.close();
        break;
    }
    std::cout << "Dictionary size: " << dictionary.size() << " terms" << std::endl;
    std::ofstream file;
    file.open(std::string(DICTIONARY_DIR_PATH) + "dictionary.words",std::ios_base::binary);
    save_dictionary(file, dictionary);
    return 0;
}