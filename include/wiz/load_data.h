
#ifndef LOAD_DATA_H_INCLUDED
#define LOAD_DATA_H_INCLUDED


#include <vector>
#include <stack>
#include <fstream>
#include <string>
#include <utility>
#include <algorithm>
#include <set>
#include <map>

#include "global.h"
//#include <wiz/Dictionary.h> /// change to map

#include "cpp_string.h"
#include "queues.h"
#include "stacks.h"
//#include <wiz/deck.h>

#include <string>

#include "load_data_types.h"
#include "load_data_utility.h"
#include "load_data_reservers.h"
#include "load_data_condition.h"


//const std::string LEFT = "{";
//const std::string RIGHT = "}";
//const std::string EQ_STR = "="; // EQ 충돌 -> EQ_STR로 변경

#include "array_map.h"

namespace wiz {
	class EventInfo
	{
	public:
		wiz::load_data::UserType* eventUT;
		wiz::ArrayStack< wiz::load_data::UserType* > nowUT; //
		wiz::ArrayStack<int> userType_idx;
		wiz::ArrayMap<std::string, std::string> parameters;
		wiz::ArrayMap<std::string, std::string> locals;
		std::string id; //
		wiz::ArrayStack<std::string> conditionStack;
		wiz::ArrayStack<int> state;
		std::string return_value;
		std::string option;
	public:
		EventInfo() : eventUT(nullptr), return_value("")
		{

		}
	};

	class ExcuteData
	{
	public:
		wiz::load_data::UserType* pEvents;
		EventInfo info; // chk!
		bool chkInfo;
		wiz::ArrayMap<std::string, wiz::load_data::UserType>* pObjectMap;
		wiz::ArrayMap<std::string, wiz::load_data::UserType>* pModule;

		long long depth;

		bool noUseInput;
		bool noUseOutput;
	public:
		explicit ExcuteData()
			: pEvents(nullptr), pObjectMap(nullptr), pModule(nullptr), chkInfo(false), depth(0),
			noUseInput(false), noUseOutput(false)
		{
			//
		}
	};

	class SortInfo // need to rename
	{
	public:
		std::string data;
		int iElement; // 2 : userType, // 1 : item
		size_t idx; // for stable? - chk!!
		wiz::StringBuilder* builder;
	public:
		SortInfo(wiz::StringBuilder* builder = nullptr) : idx(-1), builder(builder) { }
		SortInfo(const std::string& data, int iElement, size_t idx, wiz::StringBuilder* builder)
			: data(data), iElement(iElement), idx(idx), builder(builder)
		{

		}
		// for sorting..
		bool operator<(const SortInfo& info) const
		{
			std::string temp = wiz::load_data::Utility::Compare(this->data, info.data, builder);

			if (this->data == "") {
				return false;
			}
			if (info.data == "") {
				return true;
			}

			if (temp == "< 0") { return true; }
			else if (temp == "> 0") { return false; }
			else if (temp == "== 0") {
				return idx < info.idx;
			}
			else {
				throw "temp is not valid in sortinfo";
			}
		}
	};

	class SortInfo2 // need to rename, sortinfo by dsc..
	{
	public:
		std::string data;
		int iElement; // 2 : userType, // 1 : item
		size_t idx; // for stable? - chk!!
		wiz::StringBuilder* builder;
	public:
		SortInfo2(wiz::StringBuilder* builder = nullptr) : idx(-1), builder(builder) { }
		SortInfo2(const std::string& data, int iElement, size_t idx, wiz::StringBuilder* builder)
			: data(data), iElement(iElement), idx(idx)
		{

		}
		// for sorting..
		bool operator<(const SortInfo2& info) const
		{
			if (this->data == "" && info.data == "") {
				return false;
			}
			if (this->data == "") {
				return true;
			}

			if (info.data == "") {
				return false;
			}

			std::string temp = wiz::load_data::Utility::Compare(this->data, info.data, builder);
			if (temp == "< 0") { return false; }
			else if (temp == "> 0") { return true; }
			else if (temp == "== 0") {
				return idx < info.idx;
			}
			else {
				std::cout << "sortInfo2" << std::endl;
				std::cout << data << " " << info.data << std::endl;
				throw "temp is not valid in sortinfo2";
			}
		}
	};


	class Option
	{
	public:
		wiz::ArrayMap<std::string, std::pair<std::vector<std::string>, bool>>* _map = nullptr; // todo - fixed max size? and rename
		//std::vector<std::thread*> waits;
		wiz::ArrayMap<std::string, wiz::load_data::UserType>* objectMap = nullptr; // std::string -> wiz::load_data::UserType
		wiz::ArrayMap<std::string, wiz::load_data::UserType>* moduleMap = nullptr;
		std::string* module_value = nullptr;
		// data, event load..
		wiz::ArrayStack<EventInfo>* eventStack = nullptr;
		wiz::ArrayMap<std::string, int>* convert = nullptr;
		std::vector<wiz::load_data::UserType*>* _events = nullptr;
		wiz::load_data::UserType* events = nullptr;
		wiz::load_data::UserType* Main = nullptr;
		wiz::StringBuilder* builder = nullptr;

	public:
		virtual ~Option()
		{
			if (_map) {
				delete _map;
			}
			if (objectMap) {
				delete objectMap;
			}
			if (moduleMap) {
				delete moduleMap;
			}
			if (module_value) {
				delete module_value;
			}
			if (eventStack) {
				delete eventStack;
			}
			if (convert) {
				delete convert;
			}
			if (_events) {
				delete _events;
			}
			if (events) {
				delete events;
			}
			if (Main) {
				delete Main;
			}
			if (builder) {
				delete builder;
			}
		}
	};
}

namespace wiz {
	namespace load_data {
		class EXCUTE_MODULE_BASE
		{
		public:
			virtual std::string excute_module(const std::string& MainStr,
				wiz::load_data::UserType* _global, const ExcuteData& excuteData, Option& opt, int chk)
			{
				return ""; // error!
			}
		};
	}

	class ClauText : public wiz::load_data::EXCUTE_MODULE_BASE {
	public:
		ClauText(const ClauText&) = delete;
		ClauText& operator=(const ClauText&) = delete;

		ClauText()
		{
			//
		}
		virtual ~ClauText()
		{
			//
		}

	public:
		// ToDo - with wiz::load_data::LoadData::ToBool4
		void MStyleTest(wiz::load_data::UserType* pUt);

		void ShellMode(wiz::load_data::UserType& global);

		virtual std::string excute_module(const std::string& MainStr, wiz::load_data::UserType* _global, const ExcuteData& excuteData, Option& option, int chk);
	};
}

inline wiz::ClauText clauText;

namespace wiz {

	namespace load_data {
		class LoadData
		{
		public:
			inline static EXCUTE_MODULE_BASE* pExcuteModule = &clauText;
		private:
			static bool isState0(const long long state_reserve)
			{
				return 1 == state_reserve;
			}
			/// core
		public:
			template <class Reserver>
			static bool _LoadData(ARRAY_QUEUE<Token>& strVec, Reserver& reserver, UserType& global, const wiz::LoadDataOption& option) // first, strVec.empty() must be true!!
			{
				int state = 0;
				int braceNum = 0;
				long long state_reserve = 0;
				std::vector< UserType* > nestedUT(1);
				std::string var1, var2, val;

				nestedUT[0] = &global;
				{
					reserver(strVec, option);

					while (strVec.empty())
					{
						reserver(strVec, option);
						if (
							strVec.empty() &&
							reserver.end()
							) {
							return false; // throw "Err nextToken does not exist"; // cf) or empty file or empty std::string!
						}
					}
				}

				//	for (int i = 0; i < strVec.size(); ++i) {
				//		cout << strVec[i].str << endl;
				//	}

				int sum = 0;
				int a = clock();
				int b;

				while (false == strVec.empty()) {

					//	cout << state << " " << Utility::Top(strVec, nestedUT[braceNum], reserver, option) << endl;

					switch (state)
					{
					case 0:
					{
						const std::string top = Utility::Top(strVec, nestedUT[braceNum], reserver, option);
						if (top.size() == 1 && -1 != Equal(option.Left, top[0])) {
							state = 2;
						}
						else {
							std::pair<bool, std::string> bsPair = Utility::LookUp(strVec, nestedUT[braceNum], reserver, option);
							if (bsPair.first) {
								if (-1 != checkDelimiter(bsPair.second.c_str(), 
														bsPair.second.c_str() + bsPair.second.size() - 1, option.Assignment)) {
									Utility::Pop(strVec, &var2, nestedUT[braceNum], reserver, option);
									Utility::Pop(strVec, nullptr, nestedUT[braceNum], reserver, option);
									state = 2;
								}
								else {
									if (Utility::Pop(strVec, &var1, nestedUT[braceNum], reserver, option)) {
										nestedUT[braceNum]->AddItem("", move(var1));
										state = 0;
									}
								}
							}
							else {
								if (Utility::Pop(strVec, &var1, nestedUT[braceNum], reserver, option)) {
									nestedUT[braceNum]->AddItem("", move(var1));
									state = 0;
								}
							}
						}
					}
					break;
					case 1:
					{
						const std::string top = Utility::Top(strVec, nestedUT[braceNum], reserver, option);
						if (top.size() == 1 && -1 != Equal(option.Right, top[0])) {
							Utility::Pop(strVec, nullptr, nestedUT[braceNum], reserver, option);
							state = 0;
						}
						else {
							// syntax error.
							throw "syntax error 1 ";
						}
					}
					break;
					case 2:
					{
						const std::string top = Utility::Top(strVec, nestedUT[braceNum], reserver, option);
						if (top.size() == 1 && -1 != Equal(option.Left, top[0])) {
							Utility::Pop(strVec, nullptr, nestedUT[braceNum], reserver, option);

							///
							nestedUT[braceNum]->AddUserTypeItem(UserType(var2));
							UserType* pTemp = nullptr;
							nestedUT[braceNum]->GetLastUserTypeItemRef(var2, pTemp);

							braceNum++;

							/// new nestedUT
							if (nestedUT.size() == braceNum) /// changed 2014.01.23..
								nestedUT.push_back(nullptr);

							/// initial new nestedUT.
							nestedUT[braceNum] = pTemp;
							///
							state = 3;
						}
						else {
							if (Utility::Pop(strVec, &val, nestedUT[braceNum], reserver, option)) {

								nestedUT[braceNum]->AddItem(move(var2), move(val));
								var2 = "";
								val = "";

								state = 0;
							}
						}
					}
					break;
					case 3:
					{
						const std::string top = Utility::Top(strVec, nestedUT[braceNum], reserver, option);
						if (top.size() == 1 && -1 != Equal(option.Right, top[0])) {
							Utility::Pop(strVec, nullptr, nestedUT[braceNum], reserver, option);

							if (0 == braceNum) {
								throw "BraceNum is <= 0";
							}

							nestedUT[braceNum] = nullptr;
							braceNum--;

							state = 0;
						}
						else {
							{
								/// uisng struct
								state_reserve++;
								state = 4;
							}
							//else
							{
								//	throw  "syntax error 2 ";
							}
						}
					}
					break;
					case 4:
					{
						const std::string top = Utility::Top(strVec, nestedUT[braceNum], reserver, option);
						if (top.size() == 1 && -1 != Equal(option.Left, top[0])) {
							Utility::Pop(strVec, nullptr, nestedUT[braceNum], reserver, option);

							UserType temp("");

							nestedUT[braceNum]->AddUserTypeItem(temp);
							UserType* pTemp = nullptr;
							nestedUT[braceNum]->GetLastUserTypeItemRef("", pTemp);

							braceNum++;

							/// new nestedUT
							if (nestedUT.size() == braceNum) /// changed 2014.01.23..
								nestedUT.push_back(nullptr);

							/// initial new nestedUT.
							nestedUT[braceNum] = pTemp;
							///
							//}

							state = 5;
						}
						else if (top.size() == 1 && -1 != Equal(option.Right, top[0])) {
							Utility::Pop(strVec, nullptr, nestedUT[braceNum], reserver, option);
							state = isState0(state_reserve) ? 0 : 4;
							state_reserve--;

							{
								if (0 == braceNum) {
									throw "BraceNum is <= 0";
								}

								nestedUT[braceNum] = nullptr;
								braceNum--;
							}
						}
						else {
							std::pair<bool, std::string> bsPair = Utility::LookUp(strVec, nestedUT[braceNum], reserver, option);
							if (bsPair.first) {
								if (-1 != checkDelimiter(bsPair.second.c_str(),
														bsPair.second.c_str() + bsPair.second.size() - 1, option.Assignment)) {
									// var2
									Utility::Pop(strVec, &var2, nestedUT[braceNum], reserver, option);
									Utility::Pop(strVec, nullptr, nestedUT[braceNum], reserver, option); // pass EQ_STR
									state = 6;
								}
								else {
									// var1
									if (Utility::Pop(strVec, &var1, nestedUT[braceNum], reserver, option)) {
										nestedUT[braceNum]->AddItem("", move(var1));
										var1 = "";

										state = 4;
									}
								}
							}
							else
							{
								// var1
								if (Utility::Pop(strVec, &var1, nestedUT[braceNum], reserver, option))
								{
									nestedUT[braceNum]->AddItem("", move(var1));
									var1 = "";

									state = 4;
								}
							}
						}
					}
					break;
					case 5:
					{
						const std::string top = Utility::Top(strVec, nestedUT[braceNum], reserver, option);
						if (top.size() == 1 && -1 != Equal(option.Right, top[0])) {
							Utility::Pop(strVec, nullptr, nestedUT[braceNum], reserver, option);

							if (0 == braceNum) {
								throw "BraceNum is <= 0";
							}

							//if (flag1 == 0) {
							nestedUT[braceNum] = nullptr;
							braceNum--;
							// }
							//
							state = 4;
						}
						else {
							int idx = -1;
							int num = -1;


							{
								/// uisng struct
								state_reserve++;
								state = 4;
							}
							//else
							{
								//	throw "syntax error 4  ";
							}
						}
					}
					break;
					case 6:
					{
						std::string top = Utility::Top(strVec, nestedUT[braceNum], reserver, option);
						if (top.size() == 1 && -1 != Equal(option.Left, top[0])) {
							Utility::Pop(strVec, nullptr, nestedUT[braceNum], reserver, option);

							///
							{
								nestedUT[braceNum]->AddUserTypeItem(UserType(var2));
								UserType* pTemp = nullptr;
								nestedUT[braceNum]->GetLastUserTypeItemRef(var2, pTemp);
								var2 = "";
								braceNum++;

								/// new nestedUT
								if (nestedUT.size() == braceNum) /// changed 2014.01.23..
									nestedUT.push_back(nullptr);

								/// initial new nestedUT.
								nestedUT[braceNum] = pTemp;
							}
							///
							state = 7;
						}
						else {
							if (Utility::Pop(strVec, &val, nestedUT[braceNum], reserver, option)) {

								nestedUT[braceNum]->AddItem(move(var2), move(val));
								var2 = ""; val = "";

								top = Utility::Top(strVec, nestedUT[braceNum], reserver, option);

								if (strVec.empty())
								{
									//
								}
								else if (top.size() == 1 && -1 != Equal(option.Right, top[0])) {
									Utility::Pop(strVec, nullptr, nestedUT[braceNum], reserver, option);

									if (0 == braceNum) {
										throw "BraceNum is <= 0";
									}

									{
										state = isState0(state_reserve) ? 0 : 4;
										state_reserve--;

										{
											nestedUT[braceNum] = nullptr;
											braceNum--;
										}
									}
									{
										//state = 4;
									}
								}
								else {
									state = 4;
								}
							}
						}
					}
					break;
					case 7:
					{
						const std::string top = Utility::Top(strVec, nestedUT[braceNum], reserver, option);
						if (top.size() == 1 && -1 != Equal(option.Right, top[0])) {
							Utility::Pop(strVec, nullptr, nestedUT[braceNum], reserver, option);
							//

							if (0 == braceNum) {
								throw "BraceNum is <= 0";
							}

							nestedUT[braceNum] = nullptr;
							braceNum--;
							//
							state = 4;
						}
						else {
							int idx = -1;
							int num = -1;

							{
								/// uisng struct
								state_reserve++;

								state = 4;
							}
							//else
							{
								//throw "syntax error 5 ";
							}
						}
					}
					break;
					default:
						// syntax err!!

						throw "syntax error 6 ";
						break;
					}

					if (strVec.size() < 10) {
						b = clock();
						sum += std::chrono::steady_clock::duration(b - a).count();
						//std::cout << b - a << "ms" << std::endl;

						reserver(strVec, option);

						while (strVec.empty()) // (strVec.empty())
						{
							reserver(strVec, option);
							if (
								strVec.empty() &&
								reserver.end()
								) {
								// throw "Err nextToken does not exist2";
								break;
							}
						}
						a = clock();
					}
				}
				if (state != 0) {
					throw std::string("error final state is not 0!  : ") + toStr(state);
				}
				if (braceNum != 0) {
					throw std::string("chk braceNum is ") + toStr(braceNum);
				}
				 b = clock();

				//std::cout << sum << "ms" << std::endl;
				return true;
			}

		private:
			static bool _Empty(std::vector<ARRAY_QUEUE<Token>>& strVec) // chk!!
			{
				for (int i = 0; i < strVec.size(); ++i) {
					if (strVec[i].empty()) {
						continue;
					}
					else {
						return false;
					}
				}

				return true;
			}
			static int _Size(std::vector<ARRAY_QUEUE<Token>>& strVec) // chk!!
			{
				int sum = 0;

				for (int i = 0; i < strVec.size(); ++i) {
					bool isEmpty = false;
					for (int j = 0; j < strVec[i].size(); ++j) {
						if (strVec[i][j].str == "") {
							isEmpty = true;
						}
						else {
							isEmpty = false;
							break;
						}
					}
					if (isEmpty == false) {
						sum = sum + strVec[i].size();
					}
				}

				return sum;
			}
		public:
			template <class Reserver>
			static bool _LoadData2(std::vector<ARRAY_QUEUE<Token>>& strVec, Reserver& reserver, UserType& global, const wiz::LoadDataOption& option) // first, strVec.empty() must be true!!
			{
				int state = 0;
				int braceNum = 0;
				long long state_reserve = 0;
				std::vector< UserType* > nestedUT(1);
				std::string var1, var2, val;

				int now_thread = 0; // strVec no?


				nestedUT[0] = &global;
				

				{
					reserver(strVec, option, now_thread);

					while (_Empty(strVec))
					{
						reserver(strVec, option, now_thread);
						if (
							_Empty(strVec) &&
							reserver.end()
							) {
							return false; // throw "Err nextToken does not exist"; // cf) or empty file or empty std::string!
						}
					}
				}

				//for (int i = 0; i < strVec[0].size(); ++i) {
				//	std::cout << strVec[0][i].str << std::endl;
				//	GETCH();
				//}

				while (false == _Empty(strVec)) {

					//std::cout << state << " " << Utility::Top(strVec, nestedUT[braceNum], reserver, option, &now_thread) << " " << now_thread << std::endl;
					//GETCH();

					switch (state)
					{
					case 0:
					{
						const std::string top = Utility::Top(strVec, nestedUT[braceNum], reserver, option, &now_thread);
						if (top.size() == 1 && -1 != Equal(option.Left, top[0])) {
							state = 2;
						}
						else {
							std::pair<bool, std::string> bsPair = Utility::LookUp(strVec, nestedUT[braceNum], reserver, option, &now_thread);
							if (bsPair.first) {
								if (-1 != checkDelimiter(bsPair.second.c_str(),
													bsPair.second.c_str() + bsPair.second.size() - 1, option.Assignment)) {
									Utility::Pop(strVec, &var2, nestedUT[braceNum], reserver, option, &now_thread);
									Utility::Pop(strVec, nullptr, nestedUT[braceNum], reserver, option, &now_thread);
									state = 2;
								}
								else {
									if (Utility::Pop(strVec, &var1, nestedUT[braceNum], reserver, option, &now_thread)) {
										nestedUT[braceNum]->AddItem("", move(var1));
										state = 0;
									}
								}
							}
							else {
								if (Utility::Pop(strVec, &var1, nestedUT[braceNum], reserver, option, &now_thread)) {
									nestedUT[braceNum]->AddItem("", move(var1));
									state = 0;
								}
							}
						}
					}
					break;
					case 1:
					{
						const std::string top = Utility::Top(strVec, nestedUT[braceNum], reserver, option, &now_thread);
						if (top.size() == 1 && -1 != Equal(option.Right, top[0])) {
							Utility::Pop(strVec, nullptr, nestedUT[braceNum], reserver, option, &now_thread);
							state = 0;
						}
						else {
							// syntax error.
							throw "syntax error 1 ";
						}
					}
					break;
					case 2:
					{
						const std::string top = Utility::Top(strVec, nestedUT[braceNum], reserver, option, &now_thread);
						if (top.size() == 1 && -1 != Equal(option.Left, top[0])) {
							Utility::Pop(strVec, nullptr, nestedUT[braceNum], reserver, option, &now_thread);

							///
							nestedUT[braceNum]->AddUserTypeItem(UserType(var2));
							UserType* pTemp = nullptr;
							nestedUT[braceNum]->GetLastUserTypeItemRef(var2, pTemp);

							braceNum++;

							/// new nestedUT
							if (nestedUT.size() == braceNum) /// changed 2014.01.23..
								nestedUT.push_back(nullptr);

							/// initial new nestedUT.
							nestedUT[braceNum] = pTemp;
							///
							state = 3;
						}
						else {
							if (Utility::Pop(strVec, &val, nestedUT[braceNum], reserver, option, &now_thread)) {

								nestedUT[braceNum]->AddItem(move(var2), move(val));
								var2 = "";
								val = "";

								state = 0;
							}
						}
					}
					break;
					case 3:
					{
						const std::string top = Utility::Top(strVec, nestedUT[braceNum], reserver, option, &now_thread);
						if (top.size() == 1 && -1 != Equal(option.Right, top[0])) {
							Utility::Pop(strVec, nullptr, nestedUT[braceNum], reserver, option, &now_thread);

							nestedUT[braceNum] = nullptr;
							braceNum--;

							state = 0;
						}
						else {
							{
								/// uisng struct
								state_reserve++;
								state = 4;
							}
							//else
							{
								//	throw  "syntax error 2 ";
							}
						}
					}
					break;
					case 4:
					{
						const std::string top = Utility::Top(strVec, nestedUT[braceNum], reserver, option, &now_thread);
						if (top.size() == 1 && -1 != Equal(option.Left, top[0])) {
							Utility::Pop(strVec, nullptr, nestedUT[braceNum], reserver, option, &now_thread);

							UserType temp("");

							nestedUT[braceNum]->AddUserTypeItem(temp);
							UserType* pTemp = nullptr;
							nestedUT[braceNum]->GetLastUserTypeItemRef("", pTemp);

							braceNum++;

							/// new nestedUT
							if (nestedUT.size() == braceNum) /// changed 2014.01.23..
								nestedUT.push_back(nullptr);

							/// initial new nestedUT.
							nestedUT[braceNum] = pTemp;
							///
							//}

							state = 5;
						}
						else if (top.size() == 1 && -1 != Equal(option.Right, top[0])) {
							Utility::Pop(strVec, nullptr, nestedUT[braceNum], reserver, option, &now_thread);
							state = isState0(state_reserve) ? 0 : 4;
							state_reserve--;

							{
								nestedUT[braceNum] = nullptr;
								braceNum--;
							}
						}
						else {
							std::pair<bool, std::string> bsPair = Utility::LookUp(strVec, nestedUT[braceNum], reserver, option, &now_thread);
							if (bsPair.first) {
								if (-1 != checkDelimiter(bsPair.second.c_str(),
															bsPair.second.c_str() + bsPair.second.size() - 1, option.Assignment)) {
									// var2
									Utility::Pop(strVec, &var2, nestedUT[braceNum], reserver, option, &now_thread);
									Utility::Pop(strVec, nullptr, nestedUT[braceNum], reserver, option, &now_thread); // pass EQ_STR
									state = 6;
								}
								else {
									// var1
									if (Utility::Pop(strVec, &var1, nestedUT[braceNum], reserver, option, &now_thread)) {
										nestedUT[braceNum]->AddItem("", move(var1));
										var1 = "";

										state = 4;
									}
								}
							}
							else
							{
								// var1
								if (Utility::Pop(strVec, &var1, nestedUT[braceNum], reserver, option, &now_thread))
								{
									nestedUT[braceNum]->AddItem("", move(var1));
									var1 = "";

									state = 4;
								}
							}
						}
					}
					break;
					case 5:
					{
						const std::string top = Utility::Top(strVec, nestedUT[braceNum], reserver, option, &now_thread);
						if (top.size() == 1 && -1 != Equal(option.Right, top[0])) {
							Utility::Pop(strVec, nullptr, nestedUT[braceNum], reserver, option, &now_thread);

							//if (flag1 == 0) {
							nestedUT[braceNum] = nullptr;
							braceNum--;
							// }
							//
							state = 4;
						}
						else {
							int idx = -1;
							int num = -1;


							{
								/// uisng struct
								state_reserve++;
								state = 4;
							}
							//else
							{
								//	throw "syntax error 4  ";
							}
						}
					}
					break;
					case 6:
					{
						std::string top = Utility::Top(strVec, nestedUT[braceNum], reserver, option, &now_thread);
						if (top.size() == 1 && -1 != Equal(option.Left, top[0])) {
							Utility::Pop(strVec, nullptr, nestedUT[braceNum], reserver, option, &now_thread);

							///
							{
								nestedUT[braceNum]->AddUserTypeItem(UserType(var2));
								UserType* pTemp = nullptr;
								nestedUT[braceNum]->GetLastUserTypeItemRef(var2, pTemp);
								var2 = "";
								braceNum++;

								/// new nestedUT
								if (nestedUT.size() == braceNum) /// changed 2014.01.23..
									nestedUT.push_back(nullptr);

								/// initial new nestedUT.
								nestedUT[braceNum] = pTemp;
							}
							///
							state = 7;
						}
						else {
							if (Utility::Pop(strVec, &val, nestedUT[braceNum], reserver, option, &now_thread)) {

								nestedUT[braceNum]->AddItem(move(var2), move(val));
								var2 = ""; val = "";

								top = Utility::Top(strVec, nestedUT[braceNum], reserver, option, &now_thread);

								if (strVec.empty())
								{
									//
								}
								else if (top.size() == 1 && -1 != Equal(option.Right, top[0])) {
									Utility::Pop(strVec, nullptr, nestedUT[braceNum], reserver, option, &now_thread);

									{
										state = isState0(state_reserve) ? 0 : 4;
										state_reserve--;

										{
											nestedUT[braceNum] = nullptr;
											braceNum--;
										}
									}
									{
										//state = 4;
									}
								}
								else {
									state = 4;
								}
							}
						}
					}
					break;
					case 7:
					{
						const std::string top = Utility::Top(strVec, nestedUT[braceNum], reserver, option, &now_thread);
						if (top.size() == 1 && -1 != Equal(option.Right, top[0])) {
							Utility::Pop(strVec, nullptr, nestedUT[braceNum], reserver, option, &now_thread);
							//

							nestedUT[braceNum] = nullptr;
							braceNum--;
							//
							state = 4;
						}
						else {
							int idx = -1;
							int num = -1;

							{
								/// uisng struct
								state_reserve++;

								state = 4;
							}
							//else
							{
								//throw "syntax error 5 ";
							}
						}
					}
					break;
					default:
						// syntax err!!

						throw "syntax error 6 ";
						break;
					}

					if (_Size(strVec) < 10) {
						reserver(strVec, option, now_thread);

						while (_Empty(strVec)) // (strVec.empty())
						{
							reserver(strVec, option, now_thread);
							if (
								_Empty(strVec) &&
								reserver.end()
								) {
								// throw "Err nextToken does not exist2";
								break;
							}
						}
					}
				}
				if (state != 0) {
					throw std::string("error final state is not 0!  : ") + toStr(state);
				}
				if (braceNum != 0) {
					throw std::string("chk braceNum is ") + toStr(braceNum);
				}

				return true;
			}

