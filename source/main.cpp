

#define _CRT_SECURE_NO_WARNINGS

// memory leak test.
#ifdef _DEBUG 
//#include <vld.h>
#endif

// Log
#include <string>
#include <fstream>


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
	{
		wiz::load_data::UserType ut;

		wiz::load_data::LoadData::LoadDataFromFile3("C:/Users/vztpv/Documents/Paradox Interactive/Europa Universalis IV/save games/save1.eu4",
			ut, -1, 0);

		ut.clear();

		wiz::load_data::LoadData::LoadDataFromFile3("C:/Users/vztpv/Documents/Paradox Interactive/Europa Universalis IV/save games/save1.eu4",
			ut, -1, 0);

		return 0;
	}
	
	*/
	/*
	// Test for string tokenizer2.
	{
		wiz::StringBuilder builder(1024);
		std::vector<std::string> delim{ "fg", "aa" };
		// chk const char* str?
		wiz::StringTokenizer2 tokenizer("aacdefgfghijkabx", delim, &builder);
		
		while (tokenizer.hasMoreTokens()) {
			static int count = 0;
			count++;
			wiz::Out << count << " th " << tokenizer.nextToken() << ENTER;
		} 
	}	
	

	*/

	//srand(0);
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

	{// has bug..
		// temporary test.
	//	char* buffer = nullptr;
	//	wiz::load_data::LoadData::Node global;
	//	wiz::load_data::LoadData::LoadDataFromFile6(fileName, global, -1, 0, &buffer);
	//	delete[] buffer;
	//	wiz::load_data::LoadData::RemoveAll(&global);
	}

	try {
		int a = clock();
		{
			//	wiz::load_data::LoadData::LoadDataFromFile(fileName, global);
		}
		int b = clock();
		//wiz::Out << "time " << b - a << "ms" << ENTER;

	//	global.Remove();

		for (int i = 1; i <= 1; ++i) { // i : pivot_num, thread num <= pivot_num + 1
		//	global.Remove();

			a = clock();
			{
				auto thr_num = std::thread::hardware_concurrency();
				if (thr_num <= 0) {
					thr_num = 1; // ?
				}
				//char* buf = nullptr;
				//wiz::load_data::LoadData::LoadDataFromFile3_2(buf, fileName, global, 8, 8);
				// has bug? in debug x64
				//wiz::load_data::LoadData::LoadDataFromFile4(fileName, global, -1, 0); // , -1, 0); // , thr_num - 1, thr_num); // parallel? lexing + parallel parsing
				wiz::load_data::LoadData::LoadDataFromFile5_2(fileName, global, 0, 0);

			
			}
			b = clock();
			wiz::Out << "time " << b - a << "ms" << ENTER;
		}
		//global.Remove();

		a = clock();
		{
			//wiz::load_data::LoadData::LoadDataFromFile4(fileName, global); // parallel lexing + parallel parsing
		}
		b = clock();
		//wiz::Out << "time " << b - a << "ms" << ENTER;

		wiz::Out << "fileName is " << fileName << ENTER;

		//wiz::load_data::LoadData::SaveWizDB(global, "test2.eu4", "3"); // 3 : JSON

		wiz::Option opt;
		{
			int a = clock();
			wiz::Out.clear_file();
			std::string result = clauText.excute_module("", &global, wiz::ExcuteData(), opt, 0);
			int b = clock();
			wiz::Out << "excute result is " << result << "\n";
			wiz::Out << b - a << "ms" << "\n"; //
		}
	}
	catch (const char* str) {
		wiz::Out << str << ENTER;

		GETCH();
	}
	catch (const std::string& str) {
		wiz::Out << str << ENTER;
		GETCH();
	}
	//catch (const wiz::Error& e) {
	//	wiz::Out << e << ENTER;
	//	GETCH();
	//}
	catch (const std::exception&e) {
		wiz::Out << e.what() << ENTER;
		GETCH();
	}

#ifndef _DEBUG
	//catch (...) {
	//	wiz::Out << "UnKnown Error.." << ENTER;
	//	GETCH();
	//}
#endif

   	return 0;
}

