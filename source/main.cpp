﻿

#define _CRT_SECURE_NO_WARNINGS

// memory leak test.
#ifdef _DEBUG 
#include <vld.h>
#endif

// Log
#include <string>
#include <fstream>

namespace wiz 
{
	const std::string LOG_FILE_NAME = "clautext_log.txt";
	const std::string ENTER = "\n";

	class Log {
	private:
		std::ofstream outFile;
	public:
		Log() {
			// 
		}
		virtual ~Log() {
			//
		}

		Log& Open(const std::string& fileName)
		{
			outFile.open(fileName);
			if (!outFile) {
				throw "outFile error in open function in Log class";
			}
			return *this;
		}
		Log& Error() {
			outFile << "[ERROR] ";
			return *this;
		}
		Log& Message() {
			outFile << "[Message] ";
			return *this;
		}
		friend Log& operator<<(Log& log, const std::string& str)
		{
			log.outFile << str;
			return log;
		}
		void Close()
		{
			outFile.close();
		}
	};

	Log log;
}


// Array idx chk test.
//#define ARRAYS_DEBUG

#include <iostream>
#include <ctime>
#include <cstdlib>

//#define USE_FAST_LOAD_DATA // no use?
#include "wiz/ClauText.h"


int main(int argc, char* argv[])
{
	/*
	// Test for string tokenizer.
	{
		wiz::StringBuilder builder(1024);
		std::vector<std::string> delim{ "fg", "aa" };
		// chk const char* str?
		wiz::StringTokenizer tokenizer("aacdefghijkabx", delim, &builder);
		
		while (tokenizer.hasMoreTokens()) {
			std::cout << tokenizer.nextToken() << std::endl;
		} 
	}	
	*/

	srand(time(nullptr)); // 

	std::string fileName;


	if (argc == 1) {
		std::cout << "FileName: ";
		std::getline(std::cin, fileName);
	}
	else
	{
		fileName = std::string(argv[1]);
	}

	wiz::load_data::UserType global;

	try {
		int a = clock();
		{
		//	wiz::load_data::LoadData::LoadDataFromFile(fileName, global);
		}
		int b = clock();
		//std::cout << "time " << b - a << "ms" << std::endl;

		global.Remove();

		for (int i = 4; i <= 4; ++i) { // i : pivot_num, thread num <= pivot_num + 1
			global.Remove();
			
			a = clock();
			{
				auto thr_num = std::thread::hardware_concurrency();
				if (thr_num <= 0) {
					thr_num = 1; // ?
				}
				//char* buf = nullptr;
				//wiz::load_data::LoadData::LoadDataFromFile3_2(buf, fileName, global, 8, 8);
				wiz::load_data::LoadData::LoadDataFromFile(fileName, global); // , thr_num - 1, thr_num); // parallel? lexing + parallel parsing
			}
			b = clock();
			//std::cout << "time " << b - a << "ms" << std::endl;
		}
		//global.Remove();

		a = clock(); 
		{
			//wiz::load_data::LoadData::LoadDataFromFile4(fileName, global); // parallel lexing + parallel parsing
		}
		b = clock();
		//std::cout << "time " << b - a << "ms" << std::endl;

		std::cout << "fileName is " << fileName << std::endl;
	
		//wiz::load_data::LoadData::SaveWizDB(global, "test2.eu4", "3"); // 3 : JSON
		wiz::Option opt;
		std::string result = clauText.excute_module("", &global, wiz::ExcuteData(), opt, 0);
		std::cout << "excute result is " << result << std::endl;
	}
	catch (const char* str) {
		std::cout << str << std::endl;

		GETCH();
	}
	catch (const std::string& str) {
		std::cout << str << std::endl;
		GETCH();
	}
	catch (const wiz::Error& e) {
		std::cout << e << std::endl;
		GETCH();
	}
	catch (std::exception e) {
		std::cout << e.what() << std::endl;
		GETCH();
	}

#ifndef _DEBUG
	catch (...) {
		std::cout << "UnKnown Error.." << std::endl;
		GETCH();
	}
#endif

   	return 0;
}
