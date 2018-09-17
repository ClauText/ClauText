﻿#ifndef CPP_STRING_H_INCLUDED
#define CPP_STRING_H_INCLUDED

#define _CRT_SECURE_NO_WARNINGS

#include "global.h"
#include "STRINGBUILDER.H"
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>

namespace wiz{
	class String {
	public:
		static int find(const std::string& str, const std::string& dilim, const int start_index)
		{
			if (str.empty() || dilim.empty()) { return -1; }

			int k = 0;

			for (int i = start_index; i < str.size(); i++) {
				if (str[i] == dilim[k]) { k++; if (dilim.size() == k) { return i - dilim.size() + 1; } }
				else {
					k = 0;
					if (str[i] == dilim[k]) { k++; if (dilim.size() == k) { return i - dilim.size() + 1; } }
				}
			}

			return -1;
		}
		static bool Comp(const char* cstr1, const char* cstr2, const int n) /// isSameData
		{
			for (int i = 0; i < n; i++)
			{
				if (cstr1[i] != cstr2[i]) { return false; }
			}
			return true;
		}
		static bool Comp(const std::string& str1, const std::string& str2, const int n) /// isSameData
		{
			for (int i = 0; i < n; i++)
			{
				if (str1[i] != str2[i]) { return false; }
			}
			return true;
		}
		// fixed more fast!
		static std::string replace(const std::string& str, const std::string& str1, const std::string& str2)
		{
			if (str1.empty()) { return str; }

			std::string temp = str;

			if (temp.size() < str1.size()) {
				throw "Error in replace";
			}

			for (long long i = temp.size() - str1.size(); i >= 0;) {
				i = temp.rfind(str1.c_str(), i);

				if (i == std::string::npos) {
					break;
				}
				else {
					temp.replace(temp.begin() + i, temp.begin() + i + str1.size(), str2.c_str());
					--i;
				}
			}

			return temp;

			/*
			// if str1 is empty, then infinite loop. // USE more speed pointer?
			const char* pStr = str.c_str();
			std::string result;

			for( int i=0; i < str.size(); i++ ) {
				if( strlen( pStr+i ) >= str1.size()
				   && Comp( pStr+i, str1.c_str(), str1.size() ) ) // check!
				{
					result += str2; //
					i = i + str1.size() - 1;
				}
				else
				{
					result.push_back( str[i] );
				}
			}

			return result;
			*/
		}
		static std::string remove(const std::string& str, const std::string& str1)
		{
			return replace(str, str1, std::string(""));
		}
		//start index
		static int indexOf(const std::string& str, const char ch)
		{
			for (int i = 0; i < str.size(); i++)
			{
				if (str[i] == ch) { return i; }
			}
			return -1;
		}
		// chk..
		static std::pair< bool, size_t > indexOf(const std::string& str, const std::string& str1, const int fromIndex)
		{
			if (str1.empty()) { return{ false, 0 }; }
			auto idx = str.find(str1, fromIndex);
			if (idx == std::string::npos) { return{ false, 0 }; }
			return{ true, idx };
		}
		static auto indexOf(const std::string& str, const std::string& str1)
		{
			return indexOf(str, str1, 0);
		}
		/// chk....
		static std::vector<std::string> split(const std::string& str, const std::string& filter)
		{
			if (filter.empty()) {
				std::vector<std::string> result;
				if (!str.empty()) {
					result.push_back(str);
				}
				return result;
			}
			/// example...
			/// www.naver.com -> www, naver, com
			/// .www.naver.com. -> www, naver, com!!
			const char* pStr = str.c_str();
			char* pTemp = new char[str.size() + 1];
			std::vector<std::string> result;

			strcpy(pTemp, str.c_str());

			for (int i = 0; i <= str.size() - filter.size(); i++) {
				if (strlen(pStr + i) >= filter.size()
					&& Comp(pStr + i, filter.c_str(), filter.size()))
				{ // finded..
					pTemp[i] = '\0';
					for (int j = i + 1; j < i + filter.size(); j++)
					{
						pTemp[j] = '\0';
					}
					i = i + filter.size() - 1;
				}
			}
			for (int i = 0; i < str.size(); i++)
			{
				if (pTemp[i] != '\0')
				{
					result.push_back(std::string(pTemp + i));
					i = i + strlen(pTemp + i) - 1;
				}
			}

			// free
			delete[] pTemp;
			return result;
		}
		static std::string substring(const std::string& str, const int start, const int last)
		{
			return str.substr(start, last - start + 1);
		}
		static std::string substring(const std::string& str, const int start)
		{
			return substring(str, start, str.size() - 1);
		}
		static bool startsWith(const std::string& str, const std::string& start)
		{
			if (str.size() < start.size()) { return false; }
			return Comp(str.data(), start.c_str(), start.size());
		}
		static bool endsWith(const std::string& str, const std::string& last)
		{
			if (str.size() < last.size()) { return false; }
			if (last.empty()) { return true; } /// chk... return false; 
			return Comp(str.data() + (str.size() - last.size()), last.c_str(), last.size());
		}
	};

