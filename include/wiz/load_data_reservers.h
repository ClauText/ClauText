
#ifndef LOAD_DATA_RESERVERS_H
#define LOAD_DATA_RESERVERS_H

#include <vector>
#include <fstream>
#include <string>

#include "cpp_string.h"
#include "load_data_utility.h"
#include "queues.h"

namespace wiz {
	namespace load_data {
		class InFileReserver
		{
		private:
			std::ifstream* pInFile;
		public:
			int Num;
		public:
			explicit InFileReserver(std::ifstream& inFile)
			{
				pInFile = &inFile;
				Num = 1;
			}
			bool end()const { return pInFile->eof(); }
		public:
			bool operator() (ARRAY_QUEUE<Token>& strVec, const wiz::LoadDataOption& option)
			{
				return Utility::Reserve2(*pInFile, strVec, Num, option).second > 0;
			}
		};

		class InFileReserver2
		{
		private:
			std::ifstream* pInFile;
		public:
			int Num;
			int CoreNum;
		public:
			explicit InFileReserver2(std::ifstream& inFile, int num_of_core)
			{
				pInFile = &inFile;
				Num = 1;
				CoreNum = num_of_core;
			}
			bool end()const { return pInFile->eof(); }
		public:
			bool operator() (std::vector<ARRAY_QUEUE<Token>>& strVec, const wiz::LoadDataOption& option, const int offset)
			{
				return Utility::Reserve2_2(*pInFile, strVec, Num, option, CoreNum, offset).second > 0;
			}
		};

		class InFileReserver3
		{
		private:
			std::ifstream* pInFile;
			bool isFirst;
		public:
			int Num;
		public:
			explicit InFileReserver3(std::ifstream& inFile)
			{
				pInFile = &inFile;
				Num = 1;
				isFirst = true;
			}
			bool end()const { return pInFile->eof(); } //
		public:
			bool operator() (VECTOR<Token2>* strVec, const wiz::LoadDataOption& option, int thr_num, char*& buffer)
			{
				bool x = Utility::Reserve2_3(*pInFile, strVec, Num, &isFirst, option, thr_num, buffer).second > 0;
				return x;
			}
		};



		class NoneReserver
		{
		private:
			int count;
		public:
			explicit NoneReserver() : count(0) { }

			bool operator() (ARRAY_QUEUE<Token>&, const wiz::LoadDataOption&)
			{
				count = 1;
				return false;
			}
			bool operator() (VECTOR<Token2>*, const wiz::LoadDataOption&, int, char*& buffer)
			{
				count = 1;
				return false;
			}

			bool end() const { return 1 == count; }
		};
	}
}

#endif