#define _CRT_SECURE_NO_WARNINGS
#define ARRAY_QUEUE std::deque // chk?
#define VECTOR std::vector

#include <iostream>
#include <cmath>

#ifdef _MSC_VER 
#include <conio.h>
#endif

#include "wiz/STRINGBUILDER.H"
#include "wiz/load_data.h"

namespace wiz {
	namespace load_data{
		bool LoadData::operation(wiz::load_data::UserType* now, wiz::load_data::UserType& global, const WIZ_STRING_TYPE& str,
			wiz::ArrayStack<WIZ_STRING_TYPE>& operandStack, const ExcuteData& excuteData, wiz::StringBuilder* builder)
		{
			if (!operandStack.empty() && operandStack.top() == "ERROR") {
				return false;
			}

			int operandNum = 0;
			int count = 0; // for brace!
			{
				for (int i = operandStack.size() - 1; i >= 0; --i) {
					if ("{" == operandStack[i]) {
						count++;
					}
					if ("}" == operandStack[i]) {
						count--;
						if (count < 0) {
							break;
						}
						else {
							operandNum++;
						}
					}
					else {
						operandNum++;
					}
				}
			}

			for (int i = 0; i < operandNum; ++i) {
				if ("_" == operandStack[operandStack.size() - 1 - i]) {
					operandStack[operandStack.size() - 1 - i] = "";
				}
			}

			if ("$EQ" == str) {
				WIZ_STRING_TYPE x, y;
				int idx = -1;
				x = operandStack.pop();
				y = operandStack.pop();

				if (wiz::load_data::Utility::Compare(x.ToString(), y.ToString(), builder) == "== 0") {
					operandStack.push("TRUE");
				}
				else {
					operandStack.push("FALSE");
				}
			}
			else if ("$NOTEQ" == str)
			{
				WIZ_STRING_TYPE x, y;
				x = operandStack.pop();
				y = operandStack.pop();

				if (wiz::load_data::Utility::Compare(x.ToString(), y.ToString(), builder) != "== 0") {
					operandStack.push("TRUE");
				}
				else {
					operandStack.push("FALSE");
				}
			}
			else if ("$AND" == str)
			{
				WIZ_STRING_TYPE x, y;
				x = operandStack.pop();
				y = operandStack.pop();

				if (x == "TRUE" && y == "TRUE") {
					operandStack.push("TRUE");
				}
				else {
					operandStack.push("FALSE");
				}
			}
			else if ("$AND_ALL" == str) {
				std::vector<WIZ_STRING_TYPE> store(operandNum);
				for (int i = 0; i < operandNum; ++i) {
					store[i] = operandStack.pop();
				}
				for (int i = 0; i < store.size(); ++i) {
					if ("TRUE" != store[i]) {
						operandStack.push("FALSE");
						return true;
					}
				}
				operandStack.push("TRUE");
			}
			else if ("$OR" == str)
			{
				WIZ_STRING_TYPE x, y;
				x = operandStack.pop();
				y = operandStack.pop();

				if (x == "TRUE" || y == "TRUE") {
					operandStack.push("TRUE");
				}
				else {
					operandStack.push("FALSE");
				}
			}
			else if ("$OR_ALL" == str)
			{
				std::vector<WIZ_STRING_TYPE> store;

				for (int i = 0; i < operandNum; ++i) {
					store.push_back(operandStack.pop());
				}
				for (int i = 0; i < store.size(); ++i) {
					if ("TRUE" == store[i]) {
						operandStack.push("TRUE");
						return true;
					}
				}
				operandStack.push("FALSE");
			}
			else if ("$NOT" == str)
			{
				WIZ_STRING_TYPE x;
				x = operandStack.pop();

				if (x == "TRUE") {
					operandStack.push("FALSE");
				}
				else {
					operandStack.push("TRUE");
				}
			}
			else if ("$COMP<" == str)
			{
				WIZ_STRING_TYPE x, y;
				x = operandStack.pop();
				y = operandStack.pop();

				if (wiz::load_data::Utility::Compare(x.ToString(), y.ToString(), builder) == "< 0") {
					operandStack.push("TRUE");
				}
				else
				{
					operandStack.push("FALSE");
				}
			}
			else if ("$COMP>" == str)
			{
				WIZ_STRING_TYPE x, y;
				x = operandStack.pop();
				y = operandStack.pop();

				if (wiz::load_data::Utility::Compare(x.ToString(), y.ToString(), builder) == "> 0") {
					operandStack.push("TRUE");
				}
				else
				{
					operandStack.push("FALSE");
				}
			}
			else if ("$COMP<EQ" == str)
			{
				std::string x, y;
				x = operandStack.pop().ToString();
				y = operandStack.pop().ToString();

				if (wiz::load_data::Utility::Compare(x, y, builder) == "< 0" || wiz::load_data::Utility::Compare(x, y, builder) == "== 0") {
					operandStack.push("TRUE");
				}
				else
				{
					operandStack.push("FALSE");
				}
			}
			else if ("$COMP>EQ" == str)
			{
				std::string x, y;
				x = operandStack.pop().ToString();
				y = operandStack.pop().ToString();

				if (wiz::load_data::Utility::Compare(x, y, builder) == "> 0" || wiz::load_data::Utility::Compare(x, y, builder) == "== 0") {
					operandStack.push("TRUE");
				}
				else
				{
					operandStack.push("FALSE");
				}
			}
			else if ("$add" == str) // todo! = int operator double => double operator double!
			{
				auto x = operandStack.pop();
				auto y = operandStack.pop();

				if (x.GetType() == 5 && y.GetType() == 5) {
					DataType temp;
					temp.SetFloat(x.ToFloat() + y.ToFloat());
					operandStack.push(temp);
				}
				else if (x.GetType() == 3 && y.GetType() == 3) {
					DataType temp;
					temp.SetInt(x.ToInt() + y.ToInt());
					operandStack.push(temp);
				}
				else {

					operandStack.push("ERROR");
				}
			}
			else if ("$multiple" == str) // todo! = int operator double => double operator double!
			{
				std::string _x, _y;
				auto x = operandStack.pop();
				auto y = operandStack.pop();

				_x = x.ToString();
				_y = y.ToString();

				if (x.GetType() == 5 && y.GetType() == 5) {
					DataType temp;
					temp.SetFloat(x.ToFloat() * y.ToFloat());
					operandStack.push(temp);
				}
				else if (x.GetType() == 3 && y.GetType() == 3) {
					DataType temp;
					temp.SetInt(x.ToInt() * y.ToInt());
					operandStack.push(temp);
				}
				else
				{

					operandStack.push("ERROR");
				}
			}
			else if ("$divide" == str) // todo! = int operator double => double operator double!
			{
				std::string _x, _y;
				auto x = operandStack.pop();
				auto y = operandStack.pop();

				_x = x.ToString();
				_y = y.ToString();

				if (x.GetType() == 5 && y.GetType() == 5) {
					DataType temp;
					temp.SetFloat(x.ToFloat() / y.ToFloat());
					operandStack.push(temp);
				}
				else if (x.GetType() == 3 && y.GetType() == 3) {
					DataType temp;
					temp.SetInt(x.ToInt() / y.ToInt());
					operandStack.push(temp);
				}
				else
				{

					operandStack.push("ERROR");
				}
			
			}
			else if ("$modular" == str)
			{
				WIZ_STRING_TYPE x, y;
				x = operandStack.pop();
				y = operandStack.pop();
				std::string _x = x.ToString();
				std::string _y = y.ToString();

				if (x.GetType() == 3 && y.GetType() == 3) {
					DataType temp;
					temp.SetInt(x.ToInt() % y.ToInt());
					operandStack.push(temp);
				}
				else
				{
					operandStack.push("ERROR");
				}
			}
			else if ("$rand" == str)
			{
				std::string x, y;
				x = operandStack.pop().ToString();
				y = operandStack.pop().ToString();

				if (wiz::load_data::Utility::GetType(x) == wiz::load_data::Utility::GetType(y) && (wiz::load_data::Utility::GetType(y) == "INTEGER")) { /// only integer -> BigInteger
					int _x = atoi(x.c_str());
					int _y = atoi(y.c_str());
					if (_y - _x + 1 <= 0) {
						operandStack.push("ERROR x / 0");

					}
					else {
						int _z = wiz::Rand2<int>() % (_y - _x + 1) + _x; // _x <= _z <= _y
						operandStack.push(wiz::toStr(_z));
					}
				}
				else
				{
					operandStack.push("ERROR");
				}
			}
			else if ("$concat" == str)
			{
				if (builder) {
					builder->Clear();

					int total_size = 0;
					for (int i = 0; i < 2; ++i)
					{
						total_size += operandStack.top().ToString().size();
						std::string temp = operandStack.pop().ToString();
						builder->Append(temp.c_str(), temp.size()); // chk
					}

					operandStack.push(std::string(builder->Str(), total_size));
				}
				else {
					std::string x, y;
					x = operandStack.pop().ToString();
					y = operandStack.pop().ToString();

					operandStack.push(x + y);
				}
			}
			else if ("$concat2" == str) /// with space
			{
				std::string x, y;
				x = operandStack.pop().ToString();
				y = operandStack.pop().ToString();

				operandStack.push(x + " " + y);
			}
			else if ("$concat_all" == str)
			{
				//cout << "----" << endl;
				if (builder) {
					builder->Clear();

					int total_size = 0;
					for (int i = 0; i < operandNum; ++i)
					{
						total_size += operandStack.top().ToString().size();
						std::string temp = operandStack.pop().ToString();
						builder->Append(temp.c_str(), temp.size()); // chk
					}

					operandStack.push(std::string(builder->Str(), total_size));
				}
				else {
					std::string result;

					for (int i = 0; i < operandNum; ++i) {
						result = result + operandStack.pop().ToString();
						//	cout << "test" <<  result << endl;
					}
					//cout << "----" << endl;
					operandStack.push(result);
				}
			}
			else if ("$concat_all2" == str)
			{
				WIZ_STRING_TYPE result;

				for (int i = 0; i < operandNum; ++i) {
					result += operandStack.pop();
					if (i < operandNum - 1) {
						result += " ";
					}
				}
				operandStack.push(result);
			}
			else if ("$concat3" == str) { // for special case? "abc" "def" "ghi" -> "abc def ghi"
				WIZ_STRING_TYPE result;

				for (int i = 0; i < operandNum; ++i) {
					std::string temp = operandStack.pop().ToString();
					if (temp.size() >= 3 && temp.back() == temp.front() && temp.back() == '\"') {}
					else {
						operandStack.push("ERROR in $concat3, 1. must be \" \" ");
						return false;
					}
					if (i < operandNum - 1) {
						temp.erase(temp.begin() + temp.size() - 1);

						if (0 != i) {
							temp.erase(temp.begin());
						}

						result += temp;
						result += " ";
					}
					else {
						temp.erase(temp.begin());

						result += temp;
					}
				}

				operandStack.push(result);
			}
			else if ("$return_value" == str)
			{
				operandStack.push(excuteData.info.return_value);
			}
			// cf) empty test!!
			///ToDo - GetList -> // GetItemListIdxByIListIdx, GetUserTypeLisIdxtByIListIdx ?
			else if ("$back" == str) // ex) for x  = { 0 1 2 3 .. }, for usertaypelist? and mixed? and need more test!
			{
				WIZ_STRING_TYPE x = operandStack.pop();

				std::string value = wiz::load_data::LoadData::GetItemListData(global, x.ToString(), "TRUE", excuteData, builder);
				wiz::load_data::UserType ut;
				wiz::load_data::LoadData::LoadDataFromString(value, ut);

				if (ut.IsItemList(ut.GetIListSize() - 1))

				{
					auto x = ut.GetItemList(ut.GetItemListSize() - 1);
					operandStack.push(ToString(x.Get(0)));
				}
				else {
					auto x = ut.GetUserTypeList(ut.GetUserTypeListSize() - 1);
					operandStack.push("\"" + x->ToString() + "\"");
				}
			}
			// pop_back or front - remove this function?
			else if ("$pop_back" == str) // and for usertypelist? and mixed?, usertype-> "~"
			{
				std::string x = operandStack.pop().ToString();
				WIZ_STRING_TYPE name;
				for (int i = x.size() - 1; i >= 0; --i)
				{
					if (x[i] == '/' && i != 0) {
						name = wiz::String::substring(x, i + 1);
						x = wiz::String::substring(x, 0, i - 1);
						break;
					}
				}

				if (x.empty()) { x = "."; }

				wiz::load_data::UserType* ut = nullptr;
				auto finded = wiz::load_data::UserType::Find(&global, x, builder);
				ut = finded.second[0];

				if (ut->IsItemList(ut->GetIListSize() - 1))
				{
					auto x = ut->GetItemList(ut->GetItemListSize() - 1);
					operandStack.push(ToString(x.Get(0)));
					ut->RemoveItemList(ut->GetItemListSize() - 1);
				}
				else {
					auto x = ut->GetUserTypeList(ut->GetUserTypeListSize() - 1);
					operandStack.push("\"" + x->ToString() + "\"");
					ut->RemoveUserTypeList(ut->GetUserTypeListSize() - 1);
				}
			}
			// todo - $front, pop-front.
			else if ("$front" == str)
			{
				std::string x = operandStack.pop().ToString();

				std::string value = wiz::load_data::LoadData::GetItemListData(global, x, "TRUE", excuteData, builder);
				wiz::load_data::UserType ut;
				wiz::load_data::LoadData::LoadDataFromString(value, ut);

				if (ut.IsItemList(0))
				{
					auto x = ut.GetItemList(0);
					operandStack.push(ToString(x.Get(0)));
				}
				else {
					auto x = ut.GetUserTypeList(0);
					operandStack.push("\"" + x->ToString() + "\"");
				}
			}
			else if ("$pop_front" == str)
			{
				std::string x = operandStack.pop().ToString();
				std::string name;
				for (int i = x.size() - 1; i >= 0; --i)
				{
					if (x[i] == '/' && i != 0) {
						name = wiz::String::substring(x, i + 1);
						x = wiz::String::substring(x, 0, i - 1);
						break;
					}
				}

				if (x.empty()) { x = "."; }

				wiz::load_data::UserType* ut = nullptr;
				auto finded = wiz::load_data::UserType::Find(&global, x, builder);
				ut = finded.second[0];

				if (ut->IsItemList(0))
				{
					auto x = ut->GetItemList(0);
					operandStack.push(ToString(x.Get(0)));
					ut->RemoveItemList(0);
				}
				else {
					auto x = ut->GetUserTypeList(0);
					operandStack.push("\"" + x->ToString() + "\"");
					ut->RemoveUserTypeList(0);
				}
			}
			else if ("$get" == str)
			{
				std::string x = operandStack.pop().ToString();

				if ('@' == x[0]) { // chk..
					x.erase(x.begin());
				}

				if (wiz::String::startsWith(x, "now.") && nullptr != now)
				{
					x = wiz::String::substring(x, 4);
					x = "/./" + x;
					if ('/' == x[0])
					{
						std::string temp = Find(now, x, builder); if (!temp.empty()) { x = temp; operandStack.push(x); return true; }
					}
				}
				else {
					if ('/' == x[0])
					{
						std::string temp = Find(&global, x, builder); if (!temp.empty()) { x = temp; operandStack.push(x); return true; }
					}
				}

				{
					std::string temp = FindParameters(excuteData.info.parameters, x);
					if (!temp.empty()) { x = temp; operandStack.push(x); return true; }
				}
				{
					std::string temp = FindLocals(excuteData.info.locals, x);
					if (!temp.empty()) { x = temp; operandStack.push(x); return true; }
				}

				operandStack.push(x);
				return true;
			}
			else if ("$size" == str)
			{
				std::string x = operandStack.pop().ToString();

				if ('/' == x[0])
				{
					wiz::load_data::UserType* ut = wiz::load_data::UserType::Find(&global, x, builder).second[0];
					x = wiz::toStr(ut->GetItemListSize());
				}
				else
				{
					x = "FALSE";
				}

				operandStack.push(x);
			}
			else if ("$size2" == str)
			{
				std::string x = operandStack.pop().ToString();

				if ('/' == x[0])
				{
					wiz::load_data::UserType* ut = wiz::load_data::UserType::Find(&global, x, builder).second[0];
					x = wiz::toStr(ut->GetUserTypeListSize());
				}
				else
				{
					x = "FALSE";
				}

				operandStack.push(x);
			}
			else if ("$element" == str) // for list
			{
				std::string x = operandStack.pop().ToString(); // list_name
				WIZ_STRING_TYPE y = operandStack.pop(); // idx
				int idx = stoi(y.ToString());

				if ('/' == x[0])
				{
					wiz::load_data::UserType* ut = wiz::load_data::UserType::Find(&global, x, builder).second[0];
					x = ToString(ut->GetItemList(idx).Get(0));
				}
				else
				{
					x = ""; // NONE?
				}

				operandStack.push(x);
			}
			// it_name
			else if ("$name" == str) {
				std::string x = operandStack.pop().ToString(); // list_name
				WIZ_STRING_TYPE y = operandStack.pop(); // idx
				int idx = stoi(y.ToString());

				if ('/' == x[0])
				{
					wiz::load_data::UserType* ut = wiz::load_data::UserType::Find(&global, x, builder).second[0];
					x = ToString(ut->GetItemList(idx).GetName());
				}
				else
				{
					x = ""; // NONE?
				}

				operandStack.push(x);
			}
			// ut_name
			else if ("$name2" == str) {
				std::string x = operandStack.pop().ToString(); // list_name
				WIZ_STRING_TYPE y = operandStack.pop(); // idx
				int idx = stoi(y.ToString());

				if ('/' == x[0])
				{
					wiz::load_data::UserType* ut = wiz::load_data::UserType::Find(&global, x, builder).second[0];
					x = ToString(ut->GetUserTypeList(idx)->GetName());
				}
				else
				{
					x = ""; // "NONE";
				}

				operandStack.push(x);
			}
			else if ("$regex" == str) {
				std::string str = operandStack.pop().ToString();
				std::string rgx_str = operandStack.pop().ToString();

				// " ~ " , "제거?
				if (rgx_str.size() > 2 && rgx_str[0] == rgx_str.back() && rgx_str[0] == '\"') {
					std::regex rgx(rgx_str.substr(1, rgx_str.size() - 2));

					if (std::regex_match(str, rgx))
					{
						operandStack.push("TRUE");
					}
					else {
						operandStack.push("FALSE");
					}
				}
				else {
					operandStack.push("ERROR in $regex");
				}
			}
			else if ("$eval" == str) { //// now, no need ?
				std::string str = operandStack.pop().ToString();

				bool chk = wiz::load_data::Utility::ChkExist(str);
				if (chk) {
				}
				else {
					operandStack.push("ERROR in $eval, must be \" \" ");
					return false;
				}
				str = str.substr(1, str.size() - 2);
				{
					WIZ_STRING_TYPE result = ToBool4(now, global, str, excuteData, builder);

					operandStack.push(std::move(result));
				}
			}
			// big
			else if ("$is_quoted_str" == str)
			{
				std::string str = operandStack.pop().ToString();
				if (str.size() >= 2 && str[0] == str.back() && '\"' == str[0])
				{
					operandStack.push("TRUE");
				}
				else {
					operandStack.push("FALSE");
				}
			}
			// small
			else if ("$is_quoted_str2" == str)
			{
				std::string str = operandStack.pop().ToString();
				if (str.size() >= 2 && str[0] == str.back() && '\'' == str[0])
				{
					operandStack.push("TRUE");
				}
				else {
					operandStack.push("FALSE");
				}
			}
			else if ("$to_quoted_str" == str) {
				std::string str = operandStack.pop().ToString();
				str.push_back('\"');
				str.insert(str.begin(), '\"');
				operandStack.push(str);
			}
			else if ("$to_quoted_str2" == str) {
				std::string str = operandStack.pop().ToString();
				str.push_back('\'');
				str.insert(str.begin(), '\'');
				operandStack.push(str);
			}
			else if ("$add_small_quoted" == str) {
				std::string str = operandStack.pop().ToString();
				str.push_back('\'');
				str.insert(str.begin(), '\'');
				operandStack.push(str);
			}
			else if ("$remove_quoted" == str) { // chk "" WIZ_STRING_TYPE problem?
				std::string str = operandStack.pop().ToString();

				if (str.size() > 0 && str.front() == str.back()
					&& '\"' == str.back()
					)
				{
					str = wiz::String::substring(str, 1, str.size() - 2);
				}

				operandStack.push(str);
			}
			else if ("$remove_quoted2" == str) { // chk "" WIZ_STRING_TYPE problem?
				std::string str = operandStack.pop().ToString();

				if (str.size() > 0 && str.front() == str.back()
					&& '\'' == str.back()
					)
				{
					str = wiz::String::substring(str, 1, str.size() - 2);
				}

				operandStack.push(str);
			}
			else if ("$remove_small_quoted" == str) { // chk "" WIZ_STRING_TYPE problem?
				std::string str = operandStack.pop().ToString();

				if (str.size() > 0 && str.front() == str.back()
					&& '\'' == str.back()
					)
				{
					str = wiz::String::substring(str, 1, str.size() - 2);
				}

				operandStack.push(str);
			}
			else if ("$get_object_str" == str) {
				std::string object_name = operandStack.pop().ToString();
				object_name = wiz::String::substring(object_name, 1, object_name.size() - 2);
				std::string event_id = operandStack.pop().ToString();

				wiz::load_data::UserType ut = (*excuteData.pObjectMap)[object_name];

				bool pass = false;
				for (int i = 0; i < ut.GetUserTypeListSize(); ++i) {
					for (int j = 0; j < ut.GetUserTypeList(i)->GetItemListSize(); ++j) {
						if ("id" == ut.GetUserTypeList(i)->GetItemList(j).GetName()
							&& event_id == ToString(ut.GetUserTypeList(i)->GetItemList(j).Get(0))
							) {
							operandStack.push("Event = { " + ut.GetUserTypeList(i)->ToString() + " } ");
							return true;
						}
					}
				}
				operandStack.push("ERROR in $getOjbectStr");
			}
			else if ("$add_paren" == str) { // removal?
				std::string temp = operandStack.pop().ToString();
				operandStack.push(" { " + temp + " } ");
			}
			/*
			else if ("$test" == str) { // for lambda test.
			WIZ_STRING_TYPE temp = operandStack.pop();

			temp = wiz::String::replace(temp, "\"", "");

			temp = "\"" + temp + "\"";

			operandStack.push(temp);
			}
			else if ("$test2" == str) { // for lambda test?
			WIZ_STRING_TYPE temp = operandStack.pop();

			temp = wiz::String::substring(temp, 1, temp.size() - 2);

			operandStack.push(temp);
			}
			else if ("$test3" == str) { // for lambda test?
			WIZ_STRING_TYPE temp = operandStack.pop();

			operandStack.push(temp);
			}
			else if ("$test4" == str) {
			WIZ_STRING_TYPE temp = operandStack.pop();

			if (temp.size() >= 3 && temp.back() == temp.front() && temp.back() == '\"') {
			temp = wiz::String::substring(temp, 1, temp.size() - 2);
			}

			operandStack.push(temp);
			}
			else if ("$test5" == str) { // chk!!
			WIZ_STRING_TYPE temp = operandStack.pop();
			int braceNum = 0;
			bool first = true;

			int start = String::find(temp, "$return", 0) + 7;
			for (int i = start; i < temp.size(); ++i) {
			if ('{' == temp[i]) {
			if (first) {
			temp.insert(temp.begin() + i + 1, '\"');
			}
			else {
			braceNum++;
			}
			first = false;
			}
			else if ('}' == temp[i]) {
			braceNum--;
			if (!first && 0 == braceNum) {
			temp.insert(temp.begin() + i + 1, '\"');
			}
			}
			//cout << temp << endl;
			}

			operandStack.push(temp);
			}
			else if ("$test6" == str) { // for lambda test.
			std::vector<WIZ_STRING_TYPE> vec;
			for (int i = 0; i < operandNum; ++i) {
			vec.push_back(operandStack.pop());
			}
			for (int i = vec.size() - 1; i >= 0; --i) {
			vec[i] = wiz::String::replace(vec[i], "\"", "");
			operandStack.push(vec[i]);
			}
			}
			*/
			else if ("$get_item_name" == str) {
				wiz::load_data::UserType ut;
				std::string statement;

				for (int i = 0; i < operandNum; ++i) {
					statement = statement + operandStack.pop().ToString();
				}
				wiz::load_data::LoadData::LoadDataFromString(statement, ut);

				operandStack.push(wiz::ToString(ut.GetItemList(0).GetName()));
			}
			else if ("$get_item_value" == str) { // why?
				wiz::load_data::UserType ut;
				std::string statement;
				int idx = 0;

				for (int i = 0; i < operandNum - 1; ++i) {
					statement = statement + operandStack.pop().ToString();
				}
				idx = stoi(operandStack.pop().ToString());

				wiz::load_data::LoadData::LoadDataFromString(statement, ut);

				operandStack.push(ToString(ut.GetItemList(idx).Get(0)));
			}
			else if ("$get_item_size" == str) {
				wiz::load_data::UserType ut;
				std::string statement;

				for (int i = 0; i < operandNum; ++i) {
					statement = statement + operandStack.pop().ToString();
				}
				wiz::load_data::LoadData::LoadDataFromString(statement, ut);

				operandStack.push(wiz::_toString(ut.GetItem(wiz::ToString(ut.GetItemList(0).GetName())).size()));
			}
			else if ("$is_empty_string" == str) {
				operandStack.push(operandStack.pop().ToString().empty() ? "TRUE" : "FALSE");
			}
			else if ("$event_result" == str) {
				std::vector<std::string> eventVec;
				for (int i = 0; i < operandNum; ++i) {
					eventVec.push_back(operandStack.pop().ToString());
				}

				std::string statements2 = "Event = { id = NONE" + wiz::toStr(excuteData.depth + 1) + " $call = { ";
				for (int i = 0; i < eventVec.size(); ++i) {
					statements2 = statements2 + eventVec[i] + " ";
				}
				statements2 = statements2 + " } }";
				wiz::load_data::UserType* eventsTemp = excuteData.pEvents;
				wiz::load_data::LoadData::AddData(*eventsTemp, "/root", statements2, "TRUE", ExcuteData(), builder);
				//cout << " chk " << statements2 << endl;
				ExcuteData _excuteData;
				_excuteData.pModule = excuteData.pModule;
				_excuteData.pObjectMap = excuteData.pObjectMap;
				_excuteData.pEvents = eventsTemp;
				_excuteData.depth = excuteData.depth + 1;
				_excuteData.noUseInput = excuteData.noUseInput;
				_excuteData.noUseOutput = excuteData.noUseOutput;


				Option opt;
				operandStack.push(pExcuteModule->excute_module("Main = { $call = { id = NONE" + wiz::toStr(_excuteData.depth) + " } }", &global, _excuteData, opt, 0));

				{
					for (int idx = 0; idx < eventsTemp->GetUserTypeListSize(); ++idx) {
						if (ToString(eventsTemp->GetUserTypeList(idx)->GetItem("id")[0].Get(0)) == "NONE" + wiz::toStr(_excuteData.depth)) {
							eventsTemp->RemoveUserTypeList(idx);
							break;
						}
					}
				}
			}
			else if ("$get_item_value2" == str) {
				const int i = stoi(operandStack.pop().ToString());

				if (now) {
					operandStack.push(ToString(now->GetItemList(i).Get(0))); //chk
					return true;
				}
				else {
					operandStack.push("ERROR");
					return false;
				}
			}
			else if ("$space" == str) {
				operandStack.push(" ");
				return true;
			}
			else if ("$empty" == str) {
				operandStack.push("");
				return true;
			}
			else if ("$move_up" == str) {
			std::string dir;

				for (int i = 0; i < operandNum; ++i) {
					std::string temp = operandStack.pop().ToString();
					dir = dir + temp;
					//	cout << "temp is " << temp << endl;
				}

				//cout << " dir is  " << dir << endl;
				if (String::startsWith(dir, "/."))
				{
					dir = String::substring(dir, 3);
				}

				StringTokenizer tokenizer(dir, "/", builder, 1);
				std::vector<std::string> tokenVec;
				while (tokenizer.hasMoreTokens()) {
					tokenVec.push_back(tokenizer.nextToken());
				}
				dir = "/./";
				if (tokenVec.empty()) { operandStack.push(dir); return true; }
				for (int i = 0; i < tokenVec.size() - 1; ++i) {
					dir = dir + tokenVec[i] + "/";
				}
				operandStack.push(dir);
				return true;
			}
			
			else if ("$lambda" == str) {
				std::vector<std::string> store;
				for (int i = 0; i < operandNum; ++i) {
					store.push_back(operandStack.pop().ToString());
					if (store[i][0] != store[i][store[i].size() - 1] || '\'' != store[i][0])
					{
						store[i] = "\'" + store[i] + "\'";
					}
				}
				wiz::load_data::UserType ut;
				wiz::load_data::LoadData::LoadDataFromString(store[0].substr(1, store[0].size() - 2), ut);


				if (operandNum >= 1) {
					std::vector<wiz::load_data::UserType*> param = ut.GetUserTypeItem("Event")[0]->GetUserTypeItem("$parameter");
					// in order.
					std::string mainStr = "Main = { $call = { id = NONE } } ";
					std::string eventStr = "Event = { id = NONE $call = { id = " +
						ut.GetUserTypeItem("Event")[0]->GetItem("id")[0].Get(0).ToString() + " ";

					// if opeandNum != ut[0].GetUserType("$parameter").size() then push error?
					for (int i = 0; i < param[0]->GetItemListSize(); ++i) {
						eventStr += param[0]->GetItemList(i).Get(0).ToString() + " = \'" + store[i + 1].substr(1, store[i + 1].size() - 2) + "\' ";
					}

					eventStr += " }  $return = { $return_value = { } } } ";

					eventStr += store[0].substr(1, store[0].size() - 2);

					ut.Remove();
					wiz::load_data::LoadData::LoadDataFromString(eventStr, ut);

					std::string result;
					{
						ExcuteData _excuteData;
						_excuteData.noUseInput = excuteData.noUseInput;
						_excuteData.noUseOutput = excuteData.noUseOutput;

						Option opt;
						result = pExcuteModule->excute_module(mainStr, &ut, _excuteData, opt, 0);
					}
					{
						wiz::load_data::UserType ut;
						wiz::load_data::LoadData::LoadDataFromString(result.substr(1, result.size() - 2), ut);

						if (0 == ut.GetUserTypeListSize()) {
							std::string param = ut.ToString();
							if (wiz::String::startsWith(param, "$parameter.")) { //
								param = param.substr(11);
								result = std::string("\'Event = { id = identity ") + "$parameter = { " + param + " } $return = { $parameter." + param + " } }\'";
							}
						}
					}

					operandStack.push(result);
				}

				return true;
			}
			
			else if ("$to_float_from_integer" == str) { // integer, floating point number -> floating point number(long double)
				std::string value = operandStack.pop().ToString();
				if (wiz::load_data::Utility::IsInteger(value)) {
					long double x = stoll(value);
					operandStack.push(wiz::_toString(x));
				}
				else {
					operandStack.push("it is not integer");
				}
			}
			else if ("$to_integer_from_float" == str) { // integer, floating point number -> floating point number(long double)
				std::string value = operandStack.pop().ToString();
				if (wiz::load_data::Utility::IsDouble(value)) {
					long long x = std::stold(value);
					operandStack.push(wiz::_toString(x));
				}
				else {
					operandStack.push("it is not floating number");
				}
			}
			//floor, ceiling, round,
			else if ("$floor" == str) {
				std::string value = operandStack.pop().ToString();
				if (wiz::load_data::Utility::IsDouble) {
					long double x = std::floor(std::stold(value));
					operandStack.push(wiz::_toString(x));
				}
				else {
					operandStack.push("only double can");
				}
			}
			else if ("$ceiling" == str) {
				std::string value = operandStack.pop().ToString();
				if (wiz::load_data::Utility::IsDouble) {
					long double x = std::ceil(std::stold(value));
					operandStack.push(wiz::_toString(x));
				}
				else {
					operandStack.push("only double can");
				}
			}
			else if ("$round" == str) {
				std::string value = operandStack.pop().ToString();
				if (wiz::load_data::Utility::IsDouble) {
					long double x = std::round(std::stold(value));
					operandStack.push(wiz::_toString(x));
				}
				else {
					operandStack.push("only double can");
				}
			}
			//contains,
			else if ("$contains" == str) {
				std::string str = operandStack.pop().ToString();
				std::string chk_str = operandStack.pop().ToString();

				operandStack.push(std::string::npos != str.find(chk_str) ? "TRUE" : "FALSE");
			}
			//starts_with, ends_with,
			else if ("$starts_with" == str) {
				std::string str = operandStack.pop().ToString();
				std::string chk_str = operandStack.pop().ToString();

				operandStack.push(wiz::String::startsWith(str, chk_str) ? "TRUE" : "FALSE");
			}
			else if ("$ends_with" == str) {
				std::string str = operandStack.pop().ToString();
				std::string chk_str = operandStack.pop().ToString();

				operandStack.push(wiz::String::endsWith(str, chk_str) ? "TRUE" : "FALSE");
			}
			//WIZ_STRING_TYPE - length,
			else if ("$string_length" == str) {
				std::string str = operandStack.pop().ToString();
	
				operandStack.push(wiz::_toString(str.size()));
			}
			//substring ?
			else if ("$substring" == str) {
				std::string str = operandStack.pop().ToString();
				long long begin = stoll(operandStack.pop().ToString());
				long long end = stoll(operandStack.pop().ToString());

				operandStack.push(wiz::String::substring(str, begin, end - 1));
			}

			// todo - Is~ ?? others ??
			else if ("$is_integer_type" == str) {
				operandStack.push(wiz::load_data::Utility::IsInteger(operandStack.pop().ToString()) ? "TRUE" : "FALSE");
			}
			else if ("$is_float_type" == str) {
				operandStack.push(wiz::load_data::Utility::IsDouble(operandStack.pop().ToString()) ? "TRUE" : "FALSE");
			}
			else if ("$is_pure_string_type" == str) {
				operandStack.push("STRING" == wiz::load_data::Utility::GetType(operandStack.pop().ToString()) ? "TRUE" : "FALSE");
			}
			else if ("$get_type" == str) {
				operandStack.push(wiz::load_data::Utility::GetType(operandStack.pop().ToString()));
			}
			else if ("$is_itemtype_exist" == str) { // 2? - using ToBool4?
				operandStack.push(wiz::load_data::LoadData::Find(&global, operandStack.pop().ToString(), builder).empty() ? "FALSE" : "TRUE");
			}
			else if ("$is_usertype_exist" == str) { // 2? - using ToBool4?
				operandStack.push(wiz::load_data::UserType::Find(&global, operandStack.pop().ToString(), builder).first ? "TRUE" : "FALSE");
			}

			else {
				if (wiz::String::startsWith(str.ToString(), "$") && str.ToString().size() >= 2) {
					std::cout << "no exist in load-data " << str.ToString() << std::endl;
					return false;
				}
				return true;
			}
			return true;
			// cf)
			// remove "
		}

