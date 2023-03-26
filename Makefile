.PHONY: index

clean_files:
	rm -f files/corpus/*.ind
	rm -f files/reverse_index/*.ind
	rm -f files/dictionary.words

clean:
	rm -f corpus_generator
	rm -f dictionary_generator
	rm -f reverse_index_generator

build_corpus: src/bin_corpus_generator.cpp src/json.hpp src/article.hpp src/config.h
	g++ src/bin_corpus_generator.cpp -o corpus_generator

build_dictionary: src/dictionary_generator.cpp src/article.hpp src/config.h
	g++ src/dictionary_generator.cpp -o dictionary_generator

build_reverse_index: src/reverse_index_generator.cpp src/reverse_index.hpp src/dictionary.hpp src/article.hpp src/config.h
	g++  src/reverse_index_generator.cpp -o reverse_index_generator

build: build_corpus build_dictionary build_reverse_index

generate_corpus: build_corpus
	rm -f files/corpus/*.ind
	./corpus_generator

generate_dictionary: build_dictionary
	rm -f files/dictionary.words
	./dictionary_generator

generate_reverse_index: build_reverse_index
	rm -f files/reverse_index/*.ind
	rm -f files/cord_index/*.ind
	./reverse_index_generator

generate_all: generate_corpus generate_dictionary generate_reverse_index