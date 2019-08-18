

// change to char chk. (from addspace, chk #, chk " ") - 2016.02.17 

#ifndef LOAD_DATA_UTILITY_H
#define LOAD_DATA_UTILITY_H

#include <string>
#include <vector>
#include <string>
#include <fstream>
#include <thread> // maybe error with C++/CLI?
#include <algorithm>

#include "load_data_types.h"
#include "cpp_string.h"
#include "STRINGBUILDER.H"

namespace wiz {
	namespace load_data {
		class Utility
		{
		public:
			static bool IsInteger(const std::string& str) {
				//if (str.size() > 2 && str[0] == str.back() && (str[0] == '\"' || str[0] == '\'')) {
				//	str = str.substr(1, str.size() - 2);
				//}
				int state = 0;
				for (int i = 0; i < str.size(); ++i) {
					switch (state)
					{
					case 0:
						if ('+' == str[i] || '-' == str[i]) {
							state = 0;
						}
						else if (str[i] >= '0' && str[i] <= '9')
						{
							state = 1;
						}
						else return false;
						break;
					case 1:
						if (str[i] >= '0' && str[i] <= '9') {
							state = 1;
						}
						else return false;
					}
				}
				return 1 == state; /// chk..
			}
			static bool IsNumberInJson(const std::string& str)
			{
				//if (str.size() > 2 && str[0] == str.back() && (str[0] == '\"' || str[0] == '\'')) {
				//	str = str.substr(1, str.size() - 2);
				//}
				int state = 0;
				for (int i = 0; i < str.size(); ++i) {
					switch (state)
					{
					case 0:
						if ( // '+' == str[i] || // why can`t +
							'-' == str[i]
							) {
							state = 0;
						}
						else if (str[i] >= '0' && str[i] <= '9')
						{
							state = 1;
						}
						else { return false; }
						break;
					case 1:
						if (str[i] >= '0' && str[i] <= '9') {
							state = 1;
						}
						else if (str[i] == '.') {
							state = 2;
						}
						else { return false; }
						break;
					case 2:
						if (str[i] >= '0' && str[i] <= '9') { state = 3; }
						else { return false; }
						break;
					case 3:
						if (str[i] >= '0' && str[i] <= '9') { state = 3; }
						else if ('e' == str[i] || 'E' == str[i]) {
							state = 4;
						}
						else { return false; }
						break;
					case 4:
						if (str[i] == '+' || str[i] == '-') {
							state = 5;
						}
						else {
							state = 5;
						}
						break;
					case 5:
						if (str[i] >= '0' && str[i] <= '9') {
							state = 6;
						}
						else {
							return false;
						}
						break;
					case 6:
						if (str[i] >= '0' && str[i] <= '9') {
							state = 6;
						}
						else {
							return false;
						}
					}
				}
				return 3 == state || 6 == state;
			}
			static bool IsDouble(const std::string& str) {
				//if (str.size() > 2 && str[0] == str.back() && (str[0] == '\"' || str[0] == '\'')) {
				//	str = str.substr(1, str.size() - 2);
				//}
				int state = 0;
				for (int i = 0; i < str.size(); ++i) {
					switch (state)
					{
					case 0:
						if ('+' == str[i] || '-' == str[i]) {
							state = 0;
						}
						else if (str[i] >= '0' && str[i] <= '9')
						{
							state = 1;
						}
						else { return false; }
						break;
					case 1:
						if (str[i] >= '0' && str[i] <= '9') {
							state = 1;
						}
						else if (str[i] == '.') {
							state = 2;
						}
						else { return false; }
						break;
					case 2:
						if (str[i] >= '0' && str[i] <= '9') { state = 3; }
						else { return false; }
						break;
					case 3:
						if (str[i] >= '0' && str[i] <= '9') { state = 3; }
						else if ('e' == str[i] || 'E' == str[i]) {
							state = 4;
						}
						else { return false; }
						break;
					case 4:
						if (str[i] == '+' || str[i] == '-') {
							state = 5;
						}
						else {
							state = 5;
						}
						break;
					case 5:
						if (str[i] >= '0' && str[i] <= '9') {
							state = 6;
						}
						else {
							return false;
						}
						break;
					case 6:
						if (str[i] >= '0' && str[i] <= '9') {
							state = 6;
						}
						else {
							return false;
						}
					}
				}
				return 3 == state || 6 == state;
			}
			static bool IsDate(const std::string& str) /// chk!!
			{
				//if (str.size() > 2 && str[0] == str.back() && (str[0] == '\"' || str[0] == '\'')) {
				//	str = str.substr(1, str.size() - 2);
				//}
				int state = 0;
				for (int i = 0; i < str.size(); ++i) {
					switch (state)
					{
					case 0:
						if (str[i] >= '0' && str[i] <= '9')
						{
							state = 1;
						}
						else return false;
						break;
					case 1:
						if (str[i] >= '0' && str[i] <= '9') {
							state = 1;
						}
						else if (str[i] == '.') {
							state = 2;
						}
						else return false;
						break;
					case 2:
						if (str[i] >= '0' && str[i] <= '9') { state = 3; }
						else return false;
						break;
					case 3:
						if (str[i] >= '0' && str[i] <= '9') { state = 3; }
						else if (str[i] == '.') {
							state = 4;
						}
						else return false;
						break;
					case 4:
						if (str[i] >= '0' && str[i] <= '9') { state = 5; }
						else return false;
						break;
					case 5:
						if (str[i] >= '0' && str[i] <= '9') { state = 5; }
						else return false;
						break;
					}
				}
				return 5 == state;
			}
			static bool IsDateTimeA(const std::string& str) // yyyy.MM.dd.hh
			{
				//if (str.size() > 2 && str[0] == str.back() && (str[0] == '\"' || str[0] == '\'')) {
				//	str = str.substr(1, str.size() - 2);
			//	}
				int state = 0;
				for (int i = 0; i < str.size(); ++i) {
					switch (state)
					{
					case 0:
						if (str[i] >= '0' && str[i] <= '9')
						{
							state = 1;
						}
						else return false;
						break;
					case 1:
						if (str[i] >= '0' && str[i] <= '9') {
							state = 1;
						}
						else if (str[i] == '.') {
							state = 2;
						}
						else return false;
						break;
					case 2:
						if (str[i] >= '0' && str[i] <= '9') { state = 3; }
						else return false;
						break;
					case 3:
						if (str[i] >= '0' && str[i] <= '9') { state = 3; }
						else if (str[i] == '.') {
							state = 4;
						}
						else return false;
						break;
					case 4:
						if (str[i] >= '0' && str[i] <= '9') { state = 5; }
						else return false;
						break;
					case 5:
						if (str[i] >= '0' && str[i] <= '9') { state = 5; }
						else if (str[i] == '.') { state = 6; }
						else return false;
						break;
					case 6:
						if (str[i] >= '0' && str[i] <= '9') { state = 7; }
						else return false;
						break;
					case 7:
						if (str[i] >= '0' && str[i] <= '9') { state = 7; }
						else return false;
						break;
					}
				}
				return 7 == state;
			}
			static bool IsDateTimeB(const std::string& str) // yyyy.MM.dd.hh.mm
			{
			//	if (str.size() > 2 && str[0] == str.back() && (str[0] == '\"' || str[0] == '\'')) {
			//		str = str.substr(1, str.size() - 2);
			//	}
				int state = 0;
				for (int i = 0; i < str.size(); ++i) {
					switch (state)
					{
					case 0:
						if (str[i] >= '0' && str[i] <= '9')
						{
							state = 1;
						}
						else return false;
						break;
					case 1:
						if (str[i] >= '0' && str[i] <= '9') {
							state = 1;
						}
						else if (str[i] == '.') {
							state = 2;
						}
						else return false;
						break;
					case 2:
						if (str[i] >= '0' && str[i] <= '9') { state = 3; }
						else return false;
						break;
					case 3:
						if (str[i] >= '0' && str[i] <= '9') { state = 3; }
						else if (str[i] == '.') {
							state = 4;
						}
						else return false;
						break;
					case 4:
						if (str[i] >= '0' && str[i] <= '9') { state = 5; }
						else return false;
						break;
					case 5:
						if (str[i] >= '0' && str[i] <= '9') { state = 5; }
						else if (str[i] == '.') { state = 6; }
						else return false;
						break;
					case 6:
						if (str[i] >= '0' && str[i] <= '9') { state = 7; }
						else return false;
						break;
					case 7:
						if (str[i] >= '0' && str[i] <= '9') { state = 7; }
						else if (str[i] == '.') {
							state = 8;
						}
						else return false;
						break;
					case 8:
						if (str[i] >= '0' && str[i] <= '9') { state = 9; }
						else return false;
						break;
					case 9:
						if (str[i] >= '0' && str[i] <= '9') { state = 9; }
						else return false;
						break;
					}
				}
				return 9 == state;
			}
			static bool IsMinus(const std::string& str)
			{
			//	if (str.size() > 2 && str[0] == str.back() && (str[0] == '\"' || str[0] == '\'')) {
			//		str = str.substr(1, str.size() - 2);
			//	}
				return str.empty() == false && str[0] == '-';
			}

			static std::string reverse(std::string str) { /// to std::reverse ?
				std::reverse(str.begin(), str.end());
				return str;
			}
			static std::string GetType(const std::string& str) {
				//if (str.size() > 2 && str[0] == str.back() && (str[0] == '\"' || str[0] == '\'')) {
				//	str = str.substr(1, str.size() - 2);
				//}
				int state = 0;
				for (int i = 0; i < str.size(); ++i) {
					switch (state)
					{
					case 0:
						if ('+' == str[i] || '-' == str[i]) {
							state = 0;
						}
						else if (str[i] >= '0' && str[i] <= '9')
						{
							state = 1;
						}
						else { return "STRING"; }
						break;
					case 1: 
						if (str[i] >= '0' && str[i] <= '9') {
							state = 1; // INTEGER
						}
						else if (str[i] == '.') {
							state = 2;
						}
						else { return "STRING"; }
						break;
					case 2:
						if (str[i] >= '0' && str[i] <= '9') { state = 3; } // DOUBLE
						else { return "STRING"; }
						break;
					case 3:
						if (str[i] >= '0' && str[i] <= '9') { state = 3; } // DOUBLE
						else if ('e' == str[i] || 'E' == str[i]) {
							state = 4;
						}
						else if ('.' == str[i]) {
							state = 7;
						}
						else { return "STRING"; }
						break;
					case 4:
						if (str[i] == '+' || str[i] == '-') {
							state = 5;
						}
						else {
							state = 5;
						}
						break;
					case 5:
						if (str[i] >= '0' && str[i] <= '9') {
							state = 6; // DOUBLE
						}
						else {
							return "STRING";
						}
						break;
					case 6:
						if (str[i] >= '0' && str[i] <= '9') {
							state = 6; // DOUBLE
						}
						else {
							return "STRING";
						}
						break;
					case 7:
						if (str[i] >= '0' && str[i] <= '9') {
							state = 7; // DATE
						}
						else if ('.' == str[i]) {
							state = 8;
						}
						else {
							return "STRING";
						}
						break;
					case 8:
						if (str[i] >= '0' && str[i] <= '9') {
							state = 8; // DATETIMEA
						}
						else if ('.' == str[i]) {
							state = 9;
						}
						else {
							return "STRING";
						}
						break;
					case 9:
						if (str[i] >= '0' && str[i] <= '9') {
							state = 9; // DATETIMEB
						}
						else {
							return "STRING";
						}
						break;
					}
				}

				if (1 == state) { return "INTEGER"; }
				else if (3 == state || 6 == state) { return "FLOAT"; }
				else if (9 == state) { return "DATETIMEB"; }
				else if (8 == state) { return "DATETIMEA"; }
				else if (7 == state) { return "DATE"; }
				else return "STRING";
			}
			static std::string Compare(std::string str1, std::string str2, wiz::StringBuilder* builder, 
				const std::string& _type1="", const std::string& _type2="", const int type = 0)
			{
				//if (str1.size() > 2 && str1[0] == str1.back() && (str1[0] == '\"' || str1[0] == '\''))
				//{
				//	str1 = str1.substr(1, str1.size() - 2);
				//}
				//if (str2.size() > 2 && str2[0] == str2.back() && (str2[0] == '\"' || str2[0] == '\''))
				//{
				//	str2 = str2.substr(1, str2.size() - 2);
				//}


				std::string type1; // ""
				std::string type2; // ""
				if (_type1.empty()) {
					type1 = GetType(str1);
				}
				else {
					type1 = _type1;
				}
				if (_type2.empty()) {
					type2 = GetType(str2);
				}
				else {
					type2 = _type2;
				}

				if (type1 != type2) {
					return "ERROR";
				}

				if ("STRING" == type1 || type == 1)
				{
					if (str1 < str2) {
						return "< 0";
					}
					else if (str1 == str2) {
						return "== 0";
					}
					return "> 0";
				}
				else if ("INTEGER" == type1)
				{
					if (Utility::IsMinus(str1) && !Utility::IsMinus(str2)) { return "< 0"; }
					else if (!Utility::IsMinus(str1) && Utility::IsMinus(str2)) { return "> 0"; }

					const bool minusComp = Utility::IsMinus(str1) && Utility::IsMinus(str2);

					if (false == minusComp) {
						if (str1[0] == '+') { str1 = std::string(str1.c_str() + 1); }
						if (str2[0] == '+') { str2 = std::string(str2.c_str() + 1); }

						std::string x = reverse(str1);
						std::string y = reverse(str2);



						if (x.size() < y.size()) {
							while (x.size() < y.size()) {
								x.push_back('0');
							}
						}
						else {
							while (y.size() < x.size()) {
								y.push_back('0');
							}
						}
						return Compare(reverse(x), reverse(y), builder, "INTEGER", "INTEGER", 1);
					}
					else {
						return Compare(std::string(str2.c_str() + 1), std::string(str1.c_str() + 1), builder, "INTEGER", "INTEGER");
					}
				}
				else if ("DOUBLE" == type1)
				{
					StringTokenizer tokenizer1(str1, ".", builder);
					StringTokenizer tokenizer2(str2, ".", builder);

					std::string x = tokenizer1.nextToken();
					std::string y = tokenizer2.nextToken();

					std::string z = Compare(x, y, builder, "INTEGER", "INTEGER");
					if ("== 0" == z)
					{
						x = tokenizer1.nextToken();
						y = tokenizer2.nextToken();

						if (x.size() < y.size()) {
							while (x.size() < y.size()) {
								x.push_back('0');
							}
						}
						else {
							while (y.size() < x.size()) {
								y.push_back('0');
							}
						}
						return Compare(x, y, builder, "INTEGER", "INTEGER", 1);
					}
					else
					{
						return z;
					}
				}
				else if ("DATE" == type1)
				{
					StringTokenizer tokenizer1(str1, ".", builder);
					StringTokenizer tokenizer2(str2, ".", builder);

					for (int i = 0; i < 3; ++i) {
						const std::string x = tokenizer1.nextToken();
						const std::string y = tokenizer2.nextToken();

						const std::string comp = Compare(x, y, builder);

						if (comp == "< 0") { return comp; }
						else if (comp == "> 0") { return comp; }
					}
					return "== 0";
				}
				else if ("DATETIMEA" == type1) {
					StringTokenizer tokenizer1(str1, ".", builder);
					StringTokenizer tokenizer2(str2, ".", builder);

					for (int i = 0; i < 4; ++i) {
						const std::string x = tokenizer1.nextToken();
						const std::string y = tokenizer2.nextToken();

						const std::string comp = Compare(x, y,  builder, "INTEGER", "INTEGER");

						if (comp == "< 0") { return comp; }
						else if (comp == "> 0") { return comp; }
					}
					return "== 0";
				}
				else if ("DATETIMEB" == type2) {
					StringTokenizer tokenizer1(str1, ".", builder);
					StringTokenizer tokenizer2(str2, ".", builder);

					for (int i = 0; i < 5; ++i) {
						const std::string x = tokenizer1.nextToken();
						const std::string y = tokenizer2.nextToken();

						const std::string comp = Compare(x, y, builder, "INTEGER", "INTEGER");

						if (comp == "< 0") { return comp; }
						else if (comp == "> 0") { return comp; }
					}
					return "== 0";
				}
				return "ERROR";
			}