			// line comment -> accept but no save!
			// no multiple comment!
			static bool __LoadData3(VECTOR<Token2>* _strVec, long long start_idx, long long last_idx, UserType* _global, const wiz::LoadDataOption* _option,
				int start_state, int last_state, UserType** next) // first, strVec.empty() must be true!!
			{
				if (start_idx > last_idx) {
					return false;
				}
				
				VECTOR<Token2>& strVec = *_strVec;
				UserType& global = *_global;
				const wiz::LoadDataOption& option = *_option;

				int state = start_state;
				int braceNum = 0;
				//long long state_reserve = 0;

				std::vector< UserType* > nestedUT(1);
				std::string var1, var2, val;

				bool varOn = false;
				nestedUT[0] = &global;

				//	for (int i = 0; i < strVec.size(); ++i) {
				//		cout << strVec[i].str << endl;
				//	}
				long long i = start_idx;
				bool end_state = false;
				while (i <= last_idx && !end_state) {
					//std::cout << state << " ";
					//	cout << state << " " << Utility::Top(strVec, nestedUT[braceNum], reserver, option) << endl;
					
					switch (state)
					{
					case 0:
					{
						while (i <= last_idx && strVec[i].isComment) {
							
							++i;
						}
						if (i >= last_idx + 1) {
							//end_state = true;
							//
						}

						if (!end_state) {
							auto top = strVec[i];
							if (top.len == 1 && -1 != Equal(option.Left, top.str[0])) {
								state = 2;
							}
							else {
								std::pair<bool, Token2> bsPair;

								int tmp = i;
								++i;
								while (i <= last_idx && strVec[i].isComment) {
									
									++i;
								}
								if (i >= last_idx + 1) {
									//end_state = true;
									//
								}

								if (!end_state) {
									if (i <= last_idx) {
										bsPair = std::make_pair(true, strVec[i]);
									}
									else {
										bsPair = std::make_pair(false, Token2());
									}
									i = tmp;

									if (bsPair.first) {
										long long idx = -1;
										if (-1 != (idx = checkDelimiter(bsPair.second.str, 
																				bsPair.second.str + bsPair.second.len - 1, option.Assignment))) {
											
											var2 = std::string(strVec[i].str, strVec[i].len); // Utility::Pop(strVec, &var2, nestedUT[braceNum], option);
																			 //Utility::Pop(strVec, nullptr, nestedUT[braceNum], option);
											state = 2;
											i++;
											while (i <= last_idx && strVec[i].isComment) {
												
												++i;
											}
											if (i >= last_idx + 1) {
												//end_state = true;
												//
											}
											i++;
											while (i <= last_idx && strVec[i].isComment) {
												
												++i;
											}
											if (i >= last_idx + 1) {
												//end_state = true;
												//
											}

										}
										else {
											if (i <= last_idx) {
												//Utility::Pop(strVec, &var1, nestedUT[braceNum], option)) {
												var1 = std::string(strVec[i].str, strVec[i].len);

												nestedUT[braceNum]->AddItem("", move(var1));
												state = 0;
												i += 1;
												while (i <= last_idx && strVec[i].isComment) {
													
													++i;
												}
												if (i >= last_idx + 1) {
													//end_state = true;
													//
												}
											}
										}
									}
									else {
										if (i <= last_idx) {
											//Utility::Pop(strVec, &var1, nestedUT[braceNum], option)) {
											var1 = std::string(strVec[i].str, strVec[i].len);

											nestedUT[braceNum]->AddItem("", move(var1));
											i += 1;
											while (i <= last_idx && strVec[i].isComment) {
												
												++i;
											}
											if (i >= last_idx + 1) {
												//end_state = true;
												//
											}
											state = 0;
										}
									}
								}
							}
						}
					}
					break;
					case 1:
					{
						while (i <= last_idx && strVec[i].isComment) {
							
							++i;
						}
						if (i >= last_idx + 1) {
							//end_state = true;
							//
						}
						if (!end_state) {
							auto top = strVec[i];
							if (top.len == 1 && -1 != Equal(option.Right, top.str[0])) {
								//Utility::Pop(strVec, nullptr, nestedUT[braceNum], option);
								i += 1;
								while (i <= last_idx && strVec[i].isComment) {
									
									++i;
								}
								if (i >= last_idx + 1) {
									//end_state = true;
									//	break;
								}
								state = 0;
							}
							else {
								
								// syntax error.
								throw "syntax error 1 ";
							}
						}
					}
					break;
					case 2:
					{
						while (i <= last_idx && strVec[i].isComment) {
							
							++i;
						}
						if (i >= last_idx + 1) { //end_state = true; 
							//break;
						}

						if (!end_state) {
							auto top = strVec[i];
							if (top.len == 1 && -1 != Equal(option.Left, top.str[0])) {
								//Utility::Pop(strVec, nullptr, nestedUT[braceNum], option);
								i += 1;
								while (i <= last_idx && strVec[i].isComment) {
									
									++i;
								}
								if (i >= last_idx + 1) {
									//end_state = true;
									//break;
								}
								if (!end_state) {
									///
									nestedUT[braceNum]->AddUserTypeItem(UserType(var2));
									UserType* pTemp = nullptr;
									nestedUT[braceNum]->GetLastUserTypeItemRef(var2, pTemp);

									braceNum++;

									/// new nestedUT
									if (nestedUT.size() == braceNum) /// changed 2014.01.23..
										nestedUT.push_back(nullptr);

									/// initial new nestedUT.
									nestedUT[braceNum] = pTemp;
									///
									state = 3;
								}
							}
							else {
								if (i <= last_idx) {
									//Utility::Pop(strVec, &val, nestedUT[braceNum], option)) {
									val = std::string(strVec[i].str, strVec[i].len);

									nestedUT[braceNum]->AddItem(move(var2), move(val));
									i += 1;
									while (i <= last_idx && strVec[i].isComment) {
										
										++i;
									}
									if (i >= last_idx + 1) {
										//end_state = true;
										//	break;
									}
									if (!end_state) {
										var2 = "";
										val = "";

										state = 0;
									}
								}
							}
						}
					}
					break;
					case 3:
					{
						while (i <= last_idx && strVec[i].isComment) {
							
							++i;
						}
						if (i >= last_idx + 1) {
							//end_state = true;
							//	break;
						}

						if (!end_state) {
							auto top = strVec[i];
							if (top.len == 1 && -1 != Equal(option.Right, top.str[0])) {
								//Utility::Pop(strVec, nullptr, nestedUT[braceNum], option);
								i += 1;
								while (i <= last_idx && strVec[i].isComment) {
									
									++i;
								}
								if (i >= last_idx + 1) {
									//end_state = true;
									//	break;
								}

								if (braceNum == 0) {
									int utCount = 0;
									int itCount = 0;
									UserType ut;
									ut.AddUserTypeItem(UserType("#")); // option->LineComment[0]?
									UserType* pTemp = nullptr;
									ut.GetLastUserTypeItemRef("#", pTemp);

									/// move data
									ut.GetUserTypeList(0)->ReserveIList(nestedUT[braceNum]->GetIListSize());
									ut.GetUserTypeList(0)->ReserveItemList(nestedUT[braceNum]->GetItemListSize());
									ut.GetUserTypeList(0)->ReserveUserTypeList(nestedUT[braceNum]->GetUserTypeListSize());

									for (int k = 0; k < nestedUT[braceNum]->GetIListSize(); ++k) {
										if (nestedUT[braceNum]->IsUserTypeList(k)) {
											ut.GetUserTypeList(0)->AddUserTypeItem(std::move(*nestedUT[braceNum]->GetUserTypeList(utCount)));


											utCount++;
										}
										else {
											ut.GetUserTypeList(0)->AddItemList(std::move(nestedUT[braceNum]->GetItemList(itCount)));
											itCount++;
										}
									}
									nestedUT[braceNum]->Remove();
									nestedUT[braceNum]->AddUserTypeItem(std::move(*ut.GetUserTypeList(0)));

									braceNum++;
								}

								if (braceNum < nestedUT.size()) {
									nestedUT[braceNum] = nullptr;
								}
								braceNum--;

								state = 0;
							}
							else {
								{
									/// uisng struct
									//state_reserve++;
									state = 4;
								}
								//else
								{
									//	throw  "syntax error 2 ";
								}
							}
						}
					}
					break;
					case 4:
					{
						while (i <= last_idx && strVec[i].isComment) {
							
							++i;
						}
						if (i >= last_idx + 1) { //end_state = true; 
						//	break;
						}
						if (!end_state) {
							auto top = strVec[i];

							if (top.len == 1 && -1 != Equal(option.Left, top.str[0])) {
								//Utility::Pop(strVec, nullptr, nestedUT[braceNum], option);
								i += 1;
								while (i <= last_idx && strVec[i].isComment) {
									
									++i;
								}
								if (i >= last_idx + 1) {
									//end_state = true;
									//	break;
								}

								if (!end_state) {
									UserType temp("");

									nestedUT[braceNum]->AddUserTypeItem(temp);
									UserType* pTemp = nullptr;
									nestedUT[braceNum]->GetLastUserTypeItemRef("", pTemp);

									braceNum++;

									/// new nestedUT
									if (nestedUT.size() == braceNum) /// changed 2014.01.23..
										nestedUT.push_back(nullptr);

									/// initial new nestedUT.
									nestedUT[braceNum] = pTemp;
									///
									//}


									state = 5;
								}
							}
							else if (top.len == 1 && -1 != Equal(option.Right, top.str[0])) {
								//Utility::Pop(strVec, nullptr, nestedUT[braceNum], option);
								i += 1;
								while (i <= last_idx && strVec[i].isComment) {
									
									++i;
								}
								if (i >= last_idx + 1) {
									//end_state = true;
									//	break;
								}

								if (!end_state) {
									state = 4;
									//state = isState0(state_reserve) ? 0 : 4;
									//state_reserve--;


									if (braceNum == 0) {
										int utCount = 0;
										int itCount = 0;
										UserType ut;
										ut.AddUserTypeItem(UserType("#"));
										UserType* pTemp = nullptr;
										ut.GetLastUserTypeItemRef("#", pTemp);

										/// move data
										ut.GetUserTypeList(0)->ReserveIList(nestedUT[braceNum]->GetIListSize());
										ut.GetUserTypeList(0)->ReserveItemList(nestedUT[braceNum]->GetItemListSize());
										ut.GetUserTypeList(0)->ReserveUserTypeList(nestedUT[braceNum]->GetUserTypeListSize());

										for (int k = 0; k < nestedUT[braceNum]->GetIListSize(); ++k) {
											if (nestedUT[braceNum]->IsUserTypeList(k)) {
												ut.GetUserTypeList(0)->AddUserTypeItem(std::move(*nestedUT[braceNum]->GetUserTypeList(utCount)));

												utCount++;
											}
											else {
												ut.GetUserTypeList(0)->AddItemList(std::move(nestedUT[braceNum]->GetItemList(itCount)));
												itCount++;
											}
										}

										nestedUT[braceNum]->Remove();
										nestedUT[braceNum]->AddUserTypeItem(std::move(*ut.GetUserTypeList(0)));

										braceNum++;
									}

									{
										if (braceNum < nestedUT.size()) {
											nestedUT[braceNum] = nullptr;
										}
										braceNum--;
									}
								}
							}
							else {
								int temp = i;
								++i;
								while (i <= last_idx && strVec[i].isComment) {
									
									++i;
								}
								if (i >= last_idx + 1) {
									//end_state = true;
									//	break;
								}

								if (!end_state) {
									std::pair<bool, Token2> bsPair;

									if (i <= last_idx)
									{
										bsPair = std::make_pair(true, strVec[i]);
									}
									else {
										bsPair = std::make_pair(false, Token2());
									}
									i = temp;

									if (bsPair.first) {
										long long idx = -1;

										if (-1 != (idx = checkDelimiter(bsPair.second.str,
															bsPair.second.str + bsPair.second.len - 1, option.Assignment))) {
											// var2
											//Utility::Pop(strVec, &var2, nestedUT[braceNum], option);
											//Utility::Pop(strVec, nullptr, nestedUT[braceNum], option); // pass EQ_STR
											var2 = std::string(strVec[i].str, strVec[i].len);
											state = 6;
											i += 1;
											while (i <= last_idx && strVec[i].isComment) {
												
												++i;
											}
											if (i >= last_idx + 1) {
												//end_state = true;
												//	break;
											}
											i += 1;
											while (i <= last_idx && strVec[i].isComment) {
												
												++i;
											}
											if (i >= last_idx + 1) {
												//end_state = true;
												//	break;
											}
										}
										else {
											// var1
											if (i <= last_idx) {
												//Utility::Pop(strVec, &var1, nestedUT[braceNum], option)) {
												var1 = std::string(strVec[i].str, strVec[i].len);

												nestedUT[braceNum]->AddItem("", move(var1));
												var1 = "";

												state = 4;
												i += 1;
												while (i <= last_idx && strVec[i].isComment) {
													
													++i;
												}
												if (i >= last_idx + 1) {
													//end_state = true;
													//	break;
												}
											}
										}
									}
								}
								else
								{
									// var1
									if (i <= last_idx) //Utility::Pop(strVec, &var1, nestedUT[braceNum], option))
									{
										var1 = (std::string(strVec[i].str, strVec[i].len));
										nestedUT[braceNum]->AddItem("", move(var1));
										var1 = "";

										state = 4;
										i += 1;
										while (i <= last_idx && strVec[i].isComment) {
											
											++i;
										}
										if (i >= last_idx + 1) {
											//end_state = true;
											//	break;
										}
									}
								}
							}
						}
					}
					break;
					case 5:
					{
						while (i <= last_idx && strVec[i].isComment) {
							
							++i;
						}
						if (i >= last_idx + 1) {
							//end_state = true;
							//break;
						}

						if (!end_state) {
							auto top = strVec[i];
							if (top.len == 1 && -1 != Equal(option.Right, top.str[0])) {
								//Utility::Pop(strVec, nullptr, nestedUT[braceNum], option);

								if (braceNum == 0) {
									int utCount = 0;
									int itCount = 0;
									UserType ut;
									ut.AddUserTypeItem(UserType("#"));
									UserType* pTemp = nullptr;
									ut.GetLastUserTypeItemRef("#", pTemp);

									/// move data
									ut.GetUserTypeList(0)->ReserveIList(nestedUT[braceNum]->GetIListSize());
									ut.GetUserTypeList(0)->ReserveItemList(nestedUT[braceNum]->GetItemListSize());
									ut.GetUserTypeList(0)->ReserveUserTypeList(nestedUT[braceNum]->GetUserTypeListSize());

									for (int k = 0; k < nestedUT[braceNum]->GetIListSize(); ++k) {
										if (nestedUT[braceNum]->IsUserTypeList(k)) {
											ut.GetUserTypeList(0)->AddUserTypeItem(std::move(*nestedUT[braceNum]->GetUserTypeList(utCount)));
											//

											utCount++;
										}
										else {
											ut.GetUserTypeList(0)->AddItemList(std::move(nestedUT[braceNum]->GetItemList(itCount)));
											itCount++;
										}
									}
									nestedUT[braceNum]->Remove();
									nestedUT[braceNum]->AddUserTypeItem(std::move(*ut.GetUserTypeList(0)));

									braceNum++;
								}

								//if (flag1 == 0) {
								if (braceNum < nestedUT.size()) {
									nestedUT[braceNum] = nullptr;
								}
								braceNum--;
								// }
								//
								state = 4;
								i += 1;
								while (i <= last_idx && strVec[i].isComment) {
									
									++i;
								}
								if (i >= last_idx + 1) {
									//end_state = true;
									//	break;
								}
							}
							else {
								int idx = -1;
								int num = -1;


								{
									/// uisng struct
									//state_reserve++;
									state = 4;
								}
								//else
								{
									//	throw "syntax error 4  ";
								}
							}
						}
					}
					break;
					case 6:
					{
						while (i <= last_idx && strVec[i].isComment) {
							
							++i;
						}
						if (i >= last_idx + 1) {
							//end_state = true;
							//break;
						}

						if (!end_state) {
							std::string top = std::string(strVec[i].str, strVec[i].len);

							if (top.size() == 1 && -1 != Equal(option.Left, top[0])) {
								//Utility::Pop(strVec, nullptr, nestedUT[braceNum], option);
								i += 1;
								while (i <= last_idx && strVec[i].isComment) {
									
									++i;
								}
								if (i >= last_idx + 1) {
									//end_state = true;
									//	break;
								}
								if (!end_state) {
									nestedUT[braceNum]->AddUserTypeItem(UserType(var2));
									UserType* pTemp = nullptr;
									nestedUT[braceNum]->GetLastUserTypeItemRef(var2, pTemp);
									var2 = "";
									braceNum++;

									/// new nestedUT
									if (nestedUT.size() == braceNum) /// changed 2014.01.23..
										nestedUT.push_back(nullptr);

									/// initial new nestedUT.
									nestedUT[braceNum] = pTemp;


								}
								///
								state = 7;
							}
							else {
								if (i <= last_idx) { //Utility::Pop(strVec, &val, nestedUT[braceNum], option)) {
									val = (std::string(strVec[i].str, strVec[i].len));

									i += 1;
									while (i <= last_idx && strVec[i].isComment) {
										
										++i;
									}
									if (i >= last_idx + 1) {
										//end_state = true;
										//	break;
									}
									if (!end_state) {
										nestedUT[braceNum]->AddItem(move(var2), move(val));
										var2 = ""; val = "";

										top = std::string(strVec[i].str, strVec[i].len);

										if (strVec.empty())
										{
											//
										}
										else if (top.size() == 1 && -1 != Equal(option.Right, top[0])) {
											//Utility::Pop(strVec, nullptr, nestedUT[braceNum], option);
											i += 1;
											while (i <= last_idx && strVec[i].isComment) {
												
												++i;
											}
											if (i >= last_idx + 1) {
												//end_state = true;
												//	break;
											}
											if (!end_state) {
												if (braceNum == 0) {
													int utCount = 0;
													int itCount = 0;
													UserType ut;
													ut.AddUserTypeItem(UserType("#"));
													UserType* pTemp = nullptr;
													ut.GetLastUserTypeItemRef("#", pTemp);

													/// move data
													ut.GetUserTypeList(0)->ReserveIList(nestedUT[braceNum]->GetIListSize());
													ut.GetUserTypeList(0)->ReserveItemList(nestedUT[braceNum]->GetItemListSize());
													ut.GetUserTypeList(0)->ReserveUserTypeList(nestedUT[braceNum]->GetUserTypeListSize());

													for (int k = 0; k < nestedUT[braceNum]->GetIListSize(); ++k) {
														if (nestedUT[braceNum]->IsUserTypeList(k)) {
															ut.GetUserTypeList(0)->AddUserTypeItem(std::move(*nestedUT[braceNum]->GetUserTypeList(utCount)));
															//	//


															utCount++;
														}
														else {
															ut.GetUserTypeList(0)->AddItemList(std::move(nestedUT[braceNum]->GetItemList(itCount)));
															itCount++;
														}
													}
													nestedUT[braceNum]->Remove();
													nestedUT[braceNum]->AddUserTypeItem(std::move(*ut.GetUserTypeList(0)));


													braceNum++;
												}
											}
											{
												state = 4;
												//state = isState0(state_reserve) ? 0 : 4;
												//state_reserve--;

												{
													if (braceNum < nestedUT.size()) {
														nestedUT[braceNum] = nullptr;
													}
													braceNum--;
												}
											}
											{
												//state = 4;
											}
										}
										else {
											state = 4;
										}
									}
								}
							}
						}
					}
					break;
					case 7:
					{
						while (i <= last_idx && strVec[i].isComment) {
							
							++i;
						}
						if (i >= last_idx + 1) {
							//end_state = true;
							//break;
						}
						if (!end_state) {
							auto top = strVec[i];
							if (top.len == 1 && -1 != Equal(option.Right, top.str[0])) {
								//Utility::Pop(strVec, nullptr, nestedUT[braceNum], option);
								//
								i += 1;
								while (i <= last_idx && strVec[i].isComment) {
									
									++i;
								}
								if (i >= last_idx + 1) {
									//end_state = true;
									//break;
								}

								if (!end_state) {
									if (braceNum == 0) {
										int utCount = 0;
										int itCount = 0;
										UserType ut;
										ut.AddUserTypeItem(UserType("#"));
										UserType* pTemp = nullptr;
										ut.GetLastUserTypeItemRef("#", pTemp);

										/// move data
										ut.GetUserTypeList(0)->ReserveIList(nestedUT[braceNum]->GetIListSize());
										ut.GetUserTypeList(0)->ReserveItemList(nestedUT[braceNum]->GetItemListSize());
										ut.GetUserTypeList(0)->ReserveUserTypeList(nestedUT[braceNum]->GetUserTypeListSize());

										for (int k = 0; k < nestedUT[braceNum]->GetIListSize(); ++k) {
											if (nestedUT[braceNum]->IsUserTypeList(k)) {
												ut.GetUserTypeList(0)->AddUserTypeItem(std::move(*nestedUT[braceNum]->GetUserTypeList(utCount)));
												//

												utCount++;
											}
											else {
												ut.GetUserTypeList(0)->AddItemList(std::move(nestedUT[braceNum]->GetItemList(itCount)));
												itCount++;
											}
										}
										nestedUT[braceNum]->Remove();
										nestedUT[braceNum]->AddUserTypeItem(std::move(*ut.GetUserTypeList(0)));


										braceNum++;
									}

									if (braceNum < nestedUT.size()) {
										nestedUT[braceNum] = nullptr;
									}
									braceNum--;
									//
									state = 4;
								}
							}
							else {
								int idx = -1;
								int num = -1;

								{
									/// uisng struct
									//state_reserve++;

									state = 4;
								}
								//else
								{
									//throw "syntax error 5 ";
								}
							}
						}
					}
					break;
					default:
						// syntax err!!

						throw "syntax error 6 ";
						break;
					}
				}

				if (next) {
					*next = nestedUT[braceNum];
				}
				if (state != last_state) { // 0 : empty or 4 : else?
					throw std::string("error final state is not last_state!  : ") + toStr(state);
				}
				if (i > last_idx + 1) {
					throw std::string("error i > last_idx + 1: " + toStr(i) + " , " + toStr(last_idx));
				}
				return true;
			}
			/*
			static bool __LoadData3_2(VECTOR<Token2>* _strVec, long long start_idx, long long last_idx, UserType* _global, const wiz::LoadDataOption* _option,
				int start_state, int last_state, UserType** next) // first, strVec.empty() must be true!!
			{
				if (start_idx > last_idx) {
					return false;
				}

				VECTOR<Token2>& strVec = *_strVec;
				UserType& global = *_global;
				const wiz::LoadDataOption& option = *_option;

				int state = start_state;
				int braceNum = 0;
				//long long state_reserve = 0;

				std::vector< UserType* > nestedUT(1);
				wiz::Token2 var1, var2, val;

				bool varOn = false;
				nestedUT[0] = &global;

				//	for (int i = 0; i < strVec.size(); ++i) {
				//		cout << strVec[i].str << endl;
				//	}
				long long i = start_idx;
				bool end_state = false;
				while (i <= last_idx && !end_state) {
					//

					switch (state)
					{
					case 0:
					{
						while (i <= last_idx && strVec[i].isComment) { // remove this part? has bug!

							++i;
						}
						if (i >= last_idx + 1) {
							//end_state = true;
							//
						}

						if (!end_state) {
							auto top = strVec[i];
							if (top.len == 1 && -1 != Equal(option.Left, top.str[0])) {
								state = 2;
							}
							else {
								std::pair<bool, Token2> bsPair;

								int tmp = i;
								++i;
								while (i <= last_idx && strVec[i].isComment) {

									++i;
								}
								if (i >= last_idx + 1) {
									//end_state = true;
									//
								}

								if (!end_state) {
									if (i <= last_idx) {
										bsPair = std::make_pair(true, strVec[i]);
									}
									else {
										bsPair = std::make_pair(false, Token2());
									}
									i = tmp;

									if (bsPair.first) {
										long long idx = -1;

										if (-1 != (idx = checkDelimiter(bsPair.second.str,
															bsPair.second.str + bsPair.second.len - 1, option.Assignment))) {
											var2 = strVec[i]; // Utility::Pop(strVec, &var2, nestedUT[braceNum], option);
																							  //Utility::Pop(strVec, nullptr, nestedUT[braceNum], option);
											state = 2;
											i++;

											//// pass comment??
											while (i <= last_idx && strVec[i].isComment) {

												++i;
											}
											if (i >= last_idx + 1) {
												//end_state = true;
												//
											}
											i++;
											while (i <= last_idx && strVec[i].isComment) {

												++i;
											}
											if (i >= last_idx + 1) {
												//end_state = true;
												//
											}

										}
										else {
											if (i <= last_idx) {
												//Utility::Pop(strVec, &var1, nestedUT[braceNum], option)) {
												
												////var1 = std::string(strVec[i].str, strVec[i].len);

												nestedUT[braceNum]->AddItem("", strVec[i]);
												state = 0;
												i += 1;
												while (i <= last_idx && strVec[i].isComment) {

													++i;
												}
												if (i >= last_idx + 1) {
													//end_state = true;
													//
												}
											}
										}
									}
									else {
										if (i <= last_idx) {
											//Utility::Pop(strVec, &var1, nestedUT[braceNum], option)) {
											//var1 = std::string(strVec[i].str, strVec[i].len);

											nestedUT[braceNum]->AddItem("", strVec[i]);
											i += 1;
											while (i <= last_idx && strVec[i].isComment) {

												++i;
											}
											if (i >= last_idx + 1) {
												//end_state = true;
												//
											}
											state = 0;
										}
									}
								}
							}
						}
					}
					break;
					case 1:
					{
						while (i <= last_idx && strVec[i].isComment) {

							++i;
						}
						if (i >= last_idx + 1) {
							//end_state = true;
							//
						}
						if (!end_state) {
							auto top = strVec[i];
							if (top.len == 1 && -1 != Equal(option.Right, top.str[0])) {
								//Utility::Pop(strVec, nullptr, nestedUT[braceNum], option);
								i += 1;
								while (i <= last_idx && strVec[i].isComment) {

									++i;
								}
								if (i >= last_idx + 1) {
									//end_state = true;
									//	break;
								}
								state = 0;
							}
							else {
								// syntax error.
								throw "syntax error 1 ";
							}
						}
					}
					break;
					case 2:
					{
						while (i <= last_idx && strVec[i].isComment) {

							++i;
						}
						if (i >= last_idx + 1) { //end_state = true; 
												 //break;
						}

						if (!end_state) {
							auto top = strVec[i];
							if (top.len == 1 && -1 != Equal(option.Left, top.str[0])) {
								//Utility::Pop(strVec, nullptr, nestedUT[braceNum], option);
								i += 1;
								while (i <= last_idx && strVec[i].isComment) {

									++i;
								}
								if (i >= last_idx + 1) {
									//end_state = true;
									//break;
								}
								if (!end_state) {
									///
									nestedUT[braceNum]->AddUserTypeItem(UserType(var2));
									UserType* pTemp = nullptr;
									nestedUT[braceNum]->GetLastUserTypeItemRef(var2, pTemp);

									braceNum++;

									/// new nestedUT
									if (nestedUT.size() == braceNum) /// changed 2014.01.23..
										nestedUT.push_back(nullptr);

									/// initial new nestedUT.
									nestedUT[braceNum] = pTemp;
									///
									state = 3;
								}
							}
							else {
								if (i <= last_idx) {
									//Utility::Pop(strVec, &val, nestedUT[braceNum], option)) {
									val = strVec[i]; // std::string(strVec[i].str, strVec[i].len);

									nestedUT[braceNum]->AddItem(var2, val);
									i += 1;
									while (i <= last_idx && strVec[i].isComment) {

										++i;
									}
									if (i >= last_idx + 1) {
										//end_state = true;
										//	break;
									}
									if (!end_state) {
										var2.clear();
										val.clear();

										state = 0;
									}
								}
							}
						}
					}
					break;
					case 3:
					{
						while (i <= last_idx && strVec[i].isComment) {

							++i;
						}
						if (i >= last_idx + 1) {
							//end_state = true;
							//	break;
						}

						if (!end_state) {
							auto top = strVec[i];
							if (top.len == 1 && -1 != Equal(option.Right, top.str[0])) {
								//Utility::Pop(strVec, nullptr, nestedUT[braceNum], option);
								i += 1;
								while (i <= last_idx && strVec[i].isComment) {

									++i;
								}
								if (i >= last_idx + 1) {
									//end_state = true;
									//	break;
								}

								if (braceNum == 0) {
									int utCount = 0;
									int itCount = 0;
									UserType ut;
									ut.AddUserTypeItem(UserType("#")); // option->LineComment[0]?
									UserType* pTemp = nullptr;
									ut.GetLastUserTypeItemRef("#", pTemp);

									/// move data
									ut.GetUserTypeList(0)->ReserveIList(nestedUT[braceNum]->GetIListSize());
									ut.GetUserTypeList(0)->ReserveItemList(nestedUT[braceNum]->GetItemListSize());
									ut.GetUserTypeList(0)->ReserveUserTypeList(nestedUT[braceNum]->GetUserTypeListSize());

									for (int k = 0; k < nestedUT[braceNum]->GetIListSize(); ++k) {
										if (nestedUT[braceNum]->IsUserTypeList(k)) {
											ut.GetUserTypeList(0)->AddUserTypeItem(std::move(*nestedUT[braceNum]->GetUserTypeList(utCount)));


											utCount++;
										}
										else {
											ut.GetUserTypeList(0)->AddItemList(std::move(nestedUT[braceNum]->GetItemList(itCount)));
											itCount++;
										}
									}
									nestedUT[braceNum]->Remove();
									nestedUT[braceNum]->AddUserTypeItem(std::move(*ut.GetUserTypeList(0)));

									braceNum++;
								}

								if (braceNum < nestedUT.size()) {
									nestedUT[braceNum] = nullptr;
								}
								braceNum--;

								state = 0;
							}
							else {
								{
									/// uisng struct
									//state_reserve++;
									state = 4;
								}
								//else
								{
									//	throw  "syntax error 2 ";
								}
							}
						}
					}
					break;
					case 4:
					{
						while (i <= last_idx && strVec[i].isComment) {

							++i;
						}
						if (i >= last_idx + 1) { //end_state = true; 
												 //	break;
						}
						if (!end_state) {
							auto top = strVec[i];

							if (top.len == 1 && -1 != Equal(option.Left, top.str[0])) {
								//Utility::Pop(strVec, nullptr, nestedUT[braceNum], option);
								i += 1;
								while (i <= last_idx && strVec[i].isComment) {

									++i;
								}
								if (i >= last_idx + 1) {
									//end_state = true;
									//	break;
								}

								if (!end_state) {
									UserType temp("");

									nestedUT[braceNum]->AddUserTypeItem(temp);
									UserType* pTemp = nullptr;
									nestedUT[braceNum]->GetLastUserTypeItemRef("", pTemp);

									braceNum++;

									/// new nestedUT
									if (nestedUT.size() == braceNum) /// changed 2014.01.23..
										nestedUT.push_back(nullptr);

									/// initial new nestedUT.
									nestedUT[braceNum] = pTemp;
									///
									//}


									state = 5;
								}
							}
							else if (top.len == 1 && -1 != Equal(option.Right, top.str[0])) {
								//Utility::Pop(strVec, nullptr, nestedUT[braceNum], option);
								i += 1;
								while (i <= last_idx && strVec[i].isComment) {

									++i;
								}
								if (i >= last_idx + 1) {
									//end_state = true;
									//	break;
								}

								if (!end_state) {
									state = 4;
									//state = isState0(state_reserve) ? 0 : 4;
									//state_reserve--;


									if (braceNum == 0) {
										int utCount = 0;
										int itCount = 0;
										UserType ut;
										ut.AddUserTypeItem(UserType("#"));
										UserType* pTemp = nullptr;
										ut.GetLastUserTypeItemRef("#", pTemp);

										/// move data
										ut.GetUserTypeList(0)->ReserveIList(nestedUT[braceNum]->GetIListSize());
										ut.GetUserTypeList(0)->ReserveItemList(nestedUT[braceNum]->GetItemListSize());
										ut.GetUserTypeList(0)->ReserveUserTypeList(nestedUT[braceNum]->GetUserTypeListSize());

										for (int k = 0; k < nestedUT[braceNum]->GetIListSize(); ++k) {
											if (nestedUT[braceNum]->IsUserTypeList(k)) {
												ut.GetUserTypeList(0)->AddUserTypeItem(std::move(*nestedUT[braceNum]->GetUserTypeList(utCount)));

												utCount++;
											}
											else {
												ut.GetUserTypeList(0)->AddItemList(std::move(nestedUT[braceNum]->GetItemList(itCount)));
												itCount++;
											}
										}

										nestedUT[braceNum]->Remove();
										nestedUT[braceNum]->AddUserTypeItem(std::move(*ut.GetUserTypeList(0)));

										braceNum++;
									}

									{
										if (braceNum < nestedUT.size()) {
											nestedUT[braceNum] = nullptr;
										}
										braceNum--;
									}
								}
							}
							else {
								int temp = i;
								++i;
								while (i <= last_idx && strVec[i].isComment) {

									++i;
								}
								if (i >= last_idx + 1) {
									//end_state = true;
									//	break;
								}

								if (!end_state) {
									std::pair<bool, Token2> bsPair;

									if (i <= last_idx)
									{
										bsPair = std::make_pair(true, strVec[i]);
									}
									else {
										bsPair = std::make_pair(false, Token2());
									}
									i = temp;

									if (bsPair.first) {
										long long idx = -1;

										if (-1 != (idx = checkDelimiter(bsPair.second.str,
															bsPair.second.str + bsPair.second.len - 1, option.Assignment))) {
											// var2
											//Utility::Pop(strVec, &var2, nestedUT[braceNum], option);
											//Utility::Pop(strVec, nullptr, nestedUT[braceNum], option); // pass EQ_STR
											var2 = strVec[i]; // std::string(strVec[i].str, strVec[i].len);
											state = 6;
											i += 1;
											while (i <= last_idx && strVec[i].isComment) {

												++i;
											}
											if (i >= last_idx + 1) {
												//end_state = true;
												//	break;
											}
											i += 1;
											while (i <= last_idx && strVec[i].isComment) {

												++i;
											}
											if (i >= last_idx + 1) {
												//end_state = true;
												//	break;
											}
										}
										else {
											// var1
											if (i <= last_idx) {
												//Utility::Pop(strVec, &var1, nestedUT[braceNum], option)) {
												var1 = strVec[i]; // std::string(strVec[i].str, strVec[i].len);

												nestedUT[braceNum]->AddItem("", var1);
												var1.clear();// = "";

												state = 4;
												i += 1;
												while (i <= last_idx && strVec[i].isComment) {

													++i;
												}
												if (i >= last_idx + 1) {
													//end_state = true;
													//	break;
												}
											}
										}
									}
								}
								else
								{
									// var1
									if (i <= last_idx) //Utility::Pop(strVec, &var1, nestedUT[braceNum], option))
									{
										var1 = strVec[i]; // (std::string(strVec[i].str, strVec[i].len));
										nestedUT[braceNum]->AddItem("", var1);
										var1.clear();

										state = 4;
										i += 1;
										while (i <= last_idx && strVec[i].isComment) {

											++i;
										}
										if (i >= last_idx + 1) {
											//end_state = true;
											//	break;
										}
									}
								}
							}
						}
					}
					break;
					case 5:
					{
						while (i <= last_idx && strVec[i].isComment) {

							++i;
						}
						if (i >= last_idx + 1) {
							//end_state = true;
							//break;
						}

						if (!end_state) {
							auto top = strVec[i];
							if (top.len == 1 && -1 != Equal(option.Right, top.str[0])) {
								//Utility::Pop(strVec, nullptr, nestedUT[braceNum], option);

								if (braceNum == 0) {
									int utCount = 0;
									int itCount = 0;
									UserType ut;
									ut.AddUserTypeItem(UserType("#"));
									UserType* pTemp = nullptr;
									ut.GetLastUserTypeItemRef("#", pTemp);

									/// move data
									ut.GetUserTypeList(0)->ReserveIList(nestedUT[braceNum]->GetIListSize());
									ut.GetUserTypeList(0)->ReserveItemList(nestedUT[braceNum]->GetItemListSize());
									ut.GetUserTypeList(0)->ReserveUserTypeList(nestedUT[braceNum]->GetUserTypeListSize());

									for (int k = 0; k < nestedUT[braceNum]->GetIListSize(); ++k) {
										if (nestedUT[braceNum]->IsUserTypeList(k)) {
											ut.GetUserTypeList(0)->AddUserTypeItem(std::move(*nestedUT[braceNum]->GetUserTypeList(utCount)));
											//

											utCount++;
										}
										else {
											ut.GetUserTypeList(0)->AddItemList(std::move(nestedUT[braceNum]->GetItemList(itCount)));
											itCount++;
										}
									}
									nestedUT[braceNum]->Remove();
									nestedUT[braceNum]->AddUserTypeItem(std::move(*ut.GetUserTypeList(0)));

									braceNum++;
								}

								//if (flag1 == 0) {
								if (braceNum < nestedUT.size()) {
									nestedUT[braceNum] = nullptr;
								}
								braceNum--;
								// }
								//
								state = 4;
								i += 1;
								while (i <= last_idx && strVec[i].isComment) {

									++i;
								}
								if (i >= last_idx + 1) {
									//end_state = true;
									//	break;
								}
							}
							else {
								int idx = -1;
								int num = -1;


								{
									/// uisng struct
									//state_reserve++;
									state = 4;
								}
								//else
								{
									//	throw "syntax error 4  ";
								}
							}
						}
					}
					break;
					case 6:
					{
						while (i <= last_idx && strVec[i].isComment) {

							++i;
						}
						if (i >= last_idx + 1) {
							//end_state = true;
							//break;
						}

						if (!end_state) {
							std::string top = std::string(strVec[i].str, strVec[i].len);

							if (top.size() == 1 && -1 != Equal(option.Left, top[0])) {
								//Utility::Pop(strVec, nullptr, nestedUT[braceNum], option);
								i += 1;
								while (i <= last_idx && strVec[i].isComment) {

									++i;
								}
								if (i >= last_idx + 1) {
									//end_state = true;
									//	break;
								}
								if (!end_state) {
									nestedUT[braceNum]->AddUserTypeItem(UserType(var2));
									UserType* pTemp = nullptr;
									nestedUT[braceNum]->GetLastUserTypeItemRef(var2, pTemp);
									var2.clear();
									braceNum++;

									/// new nestedUT
									if (nestedUT.size() == braceNum) /// changed 2014.01.23..
										nestedUT.push_back(nullptr);

									/// initial new nestedUT.
									nestedUT[braceNum] = pTemp;


								}
								///
								state = 7;
							}
							else {
								if (i <= last_idx) { //Utility::Pop(strVec, &val, nestedUT[braceNum], option)) {
									val = strVec[i]; // (std::string(strVec[i].str, strVec[i].len));

									i += 1;
									while (i <= last_idx && strVec[i].isComment) {

										++i;
									}
									if (i >= last_idx + 1) {
										//end_state = true;
										//	break;
									}
									if (!end_state) {
										nestedUT[braceNum]->AddItem(var2, val);
										var2.clear(); val.clear();

										top = std::string(strVec[i].str, strVec[i].len);

										if (strVec.empty())
										{
											//
										}
										else if (top.size() == 1 && -1 != Equal(option.Right, top[0])) {
											//Utility::Pop(strVec, nullptr, nestedUT[braceNum], option);
											i += 1;
											while (i <= last_idx && strVec[i].isComment) {

												++i;
											}
											if (i >= last_idx + 1) {
												//end_state = true;
												//	break;
											}
											if (!end_state) {
												if (braceNum == 0) {
													int utCount = 0;
													int itCount = 0;
													UserType ut;
													ut.AddUserTypeItem(UserType("#"));
													UserType* pTemp = nullptr;
													ut.GetLastUserTypeItemRef("#", pTemp);

													/// move data
													ut.GetUserTypeList(0)->ReserveIList(nestedUT[braceNum]->GetIListSize());
													ut.GetUserTypeList(0)->ReserveItemList(nestedUT[braceNum]->GetItemListSize());
													ut.GetUserTypeList(0)->ReserveUserTypeList(nestedUT[braceNum]->GetUserTypeListSize());

													for (int k = 0; k < nestedUT[braceNum]->GetIListSize(); ++k) {
														if (nestedUT[braceNum]->IsUserTypeList(k)) {
															ut.GetUserTypeList(0)->AddUserTypeItem(std::move(*nestedUT[braceNum]->GetUserTypeList(utCount)));
															//	//


															utCount++;
														}
														else {
															ut.GetUserTypeList(0)->AddItemList(std::move(nestedUT[braceNum]->GetItemList(itCount)));
															itCount++;
														}
													}
													nestedUT[braceNum]->Remove();
													nestedUT[braceNum]->AddUserTypeItem(std::move(*ut.GetUserTypeList(0)));


													braceNum++;
												}
											}
											{
												state = 4;
												//state = isState0(state_reserve) ? 0 : 4;
												//state_reserve--;

												{
													if (braceNum < nestedUT.size()) {
														nestedUT[braceNum] = nullptr;
													}
													braceNum--;
												}
											}
											{
												//state = 4;
											}
										}
										else {
											state = 4;
										}
									}
								}
							}
						}
					}
					break;
					case 7:
					{
						while (i <= last_idx && strVec[i].isComment) {

							++i;
						}
						if (i >= last_idx + 1) {
							//end_state = true;
							//break;
						}
						if (!end_state) {
							auto top = strVec[i];
							if (top.len == 1 && -1 != Equal(option.Right, top.str[0])) {
								//Utility::Pop(strVec, nullptr, nestedUT[braceNum], option);
								//
								i += 1;
								while (i <= last_idx && strVec[i].isComment) {

									++i;
								}
								if (i >= last_idx + 1) {
									//end_state = true;
									//break;
								}

								if (!end_state) {
									if (braceNum == 0) {
										int utCount = 0;
										int itCount = 0;
										UserType ut;
										ut.AddUserTypeItem(UserType("#"));
										UserType* pTemp = nullptr;
										ut.GetLastUserTypeItemRef("#", pTemp);

										/// move data
										ut.GetUserTypeList(0)->ReserveIList(nestedUT[braceNum]->GetIListSize());
										ut.GetUserTypeList(0)->ReserveItemList(nestedUT[braceNum]->GetItemListSize());
										ut.GetUserTypeList(0)->ReserveUserTypeList(nestedUT[braceNum]->GetUserTypeListSize());

										for (int k = 0; k < nestedUT[braceNum]->GetIListSize(); ++k) {
											if (nestedUT[braceNum]->IsUserTypeList(k)) {
												ut.GetUserTypeList(0)->AddUserTypeItem(std::move(*nestedUT[braceNum]->GetUserTypeList(utCount)));
												//

												utCount++;
											}
											else {
												ut.GetUserTypeList(0)->AddItemList(std::move(nestedUT[braceNum]->GetItemList(itCount)));
												itCount++;
											}
										}
										nestedUT[braceNum]->Remove();
										nestedUT[braceNum]->AddUserTypeItem(std::move(*ut.GetUserTypeList(0)));


										braceNum++;
									}

									if (braceNum < nestedUT.size()) {
										nestedUT[braceNum] = nullptr;
									}
									braceNum--;
									//
									state = 4;
								}
							}
							else {
								int idx = -1;
								int num = -1;

								{
									/// uisng struct
									//state_reserve++;

									state = 4;
								}
								//else
								{
									//throw "syntax error 5 ";
								}
							}
						}
					}
					break;
					default:
						// syntax err!!

						throw "syntax error 6 ";
						break;
					}
				}

				if (next) {
					*next = nestedUT[braceNum];
				}
				if (state != last_state) { // 0 : empty or 4 : else?
					throw std::string("error final state is not last_state!  : ") + toStr(state);
				}
				if (i > last_idx + 1) {
					throw std::string("error i > last_idx + 1: " + toStr(i) + " , " + toStr(last_idx));
				}
				return true;
			}
			*/
			template <class Reserver>
			static bool _LoadData3(VECTOR<Token2>& strVec, Reserver& reserver, UserType& global, const wiz::LoadDataOption& option, const int pivot_num = 3, const int lex_thread_num = 4) // first, strVec.empty() must be true!!
			{
				char left[] = "{";
				char right[] = "}";

				strVec.emplace_back(left, 1, false);
				strVec.emplace_back(left, 1, false);

				char* buffer = nullptr;
				bool end = false;
				{
					end = !reserver(&strVec, option, lex_thread_num, buffer);

					while (strVec.empty())
					{
						end = !reserver(&strVec, option, lex_thread_num, buffer);
						if (
							strVec.empty() &&
							end
							) {
							return false; // throw "Err nextToken does not exist"; // cf) or empty file or empty std::string!
						}
					}
				}

				UserType* before_next = nullptr;
				UserType _global(global.GetName());

				bool first = true;
				long long sum = 0;

				while (true) {
					end = !reserver(&strVec, option, lex_thread_num, buffer);

					if (end) {
						strVec.emplace_back(right, 1, false);
						strVec.emplace_back(right, 1, false);
					}

					int a = clock();


					std::set<int> _pivots;
					std::vector<int> pivots;
					const int last_idx = FindRight(strVec, 0, strVec.size() - 1, option);

					if (pivot_num > 0 && last_idx != -1) {
						//int c1 = clock();
						std::vector<int> pivot;

						for (int i = 0; i < pivot_num; ++i) {
							pivot.push_back(FindRight(strVec, (last_idx / (pivot_num + 1)) * (i), (last_idx / (pivot_num + 1)) * (i + 1) - 1, option));
						}

						for (int i = 0; i < pivot.size(); ++i) {
							if (pivot[i] != -1) {
								_pivots.insert(pivot[i]);
							}
						}

						//std::cout << _pivots.size() << std::endl;
						for (auto& x : _pivots) {
							pivots.push_back(x);
							//	std::cout << x << " ";
						}
						//std::cout << last_idx << std::endl;
						//std::cout << std::endl;
					}


					//std::cout << pivot << "\t" << pivot3 << "\t" << pivot2 << "\t" << last_idx << "\t" << strVec.size() - 1 << std::endl;
					//std::cout << strVec[pivot + 1].str << "\t";
					//std::cout << strVec[pivot3 + 1].str << "\t";
					//std::cout << strVec[pivot2 + 1].str << "\t";
					//std::cout << std::endl;


					std::vector<UserType*> next(pivots.size() + 1, nullptr);

					{
						std::vector<UserType> __global(pivots.size() + 1);

						std::vector<std::thread> thr(pivots.size() + 1);

						
						if (first) {
							int idx = pivots.empty() ? last_idx : pivots[0];
							if (-1 == last_idx) {
								idx = strVec.size() - 1;
							}
							thr[0] = std::thread(__LoadData3, &strVec, 0, idx, &__global[0], &option, 0, 4, &next[0]);
						}
						else {
							int idx = pivots.empty() ? last_idx : pivots[0];
							if (-1 == last_idx) {
								idx = strVec.size() - 1;
							}
							thr[0] = std::thread(__LoadData3, &strVec, 0, idx, &__global[0], &option, 4, 4, &next[0]);
						}

						for (int i = 1; i < pivots.size(); ++i) {
							thr[i] = std::thread(__LoadData3, &strVec, pivots[i - 1] + 1, pivots[i], &__global[i], &option, 4, 4, &next[i]);
						}

						if (pivots.size() >= 1) {
							thr[pivots.size()] = std::thread(__LoadData3, &strVec, pivots.back() + 1, last_idx, &__global[pivots.size()],
								&option, 4, 4, &next[pivots.size()]);
						}

						// wait
						for (int i = 0; i < thr.size(); ++i) {
							thr[i].join();
						}

						//Merge
						//int merge_start = clock();

						if (first) {
							Merge(&_global, &__global[0], &next[0]);
							first = false;
						}
						else {
							Merge(before_next, &__global[0], &next[0]);
						}

						for (int i = 1; i < pivots.size() + 1; ++i) {
							Merge(next[i - 1], &__global[i], &next[i]);
						}

						//std::cout << "merge " << clock() - merge_start << "ms" << std::endl;

						//int pop_start = clock();
						//
						//for (int i = 0; i <= last_idx; ++i) {
						//	strVec.pop_front();
						//}

						if (last_idx == -1) {
							strVec.erase(strVec.begin(), strVec.end());
						}
						else {
							strVec.erase(strVec.begin(), strVec.begin() + last_idx + 1);
						}
					

						//std::cout << "pop " << clock() - pop_start << "ms" << std::endl;

						before_next = next.back();
					}

					int b = clock();
					sum += std::chrono::steady_clock::duration(b - a).count(); //std::cout << b - a << "ms" << std::endl;

					if (!end) {

						end = !reserver(&strVec, option, lex_thread_num, buffer);

						while (!end && strVec.empty())
						{
							end = !reserver(&strVec, option, lex_thread_num, buffer);
							if (
								strVec.empty() &&
								end
								) {
								break; // throw "Err nextToken does not exist"; // cf) or empty file or empty std::string!
							}
						}
					}
					else {
						break;
					}
				}

				global = std::move(*_global.GetUserTypeList(0)->GetUserTypeList(0));

				if (buffer) {
					delete[] buffer;
					buffer = nullptr;
				}
				//// todo - option?
				//std::cout << "lexing " << log_result << "ms" << std::endl;
				//log_result = 0;
				//std::cout << "parse " << sum << "ms" << std::endl;
				return true;
			}
		
