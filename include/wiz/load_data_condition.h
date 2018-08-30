﻿
#ifndef LOAD_DATA_CONDITION_H
#define LOAD_DATA_CONDITION_H

#include <vector>
#include <string>
#include <algorithm>

#include "stacks.h"
#include "load_data_types.h"
#include "load_data_utility.h"

namespace wiz {
	namespace load_data {
		class Condition {
		private:
			int braceNum;
			std::vector<std::string> tokenVec;
			ArrayStack<std::string> tokenStack;
			std::string condition;
			int i;
			UserType* position=nullptr; // need set, get ter!!
			UserType* global=nullptr;
			int option; // removal?
			StringBuilder* builder;
		public:
			explicit Condition(const std::string& condition, UserType* position, UserType* global, StringBuilder* builder, const int option = 0)
				: braceNum(0), condition(condition), i(0), position(position), option(option), global(global), builder(builder)
			{
				Init(condition, builder);
			}
		private:
			auto Get(const std::string& var, const std::string& position, UserType* utPosition, UserType* global) {
				std::string valTemp = position;
				StringTokenizer tokenizer(position, "/", builder, 1);
				UserType* utTemp;

				if (false == tokenizer.hasMoreTokens()) {
					utTemp = utPosition;
				}
				else {
					if (tokenizer.nextToken() != "root") {
						utTemp = utPosition;
					}
					else {
						valTemp = std::string(position.c_str() + 5);
						utTemp = global;
					}
				}
				return UserType::Find(utTemp, valTemp, builder);
			}
			// val -> rename?, parameter:option -> removal?
			std::string GetValue(const std::string& op, const std::string& var, const std::string& val, UserType* utPosition, UserType* global, const std::string& option = "0")
			{
				/// ExistItem, ExistUserType
				if (NULL == utPosition) { return "ERROR"; }

				if ("EXISTITEM" == op) { /// option == 1?	
					auto x = Get(var, val, utPosition, global);
					if (x.first) {
						for (int i = 0; i < x.second.size(); ++i) {
							if (x.second[i]->GetItem(var).size() > 0)
							{
								return "TRUE";
							}
						}
						return "FALSE";
					}
					return "FALSE";
				}
				else if ("EXISTUSERTYPE" == op) { /// option == 1?	
					auto x = Get(var, val, utPosition, global);
					if (x.first) {
						for (int i = 0; i < x.second.size(); ++i) {
							if (x.second[i]->GetUserTypeItem(var).size() > 0)
							{
								return "TRUE";
							}
						}
						return "FALSE";
					}
					return "FALSE";
				}
				else if ("NOTEXISTITEM" == op) { /// option == 2 ?
					auto x = Get(var, val, utPosition, global);
					if (x.first) {
						for (int i = 0; i < x.second.size(); ++i) {
							if (0 < x.second[i]->GetItem(var).size())
							{
								return "FALSE";
							}
						}
						return "TRUE";
					}
					return "FALSE";
				}
				else if ("NOTEXISTUSERTYPE" == op) { /// option == 2 ?
					auto x = Get(var, val, utPosition, global);
					if (x.first) {
						for (int i = 0; i < x.second.size(); ++i) {
							if (0 < x.second[i]->GetUserTypeItem(var).size())
							{
								return "FALSE";
							}
						}
						return "TRUE";
					}
					return "FALSE";
				}

				return "ERROR";
			}
			std::string GetValue(const std::string& op, const std::string& var1, const std::string& position1, const std::string& var2, const std::string& position2,
				UserType* utPosition, UserType* global, StringBuilder* builder, std::string option = "0")
			{
				// COMP<, COMP>, EQ, NOTEQ
				if (NULL == utPosition) { return "ERROR"; }

				int mode = 0; // 0 : defualt, 1 : only position1 is /$digit, 2: only position2 is /$digit 3 : position1 and position2 are /$digit
				std::string position1_temp = position1;
				std::string position2_temp = position2;

				if (wiz::String::startsWith(position1, "/$")) { // cf) position1 == /$0 or /$1 or ...
					position1_temp = "/";
					mode = mode | 1;
				}
				if (wiz::String::startsWith(position2, "/$")) {
					position2_temp = "/";
					mode = mode | 2;
				}

				auto x = Get(var1, position1_temp, utPosition, global);
				auto y = Get(var2, position2_temp, utPosition, global);

				if ("~" != position1 && false == x.first)
				{
					return "ERROR GV1";
				}
				if ("~" != position2 && false == y.first)
				{
					return "ERROR GV2";
				}
				//
				if (((x.first && x.second.size() > 1) || (y.first && y.second.size() > 1))) {
					return "ERROR GV3";
				}

				std::vector<ItemType<WIZ_STRING_TYPE>> value1;  // Item<std::string> <- 
				std::vector<ItemType<WIZ_STRING_TYPE>> value2;
				
				if (position1 != "~") {
					int idx = 0;
					if (mode & 1) {
						idx = stoi(wiz::String::substring(position1, 2));
						value1.push_back(ItemType<WIZ_STRING_TYPE>());
						value1[0].Push(wiz::ToString(x.second[0]->GetItemList(idx).Get(0)));
					}
					else {
						value1 = x.second[0]->GetItem(var1);
					}
				}
				if (position2 != "~") {
					int idx = 0;
					if (mode & 2) {
						idx = stoi(wiz::String::substring(position2, 2));
						value2.push_back(ItemType<WIZ_STRING_TYPE>());
						value2[0].Push(y.second[0]->GetItemList(idx).Get(0));
					}
					else {
						value2 = y.second[0]->GetItem(var2);
					}
				}
				//
				if (position1 == "~" ) {
					value1.push_back(ItemType<WIZ_STRING_TYPE>()); // var1
					value1[0].Push(var1);
				}
				if (position2 == "~") {
					value2.push_back(ItemType<WIZ_STRING_TYPE>()); // var2
					value2[0].Push(var2);
				}

				//
				if (value1.size() == 0) {
					return "ERROR GV4";
				}
				if (value2.size() == 0) {
					return "ERROR GV5";
				}

				if (option == "0" && (value1.size() > 1 || value2.size() > 1)) {
					return "ERROR GV6";
				}

				if ("COMP<" == op) {
					if ("0" != option) { // ToDo.. // 0. just 1-1, // 1. for any case // 2. for all case // .
						for (int i = 0; i < value1.size(); ++i) {
							for (int j = 0; j < value2.size(); ++j) {
								if (Utility::Compare(ToString(value1[i].Get(0)), ToString(value2[j].Get(0)), builder) == "< 0") {
									if ("1" == option) { return "TRUE"; }
								}
								else {
									if ("2" == option) { return "FALSE"; }
								}
							}
						}
						if ("1" == option) { return "FALSE"; }
						else if ("2" == option) {
							return "TRUE";
						}
					}
					else {
						if (Utility::Compare(ToString(value1[0].Get(0)), ToString(value2[0].Get(0)), builder) == "< 0") {
							return "TRUE";
						}
					}
					return "FALSE";
				}
				else if ("COMP<EQ" == op) {
					if ("0" != option) { /// ToDo.. // 0. just 1-1, // 1. for any case // 2. for all case
						for (int i = 0; i < value1.size(); ++i) {
							for (int j = 0; j < value2.size(); ++j) {
								std::string temp = Utility::Compare(ToString(value1[i].Get(0)), ToString(value2[j].Get(0)), builder);
									if (temp == "< 0" || temp == "== 0") {
										if ("1" == option) { return "TRUE"; }
									}
									else {
										if ("2" == option) { return "FALSE"; }
									}
							}
						}
						if ("1" == option) { return "FALSE"; }
						else if ("2" == option) {
							return "TRUE";
						}
					}
					else {
						std::string temp = Utility::Compare(ToString(value1[0].Get(0)), ToString(value2[0].Get(0)), builder);
						if (temp == "< 0" || temp == "== 0") {
							return "TRUE";
						}
					}
					return "FALSE";
				}
				else if ("COMP>" == op) {
					if ("0" != option) { /// ToDo.. // 0. just 1-1, // 1. for any case // 2. for all case
						for (int i = 0; i < value1.size(); ++i) {
							for (int j = 0; j < value2.size(); ++j) {
								if (Utility::Compare(ToString(value1[i].Get(0)), ToString(value2[j].Get(0)), builder) == "> 0") {
									if ("1" == option) { return "TRUE"; }
								}
								else {
									if ("2" == option) { return "FALSE"; }
								}
							}
						}
						if ("1" == option) { return "FALSE"; }
						else if ("2" == option) {
							return "TRUE";
						}
					}
					else {
						if (Utility::Compare(ToString(value1[0].Get(0)), ToString(value2[0].Get(0)), builder) == "> 0") {
							return "TRUE";
						}
					}
					return "FALSE";
				}
				else if ("COMP>EQ" == op) {
					if ("0" != option) { /// ToDo.. // 0. just 1-1, // 1. for any case // 2. for all case
						for (int i = 0; i < value1.size(); ++i) {
							for (int j = 0; j < value2.size(); ++j) {
								std::string temp = Utility::Compare(ToString(value1[i].Get(0)), ToString(value2[j].Get(0)), builder);
								if (temp == "> 0" || temp == "== 0") {
									if ("1" == option) { return "TRUE"; }
								}
								else {
									if ("2" == option) { return "FALSE"; }
								}
							}
						}
						if ("1" == option) { return "FALSE"; }
						else if ("2" == option) {
							return "TRUE";
						}
					}
					else {
						std::string temp = Utility::Compare(ToString(value1[0].Get(0)), ToString(value2[0].Get(0)), builder);
						if (temp == "> 0" || temp == "== 0") {
							return "TRUE";
						}
					}
					return "FALSE";
				}
				else if ("EQ" == op) {
					if ("0" != option) { /// ToDo.. // 0. just 1-1, // 1. for any case // 2. for all case
						for (int i = 0; i < value1.size(); ++i) {
							for (int j = 0; j < value2.size(); ++j) {
								if (Utility::Compare(ToString(value1[i].Get(0)), ToString(value2[j].Get(0)), builder) == "== 0") {
									if ("1" == option) { return "TRUE"; }
								}
								else {
									if ("2" == option) { return "FALSE"; }
								}
							}
						}
						if ("1" == option) { return "FALSE"; }
						else if ("2" == option) {
							return "TRUE";
						}
					}
					else {
						if (Utility::Compare(ToString(value1[0].Get(0)), ToString(value2[0].Get(0)), builder) == "== 0") {
							return "TRUE";
						}
					}
					return "FALSE";
				}
				else if ("NOTEQ" == op) {
					if ("0" != option) { /// ToDo.. // 0. just 1-1, // 1. for any case // 2. for all case
						for (int i = 0; i < value1.size(); ++i) {
							for (int j = 0; j < value2.size(); ++j) {
								if (Utility::Compare(ToString(value1[i].Get(0)), ToString(value2[j].Get(0)), builder) != "== 0") {
									if ("1" == option) { return "TRUE"; }
								}
								else {
									if ("2" == option) { return "FALSE"; }
								}
							}
						}
						if ("1" == option) { return "FALSE"; }
						else if ("2" == option) {
							return "TRUE";
						}
					}
					else {
						if (Utility::Compare(ToString(value1[0].Get(0)), ToString(value2[0].Get(0)), builder) != "== 0") {
							return "TRUE";
						}
					}
					return "FALSE";
				}
				else if ("REGEX" == op) {
					if ("0" != option) { /// ToDo.. // 0. just 1-1, // 1. for any case // 2. for all case
						for (int i = 0; i < value1.size(); ++i) {
							for (int j = 0; j < value2.size(); ++j) {
								std::string temp = ToString(value2[j].Get(0));
								std::regex rex(temp.substr(1, temp.size() - 2));

								if (std::regex_match(ToString(value1[i].Get(0)), rex)) {
									if ("1" == option) { return "TRUE"; }
								}
								else {
									if ("2" == option) { return "FALSE"; }
								}
							}
						}
						if ("1" == option) { return "FALSE"; }
						else if ("2" == option) {
							return "TRUE";
						}
					}
					else {
						std::string temp = ToString(value2[0].Get(0));
						std::regex rex(temp.substr(1, temp.size() - 2));

						if (std::regex_match(ToString(value1[0].Get(0)), rex)) {
							return "TRUE";
						}
					}
					return "FALSE";
				}
				
				return "ERROR GV7";
			}
			std::string GetValue3(const std::string& op, const std::string& val, const std::string& dir, UserType* utPosition, UserType* global)
			{
				if (NULL == utPosition) { return "ERROR"; }

				auto x = Get(val, dir, utPosition, global);

				if (false == x.first || x.second.size() != 1) {
					return "ERROR";
				}

				if ("EXISTITEMBYVALUE" == op)
				{
					for (int i = 0; i < x.second[0]->GetItemListSize(); ++i) {
						if (val == wiz::ToString(x.second[0]->GetItemList(i).Get(0))) {
							return "TRUE";
						}
					}
					return "FALSE";
				}

				return "ERROR";
			}
			void Init(const std::string& conditionm, StringBuilder* builder)
			{
				bool evalue = false;

				std::string str;
				Utility::AddSpace(condition, str);
				
				StringTokenizer tokenizer(str, { " ", "\t", "\n", "\r" }, builder, 1);

				while (tokenizer.hasMoreTokens()) {
					std::string temp = tokenizer.nextToken();

					if (temp == "_") { // 
						temp = "";
					}
					tokenVec.push_back(temp);
				}
			}
		public:
			bool Next() {
				try {
					if (i >= tokenVec.size()) { return false; }
					if (condition.empty()) {
						return false;
					}
					{
						if ("=" != tokenVec[i]
							&& "{" != tokenVec[i]
							&& "}" != tokenVec[i]) {
							tokenStack.push(tokenVec[i]);
						}

						if ("{" == tokenVec[i]) {
							braceNum++;
						}
						else if ("}" == tokenVec[i]) {
							braceNum--;

							if (tokenStack.size() >= 2 && "NOT" == tokenStack[tokenStack.size() - 2]) {
								const std::string op = tokenStack[tokenStack.size() - 2];
								const std::string operand1 = tokenStack[tokenStack.size() - 1];
								

								tokenStack.pop();
								tokenStack.pop();

								tokenStack.push(Utility::BoolOperation(op, operand1, operand1));
							}
							else if (tokenStack.size() >= 3 && "EXISTITEMBYVALUE" == tokenStack[tokenStack.size() - 3])
							{
								const std::string op = tokenStack[tokenStack.size() - 3];
								const std::string val = tokenStack[tokenStack.size() - 2];
								const std::string dir = tokenStack[tokenStack.size() - 1];

								tokenStack.pop();
								tokenStack.pop();
								tokenStack.pop();
								tokenStack.push(GetValue3(op, val, dir, position, global));
							}
							else if (tokenStack.size() >= 3 && 
								("AND" == tokenStack[tokenStack.size() - 3] ||
								"OR" == tokenStack[tokenStack.size() - 3] 
									//// todo EXISTITEMBYITEM, ...
								) 
							) {
								std::string op = tokenStack[tokenStack.size() - 3];
								const std::string operand1 = tokenStack[tokenStack.size() - 2];
								const std::string operand2 = tokenStack[tokenStack.size() - 1];

								if (op == "AND" || op == "OR") {
									tokenStack.pop();
									tokenStack.pop();
									tokenStack.pop();

									tokenStack.push(Utility::BoolOperation(op, operand1, operand2));
								}
								else { // EXIST, NOTEXIST for ( ITEM or USERTYPE )
									tokenStack.pop();
									tokenStack.pop();
									tokenStack.pop();

									tokenStack.push(GetValue(op, operand1, operand2, position, global));
								}
							}
							// COMP<, COMP<EQ, COMP>, COMP>EQ, EQ, NOTEQ
							else if (tokenStack.size() >= 6 && ("COMP<" == tokenStack[tokenStack.size() - 6]
								|| "COMP<EQ" == tokenStack[tokenStack.size() - 6]
								|| "COMP>" == tokenStack[tokenStack.size() - 6]
								|| "COMP>EQ" == tokenStack[tokenStack.size() - 6]
								|| "EQ" == tokenStack[tokenStack.size() - 6]
								|| "NOTEQ" == tokenStack[tokenStack.size() - 6]
								|| "REGEX" == tokenStack[tokenStack.size() - 6]
								))
							{
								const std::string op = tokenStack[tokenStack.size() - 6];
								const std::string var1 = tokenStack[tokenStack.size() - 5];
								const std::string position1 = tokenStack[tokenStack.size() - 4];
								const std::string var2 = tokenStack[tokenStack.size() - 3];
								const std::string position2 = tokenStack[tokenStack.size() - 2];
								const std::string option = tokenStack[tokenStack.size() - 1];
								tokenStack.pop();
								tokenStack.pop();
								tokenStack.pop();
								tokenStack.pop();
								tokenStack.pop();
								tokenStack.pop();

								tokenStack.push(GetValue(op, var1, position1, var2, position2, position, global, builder, option));
							}
						}
						++i;
					}
				}
				catch (const char* err) { std::cout << err << std::endl; return false; }
				catch (const std::string& err) { std::cout << err << std::endl; return false; }
				catch (Error& e) { std::cout << e << std::endl; return false; }
				return true;
			}
		public:
			int NowIndex()const { return i; }
			const ArrayStack<std::string>& Now()const { return tokenStack; }
			ArrayStack<std::string>& Now() { return tokenStack; }
			bool IsNowImportant() const { // ??
				return "}" == tokenVec[i] && "NOT" != tokenStack[tokenStack.size() - 2];
			}
		};
	}
}

#endif