			static std::string BoolOperation(const std::string& op, const std::string& x, const std::string& y)
			{
				if (x == "ERROR" || y == "ERROR") { return "ERROR"; }
				if ("NOT" == op) { return x == "TRUE" ? "FALSE" : "TRUE"; }
				else if ("AND" == op) {
					if (x == "TRUE" && y == "TRUE") { return "TRUE"; }
					else {
						return "FALSE";
					}
				}
				else if ("OR" == op) {
					if (x == "TRUE" || y == "TRUE") { return "TRUE"; }
					else {
						return "FALSE";
					}
				}
				else {
					return "ERROR";
				}
			}

		public:
			class DoThread // need to rename!
			{
			private:
				StringBuilder* strVec;
			public:
				ARRAY_QUEUE<Token>* aq;
				const wiz::LoadDataOption* option;
				//int strVecStart;
				//int strVecEnd;
			public:
				DoThread(StringBuilder* strVec, ARRAY_QUEUE<Token>* aq, const wiz::LoadDataOption* option) //, list<std::string>* aq)//, int strVecStart, int strVecEnd)
					: strVec(strVec), aq(aq), option(option) // , strVecStart(strVecStart), strVecEnd(strVecEnd)
				{
					//
				}
			private:
				// slow?
				int checkDelimiter(const StringBuilder& statement, const int start, const std::vector<std::string>& delimiter)
				{
					int sum = 0;
					const int _size = delimiter.size();

					for (int delim_num = 0; delim_num < _size; ++delim_num) {
						sum = 0;

						// size check
						if (start + delimiter[delim_num].size() - 1 > statement.Size() - 1) {
							continue;
						}

						const int last = start + delimiter[delim_num].size() - 1;
						for (int i = start; last >= start && i <= last; ++i) {
							if (statement[i] == delimiter[delim_num][i - start]) {

							}
							else {
								sum--;
								break;
							}
						}
						sum++;

						if (sum > 0) {
							return delim_num;
						}
					}

					return -1;
				}
			public:
				void operator() () {
					//std::string* strVecTemp = strVec; // enterkey 기준으로 나뉘어져있다고 가정한다.
					//for (int x = 0; x <= 0; ++x)
					{
						//StringTokenizer tokenizer(std::move( (*strVecTemp)[x] ) );
						//while (tokenizer.hasMoreTokens()) {
						//	aq.push(tokenizer.nextToken());
						//}
						StringBuilder& statement = *strVec;
						int token_first = 0, token_last = 0; // idx of token in statement.
						int state = 0;
						bool isMultipleLineComment = false;
						std::string token;


						for (int i = 0; i < statement.Size(); ++i) {
							long long idx;

							if (isMultipleLineComment && -1 != (idx = checkDelimiter(statement, i, option->MuitipleLineCommentEnd)))
							{
								isMultipleLineComment = false;

								for (int j = 0; j < option->MuitipleLineCommentEnd[idx].size(); ++j)
								{
									token.push_back(statement[i + j]);
								}

								aq->emplace_back(std::move(token), true);

								i = i + option->MuitipleLineCommentEnd[idx].size() - 1;

								statement.Divide(i);
								statement.LeftShift(i + 1);

								token.clear();

								token_first = 0;
								token_last = 0;

								i = -1;
							}
							else if (isMultipleLineComment) {
								token_last = i;

								token.push_back(statement[i]);
							}
							else if (0 != state && ('\n' == statement[i])) {
								state = 0;
								token_last = i - 1;
							}
							else if (0 == state && '\'' == statement[i]) {
								//token_last = i - 1;
								//if (token_last >= 0 && token_last - token_first + 1 > 0) {
								//	aq->emplace_back(statement.substr(token_first, token_last - token_first + 1));
								//}
								state = 2;
								//token_first = i; 
								token_last = i;

								token.push_back(statement[i]);
							}
							else if (2 == state && '\\' == statement[i - 1] && '\'' == statement[i]) {
								token_last = i;
								token.push_back(statement[i]);
							}
							else if (2 == state && '\'' == statement[i]) {
								state = 0; token_last = i;
								token.push_back(statement[i]);
							}
							else if (0 == state && '\"' == statement[i]) {
								//token_last = i - 1;
								//if (token_last >= 0 && token_last - token_first + 1 > 0) {
								//	aq->emplace_back(statement.substr(token_first, token_last - token_first + 1));
								//}
								state = 1;
								//token_first = i; 
								token_last = i;
								token.push_back(statement[i]);
							}
							else if (1 == state && '\\' == statement[i - 1] && '\"' == statement[i]) {
								token_last = i;
								token.push_back(statement[i]);
							}
							else if (1 == state && '\"' == statement[i]) {
								state = 0; token_last = i;
								token.push_back(statement[i]);
							}
							else if (0 == state && -1 != (idx = Equal(option->Removal, statement[i])))
							{
								token_last = i - 1;

								if (token_last >= 0 && token_last - token_first + 1 > 0) {
									statement.Divide(i);

									aq->emplace_back(std::move(token), false);
									token.clear();

									statement.LeftShift(i + 1);

									token_first = 0;
									token_last = 0;

									i = -1;
								}
								else {
									statement.LeftShift(1);
									token.clear();
									token_first = 0;
									token_last = 0;
									i = -1;
								}
								continue;
							}
							else if (0 == state && -1 != (idx = checkDelimiter(statement, i, option->Assignment))) {
								token_last = i - 1;
								if (token_last >= 0 && token_last - token_first + 1 > 0) {
									
									statement.Divide(i);
									
									aq->emplace_back(std::move(token), false);

									token.clear();

									statement.LeftShift(i + option->Assignment[idx].size());

									aq->emplace_back(std::string("") + option->Assignment[idx], false);
									token_first = 0;
									token_last = 0;

									i = -1;
								}
								else {
									aq->emplace_back(std::string("") + option->Assignment[idx], false);
									statement.LeftShift(option->Assignment[idx].size());

									token.clear();
									token_first = 0;
									token_last = 0;
									i = -1;
								}
							}
							else if (0 == state && isWhitespace(statement[i])) { // isspace ' ' \t \r \n , etc... ?
								token_last = i - 1;
								if (token_last >= 0 && token_last - token_first + 1 > 0) {
									statement.Divide(i);

									aq->emplace_back(std::move(token), false);

									statement.LeftShift(i + 1);
									token.clear();
									token_first = 0;
									token_last = 0;

									i = -1;
								}
								else
								{
									statement.LeftShift(1);
									token.clear();
									token_first = 0;
									token_last = 0;
									i = -1;
								}
							}
							else if (0 == state && -1 != (idx = Equal(option->Left, statement[i]))) {
								token_last = i - 1;
								if (token_last >= 0 && token_last - token_first + 1 > 0) {
									statement.Divide(i);

									aq->emplace_back(std::move(token), false);
									statement.LeftShift(i + 1);

									aq->emplace_back(std::string("") + option->Left[idx], false);
									token.clear();
									token_first = 0;
									token_last = 0;

									i = -1;
								}
								else {
									aq->emplace_back(std::string("") + option->Left[idx], false);
									statement.LeftShift(1);
									token.clear();
									token_first = 0;
									token_last = 0;
									i = -1;
								}
							}
							else if (0 == state && -1 != (idx = Equal(option->Right, statement[i]))) {
								token_last = i - 1;
								if (token_last >= 0 && token_last - token_first + 1 > 0) {
									statement.Divide(i);


									aq->emplace_back(std::move(token), false);
									statement.LeftShift(i + 1);

									aq->emplace_back(std::string("") + option->Right[idx], false);

									token.clear();
									token_first = 0;
									token_last = 0;

									i = -1;
								}
								else {
									aq->emplace_back(std::string("") + option->Right[idx], false);

									statement.LeftShift(1);

									token.clear();
									token_first = 0;
									token_last = 0;
									i = -1;
								}
							}
							else if (0 == state && option->MuitipleLineCommentStart.empty() == false 
								&& -1 != (idx = checkDelimiter(statement, i, option->MuitipleLineCommentStart))) { // different from load_data_from_file
								token_last = i - 1;
								if (token_last >= 0 && token_last - token_first + 1 > 0) {
									statement.Divide(i);
									aq->emplace_back(std::move(token), false);
								
									statement.LeftShift(i + option->MuitipleLineCommentStart[idx].size());
									i = -1;

									token_first = 0;
									token_last = 0;
								}
								else {
									statement.LeftShift(i + option->MuitipleLineCommentStart[idx].size());
									i = -1;
								}

								token = option->MuitipleLineCommentStart[idx];

								isMultipleLineComment = true;
							}
							else if (0 == state && option->LineComment.empty() == false &&
									-1 != checkDelimiter(statement, i, option->LineComment)) { // different from load_data_from_file
								token_last = i - 1;
								if (token_last >= 0 && token_last - token_first + 1 > 0) {
									char temp = statement[i];

									statement.Divide(i);
									aq->emplace_back(std::move(token), false);
									token.clear();
									
									statement[i] = temp;
									statement.LeftShift(i);
									i = 0;
								}
								int j = 0;
								for (j = i; j < statement.Size(); ++j) {
									if (statement[j] == '\n') // cf) '\r' ? '\0'?
									{
										break;
									}
								}
								--j; // "before enter key" or "before end"

								if (j - i + 1 > 0) {
									statement.Divide(j + 1);

									aq->emplace_back(std::string(statement.Str(), j - i + 1), true);
									statement.LeftShift(j + 2);

									token_first = 0;
									token_last = 0;

									i = -1;
								}
								else {
									statement.LeftShift(j + 2);
									token_first = 0;
									token_last = 0;

									i = -1;
								}
							}
							else {
								token.push_back(statement[i]);
							}
						}

						if (token.empty() == false)
						{
							aq->emplace_back(std::move(token), false);
						}
					}
				}
				//
				//DoThread(DoThread& other, tbb::split) : strVec(other.strVec) // , aq(other.aq)
				//{
					//
				//}
				//void join(DoThread& other) 
				//{
				//	aqpush(std::move(other.aq));
				//}
			};
			class DoThread2 // need to rename!
			{
			private:
				StringBuilder * strVec;
			public:
				ARRAY_QUEUE<Token>* aq;
				wiz::LoadDataOption option;
				//int strVecStart;
				//int strVecEnd;
			public:
				DoThread2(StringBuilder* strVec, ARRAY_QUEUE<Token>* aq, const wiz::LoadDataOption& option) //, list<std::string>* aq)//, int strVecStart, int strVecEnd)
					: strVec(strVec), aq(aq), option(option) // , strVecStart(strVecStart), strVecEnd(strVecEnd)
				{
					//
				}
			private:
				int checkDelimiter(const StringBuilder& statement, const int start, const std::vector<std::string>& delimiter)
				{

					int sum = 0;
					for (int delim_num = 0; delim_num < delimiter.size(); ++delim_num) {
						sum = 0;

						// size check
						if (start + delimiter[delim_num].size() - 1 > statement.Size() - 1) {
							continue;
						}

						for (int i = start; i <= start + delimiter[delim_num].size() - 1; ++i) {
							if (statement[i] == delimiter[delim_num][i - start]) {

							}
							else {
								sum--;
								break;
							}
						}
						sum++;

						if (sum > 0) {
							return delim_num;
						}
					}

					return -1;
				}
			public:
				void operator() () {
					//std::string* strVecTemp = strVec; // enterkey 기준으로 나뉘어져있다고 가정한다.
					//for (int x = 0; x <= 0; ++x)
					{
						//StringTokenizer tokenizer(std::move( (*strVecTemp)[x] ) );
						//while (tokenizer.hasMoreTokens()) {
						//	aq.push(tokenizer.nextToken());
						//}
						StringBuilder& statement = *strVec;
						int token_first = 0, token_last = 0; // idx of token in statement.
						int state = 0;
						bool isMultipleLineComment = false;
						std::string token;
						token.reserve(1024);

						for (int i = 0; i < statement.Size(); ++i) {
							long long idx;

							// has bug?
							/*
							if (i == statement.Size() - 1) {
								if (token_last >= 0 && token_last - token_first + 1 > 0) {
									statement.Divide(i);

									aq->emplace_back(std::move(token), false);
									token.clear();

									statement.LeftShift(i + 1);

									token_first = 0;
									token_last = 0;

									i = -1;
								}
								else {
									statement.LeftShift(1);

									token_first = 0;
									token_last = 0;

									i = -1;
								}
								aq->emplace_back("", true); // chk!
							}
							else 
								*/
							if (isMultipleLineComment && -1 != (idx = checkDelimiter(statement, i, option.MuitipleLineCommentEnd)))
							{
								isMultipleLineComment = false;

								for (int j = 0; j < option.MuitipleLineCommentEnd[idx].size(); ++j)
								{
									token.push_back(statement[i + j]);
								}

								//aq->emplace_back(Token(token, true));

								i = i + option.MuitipleLineCommentEnd[idx].size() - 1;

								statement.Divide(i);
								statement.LeftShift(i + 1);

								token.clear();
								token_first = 0;
								token_last = 0;

								i = -1;
							}
							else if (isMultipleLineComment) {
								token_last = i;

								token.push_back(statement[i]);
							}
							else if (0 == state && '\'' == statement[i]) {
								//token_last = i - 1;
								//if (token_last >= 0 && token_last - token_first + 1 > 0) {
								//	aq->emplace_back(statement.substr(token_first, token_last - token_first + 1));
								//}
								state = 2;
								//token_first = i; 
								token_last = i;

								token.push_back(statement[i]);
							}
							else if (2 == state && '\\' == statement[i - 1] && '\'' == statement[i]) {
								token_last = i;
								token.push_back(statement[i]);
							}
							else if (2 == state && '\'' == statement[i]) {
								state = 0; token_last = i;
								token.push_back(statement[i]);
							}
							else if (0 == state && '\"' == statement[i]) {
								//token_last = i - 1;
								//if (token_last >= 0 && token_last - token_first + 1 > 0) {
								//	aq->emplace_back(statement.substr(token_first, token_last - token_first + 1));
								//}
								state = 1;
								//token_first = i; 
								token_last = i;
								token.push_back(statement[i]);
							}
							else if (1 == state && '\\' == statement[i - 1] && '\"' == statement[i]) {
								token_last = i;
								token.push_back(statement[i]);
							}
							else if (1 == state && '\"' == statement[i]) {
								state = 0; token_last = i;
								token.push_back(statement[i]);
							}
							else if (0 == state && -1 != (idx = Equal(option.Removal, statement[i])))
							{
								token_last = i - 1;

								if (token_last >= 0 && token_last - token_first + 1 > 0) {
									statement.Divide(i);

									aq->emplace_back(std::move(token), false);
									token.clear();

									statement.LeftShift(i + 1);

									token_first = 0;
									token_last = 0;

									i = -1;
								}
								else {
									statement.LeftShift(1);
									token.clear();
									token_first = 0;
									token_last = 0;
									i = -1;
								}
								continue;
							}
							else if (0 == state && -1 != (idx = checkDelimiter(statement, i, option.Assignment))) {
								token_last = i - 1;
							//	wiz::Out << "chk " << ENTER;
								if (token_last >= 0 && token_last - token_first + 1 > 0) {
									statement.Divide(i);
				
									aq->emplace_back(std::move(token), false);
									
									token.clear();
									
									statement.LeftShift(i + option.Assignment[idx].size());

									aq->emplace_back(std::string("") + option.Assignment[idx], false);
									token_first = 0;
									token_last = 0;

									i = -1;
								}
								else {
									aq->emplace_back(std::string("") + option.Assignment[idx], false);
									statement.LeftShift(option.Assignment[idx].size());

									token.clear();
									token_first = 0;
									token_last = 0;
									i = -1;
								}
							}
							else if (0 == state && isWhitespace(statement[i])) { // isspace ' ' \t \r \n , etc... ?
								token_last = i - 1;
								if (token_last >= 0 && token_last - token_first + 1 > 0) {
									statement.Divide(i);

									aq->emplace_back(std::move(token), false);

									statement.LeftShift(i + 1);
									token.clear();
									token_first = 0;
									token_last = 0;

									i = -1;
								}
								else
								{
									statement.LeftShift(1);
									token.clear();
									token_first = 0;
									token_last = 0;
									i = -1;
								}
							}
							else if (0 == state && -1 != (idx = Equal(option.Left, statement[i]))) {
								token_last = i - 1;
								if (token_last >= 0 && token_last - token_first + 1 > 0) {
									statement.Divide(i);

									aq->emplace_back(std::move(token), false);
									statement.LeftShift(i + 1);

									aq->emplace_back(std::string("") + option.Left[idx], false);
									token.clear();
									token_first = 0;
									token_last = 0;

									i = -1;
								}
								else {
									aq->emplace_back(std::string("") + option.Left[idx], false);
									statement.LeftShift(1);
									token.clear();
									token_first = 0;
									token_last = 0;
									i = -1;
								}
							}
							else if (0 == state && -1 != (idx = Equal(option.Right, statement[i]))) {
								token_last = i - 1;
								if (token_last >= 0 && token_last - token_first + 1 > 0) {
									statement.Divide(i);


									aq->emplace_back(std::move(token), false);
									statement.LeftShift(i + 1);

									aq->emplace_back(std::string("") + option.Right[idx], false);

									token.clear();
									token_first = 0;
									token_last = 0;

									i = -1;
								}
								else {
									aq->emplace_back(std::string("") + option.Right[idx], false);

									statement.LeftShift(1);

									token.clear();
									token_first = 0;
									token_last = 0;
									i = -1;
								}
							}
							else if (0 == state && option.MuitipleLineCommentStart.empty() == false
								&& -1 != (idx = checkDelimiter(statement, i, option.MuitipleLineCommentStart))) { // different from load_data_from_file
								token_last = i - 1;
								if (token_last >= 0 && token_last - token_first + 1 > 0) {
									statement.Divide(i);
									aq->emplace_back(std::move(token), false);

									statement.LeftShift(i + option.MuitipleLineCommentStart[idx].size());
									i = -1;

									token_first = 0;
									token_last = 0;
								}
								else {
									statement.LeftShift(i + option.MuitipleLineCommentStart[idx].size());
									i = -1;
								}

								token = option.MuitipleLineCommentStart[idx];

								isMultipleLineComment = true;
							}
							else if (0 == state && option.LineComment.empty() == false &&
								-1 != checkDelimiter(statement, i, option.LineComment)) { // different from load_data_from_file
								token_last = i - 1;
								if (token_last >= 0 && token_last - token_first + 1 > 0) {
									char temp = statement[i];

									statement.Divide(i);
									aq->emplace_back(std::move(token), false);
									token.clear();

									statement[i] = temp;
									statement.LeftShift(i);
									i = 0;
								}
								int j = 0;
								for (j = i; j < statement.Size(); ++j) {
									if (statement[j] == '\n') // cf) '\r' ? '\0'?
									{
										break;
									}
								}
								--j; // "before enter key" or "before end"

								if (j - i + 1 > 0) {
									statement.Divide(j + 1);

									//aq->emplace_back(Token(std::string(statement.Str(), j - i + 1), true));
									statement.LeftShift(j + 2);

									token_first = 0;
									token_last = 0;

									i = -1;
								}
								else {
									statement.LeftShift(j + 2);
									token_first = 0;
									token_last = 0;

									i = -1;
								}
							}
							else {
								token.push_back(statement[i]);
							}
						}

						if (token.empty() == false)
						{
							aq->emplace_back(std::move(token), false);
						}
					}
				}
				//
				//DoThread(DoThread& other, tbb::split) : strVec(other.strVec) // , aq(other.aq)
				//{
				//
				//}
				//void join(DoThread& other) 
				//{
				//	aqpush(std::move(other.aq));
				//}
			};
			