			/*
			template <class Reserver>
			static bool _LoadData3_2(char*& outBuffer, VECTOR<Token2>& strVec, Reserver& reserver, UserType& global, const wiz::LoadDataOption& option, const int pivot_num = 3, const int lex_thread_num = 4) // first, strVec.empty() must be true!!
			{
				char left[] = "{";
				char right[] = "}";

				strVec.emplace_back(left, 1, false);
				strVec.emplace_back(left, 1, false);

				char* buffer = nullptr;
				bool end = false;
				{
					end = !reserver(&strVec, option, lex_thread_num, buffer);

					while (strVec.empty())
					{
						end = !reserver(&strVec, option, lex_thread_num, buffer);
						if (
							strVec.empty() &&
							end
							) {
							return false; // throw "Err nextToken does not exist"; // cf) or empty file or empty std::string!
						}
					}
				}

				UserType* before_next = nullptr;
				UserType _global;

				bool first = true;
				int sum = 0;

				while (true) {
					end = !reserver(&strVec, option, lex_thread_num, buffer);

					if (end) {
						strVec.emplace_back(right, 1, false);
						strVec.emplace_back(right, 1, false);
					}

					int a = clock();


					std::set<int> _pivots;
					std::vector<int> pivots;
					const int last_idx = FindRight(strVec, 0, strVec.size() - 1, option);

					if (pivot_num > 0) {
						//int c1 = clock();
						std::vector<int> pivot;

						for (int i = 0; i < pivot_num; ++i) {
							pivot.push_back(FindRight(strVec, (last_idx / (pivot_num + 1)) * (i), (last_idx / (pivot_num + 1)) * (i + 1) - 1, option));
						}

						for (int i = 0; i < pivot.size(); ++i) {
							if (pivot[i] != -1) {
								_pivots.insert(pivot[i]);
							}
						}

						//std::cout << _pivots.size() << std::endl;
						for (auto& x : _pivots) {
							pivots.push_back(x);
							//	std::cout << x << " ";
						}
						//std::cout << last_idx << std::endl;
						//std::cout << std::endl;
					}


					//std::cout << pivot << "\t" << pivot3 << "\t" << pivot2 << "\t" << last_idx << "\t" << strVec.size() - 1 << std::endl;
					//std::cout << strVec[pivot + 1].str << "\t";
					//std::cout << strVec[pivot3 + 1].str << "\t";
					//std::cout << strVec[pivot2 + 1].str << "\t";
					//std::cout << std::endl;


					std::vector<UserType*> next(pivots.size() + 1, nullptr);

					{
						std::vector<UserType> __global(pivots.size() + 1);

						std::vector<std::thread> thr(pivots.size() + 1);


						if (first) {
							int idx = pivots.empty() ? last_idx : pivots[0];
							thr[0] = std::thread(__LoadData3_2, &strVec, 0, idx, &__global[0], &option, 0, 4, &next[0]);
						}
						else {
							int idx = pivots.empty() ? last_idx : pivots[0];
							thr[0] = std::thread(__LoadData3_2, &strVec, 0, idx, &__global[0], &option, 4, 4, &next[0]);
						}

						for (int i = 1; i < pivots.size(); ++i) {
							thr[i] = std::thread(__LoadData3_2, &strVec, pivots[i - 1] + 1, pivots[i], &__global[i], &option, 4, 4, &next[i]);
						}

						if (pivots.size() >= 1) {
							thr[pivots.size()] = std::thread(__LoadData3_2, &strVec, pivots.back() + 1, last_idx, &__global[pivots.size()],
								&option, 4, 4, &next[pivots.size()]);
						}

						// wait
						for (int i = 0; i < thr.size(); ++i) {
							thr[i].join();
						}

						//Merge
						//int merge_start = clock();

						if (first) {
							Merge(&_global, &__global[0], &next[0]);
							first = false;
						}
						else {
							Merge(before_next, &__global[0], &next[0]);
						}

						for (int i = 1; i < pivots.size() + 1; ++i) {
							Merge(next[i - 1], &__global[i], &next[i]);
						}

						//std::cout << "merge " << clock() - merge_start << "ms" << std::endl;

						//int pop_start = clock();
						//
						//for (int i = 0; i <= last_idx; ++i) {
						//	strVec.pop_front();
						//}

						strVec.erase(strVec.begin(), strVec.begin() + last_idx + 1);

						//std::cout << "pop " << clock() - pop_start << "ms" << std::endl;

						before_next = next.back();
					}

					int b = clock();
					sum += std::chrono::steady_clock::duration(b - a).count(); //std::cout << b - a << "ms" << std::endl;

					if (!end) {

						end = !reserver(&strVec, option, lex_thread_num, buffer);

						while (!end && strVec.empty())
						{
							end = !reserver(&strVec, option, lex_thread_num, buffer);
							if (
								strVec.empty() &&
								end
								) {
								break; // throw "Err nextToken does not exist"; // cf) or empty file or empty std::string!
							}
						}
					}
					else {
						break;
					}
				}

				global = std::move(*_global.GetUserTypeList(0)->GetUserTypeList(0));
				//
				//delete[] buffer;
				outBuffer = buffer;

				//// todo - option?
				//std::cout << "lexing " << log_result << "ms" << std::endl;
				//				log_result = 0;
				//std::cout << "parse " << sum << "ms" << std::endl;
				return true;
			}
			*/

			// from New Json Parser Project.
			static bool __LoadData4(VECTOR<Token2>* _strVec, int start_idx, int last_idx, UserType* _global, const wiz::LoadDataOption* _option,
				int start_state, int last_state, UserType** next) // first, strVec.empty() must be true!!
			{
				if (start_idx > last_idx) {
					return false;
				}

				VECTOR<Token2>& strVec = *_strVec;
				UserType& global = *_global;
				const wiz::LoadDataOption& option = *_option;

				int state = start_state;
				int braceNum = 0;
				std::vector< UserType* > nestedUT(1);
				std::string var, val;


				nestedUT[0] = &global;

				int i = start_idx;

				while (false == strVec.empty() && i <= last_idx) {
					switch (state)
					{
					case 0:
					{
						if (strVec[i].len == 1 && -1 != Equal(option.Left, strVec[i].str[0])) {
							i += 1;
							UserType temp("");

							nestedUT[braceNum]->AddUserTypeItem(temp);
							UserType* pTemp = nullptr;
							nestedUT[braceNum]->GetLastUserTypeItemRef("", pTemp);

							braceNum++;

							/// new nestedUT
							if (nestedUT.size() == braceNum) { /// changed 2014.01.23..
								nestedUT.push_back(nullptr);
							}

							/// initial new nestedUT.
							nestedUT[braceNum] = pTemp;
							///

							state = 0;
						}
						else if (strVec[i].len == 1 && -1 != Equal(option.Right, strVec[i].str[0])) {
							i += 1;

							state = 0;

							if (braceNum == 0) {
								UserType ut;
								ut.AddUserTypeItem(UserType("#")); // json -> "var_name" = val  // clautext, # is line comment delimiter.
								UserType* pTemp = nullptr;
								ut.GetLastUserTypeItemRef("#", pTemp);
								int utCount = 0;
								int itCount = 0;
								auto max = nestedUT[braceNum]->GetIListSize();
								for (auto i = 0; i < max; ++i) {
									if (nestedUT[braceNum]->IsUserTypeList(i)) {
										ut.GetUserTypeList(0)->AddUserTypeItem(std::move(*(nestedUT[braceNum]->GetUserTypeList(utCount))));
										utCount++;
									}
									else {
										ut.GetUserTypeList(0)->AddItemList(std::move(nestedUT[braceNum]->GetItemList(itCount)));
										itCount++;
									}
								}

								nestedUT[braceNum]->Remove();
								nestedUT[braceNum]->AddUserTypeItem(std::move(*(ut.GetUserTypeList(0))));

								braceNum++;
							}

							{
								if (braceNum < nestedUT.size()) {
									nestedUT[braceNum] = nullptr;
								}
								braceNum--;
							}
						}
						else {
							std::pair<bool, Token2> bsPair;

							if (i < last_idx)
							{
								bsPair = std::make_pair(true, strVec[i + 1]);
							}
							else {
								bsPair = std::make_pair(false, Token2());
							}

							if (bsPair.first) {
								if (bsPair.second.len == 1 && -1 != Equal(option.Assignment, bsPair.second.str[0])) {
									// var2
									var = std::string(strVec[i].str, strVec[i].len);
									state = 1;
									i += 1;
									i += 1;
								}
								else {
									// var1
									if (i <= last_idx) {
										val = std::string(strVec[i].str, strVec[i].len);

										nestedUT[braceNum]->AddItem("", std::move(val));
										val = "";

										state = 0;
										i += 1;
									}
								}
							}
							else
							{
								// var1
								if (i <= last_idx)
								{
									val = std::string(strVec[i].str, strVec[i].len);
									nestedUT[braceNum]->AddItem("", std::move(val));
									val = "";

									state = 0;
									i += 1;
								}
							}
						}
					}
					break;
					case 1:
					{
						if (strVec[i].len == 1 && -1 != Equal(option.Left, strVec[i].str[0])) {
							i += 1;
							///
							{
								nestedUT[braceNum]->AddUserTypeItem(UserType(var));
								UserType* pTemp = nullptr;
								nestedUT[braceNum]->GetLastUserTypeItemRef(var, pTemp);
								var = "";
								braceNum++;

								/// new nestedUT
								if (nestedUT.size() == braceNum) {
									nestedUT.push_back(nullptr);
								}

								/// initial new nestedUT.
								nestedUT[braceNum] = pTemp;
							}
							///
							state = 0;
						}
						else {
							if (i <= last_idx) {
								val = std::string(strVec[i].str, strVec[i].len);

								i += 1;
								nestedUT[braceNum]->AddItem(std::move(var), std::move(val));
								var = ""; val = "";

								state = 0;
							}
						}
					}
					break;
					default:
						// syntax err!!
						throw "syntax error ";
						break;
					}
				}

				if (next) {
					*next = nestedUT[braceNum];
				}

				if (state != last_state) {
					throw std::string("error final state is not last_state!  : ") + toStr(state);
				}
				if (i > last_idx + 1) {
					throw std::string("error i > last_idx + 1: " + toStr(i) + " , " + toStr(last_idx));
				}
				return true;
			}
		