	class StringTokenizer
	{
	private:
		std::vector<std::string> _m_str;
		int count;
		bool exist;
		static const std::vector<std::string> whitespaces;
		int option;
	private:
		int FastFind(const char* str, const int n, const std::string& pat, const std::vector<int>& f, int min) {
			int posP = 0, posS = 0;
			const int lengthP = pat.size();
			const int lengthS = n;

			while (posP < lengthP && posS < lengthS) {
				// optimized!
				if (posS - lengthP > min) {
					return -1;
				}
				
				if (pat[posP] == str[posS]) {
					++posP; ++posS;
				}
				else {
					if (0 == posP) { 
						++posS; 
					}
					else {
						posP = f[posP - 1] + 1;
					}
				}
			}
			if (posP < lengthP) { return -1; }
			else {
				return posS - lengthP;
			}
		}
		
		// maybe this function has bug, when separator[idx].size() > 1?
		//  "abc   ??
		void Init(const std::string& str, const std::vector<std::string>& separator, StringBuilder* builder) // assumtion : separators are sorted by length?, long -> short
		{
			const int sep_size = separator.size();
			const int str_size = str.size();
			if (separator.empty() || str.empty()) { return; } // if str.empty() == false then _m_str.push_back(str); // ?
															  //
			builder->Clear();
			builder->Append(str.c_str(), str.size());

			_m_str.reserve(str.size() / 2);
			int left = 0;
			int right = 0;
			int state = 0;
			this->exist = false;

			{
				std::vector<std::vector<int>> f(sep_size);
				for (int t = 0; t < sep_size; ++t) {
					f[t] = std::vector<int>(separator[t].size(), -1);
					const int lengthP = f[t].size();
					for (int j = 1; j < lengthP; ++j) {
						int i = f[t][j - 1];
						while (str[j] != str[i + 1] && i >= 0) {
							i = f[t][i];
						}
						if (str[j] == str[i + 1]) {
							f[t][j] = i + 1;
						}
						else {
							f[t][j] = -1;
						}
					}
				}

				for (int i = 0; i < str.size(); ++i) {
					right = i;
					int _select = -1;
					bool pass = false;

					if (1 == option && 0 != state && str.size() - 1 == i) {
						//
					}
					else if (1 == option && 0 == state && '\'' == str[i]) {
						if (i == 0) {
							state = 2;
							continue;
						}
						else if (i > 0 && '\\' == str[i - 1]) {
							throw "ERROR for Init 1 on StringTokenizer"; // error!
						}
						else if (i > 0) {
							state = 2;
							continue;
						}
					}
					else if (1 == option && 2 == state && '\'' == str[i]) {
						if ('\\' == str[i - 1]) {
							continue;
						}
						else {
							state = 0;
							if (i == str.size() - 1) {
								if (right - left + 1 > 0) {
									_m_str.emplace_back(builder->Divide(right - left + 1), right - left + 1);
									builder->LeftShift(right - left + 2);
								}
							}
							continue;
						}
						continue;
					}
					else if (1 == option && 2 == state) {
						continue;
					}
					else if (1 == option && 0 == state && '\"' == str[i]) {
						if (i == 0) {
							state = 1;
							continue;
						}
						else if (i > 0 && '\\' == str[i - 1]) {
							throw "ERROR for Init 2 on StringTokenizer"; // error!
						}
						else if (i > 0) {
							state = 1;
							continue;
						}
					}
					else if (1 == option && 1 == state && '\"' == str[i]) {
						if ('\\' == str[i - 1]) {
							continue;
						}
						else {
							state = 0;
							if (i == str.size() - 1) {
								if (right - left + 1 > 0) {
									_m_str.emplace_back(builder->Divide(right - left + 1), right - left + 1);
									builder->LeftShift(right - left + 1);
								}
							}
							continue;
						}
					}
					else if (1 == option && 1 == state) {
						continue;
					}
					

					int min = INT_MAX, min_idx = -1;

					for (int j = 0; j < sep_size; ++j) {
						const int idx = FastFind(str.c_str() + i, str.size() - i, separator[j], f[j], min);

						if (-1 < idx) { 
							pass = true; 
							if (-1 == min_idx) {
								min = idx;
								min_idx = j;
							}
							else if (min > idx) {
								min = idx;
								min_idx = j;
							}
						}
					}
					if (min_idx > -1) {
						i = i + min;
						_select = min_idx;
					}

					if (!pass) {
						i = str.size() - 1;
					}

					right = i;

					if (pass) {
						this->exist = true;

						if (right - left + 1 >= 1) {
							const char* temp = builder->Divide(right - left);

							if (right - left + 1 > 1) {
								_m_str.emplace_back(temp, right - left);
								builder->LeftShift(right - left + separator[_select].size());
							}
							else {
								builder->LeftShift(separator[_select].size());
							}
						}

						i = i + separator[_select].size() - 1;

						left = right + separator[_select].size();
						right = left;
					}
					else if (!pass && i == str.size() - 1) {
						if (right - left + 1 > 0) {
							_m_str.emplace_back(builder->Divide(right - left + 1), right - left + 1);
						//	std::cout << "chk " << separator[0] << " | " << str << " " << _m_str.back() << std::endl;
							builder->LeftShift(right - left + 1);
						}
						else {
							//GETCH();
						}
					}
				}
			}
			//cout << "str is " << str <<  " state  is " << state << endl;
		}
		void Init(std::string&& str, const std::vector<std::string>& separator, StringBuilder* builder) // assumtion : separators are sorted by length?, long -> short
		{
			const int sep_size = separator.size();
			const int str_size = str.size();
			if (separator.empty() || str.empty()) { return; } // if str.empty() == false then _m_str.push_back(str); // ?
															  //
			builder->Clear();
			builder->Append(str.c_str(), str.size());

			_m_str.reserve(str.size() / 2);
			int left = 0;
			int right = 0;
			int state = 0;
			this->exist = false;

			{
				std::vector<std::vector<int>> f(sep_size);
				for (int t = 0; t < sep_size; ++t) {
					f[t] = std::vector<int>(separator[t].size(), -1);
					const int lengthP = f[t].size();
					for (int j = 1; j < lengthP; ++j) {
						int i = f[t][j - 1];
						while (str[j] != str[i + 1] && i >= 0) {
							i = f[t][i];
						}
						if (str[j] == str[i + 1]) {
							f[t][j] = i + 1;
						}
						else {
							f[t][j] = -1;
						}
					}
				}

				for (int i = 0; i < str.size(); ++i) {
					right = i;
					int _select = -1;
					bool pass = false;

					if (1 == option && 0 != state && str.size() - 1 == i) {
						//
					}
					else if (1 == option && 0 == state && '\'' == str[i]) {
						if (i == 0) {
							state = 2;
							continue;
						}
						else if (i > 0 && '\\' == str[i - 1]) {
							throw "ERROR for Init 1 on StringTokenizer"; // error!
						}
						else if (i > 0) {
							state = 2;
							continue;
						}
					}
					else if (1 == option && 2 == state && '\'' == str[i]) {
						if ('\\' == str[i - 1]) {
							continue;
						}
						else {
							state = 0;
							if (i == str.size() - 1) {
								if (right - left + 1 > 0) {
									_m_str.emplace_back(builder->Divide(right - left + 1), right - left + 1);
									builder->LeftShift(right - left + 2);
								}
							}
							continue;
						}
						continue;
					}
					else if (1 == option && 2 == state) {
						continue;
					}
					else if (1 == option && 0 == state && '\"' == str[i]) {
						if (i == 0) {
							state = 1;
							continue;
						}
						else if (i > 0 && '\\' == str[i - 1]) {
							throw "ERROR for Init 2 on StringTokenizer"; // error!
						}
						else if (i > 0) {
							state = 1;
							continue;
						}
					}
					else if (1 == option && 1 == state && '\"' == str[i]) {
						if ('\\' == str[i - 1]) {
							continue;
						}
						else {
							state = 0;
							if (i == str.size() - 1) {
								if (right - left + 1 > 0) {
									_m_str.emplace_back(builder->Divide(right - left + 1), right - left + 1);
									builder->LeftShift(right - left + 1);
								}
							}
							continue;
						}
					}
					else if (1 == option && 1 == state) {
						continue;
					}

					int min = INT_MAX, min_idx = -1;

					for (int j = 0; j < sep_size; ++j) {
						const int idx = FastFind(str.c_str() + i, str.size() - i, separator[j], f[j], min);

						if (-1 < idx) {
							pass = true;
							if (-1 == min_idx) {
								min = idx;
								min_idx = j;
							}
							else if (min > idx) {
								min = idx;
								min_idx = j;
							}
						}
					}
					if (min_idx > -1) {
						i = i + min;
						_select = min_idx;
					}

					if (!pass) {
						i = str.size() - 1;
					}

					right = i;

					if (pass) {
						this->exist = true;

						if (right - left + 1 >= 1) {
							const char* temp = builder->Divide(right - left);

							if (right - left + 1 > 1) {
								_m_str.emplace_back(temp, right - left);
								builder->LeftShift(right - left + separator[_select].size());
							}
							else {
								builder->LeftShift(separator[_select].size());
							}
						}

						i = i + separator[_select].size() - 1;

						left = right + separator[_select].size();
						right = left;
					}
					else if (!pass && i == str.size() - 1) {
						if (right - left + 1 > 0) {
							_m_str.emplace_back(builder->Divide(right - left + 1), right - left + 1);
							//	std::cout << "chk " << separator[0] << " | " << str << " " << _m_str.back() << std::endl;
							builder->LeftShift(right - left + 1);
						}
						else {
							//GETCH();
						}
					}
				}
			}
			//cout << "str is " << str <<  " state  is " << state << endl;
		}
	public:
		explicit StringTokenizer() : count(0), exist(false), option(0) { }
		explicit StringTokenizer(const std::string& str, const std::string& separator, StringBuilder* builder, int option = 0)
			: count(0), exist(false), option(option)
		{
			std::vector<std::string> vec; vec.push_back(separator);
			Init(str, vec, builder);
		}
		explicit StringTokenizer(std::string&& str, const std::string& separator, StringBuilder* builder, int option = 0)
		{
			std::vector<std::string> vec; vec.push_back(separator);
			Init(std::move(str), vec, builder);
		}
		explicit StringTokenizer(const std::string& str, const std::vector<std::string>& separator, StringBuilder* builder, int option = 0)
			: count(0), exist(false), option(option)
		{
			Init(str, separator, builder);
		}
		explicit StringTokenizer(std::string&& str, const std::vector<std::string>& separator, StringBuilder* builder, int option = 0)
			: count(0), exist(false), option(option)
		{
			Init(std::move(str), separator, builder);
		}