			class DoThread3 // need to rename!
			{
			private:
				char* start;
				char* last;
			public:
				VECTOR<Token2>* aq;
				const wiz::LoadDataOption* option;
				//int strVecStart;
				//int strVecEnd;
			public:
				DoThread3(char* start, char* last, VECTOR<Token2>* aq, const wiz::LoadDataOption* option) //, list<std::string>* aq)//, int strVecStart, int strVecEnd)
					: start(start), last(last), aq(aq), option(option) // , strVecStart(strVecStart), strVecEnd(strVecEnd)
				{
					//
				}
			public:
				void operator() () {
					{
						char* token_first = start;
						char* token_last = start; // idx of token in statement.
						int state = 0;
						std::string token;
						long long idx;

						for (char* x = start; x < last; ++x) {
							int offset = 0;
							int idx;
							
							if (0 != state && ('\n' == *x)) {
								state = 0;
								token_last = x - 1;
							}
							else if (0 == state && '\'' == *x) {
								//token_last = x - 1;
								//if (token_last >= 0 && token_last - token_first + 1 > 0) {
								//	aq->emplace_back(statement.substr(token_first, token_last - token_first + 1));
								//}
								state = 2;
								//token_first = i; 
								token_last = x;

								token.push_back(*x);
							}
							else if (2 == state && '\\' == *(x - 1) && '\'' == *x) {
								token_last = x;
								token.push_back(*x);
							}
							else if (2 == state && '\'' == *x) {
								state = 0; token_last = x;
								token.push_back(*x);
							}
							else if (0 == state && '\"' == *x) {
								//token_last = x - 1;
								//if (token_last >= 0 && token_last - token_first + 1 > 0) {
								//	aq->emplace_back(statement.substr(token_first, token_last - token_first + 1));
								//}

								state = 1;
								//token_first = i; 
								token_last = x;
								token.push_back(*x);
							}
							else if (1 == state && '\\' == *(x - 1) && '\"' == *x) {
								token_last = x;
								token.push_back(*x);
							}
							else if (1 == state && '\"' == *x) {
								state = 0; token_last = x;
								token.push_back(*x);
							}
							else if (0 == state && -1 != (idx = Equal(option->Removal, *x)))
							{
								token_last = x - 1;

								if (token_last >= 0 && token_last - token_first + 1 > 0) {
									aq->emplace_back(token_first, token_last - token_first + 1, false);
									token.clear();

									token_first = x + 1;
								}
								else {
									token.clear();

									token_first = token_first + 1;
								}
								continue;
							}
							else if (0 == state && -1 != (idx = Equal(option->Delimiter, *x)))
							{
								token_last = x - 1;

								if (token_last >= 0 && token_last - token_first + 1 > 0) {
									aq->emplace_back(token_first, token_last - token_first + 1, false);
									token.clear();

									aq->emplace_back(x, 1, false);
									token_first = x + 1;
								}
								else {
									token.clear();

									aq->emplace_back(x, 1, false);
									token_first = token_first + 1;
								}
								continue;
							}
							else if (0 == state && -1 != (idx = checkDelimiter(x, last, option->Assignment))) {
								token_last = x - 1;

								if (token_last >= 0 && token_last - token_first + 1 > 0) {
									aq->emplace_back(token_first, token_last - token_first + 1, false);

									token.clear();

									token_first = x + option->Assignment[idx].size();

									aq->emplace_back(x, option->Assignment[idx].size(), false);
								}
								else {
									aq->emplace_back(x, option->Assignment[idx].size(), false);
									token_first = token_first + option->Assignment[idx].size();

									token.clear();
								}
							}
							else if (0 == state && isWhitespace(*x)) { // isspace ' ' \t \r \n , etc... ?
								token_last = x - 1;
								if (token_last >= 0 && token_last - token_first + 1 > 0) {

									aq->emplace_back(token_first, token_last - token_first + 1, false);

									token_first = x + 1;
									token.clear();
								}
								else
								{
									token_first = token_first + 1;
									token.clear();
								}
							}
							else if (0 == state && -1 != (idx = Equal(option->Left, *x))) {
								token_last = x - 1;
								if (token_last >= 0 && token_last - token_first + 1 > 0) {

									aq->emplace_back(token_first, token_last - token_first + 1, false);
									token_first = x + 1;

									aq->emplace_back(x, 1, false);
									token.clear();
								}
								else {
									aq->emplace_back(x, 1, false);
									token_first = token_first + 1;
									token.clear();
								}
							}
							else if (0 == state && -1 != (idx = Equal(option->Right, *x))) {
								token_last = x - 1;
								if (token_last >= 0 && token_last - token_first + 1 > 0) {

									aq->emplace_back(token_first, token_last - token_first + 1, false);
									token_first = x + 1;

									aq->emplace_back(x, 1, false);

									token.clear();
								}
								else {
									aq->emplace_back(x, 1, false);

									token_first = token_first + 1;

									token.clear();
								}
							}
							else if (0 == state && option->LineComment.empty() == false &&
								-1 != checkDelimiter(x, last, option->LineComment)) { // different from load_data_from_file
								token_last = x - 1;
								if (token_last >= 0 && token_last - token_first + 1 > 0) {
									aq->emplace_back(token_first, token_last - token_first + 1, false);
									token.clear();
									x = token_last + 1;
									token_first = token_last + 1;
									token_last = token_last + 1;
								}
								for (; x < last; ++x) {
									token_last++;
									if (*x == '\n') //|| *x == '\0') // cf) '\r' ? '\0'?
									{
										break;
									}
								}
								//aq->emplace_back(token_first, token_last - token_first + 1, true); // cancel?
								token_first = x + 1;
								offset = 1;
							}
							else {
								//
							}

							token_last = x + offset;
						}

						if (token_first < last)
						{
							aq->emplace_back(token_first, last-1 - token_first + 1, false);
						}
					}
				}
			};
	public:
			static bool Reserve3(std::ifstream& inFile, StringBuilder& strVecTemp, const int min_num = 1)
			{
				int count = 0;
				std::string temp;
				int brace = 0;
				int i = 0;

				for (int i = 0; i < min_num && (getline(inFile, temp)); ++i) {
					if (temp.empty()) { continue; }
					strVecTemp.Append(temp.c_str(), temp.size());
					strVecTemp.AppendChar('\n');
					for (int j = 0; j < temp.size(); ++j) {
						if (temp[j] == '{') {
							brace++;
						}
						else if (temp[j] == '}') {
							brace--;
						}
					}
					count++;
				}

				while (brace != 0 && getline(inFile, temp)) {
					if (temp.empty()) { continue; }
					strVecTemp.Append(temp.c_str(), temp.size());
					for (int j = 0; j < temp.size(); ++j) {
						if (temp[j] == '{') {
							brace++;
						}
						else if (temp[j] == '}') {
							brace--;
						}
					}
					count++;
				}

				return count > 0 && 0 == brace;
			}

