#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>

#include "json.hpp"
#include "article.hpp"
#include <chrono>

namespace fs = std::filesystem;
using json = nlohmann::json;


article parse_article_json(std::string filename, size_t id){
    std::ifstream json_file(filename);
    json data = json::parse(json_file);
    article wiki_article = {
        id,
        data["title"].get<std::string>(),
        data["url"].get<std::string>(),
        data["text"].get<std::string>(),
    };
    return wiki_article;
}


int main(int argc,char* argv[]) {
    std::vector<std::string> paths;
    for (const auto & entry : fs::directory_iterator(JSON_DIR_PATH)){
        paths.push_back(entry.path());
    }
    size_t percent_size = paths.size() / 10;
    std::ofstream outfile;
    size_t file_ind = -1;
    std::cout << "Create binary corpus" << std::endl;
    auto begin = std::chrono::steady_clock::now();
    for(size_t i = 1; i < paths.size(); ++i){
        // std::cout << paths[i] <<std::endl;
        if((i + 1) % percent_size == 0){
            std::cout << "Total: " << (i + 1) / percent_size * 10 << "%" << std::endl;
        }
        if(file_ind != i / BIN_CORPUS_FILE_SIZE){
            file_ind = i / BIN_CORPUS_FILE_SIZE;
            std::string filename = std::string(CORPUS_DIR_PATH) + std::to_string(file_ind) + ".ind";
            outfile.close();
            outfile.open(filename,std::ios_base::binary);
        }
        article wiki_arcticle = parse_article_json(paths[i], i);
        save_bin_article(outfile, wiki_arcticle);
    }
    auto end = std::chrono::steady_clock::now();
    outfile.close();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    std::cout << "The time: " << elapsed_ms.count() << " ms\n";
    return 0;
}