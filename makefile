clautext:
	g++ -std=c++1z ./source/ClauText.cpp ./source/global.cpp ./source/load_data.cpp ./source/main.cpp ./source/static_declaration.cpp -I./include -lpthread -pthread -O2 -o clautext