			static std::pair<bool, int> Reserve2(std::ifstream& inFile, ARRAY_QUEUE<Token>& aq, const int num, const wiz::LoadDataOption& option)
			{
				//int a = clock();

				int count = 0;
				std::string temp;
				wiz::StringBuilder builder(128 * num); //

				builder.Clear();

				for (int i = 0; i < num &&
					(std::getline(inFile, temp)); ++i) {
					if (temp.empty()) { continue; }
					builder.Append(temp.c_str(), temp.size());
					builder.AppendChar('\n');
					count++;
				}
				
				DoThread doThread(&builder, &aq, &option);

				doThread(); // (0, count - 1);

				//tbb::parallel_reduce(tbb::blocked_range<size_t>(0, count), doThread);
				//aq.push(std::move(doThread.aq));
				//int b = clock();

				//wiz::Out << b - a << "ms" << ENTER;
				return{ count > 0, count };
			}

			// maybe remove?
			static std::pair<bool, int> Reserve2_2(std::ifstream& inFile, std::vector<ARRAY_QUEUE<Token>>& aq, const int num, const wiz::LoadDataOption& option, const int coreNum, const int offset)
			{
				// clear aq? empty empty data  ( remove empty )
				for (int i = 0; i < aq.size(); ++i) {
					for (int j = 0; j < aq[i].size(); ++j) {
						if (aq[i][j].str.empty()) {
							aq[i].pop_front();
							--j;
						}
						else {
							break;
						}
					}
				}

				int pass = 0;
				int count = 0;
				int before_count = 0;
				int count2 = 0;
				std::string temp;
				std::vector<wiz::StringBuilder> builder(coreNum, wiz::StringBuilder(32 * num / coreNum)); // *num;
				
				for (int i = 0; i < coreNum; ++i) {
					builder[i].Clear();
				}

				for (int i = 0; i < num &&
					(std::getline(inFile, temp)); ++i) {
					if (temp.empty()) { continue; }

					builder[(count) % coreNum].Append(temp.c_str(), temp.size());
					builder[(count) % coreNum].AppendChar('\n');


					if ((count2 + 1) % 50000 == 0)
					{
						builder[(count) % coreNum].AppendChar('\0'); // chk!
						before_count = count;
						count++;

						pass = 1;
					}
					else {
						pass = 0;
					}
					
					count2++;
				}

				if (count2) {
					if (0 == pass) {
						builder[(before_count) % coreNum].AppendChar('\0');
					}

					std::vector<std::thread*> threads(coreNum);

					// do parallel!
					for (int i = 0; i < coreNum; ++i) {
						DoThread2 doThread2(&builder[i], &aq[(i + offset) % coreNum], option);
						threads[(i + offset) % coreNum] = new std::thread(doThread2);
					}

					// wait
					for (int i = 0; i < coreNum; ++i) {
						threads[i]->join();
					}
					for (int i = 0; i < coreNum; ++i) {
						delete threads[i];
					}
				}

				return{ count2 > 0, count2 };
			}
			// no enter strings? " abc \n def " and problem - one line!?
	
			class UT_IT_NUM
			{
			public:
				int utNum = 0; // int -> unsigned char?
				int itNum = 0;
				int eqNum = 0;
				int valid = 0;
			public:
				UT_IT_NUM() {

				}
				UT_IT_NUM(int utNum, int itNum, int eqNum)
					: utNum(utNum), itNum(itNum), eqNum(eqNum)
				{

				}

				UT_IT_NUM& operator=(const UT_IT_NUM& other) {
					this->eqNum = other.eqNum;
					this->itNum = other.itNum;
					this->utNum = other.utNum;
					this->valid = other.valid;
					return *this;
				}
			};

			inline static int timeA[8], timeB[8];
			inline static int countTotal[8];

			class DoThread4 // need to rename!
			{
			private:
				char* start;
				char* last;
			public:
				const wiz::LoadDataOption2* option; // wiz::LoadDatOption2 ?
				long long* llptr;
				long long* llptr2;
				long long* llptr2_len;
				UT_IT_NUM* llptr3;
				long long num;
				int no;
			public:
				DoThread4(char* start, char* last, const wiz::LoadDataOption2* option,
					long long* llptr, long long* llptr2, long long num, long long* llptr2_len, UT_IT_NUM* llptr3,
				 int no) //, list<std::string>* aq)//, int strVecStart, int strVecEnd)
					: start(start), last(last), option(option), llptr3(llptr3), no(no) // , strVecStart(strVecStart), strVecEnd(strVecEnd)
				{
					this->llptr = llptr;
					this->llptr2 = llptr2;
					this->num = num;
					this->llptr2_len = llptr2_len;
				}
				~DoThread4() {
					//
				}
			private:

				long long chk2(bool make) {
					{
						std::vector<UT_IT_NUM*> _vecStack;
						_vecStack.reserve(1024);
						countTotal[no] = 0;

						long long llptr2_count = 0;
						long long start_idx = 0;
						long long last_idx = 0;
						long long count = 0; 
						long long llptr3_count = 0;
						char* token_first = start;
						char* token_last = start;
						int state = 0;

						long long now_idx = 0;
						for (long long i = 0; start + i < last; ++i, ++now_idx) {
							//llptr[i] = 0;
							char* x = start + i;
							long long offset = 0;
							int idx;

							if (0 == state && '\'' == *x) {
								//token_last = x - 1;
								//if (token_last >= 0 && token_last - token_first + 1 > 0) {
								//	aq->emplace_back(statement.substr(token_first, token_last - token_first + 1));
								//}
								state = 2;
								//token_first = i; 
								token_last = x;
								last_idx = now_idx;
							}
							else if (2 == state && '\\' == *(x - 1) && '\'' == *x) {
								token_last = x;
								last_idx = now_idx;
							}
							else if (2 == state && '\'' == *x) {
								state = 0; token_last = x;
								last_idx = now_idx;
							}
							else if (2 == state && '\n' == *x) {
								throw "Error : \n is included in \' ~ \' ";
							}
							else if (0 == state && '\"' == *x) {
								//token_last = x - 1;
								//if (token_last >= 0 && token_last - token_first + 1 > 0) {
								//	aq->emplace_back(statement.substr(token_first, token_last - token_first + 1));
								//}

								state = 1;
								//token_first = i; 
								token_last = x;
								last_idx = now_idx;
							}
							else if (1 == state && '\\' == *(x - 1) && '\"' == *x) {
								token_last = x;
								last_idx = now_idx;
							}
							else if (1 == state && '\n' == *x) {
								throw "error : \n is included.. in \" ~ \" ";
							}
							else if (1 == state && '\"' == *x) {
								state = 0; token_last = x;
								last_idx = now_idx;
							}
							else if (0 == state && -1 != (idx = Equal2(option->Removal, *x)))
							{
								token_last = x - 1;
								last_idx = now_idx - 1;

								if (token_last >= 0 && token_last - token_first + 1 > 0) {
									if (make) {
										//aq->emplace_back(token_first, token_last - token_first + 1, false);

										//llptr[start_idx] = token_last - token_first + 1;
										llptr2[llptr2_count] = ((start_idx + num) << 32) + ((token_last - token_first + 1) << 2) + 0;
										llptr2_count++;

										{
											if (token_last - token_first + 1 == 1) {
												if (start[start_idx] == option->Left) {
													llptr2[llptr2_count - 1] += 1;
												}
												if (start[start_idx] == option->Right) {
													llptr2[llptr2_count - 1] += 2;
												}
												if (start[start_idx] == option->Assignment) {
													llptr2[llptr2_count - 1] += 3;
												}
											}
										}

										if (!_vecStack.empty()) {
											_vecStack.back()->itNum++;
										}
									}
									else {

										count++;
									}


									token_first = x + 1;
									start_idx = now_idx + 1;
								}
								else {
									token_first = token_first + 1;
									start_idx = start_idx + 1;
								}
								continue;
							}
							else if (0 == state && -1 != (idx = Equal2(option->Assignment, *x))) {
								countTotal[no]++;
								token_last = x - 1;
								last_idx = now_idx - 1;

								if (token_last >= 0 && token_last - token_first + 1 > 0) {
									if (make) {
										//aq->emplace_back(token_first, token_last - token_first + 1, false);{
										//llptr[start_idx] = token_last - token_first + 1;
										llptr2[llptr2_count] = ((start_idx + num) << 32) + ((token_last - token_first + 1) << 2) + 0;
										llptr2_count++;

										{
											if (token_last - token_first + 1 == 1) {
												if (start[start_idx] == option->Left) {
													llptr2[llptr2_count - 1] += 1;
												}
												if (start[start_idx] == option->Right) {
													llptr2[llptr2_count - 1] += 2;
												}
												if (start[start_idx] == option->Assignment) {
													llptr2[llptr2_count - 1] += 3;
												}
											}
										}
										if (!_vecStack.empty()) {
											_vecStack.back()->itNum++;
										}
									}
									else {
										count++;
									}

									token_first = x + 1;
									start_idx = now_idx + 1;
									if (make) {
										//aq->emplace_back(x, 1, false);
										//llptr[i] = 1; 
										llptr2[llptr2_count] = ((i + num) << 32) + ((1) << 2) + 3;
										llptr2_count++;

										if (!_vecStack.empty()) {
											_vecStack.back()->eqNum++;
										}
									}
									else {
										count++;
									}
								}
								else {
									if (make) {
										//aq->emplace_back(x, 1, false);
										//llptr[i] = 1;
										llptr2[llptr2_count] = ((i + num) << 32) + ((1) << 2) + 3;
										llptr2_count++;

									}
									else {

										count++;
									}
									token_first = token_first + 1;
									start_idx = start_idx + 1;

								}
							}
							else if (0 == state && isWhitespace(*x)) { // isspace ' ' \t \r \n , etc... ?
								token_last = x - 1;
								last_idx = now_idx - 1;
								if (token_last >= 0 && token_last - token_first + 1 > 0) {
									if (make) {
										//aq->emplace_back(token_first, token_last - token_first + 1, false);

										//llptr[start_idx] = token_last - token_first + 1;
										llptr2[llptr2_count] = ((start_idx + num) << 32) + ((token_last - token_first + 1) << 2) + 0;
										llptr2_count++;

										{
											if (token_last - token_first + 1 == 1) {
												if (start[start_idx] == option->Left) {
													llptr2[llptr2_count - 1] += 1;
												}
												if (start[start_idx] == option->Right) {
													llptr2[llptr2_count - 1] += 2;
												}
												if (start[start_idx] == option->Assignment) {
													llptr2[llptr2_count - 1] += 3;
												}
											}
										}
										if (!_vecStack.empty()) {
											_vecStack.back()->itNum++;
										}
									}
									else {
										count++;
									}
									token_first = x + 1;

									start_idx = now_idx + 1;
								}
								else
								{
									token_first = token_first + 1;
									start_idx = start_idx + 1;
								}
							}
							else if (0 == state && -1 != (idx = Equal2(option->Left, *x))) {
								countTotal[no]++;

								if (llptr3) {
									llptr3[now_idx] = wiz::load_data::Utility::UT_IT_NUM();
									llptr3[now_idx].valid = 1;

									_vecStack.push_back(llptr3 + now_idx);
									llptr3_count++;

									if (_vecStack.size() >= 2) {
										_vecStack[_vecStack.size() - 2]->utNum++;
									}
								}

								token_last = x - 1;
								last_idx = now_idx - 1;
								if (token_last >= 0 && token_last - token_first + 1 > 0) {
									if (make) {
										//	aq->emplace_back(token_first, token_last - token_first + 1, false);

										//llptr[start_idx] = token_last - token_first + 1;
										llptr2[llptr2_count] = ((start_idx + num) << 32) + ((token_last - token_first + 1) << 2) + 0;
										llptr2_count++;

										{
											if (token_last - token_first + 1 == 1) {
												if (start[start_idx] == option->Left) {
													llptr2[llptr2_count - 1] += 1;
												}
												if (start[start_idx] == option->Right) {
													llptr2[llptr2_count - 1] += 2;
												}
												if (start[start_idx] == option->Assignment) {
													llptr2[llptr2_count - 1] += 3;
												}
											}
										}
										if (_vecStack.size() >= 2) {
											_vecStack[_vecStack.size() - 2]->itNum++;
										}
									}
									else {
										count++;
									}
									token_first = x + 1;

									start_idx = now_idx + 1;
									if (make) {
										//	aq->emplace_back(x, 1, false);
										//llptr[i] = 1;
										llptr2[llptr2_count] = ((i + num) << 32) + ((1) << 2) + 1;

										llptr2_count++;

									}
									else {
										count++;
									}
								}
								else {
									if (make) {
										//	aq->emplace_back(x, 1, false);
										//llptr[i] = 1; 
										llptr2[llptr2_count] = ((i + num) << 32) + ((1) << 2) + 1;
										llptr2_count++;

									}
									else {
										count++;
									}
									token_first = token_first + 1;
									start_idx = start_idx + 1;
								}
							}
							else if (0 == state && -1 != (idx = Equal2(option->Right, *x))) {
							countTotal[no]++;
								UT_IT_NUM* top = nullptr;
								if (llptr3) {
									if (!_vecStack.empty()) {
										llptr3[now_idx] = *_vecStack.back();

										top = _vecStack.back();
										_vecStack.pop_back();
									}
								}
								token_last = x - 1;
								last_idx = now_idx - 1;
								if (token_last >= 0 && token_last - token_first + 1 > 0) {
									if (make) {
										//	aq->emplace_back(token_first, token_last - token_first + 1, false);

										//llptr[start_idx] = token_last - token_first + 1;
										llptr2[llptr2_count] = ((start_idx + num) << 32) + ((token_last - token_first + 1) << 2) + 0;
										llptr2_count++;
										{
											if (token_last - token_first + 1 == 1) {
												if (start[start_idx] == option->Left) {
													llptr2[llptr2_count - 1] += 1;
												}
												if (start[start_idx] == option->Right) {
													llptr2[llptr2_count - 1] += 2;
												}
												if (start[start_idx] == option->Assignment) {
													llptr2[llptr2_count - 1] += 3;
												}
											}
										}
										if (!_vecStack.empty()) {
											_vecStack.back()->itNum++;
										}
									}
									else {

										count++;
									}
									token_first = x + 1;
									start_idx = now_idx + 1;
									if (make) {
										//	aq->emplace_back(x, 1, false);

										//llptr[i] = 1;
										llptr2[llptr2_count] = ((i + num) << 32) + ((1) << 2) + 2;
										llptr2_count++;
									}
									else {
										count++;
									}
								}
								else {
									if (make) {
										//	aq->emplace_back(x, 1, false);

										//llptr[i] = 1;
										llptr2[llptr2_count] = ((i + num) << 32) + ((1) << 2) + 2;
										llptr2_count++;
									}
									else {
										count++;
									}
									token_first = token_first + 1;
									start_idx = start_idx + 1;

								}
							}/*
							else if (0 == state && -1 != (idx = Equal(option->Left2, *x))) {
								llptr3[now_idx] = wiz::load_data::Utility::UT_IT_NUM();
								_stack.push_back(&llptr3[now_idx]);

								if (_stack.size() >= 2) {
									_stack[_stack.size() - 2]->utNum++;
								}


								token_last = x - 1;
								last_idx = now_idx - 1;
								if (token_last >= 0 && token_last - token_first + 1 > 0) {
									if (make) {
										llptr[start_idx] = token_last - token_first + 1;
										//	aq->emplace_back(token_first, token_last - token_first + 1, false);
										llptr2[llptr2_count] = start_idx + num;
										llptr2_count++;

										if (_stack.size() >= 2) {
											_stack[_stack.size() - 2]->itNum++;
										}
									}
									else {
										count++;
									}
									token_first = x + 1;

									start_idx = now_idx + 1;
									if (make) {
										//	aq->emplace_back(x, 1, false);

										llptr[i] = 1;
										llptr2[llptr2_count] = i + num;
										llptr2_count++;
									}
									else {
										count++;
									}
								}
								else {
									if (make) {
										//	aq->emplace_back(x, 1, false);

										llptr[i] = 1;
										llptr2[llptr2_count] = i + num;
										llptr2_count++;
									}
									else {
										count++;
									}
									token_first = token_first + 1;
									start_idx = start_idx + 1;
								}
							}
							else if (0 == state && -1 != (idx = Equal(option->Right2, *x))) {
								UT_IT_NUM* top = nullptr;
								if (!_stack.empty()) {
									top = _stack.back();
									_stack.pop_back();
								}

								token_last = x - 1;
								last_idx = now_idx - 1;
								if (token_last >= 0 && token_last - token_first + 1 > 0) {
									if (make) {
										llptr[start_idx] = token_last - token_first + 1;
										//	aq->emplace_back(token_first, token_last - token_first + 1, false);
										llptr2[llptr2_count] = start_idx + num;
										llptr2_count++;

										if (top) {
											_stack.back()->itNum++;
										}
									}
									else {
										count++;
									}
									token_first = x + 1;
									start_idx = now_idx + 1;
									if (make) {
										//	aq->emplace_back(x, 1, false);

										llptr[i] = 1;
										llptr2[llptr2_count] = i + num;
										llptr2_count++;
									}
									else {
										count++;
									}
								}
								else {
									if (make) {
										//	aq->emplace_back(x, 1, false);

										llptr[i] = 1;
										llptr2[llptr2_count] = i + num;
										llptr2_count++;
									}
									else {
										count++;
									}
									token_first = token_first + 1;
									start_idx = start_idx + 1;

								}
							}*/
							else if (0 == state &&
								-1 != Equal2(*x, option->LineComment)) { // different from load_data_from_file
								token_last = x - 1;
								last_idx = now_idx - 1;
								if (token_last >= 0 && token_last - token_first + 1 > 0) {
									if (make) {
										//	aq->emplace_back(token_first, token_last - token_first + 1, false);

										//llptr[start_idx] = token_last - token_first + 1;
										llptr2[llptr2_count] = ((start_idx + num) << 32) + ((token_last - token_first + 1) << 2) + 0;
										llptr2_count++; 
										{
											if (token_last - token_first + 1 == 1) {
												if (start[start_idx] == option->Left) {
													llptr2[llptr2_count - 1] += 1;
												}
												if (start[start_idx] == option->Right) {
													llptr2[llptr2_count - 1] += 2;
												}
												if (start[start_idx] == option->Assignment) {
													llptr2[llptr2_count - 1] += 3;
												}
											}
										}
									}
									else {
										count++;
									}
									x = token_last + 1;
									now_idx = last_idx + 1;
									token_first = token_last + 1;
									start_idx = last_idx + 1;
									token_last = token_last + 1;

									last_idx = last_idx + 1;
								}
								
								for (; x <= last; ++x) {
									//llptr[i] = 0;
									
									if (*x == '\n' || *x == '\0') // cf) '\r' ? '\0'?
									{
										break;
									}
									
									i++;
									now_idx++;
									token_last++;

									last_idx++;
								}

								//aq->emplace_back(token_first, token_last - token_first + 1, true); // cancel?
								token_first = x + 1;
								start_idx = now_idx + 1;
								
								token_last = x + 1;
								last_idx = now_idx + 1;
								continue;
							}
							else {
								//
							}

							token_last = x + offset;

							last_idx = now_idx + offset;
							
						}

						if (token_first < last)
						{
							if (make) {
								//aq->emplace_back(token_first, last - 1 - token_first + 1, false);
								//llptr[start_idx] = last - 1 - token_first + 1;
								if (last - 1 - token_first + 1 > 0) {
									llptr2[llptr2_count] = ((start_idx + num) << 32) + ((last -1  - token_first + 1) << 2) + 0;
									llptr2_count++;

									{
										if (last - 1 - token_first + 1 == 1) {
											if (start[start_idx] == option->Left) {
												llptr2[llptr2_count - 1] += 1;
											}
											if (start[start_idx] == option->Right) {
												llptr2[llptr2_count - 1] += 2;
											}
											if (start[start_idx] == option->Assignment) {
												llptr2[llptr2_count - 1] += 3;
											}
										}
									}
								}
							}
							else {
								count++;
							}
						}

						if (this->llptr2_len) {
							*(this->llptr2_len) = llptr2_count;
						}

						if (state != 0) {
							throw "Scanning Error in qouted string";
						}
						return count;
					}
				}
			public:
				void operator() () {
					timeA[no] = clock();
					long long size = chk2(true);
					timeB[no] = clock();
					//wiz::Out << b - a << "ms\n";
					/*aq->reserve(size);
					std::vector<long long>& length = *_length;
					int count = 0;
					for (int i = 0; i < length.size();) {
						if (length[i] > 0) {
							aq->emplace_back(this->start + i, length[i], false);
							i += (length)[i];
							count++;
						}
						else {
							++i;
						}
					}*/
				}
			};

			class BomInfo
			{
			public:
				long long bom_size;
				char seq[5];
			};
			
			const static size_t BOM_COUNT = 1;

			enum BomType { UTF_8, UNDEFINED = -1 }; // ANSI - UNDEFINED
			
			inline static const BomInfo bomInfo[1] = {
				{ 3, { 0xEF, 0xBB, 0xBF } }
			};
			static BomType ReadBom(std::ifstream& file) {
				char btBom[5] = { 0, };
				file.read(btBom, 5);
				size_t readSize = file.gcount();

				if (0 == readSize) {
					file.clear();
					file.seekg(0, std::ios_base::beg);
					return UNDEFINED;
				}

				BomInfo stBom = { 0, };
				BomType type = ReadBom(btBom, readSize, stBom);
				
				if (type == UNDEFINED) { // ansi
					file.clear();
					file.seekg(0, std::ios_base::beg);
					return UNDEFINED;
				}
				
				file.clear();
				file.seekg(stBom.bom_size, std::ios_base::beg);
				return type;
			}

			static BomType ReadBom(const char* contents, size_t length, BomInfo& outInfo) {
				char btBom[5] = { 0, };
				size_t testLength = length < 5 ? length : 5;
				memcpy(btBom, contents, testLength);

				size_t i, j;
				for (i = 0; i < BOM_COUNT; ++i) {
					const BomInfo& bom = bomInfo[i];
					
					if (bom.bom_size > testLength) {
						continue;
					}

					bool matched = true;

					for (j = 0; j < bom.bom_size; ++j) {
						if (bom.seq[j] == btBom[j]) {
							continue;
						}

						matched = false;
						break;
					}

					if (!matched) {
						continue;
					}

					outInfo = bom;

					return (BomType)i;
				}

				return UNDEFINED;
			}