		std::string LoadData::ToBool3(wiz::load_data::UserType& global, const wiz::ArrayMap<std::string, std::string>& parameters, const std::string& temp,
			const EventInfo& info, wiz::StringBuilder* builder) /// has bug!
		{
			wiz::StringTokenizer tokenizer(temp, std::vector<std::string>{ "/" }, builder, 1);
			std::vector<std::string> tokenVec;
			std::string result;

			tokenVec.reserve(tokenizer.countTokens());
			while (tokenizer.hasMoreTokens()) {
				tokenVec.push_back(tokenizer.nextToken());
			}

			for (int i = 0; i < tokenVec.size(); ++i)
			{
				result += "/";
				if (wiz::String::startsWith(tokenVec[i], "$parameter.")) {
					int last = -1;
					for (int j = 0; j < tokenVec[i].size(); ++j)
					{
						if (wiz::isWhitespace(tokenVec[i][j]) || tokenVec[i][j] == '{' || tokenVec[i][j] == '}' || tokenVec[i][j] == '=') {
							last = j - 1;
							break;
						}
					}
					if (last != -1)
					{
						std::string temp = FindParameters(parameters, wiz::String::substring(tokenVec[i], 0, last));

						if (!temp.empty()) {
							tokenVec[i] = wiz::String::replace(wiz::String::substring(tokenVec[i], 0, last), wiz::String::substring(tokenVec[i], 0, last), std::move(temp))
								+ wiz::String::substring(tokenVec[i], last + 1);
						}
					}
					else
					{
						std::string temp = FindParameters(parameters, tokenVec[i]);
						if (!temp.empty()) {
							tokenVec[i] = std::move(temp);
						}
					}
				}
				else if (wiz::String::startsWith(tokenVec[i], "$local.")) {
					int last = -1;
					for (int j = 0; j < tokenVec[i].size(); ++j)
					{
						if (wiz::isWhitespace(tokenVec[i][j]) || tokenVec[i][j] == '{' || tokenVec[i][j] == '}' || tokenVec[i][j] == '=') {
							last = j - 1;
							break;
						}
					}
					if (last != -1)
					{
						std::string temp = FindLocals(info.locals, wiz::String::substring(tokenVec[i], 0, last));

						if (!temp.empty()) {
							tokenVec[i] = wiz::String::replace(wiz::String::substring(tokenVec[i], 0, last), wiz::String::substring(tokenVec[i], 0, last), std::move(temp))
								+ wiz::String::substring(tokenVec[i], last + 1);
						}
					}
					else
					{
						std::string temp = FindLocals(info.locals, tokenVec[i]);
						if (!temp.empty()) {
							tokenVec[i] = std::move(temp);
						}
					}
				}

				result += std::move(tokenVec[i]);
			}
			return result;
		}
		std::string LoadData::ToBool3(wiz::load_data::UserType& global, const wiz::ArrayMap<std::string, std::string>& parameters, std::string&& temp,
			const EventInfo& info, wiz::StringBuilder* builder) /// has bug!
		{
			wiz::StringTokenizer tokenizer(std::move(temp), std::vector<std::string>{ "/" }, builder, 1);
			std::vector<std::string> tokenVec;
			std::string result;

			tokenVec.reserve(tokenizer.countTokens());
			while (tokenizer.hasMoreTokens()) {
				tokenVec.push_back(tokenizer.nextToken());
			}

			for (int i = 0; i < tokenVec.size(); ++i)
			{
				result += "/";
				if (wiz::String::startsWith(tokenVec[i], "$parameter.")) {
					int last = -1;
					for (int j = 0; j < tokenVec[i].size(); ++j)
					{
						if (wiz::isWhitespace(tokenVec[i][j]) || tokenVec[i][j] == '{' || tokenVec[i][j] == '}' || tokenVec[i][j] == '=') {
							last = j - 1;
							break;
						}
					}
					if (last != -1)
					{
						std::string temp = FindParameters(parameters, wiz::String::substring(tokenVec[i], 0, last));

						if (!temp.empty()) {
							tokenVec[i] = wiz::String::replace(wiz::String::substring(tokenVec[i], 0, last), wiz::String::substring(tokenVec[i], 0, last), std::move(temp))
								+ wiz::String::substring(tokenVec[i], last + 1);
						}
					}
					else
					{
						std::string temp = FindParameters(parameters, tokenVec[i]);
						if (!temp.empty()) {
							tokenVec[i] = std::move(temp);
						}
					}
				}
				else if (wiz::String::startsWith(tokenVec[i], "$local.")) {
					int last = -1;
					for (int j = 0; j < tokenVec[i].size(); ++j)
					{
						if (wiz::isWhitespace(tokenVec[i][j]) || tokenVec[i][j] == '{' || tokenVec[i][j] == '}' || tokenVec[i][j] == '=') {
							last = j - 1;
							break;
						}
					}
					if (last != -1)
					{
						std::string temp = FindLocals(info.locals, wiz::String::substring(tokenVec[i], 0, last));

						if (!temp.empty()) {
							tokenVec[i] = wiz::String::replace(wiz::String::substring(tokenVec[i], 0, last), wiz::String::substring(tokenVec[i], 0, last), std::move(temp))
								+ wiz::String::substring(tokenVec[i], last + 1);
						}
					}
					else
					{
						std::string temp = FindLocals(info.locals, tokenVec[i]);
						if (!temp.empty()) {
							tokenVec[i] = std::move(temp);
						}
					}
				}

				result += std::move(tokenVec[i]);
			}
			return result;
		}
		std::string LoadData::ToBool4(wiz::load_data::UserType* now, wiz::load_data::UserType& global, const std::string& temp, const ExcuteData& excuteData, wiz::StringBuilder* builder)
		{
			std::string result = temp;

			if (false == result.empty() && '^' == result[0]) { // for pass ToBool4
				result.erase(result.begin());
				return result;
			}
			//cout << "temp is " << temp << endl;
			/*set<WIZ_STRING_TYPE> utNames; // rename?
			{ // removal??
			UserType utTemp;
			LoadData::LoadDataFromString(result, utTemp);

			for (int i = 0; i < utTemp.GetUserTypeListSize(); ++i) {
			if (utTemp.GetUserTypeList(i)->GetName().empty() == false
			&& utTemp.GetUserTypeList(i)->GetName()[0] == '$'
			) {
			utNames.insert(utTemp.GetUserTypeList(i)->GetName());
			}
			}
			}*/
			//	cout << "result is " << result << endl;

			wiz::ArrayStack<std::string> resultStack;
			//wiz::load_data::UserType ut;
			bool chk = false;
			int count_change = 0;

			bool flag_A = false;
			if (result.size() > 1 && result[0] == '/')
			{
				flag_A = true;
			}
			bool flag_B = false;
			for (int i = 0; i < result.size(); ++i) {
				if (result[i] == '/') {
					flag_B = true;
					break;
				}
			}
			if (flag_B) {
				result = ToBool3(global, excuteData.info.parameters, std::move(result), excuteData.info, builder);
			}
			if (result.empty()) { return ""; }
			if (!flag_A && flag_B) {
				result = std::string(result.c_str() + 1, result.size() - 1);
			}
			//wiz::load_data::LoadData::LoadDataFromString(result, ut);
			//result = ut.ToString();
			if (result.empty()) { return result; }

			//if (ut.empty()) {
			//	return "";
			//}
			//	if (ut.GetUserTypeListSize() == 0 && ut.GetItemListSize() == 1) /// chk
			if (!flag_B) { // chk
				if ('/' == result[0] && result.size() > 1)
				{
					std::string temp = Find(&global, result, builder);

					if (!temp.empty()) {
						result = std::move(temp);
						return result;
					}
				}
				else if (wiz::String::startsWith(result, "$local.")) {
					std::string _temp = FindLocals(excuteData.info.locals, result);
					if (!_temp.empty()) {
						result = std::move(_temp);
						return result;
					}
				}
				else if (wiz::String::startsWith(result, "$parameter.")) {
					std::string _temp = FindParameters(excuteData.info.parameters, result);
					if (!_temp.empty()) {
						result = std::move(_temp);
						return result;
					}
				}

			}

			std::vector<std::string> tokenVec;
			tokenVec.reserve(result.size());

			{
				wiz::StringTokenizer tokenizer(std::move(result), { " ", "\n", "\t", "\r" }, builder, 1); // , "{", "=", "}" }); //
																										  //wiz::StringTokenizer tokenizer2(result, { " ", "\n", "\t", "\r" } ); //
					/*																						//vector<WIZ_STRING_TYPE> tokenVec2;
				int len = 0;
				for (int i = 0; i < result.size(); ++i) {
					bool chk = wiz::isWhitespace(result[i]);
					if (len > 0 && chk) {
						tokenVec.push_back(std::string(&result[i] - len, len));
						len = 0;
					}
					else if (chk) {
						//
					}
					else {
						len++;
					}
				}

				if (len > 0) {
					tokenVec.push_back(std::string(result.c_str() + result.size() - len, len));
				}
				*/
				tokenVec.reserve(tokenizer.countTokens());
				while (tokenizer.hasMoreTokens()) {
					tokenVec.push_back(tokenizer.nextToken());
				}
				

				for (int i = tokenVec.size() - 1; i >= 0; --i)
				{
					std::string before = tokenVec[i];
					if ('/' == tokenVec[i][0] && tokenVec[i].size() > 1)
					{
						std::string _temp = Find(&global, tokenVec[i], builder);

						if ("" != _temp) {
							tokenVec[i] = std::move(_temp);
						}
					}
					else if (wiz::String::startsWith(tokenVec[i], "$local.")) { // && length?
						std::string _temp = FindLocals(excuteData.info.locals, tokenVec[i]);
						if (!_temp.empty()) {
							tokenVec[i] = std::move(_temp);
						}
					}
					else if (wiz::String::startsWith(tokenVec[i], "$parameter.")) { // && length?
						std::string _temp = FindParameters(excuteData.info.parameters, tokenVec[i]);
						if (!_temp.empty()) {
							tokenVec[i] = std::move(_temp);
						}
					}
				}

				//while (tokenizer2.hasMoreTokens()) {
				//	tokenVec2.push_back(tokenizer2.nextToken());
				//}
				//	cout << "result is " << result << endl;
				//result = "";

				//int j = tokenVec.size() - 1;
				//for (int i = tokenVec2.size() - 1; i >= 0 && j >= 0; --i) {
				//	if (tokenVec2[i] == "{" || tokenVec2[i] == "}" || tokenVec2[i] == "=") { 
				//		continue;
				//	}
				//	else {
				//		tokenVec2[i] = tokenVec[j];
				//		--j;
				//	}
				//}
				//	result = "";
				//	for (int i = 0; i < tokenVec.size(); ++i) {
				//		if (i != 0) { result = result + " "; }
				//		
				//		result += tokenVec[i];
				//}
			}
			//cout << "result is " << result << endl;
			//
			wiz::ArrayStack<WIZ_STRING_TYPE> operandStack;
			wiz::ArrayStack<std::string> operatorStack;

			operandStack.reserve(tokenVec.size());
			operatorStack.reserve(tokenVec.size());

			//wiz::StringTokenizer tokenizer(result, { " ", "\n", "\t", "\r" }, builder, 1);
			//vector<WIZ_STRING_TYPE> tokenVec;

			//while (tokenizer.hasMoreTokens()) {
			//		tokenVec.push_back(tokenizer.nextToken());
			//}

			for (int i = tokenVec.size() - 1; i >= 0; --i) {
				// todo - chk first? functions in Event
				if (String::startsWith(tokenVec[i], "$parameter.") ||
					//	tokenVec[i] == "$parameter" || // for lambda
					//	tokenVec[i] == "$local" || // for lambda
					String::startsWith(tokenVec[i], "$local.") ||
					//	"$return" == tokenVec[i] || // for lambda
					'$' != tokenVec[i][0] || ('$' == tokenVec[i][0] && tokenVec[i].size() == 1)
					) {
					operandStack.push(tokenVec[i]);
				}
				else
				{
					// cout << tokenVec[i] << endl;
					operandStack.pop(); // =
					operandStack.pop(); // {
					operatorStack.push(tokenVec[i]);

					if (false == operation(now, global, tokenVec[i], operandStack, excuteData, builder)) // chk!!
					{
						// chk removal here?
						std::cout << " false " << std::endl;
						GETCH();
						//
						operatorStack.pop();
						operandStack.push("{");
						operandStack.push("=");
						operandStack.push(tokenVec[i]);
						continue;
					}

					operandStack[operandStack.size() - 2] = operandStack[operandStack.size() - 1];
					operandStack.pop(); // } 
				}
			}

			//std::vector<WIZ_STRING_TYPE> strVec;
			//std::stack<int> chkBrace;

			//chkBrace.push(0);

			//for (int i = operandStack.size() - 1; i >= 0; --i)
			{
				/*
				if (operandStack[i] == "}") {
				chkBrace.top()++;
				if (chkBrace.top() == 2 && !(i + 4 <= operandStack.size() - 1 && operandStack[i+3] == "=" && operandStack[i+4][0] == '$' && operandStack[i+4].size() > 1))
				{
				WIZ_STRING_TYPE temp = strVec.back();
				strVec.pop_back();
				strVec.pop_back();
				strVec.push_back(temp);

				chkBrace.pop();
				continue;
				}
				chkBrace.pop();
				}
				else if (operandStack[i] == "{") {
				chkBrace.top()++;
				chkBrace.push(0);
				}
				else {
				chkBrace.top()++;
				}
				*/
				//strVec.push_back(operandStack[i]);
			}

			//cout << "result is " << result << endl;
			//result = "";
			//builder->Clear();
			//for (int i = 0; i < strVec.size(); ++i) {
			//	if (i != 0) { 
			//	result = result + " "; 
			//		builder->Append(" ", 1);
			//	}
			// result = result + strVec[i] + " "; // add space!
			//	builder->Append(strVec[i].c_str(), strVec[i].size());
			//	builder->Append(" ", 1);
			//}
			//result = WIZ_STRING_TYPE(builder->Str(), builder->size());
			// todo!  $C = 3 => $C = { 3 } 
			{
				//StringTokenizer tokenizer(result, builder, 1);
				//result = "";
				builder->Clear();

				//while (tokenizer.hasMoreTokens()) {
				for (int i = operandStack.size() - 1; i >= 0; --i) {
					//const WIZ_STRING_TYPE temp = strVec[i]; // tokenizer.nextToken();
					/*
					// chk!! @$paramter - removal? @$. (for regex)??
					if (temp.size() >= 3 && String::startsWith(temp, "$.")) { // cf) @$. ?
					//result = result + temp + " ";
					builder->Append(temp.c_str(), temp.size());
					builder->Append(" ", 1);
					}
					else if (temp.size() >= 12 && String::startsWith(temp, "$parameter.") || (temp.size()) >= 13 && String::startsWith(temp, "@$parameter.")) {
					//result = result + temp + " ";
					builder->Append(temp.c_str(), temp.size());
					builder->Append(" ", 1);
					}
					else if (temp.size() >= 8 && String::startsWith(temp, "$local.") || (temp.size()>=9 && String::startsWith(temp, "@$local."))) {
					//result = result + temp + " ";
					builder->Append(temp.c_str(), temp.size());
					builder->Append(" ", 1);
					}
					else if (
					(temp.size() >= 3 && temp[0] == '@' && temp[1] == '$')) { // chk - removal??
					++i; // tokenizer.nextToken(); // =
					++i;
					WIZ_STRING_TYPE temp2 = strVec[i]; //tokenizer.nextToken(); // " ~~ "
					//result = result + temp + " = { " + temp2 + " } ";

					builder->Append(temp.c_str() + 1, temp.size());
					builder->Append(" = { ", 5);

					builder->Append(temp2.c_str(), temp2.size());
					builder->Append(" } ", 3);
					}
					else
					*/
					{
						//result = result + temp + " "; 
						// temp -> strVec
						builder->Append(operandStack[i].ToString().c_str(), operandStack[i].ToString().size());
						builder->Append(" ", 1);
					}
				}
			}
			result = std::string(builder->Str(), builder->Size());
			if (!result.empty()) {
				result.erase(result.begin() + result.size() - 1);
			}
			/*

			{ // removal?? -why?? - reason?
			UserType ut;
			LoadData::LoadDataFromString(result, ut);

			for (int i = 0; i < ut.GetItemListSize(); ++i) {
			if (utNames.find(ut.GetItemList(i).GetName()) != utNames.end()) {
			UserType temp(ut.GetItemList(i).GetName());
			temp.AddItem("", ut.GetItemList(i).Get(0));
			ut.AddUserTypeItem(move(temp));
			ut.RemoveItemList(i);
			--i;
			}
			}

			result = ut.ToString();
			}

			{ // chk.. - removal?
			for (auto x = excuteData.info.parameters.rbegin(); x != excuteData.info.parameters.rend(); ++x) {
			WIZ_STRING_TYPE temp;
			Utility::ChangeStr(result, { "$parameter." + x->first }, { x->second }, temp);
			result = move(temp);
			}
			for (auto x = excuteData.info.locals.rbegin(); x != excuteData.info.locals.rend(); ++x) {
			WIZ_STRING_TYPE temp;
			Utility::ChangeStr(result, { "$local." + x->first }, { x->second }, temp);
			result = move(temp);
			}
			}
			*/
			//	cout << "result is " << result << endl;
			//	cout << endl;
			return std::move(result);
		}
	}
}