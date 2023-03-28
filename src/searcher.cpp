#include <iostream>
#include <fstream>
#include <string>
#include "parser.hpp"
#include <chrono>
#include "article.hpp"
#include "cord_index.hpp"

using namespace std;

int main() {
    std::unordered_map<size_t, std::vector<size_t>> map = load_cord_index(100);
    // Открываем файл для чтения
    std::cout << "ok" << std::endl;
    ifstream fin("files/in");
    string line;
    // Проверяем, открыт ли файл
    if (fin.is_open()) {
        std::cout << "ok" << std::endl;
        // Считываем первую строку из файла
        getline(fin, line);
        // Выводим считанную строку на экран
        cout << line << endl;
        // Закрываем файл
        fin.close();
    }
    
    auto tockens = parse_request(line);
    auto polsk_tockens = to_reverse_polish(tockens);
    auto ans = run_queue(polsk_tockens);


    ofstream fout("files/out");

    // Проверяем, открыт ли файл
    if (fout.is_open()) {
        // Записываем каждую строку массива в файл
        for (int i = 0; i < ans.size(); i++) {
            auto article = read_article(ans[i]);
            fout << article.title << "\\\\" <<  article.url << endl;
        }
        // Закрываем файл
        fout.close();
        cout << "Данные успешно записаны в файл" << endl;
    }

    return 0;
}