			static std::pair<bool, int> Reserve2_3(std::ifstream& inFile, VECTOR<Token2>* aq, const int num, bool* isFirst, const wiz::LoadDataOption& option, int thr_num, char*& _buffer)
			{
				if (inFile.eof()) {
					return { false, 0 };
				}

				int a = clock();

				//int count = 0;
				std::string temp;
				char* buffer = nullptr;// = new char[length + 1]; // 
				std::vector<long long> start(thr_num + 1, 0);
				std::vector<long long> last(thr_num + 1, 0);

				if (thr_num > 1) {
					inFile.seekg(0, inFile.end);
					unsigned long long length = inFile.tellg();
					inFile.seekg(0, inFile.beg);

					BomType x = ReadBom(inFile);
					if (x == UTF_8) {
						wiz::Out << "UTF-8" << "\n";
					}
					length = length - inFile.tellg();
					

					buffer = new char[length +1]; // 

												   // read data as a block:
					inFile.read(buffer, length);
					inFile.seekg(0, inFile.end);
					char temp;
					inFile >> temp;

					buffer[length] = '\0';
					
					start[0] = 0;

					
					for (int i = 1; i < thr_num; ++i) {
						start[i] = length / thr_num * i;
						for (int x = start[i]; x <= length; ++x) {
							// here bug is..  " check "
							if ('\r' == buffer[x] || '\n' == (buffer[x]) || x == length) {
								start[i] = x;
								//	wiz::Out << "start " << start[i] << ENTER;
								break;
							}
						}
					}
					for (int i = 0; i < thr_num - 1; ++i) {
						last[i] = start[i + 1] - 1;
						for (int x = last[i]; x <= length; ++x) {
							if ('\r' == buffer[x] || '\n' == (buffer[x]) || x == length) {
								last[i] = x;
								//	wiz::Out << "start " << start[i] << ENTER;
								break;
							}
						}
					}
					last[thr_num - 1] = length;
					//	wiz::Out << last[thr_num - 1] << ENTER;
				}
				else {
					inFile.seekg(0, inFile.end);
					unsigned long long length = inFile.tellg();
					inFile.seekg(0, inFile.beg);

					ReadBom(inFile);
					length = length - inFile.tellg();

					buffer = new char[length + 1]; // 

												   // read data as a block:
					inFile.read(buffer, length);
					buffer[length] = '\0';

					inFile.seekg(0, inFile.end);
					char temp;
					inFile >> temp;

					start[0] = 0;
					last[0] = length;
				}

				//int a = clock();
				//debug

				if (thr_num > 1) {
					//// in string, there are '\r' or '\n' etc.. - no '\r' or '\n'?
					for (int i = thr_num - 1; i > 0; --i) {
						int last_enter_idx = -1;
						int first_enter_idx = -1;

						// find last_enter_idx, first_enter_idx -
						//// has bug - " abc
						////				def"
						for (int j = last[i - 1]; j <= last[i]; ++j) {
							if (buffer[j] == '\r' || buffer[j] == '\n' || j == last[thr_num - 1]) {
								last_enter_idx = j;
								break;
							}
						}

						{
							int state = 0;
							int j = last[i - 1] - 1;
							for (; j >= 0; --j) {
								if (state == 0 && (buffer[j] == '\r' || buffer[j] == '\n')) {
									first_enter_idx = j;
									break;
								}
								else if (j == start[i - 1]) {
									--i;
								}
							}

							if (-1 == first_enter_idx) {
								first_enter_idx = 0;
							}
						}

						// exchange with whitespace
						{
							int state = 0;
							int sharp_start = -1;

							for (int j = first_enter_idx + 1; j < last_enter_idx; ++j) {
								if (0 == state && buffer[j] == '\'') {
									state = 1;
								}
								else if (1 == state && buffer[j-1] == '\\' && buffer[j] == '\'') {
									
								}
								else if (1 == state && buffer[j] == '\'') {
									state = 0;
								}
								else if (0 == state && buffer[j] == '\"') {
									state = 3;
								}
								else if (3 == state && buffer[j - 1] == '\\' && buffer[j] == '\"') {

								}
								else if (3 == state && buffer[j] == '\"') {
									state = 0;
								}
								else if (0 == state && buffer[j] == '#') {
									sharp_start = j;
								}
							}

							if (-1 != sharp_start) {
								for (int k = sharp_start; k < last_enter_idx; ++k) {
									buffer[k] = ' ';
								}
							}
						}
					}

					std::vector<VECTOR<Token2>> partial_list(thr_num, VECTOR<Token2>());
					std::vector<std::thread> thr(thr_num);

					for (int i = 0; i < thr_num; ++i) {
						//	wiz::Out << last[i] - start[i] << ENTER;
						partial_list[i].reserve((last[i] - start[i]));
						thr[i] = std::thread(DoThread3(buffer + start[i], buffer + last[i], &partial_list[i], &option));
					}

					for (int i = 0; i < thr_num; ++i) {
						thr[i].join();
					}

					int new_size = aq->size() + 2;
					for (int i = 0; i < thr_num; ++i) {
						new_size = new_size + partial_list[i].size();
					}

					aq->reserve(new_size);

					for (int i = 0; i < thr_num; ++i) {
						//for (int j = 0; j < partial_list[i].size(); ++j) {
						//	aq->push_back(std::move(partial_list[i][j]));
						//}
						aq->insert(aq->end(), make_move_iterator(partial_list[i].begin()), make_move_iterator(partial_list[i].end()));
					}
				}
				else {
					VECTOR<Token2> temp;

					temp.reserve((last[0] - start[0]) / 10);

					DoThread3 dothr(buffer + start[0], buffer + last[0], &temp, &option);

					dothr();

					aq->insert(aq->end(), make_move_iterator(temp.begin()), make_move_iterator(temp.end()));
				}

				// debug!
				{
					//	for (int i = 0; i < aq->size(); ++i) {
					//		wiz::Out << i + 1 << " " << std::string((*aq)[i].str, (*aq)[i].len) << ENTER;
					//	}
				}

				//delete[] buffer;

			//				log_result = log_result + clock() - a;
				//wiz::Out << "lexing " << clock() - a << "ms" << ENTER;

				_buffer = buffer;

				return{ true, 1 };
			}

			class DoThread3_2 // need to rename!
			{
			private:
				char* start;
				char* last;
			public:
				VECTOR<Token3>* aq;
				const wiz::LoadDataOption2* option; // wiz::LoadDatOption2 ?
				std::vector<long long>* _length;
			public:
				DoThread3_2(char* start, char* last, VECTOR<Token3>* aq, const wiz::LoadDataOption2* option,
					std::vector<long long>* _length) //, list<std::string>* aq)//, int strVecStart, int strVecEnd)
					: start(start), last(last), aq(aq), option(option) // , strVecStart(strVecStart), strVecEnd(strVecEnd)
				{
					this->_length = _length;
				}
				~DoThread3_2() {
					//
				}
			private:
				int checkDelimiter(const char* start, const char* last, const std::vector<std::string>& delimiter)
				{

					int sum = 0;
					for (int delim_num = 0; delim_num < delimiter.size(); ++delim_num) {
						// size check
						if (start + delimiter[delim_num].size() - 1 > last) {
							continue;
						}

						for (const char* x = start; x <= start + delimiter[delim_num].size() - 1; ++x) {
							if (*x == delimiter[delim_num][x - start]) {

							}
							else {
								sum--;
								break;
							}
						}
						sum++;

						if (sum > 0) {
							return delim_num;
						}
					}

					return -1;
				}

				long long chk2(bool make) {
					{
						std::vector<long long>& length = *_length; // removal?

						long long start_idx = 0;
						long long last_idx = 0;
						long long count = 0;
						char* token_first = start;
						char* token_last = start;
						int state = 0;

						long long now_idx = 0;
						for (long long i = 0; start + i < last; ++i, ++now_idx) {
							char* x = start + i;
							long long offset = 0;
							int idx;
							if (0 == state && '\'' == *x) {
								//token_last = x - 1;
								//if (token_last >= 0 && token_last - token_first + 1 > 0) {
								//	aq->emplace_back(statement.substr(token_first, token_last - token_first + 1));
								//}
								state = 2;
								//token_first = i; 
								token_last = x;
								last_idx = now_idx;
							}
							else if (2 == state && '\\' == *(x - 1) && '\'' == *x) {
								token_last = x;
								last_idx = now_idx;
							}
							else if (2 == state && '\'' == *x) {
								state = 0; token_last = x;
								last_idx = now_idx;
							}
							else if (0 == state && '\"' == *x) {
								//token_last = x - 1;
								//if (token_last >= 0 && token_last - token_first + 1 > 0) {
								//	aq->emplace_back(statement.substr(token_first, token_last - token_first + 1));
								//}

								state = 1;
								//token_first = i; 
								token_last = x;
								last_idx = now_idx;
							}
							else if (1 == state && '\\' == *(x - 1) && '\"' == *x) {
								token_last = x;
								last_idx = now_idx;
							}
							else if (1 == state && '\"' == *x) {
								state = 0; token_last = x;
								last_idx = now_idx;
							}
							else if (0 == state && -1 != (idx = Equal2(option->Removal, *x)))
							{
								token_last = x - 1;
								last_idx = now_idx - 1;

								if (token_last >= 0 && token_last - token_first + 1 > 0) {
									if (make) {
										aq->emplace_back(token_first, token_last - token_first + 1, false);

									}
									else {
										length[start_idx] = token_last - token_first + 1;
										count++;
									}


									token_first = x + 1;
									start_idx = now_idx + 1;
								}
								else {
									token_first = token_first + 1;
									start_idx = start_idx + 1;
								}
								continue;
							}
							else if (0 == state && -1 != (idx = Equal2(option->Assignment, *x))) {
								token_last = x - 1;
								last_idx = now_idx - 1;

								if (token_last >= 0 && token_last - token_first + 1 > 0) {
									if (make) {
										aq->emplace_back(token_first, token_last - token_first + 1, false);

									}
									else {
										length[start_idx] = token_last - token_first + 1;
										count++;
									}

									token_first = x + 1;
									start_idx = now_idx + 1;
									if (make) {
										aq->emplace_back(x, 1, false);

									}
									else {
										length[now_idx] = 1; count++;
									}
								}
								else {
									if (make) {
										aq->emplace_back(x, 1, false);
									}
									else {
										length[now_idx] = 1; count++;
									}
									token_first = token_first + 1;
									start_idx = start_idx + 1;

								}
							}
							else if (0 == state && isWhitespace(*x)) { // isspace ' ' \t \r \n , etc... ?
								token_last = x - 1;
								last_idx = now_idx - 1;
								if (token_last >= 0 && token_last - token_first + 1 > 0) {
									if (make) {
										aq->emplace_back(token_first, token_last - token_first + 1, false);

									}
									else {
										length[start_idx] = token_last - token_first + 1; count++;
									}
									token_first = x + 1;

									start_idx = now_idx + 1;
								}
								else
								{
									token_first = token_first + 1;
									start_idx = start_idx + 1;
								}
							}
							else if (0 == state && -1 != (idx = Equal2(option->Left, *x))) {
								token_last = x - 1;
								last_idx = now_idx - 1;
								if (token_last >= 0 && token_last - token_first + 1 > 0) {
									if (make) {
										aq->emplace_back(token_first, token_last - token_first + 1, false);

									}
									else {
										length[start_idx] = token_last - token_first + 1; count++;
									}
									token_first = x + 1;

									start_idx = now_idx + 1;
									if (make) {
										aq->emplace_back(x, 1, false);
										count++;
									}
									else {
										length[now_idx] = 1;
									}
								}
								else {
									if (make) {
										aq->emplace_back(x, 1, false);

									}
									else {
										length[now_idx] = 1; count++;
									}
									token_first = token_first + 1;
									start_idx = start_idx + 1;
								}
							}
							else if (0 == state && -1 != (idx = Equal2(option->Right, *x))) {
								token_last = x - 1;
								last_idx = now_idx - 1;
								if (token_last >= 0 && token_last - token_first + 1 > 0) {
									if (make) {
										aq->emplace_back(token_first, token_last - token_first + 1, false);

									}
									else {
										length[start_idx] = token_last - token_first + 1; count++;
									}
									token_first = x + 1;
									start_idx = now_idx + 1;
									if (make) {
										aq->emplace_back(x, 1, false);

									}
									else {
										length[now_idx] = 1; count++;
									}
								}
								else {
									if (make) {
										aq->emplace_back(x, 1, false);

									}
									else {
										length[now_idx] = 1; count++;
									}
									token_first = token_first + 1;
									start_idx = start_idx + 1;

								}
							}
							else if (0 == state && -1 != (idx = Equal2(option->Left2, *x))) {
								token_last = x - 1;
								last_idx = now_idx - 1;
								if (token_last >= 0 && token_last - token_first + 1 > 0) {
									if (make) {
										aq->emplace_back(token_first, token_last - token_first + 1, false);

									}
									else {
										length[start_idx] = token_last - token_first + 1; count++;
									}
									token_first = x + 1;

									start_idx = now_idx + 1;
									if (make) {
										aq->emplace_back(x, 1, false);
										count++;
									}
									else {
										length[now_idx] = 1;
									}
								}
								else {
									if (make) {
										aq->emplace_back(x, 1, false);

									}
									else {
										length[now_idx] = 1; count++;
									}
									token_first = token_first + 1;
									start_idx = start_idx + 1;
								}
							}
							else if (0 == state && -1 != (idx = Equal2(option->Right2, *x))) {
								token_last = x - 1;
								last_idx = now_idx - 1;
								if (token_last >= 0 && token_last - token_first + 1 > 0) {
									if (make) {
										aq->emplace_back(token_first, token_last - token_first + 1, false);

									}
									else {
										length[start_idx] = token_last - token_first + 1; count++;
									}
									token_first = x + 1;
									start_idx = now_idx + 1;
									if (make) {
										aq->emplace_back(x, 1, false);

									}
									else {
										length[now_idx] = 1; count++;
									}
								}
								else {
									if (make) {
										aq->emplace_back(x, 1, false);

									}
									else {
										length[now_idx] = 1; count++;
									}
									token_first = token_first + 1;
									start_idx = start_idx + 1;

								}
							}
							else if (0 == state &&
								-1 != Equal2(*x, option->LineComment)) { // different from load_data_from_file
								token_last = x - 1;
								last_idx = now_idx - 1;
								if (token_last >= 0 && token_last - token_first + 1 > 0) {
									if (make) {
										aq->emplace_back(token_first, token_last - token_first + 1, false);

									}
									else {
										length[start_idx] = token_last - token_first + 1; count++;
									}
									x = token_last + 1;
									now_idx = last_idx + 1;
									token_first = token_last + 1;
									start_idx = last_idx + 1;
									token_last = token_last + 1;

									last_idx = last_idx + 1;
								}
								for (; x < last; ++x) {
									token_last++;

									last_idx++;
									if (*x == '\n' || *x == '\0') // cf) '\r' ? '\0'?
									{
										break;
									}
								}
								//aq->emplace_back(token_first, token_last - token_first + 1, true); // cancel?
								token_first = x + 1;
								start_idx = now_idx + 1;
								offset = 1;
							}
							else {
								//
							}

							token_last = x + offset;

							last_idx = now_idx + offset;
						}

						if (token_first < last)
						{
							if (make) {
								aq->emplace_back(token_first, last - 1 - token_first + 1, false);

							}
							else {
								length[start_idx] = last - 1 - token_first + 1; count++;
							}
						}

						return count;
					}
				}
			public:
				void operator() () {
					long long size = chk2(true);
					/*aq->reserve(size);
					std::vector<long long>& length = *_length;
					int count = 0;
					for (int i = 0; i < length.size();) {
						if (length[i] > 0) {
							aq->emplace_back(this->start + i, length[i], false);
							i += (length)[i];
							count++;
						}
						else {
							++i;
						}
					}*/
				}
			};
			// no enter strings? " abc \n def " and problem - one line!?
			static std::pair<bool, int> Reserve2_3_2(std::ifstream& inFile, VECTOR<Token3>* aq, const int num, bool* isFirst, const wiz::LoadDataOption2& option, int thr_num, char*& _buffer)
			{
				if (inFile.eof()) {
					return { false, 0 };
				}

				//int a = clock();

				//int count = 0;
				std::string temp;
				char* buffer = nullptr;// = new char[length + 1]; // 
				std::vector<long long> start(thr_num + 1, 0);
				std::vector<long long> last(thr_num + 1, 0);

				if (thr_num > 0) {
					inFile.seekg(0, inFile.end);
					unsigned long long length = inFile.tellg();
					inFile.seekg(0, inFile.beg);

					ReadBom(inFile);
					length = length - inFile.tellg();

					buffer = new char[length + 1]; // 

												   // read data as a block:
					inFile.read(buffer, length);
					inFile.seekg(0, inFile.end);
					char temp;
					inFile >> temp;

					buffer[length] = '\0';

					start[0] = 0;


					// todo - linear check?
					for (int i = 1; i < thr_num; ++i) {
						start[i] = length / thr_num * i;
						for (int x = start[i]; x <= length; ++x) {
							// here bug is..  " check "
							if ('\r' == buffer[x] || '\n' == (buffer[x]) || '\0' == buffer[x]) {
								start[i] = x;
								//	wiz::Out << "start " << start[i] << ENTER;
								break;
							}
						}
					}
					for (int i = 0; i < thr_num - 1; ++i) {
						last[i] = start[i + 1] - 1;
						for (int x = last[i]; x <= length; ++x) {
							if ('\r' == buffer[x] || '\n' == (buffer[x]) || '\0' == buffer[x]) {
								last[i] = x;
								//	wiz::Out << "start " << start[i] << ENTER;
								break;
							}
						}
					}
					last[thr_num - 1] = length;
					//	wiz::Out << last[thr_num - 1] << ENTER;
				}
				else {
					inFile.seekg(0, inFile.end);
					unsigned long long length = inFile.tellg();
					inFile.seekg(0, inFile.beg);

					ReadBom(inFile);
					length = length - inFile.tellg();

					buffer = new char[length + 1]; // 

												   // read data as a block:
					inFile.read(buffer, length);
					buffer[length] = '\0';

					inFile.seekg(0, inFile.end);
					char temp;
					inFile >> temp;

					start[0] = 0;
					last[0] = length;
				}

				//int a = clock();
				//debug

				if (thr_num > 0) {
					//// in string, there are '\r' or '\n' etc.. - no '\r' or '\n'?
					for (int i = thr_num - 1; i > 0; --i) {
						int last_enter_idx = -1;
						int first_enter_idx = -1;

						// find last_enter_idx, first_enter_idx -
						//// has bug - " abc
						////				def"
						for (int j = last[i - 1]; j <= last[i]; ++j) {
							if (buffer[j] == '\r' || buffer[j] == '\n' || j == last[thr_num - 1]) {
								last_enter_idx = j;
								break;
							}
						}

						{
							int state = 0;
							int j = last[i - 1] - 1;
							for (; j >= 0; --j) {
								if (state == 0 && (buffer[j] == '\r' || buffer[j] == '\n')) {
									first_enter_idx = j;
									break;
								}
								else if (j == start[i - 1]) {
									--i;
								}
							}

							if (-1 == first_enter_idx) {
								first_enter_idx = 0;
							}
						}

						// exchange with whitespace
						{
							int state = 0;
							int sharp_start = -1;

							for (int j = first_enter_idx + 1; j < last_enter_idx; ++j) {
								if (0 == state && buffer[j] == '\'') {
									state = 1;
								}
								else if (1 == state && buffer[j - 1] == '\\' && buffer[j] == '\'') {

								}
								else if (1 == state && buffer[j] == '\'') {
									state = 0;
								}
								else if (0 == state && buffer[j] == '\"') {
									state = 3;
								}
								else if (3 == state && buffer[j - 1] == '\\' && buffer[j] == '\"') {

								}
								else if (3 == state && buffer[j] == '\"') {
									state = 0;
								}
								else if (0 == state && buffer[j] == '#') {
									sharp_start = j;
								}
							}

							if (-1 != sharp_start) {
								for (int k = sharp_start; k < last_enter_idx; ++k) {
									buffer[k] = ' ';
								}
							}
						}
					}

					std::vector<VECTOR<Token3>> partial_list(thr_num, VECTOR<Token3>());
					std::vector<std::thread> thr(thr_num);
					std::vector<std::vector<long long>> length(thr_num);
					for (int i = 0; i < length.size(); ++i) {
						//length[i].resize(last[i] - start[i] + 1);
					}
					for (int i = 0; i < thr_num; ++i) {
						//	wiz::Out << last[i] - start[i] << ENTER;
						//partial_list[i].reserve((last[i] - start[i]) / 10;
						thr[i] = std::thread(DoThread3_2(buffer + start[i], buffer + last[i], &partial_list[i], &option, &length[i]));
					}

					for (int i = 0; i < thr_num; ++i) {
						thr[i].join();
					}

					int new_size = aq->size() + 2; // chk!
					for (int i = 0; i < thr_num; ++i) {
						new_size = new_size + partial_list[i].size();
					}

					aq->reserve(new_size);

					for (int i = 0; i < thr_num; ++i) {
						//for (int j = 0; j < partial_list[i].size(); ++j) {
						//	aq->push_back(std::move(partial_list[i][j]));
						//}
						aq->insert(aq->end(), make_move_iterator(partial_list[i].begin()), make_move_iterator(partial_list[i].end()));
					}
				}
				else {
					VECTOR<Token3> temp;

					temp.reserve((last[0] - start[0]) / 10);
					std::vector<long long> length; //  (last[0] - start[0] + 1);
					DoThread3_2 dothr(buffer + start[0], buffer + last[0], &temp, &option, &length);

					dothr();

					aq->insert(aq->end(), make_move_iterator(temp.begin()), make_move_iterator(temp.end()));
				}

				//delete[] buffer;

				//				log_result = log_result + clock() - a;
				//wiz::Out << "lexing " << clock() - a << "ms" << ENTER;

				_buffer = buffer;

				return{ true, 1 };
			}