		explicit StringTokenizer(const std::string& str, StringBuilder* builder, int option = 0)
			: count(0), exist(false), option(option)
		{
			Init(str, whitespaces, builder);
		}
		explicit StringTokenizer(std::string&& str, StringBuilder* builder, int option = 0)
			: count(0), exist(false), option(option)
		{
			Init(std::move(str), whitespaces, builder);
		}
		int countTokens()const
		{
			return _m_str.size();
		}
		std::string nextToken()
		{
			if (hasMoreTokens())
				return _m_str[count++];
			else
				throw "error in nextToken!";
		}
		bool hasMoreTokens()const
		{
			return count < _m_str.size();
		}

		bool isFindExist()const
		{
			return exist;
		}

	};
	
/*
	class StringTokenizer
	{
	private:
		std::vector<std::string> _m_str;
		int count;
		bool exist;
		static const std::vector<std::string> whitespaces;
		int option;
	private:
		void Init(std::string str, const std::vector<std::string>& separator) // assumtion : separators are sorted by length?, long -> short
		{
			if (separator.empty() || str.empty()) { return; } // if str.empty() == false then _m_str.push_back(str); // ?

			int left = 0;
			int right = 0;
			int state = 0;
			this->exist = false;

			for (int i = 0; i < str.size(); ++i) {
				right = i;
				int _select = -1;
				bool pass = false;
				
				if (1 == option && 0 == state && '\"' == str[i]) {
					if (i == 0) {
						state = 1;
						continue;
					}
					else if (i > 0 && '\\' == str[i - 1]) {
						throw "ERROR for Init on StringTokenizer"; // error!
					}
					else if (i > 0) {
						state = 1;
						continue;
					}
				}
				else if (1 == option && 1 == state && '\"' == str[i]) {
					if ('\\' == str[i - 1]) {
						continue;
					}
					else {
						state = 0;
					}
				}
				else if (1 == option && 1 == state) {
					continue;
				}


				for (int j = 0; j < separator.size(); ++j) {
					for (int k = 0; k < separator[j].size() && i + k < str.size(); ++k) {
						if (str[i + k] == separator[j][k]) {
							pass = true;
						}
						else {
							pass = false;
							break;
						}
					}
					if (pass) { _select = j; break; }
				}

				if (pass) {
					this->exist = true;

					if (right-1 - left + 1 > 0) {
						_m_str.push_back(String::substd::string(str, left, right-1));
					}
					i = i + separator[_select].size() - 1;
					left = i + 1;
					right = left;
				}
				else if (!pass && i == str.size() - 1) {
					if (right - left + 1 > 0) {
						_m_str.push_back(String::substd::string(str, left, right));
					}
				}
			}
			//cout << "str is " << str <<  " state  is " << state << endl;
		}
		
	public:
		explicit StringTokenizer() : count(0), exist(false), option(0) { }
		explicit StringTokenizer(const std::string& str, const std::string& separator, int option=0)
			: count(0), exist(false), option(option)
		{
			std::vector<std::string> vec; vec.push_back(separator);
			Init(str, vec);
		}
		explicit StringTokenizer(const std::string& str, const std::vector<std::string>& separator, int option = 0)
			: count(0), exist(false), option(option)
		{
			Init(str, separator);
		}
		explicit StringTokenizer(const std::string& str, int option = 0)
			: count(0), exist(false), option(option)
		{
			Init(str, whitespaces);
		}
		explicit StringTokenizer(std::string&& str, int option = 0)
			: count(0), exist(false), option(option)
		{
			Init(std::move(str), whitespaces);
		}
		int countTokens()const
		{
			return _m_str.size();
		}
		std::string nextToken()
		{
			if (hasMoreTokens())
				return _m_str[count++];
			else
				throw "error in nextToken!";
		}
		bool hasMoreTokens()const
		{
			return count < _m_str.size();
		}

		bool isFindExist()const
		{
			return exist;
		}

	};
	const std::vector<std::string> StringTokenizer::whitespaces = { " ", "\t", "\r", "\n" };
	*/
}
#endif // CPP_STRING_H_INCLUDED