			static bool __LoadData5(VECTOR<Token2>* _strVec, int start_idx, int last_idx, UserType* _global, const wiz::LoadDataOption* _option,
				int start_state, int last_state, UserType** next) // first, strVec.empty() must be true!!
			{
				std::vector<std::string> varVec;
				std::vector<std::string> valVec;

				if (start_idx > last_idx) {
					return false;
				}

				VECTOR<Token2>& strVec = *_strVec;
				UserType& global = *_global;
				const wiz::LoadDataOption& option = *_option;

				int state = start_state;
				int braceNum = 0;
				std::vector< UserType* > nestedUT(1);
				std::string var, val;


				nestedUT[0] = &global;

				int i = start_idx;

				while (false == strVec.empty() && i <= last_idx) {
					switch (state)
					{
					case 0:
					{
						if (strVec[i].len == 1 && -1 != Equal(option.Left, strVec[i].str[0])) {
							i += 1;

							nestedUT[braceNum]->ReserveIList(nestedUT[braceNum]->GetIListSize() + varVec.size());
							nestedUT[braceNum]->ReserveItemList(nestedUT[braceNum]->GetItemListSize() + varVec.size());

							for (int i = 0; i < varVec.size(); ++i) {
								nestedUT[braceNum]->AddItem(varVec[i], valVec[i]);
							}
							varVec.clear();
							valVec.clear();


							UserType temp("");

							nestedUT[braceNum]->AddUserTypeItem(temp);
							UserType* pTemp = nullptr;
							nestedUT[braceNum]->GetLastUserTypeItemRef("", pTemp);

							braceNum++;

							/// new nestedUT
							if (nestedUT.size() == braceNum) { /// changed 2014.01.23..
								nestedUT.push_back(nullptr);
							}

							/// initial new nestedUT.
							nestedUT[braceNum] = pTemp;
							///

							state = 0;
						}
						else if (strVec[i].len == 1 && -1 != Equal(option.Right, strVec[i].str[0])) {
							i += 1;

							state = 0;

							// UserTypeListsize?
							nestedUT[braceNum]->ReserveIList(nestedUT[braceNum]->GetIListSize() + varVec.size());
							nestedUT[braceNum]->ReserveItemList(nestedUT[braceNum]->GetItemListSize() + varVec.size());

							for (int i = 0; i < varVec.size(); ++i) {
								nestedUT[braceNum]->AddItem(varVec[i], valVec[i]);
							}
							varVec.clear();
							valVec.clear();

							if (braceNum == 0) {
								UserType ut;
								ut.AddUserTypeItem(UserType("#")); // json -> "var_name" = val  // clautext, # is line comment delimiter.
								UserType* pTemp = nullptr;
								ut.GetLastUserTypeItemRef("#", pTemp);
								int utCount = 0;
								int itCount = 0;
								auto max = nestedUT[braceNum]->GetIListSize();
								for (auto i = 0; i < max; ++i) {
									if (nestedUT[braceNum]->IsUserTypeList(i)) {
										ut.GetUserTypeList(0)->AddUserTypeItem(std::move(*(nestedUT[braceNum]->GetUserTypeList(utCount))));
										utCount++;
									}
									else {
										ut.GetUserTypeList(0)->AddItemList(std::move(nestedUT[braceNum]->GetItemList(itCount)));
										itCount++;
									}
								}

								nestedUT[braceNum]->Remove();
								nestedUT[braceNum]->AddUserTypeItem(std::move(*(ut.GetUserTypeList(0))));

								braceNum++;
							}

							{
								if (braceNum < nestedUT.size()) {
									nestedUT[braceNum] = nullptr;
								}
								braceNum--;
							}
						}
						else {
							std::pair<bool, Token2> bsPair;

							if (i < last_idx)
							{
								bsPair = std::make_pair(true, strVec[i + 1]);
							}
							else {
								bsPair = std::make_pair(false, Token2());
							}

							if (bsPair.first) {
								if (bsPair.second.len == 1 && -1 != Equal(option.Assignment, bsPair.second.str[0])) {
									// var2
									var = std::string(strVec[i].str, strVec[i].len);
									state = 1;
									i += 1;
									i += 1;
								}
								else {
									// var1
									if (i <= last_idx) {
										val = std::string(strVec[i].str, strVec[i].len);

										varVec.push_back(var);
										valVec.push_back(val);
										//nestedUT[braceNum]->AddItem("", ""); // std::move(val));
										val = "";

										state = 0;
										i += 1;
									}
								}
							}
							else
							{
								// var1
								if (i <= last_idx)
								{
									val = std::string(strVec[i].str, strVec[i].len);
									varVec.push_back(var);
									valVec.push_back(val);
									//nestedUT[braceNum]->AddItem("", "");// std::move(val));
									val = "";

									state = 0;
									i += 1;
								}
							}
						}
					}
					break;
					case 1:
					{
						if (strVec[i].len == 1 && -1 != Equal(option.Left, strVec[i].str[0])) {
							i += 1;
							nestedUT[braceNum]->ReserveIList(nestedUT[braceNum]->GetIListSize() + varVec.size());
							nestedUT[braceNum]->ReserveItemList(nestedUT[braceNum]->GetItemListSize() + varVec.size());

							for (int i = 0; i < varVec.size(); ++i) {
								nestedUT[braceNum]->AddItem(varVec[i], valVec[i]);
							}
							varVec.clear();
							valVec.clear();


							///
							{
								nestedUT[braceNum]->AddUserTypeItem(UserType(var));
								UserType* pTemp = nullptr;
								nestedUT[braceNum]->GetLastUserTypeItemRef(var, pTemp);
								var = "";
								braceNum++;

								/// new nestedUT
								if (nestedUT.size() == braceNum) {
									nestedUT.push_back(nullptr);
								}

								/// initial new nestedUT.
								nestedUT[braceNum] = pTemp;
							}
							///
							state = 0;
						}
						else {
							if (i <= last_idx) {
								val = std::string(strVec[i].str, strVec[i].len);

								i += 1;
								varVec.push_back(var);
								valVec.push_back(val);
								//nestedUT[braceNum]->AddItem("", ""); // std::move(var), std::move(val));
								var = ""; val = "";

								state = 0;
							}
						}
					}
					break;
					default:
						// syntax err!!
						throw "syntax error ";
						break;
					}
				}

				if (next) {
					*next = nestedUT[braceNum];
				}

				if (varVec.empty() == false) {
					nestedUT[braceNum]->ReserveIList(nestedUT[braceNum]->GetIListSize() + varVec.size());
					nestedUT[braceNum]->ReserveItemList(nestedUT[braceNum]->GetItemListSize() + varVec.size());

					for (int i = 0; i < varVec.size(); ++i) {
						nestedUT[braceNum]->AddItem(varVec[i], valVec[i]);
					}
					varVec.clear();
					valVec.clear();
				}

				if (state != last_state) {
					throw std::string("error final state is not last_state!  : ") + toStr(state);
				}
				if (i > last_idx + 1) {
					throw std::string("error i > last_idx + 1: " + toStr(i) + " , " + toStr(last_idx));
				}
				return true;
			}
	private:
		// [must] option.Assignment[i].size() == 1  - chk!
			inline int FindRight2(VECTOR<Token2>& strVec, int start, int last, const wiz::LoadDataOption& option)
			{
				for (int i = last; i >= start; --i) {
					if (strVec[i].len == 1 && -1 != Equal(option.Right, strVec[i].str[0])) {
						return i;
					}

					if (strVec[i].len == 1 && -1 != Equal(option.Left, strVec[i].str[0])) {
						return i;
					}
					else if (strVec[i].len == 1 && -1 != Equal(option.Assignment, strVec[i].str[0])) {
						//
					}
					else if (i < last && !(strVec[i + 1].len == 1 && -1 != Equal(option.Assignment, strVec[i + 1].str[0])))
					{                // NOT
						return i;
					}
				}

				return -1;
			}

		public:
			//// ToDo : # => option.~?
			template <class Reserver>
			static bool _LoadData4(VECTOR<Token2>& strVec, Reserver& reserver, UserType& global, const wiz::LoadDataOption& option, const int lex_thr_num, const int parse_num) // first, strVec.empty() must be true!!
			{
				const int pivot_num = parse_num - 1;
				char* buffer = nullptr;

				bool end = false;
				{
					end = !reserver(&strVec, option, lex_thr_num, buffer);
				}

				UserType* before_next = nullptr;
				UserType _global;

				bool first = true;
				long long sum = 0;

				while (true) {
					end = true;

					std::chrono::steady_clock::time_point a = std::chrono::steady_clock::now();

					std::set<int> _pivots;
					std::vector<int> pivots;
					const int last_idx = strVec.size() - 1;

					if (pivot_num > 0) {
						std::vector<int> pivot;

						for (int i = 0; i < pivot_num; ++i) {
							pivot.push_back(FindRight(strVec, (last_idx / (pivot_num + 1)) * (i), (last_idx / (pivot_num + 1)) * (i + 1) - 1, option));
						}

						for (int i = 0; i < pivot.size(); ++i) {
							if (pivot[i] != -1) {
								_pivots.insert(pivot[i]);
							}
						}

						for (auto& x : _pivots) {
							pivots.push_back(x);
						}
					}

					std::vector<UserType*> next(pivots.size() + 1, nullptr);

					{
						std::vector<UserType> __global(pivots.size() + 1);

						std::vector<std::thread> thr(pivots.size() + 1);

						{
							int idx = pivots.empty() ? last_idx : pivots[0];
							thr[0] = std::thread(__LoadData5, &strVec, 0, idx, &__global[0], &option, 0, 0, &next[0]);
											// __LoadData4 -> __LoadData5
						}

						for (int i = 1; i < pivots.size(); ++i) {
							thr[i] = std::thread(__LoadData5, &strVec, pivots[i - 1] + 1, pivots[i], &__global[i], &option, 0, 0, &next[i]);
						}

						if (pivots.size() >= 1) {
							thr[pivots.size()] = std::thread(__LoadData5, &strVec, pivots.back() + 1, last_idx, &__global[pivots.size()],
								&option, 0, 0, &next[pivots.size()]);
						}

						// wait
						for (int i = 0; i < thr.size(); ++i) {
							thr[i].join();
						}

						// Merge
						try {
							if (first) {
								Merge(&_global, &__global[0], &next[0]);
								first = false;
							}
							else {
								Merge(before_next, &__global[0], &next[0]);
							}

							for (int i = 1; i < pivots.size() + 1; ++i) {
								Merge(next[i - 1], &__global[i], &next[i]);
							}

						}
						catch (...) {
							delete[] buffer;
							buffer = nullptr;
							throw "in Merge, error";
						}
						{
							if (next[pivots.size()]->GetParent() != nullptr) {
								delete[] buffer;
								buffer = nullptr;
								throw "merge error";
							}
						}

						strVec.erase(strVec.begin(), strVec.begin() + last_idx + 1);

						before_next = next.back();
					}

					std::chrono::steady_clock::time_point b = std::chrono::steady_clock::now();
					sum = sum + std::chrono::steady_clock::duration(b - a).count(); //std::cout << b - a << "ms" << std::endl;

					if (!end) {
						//
					}
					else {
						break;
					}
				}

				delete[] buffer;

				global = std::move(_global); 

				return true;
			}

			static bool LoadDataFromFile4(const std::string& fileName, UserType& global, int pivot_num, int lex_thr_num) /// global should be empty
			{
				if (pivot_num < 0) {
					pivot_num = std::thread::hardware_concurrency() - 1;
				}
				if (lex_thr_num <= 0) {
					lex_thr_num = std::thread::hardware_concurrency();
				}
				if (pivot_num <= -1) {
					pivot_num = 0;
				}
				if (lex_thr_num <= 0) {
					lex_thr_num = 1;
				}


				bool success = true;
				std::ifstream inFile;
				inFile.open(fileName, std::ios::binary);


				if (true == inFile.fail())
				{
					inFile.close(); return false;
				}
				UserType globalTemp;
				static VECTOR<Token2> strVec;

				strVec.clear();

				try {
					InFileReserver3 ifReserver(inFile);
					wiz::LoadDataOption option;
					option.Assignment.push_back("=");
					option.Left.push_back('{');
					option.Right.push_back('}');
					option.LineComment.push_back("#");

					ifReserver.Num = 1 << 19;
					//	strVec.reserve(ifReserver.Num);
					// cf) empty file..
					if (false == _LoadData4(strVec, ifReserver, globalTemp, option, lex_thr_num, pivot_num + 1))
					{
						inFile.close();
						return false; // return true?
					}

					inFile.close();
				}
				catch (const char* err) { std::cout << err << std::endl; inFile.close(); return false; }
				catch (const std::string& e) { std::cout << e << std::endl; inFile.close(); return false; }
				catch (std::exception e) { std::cout << e.what() << std::endl; inFile.close(); return false; }
				catch (...) { std::cout << "not expected error" << std::endl; inFile.close(); return false; }

				global = std::move(globalTemp);
				return true;
			}
		private:
			static int FindRight(VECTOR<Token2>& strVec, int start, int last, const wiz::LoadDataOption& option)
			{
				for (int i = last; i >= start; --i) {
					if (strVec[i].len == 1 && -1 != Equal(option.Right, strVec[i].str[0])) {
						return i;
					}
				}

				return -1;
			}
			
			static void Merge(UserType* next, UserType* ut, UserType** ut_next)
			{
				int ut_depth = 0;
				int next_depth = 0;

				//check!!
				while (ut->GetIListSize() >= 1 && ut->GetUserTypeListSize() >= 1 
					&& (ut->GetUserTypeList(0)->GetName() == "##" ||
					ut->GetUserTypeList(0)->GetName() == "#"))
				{
					ut_depth++;
					ut = ut->GetUserTypeList(0);
				}

				//int chk = 0;
				while (true) {
					int itCount = 0;
					int utCount = 0;

					UserType* _ut = ut;
					UserType* _next = next;

					if (ut_next && _ut == *ut_next) {
						*ut_next = _next;
					}

					for (int i = 0; i < _ut->GetIListSize(); ++i) {
						if (_ut->IsUserTypeList(i)) {
							if (_ut->GetUserTypeList(utCount)->GetName() == "#" ||
								_ut->GetUserTypeList(utCount)->GetName() == "##") {
								_ut->GetUserTypeList(utCount)->SetName("");
							}
							else {
								{
									_next->LinkUserType(_ut->GetUserTypeList(utCount));
									_ut->GetUserTypeList(utCount) = nullptr;
								}
								//chk++;
							}
							utCount++;
						}
						else if (_ut->IsItemList(i)) {
							_next->AddItemList(std::move(_ut->GetItemList(itCount)));
							itCount++;
						}
					}
					_ut->Remove();

					if (ut->GetParent() && next->GetParent()) {
						ut = ut->GetParent();
						next = next->GetParent();
						next_depth++;
					}
					else {
						break;
					}
				}
				if (next_depth < ut_depth) {
					throw "Merge error depth difference";
				}
			}


		public:
			//// ToDo : # => option.~?
			static bool LoadDataFromFile3(const std::string& fileName, UserType& global, int pivot_num, int lex_thread_num) /// global should be empty
			{
				if (pivot_num < 0) {
					pivot_num = std::thread::hardware_concurrency() - 1;
				}
				if (lex_thread_num <= 0) {
					lex_thread_num = std::thread::hardware_concurrency();
				}
				if (pivot_num <= -1) {
					pivot_num = 0;
				}
				if (lex_thread_num <= 0) {
					lex_thread_num = 1;
				}


				std::chrono::steady_clock::time_point a = std::chrono::steady_clock::now();
				{
					bool success = true;
					std::ifstream inFile;
					inFile.open(fileName, std::ios::binary);


					if (true == inFile.fail())
					{
						inFile.close(); return false;
					}
					UserType globalTemp = global;
					static VECTOR<Token2> strVec;

					try {
						InFileReserver3 ifReserver(inFile);
						wiz::LoadDataOption option;

						option.Assignment.push_back("=");
						option.Left.push_back('{');
						option.Right.push_back('}');
						//option.MuitipleLineCommentStart.push_back("###");
						//option.MuitipleLineCommentEnd.push_back("###");
						option.LineComment.push_back("#");

						ifReserver.Num = 1 << 19;
						//	strVec.reserve(ifReserver.Num);
						// cf) empty file..
						if (false == _LoadData3(strVec, ifReserver, globalTemp, option, pivot_num, lex_thread_num))
						{
							inFile.close();
							return false; // return true?
						}

						inFile.close();
					}
					catch (const char* err) { std::cout << err << std::endl; inFile.close(); return false; }
					catch (const std::string& e) { std::cout << e << std::endl; inFile.close(); return false; }
					catch (const std::exception&e) { std::cout << e.what() << std::endl; inFile.close(); return false; }
					catch (...) { std::cout << "not expected error" << std::endl; inFile.close(); return false; }

					global = std::move(globalTemp);
				}
				//std::chrono::steady_clock::time_point b = std::chrono::steady_clock::now();
				//std::cout << (double)std::chrono::steady_clock::duration(b - a).count() 
				//	* std::chrono::steady_clock::period().num / std::chrono::steady_clock::period().den << "s" << std::endl;
				return true;
			}
			/*
			static bool LoadDataFromFile3_2(char*& buffer, const std::string& fileName, UserType& global, int pivot_num, int lex_thread_num) /// global should be empty
			{
				if (pivot_num < 0) {
					pivot_num = std::thread::hardware_concurrency() - 1;
				}
				if (lex_thread_num <= 0) {
					lex_thread_num = std::thread::hardware_concurrency();
				}
				if (pivot_num <= -1) {
					pivot_num = 0;
				}
				if (lex_thread_num <= 0) {
					lex_thread_num = 1;
				}

				std::chrono::steady_clock::time_point a = std::chrono::steady_clock::now();

				bool success = true;
				std::ifstream inFile;
				inFile.open(fileName, std::ios::binary);


				if (true == inFile.fail())
				{
					inFile.close(); return false;
				}
				UserType globalTemp;
				static VECTOR<Token2> strVec;


				try {
					InFileReserver3 ifReserver(inFile);
					wiz::LoadDataOption option;
					option.Assignment.push_back("=");

					//option.Assignment.push_back("<-"); // test 

					option.Left.push_back('{');
					option.Right.push_back('}');
					//option.MuitipleLineCommentStart.push_back("###");
					//option.MuitipleLineCommentEnd.push_back("###");
					option.LineComment.push_back("#"); // always #! - now... - todo! for json -> # + other? otherwise -> LineComment[0]?

					ifReserver.Num = 1 << 19;
					//	strVec.reserve(ifReserver.Num);
					// cf) empty file..
					if (false == _LoadData3_2(buffer, strVec, ifReserver, globalTemp, option, pivot_num, lex_thread_num))
					{
						inFile.close();
						return false; // return true?
					}

					inFile.close();
				}
				catch (const char* err) { std::cout << err << std::endl; inFile.close(); return false; }
				catch (const std::string& e) { std::cout << e << std::endl; inFile.close(); return false; }
				catch (const std::exception&e) { std::cout << e.what() << std::endl; inFile.close(); return false; }
				catch (...) { std::cout << "not expected error" << std::endl; inFile.close(); return false; }

				global = std::move(globalTemp);

				std::chrono::steady_clock::time_point b = std::chrono::steady_clock::now();
				std::cout << (double)std::chrono::steady_clock::duration(b - a).count()
					* std::chrono::steady_clock::period().num / std::chrono::steady_clock::period().den << "s" << std::endl;
				return true;
			}
			*/
		private:
			static bool isOpenTagStart(const std::string& word) {
				return wiz::String::startsWith(word, "<") && !wiz::String::startsWith(word, "</");
			}
			static bool isOpenTagEnd(const std::string& word) {
				return wiz::String::endsWith(word, ">") && !wiz::String::endsWith(word, "/>");
			}
			static bool isCloseTagStart(const std::string& word) {
				return wiz::String::startsWith(word, "</");
			}
			static bool isCloseTagEnd(const std::string& word) {
				return wiz::String::endsWith(word, ">") && !wiz::String::endsWith(word, "/>");
			}
			// return wiz::String::endsWith(word, "/>");
		public:
			/// assume valid html file?, no comment!
			template <class Reserver>
			static bool _LoadDataHTML(ARRAY_QUEUE<Token>& strVec, Reserver& reserver, UserType& global, const wiz::LoadDataOption& option) // first, strVec.empty() must be true!!
			{
				int before_state = -1;
				int state = 0;
				UserType* now = &global; // global`s parent is nullptr

				{
					reserver(strVec, option);

					while (strVec.empty())
					{
						reserver(strVec, option);
						if (
							strVec.empty() &&
							reserver.end()
							) {
							return false; // throw "Err nextToken does not exist"; // cf) or empty file or empty std::string!
						}
					}
				}

				while (false == strVec.empty()) {

					//	std::cout << state << " " << Utility::Top(strVec, now, reserver, option) << std::endl;

					switch (state)
					{
					case 0:
						if (isOpenTagStart(Utility::Top(strVec, now, reserver, option))) {
							std::string tagName;
							std::string token;
							Utility::Pop(strVec, &token, now, reserver, option);
							// ex)  <test>
							if (isOpenTagEnd(token)) {
								tagName = token.substr(1, token.size() - 2);
								state = 4; // ?
							}
							// <test/>
							else if (wiz::String::endsWith(token, "/>")) {
								tagName = token.substr(1, token.size() - 3);
								state = 2;
							}
							// <test aa=bb > or <test aa=bb /> or <test aa=bb>
							else {
								tagName = token.substr(1, token.size() - 1);
								state = 1;
							}

							// make, test = { }
							now->AddUserTypeItem(UserType(tagName));
							now = now->GetUserTypeList(now->GetUserTypeListSize() - 1);
						}
						else {
							// err
							throw "err 0";
						}
						break;
					case 1:
						if ((isCloseTagStart(Utility::Top(strVec, now, reserver, option))
							&& isCloseTagEnd(Utility::Top(strVec, now, reserver, option))) ||
							Utility::Top(strVec, now, reserver, option) == "/>") {
							now->AddUserTypeItem(UserType(""));
							now = now->GetUserTypeList(now->GetUserTypeListSize() - 1);

							state = 5;
						}
						else if (//isOpenTagEnd(Utility::Top(strVec, now, reserver, option))) {
							Utility::Top(strVec, now, reserver, option) == ">"
							) {
							Utility::Pop(strVec, nullptr, now, reserver, option);

							state = 3;
						}
						else {
							std::string token;
							Utility::Pop(strVec, &token, now, reserver, option);
							std::string::size_type equal_idx = token.find('=');

							if (equal_idx == std::string::npos) {
								throw "err 1";
							}
							else {
								if (wiz::String::endsWith(token, "/>")) {
									now->AddItem(token.substr(0, equal_idx), token.substr(equal_idx + 1, token.size() - equal_idx - 1 - 2));

									state = 2;
								}
								else if (wiz::String::endsWith(token, ">")) {
									now->AddItem(token.substr(0, equal_idx), token.substr(equal_idx + 1, token.size() - equal_idx - 1 - 1));

									state = 4;
								}
								else {
									now->AddItem(token.substr(0, equal_idx), token.substr(equal_idx + 1, token.size() - equal_idx - 1));

									state = 1;
								}
							}
						}
						break;
					case 2:
						now->AddUserTypeItem(UserType(""));
						now = now->GetParent();
						state = 5;
						break;
					case 3:
						if ((isCloseTagStart(Utility::Top(strVec, now, reserver, option)) && isCloseTagEnd(Utility::Top(strVec, now, reserver, option))) ||
							wiz::String::endsWith(Utility::Top(strVec, now, reserver, option), "/>")) {
							now->AddUserTypeItem(UserType(""));
							now = now->GetUserTypeList(now->GetUserTypeListSize() - 1);

							state = 5;
						}
						else {
							std::string token;

							Utility::Pop(strVec, &token, now, reserver, option);

							now->AddUserTypeItem(UserType(""));
							now = now->GetUserTypeList(now->GetUserTypeListSize() - 1);

							now->AddItem("", token);
							state = 5;
						}
						break;
					case 4:
						now->AddUserTypeItem(UserType(""));
						now = now->GetUserTypeList(now->GetUserTypeListSize() - 1);

						// </test>
						if (isCloseTagStart(Utility::Top(strVec, now, reserver, option)) &&
							isCloseTagEnd(Utility::Top(strVec, now, reserver, option))) {

							Utility::Pop(strVec, nullptr, now, reserver, option);
							if (nullptr == now->GetParent()) {
								state = 0;
							}
							else {
								now = now->GetParent()->GetParent();
								state = 5;
							}
							if (nullptr == now->GetParent()) {
								state = 0;
							}

						}
						else if (isOpenTagStart(Utility::Top(strVec, now, reserver, option))) {
							state = 0;
						}
						else {
							std::string token;

							Utility::Pop(strVec, &token, now, reserver, option);
							now->AddItem("", token);
							state = 5;
						}

						break;
					case 5:
						if (isOpenTagStart(Utility::Top(strVec, now, reserver, option))) {
							state = 0;
						}
						else if (isCloseTagStart(Utility::Top(strVec, now, reserver, option)) &&
							isCloseTagEnd(Utility::Top(strVec, now, reserver, option))) {
							Utility::Pop(strVec, nullptr, now, reserver, option);
							if (nullptr == now->GetParent()) {
								state = 0;
							}
							else {
								now = now->GetParent()->GetParent();
								state = 5;
							}
							if (nullptr == now->GetParent()) {
								state = 0;
							}
						}
						else if (wiz::String::endsWith(Utility::Top(strVec, now, reserver, option), "/>")) {
							Utility::Pop(strVec, nullptr, now, reserver, option);
							if (nullptr == now->GetParent()) {
								state = 0;
							}
							else {
								now = now->GetParent()->GetParent();
								state = 5;
							}
							if (nullptr == now->GetParent()) {
								state = 0;
							}
						}
						else {
							std::string token;
							Utility::Pop(strVec, &token, now, reserver, option);

							now->AddItem("", token);
						}

						break;
					case 6:
						// </test>
						if (isCloseTagStart(Utility::Top(strVec, now, reserver, option)) &&
							isCloseTagEnd(Utility::Top(strVec, now, reserver, option))) {

							Utility::Pop(strVec, nullptr, now, reserver, option);
							if (nullptr == now->GetParent()) {
								state = 0;
							}
							else {
								now = now->GetParent()->GetParent();
								state = 5;
							}
							if (nullptr == now->GetParent()) {
								state = 0;
							}
						}
						//
						else {
							std::string token;
							Utility::Pop(strVec, &token, now, reserver, option);

							now->AddItem("", token);

							state = 6;
						}


						break;
					default:
						// syntax err!!
						break;
					}

					if (strVec.size() < 10) {
						reserver(strVec, option);

						while (strVec.empty()) // (strVec.empty())
						{
							reserver(strVec, option);
							if (
								strVec.empty() &&
								reserver.end()
								) {
								// throw "Err nextToken does not exist2";
								break;
							}
						}
					}
				}

				if (state != 0) {
					std::cout << global.ToString() << std::endl;
					throw std::string("error final state is not 0!  : ") + toStr(state);
				}

				return true;
			}
#ifdef USE_FAST_LOAD_DATA

		private:
			static std::string _Convert(wiz_string* from)
			{
				return std::string(get_cstr_wiz_string(from), size_wiz_string(from));
			}
			static void Convert(user_type* from, UserType& ut)
			{
				// name
				ut.SetName(_Convert(&from->name));

				free_wiz_string(&from->name);
				// parent
				ut.SetParent(nullptr);
				// comment
				for (int i = 0; i < size_wiz_vector_wiz_string(&from->comment_list); ++i) {
					ut.PushComment(_Convert(get_wiz_vector_wiz_string(&from->comment_list, i)));
				}
				// itemtype
				// usertype
				int item_count = 0, user_count = 0;
				const int n = size_wiz_vector_int(&from->ilist);

				ut.ReserveIList(n);
				ut.ReserveItemList(size_wiz_vector_item_type(&from->item_list));
				ut.ReserveUserTypeList(size_wiz_vector_any2(&from->user_type_list));

				for (int i = 0; i < n; ++i) {
					if (1 == *get_wiz_vector_int(&from->ilist, i)) {
						item_type* it = get_wiz_vector_item_type(&from->item_list, item_count);
						ut.AddItem(_Convert(&it->name), _Convert(&it->value));
						item_count++;
						free_item_value_in_item_type(it);
					}
					else {
						user_type* temp = (user_type*)get_wiz_vector_any2(&from->user_type_list, user_count);

						ut.AddUserTypeItem(UserType(""));

						Convert(temp, *ut.GetUserTypeList(user_count));

						user_count++;

						free_user_type_in_user_type(temp);
					}
				}

				free_wiz_vector_int(&from->ilist);
				free_wiz_vector_any2(&from->user_type_list);
				free_wiz_vector_item_type(&from->item_list);
				free_user_type_in_user_type(from);
			}
		public:
			static bool FastLoadDataFromFile(const std::string& fileName, UserType& global)
			{
				bool success = true;
				wiz_string temp = make_wiz_string("", 0);
				user_type data; init_in_user_type(&data, &temp);
				wiz_string _fileName = make_wiz_string(fileName.c_str(), fileName.size());

				try {
					free_user_type_in_user_type(&data);
					data = load_data_from_file_in_load_data(&_fileName);
					Convert(&data, global);
				}
				catch (...) {
					success = false;
				}

				free_wiz_string(&_fileName);
				free_user_type_in_user_type(&data);
				free_wiz_string(&temp);
				return success;
			}