			static std::pair<bool, int> Reserve2_4(std::ifstream& inFile, const int num, bool* isFirst, const wiz::LoadDataOption2& option, int thr_num,
				char*& _buffer, long long*& _llptr, long long* _buffer_len, long long*& _llptr2, long long* _llptr2_len, UT_IT_NUM*& _llptr3)
			{
				if (inFile.eof()) {
					return { false, 0 };
				}

				int a = clock();

				//int count = 0;
				std::string temp;
				char* buffer = nullptr;// = new char[length + 1]; // 
				std::vector<long long> start(thr_num + 1, 0);
				std::vector<long long> last(thr_num + 1, 0);
				std::vector<long long> llptr2_len(thr_num + 1, 0);
				long long file_length;
				if (thr_num > 0) {
					inFile.seekg(0, inFile.end);
					unsigned long long length = inFile.tellg();
					inFile.seekg(0, inFile.beg);

					BomType x = ReadBom(inFile);
				//	wiz::Out << "length " << length << "\n";
					if (x == UTF_8) {
						length = length - 3;
					}
				//	wiz::Out << "length " << length << "\n";
					file_length = length;
					if (x == UTF_8) {
						wiz::Out << "UTF-8" << "\n";
					}

					buffer = new char[file_length + 1]; // 

												   // read data as a block:
					inFile.read(buffer, file_length);
				//	inFile.seekg(0, inFile.end);
				//	char temp;
				//	inFile >> temp;
					
					buffer[file_length] = '\0';

					start[0] = 0;


					// todo - linear check?
					for (int i = 1; i < thr_num; ++i) {
						start[i] = file_length / thr_num * i;
						for (int x = start[i]; x <= file_length; ++x) {
							// here bug is..  " check "
							if ('\r' == buffer[x] || '\n' == (buffer[x]) || '\0' == buffer[x]) {
								start[i] = x;
								//	wiz::Out << "start " << start[i] << ENTER;
								break;
							}
						}
					}
					for (int i = 0; i < thr_num - 1; ++i) {
						last[i] = start[i + 1] - 1;
						for (int x = last[i]; x <= file_length; ++x) {
							if ('\r' == buffer[x] || '\n' == (buffer[x]) || '\0' == buffer[x]) {
								last[i] = x;
								//	wiz::Out << "start " << start[i] << ENTER;
								break;
							}
						}
					}
					last[thr_num - 1] = file_length;
					//	wiz::Out << last[thr_num - 1] << ENTER;
				}
				else {
					inFile.seekg(0, inFile.end);
					unsigned long long length = inFile.tellg();
					inFile.seekg(0, inFile.beg);

					BomType x = ReadBom(inFile);
					//wiz::Out << "length " << length << "\n";
					if (x == UTF_8) {
						length = length - 3;
					}
					//wiz::Out << "length " << length << "\n";
					file_length = length;
					if (x == UTF_8) {
						wiz::Out << "UTF-8" << "\n";
					}
					buffer = new char[file_length + 1]; // 

												   // read data as a block:
					inFile.read(buffer, file_length);
					buffer[file_length] = '\0';

				//	inFile.seekg(0, inFile.end);
				//	char temp;
				//	inFile >> temp;

					start[0] = 0;
					last[0] = file_length;
				}

				int b = clock();
				wiz::Out << "load file to char array " << b - a << "ms\n";

				long long* llptr = nullptr;
				long long* llptr2 = nullptr;
				UT_IT_NUM* llptr3 = nullptr;

				if (thr_num > 0) {
					//// in string, there are '\r' or '\n' etc.. - no '\r' or '\n'?
					for (int i = thr_num - 1; i > 0; --i) {
						int last_enter_idx = -1;
						int first_enter_idx = -1;

						// find last_enter_idx, first_enter_idx -
						//// has bug - " abc
						////				def"
						for (int j = last[i - 1]; j <= last[i]; ++j) {
							if (buffer[j] == '\r' || buffer[j] == '\n' || j == last[thr_num - 1]) {
								last_enter_idx = j;
								break;
							}
						}

						{
							int state = 0;
							int j = last[i - 1] - 1;
							for (; j >= 0; --j) {
								if (state == 0 && (buffer[j] == '\r' || buffer[j] == '\n')) {
									first_enter_idx = j;
									break;
								}
								else if (j == start[i - 1]) {
									--i;
								}
							}

							if (-1 == first_enter_idx) {
								first_enter_idx = 0;
							}
						}

						// exchange with whitespace
						{
							int state = 0;
							int sharp_start = -1;

							for (int j = first_enter_idx + 1; j < last_enter_idx; ++j) {
								if (0 == state && buffer[j] == '\'') {
									state = 1;
								}
								else if (1 == state && buffer[j - 1] == '\\' && buffer[j] == '\'') {

								}
								else if (1 == state && buffer[j] == '\'') {
									state = 0;
								}
								else if (0 == state && buffer[j] == '\"') {
									state = 3;
								}
								else if (3 == state && buffer[j - 1] == '\\' && buffer[j] == '\"') {

								}
								else if (3 == state && buffer[j] == '\"') {
									state = 0;
								}
								else if (0 == state && buffer[j] == '#') {
									sharp_start = j;
								}
							}

							if (-1 != sharp_start) {
								for (int k = sharp_start; k < last_enter_idx; ++k) {
									buffer[k] = ' ';
								}
							}
						}
					}

					//std::vector<VECTOR<Token2>> partial_list(thr_num, VECTOR<Token2>());
					std::vector<std::thread> thr(thr_num);
					
					int x = clock();
					//llptr = new long long[file_length];
					llptr2 = new long long[file_length];
					
					//llptr3 = new wiz::load_data::Utility::UT_IT_NUM[file_length];
					llptr3 = nullptr;// (wiz::load_data::Utility::UT_IT_NUM*)calloc(file_length, sizeof(wiz::load_data::Utility::UT_IT_NUM));
					int y = clock();
					wiz::Out << "calloc " << y - x << "ms \n";

					std::vector<long long> counter(thr_num, 0);
					for (int i = 0; i < thr_num; ++i) {
						thr[i] = std::thread(DoThread4(buffer + start[i], buffer + last[i], &option,
							llptr + start[i], llptr2 + start[i], start[i], &counter[i], nullptr, i));// llptr3 + start[i], i));
					}

					for (int i = 0; i < thr_num; ++i) {
						thr[i].join();
					}

					y = clock();
					wiz::Out << "parallel lexing " << y - x << "ms\n";


					//for (int i = 0; i < 8; ++i) {
					//	wiz::Out << timeB[i] - timeA[i] << "ms ";
					//}
				//	wiz::Out << "\n";

					int a = clock();
					long long sum = 0;
					//
					for (int i = 1; i < counter.size(); ++i) {
						//wiz::Out << counter[i - 1] << " ";
						sum += counter[i - 1];

						// memcpy?
						memmove(llptr2 + sum, llptr2 + start[i], counter[i] * sizeof(long long) / sizeof(char));

						//for (int j = 0; j < counter[i]; ++j) {
						//	llptr2[sum + j] = llptr2[start[i] + j];
						//}
					}
					//wiz::Out << counter.back() << "\n";

					//for (int i = 0; i < 8; ++i) {
					//	wiz::Out << countTotal[i] << " ";
					//}
				//	wiz::Out << "\n";


					*_llptr2_len = sum + counter.back();
					int b = clock();
					//wiz::Out << "etc " << b - a << "ms\n";
					//	int new_size = aq->size() + 2; // chk!
					//	for (int i = 0; i < thr_num; ++i) {
					//		new_size = new_size + partial_list[i].size();
					//	}

						//aq->reserve(new_size);

						//for (int i = 0; i < thr_num; ++i) 
						//{
							//for (int j = 0; j < partial_list[i].size(); ++j) {
							//	aq->push_back(std::move(partial_list[i][j]));
							//}
							//aq->insert(aq->end(), make_move_iterator(partial_list[i].begin()), make_move_iterator(partial_list[i].end()));
						//}
				}
				else {
					//llptr = new long long[file_length];
					llptr2 = new long long[file_length];
					//llptr3 = (UT_IT_NUM*)malloc(sizeof(UT_IT_NUM) * file_length);

					long long len;
					DoThread4 dothr(buffer + start[0], buffer + last[0], &option, llptr, llptr2, start[0], &len, llptr3, 0);

					dothr();

					*_llptr2_len = len;
					//aq->insert(aq->end(), make_move_iterator(temp.begin()), make_move_iterator(temp.end()));
				}

				//delete[] buffer;

				//				log_result = log_result + clock() - a;
				//wiz::Out << "lexing " << clock() - a << "ms" << ENTER;

				_buffer = buffer;
				_llptr = llptr;
				_llptr2 = llptr2;
				*_buffer_len = file_length;
				_llptr3 = llptr3;

				return{ true, 1 };
			}

