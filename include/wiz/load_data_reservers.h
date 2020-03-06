
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

		class Scanner
		{
		private:
			long long start;
			long long last;
		public:
			const wiz::LoadDataOption2* option;
			long long* token_arr;
			long long* token_arr_len;
			long long num;
			int* err;
			long long* arr_count;
			long long count;
			long long arr_count_size;
			char* buffer;
		public:
			Scanner(long long start, long long last, const wiz::LoadDataOption2* option,
				long long* token_arr, long long num, long long* token_arr_len, char* buffer, int* err, long long* arr_count, long long count, long long arr_count_size)
				: start(start), last(last), option(option), buffer(buffer), err(err), arr_count(arr_count), count(count), arr_count_size(arr_count_size)
			{
				this->token_arr = token_arr;
				this->num = num;
				this->token_arr_len = token_arr_len;
			}
			~Scanner() {
				//
			}
		public:
			void operator() () {
				//arr_count = nullptr;
				if (true) {
					long long token_arr_count = 0;
					long long start_idx = 0;
					long long last_idx = 0;
					long long token_first = start;
					long long token_last = start - 1; //?
					int state = 0;

					long long now_idx = 0;

					for (long long i = 0; start + i < last; ++i, ++now_idx) {
						const char x = buffer[start + i];
						const long long _x = start + i;
						long long offset = 0;
						int idx;

						if (arr_count_size > 0 && count >= 0 && count < arr_count_size && _x == arr_count[count]) {

							if (token_last - token_first + 1 > 0) {
								token_arr[token_arr_count] = ((start_idx + num) << 32) + ((token_last - token_first + 1) << 2) + 0;
								token_arr_count++;
								{
									if (token_last - token_first + 1 == 1) {
										if (buffer[start + start_idx] == option->Left) {
											token_arr[token_arr_count - 1] += 1;
										}
										if (buffer[start + start_idx] == option->Right) {
											token_arr[token_arr_count - 1] += 2;
										}
										if (buffer[start + start_idx] == option->Assignment) {
											token_arr[token_arr_count - 1] += 3;
										}
									}
								}
							}

							start_idx = i;
							token_first = arr_count[count];
							token_last = arr_count[count + 1]; // count : even index;

							if (token_last - token_first + 1 > 0) {
								token_arr[token_arr_count] = ((start_idx + num) << 32) + ((token_last - token_first + 1) << 2) + 0;
								token_arr_count++;
							}

							token_last = token_last + 1;
							token_first = token_last;

							i += (arr_count[count + 1] - arr_count[count]);

							now_idx = i;

							start_idx = i + 1;
							last_idx = i + 1;

							count += 2;
							continue;
						}
						else if (-1 != (idx = Equal2(option->Removal, x)))
						{
							token_last = _x - 1;
							last_idx = now_idx - 1;

							if (token_last - token_first + 1 > 0) {
								token_arr[token_arr_count] = ((start_idx + num) << 32) + ((token_last - token_first + 1) << 2) + 0;
								token_arr_count++;

								{
									if (token_last - token_first + 1 == 1) {
										if (buffer[start + start_idx] == option->Left) {
											token_arr[token_arr_count - 1] += 1;
										}
										if (buffer[start + start_idx] == option->Right) {
											token_arr[token_arr_count - 1] += 2;
										}
										if (buffer[start + start_idx] == option->Assignment) {
											token_arr[token_arr_count - 1] += 3;
										}
									}
								}

								token_first = _x + 1;
								start_idx = now_idx + 1;
							}
							else {
								token_first = token_first + 1;
								start_idx = start_idx + 1;
							}
							continue;
						}
						else if (-1 != (idx = Equal2(option->Assignment, x))) {
							token_last = _x - 1;
							last_idx = now_idx - 1;

							if (token_last - token_first + 1 > 0) {
								token_arr[token_arr_count] = ((start_idx + num) << 32) + ((token_last - token_first + 1) << 2) + 0;
								token_arr_count++;

								{
									if (token_last - token_first + 1 == 1) {
										if (buffer[start + start_idx] == option->Left) {
											token_arr[token_arr_count - 1] += 1;
										}
										if (buffer[start + start_idx] == option->Right) {
											token_arr[token_arr_count - 1] += 2;
										}
										if (buffer[start + start_idx] == option->Assignment) {
											token_arr[token_arr_count - 1] += 3;
										}
									}
								}

								token_first = _x + 1;
								start_idx = now_idx + 1;

								token_arr[token_arr_count] = ((i + num) << 32) + ((1) << 2) + 3;
								token_arr_count++;
							}
							else {
								token_arr[token_arr_count] = ((i + num) << 32) + ((1) << 2) + 3;
								token_arr_count++;

								token_first = token_first + 1;
								start_idx = start_idx + 1;
							}
						}
						else if (isWhitespace(x)) { // isspace ' ' \t \r \n , etc... ?
							token_last = _x - 1;
							last_idx = now_idx - 1;
							if (token_last - token_first + 1 > 0) {
								token_arr[token_arr_count] = ((start_idx + num) << 32) + ((token_last - token_first + 1) << 2) + 0;
								token_arr_count++;

								{
									if (token_last - token_first + 1 == 1) {
										if (buffer[start + start_idx] == option->Left) {
											token_arr[token_arr_count - 1] += 1;
										}
										if (buffer[start + start_idx] == option->Right) {
											token_arr[token_arr_count - 1] += 2;
										}
										if (buffer[start + start_idx] == option->Assignment) {
											token_arr[token_arr_count - 1] += 3;
										}
									}
								}

								token_first = _x + 1;

								start_idx = now_idx + 1;
							}
							else
							{
								token_first = token_first + 1;
								start_idx = start_idx + 1;
							}
						}
						else if (-1 != (idx = Equal2(option->Left, x))) {
							token_last = _x - 1;
							last_idx = now_idx - 1;
							if (token_last - token_first + 1 > 0) {
								token_arr[token_arr_count] = ((start_idx + num) << 32) + ((token_last - token_first + 1) << 2) + 0;
								token_arr_count++;

								{
									if (token_last - token_first + 1 == 1) {
										if (buffer[start + start_idx] == option->Left) {
											token_arr[token_arr_count - 1] += 1;
										}
										if (buffer[start + start_idx] == option->Right) {
											token_arr[token_arr_count - 1] += 2;
										}
										if (buffer[start + start_idx] == option->Assignment) {
											token_arr[token_arr_count - 1] += 3;
										}
									}
								}

								token_first = _x + 1;

								start_idx = now_idx + 1;

								token_arr[token_arr_count] = ((i + num) << 32) + ((1) << 2) + 1;

								token_arr_count++;
							}
							else {
								token_arr[token_arr_count] = ((i + num) << 32) + ((1) << 2) + 1;
								token_arr_count++;

								token_first = token_first + 1;
								start_idx = start_idx + 1;
							}
						}
						else if (-1 != (idx = Equal2(option->Right, x))) {
							token_last = _x - 1;
							last_idx = now_idx - 1;
							if (token_last - token_first + 1 > 0) {
								token_arr[token_arr_count] = ((start_idx + num) << 32) + ((token_last - token_first + 1) << 2) + 0;
								token_arr_count++;
								{
									if (token_last - token_first + 1 == 1) {
										if (buffer[start + start_idx] == option->Left) {
											token_arr[token_arr_count - 1] += 1;
										}
										if (buffer[start + start_idx] == option->Right) {
											token_arr[token_arr_count - 1] += 2;
										}
										if (buffer[start + start_idx] == option->Assignment) {
											token_arr[token_arr_count - 1] += 3;
										}
									}
								}

								token_first = _x + 1;
								start_idx = now_idx + 1;

								token_arr[token_arr_count] = ((i + num) << 32) + ((1) << 2) + 2;
								token_arr_count++;
							}
							else {
								token_arr[token_arr_count] = ((i + num) << 32) + ((1) << 2) + 2;
								token_arr_count++;

								token_first = token_first + 1;
								start_idx = start_idx + 1;
							}
						}

						token_last = _x + offset;

						last_idx = now_idx + offset;

					}

					if (token_first < last)
					{
						if (last - 1 - token_first + 1 > 0) {
							token_arr[token_arr_count] = ((start_idx + num) << 32) + ((last - 1 - token_first + 1) << 2) + 0;
							token_arr_count++;

							{
								if (last - 1 - token_first + 1 == 1) {
									if (buffer[start + start_idx] == option->Left) {
										token_arr[token_arr_count - 1] += 1;
									}
									if (buffer[start + start_idx] == option->Right) {
										token_arr[token_arr_count - 1] += 2;
									}
									if (buffer[start + start_idx] == option->Assignment) {
										token_arr[token_arr_count - 1] += 3;
									}
								}
							}
						}
					}

					if (this->token_arr_len) {
						*(this->token_arr_len) = token_arr_count;
					}

					if (state != 0) {
						*err = -3;
						return; // throw "Scanning Error in qouted string";
					}

					*err = 0;
					return;
				}
			}
		};


		class InFileReserver5
		{
		private:
			static void _func(const int dif, const char* text, const int length, long long* arr, long long* arr_count) {
				int _arr_count = 0;

				for (int i = 0; i < length; ++i) {
					switch (text[i]) {
					case '\"':
					case '\\':
						//case '=':
					case '\n':
					case '#': // line comment start char
						arr[_arr_count] = 1 + i + dif; // after called this function, arr[index] += -1?
						_arr_count += 1;
						break;
					}
				}

				*arr_count = _arr_count;
			}

			static void func(char* text, const int length, long long*& _arr_count, const int thr_num, long long& _arr_count_size) {
				long long* arr = (long long*)calloc(length + 1, sizeof(long long));// long[length];, null -> +1
				long long* arr_count = new long long[thr_num];
				long long* arr_start = new long long[thr_num];
				long count = -2;

				for (int i = 0; i < thr_num; ++i) {
					arr_start[i] = length / thr_num * i;
				}

				{
					std::thread* thr = new std::thread[thr_num];
					for (int i = 0; i < thr_num - 1; ++i) {
						thr[i] = std::thread(_func, length / thr_num * i, text + length / thr_num * i, length / thr_num, arr + length / thr_num * i, arr_count + i);
					}
					int last_length = length - length / thr_num * (thr_num - 1);
					thr[thr_num - 1] = std::thread(_func, length / thr_num * (thr_num - 1), text + length / thr_num * (thr_num - 1), last_length, arr + length / thr_num * (thr_num - 1), arr_count + thr_num - 1);
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
					int _count = 0;
					for (int i = 0; i < thr_num; ++i) {
						for (int j = 0, k = 0; j < arr_count[i]; ++j, ++k) {
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
					int _count = 0;
					int state = 0;
					int line_comment_start = 0;

					arr[count] = length; // text[arr[count]] == '\0'
					for (long long i = 0; i <= count; ++i) {
						const char ch = text[arr[i]];

						if (0 == state) {
							if ('#' == ch) {
								state = 3;
								line_comment_start = arr[i];
							}
							else if ('\"' == ch) {
								state = 1;

								arr[_count] = arr[i];

								// for abc"def"ghi
								for (long long j = arr[i]; j >= 0; --j) {
									bool pass = false;
									switch (text[j]) {
									case ' ':
									case '\t':
									case '\r':
									case '\n':
									case '\v':
									case '\f':
									case '\0':
									case '{':
									case '}':
									case '=':
										pass = true;
										break;
									}
									if (pass) {
										arr[_count] = j + 1;
										break;
									}
									if (0 == j) {
										arr[_count] = 0;
									}
								}
								_count++;
							}
						}
						else if (1 == state) {
							if ('\\' == ch) {
								state = 2;
							}
							else if ('\"' == ch) {
								state = 0;

								arr[_count] = arr[i];

								for (long long j = arr[i]; j <= length; ++j) {
									bool pass = false;
									switch (text[j]) {
									case ' ':
									case '\t':
									case '\r':
									case '\n':
									case '\v':
									case '\f':
									case '\0':
									case '{':
									case '}':
									case '=':
										pass = true;
										break;
									}
									if (pass) {
										arr[_count] = j - 1;
										break;
									}
								}

								_count++;
							}
						}
						else if (2 == state) {
							state = 1;
						}
						else if (3 == state) {
							if ('\n' == ch || '\0' == ch) {
								for (int j = line_comment_start; j < arr[i]; ++j) {
									text[j] = ' ';
								}

								state = 0;
							}
						}
					}

					count = _count;

					// odd case. -> error, count must be even!
					if (count % 2 == 1) {
						std::cout << "[" << count << "] " << "valid \"`s num is not even.\n"; //
					}
				}

				{ //debug
					//for (int i = 0; i < count; ++i) {
					//	std::cout << arr[i] << " ";
					//}
					//std::cout << "\n";
				}

				{
					delete[] arr_start;
					delete[] arr_count;
					_arr_count = arr;
					_arr_count_size = count;
				}
			}


			static std::pair<bool, int> Scan(std::ifstream& inFile, const int num, bool* isFirst, const wiz::LoadDataOption2& option, int thr_num,
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

				if (thr_num > 1) {
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
						func(buffer, file_length, arr_count, thr_num, arr_count_size); // todo - rename
						//int b = clock();
					//	std::cout << b - a << "ms\n";
					}


					start[0] = 0;

					for (int i = 1; i < thr_num; ++i) {
						start[i] = file_length / thr_num * i;

						for (long long x = start[i]; x <= file_length; ++x) {
							if (isWhitespace(buffer[x]) || '\0' == buffer[x] ||
								option.Left == buffer[x] || option.Right == buffer[x] || option.Assignment == buffer[x]) {
								start[i] = x;
								break;
							}
						}
					}
					for (int i = 0; i < thr_num - 1; ++i) {
						last[i] = start[i + 1];
						for (long long x = last[i]; x <= file_length; ++x) {
							if (isWhitespace(buffer[x]) || '\0' == buffer[x] ||
								option.Left == buffer[x] || option.Right == buffer[x] || option.Assignment == buffer[x]) {
								last[i] = x;
								break;
							}
						}
					}
					last[thr_num - 1] = file_length;

				}
				else {
					inFile.seekg(0, inFile.end);
					unsigned long long length = inFile.tellg();
					inFile.seekg(0, inFile.beg);

					file_length = length;
					buffer = new char[file_length + 1]; // 

					// read data as a block:
					inFile.read(buffer, file_length);
					buffer[file_length] = '\0';

					start[0] = 0;
					last[0] = file_length;

					{
						//int a = clock();
						func(buffer, file_length, arr_count, thr_num, arr_count_size);
						//int b = clock();
					//	std::cout << b - a << "ms\n";
					}

				}

				long long* token_arr = nullptr;

				if (thr_num > 1) {
					std::vector<std::thread> thr(thr_num);

					token_arr = new long long[file_length];

					std::vector<long long> counter(thr_num, 0);
					std::vector<int> err(thr_num, 0);

					std::vector<long long> count(thr_num, -1); // start_count?

					{
						long long start_count = 0;

						for (int i = 0; i < thr_num; ++i) {
							if (i > 0) {
								start[i] = last[i - 1];
								if (last[i] < start[i]) {
									last[i] = start[i];
								}
							}
							for (; start_count < arr_count_size; start_count += 2) {
								if (start[i] <= arr_count[start_count] && arr_count[start_count] < last[i]) {
									count[i] = start_count;

									if (last[i] <= arr_count[count[i] + 1]) {
										last[i] = arr_count[count[i] + 1] + 1;
									}
									break;
								}
								else if (arr_count[start_count] >= last[i]) {
									break;
								}
							}
						}
					}

					{
						//	for (int i = 0; i < thr_num; ++i) {
						//		std::cout << start[i] << " " << last[i] << "\n";
						//	}
					}

					for (int i = 0; i < thr_num; ++i) {
						thr[i] = std::thread(Scanner(start[i], last[i], &option,
							token_arr + start[i], start[i], &counter[i], buffer, &err[i], arr_count, count[i], arr_count_size));
					}

					for (int i = 0; i < thr_num; ++i) {
						thr[i].join();
					}

					{
						free(arr_count);//free(arr);
					}

					for (int i = 0; i < thr_num; ++i) {
						switch (err[i]) {
						case 0:
							// no errors.
							break;
						case -1:
							std::cout << "\'\' has enter key\n";
							break;
						case -2:
							std::cout << "\"\" has enter key\n";
							break;
						case -3:
							std::cout << "Scanning Error in qouted string\n";
							break;
						default:
							std::cout << "unknown scnner error\n";
							break;
						}
					}

					long long sum = 0;

					for (int i = 1; i < counter.size(); ++i) {
						sum += counter[i - 1];

						memmove(token_arr + sum, token_arr + start[i], counter[i] * sizeof(long long) / sizeof(char));
					}

					*_token_arr_len = sum + counter.back();
				}
				else {
					token_arr = new long long[file_length];

					long long len;
					int err = 0;
					long long count = 0;

					Scanner scanner(start[0], last[0], &option, token_arr, start[0], &len, buffer, &err, arr_count, count, arr_count_size);

					scanner();

					{
						free(arr_count);//free(arr);
					}

					switch (err) {
					case 0:
						// no errors.
						break;
					case -1:
						std::cout << "\'\' has enter key\n";
						break;
					case -2:
						std::cout << "\"\" has enter key\n";
						break;
					case -3:
						std::cout << "Scanning Error in qouted string\n";
						break;
					default:
						std::cout << "unknown scnner error\n";
						break;

					}
					*_token_arr_len = len;
				}

				_buffer = buffer;
				_token_arr = token_arr;
				*_buffer_len = file_length;

				return{ true, 1 };
			}

		private:
			std::ifstream* pInFile;
			bool isFirst;
		public:
			int Num;
		public:
			explicit InFileReserver5(std::ifstream& inFile)
			{
				pInFile = &inFile;
				Num = 1;
				isFirst = true;
			}
			bool end()const { return pInFile->eof(); } //
		public:
			bool operator() (const wiz::LoadDataOption2& option, int thr_num, char*& buffer, long long* buffer_len, long long*& token_arr, long long* token_arr_len)
			{
				bool x = Scan(*pInFile, Num, &isFirst, option, thr_num, buffer, buffer_len, token_arr, token_arr_len).second > 0;
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