			static bool FastLoadDataFromString(const std::string& str, UserType& global)
			{
				bool success;
				wiz_string temp = make_wiz_string("", 0);
				user_type data; init_in_user_type(&data, &temp);
				wiz_string _str = make_wiz_string(str.c_str(), str.size());

				try {
					free_user_type_in_user_type(&data);
					data = load_data_from_string_in_load_data(&_str);
					Convert(&data, global);
				}
				catch (...) {
					success = false;
				}

				free_wiz_string(&_str);
				free_user_type_in_user_type(&data);
				free_wiz_string(&temp);
				return success;
			}
#endif
		public:
			static bool LoadDataFromFile(const std::string& fileName, UserType& global) /// global should be empty
			{
				bool success = true;
				std::ifstream inFile;
				inFile.open(fileName, std::ios::binary);


				if (true == inFile.fail())
				{
					inFile.close(); return false;
				}
				UserType globalTemp = global;
				ARRAY_QUEUE<Token> strVec;

				try {
					InFileReserver ifReserver(inFile);
					wiz::LoadDataOption option;
					option.Assignment.push_back("=");
					option.Left.push_back('{');
					option.Right.push_back('}');
					//option.MuitipleLineCommentStart.push_back("###");
					//option.MuitipleLineCommentEnd.push_back("###");
					option.LineComment.push_back("#");  
					
					ifReserver.Num = 1 << 19;
					//strVec.reserve(ifReserver.Num);
					// cf) empty file..
					if (false == _LoadData(strVec, ifReserver, globalTemp, option))
					{
						inFile.close();
						return false; // return true?
					}

					inFile.close();
				}
				catch (const Error& e) { std::cout << e << std::endl; inFile.close(); return false; }
				catch (const char* err) { std::cout << err << std::endl; inFile.close(); return false; }
				catch (const std::string& e) { std::cout << e << std::endl; inFile.close(); return false; }
				catch (const std::exception&e) { std::cout << e.what() << std::endl; inFile.close(); return false; }
				catch (...) { std::cout << "not expected error" << std::endl; inFile.close(); return false; }

				global = std::move(globalTemp);
				return true;
			}

			// no multiple line comment, no multiple line string!, removal??
			static bool LoadDataFromFile2(const std::string& fileName, UserType& global) /// global should be empty
			{
				bool success = true;
				std::ifstream inFile;
				inFile.open(fileName, std::ios::binary);



				if (true == inFile.fail())
				{
					inFile.close(); return false;
				}
				UserType globalTemp = global;
				std::vector<ARRAY_QUEUE<Token>> strVec(4);

				//try 
				{
					InFileReserver2 ifReserver(inFile, 4);
					wiz::LoadDataOption option;
					option.Assignment.push_back("=");
					option.Left.push_back('{');
					option.LineComment.push_back("#");
					option.Right.push_back('}');

					ifReserver.Num = 1 << 20;
					strVec.reserve(ifReserver.Num);
					// cf) empty file..
					if (false == _LoadData2(strVec, ifReserver, globalTemp, option)) // 4 is # of cpu core?
					{
						inFile.close();
						return false; // return true?
					}

					inFile.close();
				}
				/*
				catch (const Error& e) { std::cout << e << std::endl; inFile.close(); return false; }
				catch (const char* err) { std::cout << err << std::endl; inFile.close(); return false; }
				catch (const std::string& e) { std::cout << e << std::endl; inFile.close(); return false; }
				catch (const std::exception&e) { std::cout << e.what() << std::endl; inFile.close(); return false; }
				catch (...) { std::cout << "not expected error" << std::endl; inFile.close(); return false; }
				*/
				global = std::move(globalTemp);

				return true;
			}
			static bool LoadDataFromFileFastJson(const std::string& fileName, UserType& global, int pivot_num, int lex_thread_num) /// global should be empty
			{
				if (pivot_num < 0) {
					pivot_num = std::thread::hardware_concurrency() - 1;
				}
				if (lex_thread_num <= 0) {
					lex_thread_num = std::thread::hardware_concurrency();
				}
				if (pivot_num <= -1) {
					pivot_num = 0;
				}
				if (lex_thread_num <= 0) {
					lex_thread_num = 1;
				}


				bool success = true;
				std::ifstream inFile;
				inFile.open(fileName, std::ios::binary);

				if (true == inFile.fail())
				{
					inFile.close(); return false;
				}
				UserType globalTemp;
				static VECTOR<Token2> strVec;

				try {
					InFileReserver3 ifReserver(inFile);
					wiz::LoadDataOption option;
					option.Removal.push_back(',');
					option.Assignment.push_back(":");
					option.Left.push_back('{');
					option.Left.push_back('[');
					option.Right.push_back('}');
					option.Right.push_back(']');
					//option.MuitipleLineCommentStart.push_back("###");
					//option.MuitipleLineCommentEnd.push_back("###");
					//option.LineComment.push_back("#");

					ifReserver.Num = 10000;
					//	strVec.reserve(ifReserver.Num);
					// cf) empty file..
					if (false == _LoadData3(strVec, ifReserver, globalTemp, option, pivot_num, lex_thread_num))
					{
						inFile.close();
						return false; // return true?
					}

					inFile.close();
				}
				catch (const Error& e) { std::cout << e << std::endl; inFile.close(); return false; }
				catch (const char* err) { std::cout << err << std::endl; inFile.close(); return false; }
				catch (const std::string& e) { std::cout << e << std::endl; inFile.close(); return false; }
				catch (const std::exception&e) { std::cout << e.what() << std::endl; inFile.close(); return false; }
				catch (...) { std::cout << "not expected error" << std::endl; inFile.close(); return false; }

				global = std::move(globalTemp);
				return true;
			}
		private:
			static void RemoveQuotation(UserType* ut) {
				int it_count = 0;
				int ut_count = 0;

				for (int i = 0; i < ut->GetIListSize(); ++i) {
					if (ut->IsItemList(i)) {
						std::string temp = wiz::ToString(ut->GetItemList(it_count).GetName());

						if (temp.empty() == false) {
							ut->GetItemList(it_count).SetName(temp.substr(1, temp.size() - 2));
						}
						it_count++;
					}
					else {
						std::string temp = wiz::ToString(ut->GetUserTypeList(ut_count)->GetName());
						if (temp.empty() == false) {
							ut->GetUserTypeList(ut_count)->SetName(temp.substr(1, temp.size() - 2));
						}
						RemoveQuotation(ut->GetUserTypeList(ut_count));

						ut_count++;
					}
				}
			}
			static void RemoveUnderbar(UserType* ut) {
				int it_count = 0;
				int ut_count = 0;

				for (int i = 0; i < ut->GetIListSize(); ++i) {
					if (ut->IsItemList(i)) {
						if (ut->GetItemList(it_count).GetName() == "_") {
							ut->GetItemList(it_count).SetName("");
						}
						it_count++;
					}
					else {
						if (ut->GetUserTypeList(ut_count)->GetName() == "_") {
							ut->GetUserTypeList(ut_count)->SetName("");
						}
						RemoveUnderbar(ut->GetUserTypeList(ut_count));
						ut_count++;
					}
				}
			}

		public:
			// global is empty?
			static bool LoadDataFromFileWithJson(const std::string& fileName, UserType& global) /// global should be empty
			{
				std::ifstream inFile;
				inFile.open(fileName);
				if (true == inFile.fail())
				{
					inFile.close(); return false;
				}
				UserType globalTemp = global;
				ARRAY_QUEUE<Token> strVec;

				try {
					InFileReserver ifReserver(inFile);
					wiz::LoadDataOption option;
					option.Removal.push_back(',');
					option.Assignment.push_back(":");
					option.Left.push_back('{');
					option.Left.push_back('[');
					//option.LineComment.push_back('#');
					option.Right.push_back('}');
					option.Right.push_back(']');

					ifReserver.Num = 1 << 19;
					// cf) empty file..
					if (false == _LoadData(strVec, ifReserver, globalTemp, option))
					{
						inFile.close();
						return false; // return true?
					}

					inFile.close();
				}
				catch (const Error& e) { std::cout << e << std::endl; inFile.close(); return false; }
				catch (const char* err) { std::cout << err << std::endl; inFile.close(); return false; }
				catch (const std::string& e) { std::cout << e << std::endl; inFile.close(); return false; }
				catch (const std::exception&e) { std::cout << e.what() << std::endl; inFile.close(); return false; }
				catch (...) { std::cout << "not expected error" << std::endl; inFile.close(); return false; }

				RemoveQuotation(&globalTemp);
				RemoveUnderbar(&globalTemp);

				global = std::move(globalTemp); // check!!

				return true;
			}
		private:
			static void ConvertHtmlToClau(const UserType* htmlUT, UserType* clauUT) {
				int itemTypeCount = 0;
				int userTypeCount = 0;

				clauUT->ReserveIList(htmlUT->GetIListSize());
				clauUT->ReserveItemList(htmlUT->GetItemListSize());
				clauUT->ReserveUserTypeList(htmlUT->GetUserTypeListSize());

				for (int i = 0; i < htmlUT->GetIListSize(); ++i) {
					if (htmlUT->IsItemList(i)) {
						// additemlist -> rename?
						clauUT->AddItemList(std::move(htmlUT->GetItemList(itemTypeCount)));

						itemTypeCount++;
					}
					else {
						clauUT->AddUserTypeItem(UserType(wiz::ToString(htmlUT->GetUserTypeList(userTypeCount)->GetName())));

						ConvertHtmlToClau(htmlUT->GetUserTypeList(userTypeCount)->GetUserTypeList(0), clauUT->GetUserTypeList(userTypeCount));

						userTypeCount++;
					}
				}
			}
			static void ConvertClauToHtml(const UserType* clauUT, UserType* htmlUT) {
				int itemTypeCount = 0;
				int userTypeCount = 0;

				htmlUT->ReserveIList(clauUT->GetIListSize());
				htmlUT->ReserveItemList(clauUT->GetItemListSize());
				htmlUT->ReserveUserTypeList(clauUT->GetUserTypeListSize());

				for (int i = 0; i < clauUT->GetIListSize(); ++i) {
					if (clauUT->IsItemList(i)) {
						// additemlist -> rename?
						htmlUT->AddItemList(std::move(clauUT->GetItemList(itemTypeCount)));

						itemTypeCount++;
					}
					else {
						htmlUT->AddUserTypeItem(UserType(wiz::ToString((clauUT->GetUserTypeList(userTypeCount)->GetName()))));
						htmlUT->GetUserTypeList(userTypeCount)->AddUserTypeItem(UserType(""));

						ConvertClauToHtml(clauUT->GetUserTypeList(userTypeCount), htmlUT->GetUserTypeList(userTypeCount)->GetUserTypeList(0));

						userTypeCount++;
					}
				}
			}
		public:
			/// todo- comment
			static bool LoadDataFromFileWithHtml(const std::string& fileName, UserType& global) /// global should be empty
			{
				bool success = true;
				std::ifstream inFile;
				inFile.open(fileName);


				if (true == inFile.fail())
				{
					inFile.close(); return false;
				}
				UserType globalTemp = global;
				ARRAY_QUEUE<Token> strVec;

				try {
					InFileReserver ifReserver(inFile);
					wiz::LoadDataOption option;

					//option.Assignment.push_back('=');
					//option.Left.push_back('{');
					//option.LineComment.push_back('#');
					//option.Right.push_back('}');

					ifReserver.Num = 1 << 20;
					//strVec.reserve(ifReserver.Num);
					// cf) empty file..
					if (false == _LoadDataHTML(strVec, ifReserver, globalTemp, option))
					{
						inFile.close();
						return false; // return true?
					}

					inFile.close();
				}
				catch (const Error& e) { std::cout << e << std::endl; inFile.close(); return false; }
				catch (const char* err) { std::cout << err << std::endl; inFile.close(); return false; }
				catch (const std::string& e) { std::cout << e << std::endl; inFile.close(); return false; }
				catch (const std::exception&e) { std::cout << e.what() << std::endl; inFile.close(); return false; }
				catch (...) { std::cout << "not expected error" << std::endl; inFile.close(); return false; }

				ConvertHtmlToClau(&globalTemp, &global);

				//global = std::move(globalTemp);
				return true;
			}

			static bool LoadDataFromString(const std::string& str, UserType& ut)
			{
				/*
				char LEFT = '{';
				char RIGHT = '}';

				std::cout << str.size() << " ";
				UserType utTemp = ut;
				VECTOR<Token2> strVec;
				long long thread_num = 1; // std::thread::hardware_concurrency();
				long long length = str.size();
				char* buffer = new char[str.size() + 1];
				strcpy(buffer, str.c_str());

				wiz::LoadDataOption option;
				option.Assignment.push_back("=");
				option.Left.push_back('{');

				//option.MuitipleLineCommentStart.push_back("###");
				//option.MuitipleLineCommentEnd.push_back("###");

				option.LineComment.push_back("#");
				option.Right.push_back('}');
				std::string temp;
				std::vector<long long> start(thread_num + 1, 0);
				std::vector<long long> last(thread_num + 1, 0);

				if (thread_num > 1) {
					start[0] = 0;

					// todo - linear check?
					for (int i = 1; i < thread_num; ++i) {
						start[i] = length / thread_num * i;
						for (int x = start[i]; x <= length; ++x) {
							// here bug is..  " check "
							if ('\r' == buffer[x] || '\n' == (buffer[x]) || length == x) {
								start[i] = x;
								//	std::cout << "start " << start[i] << std::endl;
								break;
							}
						}
					}
					for (int i = 0; i < thread_num - 1; ++i) {
						last[i] = start[i + 1] - 1;
						for (int x = last[i]; x <= length; ++x) {
							if ('\r' == buffer[x] || '\n' == (buffer[x]) || length == x) {
								last[i] = x;
								//	std::cout << "start " << start[i] << std::endl;
								break;
							}
						}
					}
					last[thread_num - 1] = length;
					//	std::cout << last[thread_num - 1] << std::endl;
				}
				else {
					start[0] = 0;
					last[0] = length;
				}

				//int a = clock();
				//debug

				if (thread_num > 1) {
					//// in string, there are '\r' or '\n' etc.. - no '\r' or '\n'?
					for (int i = thread_num - 1; i > 0; --i) {
						int last_enter_idx = -1;
						int first_enter_idx = -1;

						// find last_enter_idx, first_enter_idx -
						//// has bug - " abc
						////				def"
						for (int j = last[i - 1]; j <= last[i]; ++j) {
							if (buffer[j] == '\r' || buffer[j] == '\n' || j == last[thread_num - 1]) {
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

					std::vector<VECTOR<Token2>> partial_list(thread_num, VECTOR<Token2>());
					std::vector<std::thread> thr(thread_num);

					for (int i = 0; i < thread_num; ++i) {
						//	std::cout << last[i] - start[i] << std::endl;
						partial_list[i].reserve((last[i] - start[i]) / 10);
						thr[i] = std::thread(wiz::load_data::Utility::DoThread3(buffer + start[i], buffer + last[i], &partial_list[i], &option));
					}

					for (int i = 0; i < thread_num; ++i) {
						thr[i].join();
					}

					long long new_size = strVec.size() + 4 + 4;
					for (int i = 0; i < thread_num; ++i) {
						new_size = new_size + partial_list[i].size();
					}

					strVec.reserve(new_size);

					//check { {
					strVec.push_back(wiz::Token2(&LEFT, 1, false));
					strVec.push_back(wiz::Token2(&LEFT, 1, false));

					for (int i = 0; i < thread_num; ++i) {
						//for (int j = 0; j < partial_list[i].size(); ++j) {
						//	aq->push_back(std::move(partial_list[i][j]));
						//}
						strVec.insert(strVec.end(), make_move_iterator(partial_list[i].begin()), make_move_iterator(partial_list[i].end()));
					}
				}
				else {
					VECTOR<Token2> temp;

					temp.reserve((last[0] - start[0]) / 10);

					wiz::load_data::Utility::DoThread3 dothr(buffer + start[0], buffer + last[0], &temp, &option);

					dothr();

					//check { {
					strVec.push_back(wiz::Token2(&LEFT, 1, false));
					strVec.push_back(wiz::Token2(&LEFT, 1, false));


					strVec.insert(strVec.end(), make_move_iterator(temp.begin()), make_move_iterator(temp.end()));
				}

				//check } }
				strVec.push_back(wiz::Token2(&RIGHT, 1, false));
				strVec.push_back(wiz::Token2(&RIGHT, 1, false));

				try {
					// empty std::string!
					NoneReserver nonReserver;
					if (false == _LoadData3(strVec, nonReserver, utTemp, option, thread_num - 1, thread_num))
					{
						return true;
					}
				}
				catch (Error& e) { std::cout << e << std::endl; return false; }
				catch (const char* err) { std::cout << err << std::endl; return false; }
				catch (std::exception& e) { std::cout << e.what() << std::endl; return false; }
				catch (const std::string& estr) { std::cout << str << std::endl; return false; }
				catch (...) { std::cout << "not expected error" << std::endl; return  false; }

				ut = std::move(utTemp);
				delete[] buffer;
				return true;
			*/
				
				UserType utTemp;
				ARRAY_QUEUE<Token> strVec;

				wiz::StringBuilder builder(str.size(), str.c_str(), str.size());

				wiz::LoadDataOption option;
				option.Assignment.push_back("=");
				option.Left.push_back('{');

				//option.MuitipleLineCommentStart.push_back("###");
				//option.MuitipleLineCommentEnd.push_back("###");

				option.LineComment.push_back("#");
				option.Right.push_back('}');
				wiz::load_data::Utility::DoThread doThread(&builder, &strVec, &option);

				doThread();

				try {
					// empty std::string!
					NoneReserver nonReserver;
					if (false == _LoadData(strVec, nonReserver, utTemp, option))
					{
						return true;
					}
				}
				catch (Error& e) { std::cout << e << std::endl; return false; }
				catch (const char* err) { std::cout << err << std::endl; return false; }
				catch (std::exception& e) { std::cout << e.what() << std::endl; return false; }
				catch (const std::string& e) { std::cout << str << std::endl; return false; }
				catch (...) { std::cout << "not expected error" << std::endl; return  false; }

				{
					long long itCount = 0;
					long long utCount = 0;

					for (int i = 0; i < utTemp.GetIListSize(); ++i) {
						if (utTemp.IsItemList(i)) {
							ut.AddItemList(std::move(utTemp.GetItemList(itCount)));
							itCount++;
						}
						else {
							ut.AddUserTypeItem(std::move(*utTemp.GetUserTypeList(utCount)));
							utCount++;
						}
					}
				}
				return true;
			}

			static std::string DoCondition(UserType& global, const std::string& condition, const ExcuteData& excuteData, wiz::StringBuilder* builder)
			{
				std::string _condition = condition;

				_condition = ToBool4(nullptr, global, _condition, excuteData, builder);

				Condition cond(_condition, &global, &global, builder);

				while (cond.Next());

				if (cond.Now().size() != 1 || "TRUE" != cond.Now()[0]) // || cond.Now().size()  != 1
				{
					return "FALSE";
				}
				else {
					return "TRUE";
				}
			}
		private:
			UserType global; // ToDo - remove!
		public:
			/// To Do.. static function -> ~.function.!
			// InitQuery or LoadQuery
			explicit LoadData() { InitWizDB(); }

			explicit LoadData(EXCUTE_MODULE_BASE* pExcuteModule) {
				this->pExcuteModule = pExcuteModule;
			}
			/// need testing!
			LoadData(const LoadData& ld)
				: global(ld.global)
			{
				//global = ld.global;
			}
			virtual ~LoadData() { AllRemoveWizDB(); }

			/// need testing!
			LoadData& operator=(const LoadData& ld)
			{
				if (this == &ld) { return *this; }

				global = ld.global;
				return *this;
			}
			//
			bool InitWizDB() {
				return InitWizDB(global);
			}
			// allRemove Query 
			bool AllRemoveWizDB() {
				return AllRemoveWizDB(global);
			}
			// AddQuery AddData, AddUserTypeData
			bool AddData(const std::string& position, const std::string& data, const std::string& condition, const ExcuteData& excuteData, wiz::StringBuilder* builder) {
				return AddData(global, position, data, condition, excuteData, builder);
			}
			// 
			bool AddNoNameUserType(const std::string& position, const std::string& data, const std::string& condition, const ExcuteData& excuteData, wiz::StringBuilder* builder)
			{
				return AddNoNameUserType(global, position, data, condition, excuteData, builder);
			}
			// SetQuery
			bool SetData(const std::string& position, const std::string& varName, const std::string& data, const std::string& condition, const ExcuteData& excuteData, wiz::StringBuilder* builder)
			{
				return SetData(global, position, varName, data, condition, excuteData, builder);
			}
			/// 
			std::string GetData(const std::string& position, const std::string& condition, const ExcuteData& excuteData, wiz::StringBuilder* builder) {
				return GetData(global, position, condition, excuteData, builder);
			}
			std::string GetItemListData(const std::string& position, const std::string& condition, const ExcuteData& excuteData, wiz::StringBuilder* builder)
			{
				return GetItemListData(global, position, condition, excuteData, builder);
			}
			std::string GetItemListNamesData(const std::string& position, const std::string& condition, const ExcuteData& excuteData, wiz::StringBuilder* builder)
			{
				return GetItemListNamesData(global, position, condition, excuteData, builder);
			}
			std::string GetUserTypeListNamesData(const std::string& position, const std::string& condition, const ExcuteData& excuteData, wiz::StringBuilder* builder)
			{
				return GetUserTypeListNamesData(global, position, condition, excuteData, builder);
			}
			/// varName = val - do
			/// varName = { val val val } - GetData(position+"/varName", ""); 
			/// varName = { var = val } - GetData(position+"/varname", var);
			std::string GetData(const std::string& position, const std::string& varName, const std::string& condition, const ExcuteData& excuteData, wiz::StringBuilder* builder) // 
			{
				return GetData(global, position, varName, condition, excuteData, builder);
			}
			bool Remove(const std::string& position, const std::string& var, const std::string& condition, const ExcuteData& excuteData, wiz::StringBuilder* builder) {
				return Remove(global, position, var, condition, excuteData, builder);
			}

			bool LoadWizDB(const std::string& fileName) {
				return LoadWizDB(global, fileName);
			}
			// SaveQuery
			bool SaveWizDB(const std::string& fileName, const std::string& option = "0") { /// , int option
				return SaveWizDB(global, fileName, option);
			}

			/// To Do - ExistItem, ExistUserType, SetUserType GetUserType
			bool ExistData(const std::string& position, const std::string& varName, const std::string& condition, const ExcuteData& excuteData, wiz::StringBuilder* builder) // 
			{
				return ExistData(global, position, varName, condition, excuteData, builder);
			}

			/// ToDo - recursive function
			std::string SearchItem(const std::string& var, const std::string& condition, const ExcuteData& excuteData, wiz::StringBuilder* builder)
			{
				return SearchItem(global, var, condition, excuteData, "root", builder);
			}
			std::string SearchUserType(const std::string& var, const std::string& condition, const ExcuteData& excuteData, wiz::StringBuilder* builder)
			{
				return SearchUserType(global, var, condition, excuteData, builder);
			}

			//cf) /$ and /$/ ??
			static std::string GetRealDir(const std::string& dir, const wiz::load_data::UserType* ut, wiz::StringBuilder* builder)
			{
				std::vector<std::string> real_dir;
				std::string result;


				while (nullptr != ut->GetParent()) {
					std::string str = wiz::ToString(ut->GetName());
					
					if (str.empty()) {
						str = "_"; // check!
					}
					
					real_dir.push_back(str);

					ut = ut->GetParent();
				}

				result += "/./";

				for (int i = real_dir.size() - 1; i >= 0; --i) {
					result += real_dir[i] + "/";
				}

				return result;
			}

			static void _Iterate(Option& opt, UserType& global, const std::string& dir, const std::vector<wiz::load_data::UserType*>& ut, UserType* eventsTemp, const std::string& recursive, const ExcuteData& excuteData, wiz::StringBuilder* builder)
			{
				for (int i = 0; i < ut.size(); ++i) {
					int itemCount = 0;
					int utCount = 0;

					for (int j = 0; j < ut[i]->GetItemListSize(); ++j) {
						//if (ut[i]->IsItemList(j)) 
						{
							ExcuteData _excuteData;
							//_excuteData.info = excuteData.info;
							_excuteData.pModule = excuteData.pModule;
							_excuteData.pObjectMap = excuteData.pObjectMap;
							_excuteData.pEvents = eventsTemp;
							_excuteData.depth = excuteData.depth;
							_excuteData.noUseInput = excuteData.noUseInput;
							_excuteData.noUseOutput = excuteData.noUseOutput;
							//_excuteData.chkInfo = true;

							
							auto x = eventsTemp->GetUserTypeList(eventsTemp->GetUserTypeListSize() - 1)->GetUserTypeItem("$call");
							for (int k = 0; k < x.size() && k + itemCount < ut[i]->GetItemListSize(); ++k) {
								x[k]->GetItemList(1).Set(0, ut[i]->GetItemList(itemCount + k).GetName());
								x[k]->GetItemList(2).Set(0, ut[i]->GetItemList(itemCount + k).Get(0));
								x[k]->GetItemList(3).Set(0, "FALSE");
								x[k]->GetItemList(4).Set(0, GetRealDir(dir, ut[i], builder));
								wiz::DataType temp;
								temp.SetInt(j);
								x[k]->GetItemList(5).Set(0, temp);
							}
							std::string result = pExcuteModule->excute_module("Main = { $call = { id = NONE__  } }", &global, _excuteData, opt, 0);

							if (result.empty() == false) {
								UserType resultUT;
								wiz::load_data::LoadData::LoadDataFromString(result, resultUT);

								auto name = resultUT.GetItem("name");
								if (name.empty() == false) {
									ut[i]->GetItemList(itemCount).SetName(ToString(name[0].Get(0)));
								}

								auto value = resultUT.GetItem("value");
								if (value.empty() == false) {
									ut[i]->GetItemList(itemCount).Set(0, value[0].Get(0));
								}
							}

							itemCount++;
							//itemCount += ut[i]->GetItemListSize();
							//j = itemCount - 1;
						}
					}

					for (int j = 0; j < ut[i]->GetUserTypeListSize(); ++j) {
						ExcuteData _excuteData;
						//_excuteData.info = excuteData.info;
						_excuteData.pModule = excuteData.pModule;
						_excuteData.pObjectMap = excuteData.pObjectMap;
						_excuteData.pEvents = eventsTemp;
						_excuteData.depth = excuteData.depth + 1;
						_excuteData.noUseInput = excuteData.noUseInput;
						_excuteData.noUseOutput = excuteData.noUseOutput;
						//_excuteData.chkInfo = true;


						auto x = eventsTemp->GetUserTypeList(eventsTemp->GetUserTypeListSize() - 1)->GetUserTypeItem("$call");
						for (int k = 0; k < x.size(); ++k) {
							x[k]->GetItemList(1).Set(0, ut[i]->GetUserTypeList(utCount)->GetName());
							x[k]->GetItemList(2).Set(0, "NONE"); // check..
							x[k]->GetItemList(3).Set(0, "TRUE");
							const std::string name = ut[i]->GetUserTypeList(utCount)->GetName().ToString();
							x[k]->GetItemList(4).Set(0, GetRealDir(dir + (name.empty() ? "_" : name) + "/", ut[i]->GetUserTypeList(utCount), builder));
							
							wiz::DataType temp;
							temp.SetInt(j);
							x[k]->GetItemList(5).Set(0, temp);
						}

						Option opt;
 						std::string result = pExcuteModule->excute_module("Main = { $call = { id = NONE__  } }", &global, _excuteData, opt, 0);

						if (result.empty() == false) {
							UserType resultUT;
							wiz::load_data::LoadData::LoadDataFromString(result, resultUT);

							auto name = resultUT.GetItem("name");
							if (name.empty() == false) {
								ut[i]->GetUserTypeList(utCount)->SetName(ToString(name[0].Get(0)));
							}
						}

						//// recursive
						if ("TRUE" == recursive) {
							const std::string name = wiz::ToString(ut[i]->GetUserTypeList(utCount)->GetName());
							_Iterate(opt, global, dir + (name.empty() ? "_" : name) + "/",
								std::vector<UserType*>{ ut[i]->GetUserTypeList(utCount) }, eventsTemp, recursive, excuteData, builder);
						}

						utCount++;
					}
				}
			}
			// new function! - check UserType::Find().second[0] ?
			static void Iterate(wiz::load_data::UserType& global, const std::string& dir, const std::vector<std::string>& events, const std::string& recursive, const std::string& before_value, const ExcuteData& excuteData, wiz::StringBuilder* builder)
			{
				std::vector<wiz::load_data::UserType*> ut = wiz::load_data::UserType::Find(&global, dir, builder).second; // chk first?
				wiz::load_data::UserType eventsTemp = *excuteData.pEvents;

				Option opt;
				std::string statements2; // = " return_values = { } ";
					
				statements2 += " Event = { id = NONE__  $local = { temp } ";
				statements2 += " $assign = { $local.temp data = { " + before_value + " } } ";

				//for (int t = 0; t < 100; ++t) {
					for (int i = 0; i < events.size(); ++i) {

						statements2 += " $call = { id = ";

						statements2 += events[i];
						statements2 += " name = __name ";
						statements2 += " value = __value ";
						statements2 += " is_user_type = __is_user_type ";
						statements2 += " real_dir = __real_dir ";
						statements2 += " before_value = { $local.temp } ";
						statements2 += " idx = __idx  ";
						statements2 += " } ";

						statements2 += " $assign = { $local.temp data = { $return_value = { } } } ";
					//	statements2 += " $insert2 = { where = { /./return_values/ } data = { $return_value = { } } } ";
					}
//}
				statements2 += " } ";

				wiz::load_data::LoadData::AddData(eventsTemp, "/root", statements2, "TRUE", ExcuteData(), builder);

				_Iterate(opt, global, dir, ut, &eventsTemp, recursive, excuteData, builder);

				eventsTemp.RemoveUserTypeList(eventsTemp.GetUserTypeListSize() - 1);
			//	eventsTemp->RemoveUserTypeList(eventsTemp->GetUserTypeListSize() - 1);
			}
			
