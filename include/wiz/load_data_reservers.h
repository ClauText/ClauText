
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

		class InFileReserver3_2
		{
		private:
			std::ifstream* pInFile;
			bool isFirst;
		public:
			int Num;
		public:
			explicit InFileReserver3_2(std::ifstream& inFile)
			{
				pInFile = &inFile;
				Num = 1;
				isFirst = true;
			}
			bool end()const { return pInFile->eof(); } //
		public:
			bool operator() (VECTOR<Token3>* strVec, const wiz::LoadDataOption2& option, int thr_num, char*& buffer)
			{
				bool x = Utility::Reserve2_3_2(*pInFile, strVec, Num, &isFirst, option, thr_num, buffer).second > 0;
				return x;
			}
		};

		class InFileReserver4
		{
		private:
			std::ifstream* pInFile;
			bool isFirst;
		public:
			int Num;
		public:
			explicit InFileReserver4(std::ifstream& inFile)
			{
				pInFile = &inFile;
				Num = 1;
				isFirst = true;
			}
			bool end()const { return pInFile->eof(); } //
		public:
			bool operator() (const wiz::LoadDataOption2& option, int thr_num, char*& buffer, long long*& llptr, long long* buffer_len, long long*& llptr2, long long* llptr2_len,
				wiz::load_data::Utility::UT_IT_NUM*& llptr3)
			{
				bool x = Utility::Reserve2_4(*pInFile, Num, &isFirst, option, thr_num, buffer, llptr, buffer_len, llptr2, llptr2_len, llptr3).second > 0;
				return x;
			}
		};

		class InFileReserver5
		{
		private:
			// todo - rename.
			static long long Get(long long position, long long length, char ch) {
				long long x = (position << 32) + (length << 2) + 0;

				if (length != 1) {
					return x;
				}

				switch (ch) {
				case '{':
					x += 1;
					break;
				case '}':
					x += 2;
					break;
				case '=':
					x += 3;
					break;
				}

				return x;
			}

			static void preScanning(const long long dif, const char* text, const long long length, long long* arr, long long* arr_count) {
				int _arr_count = 0;

				for (long long i = 0; i < length; ++i) {
					switch (text[i]) {
					case ' ':
					case '\t':
					case '\r':
					case '\n':
					case '\v':
					case '\f':
					case '\"':
					case '\\':
					case '=':
					case '{':
					case '}':
					case '#': // line comment start char
						arr[_arr_count] = 1 + i + dif; // after called this function, arr[index] += -1?
						_arr_count++;
						break;
					}
				}

				*arr_count = _arr_count;
			}

			static void Scanning(char* text, const long long length,
				const int thr_num,
				long long*& _token_arr, long long& _token_arr_size) {

				long long* token_arr = new long long[length + 1];
				long long token_arr_size = 0;
				long long* arr = (long long*)calloc(length + 1, sizeof(long long));// long[length];, null -> +1
				long long* arr_count = new long long[thr_num];
				long long* arr_start = new long long[thr_num];
				long long count = -2;

				for (int i = 0; i < thr_num; ++i) {
					arr_start[i] = length / thr_num * i;
				}

				{
					std::thread* thr = new std::thread[thr_num];
					for (int i = 0; i < thr_num - 1; ++i) {
						thr[i] = std::thread(preScanning, length / thr_num * i, text + length / thr_num * i, length / thr_num, arr + length / thr_num * i, arr_count + i);
					}
					int last_length = length - length / thr_num * (thr_num - 1);
					thr[thr_num - 1] = std::thread(preScanning, length / thr_num * (thr_num - 1), text + length / thr_num * (thr_num - 1), last_length, arr + length / thr_num * (thr_num - 1), arr_count + thr_num - 1);
					for (int i = 0; i < thr_num; ++i) {
						thr[i].join();
					}

					delete[] thr;
				}
				{ // debug
					//for (int i = 0; i < length; ++i) {
					//	std::cout << arr[i] << " ";
					//}
					//std::cout << "\n";
				}

				{
					long long _count = 0;
					for (int i = 0; i < thr_num; ++i) {
						for (long long j = 0, k = 0; j < arr_count[i]; ++j, ++k) {
							// pass zero.. 
							if (0 == arr[arr_start[i] + k]) {
								--j;
								break;
							}
							arr[_count] = arr[arr_start[i] + k] - 1; // chk this.
							_count++;
						}
					}
					count = _count;
				}

				{ // debug
					//std::cout << "debug2\n";
					//for (int i = 0; i < count; ++i) {
					//	std::cout << arr[i] << " ";
					//}
					//std::cout << "\n";
				}

				{ // todo - add error check!
					long long _count = 0;
					int state = 0;

					long long token_first = 0;
					long long token_last = -1;

					long long token_arr_count = 0;

					arr[count] = length; // text[arr[count]] == '\0'
					for (long long i = 0; i <= count; ++i) {
						const char ch = text[arr[i]];

						if (0 == state) {
							if ('#' == ch) {
								token_last = arr[i] - 1;
								if (token_last - token_first + 1 > 0) {
									token_arr[token_arr_count] = Get(token_first, token_last - token_first + 1, text[token_first]);
									token_arr_count++;
								}

								state = 3;
							}
							else if ('\"' == ch) {
								state = 1;
							}
							else if (isWhitespace(ch) || '\0' == ch) {
								token_last = arr[i] - 1;
								if (token_last - token_first + 1 > 0) {
									token_arr[token_arr_count] = Get(token_first, token_last - token_first + 1, text[token_first]);
									token_arr_count++;
								}
								token_first = arr[i] + 1;
								token_last = arr[i] + 1;
							}
							else if ('{' == ch) {
								token_last = arr[i] - 1;
								if (token_last - token_first + 1 > 0) {
									token_arr[token_arr_count] = Get(token_first, token_last - token_first + 1, text[token_first]);
									token_arr_count++;
								}

								token_first = arr[i];
								token_last = arr[i];

								token_arr[token_arr_count] = Get(token_first, token_last - token_first + 1, text[token_first]);
								token_arr_count++;

								token_first = arr[i] + 1;
								token_last = arr[i] + 1;
							}
							else if ('}' == ch) {
								token_last = arr[i] - 1;
								if (token_last - token_first + 1 > 0) {
									token_arr[token_arr_count] = Get(token_first, token_last - token_first + 1, text[token_first]);
									token_arr_count++;
								}
								token_first = arr[i];
								token_last = arr[i];

								token_arr[token_arr_count] = Get(token_first, token_last - token_first + 1, text[token_first]);
								token_arr_count++;

								token_first = arr[i] + 1;
								token_last = arr[i] + 1;

							}
							else if ('=' == ch) {
								token_last = arr[i] - 1;
								if (token_last - token_first + 1 > 0) {
									token_arr[token_arr_count] = Get(token_first, token_last - token_first + 1, text[token_first]);
									token_arr_count++;
								}
								token_first = arr[i];
								token_last = arr[i];

								token_arr[token_arr_count] = Get(token_first, token_last - token_first + 1, text[token_first]);
								token_arr_count++;

								token_first = arr[i] + 1;
								token_last = arr[i] + 1;
							}
						}
						else if (1 == state) {
							if ('\\' == ch) {
								state = 2;
							}
							else if ('\"' == ch) {
								state = 0;
							}
						}
						else if (2 == state) {
							if (arr[i] > arr[i - 1] + 1) {
								--i;
							}
							state = 1;
						}
						else if (3 == state) {
							if ('\n' == ch || '\0' == ch) {
								state = 0;

								token_first = arr[i] + 1;
								token_last = arr[i] + 1;
							}
						}
					}

					token_arr_size = token_arr_count;

					count = _count;

					if (0 != state) {
						std::cout << "[" << state << "] state is not zero.\n";
					}
				}

				{ //debug
					//for (int i = 0; i < count; ++i) {
					//	std::cout << arr[i] << " ";
					//}
					//std::cout << "\n";
				}

				{
					_token_arr = token_arr;
					_token_arr_size = token_arr_size;

					delete[] arr_start;
					delete[] arr_count;
					free(arr);
				}
			}


			static std::pair<bool, int> Scan(std::ifstream& inFile, const int num, const wiz::LoadDataOption2& option, int thr_num,
				char*& _buffer, long long* _buffer_len, long long*& _token_arr, long long* _token_arr_len)
			{
				if (inFile.eof()) {
					return { false, 0 };
				}

				long long* arr_count = nullptr; //
				long long arr_count_size = 0;

				std::string temp;
				char* buffer = nullptr;
				std::vector<long long> start(thr_num + 1, 0);
				std::vector<long long> last(thr_num + 1, 0);
				std::vector<long long> token_arr_len(thr_num + 1, 0);
				long long file_length;

				{
					inFile.seekg(0, inFile.end);
					unsigned long long length = inFile.tellg();
					inFile.seekg(0, inFile.beg);

					file_length = length;
					buffer = new char[file_length + 1]; // 

					// read data as a block:
					inFile.read(buffer, file_length);

					buffer[file_length] = '\0';

					{
						//int a = clock();
						long long* token_arr;
						long long token_arr_size;

						Scanning(buffer, file_length, thr_num, token_arr, token_arr_size); // todo - rename
						//int b = clock();
					//	std::cout << b - a << "ms\n";
						_buffer = buffer;
						_token_arr = token_arr;
						*_token_arr_len = token_arr_size;
						*_buffer_len = file_length;
					}
				}

				return{ true, 1 };
			}

		private:
			std::ifstream* pInFile;
		public:
			int Num;
		public:
			explicit InFileReserver5(std::ifstream& inFile)
			{
				pInFile = &inFile;
				Num = 1;
			}
			bool end()const { return pInFile->eof(); } //
		public:
			bool operator() (const wiz::LoadDataOption2& option, int thr_num, char*& buffer, long long* buffer_len, long long*& token_arr, long long* token_arr_len)
			{
				bool x = Scan(*pInFile, Num, option, thr_num, buffer, buffer_len, token_arr, token_arr_len).second > 0;
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