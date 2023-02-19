#pragma once
#include <string>
#include <fstream>
#include <filesystem>
#include "config.h"

namespace fs = std::filesystem;

struct article{
    size_t id;
    std::string title;
    std::string url;
    std::string text;
};

article read_article(std::ifstream &file){
    article wiki_article;
    size_t article_size;
    size_t str_size;
    file.read(reinterpret_cast<char *> (&article_size), sizeof(size_t));    //full size
    file.read(reinterpret_cast<char *> (&wiki_article.id), sizeof(size_t)); //id
    file.read(reinterpret_cast<char *> (&str_size), sizeof(size_t));        //title size
    wiki_article.title.resize(str_size);                                    
    file.read(&wiki_article.title[0], str_size);                            //title
    file.read(reinterpret_cast<char *> (&str_size), sizeof(size_t));        //url size
    wiki_article.url.resize(str_size);
    file.read(&wiki_article.url[0], str_size);                              //url
    file.read(reinterpret_cast<char *> (&str_size), sizeof(size_t));        //text size
    wiki_article.text.resize(str_size);
    file.read(&wiki_article.text[0], str_size);                             //text
    return wiki_article;
}

article read_article(const size_t id){
    article output = {0, "", "", ""};

    std::string filename = std::to_string(id / BIN_CORPUS_FILE_SIZE) + ".ind";
    std::ifstream file(std::string(CORPUS_DIR_PATH) + filename, std::ios_base::binary);
    if(file.good()){
        size_t article_size;
        size_t cur_id;
        while(file.peek() != EOF)
        {
            file.read(reinterpret_cast<char *> (&article_size), sizeof(size_t));    //full size
            file.read(reinterpret_cast<char *> (&cur_id), sizeof(size_t));  //id
            if(id == cur_id){
                file.seekg(-2*sizeof(size_t), std::ios_base::cur);
                output = read_article(file);
                return output;
            } else {
                file.seekg(article_size - sizeof(size_t), std::ios_base::cur);
            }
        }
    }
    file.close();
    return output;
}


void save_bin_article(std::ofstream &file, article &wiki_article){
    size_t article_size = sizeof(size_t)    // id
        + sizeof(size_t)    //title size
        + wiki_article.title.size() //title
        + sizeof(size_t)    //url size
        + wiki_article.url.size() //url
        + sizeof(size_t)    //text size
        + wiki_article.text.size(); //text
    
    size_t title_size = wiki_article.title.size();
    size_t url_size = wiki_article.url.size();
    size_t text_size = wiki_article.text.size();

    file.write(reinterpret_cast<char *>(&article_size), sizeof(size_t));
    file.write(reinterpret_cast<char *>(&wiki_article.id), sizeof(size_t));
    file.write(reinterpret_cast<char *>(&title_size), sizeof(size_t));
    file.write(wiki_article.title.c_str(), title_size);
    file.write(reinterpret_cast<char *>(&url_size), sizeof(size_t));
    file.write(wiki_article.url.c_str(), url_size);
    file.write(reinterpret_cast<char *>(&text_size), sizeof(size_t));
    file.write(wiki_article.text.c_str(), text_size);
}

size_t get_total_documents(){
    size_t total = 0;
    size_t last_file = 0;

    
    for (const auto & entry : fs::directory_iterator(CORPUS_DIR_PATH)){
        std::string filename(entry.path());
        filename = filename.substr(filename.rfind('/')+1);
        size_t num = std::stoul(filename.substr(0, filename.rfind('.')));
        if(num > last_file){
            last_file = num;
        }
    }
    std::string filename = std::to_string(last_file) + ".ind";
    std::ifstream file(std::string(CORPUS_DIR_PATH) + filename, std::ios_base::binary);
    if(file.good()){
        size_t article_size;
        while(file.peek() != EOF)
        {
            file.read(reinterpret_cast<char *> (&article_size), sizeof(size_t));    //full size
            file.read(reinterpret_cast<char *> (&total), sizeof(size_t));  //id
            file.seekg(article_size - sizeof(size_t), std::ios_base::cur);
        }
    }
    file.close();
    return total;
}