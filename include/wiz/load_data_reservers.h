
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
				long long x = (position << 32) + (length << 3) + 0;

				if (length != 1) {
					return x;
				}

				switch (ch) {
				case '{':
					x += 2; // 010
					break;
				case '}':
					x += 4; // 100
					break;
				case '=':
					x += 6; // 110
					break;
				}

				return x;
			}

			static long long GetIdx(long long x) {
				return (x >> 32) & 0x00000000FFFFFFFF;
			}
			static long long GetLength(long long x) {
				return (x & 0x00000000FFFFFFF8) >> 3;
			}
			static long long GetType(long long x) { //to enum or enum class?
				return (x & 6) >> 1;
			}
			static bool IsToken2(long long x) {
				return (x & 1);
			}

			static void _Scanning(char* text, long long num, const long long length,
				long long*& token_arr, long long& _token_arr_size, const LoadDataOption2& option) {


				long long token_arr_size = 0;

				{
					int state = 0;

					long long token_first = 0;
					long long token_last = -1;

					long long token_arr_count = 0;

					for (long long i = 0; i < length; ++i) {
						const char ch = text[i];

						if ('\"' == ch) {

							token_last = i - 1;
							if (token_last - token_first + 1 > 0) {
								token_arr[num + token_arr_count] = Get(token_first + num, token_last - token_first + 1, text[token_first]);
								token_arr_count++;
							}

							token_first = i;
							token_last = i;

							token_first = i + 1;
							token_last = i + 1;

							{//
								token_arr[num + token_arr_count] = 1;
								token_arr[num + token_arr_count] += Get(i + num, 1, ch);
								token_arr_count++;
							}

						}
						else if ('\'' == ch) {

							token_last = i - 1;
							if (token_last - token_first + 1 > 0) {
								token_arr[num + token_arr_count] = Get(token_first + num, token_last - token_first + 1, text[token_first]);
								token_arr_count++;
							}

							token_first = i;
							token_last = i;

							token_first = i + 1;
							token_last = i + 1;

							{//
								token_arr[num + token_arr_count] = 1;
								token_arr[num + token_arr_count] += Get(i + num, 1, ch);
								token_arr_count++;
							}

						}
						else if ('\\' == ch) {
							{//
								token_arr[num + token_arr_count] = 1;
								token_arr[num + token_arr_count] += Get(i + num, 1, ch);
								token_arr_count++;
							}
						}
						else if ('\n' == ch) {
							token_last = i - 1;
							if (token_last - token_first + 1 > 0) {
								token_arr[num + token_arr_count] = Get(token_first + num, token_last - token_first + 1, text[token_first]);
								token_arr_count++;
							}
							token_first = i + 1;
							token_last = i + 1;

							{//
								token_arr[num + token_arr_count] = 1;
								token_arr[num + token_arr_count] += Get(i + num, 1, ch);
								token_arr_count++;
							}
						}
						else if ('\0' == ch) {
							token_last = i - 1;
							if (token_last - token_first + 1 > 0) {
								token_arr[num + token_arr_count] = Get(token_first + num, token_last - token_first + 1, text[token_first]);
								token_arr_count++;
							}
							token_first = i + 1;
							token_last = i + 1;

							{//
								token_arr[num + token_arr_count] = 1;
								token_arr[num + token_arr_count] += Get(i + num, 1, ch);
								token_arr_count++;
							}
						}
						else if (option.LineComment == ch) {
							token_last = i - 1;
							if (token_last - token_first + 1 > 0) {
								token_arr[num + token_arr_count] = Get(token_first + num, token_last - token_first + 1, text[token_first]);
								token_arr_count++;
							}
							token_first = i + 1;
							token_last = i + 1;

							{//
								token_arr[num + token_arr_count] = 1;
								token_arr[num + token_arr_count] += Get(i + num, 1, ch);
								token_arr_count++;
							}

						}
						else if (isWhitespace(ch)) {
							token_last = i - 1;
							if (token_last - token_first + 1 > 0) {
								token_arr[num + token_arr_count] = Get(token_first + num, token_last - token_first + 1, text[token_first]);
								token_arr_count++;
							}
							token_first = i + 1;
							token_last = i + 1;
						}
						else if (option.Left == ch) {
							token_last = i - 1;
							if (token_last - token_first + 1 > 0) {
								token_arr[num + token_arr_count] = Get(token_first + num, token_last - token_first + 1, text[token_first]);
								token_arr_count++;
							}

							token_first = i;
							token_last = i;

							token_arr[num + token_arr_count] = Get(token_first + num, token_last - token_first + 1, text[token_first]);
							token_arr_count++;

							token_first = i + 1;
							token_last = i + 1;
						}
						else if (option.Right == ch) {
							token_last = i - 1;
							if (token_last - token_first + 1 > 0) {
								token_arr[num + token_arr_count] = Get(token_first + num, token_last - token_first + 1, text[token_first]);
								token_arr_count++;
							}
							token_first = i;
							token_last = i;

							token_arr[num + token_arr_count] = Get(token_first + num, token_last - token_first + 1, text[token_first]);
							token_arr_count++;

							token_first = i + 1;
							token_last = i + 1;

						}
						else if (option.Assignment == ch) {
							token_last = i - 1;
							if (token_last - token_first + 1 > 0) {
								token_arr[num + token_arr_count] = Get(token_first + num, token_last - token_first + 1, text[token_first]);
								token_arr_count++;
							}
							token_first = i;
							token_last = i;

							token_arr[num + token_arr_count] = Get(token_first + num, token_last - token_first + 1, text[token_first]);
							token_arr_count++;

							token_first = i + 1;
							token_last = i + 1;
						}
					}

					if (length - 1 - token_first + 1 > 0) {
						token_arr[num + token_arr_count] = Get(token_first + num, length - 1 - token_first + 1, text[token_first]);
						token_arr_count++;
					}
					token_arr_size = token_arr_count;
				}

				{
					_token_arr_size = token_arr_size;
				}
			}

			static void ScanningNew(char* text, const long long length,
				long long*& _token_arr, long long& _token_arr_size, const LoadDataOption2& option)
			{
				const int thr_num = 8;
				std::thread thr[thr_num];
				long long start[thr_num];
				long long last[thr_num];

				{
					start[0] = 0;

					for (int i = 1; i < thr_num; ++i) {
						start[i] = length / thr_num * i;

						for (long long x = start[i]; x <= length; ++x) {
							if (isWhitespace(text[x]) || '\0' == text[x] ||
								option.Left == text[x] || option.Right == text[x] || option.Assignment == text[x]) {
								start[i] = x;
								break;
							}
						}
					}
					for (int i = 0; i < thr_num - 1; ++i) {
						last[i] = start[i + 1];
						for (long long x = last[i]; x <= length; ++x) {
							if (isWhitespace(text[x]) || '\0' == text[x] ||
								option.Left == text[x] || option.Right == text[x] || option.Assignment == text[x]) {
								last[i] = x;
								break;
							}
						}
					}
					last[thr_num - 1] = length + 1;
				}
				long long real_token_arr_count = 0;

				long long* tokens = new long long[length + 1];
				long long token_count = 0;

				long long token_arr_size[thr_num];

				for (int i = 0; i < thr_num; ++i) {
					thr[i] = std::thread(_Scanning, text + start[i], start[i], last[i] - start[i], std::ref(tokens), std::ref(token_arr_size[i]), option);
				}

				for (int i = 0; i < thr_num; ++i) {
					thr[i].join();
				}

				{
					long long _count = 0;
					for (int i = 0; i < thr_num; ++i) {
						for (long long j = 0; j < token_arr_size[i]; ++j) {
							tokens[token_count] = tokens[start[i] + j];
							token_count++;
						}
					}
				}

				int state = 0;
				long long qouted_start;
				long long slush_start;

				for (long long i = 0; i < token_count; ++i) {
					const long long len = GetLength(tokens[i]);
					const char ch = text[GetIdx(tokens[i])];
					const long long idx = GetIdx(tokens[i]);
					const bool isToken2 = IsToken2(tokens[i]);
					const long long type = GetType(tokens[i]);

					if (isToken2) {
						if (0 == state && '\"' == ch) {
							state = 1;
							qouted_start = i;
						}
						else if (0 == state && '\'' == ch) { // for $lint
							state = 4;
							qouted_start = i;
						}
						else if (4 == state && '\'' == ch) { 
							state = 0;

							{
								long long idx = GetIdx(tokens[qouted_start]);
								long long len = GetLength(tokens[qouted_start]);
								long long type = GetType(tokens[qouted_start]);

								len = GetIdx(tokens[i]) - idx + 1;

								
								if (qouted_start > 0 && 0 == GetType(tokens[qouted_start - 1])	&& !IsToken2(tokens[qouted_start - 1])
									&& GetIdx(tokens[qouted_start - 1]) + GetLength(tokens[qouted_start - 1]) == GetIdx(tokens[qouted_start])) {
									len += GetLength(tokens[qouted_start - 1]);
									idx = GetIdx(tokens[qouted_start - 1]);
								}
								if (i < token_count - 1 && 0 == GetType(tokens[i + 1]) && !IsToken2(tokens[i + 1]) 
									&& GetIdx(tokens[i + 1]) == GetIdx(tokens[i]) + GetLength(tokens[i])) {
									len += GetLength(tokens[i + 1]);
								}

								tokens[real_token_arr_count] = Get(idx, len, type);
								real_token_arr_count++;
							}
						}
						else if (0 == state && option.LineComment == ch) {
							state = 2;
						}
						else if (1 == state && '\\' == ch) {
							state = 3;
							slush_start = idx;
						}
						else if (1 == state && '\"' == ch) {
							state = 0;

							{
								long long idx = GetIdx(tokens[qouted_start]);
								long long len = GetLength(tokens[qouted_start]);
								long long type = GetType(tokens[qouted_start]);

								len = GetIdx(tokens[i]) - idx + 1;

								tokens[real_token_arr_count] = Get(idx, len, type);
								real_token_arr_count++;
							}
						}
						else if (3 == state) {
							if (idx != slush_start + 1) {
								--i;
							}
							state = 1;
						}
						else if (2 == state && ('\n' == ch || '\0' == ch)) {
							state = 0;
						}
					}
					else if (0 == state && !('\n' == ch || '\0' == ch)) { // '\\' case?
						// case :  abc'def ghi'abc
						if (i < token_count - 1 && 0 == type && IsToken2(tokens[i + 1]) && '\'' == text[GetIdx(tokens[i + 1])]
							&& idx + len == GetIdx(tokens[i+1])) {
							//
						}
						else {
							tokens[real_token_arr_count] = tokens[i];
							real_token_arr_count++;
						}
					}
				}

				{
					_token_arr = tokens;
					_token_arr_size = real_token_arr_count;
				}
			}

			static void Scanning(char* text, const long long length,
				long long*& _token_arr, long long& _token_arr_size, const LoadDataOption2& option) {

				long long* token_arr = new long long[length + 1];
				long long token_arr_size = 0;

				{
					int state = 0;

					long long token_first = 0;
					long long token_last = -1;

					long long token_arr_count = 0;

					for (long long i = 0; i <= length; ++i) {
						const char ch = text[i];

						if (0 == state) {
							if (option.LineComment == ch) {
								token_last = i - 1;
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
								token_last = i - 1;
								if (token_last - token_first + 1 > 0) {
									token_arr[token_arr_count] = Get(token_first, token_last - token_first + 1, text[token_first]);
									token_arr_count++;
								}
								token_first = i + 1;
								token_last = i + 1;
							}
							else if (option.Left == ch) {
								token_last = i - 1;
								if (token_last - token_first + 1 > 0) {
									token_arr[token_arr_count] = Get(token_first, token_last - token_first + 1, text[token_first]);
									token_arr_count++;
								}

								token_first = i;
								token_last = i;

								token_arr[token_arr_count] = Get(token_first, token_last - token_first + 1, text[token_first]);
								token_arr_count++;

								token_first = i + 1;
								token_last = i + 1;
							}
							else if (option.Right == ch) {
								token_last = i - 1;
								if (token_last - token_first + 1 > 0) {
									token_arr[token_arr_count] = Get(token_first, token_last - token_first + 1, text[token_first]);
									token_arr_count++;
								}
								token_first = i;
								token_last = i;

								token_arr[token_arr_count] = Get(token_first, token_last - token_first + 1, text[token_first]);
								token_arr_count++;

								token_first = i + 1;
								token_last = i + 1;

							}
							else if (option.Assignment == ch) {
								token_last = i - 1;
								if (token_last - token_first + 1 > 0) {
									token_arr[token_arr_count] = Get(token_first, token_last - token_first + 1, text[token_first]);
									token_arr_count++;
								}
								token_first = i;
								token_last = i;

								token_arr[token_arr_count] = Get(token_first, token_last - token_first + 1, text[token_first]);
								token_arr_count++;

								token_first = i + 1;
								token_last = i + 1;
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
							state = 1;
						}
						else if (3 == state) {
							if ('\n' == ch || '\0' == ch) {
								state = 0;

								token_first = i + 1;
								token_last = i + 1;
							}
						}
					}

					token_arr_size = token_arr_count;

					if (0 != state) {
						std::cout << "[" << state << "] state is not zero.\n";
					}
				}

				{
					_token_arr = token_arr;
					_token_arr_size = token_arr_size;
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

						ScanningNew(buffer, file_length, token_arr, token_arr_size, option);
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