			// chk remove!
			static void _Iterate2(UserType& global, const std::string& dir, const std::vector<wiz::load_data::UserType*>& ut, UserType* eventsTemp, const std::string& recursive, const ExcuteData& excuteData, wiz::StringBuilder* builder)
			{
				for (int i = 0; i < ut.size(); ++i) {
					int itemCount = ut[i]->GetItemListSize();
					int utCount = ut[i]->GetUserTypeListSize();

					for (int j = ut[i]->GetItemListSize() - 1; j >= 0; --j) {
						itemCount--;
						//if (ut[i]->IsItemList(j)) 
						{
							ExcuteData _excuteData;
							//_excuteData.info = excuteData.info;
							_excuteData.pModule = excuteData.pModule;
							_excuteData.pObjectMap = excuteData.pObjectMap;
							_excuteData.pEvents = eventsTemp;
							_excuteData.depth = excuteData.depth;
							_excuteData.noUseInput = excuteData.noUseInput;
							_excuteData.noUseOutput = excuteData.noUseOutput;
							//_excuteData.chkInfo = true;

							auto x = eventsTemp->GetUserTypeList(eventsTemp->GetUserTypeListSize() - 1)->GetUserTypeItem("$call");
							for (int k = 0; k < x.size(); ++k) {
								x[k]->GetItemList(1).Set(0, ut[i]->GetItemList(itemCount).GetName());
								x[k]->GetItemList(2).Set(0, ut[i]->GetItemList(itemCount).Get(0));
								x[k]->GetItemList(3).Set(0, "FALSE");
								x[k]->GetItemList(4).Set(0, GetRealDir(dir, ut[i], builder));
							}

							Option opt;
							std::string result = pExcuteModule->excute_module("Main = { $call = { id = NONE__  } }", &global, _excuteData, opt, 0);

							if (result.empty() == false) {
								UserType resultUT;
								wiz::load_data::LoadData::LoadDataFromString(result, resultUT);

								auto name = resultUT.GetItem("name");
								if (name.empty() == false) {
									ut[i]->GetItemList(itemCount).SetName(ToString(name[0].Get(0)));
								}

								auto value = resultUT.GetItem("value");
								if (value.empty() == false) {
									ut[i]->GetItemList(itemCount).Set(0, value[0].Get(0));
								}

								auto remove = resultUT.GetItem("remove");
								if (remove.empty() == false && remove[0].Get(0) == "TRUE") {
									ut[i]->RemoveItemList(itemCount);
								}
							}
						}
					}
					for (int j = ut[i]->GetUserTypeListSize() - 1; j >= 0; --j) {
						utCount--;

						ExcuteData _excuteData;
						//_excuteData.info = excuteData.info;
						_excuteData.pModule = excuteData.pModule;
						_excuteData.pObjectMap = excuteData.pObjectMap;
						_excuteData.pEvents = eventsTemp;
						_excuteData.depth = excuteData.depth + 1;
						_excuteData.noUseInput = excuteData.noUseInput;
						_excuteData.noUseOutput = excuteData.noUseOutput;
						//_excuteData.chkInfo = true;


						auto x = eventsTemp->GetUserTypeList(eventsTemp->GetUserTypeListSize() - 1)->GetUserTypeItem("$call");
						for (int k = 0; k < x.size(); ++k) {
							x[k]->GetItemList(1).Set(0, ut[i]->GetUserTypeList(utCount)->GetName());
							x[k]->GetItemList(2).Set(0, "NONE"); // check..
							x[k]->GetItemList(3).Set(0, "TRUE");
							const std::string name = ut[i]->GetUserTypeList(utCount)->GetName().ToString();
							x[k]->GetItemList(4).Set(0, GetRealDir(dir + (name.empty() ? "_" : name) + "/", ut[i]->GetUserTypeList(utCount), builder));
						}

						Option opt;
						std::string result = pExcuteModule->excute_module("Main = { $call = { id = NONE__  } }", &global, _excuteData, opt, 0);

						if (result.empty() == false) {
							UserType resultUT;
							wiz::load_data::LoadData::LoadDataFromString(result, resultUT);

							auto name = resultUT.GetItem("name");
							if (name.empty() == false) {
								ut[i]->GetUserTypeList(utCount)->SetName(ToString(name[0].Get(0)));
							}

							auto remove = resultUT.GetItem("remove");
							if (remove.empty() == false && remove[0].Get(0) == "TRUE") {
								ut[i]->RemoveUserTypeList(utCount);
								continue;
							}
						}

						//// recursive
						if ("TRUE" == recursive) {
							const std::string name = wiz::ToString(ut[i]->GetUserTypeList(utCount)->GetName());
							_Iterate2(global, dir + (name.empty() ? "_" : name) + "/",
								std::vector<UserType*>{ ut[i]->GetUserTypeList(utCount) }, eventsTemp, recursive, excuteData, builder);
						}
					}
				}
			}
			// new function! - check UserType::Find().second[0] ?
			// todo - beforevalue! - as parameter.
			static void Iterate2(wiz::load_data::UserType& global, const std::string& dir, const std::vector<std::string>& events, const std::string& recursive, const ExcuteData& excuteData, wiz::StringBuilder* builder)
			{
				std::vector<wiz::load_data::UserType*> ut = wiz::load_data::UserType::Find(&global, dir, builder).second; // chk first?
				wiz::load_data::UserType eventsTemp = *excuteData.pEvents;

				std::string statements2 = " Event = { id = NONE__  $local = { temp } ";
				statements2 += " $assign = { $local.temp data = { empty } } ";

				for (int i = 0; i < events.size(); ++i) {

					statements2 += " $call = { id = ";

					statements2 += events[i];
					statements2 += " name = __name ";
					statements2 += " value = __value ";
					statements2 += " is_user_type = __is_user_type ";
					statements2 += " real_dir = __real_dir ";
					statements2 += " before_value = { $local.temp } ";
					statements2 += " } ";

					statements2 += " $assign = { $local.temp data = { $return_value = { } } } ";
				}
				statements2 += " } ";

				wiz::load_data::LoadData::AddData(eventsTemp, "/root", statements2, "TRUE", ExcuteData(), builder);

				_Iterate2(global, dir, ut, &eventsTemp, recursive, excuteData, builder);

				eventsTemp.RemoveUserTypeList(eventsTemp.GetUserTypeListSize() - 1);

			}
			
			// using dir, name
			static void _Iterate3(Option& opt, UserType& global, const std::string& dir, const std::string& name, const std::vector<wiz::load_data::UserType*>& ut, UserType* eventsTemp, const std::string& recursive, const ExcuteData& excuteData, wiz::StringBuilder* builder)
			{
				for (int i = 0; i < ut.size(); ++i) {
					std::vector<int> itemData = ut[i]->GetItemPtr(name);
					std::vector<int> utData = ut[i]->GetUserTypeItemPtr(name);
					int itemNum = 0;
					int utNum = 0;
					for (int j = 0; j < itemData.size(); ++j) {
						//if (ut[i]->IsItemList(j)) 
						{
							ExcuteData _excuteData;
							//_excuteData.info = excuteData.info;
							_excuteData.pModule = excuteData.pModule;
							_excuteData.pObjectMap = excuteData.pObjectMap;
							_excuteData.pEvents = eventsTemp;
							_excuteData.depth = excuteData.depth;
							_excuteData.noUseInput = excuteData.noUseInput;
							_excuteData.noUseOutput = excuteData.noUseOutput;
							//_excuteData.chkInfo = true;

							auto x = eventsTemp->GetUserTypeList(eventsTemp->GetUserTypeListSize() - 1)->GetUserTypeItem("$call");
							for (int k = 0; k < x.size(); ++k) {
								x[k]->GetItemList(1).Set(0, ut[i]->GetItemList(itemData[itemNum]).GetName());
								x[k]->GetItemList(2).Set(0, ut[i]->GetItemList(itemData[itemNum]).Get(0));
								x[k]->GetItemList(3).Set(0, "FALSE");
								x[k]->GetItemList(4).Set(0, GetRealDir(dir, ut[i], builder));
								wiz::DataType temp;
								temp.SetInt(itemData[itemNum]);
								x[k]->GetItemList(5).Set(0, temp);
							}
							std::string result = pExcuteModule->excute_module("Main = { $call = { id = NONE__  } }", &global, _excuteData, opt, 0);

							if (result.empty() == false) {
								UserType resultUT;
								wiz::load_data::LoadData::LoadDataFromString(result, resultUT);

								auto name = resultUT.GetItem("name");
								if (name.empty() == false) {
									ut[i]->GetItemList(itemData[itemNum]).SetName(ToString(name[0].Get(0)));
								}

								auto value = resultUT.GetItem("value");
								if (value.empty() == false) {
									ut[i]->GetItemList(itemData[itemNum]).Set(0, value[0].Get(0));
								}
							}

							itemNum++;
						}
					}

					for (int j = 0; j < ut[i]->GetUserTypeListSize(); ++j) {
						ExcuteData _excuteData;
						//_excuteData.info = excuteData.info;
						_excuteData.pModule = excuteData.pModule;
						_excuteData.pObjectMap = excuteData.pObjectMap;
						_excuteData.pEvents = eventsTemp;
						_excuteData.depth = excuteData.depth + 1;
						_excuteData.noUseInput = excuteData.noUseInput;
						_excuteData.noUseOutput = excuteData.noUseOutput;
						//_excuteData.chkInfo = true;

						if (j == utData[utNum]) {
							auto x = eventsTemp->GetUserTypeList(eventsTemp->GetUserTypeListSize() - 1)->GetUserTypeItem("$call");
							for (int k = 0; k < x.size(); ++k) {
								x[k]->GetItemList(1).Set(0, ut[i]->GetUserTypeList(utData[utNum])->GetName());
								x[k]->GetItemList(2).Set(0, "NONE"); // check..
								x[k]->GetItemList(3).Set(0, "TRUE");
								const std::string name = ut[i]->GetUserTypeList(utData[utNum])->GetName().ToString();
								x[k]->GetItemList(4).Set(0, GetRealDir(dir + (name.empty() ? "_" : name) + "/", ut[i]->GetUserTypeList(utData[utNum]), builder));

								wiz::DataType temp;
								temp.SetInt(utData[utNum]);
								x[k]->GetItemList(5).Set(0, temp);
							}

							Option opt;
							std::string result = pExcuteModule->excute_module("Main = { $call = { id = NONE__  } }", &global, _excuteData, opt, 0);

							if (result.empty() == false) {
								UserType resultUT;
								wiz::load_data::LoadData::LoadDataFromString(result, resultUT);

								auto name = resultUT.GetItem("name");
								if (name.empty() == false) {
									ut[i]->GetUserTypeList(utData[utNum])->SetName(ToString(name[0].Get(0)));
								}
							}

							utNum++;
						}

						//// recursive
						if ("TRUE" == recursive) {
							const std::string _name = wiz::ToString(ut[i]->GetUserTypeList(j)->GetName());
							_Iterate3(opt, global, dir + (_name.empty() ? "_" : _name) + "/", name,
								std::vector<UserType*>{ ut[i]->GetUserTypeList(j) }, eventsTemp, recursive, excuteData, builder);
						}
					}
				}
			}
			// new function! - check UserType::Find().second[0] ?
			static void Iterate3(wiz::load_data::UserType& global, const std::string& dir, const std::string& name, const std::vector<std::string>& events, const std::string& recursive, const std::string& before_value, const ExcuteData& excuteData, wiz::StringBuilder* builder)
			{
				std::vector<wiz::load_data::UserType*> ut = wiz::load_data::UserType::Find(&global, dir, builder).second; // chk first?
				wiz::load_data::UserType eventsTemp = *excuteData.pEvents;

				Option opt;
				std::string statements2; // = " return_values = { } ";

				statements2 += " Event = { id = NONE__  $local = { temp } ";
				statements2 += " $assign = { $local.temp data = { " + before_value + " } } ";

				//for (int t = 0; t < 100; ++t) {
				for (int i = 0; i < events.size(); ++i) {

					statements2 += " $call = { id = ";

					statements2 += events[i];
					statements2 += " name = __name ";
					statements2 += " value = __value ";
					statements2 += " is_user_type = __is_user_type ";
					statements2 += " real_dir = __real_dir ";
					statements2 += " before_value = { $local.temp } ";
					statements2 += " idx = __idx  ";
					statements2 += " } ";

					statements2 += " $assign = { $local.temp data = { $return_value = { } } } ";
					//	statements2 += " $insert2 = { where = { /./return_values/ } data = { $return_value = { } } } ";
				}
				//}
				statements2 += " } ";

				wiz::load_data::LoadData::AddData(eventsTemp, "/root", statements2, "TRUE", ExcuteData(), builder);

				_Iterate3(opt, global, dir, name, ut, &eventsTemp, recursive, excuteData, builder);

				eventsTemp.RemoveUserTypeList(eventsTemp.GetUserTypeListSize() - 1);
				//	eventsTemp->RemoveUserTypeList(eventsTemp->GetUserTypeListSize() - 1);
			}


			
			static void _RIterate(UserType& global, const std::string& dir, const std::vector<wiz::load_data::UserType*>& ut, UserType* eventsTemp, const std::string& recursive, const ExcuteData& excuteData, wiz::StringBuilder* builder)
			{
				for (int i = 0; i < ut.size(); ++i) {
					int itemCount = ut[i]->GetItemListSize();
					int utCount = ut[i]->GetUserTypeListSize();

					for (int j = ut[i]->GetItemListSize() - 1; j >= 0; --j) {
						itemCount--;
						//if (ut[i]->IsItemList(j)) 
						{
							ExcuteData _excuteData;
							//_excuteData.info = excuteData.info;
							_excuteData.pModule = excuteData.pModule;
							_excuteData.pObjectMap = excuteData.pObjectMap;
							_excuteData.pEvents = eventsTemp;
							_excuteData.depth = excuteData.depth;
							_excuteData.noUseInput = excuteData.noUseInput;
							_excuteData.noUseOutput = excuteData.noUseOutput;
							//_excuteData.chkInfo = true;

							auto x = eventsTemp->GetUserTypeList(eventsTemp->GetUserTypeListSize() -
								(eventsTemp->GetUserTypeListSize() - excuteData.pEvents->GetUserTypeListSize()))->GetUserTypeItem("$call");
							for (int k = 0; k < x.size(); ++k) {
								x[k]->GetItemList(1).Set(0, ut[i]->GetItemList(itemCount).GetName());
								x[k]->GetItemList(2).Set(0, ut[i]->GetItemList(itemCount).Get(0));
								x[k]->GetItemList(3).Set(0, "FALSE");
								x[k]->GetItemList(4).Set(0, GetRealDir(dir, ut[i], builder));
							}

							Option opt;
							std::string result = pExcuteModule->excute_module("Main = { $call = { id = NONE__  } }", &global, _excuteData, opt, 0);

							if (result.empty() == false) {
								UserType resultUT;
								wiz::load_data::LoadData::LoadDataFromString(result, resultUT);

								auto name = resultUT.GetItem("name");
								if (name.empty() == false) {
									ut[i]->GetItemList(itemCount).SetName(ToString(name[0].Get(0)));
								}

								auto value = resultUT.GetItem("value");
								if (value.empty() == false) {
									ut[i]->GetItemList(itemCount).Set(0, value[0].Get(0));
								}
							}
						}
					}
					for (int j = ut[i]->GetUserTypeListSize() - 1; j >= 0; --j) {
						utCount--;

						ExcuteData _excuteData;
						//_excuteData.info = excuteData.info;
						_excuteData.pModule = excuteData.pModule;
						_excuteData.pObjectMap = excuteData.pObjectMap;
						_excuteData.pEvents = eventsTemp;
						_excuteData.depth = excuteData.depth + 1;
						_excuteData.noUseInput = excuteData.noUseInput;
						_excuteData.noUseOutput = excuteData.noUseOutput;
						//_excuteData.chkInfo = true;


						auto x = eventsTemp->GetUserTypeList(eventsTemp->GetUserTypeListSize() - 1)->GetUserTypeItem("$call");
						for (int k = 0; k < x.size(); ++k) {
							x[k]->GetItemList(1).Set(0, ut[i]->GetUserTypeList(utCount)->GetName());
							x[k]->GetItemList(2).Set(0, "NONE"); // check..
							x[k]->GetItemList(3).Set(0, "TRUE");
							const std::string name = ut[i]->GetUserTypeList(utCount)->GetName().ToString();
							x[k]->GetItemList(4).Set(0, GetRealDir(dir + (name.empty() ? "_" : name) + "/", ut[i]->GetUserTypeList(utCount), builder));
						}

						Option opt;
						std::string result = pExcuteModule->excute_module("Main = { $call = { id = NONE__  } }", &global, _excuteData, opt, 0);

						if (result.empty() == false) {
							UserType resultUT;
							wiz::load_data::LoadData::LoadDataFromString(result, resultUT);

							auto name = resultUT.GetItem("name");
							if (name.empty() == false) {
								ut[i]->GetUserTypeList(utCount)->SetName(ToString(name[0].Get(0)));
							}
						}

						//// recursive
						if ("TRUE" == recursive) {
							const std::string name = wiz::ToString(ut[i]->GetUserTypeList(utCount)->GetName());
							_RIterate(global, dir + (name.empty() ? "_" : name) + "/",
								std::vector<UserType*>{ ut[i]->GetUserTypeList(utCount) }, eventsTemp, recursive, excuteData, builder);
						}
					}
				}
			}

			// new function! - check UserType::Find().second[0] ?
			static void RIterate(wiz::load_data::UserType& global, const std::string& dir, const std::vector<std::string>& events, const std::string& recursive, const ExcuteData& excuteData, wiz::StringBuilder* builder)
			{
				std::vector<wiz::load_data::UserType*> ut = wiz::load_data::UserType::Find(&global, dir, builder).second; // chk first?
				wiz::load_data::UserType eventsTemp = *excuteData.pEvents;

				std::string statements2 = " Event = { id = NONE__  $local = { temp } ";
				statements2 += " $assign = { $local.temp data = { empty } } ";

				for (int i = 0; i < events.size(); ++i) {

					statements2 += " $call = { id = ";

					statements2 += events[i];
					statements2 += " name = __name ";
					statements2 += " value = __value ";
					statements2 += " is_user_type = __is_user_type ";
					statements2 += " real_dir = __real_dir ";
					statements2 += " before_value = { $local.temp } ";
					statements2 += " } ";

					statements2 += " $assign = { $local.temp data = { $return_value = { } } } ";
				}
				statements2 += " } ";

				wiz::load_data::LoadData::AddData(eventsTemp, "/root", statements2, "TRUE", ExcuteData(), builder);

				_RIterate(global, dir, ut, &eventsTemp, recursive, excuteData, builder);

				eventsTemp.RemoveUserTypeList(eventsTemp.GetUserTypeListSize() - 1);
			}
		private:
			void SearchItem(std::vector<std::string>& positionVec, const std::string& var, const std::string& nowPosition,
				UserType* ut, const std::string& condition, const ExcuteData& excuteData, wiz::StringBuilder* builder)
			{
				SearchItem(global, positionVec, var, nowPosition, ut, condition, excuteData, builder);
			}
			void SearchUserType(std::vector<std::string>& positionVec, const std::string& var, const std::string& nowPosition,
				UserType* ut, const std::string& condition, const ExcuteData& excuteData, wiz::StringBuilder* builder)
			{
				SearchUserType(global, positionVec, var, nowPosition, ut, condition, excuteData, builder);
			}
		private:
			// chk - Search(item or usertype) : add ~~~ (option?)?? and ToBool4? // chk more thinking!!
			static void SearchItem(UserType& global, std::vector<std::string>& positionVec, const std::string& var, const std::string& nowPosition,
				UserType* ut, const std::string& condition, const ExcuteData& excuteData, wiz::StringBuilder* builder)
			{
				std::string _var = var;
				if (_var == " " || _var == "_") { _var = ""; }

				// $it?

				if (ut->GetItem(_var).size() > 0) {
					std::string _condition = condition;

					if (_var == "") { _condition = wiz::String::replace(_condition, "~~", "_"); }
					else {
						_condition = wiz::String::replace(_condition, "~~", _var); //
					}

					if (nowPosition == "") {
						_condition = wiz::String::replace(_condition, "////", "@/./");
					}
					else {
						_condition = wiz::String::replace(_condition, "////", "@" + nowPosition);
					}
					_condition = ToBool4(ut, global, _condition, excuteData, builder);

					Condition cond(_condition, ut, &global, builder);


					while (cond.Next());

					if (cond.Now().size() == 1 && "TRUE" == cond.Now()[0])
					{
						positionVec.push_back(nowPosition);
					}
				}

				for (int i = 0; i < ut->GetUserTypeListSize(); ++i) {
					std::string temp = wiz::ToString(ut->GetUserTypeList(i)->GetName());
					if (temp == "") { temp = " "; }
					SearchItem(
						global,
						positionVec,
						_var,
						nowPosition + "/" + temp,
						ut->GetUserTypeList(i),
						condition,
						excuteData,
						builder
					);
				}
			}

			static void SearchUserType(UserType& global, std::vector<std::string>& positionVec, const std::string& var, const std::string& nowPosition,
				UserType* ut, const std::string& condition, const ExcuteData& excuteData, wiz::StringBuilder* builder)
			{
				std::string _var = var;
				if (_var == " " || _var == "_") {
					_var = "";
				}
				if (ut->GetUserTypeItem(_var).size() > 0) {
					std::string _condition = condition;

					if (_var == "") { _condition = wiz::String::replace(_condition, "~~", "_"); }
					else  _condition = wiz::String::replace(_condition, "~~", _var); //


					if (nowPosition == "") {
						_condition = wiz::String::replace(_condition, "////", "@/./");
					}
					else {
						_condition = wiz::String::replace(_condition, "////", "@" + nowPosition);
					}
					_condition = ToBool4(ut, global, _condition, excuteData, builder);

					Condition cond(_condition, ut, &global, builder);

					while (cond.Next());

					if (cond.Now().size() == 1 && "TRUE" == cond.Now()[0])
					{
						positionVec.push_back(nowPosition);
					}
				}

				for (int i = 0; i < ut->GetUserTypeListSize(); ++i) {
					std::string temp = wiz::ToString(ut->GetUserTypeList(i)->GetName());

					if (temp == "") { temp = " "; }
					SearchUserType(
						global,
						positionVec,
						_var,
						nowPosition + "/" + temp,
						ut->GetUserTypeList(i),
						condition,
						excuteData,
						builder
					);
				}
			}

		public:
			static bool InitWizDB(UserType& global) {
				global = UserType("global");
				return true;
			}
			// allRemove Query 
			static bool AllRemoveWizDB(UserType& global) {
				global = UserType("");
				return true;
			}
			// AddQuery AddData, AddUserTypeData
			static bool AddDataAtFront(UserType& global, const std::string& position, const std::string& data, const std::string& condition, const ExcuteData& excuteData, wiz::StringBuilder* builder) {
				UserType utTemp = UserType("global");
				bool isTrue = false;

				if (false == LoadDataFromString(data, utTemp))
				{
					return false;
				}
				auto finded = UserType::Find(&global, position, builder);
				if (finded.first) {
					for (int i = 0; i < finded.second.size(); ++i) {
						int item_n = utTemp.GetItemListSize();
						int user_n = utTemp.GetUserTypeListSize();

						/// chk temp test codes - > using flag 1->Exist 2->Comparision
						//if (finded.second[i]->GetItem("base_tax").GetCount() > 0) { continue; }
						///~end
						if (false == condition.empty()) {
							std::string _condition = condition;

							_condition = ToBool4(finded.second[i], global, _condition, excuteData, builder);

							Condition cond(_condition, finded.second[i], &global, builder);

							while (cond.Next());

							if (cond.Now().size() != 1 || "TRUE" != cond.Now()[0]) // || cond.Now().size()  != 1
							{
								//std::cout << cond.Now()[0] << endl;
								continue;
							}
						}

						for (int k = utTemp.GetIListSize() - 1; k >= 0; --k) {
							if (utTemp.IsItemList(k)) {
								finded.second[i]->AddItemAtFront(wiz::ToString(utTemp.GetItemList(item_n - 1).GetName()), ToString(utTemp.GetItemList(item_n - 1).Get(0)));
								item_n--;
							}
							else {
								finded.second[i]->AddUserTypeItemAtFront(*utTemp.GetUserTypeList(user_n - 1));
								user_n--;
							}
						}
						isTrue = true;
					}
					return isTrue;
				}
				else {
					UserType::Find(&global, position, builder);
					return false;
				}
			}
			static bool AddData(UserType& global, const std::string& position, const std::string& data, const std::string& condition, const ExcuteData& excuteData, wiz::StringBuilder* builder) {
				UserType utTemp = UserType("global");
				bool isTrue = false;

				if (false == LoadDataFromString(data, utTemp))
				{
					return false;
				}
				auto finded = UserType::Find(&global, position, builder);
				if (finded.first) {
					for (int i = 0; i < finded.second.size(); ++i) {
						int item_n = 0;
						int user_n = 0;

						if (false == condition.empty()) {
							std::string _condition = condition;

							//// todo - add ~~ ~~~ /// ////

							_condition = ToBool4(finded.second[i], global, _condition, excuteData, builder);

							Condition cond(_condition, finded.second[i], &global, builder);

							while (cond.Next());

							if (cond.Now().size() != 1 || "TRUE" != cond.Now()[0]) // || cond.Now().size()  != 1
							{
								//std::cout << cond.Now()[0] << endl;
								continue;
							}
						}

						for (int k = 0; k < utTemp.GetIListSize(); ++k) {
							if (utTemp.IsItemList(k)) {
								finded.second[i]->AddItem(utTemp.GetItemList(item_n).GetName(), utTemp.GetItemList(item_n).Get(0));
								item_n++;
							}
							else {
								finded.second[i]->AddUserTypeItem(*utTemp.GetUserTypeList(user_n));
								user_n++;
							}
						}
						isTrue = true;
					}
					return isTrue;
				}
				else {
					return false;
				}
			}
			static bool Insert(UserType& global, const std::string& position, const int idx, const std::string& data, const std::string& condition, const ExcuteData& excuteData, wiz::StringBuilder* builder) {
				UserType utTemp = UserType("global");
				bool isTrue = false;

				if (false == LoadDataFromString(data, utTemp))
				{
					return false;
				}
				auto finded = UserType::Find(&global, position, builder);
				if (finded.first) {
					for (int i = 0; i < finded.second.size(); ++i) {
						int item_n = utTemp.GetIListSize();
						int user_n = utTemp.GetIListSize();

						/// chk temp test codes - > using flag 1->Exist 2->Comparision
						//if (finded.second[i]->GetItem("base_tax").GetCount() > 0) { continue; }
						///~end
						if (false == condition.empty()) {
							std::string _condition = condition;

							_condition = ToBool4(finded.second[i], global, _condition, excuteData, builder);

							Condition cond(_condition, finded.second[i], &global, builder);

							while (cond.Next());

							if (cond.Now().size() != 1 || "TRUE" != cond.Now()[0]) // || cond.Now().size()  != 1
							{
								//std::cout << cond.Now()[0] << endl;
								continue;
							}
						}

						for (int k = utTemp.GetIListSize() - 1; k >= 0; --k) {
							if (utTemp.IsItemList(k)) {
								finded.second[i]->InsertItemByIlist(idx, wiz::ToString(utTemp.GetItemList(item_n - 1).GetName()),
									ToString(utTemp.GetItemList(item_n - 1).Get(0)));
								item_n--;
							}
							else {
								finded.second[i]->InsertUserTypeByIlist(idx, *utTemp.GetUserTypeList(user_n - 1));
								user_n--;
							}
						}
						isTrue = true;
					}
					return isTrue;
				}
				else {
					return false;
				}
			}
			static bool AddNoNameUserType(UserType& global, const std::string& position, const std::string& data, const std::string& condition, const ExcuteData& excuteData, wiz::StringBuilder* builder)
			{
				UserType utTemp = UserType("");
				bool isTrue = false;

				if (false == LoadDataFromString(data, utTemp))
				{
					return false;
				}
				auto finded = UserType::Find(&global, position, builder);
				if (finded.first) {
					for (int i = 0; i < finded.second.size(); ++i) {
						int item_n = 0;
						int user_n = 0;

						/// chk temp test codes - > using flag 1->Exist 2->Comparision
						//if (finded.second[i]->GetItem("base_tax").GetCount() > 0) { continue; }
						///~end
						if (false == condition.empty()) {
							std::string _condition = condition;
							_condition = ToBool4(finded.second[i], global, _condition, excuteData, builder);

							Condition cond(_condition, finded.second[i], &global, builder);

							while (cond.Next());

							if (cond.Now().size() != 1 || "TRUE" != cond.Now()[0])
							{
								//std::cout << cond.Now()[0] << endl;
								continue;
							}
						}
						finded.second[i]->AddUserTypeItem(utTemp);

						isTrue = true;
					}
					return isTrue;
				}
				else {
					return false;
				}
			}