			/// must lineNum > 0
			static std::pair<bool, int> Reserve(std::ifstream& inFile, std::list<std::string>& strVec, const int num = 1)
			{
				std::string temp;
				int count = 0;

				for (int i = 0; i < num && (inFile >> temp); ++i) {
					strVec.push_back(temp);
					count++;
				}
				return{ count > 0, count };
			}
		private:
			// chk!!
			template <class Reserver>
			static bool ChkComment(ARRAY_QUEUE<Token>& strVec, wiz::load_data::UserType* ut, Reserver reserver, const int offset, const wiz::LoadDataOption& option)
			{
				if (strVec.size() < offset) {
					reserver(strVec, option);
					while (strVec.size() < offset) // 
					{
						reserver(strVec, option);
						if (
							strVec.size() < offset &&
							reserver.end()
							) {
							return false;
						}
					}
				}

				auto x = strVec.begin();
				int count = 0;

				do {
					if (x->isComment) { // x.ptr->, x.data->?
						ut->PushComment((std::move(x->str)));
						x = strVec.erase(x);
					}
					else if (count == offset - 1) {
						return true;
					}
					else {
						count++;
						++x;
						//x.ptr++;
						//x.pos++;
					}

					if (x == strVec.end()) {
						reserver(strVec, option);
						x = strVec.begin() + count;

						while (strVec.size() < offset) // 
						{
							reserver(strVec, option);
							x = strVec.begin() + count;
							if (
								strVec.size() < offset &&
								reserver.end()
								) {
								return false;
							}
						}
					}
				} while (true);
			}
		public:
			template<class Reserver>
			static std::string Top(ARRAY_QUEUE<Token>& strVec, wiz::load_data::UserType* ut, Reserver reserver, const wiz::LoadDataOption& option)
			{
				if (strVec.empty() || strVec[0].isComment) {
					if (false == ChkComment(strVec, ut, reserver, 1, option)) {
						return std::string();
					}
				}
				if (strVec.empty()) { return std::string(); }
				return strVec[0].str;
			}
			template <class Reserver>
			static bool Pop(ARRAY_QUEUE<Token>& strVec, std::string* str, wiz::load_data::UserType* ut, Reserver reserver, const wiz::LoadDataOption& option)
			{
				if (strVec.empty() || strVec[0].isComment) {
					if (false == ChkComment(strVec, ut, reserver, 1, option)) {
						return false;
					}
				}

				if (strVec.empty()) {
					return false;
				}

				if (str) {
					Token token;
					token = strVec.front();
					strVec.pop_front();
					*str = (std::move(token.str));
					//*str = std::move(strVec.front().str);
				}
				else {
					strVec.pop_front();
				}
				//strVec.pop_front();

				return true;
			}

			// lookup just one!
			template <class Reserver>
			static std::pair<bool, std::string> LookUp(ARRAY_QUEUE<Token>& strVec, wiz::load_data::UserType* ut, Reserver reserver, const wiz::LoadDataOption& option)
			{
				if (!(strVec.size() >= 2 && false == strVec[0].isComment && false == strVec[1].isComment)) {
					if (false == ChkComment(strVec, ut, reserver, 2, option)) {
						return{ false, "" };
					}
				}
			
				if (strVec.size() >= 2) {
					return{ true, strVec[1].str };
				}
				return{ false, "" };
			}
			
			template<class Reserver>
			static std::string Top(std::vector<ARRAY_QUEUE<Token>>& strVec, wiz::load_data::UserType* ut, Reserver reserver, const wiz::LoadDataOption& option, int* thread_id)
			{
				const int coreNum = strVec.size();
				int next_thread_id = *thread_id;
				int ok = 0;

				while (strVec[next_thread_id].empty()) {
					next_thread_id = (*thread_id + 1) % coreNum;
					*thread_id = next_thread_id;
					ok++;
					if (ok == strVec.size() && strVec[next_thread_id].empty()) {
						//wiz::Out << "errorA" << ENTER;
						break;
					}
				}

				while (strVec[next_thread_id].empty() == false && strVec[next_thread_id].front().str.empty()) {
					strVec[next_thread_id].pop_front();

					next_thread_id = (*thread_id + 1) % coreNum;

					*thread_id = next_thread_id;
				}

				if (strVec[next_thread_id].empty()) { return std::string(); }
				return strVec[next_thread_id][0].str;
			}
			template <class Reserver>
			static bool Pop(std::vector<ARRAY_QUEUE<Token>>& strVec, std::string* str, wiz::load_data::UserType* ut, Reserver reserver, const wiz::LoadDataOption& option, int* thread_id)
			{
				const int coreNum = strVec.size();
				int next_thread_id = *thread_id;
				int ok = 0;

				while (strVec[next_thread_id].empty()) {
					next_thread_id = (*thread_id + 1) % coreNum;
					*thread_id = next_thread_id;
					ok++;
					if (ok == strVec.size() && strVec[next_thread_id].empty()) {
						//wiz::Out << "errorB" << ENTER;
						break;
					}
				}

				while (strVec[next_thread_id].empty() == false && strVec[next_thread_id].front().str.empty()) {
					strVec[next_thread_id].pop_front();
					next_thread_id = (*thread_id + 1) % coreNum;

					*thread_id = next_thread_id;
				}

				if (strVec[next_thread_id].empty()) {
					return false;
				}

				if (str) {
					Token token;
					token = strVec[next_thread_id].front();
					strVec[next_thread_id].pop_front();
					*str = (std::move(token.str));
					//*str = std::move(strVec.front().str);
				}
				else {
					strVec[next_thread_id].pop_front();
				}
				//strVec.pop_front();

				return true;
			}

			// lookup just one!
			template <class Reserver>
			static std::pair<bool, std::string> LookUp(std::vector<ARRAY_QUEUE<Token>>& strVec, wiz::load_data::UserType* ut, Reserver reserver, const wiz::LoadDataOption& option, int* thread_id)
			{
				const int coreNum = strVec.size();
				int next_thread_id = *thread_id;
				int next_thread_id2 = *thread_id;
				int ok = 0;

				while (strVec[next_thread_id].empty()) {
					next_thread_id = (*thread_id + 1) % coreNum;
					*thread_id = next_thread_id;
					ok++;
					if (ok == strVec.size() && strVec[next_thread_id].empty()) {
						//wiz::Out << "errorC" << ENTER;
						break;
					}
				}

				while (strVec[next_thread_id].empty() == false && strVec[next_thread_id].front().str.empty()) {
					strVec[next_thread_id].pop_front();

					next_thread_id = (*thread_id + 1) % coreNum;
					next_thread_id2 = next_thread_id;

					if (strVec[next_thread_id].size() == 1) {
						next_thread_id2 = (next_thread_id + 1) % coreNum;
					}
					
					*thread_id = next_thread_id2;
				}

				if (strVec[next_thread_id].size() >= 2) {
					return{ true, strVec[next_thread_id][1].str };
				}

				return{ false, "" };
			}

		public:
			//
			static bool ChkExist(const std::string& str) // for \", str is separated by enterkey
			{
				bool start = false;
				int state = 0;

				for (std::string::size_type i = 0; i < str.size(); ++i)
				{
					if (0 == state && i == 0 && '\"' == str[i]) {
						state = 1;
						start = true;
					}
					else if (0 == state && i == 0 && '\'' == str[i]) {
						state = 2;
						start = true;
					}
					else if (0 == state && i > 0 && '\"' == str[i] && '\\' != str[i - 1])
					{
						state = 1;
						start = true;
					}
					else if (0 == state && i > 0 && '\'' == str[i] && '\\' != str[i - 1])
					{
						state = 2;
						start = true;
					}
					else if (1 == state && i > 0 && '\\' != str[i - 1] && '\"' == str[i]) {
						state = 0;
					}
					else if (2 == state && i > 0 && '\\' != str[i - 1] && '\'' == str[i]) {
						state = 0;
					}
					else if (0 == state && str[i] == '#') {
						break;
					}
				}

				return start && 0 == state; // exist and valid !! chk - todo!
			}
			

			// AddSpace : return std::string
			static void AddSpace(const std::string& str, std::string& temp)
			{
				temp = "";

				for (int i = 0; i < str.size(); ++i)
				{
					/// To Do - chabnge to switch statement.
					if ('=' == str[i]) {
						temp.push_back(' ');
						temp.push_back('=');
						temp.push_back(' ');
					}
					else if ('{' == str[i]) {
						temp.push_back(' ');
						temp.push_back('{');
						temp.push_back(' ');
					}
					else if ('}' == str[i]) {
						temp.push_back(' ');
						temp.push_back('}');
						temp.push_back(' ');
					}
					else {
						temp.push_back(str[i]);
					}
				}

				//return temp;
			}

			/// need testing!
			static void PassSharp(const std::string& str, std::string& temp) { // txt file로 들어온다?
				temp = "";
				int state = 0;

				for (int i = 0; i < str.size(); ++i) {
					if (str[i] == '#') { state = 1; continue; }
					else if (str[i] == '\n') { state = 0; continue; }

					if (0 == state) {
						temp.push_back(str[i]);
					}
				}
				//return temp;
			}
			
			static bool _ChangeStr(const std::string& str, const std::vector<std::string>& changed_str, const std::vector<std::string>& result_str, std::string::size_type& i, int& state, std::string& temp) {
				for (std::string::size_type j = 0; j < changed_str.size(); ++j) {
					if (wiz::String::Comp(changed_str[j].c_str(), str.c_str() + i, changed_str[j].size())) {
						state = 1;
						temp.append(result_str[j]);
						i = i + changed_str[j].size() - 1;
						return true;
					}
				}
				return false;
			}
			/*

			// 길이가 긴 문자열이 먼저 나와야 한다?
			static void ChangeStr(const std::string& str, const std::vector<std::string>& changed_str, const std::vector<std::string>& result_str, std::string& temp) {
				temp = "";
				int state = 0;


				for (std::string::size_type i = 0; i < str.size(); ++i)
				{
					if (0 == state && i == 0 && '\"' == str[i]) {
						state = 1;
						temp.push_back(str[i]);
					}
					else if (0 == state && i > 0 && '\"' == str[i] && '\\' != str[i - 1])
					{
						state = 1;
						temp.push_back(str[i]);
					}
					else if (1 == state  && _ChangeStr(str, changed_str, result_str, i, state, temp)) {
						//
					}
					else if ((1 == state && i > 0 && '\\' != str[i - 1] && '\"' == str[i])) {
						state = 0;
						temp.push_back('\"');
					}
					else
					{
						temp.push_back(str[i]);
					}
				}

				//return temp;
			}
			*/
			// no chk "
			static void ChangeStr2(const std::string& str, const std::vector<std::string>& changed_str, const std::vector<std::string>& result_str, std::string& temp) {
				temp = "";
				int state = 1;

				for (std::string::size_type i = 0; i < str.size(); ++i)
				{
					if(_ChangeStr(str, changed_str, result_str, i, state, temp)) {
						//
					}
					else {
						temp.push_back(str[i]);
					}
				}

				//return temp;
			}
		};
	}
}

#endif