			// todo - find example code?  a/b/c/d/e/f/ ??
			static bool AddUserType(UserType& global, const std::string& position, const std::string& var, const std::string& data, const std::string& condition, const ExcuteData& excuteData, wiz::StringBuilder* builder)
			{
				bool isTrue = false;
				auto finded = UserType::Find(&global, position, builder);
				if (finded.first) {
					std::string temp = var;
					if (temp == "") { temp = " "; }
					StringTokenizer tokenizer(temp, "/", builder, 1);
					UserType utTemp = UserType("");
					if (false == LoadDataFromString(data, utTemp))
					{
						return false;
					}

					while (tokenizer.hasMoreTokens()) {
						std::string utName = tokenizer.nextToken();
						std::vector<std::string> strVec;
						if (utName == " " || utName == "_") { utName = ""; }

						if (utName.size() >= 3 && utName[0] == '[' && utName[utName.size() - 1] == ']')
						{
							StringTokenizer tokenizer2(utName, std::vector<std::string>{ "[", "~", "]" }, builder, 1);
							while (tokenizer2.hasMoreTokens())
							{
								strVec.push_back(tokenizer2.nextToken());
							}
						}

						long long int a = 0, b = 0, Min = 0, Max = 0;
						bool chkInt = false;

						if (strVec.size() == 2 && Utility::IsInteger(strVec[0]) && Utility::IsInteger(strVec[1])) {
							chkInt = true;
							a = atoll(strVec[0].c_str());
							b = atoll(strVec[1].c_str());
							Min = std::min(a, b);
							Max = std::max(a, b);
						}

						for (auto x = Min; x <= Max; ++x)
						{
							if (strVec.size() == 2 && chkInt)
							{
								utName = std::to_string(x);
							}
							else {}
							utTemp.SetName(utName);

							for (int i = 0; i < finded.second.size(); ++i) {
								int item_n = 0;
								int user_n = 0;


								if (false == condition.empty()) {
									std::string _condition = condition;

									if (utName == "") { _condition = wiz::String::replace(_condition, "~~", "_"); }// do not use ^ in condition!
									else
										_condition = wiz::String::replace(_condition, "~~", utName); //

									_condition = ToBool4(finded.second[i], global, _condition, excuteData, builder);
									//	cout << "condition is " << _condition << endl;

									Condition cond(_condition, finded.second[i], &global, builder);

									while (cond.Next());

									if (cond.Now().size() != 1 || "TRUE" != cond.Now()[0])
									{
										//std::cout << cond.Now()[0] << endl;
										continue;
									}
								}

								finded.second[i]->AddUserTypeItem(utTemp);

								isTrue = true; // chk!!
							}

							// prevent from infinity loop.
							if (x == Max) { break; }
						}
					}
					return isTrue;
				}
				else {
					return false;
				}
			}
			/// SetData - Re Do!
			static bool SetData(UserType& global, const std::string& position, const std::string& varName, const std::string& data, const std::string& condition, const ExcuteData& excuteData, wiz::StringBuilder* builder)
			{
				auto finded = UserType::Find(&global, position, builder);
				bool isTrue = false;

				if (finded.first) {
					std::string temp = varName;
					if (temp == "") { temp = " "; }
					StringTokenizer tokenizer(temp, "/", builder, 1);
					UserType utTemp("");
					if (false == LoadDataFromString(data, utTemp)) {
						return false;
					}
					while (tokenizer.hasMoreTokens()) {
						std::string _varName = tokenizer.nextToken();
						/// todo - if varName is "" then data : val val val ... 
						if (_varName == "" || _varName == " " || _varName == "_") { // re?
							const int n = utTemp.GetItem("").size();
							for (int i = 0; i < finded.second.size(); ++i) {
								if (false == condition.empty()) {
									std::string _condition = condition;
									if (_varName == "" || _varName == " " || _varName == "_") { _condition = wiz::String::replace(_condition, "~~", "_"); }
									else
										_condition = wiz::String::replace(_condition, "~~", _varName); //

									_condition = ToBool4(finded.second[i], global, _condition, excuteData, builder);

									Condition cond(_condition, finded.second[i], &global, builder);

									while (cond.Next());

									if (cond.Now().size() != 1 || "TRUE" != cond.Now()[0])
									{
										//	std::cout << cond.Now()[0] << endl;
										continue;
									}
								}
								finded.second[i]->RemoveItemList("");

								for (int j = 0; j < n; ++j) {
									finded.second[i]->AddItem("", utTemp.GetItem("")[j].Get(0));
								}
								isTrue = true;
							}
						}
						else {
							std::vector<std::string> strVec;

							if (_varName.size() >= 3 && _varName[0] == '[' && _varName[_varName.size() - 1] == ']')
							{
								StringTokenizer tokenizer2(_varName, std::vector<std::string>{ "[", "~", "]" }, builder, 1);
								while (tokenizer2.hasMoreTokens())
								{
									strVec.push_back(tokenizer2.nextToken());
								}
							}

							long long int a = 0, b = 0, Min = 0, Max = 0;
							bool chkInt = false;

							if (strVec.size() == 2 && Utility::IsInteger(strVec[0]) && Utility::IsInteger(strVec[1])) {
								chkInt = true;
								a = atoll(strVec[0].c_str());
								b = atoll(strVec[1].c_str());
								Min = std::min(a, b);
								Max = std::max(a, b);
							}
							for (long long x = Min; x <= Max; ++x) {
								if (strVec.size() == 2 && chkInt)
								{
									_varName = std::to_string(x);
								}
								else {}

								for (int i = 0; i < finded.second.size(); ++i) {
									if (false == condition.empty()) {
										std::string _condition = condition;
										if (_varName == "") { _condition = wiz::String::replace(_condition, "~~", "_"); }
										else
											_condition = wiz::String::replace(_condition, "~~", _varName); //

										_condition = ToBool4(finded.second[i], global, _condition, excuteData, builder);

										Condition cond(_condition, finded.second[i], &global, builder);

										while (cond.Next());

										if (cond.Now().size() != 1 || "TRUE" != cond.Now()[0])
										{
											//	std::cout << cond.Now()[0] << endl;
											continue;
										}
									}
									if (wiz::String::startsWith(_varName, "$it")) {
										int index = atoi(wiz::String::substring(_varName, 3).c_str());

										finded.second[i]->SetItem(index, data);
										isTrue = true;
									}
									else {
										finded.second[i]->SetItem(_varName, data); /// chk
										isTrue = true;
									}
								}

								// prevent from infinity loop.
								if (x == Max) { break; }
							}
						}
					}
					return isTrue;
				}
				else {
					return false;
				}
			}

			static bool SetData(UserType& global, const std::string& position, const int var_idx, const std::string& data, const std::string& condition, const ExcuteData& excuteData, wiz::StringBuilder* builder)
			{
				auto finded = UserType::Find(&global, position, builder);
				bool isTrue = false;

				if (finded.first) {
					UserType utTemp("");
					if (false == LoadDataFromString(data, utTemp)) {
						return false;
					}
					long long int a = 0, b = 0, Min = 0, Max = 0;

					for (long long x = Min; x <= Max; ++x) {
						for (int i = 0; i < finded.second.size(); ++i) {
							if (false == condition.empty()) {
								std::string _condition = condition;


								_condition = ToBool4(finded.second[i], global, _condition, excuteData, builder);

								Condition cond(_condition, finded.second[i], &global, builder);

								while (cond.Next());

								if (cond.Now().size() != 1 || "TRUE" != cond.Now()[0])
								{
									//	std::cout << cond.Now()[0] << endl;
									continue;
								}
							}
							finded.second[i]->SetItem(var_idx, data); /// chk
							isTrue = true;
						}

						// prevent from infinity loop.
						if (x == Max) { break; }
					}
					return isTrue;
				}
				else {
					return false;
				}
			}
			/// 
			static std::string GetData(UserType& global, const std::string& position, const std::string& condition, const ExcuteData& excuteData, wiz::StringBuilder* builder) {
				std::string str;
				auto finded = UserType::Find(&global, position, builder);
				if (finded.first) {
					for (int i = 0; i < finded.second.size(); ++i) {
						if (false == condition.empty()) {
							std::string _condition = condition;

							_condition = ToBool4(finded.second[i], global, _condition, excuteData, builder);

							Condition cond(_condition, finded.second[i], &global, builder);

							while (cond.Next());

							if (cond.Now().size() != 1 || "TRUE" != cond.Now()[0])
							{
								//	std::cout << cond.Now()[0] << endl;
								continue;
							}
						}
						str = str + finded.second[i]->ToString() + "\n";
					}
					return str;
				}
				else {
					return "";
				}
			}
			static std::string GetItemListData(UserType& global, const std::string& position, const std::string& condition, const ExcuteData& excuteData, wiz::StringBuilder* builder)
			{
				std::string str;
				auto finded = UserType::Find(&global, position, builder);
				if (finded.first) {
					for (int i = 0; i < finded.second.size(); ++i) {
						if (false == condition.empty()) {
							std::string _condition = condition;

							_condition = ToBool4(finded.second[i], global, _condition, excuteData, builder);

							Condition cond(_condition, finded.second[i], &global, builder);

							while (cond.Next());

							if (cond.Now().size() != 1 || "TRUE" != cond.Now()[0])
							{
								//	std::cout << cond.Now()[0] << endl;
								continue;
							}
						}
						str = str + finded.second[i]->ItemListToString() + "\n";
					}
					return str;
				}
				else {
					return "";
				}
			}
			static std::string GetItemListNamesData(UserType& global, const std::string& position, const std::string& condition, const ExcuteData& excuteData, wiz::StringBuilder* builder)
			{
				std::string str;
				auto finded = UserType::Find(&global, position, builder);
				if (finded.first) {
					for (int i = 0; i < finded.second.size(); ++i) {
						if (false == condition.empty()) {
							std::string _condition = condition;

							_condition = ToBool4(finded.second[i], global, _condition, excuteData, builder);

							Condition cond(_condition, finded.second[i], &global, builder);

							while (cond.Next());

							if (cond.Now().size() != 1 || "TRUE" != cond.Now()[0])
							{
								//	std::cout << cond.Now()[0] << endl;
								continue;
							}
						}
						str = str + finded.second[i]->ItemListNamesToString() + "\n";
					}
					return str;
				}
				else {
					return "";
				}
			}
			static std::string GetUserTypeListNamesData(UserType& global, const std::string& position, const std::string& condition, const ExcuteData& excuteData, wiz::StringBuilder* builder)
			{
				std::string str;
				auto finded = UserType::Find(&global, position, builder);
				if (finded.first) {
					for (int i = 0; i < finded.second.size(); ++i) {
						if (false == condition.empty()) {
							std::string _condition = condition;

							_condition = ToBool4(finded.second[i], global, _condition, excuteData, builder);

							Condition cond(_condition, finded.second[i], &global, builder);

							while (cond.Next());

							if (cond.Now().size() != 1 || "TRUE" != cond.Now()[0])
							{
								//	std::cout << cond.Now()[0] << endl;
								continue;
							}
						}
						str = str + finded.second[i]->UserTypeListNamesToString() + "\n";
					}
					return str;
				}
				else {
					return "";
				}
			}
			/// varName = val - do
			/// varName = { val val val } - GetData(position+"/varName", ""); 
			/// varName = { var = val } - GetData(position+"/varname", var);
			static std::string GetData(UserType& global, const std::string& position, const std::string& varName, const std::string& condition, const ExcuteData& excuteData, wiz::StringBuilder* builder) // 
			{
				std::string str;
				std::string _var = varName;
				if (_var == " " || _var == "_") { _var = ""; }

				auto finded = UserType::Find(&global, position, builder);
				if (finded.first) {
					for (int i = 0; i < finded.second.size(); ++i) {
						if (false == condition.empty()) {
							std::string _condition = condition;

							// ~~ and ^ -> do not used other mean?
							if (_var == "") { _condition = wiz::String::replace(_condition, "~~", "_"); }
							else
								_condition = wiz::String::replace(_condition, "~~", _var); /// varName -> _var.

							_condition = ToBool4(finded.second[i], global, _condition, excuteData, builder);

							Condition cond(_condition, finded.second[i], &global, builder);

							while (cond.Next());

							if (cond.Now().size() != 1 || "TRUE" != cond.Now()[0])
							{
								//	std::cout << cond.Now()[0] << endl;
								continue;
							}
						}

						if (wiz::String::startsWith(_var, "$it")) {
							int itemIdx = stoi(wiz::String::substring(_var, 3));

							str = str + ToString(finded.second[i]->GetItemList(itemIdx).Get(0)) + "\n";
						}
						else {
							const int num = finded.second[i]->GetItem(_var).size();
							for (int k = 0; k < num; ++k) {
								str = str + ToString(finded.second[i]->GetItem(_var)[k].Get(0)) + "\n";
							}
						}
					}
				}
				return str;
			}

			static bool Remove(UserType& global, const std::string& position, const std::string& var, const std::string& condition, const ExcuteData& excuteData, wiz::StringBuilder* builder) {
				auto finded = UserType::Find(&global, position, builder);
				bool isTrue = false;

				if (finded.first) {
					std::string temp = var;
					if (temp == "") { temp = " "; }
					StringTokenizer tokenizer(temp, "/", builder, 1);
					while (tokenizer.hasMoreTokens()) {
						std::string _var = tokenizer.nextToken();
						if (_var == " " || _var == "_") { _var = ""; }
						std::vector<std::string> strVec;

						if (_var.size() >= 3 && _var[0] == '[' && _var[_var.size() - 1] == ']')
						{
							StringTokenizer tokenizer2(_var, std::vector<std::string>{ "[", "~", "]" }, builder, 1);
							while (tokenizer2.hasMoreTokens())
							{
								strVec.push_back(tokenizer2.nextToken());
							}
						}

						long long int a = 0, b = 0, Min = 0, Max = 0;
						bool chkInt = false;

						if (strVec.size() == 2 && Utility::IsInteger(strVec[0]) && Utility::IsInteger(strVec[1])) {
							chkInt = true;
							a = atoll(strVec[0].c_str());
							b = atoll(strVec[1].c_str());
							Min = std::min(a, b);
							Max = std::max(a, b);
						}
						for (long long x = Min; x <= Max; ++x) {
							if (strVec.size() == 2 && chkInt)
							{
								_var = std::to_string(x);
							}
							else {}

							for (int i = 0; i < finded.second.size(); ++i) {
								UserType* temp = finded.second[i];

								if (false == condition.empty()) {
									std::string _condition = condition;
									if (_var == "") { _condition = wiz::String::replace(_condition, "~~", "_"); }
									else
										_condition = wiz::String::replace(_condition, "~~", _var); //

									_condition = ToBool4(finded.second[i], global, _condition, excuteData, builder);

									Condition cond(_condition, finded.second[i], &global, builder);
									while (cond.Next());

									if (cond.Now().size() != 1 || "TRUE" != cond.Now()[0])
									{
										// std::cout << cond.Now()[0] << endl;
										continue;
									}
								}

								temp->RemoveItemList(_var);
								temp->RemoveUserTypeList(_var);
								isTrue = true;
							}

							// prevent from infinity loop.
							if (x == Max) { break; }
						}
					}
					return isTrue;
				}
				else {
					return false;
				}
			}
			static bool Remove(UserType& global, const std::string& position, const std::string& condition, const ExcuteData& excuteData, wiz::StringBuilder* builder) {
				auto finded = UserType::Find(&global, position, builder);
				bool isTrue = false;

				if (finded.first) {
					for (int i = 0; i < finded.second.size(); ++i) {
						UserType* temp = finded.second[i];

						if (false == condition.empty()) {
							std::string _condition = condition;

							_condition = ToBool4(finded.second[i], global, _condition, excuteData, builder);

							Condition cond(_condition, finded.second[i], &global, builder);

							while (cond.Next());

							if (cond.Now().size() != 1 || "TRUE" != cond.Now()[0])
							{
								// std::cout << cond.Now()[0] << endl;
								continue;
							}
						}

						temp->Remove();
						isTrue = true;
					}
					return isTrue;
				}
				else {
					return false;
				}
			}
			static bool RemoveUserType(UserType& global, const std::string& position, const std::string& name, const std::string& condition, const ExcuteData& excuteData, wiz::StringBuilder* builder) {
				auto finded = UserType::Find(&global, position, builder);
				bool isTrue = false;

				if (finded.first) {
					for (int i = 0; i < finded.second.size(); ++i) {
						UserType* temp = finded.second[i];

						if (false == condition.empty()) {
							std::string _condition = condition;

							_condition = ToBool4(finded.second[i], global, _condition, excuteData, builder);

							Condition cond(_condition, finded.second[i], &global, builder);

							while (cond.Next());

							if (cond.Now().size() != 1 || "TRUE" != cond.Now()[0])
							{
								// std::cout << cond.Now()[0] << endl;
								continue;
							}
						}

						temp->RemoveUserTypeList(name);
						isTrue = true;
					}
					return isTrue;
				}
				else {
					return false;
				}
			}


			static bool RemoveItemType(UserType& global, const std::string& position, const std::string& name, const std::string& condition, const ExcuteData& excuteData, wiz::StringBuilder* builder) {
				auto finded = UserType::Find(&global, position, builder);
				bool isTrue = false;

				if (finded.first) {
					for (int i = 0; i < finded.second.size(); ++i) {
						UserType* temp = finded.second[i];

						if (false == condition.empty()) {
							std::string _condition = condition;

							_condition = ToBool4(finded.second[i], global, _condition, excuteData, builder);

							Condition cond(_condition, finded.second[i], &global, builder);

							while (cond.Next());

							if (cond.Now().size() != 1 || "TRUE" != cond.Now()[0])
							{
								// std::cout << cond.Now()[0] << endl;
								continue;
							}
						}

						temp->RemoveItemList(name);
						isTrue = true;
					}
					return isTrue;
				}
				else {
					return false;
				}
			}

			// todo - static bool Remove(UserType& global, const std::string& positiion, oonst int idx, const std::string& condition, const ExcuteData& excuteData, wiz::StringBuilder* builder)
			static bool Remove(UserType& global, const std::string& position, const int idx, const std::string& condition, const ExcuteData& excuteData, wiz::StringBuilder* builder) {
				auto finded = UserType::Find(&global, position, builder);
				bool isTrue = false;

				if (finded.first) {
					for (int i = 0; i < finded.second.size(); ++i) {
						UserType* temp = finded.second[i];

						if (false == condition.empty()) {
							std::string _condition = condition;

							_condition = ToBool4(finded.second[i], global, _condition, excuteData, builder);

							Condition cond(_condition, finded.second[i], &global, builder);

							while (cond.Next());

							if (cond.Now().size() != 1 || "TRUE" != cond.Now()[0])
							{
								// std::cout << cond.Now()[0] << endl;
								continue;
							}
						}

						temp->RemoveList(idx);
						isTrue = true;
					}
					return isTrue;
				}
				else {
					return false;
				}
			}

			static bool LoadWizDB(UserType& global, const std::string& fileName) {
				UserType globalTemp = UserType("global");

				// Scan + Parse 
				if (false == LoadDataFromFile(fileName, globalTemp)) { return false; }
				std::cout << "LoadData End" << std::endl;

				global = std::move(globalTemp);
				return true;
			}
			// SaveQuery
			static bool SaveWizDB(const UserType& global, const std::string& fileName, const std::string& option = "0", const std::string& option2 = "") { /// , int option
				std::ofstream outFile;
				if (fileName.empty()) { return false; }
				if (option2 == "") {
					outFile.open(fileName);
					if (outFile.fail()) { return false; }
				}
				else {
					outFile.open(fileName, std::ios::app);
					if (outFile.fail()) { return false; }

					outFile << "\n";
				}

				/// saveFile
				if (option == "1") { // for eu4.
					global.Save1(outFile); // cf) friend
				}
				else if (option == "2") {
					global.Save2(outFile);
				}
				else if (option == "3") {
					global.SaveWithJson(outFile);
				}
				else if (option == "3_2") {
					global.SaveWithJson2(outFile);
				}
				else if (option == "4") {
					UserType temp;
					ConvertClauToHtml(&global, &temp);
					temp.SaveWithHtml(outFile);
				}
				else if (option == "5") {
					global.SaveWithHtml2(outFile);
				}

				outFile.close();

				return true;
			}

			/// To Do - ExistItem, ExistUserType, SetUserType GetUserType
			static bool ExistData(UserType& global, const std::string& position, const std::string& varName, const std::string& condition, const ExcuteData& excuteData, wiz::StringBuilder* builder) // 
			{
				int count = 0;
				std::string _var = varName;
				if (_var == " " || _var == "_") { _var = ""; }

				auto finded = UserType::Find(&global, position, builder);
				if (finded.first) {
					for (int i = 0; i < finded.second.size(); ++i) {
						if (false == condition.empty()) {
							std::string _condition = condition;

							_condition = ToBool4(finded.second[i], global, _condition, excuteData, builder);

							Condition cond(_condition, finded.second[i], &global, builder);

							while (cond.Next());

							if (cond.Now().size() != 1 || "TRUE" != cond.Now()[0])
							{
								//	std::cout << cond.Now()[0] << endl;
								continue;
							}
						}
						if (wiz::String::startsWith(_var, "$it")) {
							int itemIdx = stoi(wiz::String::substring(_var, 3));
							count = count + (finded.second[i]->GetItemList(itemIdx).size());
						}
						else {
							count = count + (finded.second[i]->GetItem(_var).size());
						}
					}
				}
				return 0 != count;
			}
			static bool ExistUserType(UserType& global, const std::string& position, const std::string& condition,
				const ExcuteData& excuteData, wiz::StringBuilder* builder) // 
			{
				int count = 0;

				auto finded = UserType::Find(&global, position, builder);
				if (finded.first) {
					for (int i = 0; i < finded.second.size(); ++i) {
						if (false == condition.empty()) {
							std::string _condition = condition;

							_condition = ToBool4(finded.second[i], global, _condition, excuteData, builder);

							Condition cond(_condition, finded.second[i], &global, builder);

							while (cond.Next());

							if (cond.Now().size() != 1 || "TRUE" != cond.Now()[0])
							{
								//	std::cout << cond.Now()[0] << endl;
								continue;
							}
						}
						count = count + (finded.second[i]->GetUserTypeListSize());
					}
				}
				return 0 != count;
			}
			static bool ExistOneUserType(UserType& global, const std::string& position, const std::string& condition,
				const ExcuteData& excuteData, wiz::StringBuilder* builder) // 
			{
				int count = 0;

				auto finded = UserType::Find(&global, position, builder);
				if (finded.second[0] == &global) {
					return true;
				}
				if (finded.first) {
					for (int i = 0; i < finded.second.size(); ++i) {
						if (false == condition.empty()) {
							std::string _condition = condition;

							_condition = ToBool4(finded.second[i], global, _condition, excuteData, builder);

							Condition cond(_condition, finded.second[i], &global, builder);

							while (cond.Next());

							if (cond.Now().size() != 1 || "TRUE" != cond.Now()[0])
							{
								//	std::cout << cond.Now()[0] << endl;
								continue;
							}
						}
						count = count + (finded.second[i]->GetUserTypeListSize());
					}
				}
				return 1 == count;
			}
			static bool ExistItem(UserType& global, const std::string& position, const std::string& varName, const std::string& condition,
				const ExcuteData& excuteData, wiz::StringBuilder* builder) // 
			{
				int count = 0;
				std::string _var = varName;
				if (_var == " " || _var == "_") { _var = ""; }

				auto finded = UserType::Find(&global, position, builder);
				if (finded.first) {
					for (int i = 0; i < finded.second.size(); ++i) {
						if (false == condition.empty()) {
							std::string _condition = condition;

							_condition = ToBool4(finded.second[i], global, _condition, excuteData, builder);

							Condition cond(_condition, finded.second[i], &global, builder);

							while (cond.Next());

							if (cond.Now().size() != 1 || "TRUE" != cond.Now()[0])
							{
								//	std::cout << cond.Now()[0] << endl;
								continue;
							}
						}
						if (wiz::String::startsWith(_var, "$it")) {
							int itemIdx = stoi(wiz::String::substring(_var, 3));
							count = count + (finded.second[i]->GetItemList(itemIdx).size());
						}
						else {
							count = count + (finded.second[i]->GetItem(_var).size());
						}
					}
				}
				return 0 != count;
			}

			/// ToDo - global, position, var, condition + var is " "!
			// "root" -> position.
			static std::string SearchItem(UserType& global, const std::string& var, const std::string& condition,
				const ExcuteData& excuteData,
				const std::string& start_dir = "root",
				StringBuilder* builder = nullptr)
			{
				std::vector<std::string> positionVec;
				std::string temp;

				SearchItem(global, positionVec, var, start_dir, &global, condition, excuteData, builder);

				for (int i = 0; i < positionVec.size(); ++i)
				{
					temp = temp + positionVec[i] + "\n";
				}

				return temp;
			}
			static std::string SearchUserType(UserType& global, const std::string& var, const std::string& condition,
				const ExcuteData& excuteData, wiz::StringBuilder* builder)
			{
				std::vector<std::string> positionVec;
				std::string temp;

				SearchUserType(global, positionVec, var, "root", &global, condition, excuteData, builder);

				for (int i = 0; i < positionVec.size(); ++i)
				{
					temp = temp + positionVec[i] + "\n";
				}

				return temp;
			}
			static void ReplaceItem(UserType& global, const std::string& var, const std::string& val, const std::string& condition, const std::string& start_dir,
				const ExcuteData& excuteData, bool recursive, wiz::StringBuilder* builder)
			{
				auto temp = wiz::load_data::UserType::Find(&global, start_dir, builder);
				if (temp.first) {
					for (int i = 0; i < temp.second.size(); ++i) // chk!!
					{
						UserType* ut = temp.second[i];
						ReplaceItem(global, var, start_dir, ut, val, condition, excuteData, builder, recursive);
					}
				}
			}
			static void RemoveUserTypeTotal(UserType& global, const std::string& ut_name, const std::string& condition, const std::string& start_dir,
				const ExcuteData& excuteData, bool recursive, wiz::StringBuilder* builder)
			{
				auto temp = wiz::load_data::UserType::Find(&global, start_dir, builder);
				if (temp.first) {
					for (int i = 0; i < temp.second.size(); ++i) // chk!!
					{
						UserType* ut = temp.second[i];
						RemoveUserTypeTotal(global, ut_name, start_dir, ut, condition, excuteData, builder, recursive);

					}
				}
			}
			static void ReplaceDateType(UserType& global, const std::string& val, const std::string& condition, const std::string& start_dir,
				const ExcuteData& excuteData, bool recursive, wiz::StringBuilder* builder
			) {
				auto temp = wiz::load_data::UserType::Find(&global, start_dir, builder);
				if (temp.first) {
					for (int i = 0; i < temp.second.size(); ++i) // chk!!
					{
						UserType* ut = temp.second[i];
						ReplaceDateType(global, start_dir, ut, val, condition, excuteData, builder, recursive);
					}
				}
			}
			static void ReplaceDateType2(UserType& global, const std::string& val, const std::string& condition, const std::string& start_dir,
				const ExcuteData& excuteData, bool recursive, wiz::StringBuilder* builder)
			{
				auto temp = wiz::load_data::UserType::Find(&global, start_dir, builder);
				if (temp.first) {
					for (int i = 0; i < temp.second.size(); ++i) // chk!!
					{
						UserType* ut = temp.second[i];
						ReplaceDateType2(global, start_dir, ut, val, condition, excuteData, builder, recursive);
					}
				}
			}
			static void ReplaceDataType1(UserType& global, const std::string& rex, const std::vector<std::string>& val, const std::vector<std::string>& condition,
				const std::string& start_dir, const ExcuteData& excuteData, bool recursive, wiz::StringBuilder* builder)
			{
				std::regex rgx(rex);
				auto temp = wiz::load_data::UserType::Find(&global, start_dir, builder);
				if (temp.first) {
					for (int i = 0; i < temp.second.size(); ++i) // chk!!
					{
						UserType* ut = temp.second[i];
						ReplaceDataType1(global, start_dir, ut, rgx, val, condition, excuteData, builder, recursive);
					}
				}
			}
			static void ReplaceDataType1_2(UserType& global, const std::string& rex, const std::vector<std::string>& val, const std::vector<std::string>& condition,
				const std::string& start_dir, const ExcuteData& excuteData, bool recursive, wiz::StringBuilder* builder)
			{
				std::regex rgx(rex);

				auto temp = wiz::load_data::UserType::Find(&global, start_dir, builder);
				if (temp.first) {
					for (int i = 0; i < temp.second.size(); ++i) // chk!!
					{
						UserType* ut = temp.second[i];

						ReplaceDataType1_2(global, start_dir, ut, rgx, val, condition, excuteData, builder, recursive);
					}
				}
			}
			static void ReplaceDataType2(UserType& global, const std::string& rex, const std::vector<std::string>& val, const std::vector<std::string>& condition,
				const std::string& start_dir, const ExcuteData& excuteData, bool recursive, wiz::StringBuilder* builder)
			{
				std::regex rgx(rex);

				auto temp = wiz::load_data::UserType::Find(&global, start_dir, builder);
				if (temp.first) {
					for (int i = 0; i < temp.second.size(); ++i) // chk!!
					{
						UserType* ut = temp.second[i];
						ReplaceDataType2(global, start_dir, ut, rgx, val, condition, excuteData, builder, recursive);
					}
				}
			}
		private:
			static void ReplaceItem(UserType& global, const std::string& var, const std::string& nowPosition,
				UserType* ut, const std::string& val, const std::string& condition, const ExcuteData& excuteData, wiz::StringBuilder* builder, bool recursive)
			{
				std::string _var = var;
				if (_var == " " || _var == "_") { _var = ""; }

				for (int i = 0; i < ut->GetItemListSize(); ++i) {
					if (ut->GetItemList(i).GetName().ToString() == _var) {
						std::string _condition = condition;

						if (_var == "") { _condition = wiz::String::replace(_condition, "~~", "_"); }
						else {
							_condition = wiz::String::replace(_condition, "~~~", val); //
							_condition = wiz::String::replace(_condition, "~~", _var); //
						}
						if (nowPosition == "") {
							_condition = wiz::String::replace(_condition, "////", "@/./");
						}
						else {
							_condition = wiz::String::replace(_condition, "////", "@" + nowPosition);
						}
						_condition = wiz::String::replace(_condition, "///", wiz::_toString(i));
						_condition = ToBool4(ut, global, _condition, excuteData, builder);

						Condition cond(_condition, ut, &global, builder);

						while (cond.Next());

						if (cond.Now().size() == 1 && "TRUE" == cond.Now()[0])
						{
							std::string _val = val;
							_val = wiz::String::replace(_val, "~~~", val); //
							_val = wiz::String::replace(_val, "~~", _var); //

							if (nowPosition == "") {
								_val = wiz::String::replace(_val, "////", "@/./");
							}
							else {
								_val = wiz::String::replace(_val, "////", "@" + nowPosition);
							}
							_val = wiz::String::replace(_val, "///", wiz::_toString(i));
							_val = ToBool4(ut, global, _val, excuteData, builder);

							ut->GetItemList(i).Set(0, _val);
						}
					}
				}

				if (recursive) {
					for (int i = 0; i < ut->GetUserTypeListSize(); ++i) {
						std::string temp = wiz::ToString(ut->GetUserTypeList(i)->GetName());
						if (temp == "") { temp = " "; }
						ReplaceItem(
							global,
							_var,
							nowPosition + "/" + temp,
							ut->GetUserTypeList(i),
							val,
							condition,
							excuteData,
							builder,
							recursive
						);
					}
				}
			}
			static void RemoveUserTypeTotal(UserType& global, const std::string& ut_name, const std::string& nowPosition,
				UserType* ut, const std::string& condition,
				const ExcuteData& excuteData, wiz::StringBuilder* builder, bool recursive)
			{
				std::string _var = ut_name;

				if (_var == " " || _var == "_") { _var = ""; }

				for (int i = 0; i < ut->GetUserTypeListSize(); ++i) {
					if (ut->GetUserTypeList(i)->GetName().ToString() == _var) {
						std::string _condition = condition;

						if (_var == "") { _condition = wiz::String::replace(_condition, "~~", "_"); }
						else {
							_condition = wiz::String::replace(_condition, "~~", _var); //
						}

						if (nowPosition == "") {
							_condition = wiz::String::replace(_condition, "////", "@/./");
						}
						else {
							_condition = wiz::String::replace(_condition, "////", "@" + nowPosition);
						}

						_condition = ToBool4(ut, global, _condition, excuteData, builder);

						Condition cond(_condition, ut, &global, builder);

						while (cond.Next());

						if (cond.Now().size() == 1 && "TRUE" == cond.Now()[0])
						{
							ut->RemoveUserTypeList(i);
							--i;
						}
					}
				}
				/*
				if (recursive) {
					for (int i = 0; i < ut->GetUserTypeListSize(); ++i) {
						std::string temp = ut->GetUserTypeList(i)->GetName();
						if (temp == "") { temp = " "; }
						RemoveUserTypeTotal(
							global,
							_var,
							nowPosition + "/" + temp,
							ut->GetUserTypeList(i),
							condition,
							excuteData,
							builder,
							recursive
						);
					}
				}
				*/
			}
			static void ReplaceDateType(UserType& global, const std::string& nowPosition,
				UserType* ut, const std::string& val, const std::string& condition,
				const ExcuteData& excuteData, wiz::StringBuilder* builder, bool recursive)
			{
				for (int i = 0; i < ut->GetItemListSize(); ++i) {
					std::string name = wiz::ToString(ut->GetItemList(i).GetName());
					std::string value = ToString(ToString(ut->GetItemList(i).Get(0)));

					if (name.size() > 2 && name[0] == name.back() && name.back() == '\"')
					{
						name = name.substr(1, name.size() - 2);
					}
					if (value.size() > 2 && value[0] == value.back() && value.back() == '\"')
					{
						value = value.substr(1, value.size() - 2);
					}

					if (Utility::IsDate(name) || Utility::IsDate(value)) {
						std::string _condition = condition;
						std::string _val = val;
						std::string _var = wiz::ToString(ut->GetItemList(i).GetName());

						if (_var.empty()) {
							_var = "_";
						}

						_condition = wiz::String::replace(_condition, "~~~", ToString(ut->GetItemList(i).Get(0)));
						_condition = wiz::String::replace(_condition, "~~", _var); //
						if (nowPosition == "") {
							_condition = wiz::String::replace(_condition, "////", "@/./");
						}
						else {
							_condition = wiz::String::replace(_condition, "////", "@" + nowPosition);
						}
						_condition = wiz::String::replace(_condition, "///", wiz::_toString(i));

						_condition = ToBool4(ut, global, _condition, excuteData, builder);

						Condition cond(_condition, ut, &global, builder);

						while (cond.Next())
						{
							//
						}

						if (cond.Now().size() == 1 && "TRUE" == cond.Now()[0])
						{
							_val = wiz::String::replace(_val, "~~~", ToString(ut->GetItemList(i).Get(0)));
							_val = wiz::String::replace(_val, "~~", _var); //

							if (nowPosition == "") {
								_val = wiz::String::replace(_val, "////", "@/./");
							}
							else {
								_val = wiz::String::replace(_val, "////", "@" + nowPosition);
							}
							_val = wiz::String::replace(_val, "///", wiz::_toString(i));
							_val = ToBool4(ut, global, _val, excuteData, builder);


							// todo? - split two function?
							if (Utility::IsDate(name)) {
								ut->GetItemList(i).SetName(_val);
							}
							if (Utility::IsDate(value)) {
								ut->GetItemList(i).Set(0, _val);
							}
						}
					}
				}
				if (recursive) {
					for (int i = 0; i < ut->GetUserTypeListSize(); ++i) {
						std::string temp = wiz::ToString(ut->GetUserTypeList(i)->GetName());
						if (temp == "") { temp = " "; }

						ReplaceDateType(
							global,
							nowPosition + "/" + temp,
							ut->GetUserTypeList(i),
							val,
							condition,
							excuteData,
							builder,
							recursive
						);
					}
				}
			}
			static void ReplaceDateType2(UserType& global, const std::string& nowPosition,
				UserType* ut, const std::string& val, const std::string& condition,
				const ExcuteData& excuteData, wiz::StringBuilder* builder, bool recursive)
			{
				for (int i = 0; i < ut->GetUserTypeListSize(); ++i) {
					std::string temp = wiz::ToString(ut->GetUserTypeList(i)->GetName());
					if (temp == "") { temp = " "; }

					if (temp.size() > 2 && temp[0] == temp.back() && temp.back() == '\"')
					{
						temp = temp.substr(1, temp.size() - 2);
					}

					if (Utility::IsDate(temp)) {
						std::string _condition = condition;
						std::string _val = val;

						std::string _var = wiz::ToString(ut->GetUserTypeList(i)->GetName());

						_condition = wiz::String::replace(_condition, "~~", _var); //

						if (nowPosition == "") {
							_condition = wiz::String::replace(_condition, "////", "@/./");
						}
						else {
							_condition = wiz::String::replace(_condition, "////", "@" + nowPosition);
						}

						_condition = wiz::String::replace(_condition, "///", wiz::_toString(i));

						_condition = ToBool4(ut, global, _condition, excuteData, builder);


						Condition cond(_condition, ut, &global, builder);

						while (cond.Next()) {
						//	for (int i = 0; i < cond.Now().size(); ++i) {
						//		std::cout << cond.Now()[i] << " ";
						//	}
						//	std::cout << std::endl;
							//GETCH();
						}

						if (cond.Now().size() == 1 && "TRUE" == cond.Now()[0])
						{
							_val = wiz::String::replace(_val, "~~", _var); //

							if (nowPosition == "") {
								_val = wiz::String::replace(_val, "////", "@/./");
							}
							else {
								_val = wiz::String::replace(_val, "////", "@" + nowPosition);
							}
							_val = ToBool4(ut, global, _val, excuteData, builder);

							ut->GetUserTypeList(i)->SetName(_val);
						}
						else {
						//	if (cond.Now().size() != 1 || "Error" == cond.Now()[0]) {
							//	for (int i = 0; i < cond.Now().size(); ++i) {
							//		std::cout << cond.Now()[i] << " ";
							//	}
							//	std::cout << std::endl;
							//	GETCH();
							//}
						}
					}
					if (recursive) {
						ReplaceDateType2(
							global,
							nowPosition + "/" + temp,
							ut->GetUserTypeList(i),
							val,
							condition,
							excuteData,
							builder,
							recursive
						);
					}
				}
			}
			static void ReplaceDataType1(UserType& global, const std::string& nowPosition,
				UserType* ut, const std::regex& rgx, const std::vector<std::string>& val, const std::vector<std::string>& condition,
				const ExcuteData& excuteData, wiz::StringBuilder* builder, bool recursive)
			{
				for (int i = 0; i < ut->GetItemListSize(); ++i) {
					if (std::regex_match(wiz::ToString(ut->GetItemList(i).GetName()), rgx)) { // || Utility::IsDate(ut->GetItemList(i).Get(0))) {
						for (int k = 0; k < val.size(); ++k) {
							std::string _condition = condition[k];
							std::string _val = val[k];
							std::string _var = wiz::ToString(ut->GetItemList(i).GetName());


							_condition = wiz::String::replace(_condition, "~~~", ToString(ut->GetItemList(i).Get(0)));
							_condition = wiz::String::replace(_condition, "~~", _var); //
							if (nowPosition == "") {
								_condition = wiz::String::replace(_condition, "////", "@/./");
							}
							else {
								_condition = wiz::String::replace(_condition, "////", "@" + nowPosition);
							}
							_condition = wiz::String::replace(_condition, "///", wiz::_toString(i));
							_condition = ToBool4(ut, global, _condition, excuteData, builder);


							Condition cond(_condition, ut, &global, builder);

							while (cond.Next());

							if (cond.Now().size() == 1 && "TRUE" == cond.Now()[0])
							{
								_val = wiz::String::replace(_val, "~~~", ToString(ut->GetItemList(i).Get(0)));
								_val = wiz::String::replace(_val, "~~", wiz::ToString(ut->GetItemList(i).GetName()));

								if (nowPosition == "") {
									_val = wiz::String::replace(_val, "////", "@/./");
								}
								else {
									_val = wiz::String::replace(_val, "////", "@" + nowPosition);
								}
								_val = wiz::String::replace(_val, "///", wiz::_toString(i));
								_val = ToBool4(ut, global, _val, excuteData, builder);

								//if (_val[0] == '@') { _val.erase(_val.begin()); }
								//_val = wiz::String::replace(_val, "~~", _var);
								//_val = ToBool4(ut, global, map<std::string, std::string>(), _val, EventInfo(), objectMapPtr,  excuteData.pEvents);

								ut->GetItemList(i).SetName(_val);
								break;
							}
						}
					}
				}
				if (recursive) {
					for (int i = 0; i < ut->GetUserTypeListSize(); ++i) {
						std::string temp = wiz::ToString(ut->GetUserTypeList(i)->GetName());
						if (temp == "") { temp = " "; }

						ReplaceDataType1(
							global,
							nowPosition + "/" + temp,
							ut->GetUserTypeList(i),
							rgx,
							val,
							condition,
							excuteData,
							builder,
							recursive
						);
					}
				}
			}
			static void ReplaceDataType1_2(UserType& global, const std::string& nowPosition,
				UserType* ut, const std::regex& rgx, const std::vector<std::string>& val, const std::vector<std::string>& condition,
				const ExcuteData& excuteData, wiz::StringBuilder* builder, bool recursive)
			{
				for (int i = 0; i < ut->GetItemListSize(); ++i) {
					if (std::regex_match(wiz::ToString(ut->GetItemList(i).GetName()), rgx)) { // || Utility::IsDate(ut->GetItemList(i).Get(0))) {
						for (int k = 0; k < val.size(); ++k) {
							std::string _condition = condition[k];
							std::string _val = val[k];
							std::string _var = wiz::ToString(ut->GetItemList(i).GetName());
							// ~~~ -> _val?

							_condition = wiz::String::replace(_condition, "~~~", ToString(ut->GetItemList(i).Get(0)));
							_condition = wiz::String::replace(_condition, "~~", _var); //
							if (nowPosition == "") {
								_condition = wiz::String::replace(_condition, "////", "@/./");
							}
							else {
								_condition = wiz::String::replace(_condition, "////", "@/./" + nowPosition);
							}
							_condition = wiz::String::replace(_condition, "///", wiz::_toString(i));
							_condition = ToBool4(ut, global, _condition, excuteData, builder);

							Condition cond(_condition, ut, &global, builder);

							while (cond.Next());

							if (cond.Now().size() == 1 && "TRUE" == cond.Now()[0])
							{
								_val = wiz::String::replace(_val, "~~~", ToString(ut->GetItemList(i).Get(0)));
								_val = wiz::String::replace(_val, "~~", _var); //
								if (nowPosition == "") {
									_val = wiz::String::replace(_val, "////", "@/./");
								}
								else {
									_val = wiz::String::replace(_val, "////", "@" + nowPosition);
								}
								_val = wiz::String::replace(_val, "///", wiz::_toString(i));
								_val = ToBool4(ut, global, _val, excuteData, builder);

								// remove 3 lines?
								//	if (_val[0] == '@') { _val.erase(_val.begin()); } // chk, same do(chk @) to other functions?
								//	_val = wiz::String::replace(_val, "~~", _var);
								//		_val = ToBool4(ut, global, map<std::string, std::string>(), _val, EventInfo(), objectMapPtr,  excuteData.pEvents); // chk!!

								ut->GetItemList(i).Set(0, _val);
								break;
							}
						}
					}
				}
				if (recursive) {
					for (int i = 0; i < ut->GetUserTypeListSize(); ++i) {
						std::string temp = wiz::ToString(ut->GetUserTypeList(i)->GetName());
						if (temp == "") { temp = " "; }

						ReplaceDataType1_2(
							global,
							nowPosition + "/" + temp,
							ut->GetUserTypeList(i),
							rgx,
							val,
							condition,
							excuteData,
							builder,
							recursive
						);
					}
				}
			}
			static void ReplaceDataType1_3(UserType& global, const std::string& nowPosition,
				UserType* ut, const std::regex& rgx, const std::vector<std::string>& val, const std::vector<std::string>& condition,
				const ExcuteData& excuteData, wiz::StringBuilder* builder, bool recursive)
			{
				for (int i = 0; i < ut->GetItemListSize(); ++i) {
					if (std::regex_match(ToString(ut->GetItemList(i).Get(0)), rgx)) {
						for (int k = 0; k < val.size(); ++k) {
							std::string _condition = condition[k];
							std::string _val = val[k];

							std::string _var = wiz::ToString(ut->GetItemList(i).GetName());


							_condition = wiz::String::replace(_condition, "~~~", ToString(ut->GetItemList(i).Get(0)));
							_condition = wiz::String::replace(_condition, "~~", _var); //
							if (nowPosition == "") {
								_condition = wiz::String::replace(_condition, "////", "@/./");
							}
							else {
								_condition = wiz::String::replace(_condition, "////", "@" + nowPosition);
							}
							_condition = wiz::String::replace(_condition, "///", wiz::_toString(i));
							_condition = ToBool4(ut, global, _condition, excuteData, builder);


							Condition cond(_condition, ut, &global, builder);

							while (cond.Next());

							if (cond.Now().size() == 1 && "TRUE" == cond.Now()[0])
							{
								_val = wiz::String::replace(_val, "~~~", ToString(ut->GetItemList(i).Get(0)));
								_val = wiz::String::replace(_val, "~~", _var); //
								if (nowPosition == "") {
									_val = wiz::String::replace(_val, "////", "@/./");
								}
								else {
									_val = wiz::String::replace(_val, "////", "@" + nowPosition);
								}
								_val = wiz::String::replace(_val, "///", wiz::_toString(i));
								_val = ToBool4(ut, global, _val, excuteData, builder);

								//if (_val[0] == '@') { _val.erase(_val.begin()); }
								//_val = wiz::String::replace(_val, "~~", _var);
								//_val = ToBool4(ut, global, map<std::string, std::string>(), _val, EventInfo(), objectMapPtr,  excuteData.pEvents);
								ut->GetItemList(i).Set(0, _val);
								break;
							}
						}
					}
				}
				if (recursive) {
					for (int i = 0; i < ut->GetUserTypeListSize(); ++i) {
						std::string temp = wiz::ToString(ut->GetUserTypeList(i)->GetName());
						if (temp == "") { temp = " "; }

						ReplaceDataType1_3(
							global,
							nowPosition + "/" + temp,
							ut->GetUserTypeList(i),
							rgx,
							val,
							condition,
							excuteData,
							builder,
							recursive
						);
					}
				}
			}

			// cf) replacedatatype1_4  match value, change var_name?

			static void ReplaceDataType2(UserType& global, const std::string& nowPosition,
				UserType* ut, const std::regex& rgx, const std::vector<std::string>& val, const std::vector<std::string>& condition,
				const ExcuteData& excuteData, wiz::StringBuilder* builder, bool recursive) // first val test
			{
				for (int i = 0; i < ut->GetUserTypeListSize(); ++i) {
					std::string temp = wiz::ToString(ut->GetUserTypeList(i)->GetName());
					if (temp == "") { temp = " "; }

					if (std::regex_match(temp, rgx)) {
						for (int k = 0; k < val.size(); ++k) {
							std::string _condition = condition[k];
							std::string _val = val[k];
							std::string _var = wiz::ToString(ut->GetUserTypeList(i)->GetName());


							_condition = wiz::String::replace(_condition, "~~", _var); //
							if (nowPosition == "") {
								_condition = wiz::String::replace(_condition, "////", "@/./");
							}
							else {
								_condition = wiz::String::replace(_condition, "////", "@" + nowPosition);
							}
							_condition = ToBool4(ut, global, _condition, excuteData, builder);



							Condition cond(_condition, ut, &global, builder);

							while (cond.Next());

							if (cond.Now().size() == 1 && "TRUE" == cond.Now()[0])
							{
								_val = wiz::String::replace(_val, "~~", _var); //
								if (nowPosition == "") {
									_val = wiz::String::replace(_val, "////", "@/./");
								}
								else {
									_val = wiz::String::replace(_val, "////", "@" + nowPosition);
								}
								_val = ToBool4(ut, global, _val, excuteData, builder);

								//if (_val[0] == '@') { _val.erase(_val.begin()); } // removal?

								//_val = wiz::String::replace(_val, "~~", _var);
								//_val = ToBool4(ut, global, map<std::string, std::string>(), _val, EventInfo(), objectMapPtr,  excuteData.pEvents);
								ut->GetUserTypeList(i)->SetName(_val);
								break;
							}
						}
					}
					if (recursive) {
						ReplaceDataType2(
							global,
							nowPosition + "/" + temp,
							ut->GetUserTypeList(i),
							rgx,
							val,
							condition,
							excuteData,
							builder,
							recursive
						);
					}
				}
			}
		public:
			// only one exist or do not exist
			static inline wiz::load_data::ItemType<WIZ_STRING_TYPE> GetItem(const wiz::load_data::UserType* ut, const std::string& name) {
				return wiz::load_data::ItemType<WIZ_STRING_TYPE>(ut->GetItem(name)[0].GetName(), ToString(ut->GetItem(name)[0].Get(0)));
			}

			static inline std::vector<wiz::load_data::UserType*> GetUserType(const wiz::load_data::UserType* ut, const std::string& name)
			{
				return ut->GetUserTypeItem(name);
			}
			static std::string Find(wiz::load_data::UserType* ut, const std::string& str, StringBuilder* builder)
			{ // std::string 대신 vector<std::string> ??
				int count = 0;
				int idx = -1;
				for (int i = str.size() - 1; i >= 0; --i) {
					if ('/' == str[i]) {
						if (count == 0) {
							idx = i;
						}
						count++;
					}
				}

				std::string result;

				if (count == 1)
				{
					return "";
				}
				else {
					auto x = wiz::load_data::UserType::Find(ut,
						wiz::String::substring(str, 0, idx), builder);
					if (x.first == false) { return ""; }
					for (int i = 0; i < x.second.size(); ++i) {
						std::string itemName = wiz::String::substring(str, idx + 1);
						if (wiz::String::startsWith(itemName, "$it") && itemName.size() >= 4) {
							int itemIdx = stoi(wiz::String::substring(itemName, 3));

							result += ToString(x.second[i]->GetItemList(itemIdx).Get(0));
							if (i < x.second.size() - 1) {
								result += "\n";
							}
						}
						else {
							if (itemName == "_") {
								itemName = "";
							}
							auto temp = x.second[i]->GetItem(itemName);
							if (!temp.empty()) {
								for (int j = 0; j < temp.size(); ++j) {
									result += ToString(temp[j].Get(0));
									if (j < temp.size() - 1) {
										result += " ";
									}
								}
								if (i < x.second.size() - 1) {
									result += "\n";
								}
							}
						}
					}
				}
				return result;
			}

			static inline bool Exist(wiz::load_data::UserType* ut, const std::string& dir, StringBuilder* builder)
			{
				auto x = wiz::load_data::UserType::Find(ut, dir, builder);
				return x.first;
			}
			// to do - rename!
			static std::pair<std::string, std::string> Find2(wiz::load_data::UserType* ut, const std::string& str)
			{
				int idx = -1;
				for (int i = str.size() - 1; i >= 0; --i) {
					if ('/' == str[i]) {
						idx = i;
						break;
					}
				}
				return{ wiz::String::substring(str, 0, idx), wiz::String::substring(str,idx + 1) };
			}

			static inline std::string FindParameters(const wiz::ArrayMap<std::string, std::string>& parameters, const std::string& operand)
			{
				wiz::ArrayMap<std::string, std::string>::const_iterator x;
				for (int i = 0; i < parameters.size(); ++i) {
					if (wiz::String::startsWith(operand, "$parameter.")
						&& (x = parameters.find(wiz::String::substring(operand, 11))) != parameters.end()) {
						return x->second;
					}
				}
				return "";
			}
			static inline std::string FindLocals(const wiz::ArrayMap<std::string, std::string>& locals, const std::string& operand)
			{
				if (wiz::String::startsWith(operand, "$local.") && locals.end() != locals.find(wiz::String::substring(operand, 7)))
				{
					return locals.at(wiz::String::substring(operand, 7));
				}
				return "";
			}
			//need to renewal. add $AND $OR $NOT

			/// remove /, parameter chk!!
			static bool operation(wiz::load_data::UserType* now, wiz::load_data::UserType& global, const WIZ_STRING_TYPE& str,
				wiz::ArrayStack<WIZ_STRING_TYPE>& operandStack, const ExcuteData& excuteData, wiz::StringBuilder* builder);

			static std::string ToBool3(wiz::load_data::UserType& global, const wiz::ArrayMap<std::string, std::string>& parameters, const std::string& temp,
				const EventInfo& info, StringBuilder* builder);
			static std::string ToBool3(wiz::load_data::UserType& global, const wiz::ArrayMap<std::string, std::string>& parameters, std::string&& temp,
				const EventInfo& info, StringBuilder* builder);
			// remove - 4_A, 4_B ?
			static std::pair<std::vector<std::string>, bool> ToBool4_A(wiz::load_data::UserType* now, wiz::load_data::UserType& global, const std::string& temp, const ExcuteData& excuteData, wiz::StringBuilder* builder)
			{
				std::string result = temp;

				//	cout << "result is " << result << endl;
				bool flag_A = false;
				if (result.size() > 1 && result[0] == '/')
				{
					flag_A = true;
				}

				wiz::ArrayStack<std::string> resultStack;

				std::vector<std::string> tokenVec;
				{
					wiz::StringTokenizer tokenizer(std::move(result), { " ", "\n", "\t", "\r" }, builder, 1); // , "{", "=", "}" }); //
																											  //wiz::StringTokenizer tokenizer2(result, { " ", "\n", "\t", "\r" } ); //

																											  //vector<std::string> tokenVec2;

					tokenVec.reserve(tokenizer.countTokens());
					while (tokenizer.hasMoreTokens()) {
						tokenVec.push_back(tokenizer.nextToken());
					}
				}
				return { tokenVec, flag_A };
			}
			static std::string ToBool4_B(wiz::load_data::UserType* now, wiz::load_data::UserType& global, std::vector<std::string>& tokenVec, const ExcuteData& excuteData, wiz::StringBuilder* builder)
			{

				bool chk = false;
				int count_change = 0;

				for (int i = 0; i < tokenVec.size(); ++i) {
					std::string result = tokenVec[i];
					bool flag_A = false;
					if (result.size() > 1 && result[0] == '/')
					{
						flag_A = true;
					}

					bool flag_B = false;
					//for (int j = 0; j < result.size(); ++j) {
					//	if (result[j] == '/') {
					//		flag_B = true;
					//		break;
					//	}
					//}
					if (flag_B) {
						//	result = ToBool3(global, excuteData.info.parameters, std::move(result), excuteData.info, builder);
					}
					if (result.empty()) {
						continue;
					}
					if (!flag_A && flag_B) {
						//	result = std::string(result.c_str() + 1, result.size() - 1);
					}

					{
						if ('/' == result[0] && result.size() > 1)
						{
							std::string _temp = Find(&global, result, builder);

							if ("" != _temp) {
								result = std::move(_temp);
							}
						}
						else if (wiz::String::startsWith(result, "$local.")) { // && length?
							std::string _temp = FindLocals(excuteData.info.locals, result);
							if (!_temp.empty()) {
								result = std::move(_temp);
							}
						}
						else if (wiz::String::startsWith(result, "$parameter.")) { // && length?
							std::string _temp = FindParameters(excuteData.info.parameters, result);
							if (!_temp.empty()) {
								result = std::move(_temp);
							}
						}
					}

					tokenVec[i] = std::move(result);
				}

				if (tokenVec.empty()) {
					return "";
				}
				if (1 == tokenVec.size())
				{
					return tokenVec[0];
				}

				std::string result;
				//
				wiz::ArrayStack<WIZ_STRING_TYPE> operandStack;
				wiz::ArrayStack<std::string> operatorStack;
				//wiz::StringTokenizer tokenizer(result, { " ", "\n", "\t", "\r" }, builder, 1);
				//vector<std::string> tokenVec;

				//while (tokenizer.hasMoreTokens()) {
				//		tokenVec.push_back(tokenizer.nextToken());
				//}

				for (int i = tokenVec.size() - 1; i >= 0; --i) {
					// todo - chk first? functions in Event
					if (String::startsWith(tokenVec[i], "$parameter.") ||
						//tokenVec[i] == "$parameter" ||
						//tokenVec[i] == "$local" ||
						String::startsWith(tokenVec[i], "$local.") ||
						//"$return" == tokenVec[i] ||
						'$' != tokenVec[i][0] || ('$' == tokenVec[i][0] && tokenVec[i].size() == 1)
						) {
						operandStack.push(std::move(tokenVec[i]));
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
							// todo - exit ? or throw?
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

				// ex) A = { B = 1 $C = { 3 } } D = { E }  $F = { G }
				// =>  A = { B = 1 $C = 3  }  D = E $F = G 
				// =>  A = { B = 1 $C = { 3 } } D = E  $F = { G } : ToDo!

				std::vector<std::string> strVec;
				std::stack<int> chkBrace;

				chkBrace.push(0);

				for (int i = operandStack.size() - 1; i >= 0; --i)
				{
					if (operandStack[i] == "}") {
						chkBrace.top()++;
						if (chkBrace.top() == 2 && !(i + 4 <= operandStack.size() - 1 && operandStack[i + 3] == "=" && operandStack[i + 4].ToString()[0] == '$' && operandStack[i + 4].ToString().size() > 1))
						{
							std::string temp = strVec.back();
							strVec.pop_back();
							strVec.pop_back();
							strVec.push_back(std::move(temp));

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

					strVec.push_back(std::move(operandStack[i].ToString()));
				}

				//result = std::string(builder->Str(), builder->size());
				// todo!  $C = 3 => $C = { 3 } 
				{
					//StringTokenizer tokenizer(result, builder, 1);
					//result = "";
					builder->Clear();

					//while (tokenizer.hasMoreTokens()) {
					for (int i = 0; i < strVec.size(); ++i) {
						//const std::string temp = strVec[i]; // tokenizer.nextToken();

						{
							//result = result + temp + " "; 
							// temp -> strVec
							builder->Append(strVec[i].c_str(), strVec[i].size());
							builder->Append(" ", 1);
						}
					}
				}
				result = std::string(builder->Str(), builder->Size());
				if (!result.empty()) {
					result.erase(result.begin() + result.size() - 1);
				}

				return std::move(result);
			}

			static std::string ToBool4(wiz::load_data::UserType* now, wiz::load_data::UserType& global, const std::string& temp, const ExcuteData& excuteData, wiz::StringBuilder* builder);
		};
	}
}

#endif // LOAD_DATA_H_INCLUDED
