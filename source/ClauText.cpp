#define _CRT_SECURE_NO_WARNINGS

#include "wiz/ClauText.h"

namespace wiz {

#define ENTER '\n' // cf) std::endl

	namespace Lint {
		wiz::StringBuilder builder(102400);
		bool cout_off = false; // print yes or no?
		wiz::Option opt;

		// justOne + optional_ -> no ok?
		// onemore + optional_ -> no ok?
		class Option
		{
		public:
			enum class Type_ { ANY, INT, FLOAT, QUOTED_STRING, STRING, DATETIME, DATETIME_A, DATETIME_B };

			enum class Id_ { NONE, ID, TOTAL_ID };
			enum class OneMore_ { NONE, ONEMORE, JUSTONE };
			enum class Required_ { REQUIRED, OPTIONAL_ };
			enum class EmptyUT_ { NONE, OFF, ON };

			enum class Order_ { NONE, OFF, ON }; // with global??
			enum class Multiple_ { NONE, OFF, ON };
		public:
			std::vector<Type_> type;
			Id_ id;
			OneMore_ onemore;
			EmptyUT_ empty_ut;
			Required_ required;
			std::string event_ids;

			std::string prefix;
		public:
			Option() : type(), //id(Id_::NONE), 
				//onemore(OneMore_::NONE), 
				required(Required_::REQUIRED),
				empty_ut(EmptyUT_::NONE)
			{
				//
			}
		public:
			Option& Type(Type_ type) {
				this->type.push_back(type);
				return *this;
			}
			Option& Id(Id_ id) {
				this->id = id;
				return *this;
			}
			Option& OneMore(OneMore_ onemore) {
				this->onemore = onemore;
				return *this;
			}
			Option& EmptyUT(EmptyUT_ empty_ut) {
				this->empty_ut = empty_ut;
				return *this;
			}
			Option& Required(Required_ required) {
				this->required = required;
				return *this;
			}
			Option& Event(const std::string& event_id) {
				this->event_ids += "|";
				this->event_ids += event_id;
				return *this;
			}
		public:
			// check valid function?
			bool IsValid() const {
				// to do
			}
		};


		Option OptionFrom(const std::string& option_str)
		{
			Option option;
			std::string::size_type start = 0;
			std::string::size_type find_percent = std::string::npos; // % : delimeter.

			if ((find_percent = option_str.find('%', start)) == std::string::npos)
			{
				option.prefix = option_str;
			}
			else {
				option.prefix = option_str.substr(start, find_percent - start);
			}

			while ((find_percent = option_str.find('%', start)) != std::string::npos)
			{
				std::string::size_type end_ = option_str.find('%', find_percent + 1);

				start = find_percent; // ex) abc%id?

				if (end_ == std::string::npos) {
					end_ = option_str.size();
				}

				const std::string opt = option_str.substr(start, end_ - 1 - start + 1);

				if ("%int" == opt) {
					option.Type(Option::Type_::INT);
				}
				else if ("%float" == opt) {
					option.Type(Option::Type_::FLOAT);
				}
				else if ("%quoted_string" == opt) {
					option.Type(Option::Type_::QUOTED_STRING);
				}
				else if ("%string" == opt) {
					option.Type(Option::Type_::STRING);
				}
				else if ("%datetime" == opt) {
					option.Type(Option::Type_::DATETIME);
				}
				else if ("%datetime_a" == opt) {
					option.Type(Option::Type_::DATETIME_A);
				}
				else if ("%datetime_b" == opt) {
					option.Type(Option::Type_::DATETIME_B);
				}
				else if ("%id" == opt) {
					option.Id(Option::Id_::ID);
				}
				else if ("%total_id" == opt) {
					option.Id(Option::Id_::TOTAL_ID);
				}
				else if ("%any" == opt) {
					option.Type(Option::Type_::ANY);
				}
				else if ("%one_more" == opt) { // x = { 1 2 3 4  } -> x = { %int%one_more%event_plus_test }
					option.OneMore(Option::OneMore_::ONEMORE);
				}
				else if ("%just_one" == opt) {
					option.OneMore(Option::OneMore_::JUSTONE);
				}
				else if ("%optional" == opt) {
					option.Required(Option::Required_::OPTIONAL_);
				}
				else if ("%required" == opt) {
					option.Required(Option::Required_::REQUIRED);
				}
				else if ("%can_empty_ut" == opt) {
					option.EmptyUT(Option::EmptyUT_::ON);
				}
				else if (wiz::String::startsWith(opt, "%event_")) { // size check?
					std::string event_name = wiz::String::substring(opt, 7);
					option.Event(std::move(event_name));
				}
				else {
					std::cout << "wrong option" << ENTER;
					exit(-2);
				}

				start = end_;
			}

			if (option.type.empty()) {
				option.type.push_back(Option::Type_::ANY);
			}
			return option;
		}

		// chk valid in here!
		inline bool OptionDoA(const Option& option, const std::string& str)
		{
			if (option.prefix.empty() == false &&
				option.prefix == str) {
				return true;
			}
			else if (option.prefix.empty() == false &&
				option.prefix != str) {
				return false;
			}

			long long count = option.type.size();
			auto type_list = option.type;

			while (!type_list.empty()) {
				switch (type_list.back()) {
				case Option::Type_::ANY:
					// pass
					break;
				case Option::Type_::INT:
					if (wiz::load_data::Utility::IsInteger(str)) {
						//
					}
					else {
						std::cout << str << " is not integer" << ENTER;
						count--;
					}
					break;
				case Option::Type_::FLOAT:
					if (wiz::load_data::Utility::IsDouble(str)) {
						//
					}
					else {
						std::cout << str << " is not double" << ENTER;
						count--;
					}
					break;
				case Option::Type_::DATETIME:
					if (wiz::load_data::Utility::IsDate(str)) {
						//
					}
					else {
						std::cout << str << " is not date" << ENTER;
						count--;
					}
					break;
				case Option::Type_::DATETIME_A:
					if (wiz::load_data::Utility::IsDateTimeA(str)) {
						//
					}
					else {
						std::cout << str << " is not datetime A" << ENTER;
						count--;
					}
					break;
				case Option::Type_::DATETIME_B:
					if (wiz::load_data::Utility::IsDateTimeB(str)) {
						//
					}
					else {
						std::cout << str << " is not datetime B" << ENTER;
						count--;
					}
					break;
				case Option::Type_::QUOTED_STRING:
					if (str.size() >= 2 && str[0] == str.back() && str.back() == '\"') { // cf '\''
						//
					}
					else {
						std::cout << str << " is not quoted string" << ENTER;
						count--;
					}
					break;
				case Option::Type_::STRING:
					// pass
					break;
				}
				type_list.pop_back();
			}

			return count > 0;
		}

		std::tuple<bool, Option, Option> _Check(wiz::load_data::UserType* schema_eventUT,
			const wiz::load_data::ItemType<WIZ_STRING_TYPE>& x, const wiz::load_data::ItemType<WIZ_STRING_TYPE>& y, const std::string& real_dir) //, Order?
		{
			const Option var_option = OptionFrom(x.GetName().ToString()); // name, value check - not start with % ??
			const Option val_option = OptionFrom(x.Get(0).ToString());

			// val only case, ex) A = { a b c d } , a, b, c, d `s name is empty.
			if (x.GetName().ToString().empty()) {
				if (!y.GetName().ToString().empty()) {
					//
					return{ false, var_option, val_option };
				}
			}
			else { // bug?
				if (y.GetName().ToString().empty()) {
					//
					return{ false, var_option, val_option };
				}
			}

			// option type check.
			const bool name_do = OptionDoA(var_option, y.GetName().ToString());
			const bool val_do = OptionDoA(val_option, y.Get(0).ToString());

			if (name_do && val_do) {
				// event check.
				wiz::ClauText clauText;
				wiz::StringTokenizer tokenizer(var_option.event_ids, "|", &builder);
				wiz::StringTokenizer tokenizer2(val_option.event_ids, "|", &builder);
				std::string event_name;

				while (tokenizer.hasMoreTokens()) {
					event_name = tokenizer.nextToken();

					wiz::StringTokenizer tokenizer3(event_name, std::vector<std::string>{ "@" }, &builder);

					event_name = tokenizer3.nextToken();
					std::string parameters;

					while (tokenizer3.hasMoreTokens()) {
						std::string token = tokenizer3.nextToken();
						token = token.substr(1, token.size() - 2);
						parameters += token + " ";
					}

					std::string y_name = y.GetName().ToString();
					std::string y_data = y.Get(0).ToString();

					if (y_name.empty()) {
						y_name = "NONE";
					}
					if (y_data.empty()) {
						y_data = "NONE";
					}

					
					// for var // chk no start with __, no end with __ ?
					wiz::load_data::LoadData::AddData(*schema_eventUT, "/./",
						"Event = { id = __" + event_name + "__ $call = { id = " + event_name +
						" name = " + y_name + " value = " + y_data +
						" is_usertype = FALSE " +
						" real_dir = " + real_dir +
						" select = NAME " +
						" input = " + y_name + " "
						+ parameters + " " +
						" } }",
						"TRUE", wiz::ExcuteData(), &builder);

					if ("TRUE" == clauText.excute_module("Main = { $call = { id = __" + event_name + "__ } }", schema_eventUT, wiz::ExcuteData(), opt, 1)) {
						schema_eventUT->RemoveUserTypeList(schema_eventUT->GetUserTypeListSize() - 1);
					}
					else {
						schema_eventUT->RemoveUserTypeList(schema_eventUT->GetUserTypeListSize() - 1);

						std::cout << "clauText is not valid1" << ENTER;
						return { false, var_option, val_option };
					}
				}
				while (tokenizer2.hasMoreTokens()) {
					event_name = tokenizer2.nextToken();
					
					wiz::StringTokenizer tokenizer3(event_name, std::vector<std::string>{ "@" }, &builder);

					event_name = tokenizer3.nextToken();
					std::string parameters;

					while (tokenizer3.hasMoreTokens()) {
						std::string token = tokenizer3.nextToken();
						token = token.substr(1, token.size() - 2);
						parameters += token + " ";
					}

					std::string y_name = y.GetName().ToString();
					std::string y_data = y.Get(0).ToString();

					if (y_name.empty()) {
						y_name = "NONE";
					}
					if (y_data.empty()) {
						y_data = "NONE";
					}

					// for val
					wiz::load_data::LoadData::AddData(*schema_eventUT, "/./",
						"Event = { id = __" + event_name + "__ $call = { id = " + event_name +
						" name = " + y_name + " value = " + y_data +
						" is_usertype = FALSE " +
						" real_dir = " + real_dir +
						" select = VALUE " +
						" input = " + y_data + " " 
						+ parameters + " " +
						" } }",
						"TRUE", wiz::ExcuteData(), &builder);

					if ("TRUE" == clauText.excute_module("Main = { $call = { id = __" + event_name + "__ } }", schema_eventUT, wiz::ExcuteData(), opt, 1)) {
						schema_eventUT->RemoveUserTypeList(schema_eventUT->GetUserTypeListSize() - 1);
					}
					else {
						schema_eventUT->RemoveUserTypeList(schema_eventUT->GetUserTypeListSize() - 1);

						std::cout << "clauText is not valid2" << ENTER;
						return { false, var_option, val_option };
					}
				}
			}
			else {
				return { false, var_option, val_option };
			}

			return { true, var_option, val_option };
		}

		std::tuple<bool, Option> _Check(wiz::load_data::UserType* schema_eventUT,
			const wiz::load_data::UserType* x, const wiz::load_data::UserType* y, const std::string& real_dir // Order?
		)
		{
			Option var_option = OptionFrom(x->GetName().ToString()); // name, value check - not start with % ??

			// val only case, ex) A = { a b c d } , a, b, c, d `s name is empty.
			if (x->GetName().ToString().empty()) {
				if (!y->GetName().ToString().empty()) {
					//
					return{ false, var_option };
				}
			}
			else { // bug?
				if (!y || y->GetName().ToString().empty()) {
					//
					return{ false, var_option };
				}
			}

			// option type check.
			const bool name_do = OptionDoA(var_option, y->GetName().ToString());

			if (name_do) {
				// event check.
				wiz::ClauText clauText;
				wiz::StringTokenizer tokenizer(var_option.event_ids, "|", &builder);
				std::string event_name;

				while (tokenizer.hasMoreTokens()) {
					event_name = tokenizer.nextToken();

					wiz::StringTokenizer tokenizer3(event_name, std::vector<std::string>{ "@" }, &builder);

					event_name = tokenizer3.nextToken();
					std::string parameters;

					while (tokenizer3.hasMoreTokens()) {
						std::string token = tokenizer3.nextToken();
						token = token.substr(1, token.size() - 2);
						parameters += token + " ";
					}

					std::string y_name = y->GetName().ToString();
					if (y_name.empty()) {
						y_name = "NONE";
					}

					// for var // chk no start with __, no end with __ ?
					wiz::load_data::LoadData::AddData(*schema_eventUT, "/./",
						"Event = { id = __" + event_name + "__ $call = { id = " + event_name +
						" name = " + y_name +
						" is_usertype = TRUE " +
						" real_dir = " + real_dir +
						" select = NAME " +
						" input = " + y_name + " " +
						parameters + " " +
						" }  } ",
						"TRUE", wiz::ExcuteData(), &builder);

					if ("TRUE" == clauText.excute_module("Main = { $call = { id = __" + event_name + "__ } }", schema_eventUT, wiz::ExcuteData(), opt, 1)) {
						schema_eventUT->RemoveUserTypeList(schema_eventUT->GetUserTypeListSize() - 1);
					}
					else {
						schema_eventUT->RemoveUserTypeList(schema_eventUT->GetUserTypeListSize() - 1);

						std::cout << "clauText is not valid3" << ENTER;
						return { false, var_option };
					}
				}
			}
			else {
				return { false, var_option };
			}

			return { true, var_option };
		}

		// varaible!
		std::set<std::tuple<std::string, std::string, std::string>> check_total_id;

		bool Check(wiz::load_data::UserType* schema_eventUT, const wiz::load_data::UserType* schemaUT,
			const wiz::load_data::UserType* clautextUT, int depth, bool& log_on, bool is_optional = false)
		{
			bool use_it_order_on = false;
			bool use_ut_order_on = false;

			Option::Order_ order = Option::Order_::OFF;
			long long ct_itCount = 0; // for clautextUT?
			long long ct_utCount = 0; // for clautextUT?
			long long itCount = 0;
			long long utCount = 0;

			long multiple_flag = 0; // 0 : no multiple, 1 : multiple

			// for ORDER_::OFF
			std::vector<bool> validVisit(schemaUT->GetIListSize(), false);
			std::vector<Option> varOptionVisit(schemaUT->GetIListSize()); // remove?
			std::vector<bool> mark(clautextUT->GetItemListSize(), false); // ct_it
			std::vector<bool> mark2(clautextUT->GetUserTypeListSize(), false); // ct_ut

			std::set<std::pair<std::string, std::string>> check_id;

			int multiple_run = 0; // 1 - it(itemtype) run, 2 - ut(usertype) run.

			long long count_ct_it = 0;
			long long count_ct_ut = 0;


			for (long long i = 0; i < schemaUT->GetIListSize(); ++i)
			{
				if (depth == 0) { // chk - clau`s depth >= 1 ( { ~~ } )
					check_total_id.clear();
				}
				const bool chk_ct_it = ct_itCount < clautextUT->GetItemListSize();
				const bool chk_ct_ut = ct_utCount < clautextUT->GetUserTypeListSize();

				bool check_pass = false;

				if (schemaUT->IsItemList(i)) {
					if (schemaUT->GetItemListSize() > 0 && schemaUT->GetItemList(itCount).ToString() == "%order_on") {
						order = Option::Order_::ON;
						validVisit[i] = true;
						itCount++;

						if (use_it_order_on) {
							ct_itCount += count_ct_it;
							count_ct_it = 0;
						}
						if (use_ut_order_on) {
							ct_utCount += count_ct_ut;
							count_ct_ut = 0;
						}

						use_it_order_on = false;
						use_ut_order_on = false;

						continue;
					}
					else if (schemaUT->GetItemList(itCount).ToString() == "%order_off") {
						order = Option::Order_::OFF;

						if (use_it_order_on) {
							ct_itCount += count_ct_it;
							count_ct_it = 0;
						}
						if (use_ut_order_on) {
							ct_utCount += count_ct_ut;
							count_ct_ut = 0;
						}


						use_it_order_on = false;
						use_ut_order_on = false;

						validVisit[i] = true;
						itCount++;
						continue;
					}
					else if (schemaUT->GetItemList(itCount).ToString() == "%multiple_on") {
						//	if (order == Option::Order_::OFF) {
						//		std::cout << "to do %multple_on, need to %order_on!" << ENTER;
						//		throw "ERROR1";
						//	}
						multiple_flag = 1;
						validVisit[i] = true;
						itCount++;
						continue;
					}
					else if (schemaUT->GetItemList(itCount).ToString() == "%multiple_off") {
						//if (order == Option::Order_::OFF) {
						//	std::cout << "to do %multple_off, need to %order_on!" << ENTER;
						//	throw "ERROR2";
						//}
						multiple_flag = 0;
						multiple_run = 0;
						validVisit[i] = true;
						itCount++;
						continue;
					}
					else if (schemaUT->GetItemList(itCount).ToString() == "%log_on") {
						log_on = true;
						validVisit[i] = true;
						itCount++;
						continue;
					}
					else if (schemaUT->GetItemList(itCount).ToString() == "%log_off") {
						log_on = false;
						validVisit[i] = true;
						itCount++;
						continue;
					}


					// log
					if (log_on) {
						std::cout << ENTER << "<itemtype> ";
						std::cout << "[depth] : " << depth << " ";
						std::cout << "[~th] : " << itCount << " ";
						std::cout << "[schema] : " << schemaUT->GetItemList(itCount).ToString() << " ";
					}



					// off -> no order? : slow??
					if (order == Option::Order_::OFF) {
						bool pass = false;
						bool use_onemore = false;
						std::tuple<bool, Option, Option> temp;

						int check_justone = 0;

						int count_iter = 0;

						// schemaUT?
						for (long long j = 0; j < clautextUT->GetItemListSize(); ++j) {
							if (count_iter > 0 && 0 == multiple_flag) {
								break;
							}

							if (log_on) {
								std::cout << ENTER << "\t" << "[clau ~th] : " << j << " "
									<< "[clautext] : " << clautextUT->GetItemList(j).ToString() << ENTER;
							}

							if (mark[j] == true) {
								continue;
							}

							temp = _Check(schema_eventUT, schemaUT->GetItemList(itCount), clautextUT->GetItemList(j), wiz::load_data::LoadData::GetRealDir(clautextUT->GetItemList(j).GetName().ToString(), clautextUT, &builder));


							if (mark[j] == false &&
								std::get<0>(temp)
								) {
								count_iter++;

								// visit vector? chk?
								validVisit[i] = true;
								varOptionVisit[i] = std::get<1>(temp);
								mark[j] = true;

								pass = true;

								// check id, total id! 
								if (std::get<1>(temp).id == Option::Id_::ID) {
									const std::string key_1 = clautextUT->GetItemList(j).GetName().ToString();
									const std::string key_2 = "it_name";
									const std::pair<std::string, std::string> key(key_1, key_2);

									if (check_id.find(key) == check_id.end()) {
										check_id.insert(key);
									}
									else {
										std::cout << "clauText is not valid - ID1" << ENTER;
										return false;
									}
								}
								else if (std::get<1>(temp).id == Option::Id_::TOTAL_ID) {
									if (0 == depth) {
										std::cout << "ERROR schema is not valid1" << ENTER;
										return false;
									}
									const std::string key_1 = clautextUT->GetItemList(j).GetName().ToString();
									const std::string key_2 = wiz::load_data::LoadData::GetRealDir(schemaUT->GetName().ToString(), schemaUT, &builder);
									const std::string key_3 = "it_name";

									std::tuple<std::string, std::string, std::string> key(key_1, key_2, key_3);

									if (check_total_id.find(key) == check_total_id.end()) {
										check_total_id.insert(key);
									}
									else {
										std::cout << "clauText is not valid _ ID2" << ENTER;
										return false;
									}
								}
								if (std::get<2>(temp).id == Option::Id_::ID) {
									const std::string key_1 = clautextUT->GetItemList(j).Get(0).ToString();
									const std::string key_2 = "it_value";
									const std::pair<std::string, std::string> key(key_1, key_2);

									if (check_id.find(key) == check_id.end()) {
										check_id.insert(key);
									}
									else {
										std::cout << "clauText is not valid - ID3" << ENTER;
										return false;
									}
								}
								else if (std::get<2>(temp).id == Option::Id_::TOTAL_ID) {
									if (0 == depth) {
										std::cout << "ERROR schema is not valid2" << ENTER;
										return false;
									}

									const std::string key_1 = clautextUT->GetItemList(j).Get(0).ToString();
									const std::string key_2 = wiz::load_data::LoadData::GetRealDir(schemaUT->GetName().ToString(), schemaUT, &builder);
									const std::string key_3 = "it_value";

									std::tuple<std::string, std::string, std::string> key(key_1, key_2, key_3);

									if (check_total_id.find(key) == check_total_id.end()) {
										check_total_id.insert(key);
									}
									else {
										std::cout << "clauText is not valid _ ID4" << ENTER;
										return false;
									}
								}

								// check justone, (onemore)
								if (std::get<1>(temp).onemore == Option::OneMore_::JUSTONE) { // justone -> only for name! , not for value!
									if (check_justone > 0) {
										std::cout << "clauText is not valid, justone is set, but not justone. 1" << ENTER;
										return false;
									}
									else {
										check_justone++;
										count_ct_it++;
										use_it_order_on = true;
									}
								}
								else {
									use_it_order_on = true;
									count_ct_it++; //ct_itCount++;
									use_onemore = true;
								}
							}
						}

						if (false == pass) {
							Option var_option = OptionFrom(schemaUT->GetItemList(itCount).GetName().ToString());

							if (var_option.required == Option::Required_::OPTIONAL_) { // optional -> only for name
								validVisit[i] = true;
							}
							else {
								std::cout << "clauText is not valid4" << ENTER;
								return false;
							}
						}
					}
					else if (order == Option::Order_::ON) {
						if (!chk_ct_it) {
							std::cout << "chk_ct_it is false" << ENTER;

							if (1 == multiple_run) {
								itCount++;
								continue;
							}
							else {
								break;
							}
						}

						if (log_on) {
							std::cout << "[clau ~th] : " << ct_itCount << " "
								<< "[clautext] : " << clautextUT->GetItemList(ct_itCount).ToString() << ENTER;
						}

						check_pass = true;

						int check_justone = 0;

						std::tuple<bool, Option, Option> temp;
						temp = _Check(schema_eventUT, schemaUT->GetItemList(itCount), clautextUT->GetItemList(ct_itCount), wiz::load_data::LoadData::GetRealDir(clautextUT->GetItemList(ct_itCount).GetName().ToString(), clautextUT, &builder));

						if (mark[ct_itCount]) {
							std::cout << "mark " << ENTER;
							return false;
						}
						else if (std::get<0>(temp)) {
							validVisit[i] = true;

							// check id, total id!
							if (std::get<1>(temp).id == Option::Id_::ID) {
								const std::string key_1 = clautextUT->GetItemList(ct_itCount).GetName().ToString();
								const std::string key_2 = "it_name";
								const std::pair<std::string, std::string> key(key_1, key_2);

								if (check_id.find(key) == check_id.end()) {
									check_id.insert(key);
								}
								else {
									std::cout << "clauText is not valid - ID5" << ENTER;
									return false;
								}
							}
							else if (std::get<1>(temp).id == Option::Id_::TOTAL_ID) {
								if (0 == depth) {
									std::cout << "ERROR schema is not valid3" << ENTER;
									return false;
								}

								const std::string key_1 = clautextUT->GetItemList(ct_itCount).GetName().ToString();
								const std::string key_2 = wiz::load_data::LoadData::GetRealDir(schemaUT->GetName().ToString(), schemaUT, &builder);
								const std::string key_3 = "it_name";

								std::tuple<std::string, std::string, std::string> key(key_1, key_2, key_3);

								if (check_total_id.find(key) == check_total_id.end()) {
									check_total_id.insert(key);
								}
								else {
									std::cout << "clauText is not valid _ ID6" << ENTER;
									return false;
								}
							}

							if (std::get<2>(temp).id == Option::Id_::ID) {
								const std::string key_1 = clautextUT->GetItemList(ct_itCount).Get(0).ToString();
								const std::string key_2 = "it_value";
								const std::pair<std::string, std::string> key(key_1, key_2);

								if (check_id.find(key) == check_id.end()) {
									check_id.insert(key);
								}
								else {
									std::cout << "clauText is not valid - ID7" << ENTER;
									return false;
								}
							}
							else if (std::get<2>(temp).id == Option::Id_::TOTAL_ID) {
								if (0 == depth) {
									std::cout << "ERROR schema is not valid4" << ENTER; // use id!
									return false;
								}

								const std::string key_1 = clautextUT->GetItemList(ct_itCount).Get(0).ToString();
								const std::string key_2 = wiz::load_data::LoadData::GetRealDir(schemaUT->GetName().ToString(), schemaUT, &builder);
								const std::string key_3 = "it_value";

								std::tuple<std::string, std::string, std::string> key(key_1, key_2, key_3);

								if (check_total_id.find(key) == check_total_id.end()) {
									check_total_id.insert(key);
								}
								else {
									std::cout << "clauText is not valid _ ID8" << ENTER;
									return false;
								}
							}

							// check justone, (onemore)
							if (std::get<1>(temp).onemore == Option::OneMore_::JUSTONE) { // justone -> only for name! , not for value!
								//
							}
							else if (std::get<1>(temp).onemore == Option::OneMore_::ONEMORE) {
								std::cout << "clauText is not valid, in order_on no onemore! 1" << ENTER;
								return false;
							}
							else {
								//
							}
						}
						else if (std::get<1>(temp).required == Option::Required_::OPTIONAL_) {
							if (i == schemaUT->GetIListSize() - 1) {
								throw "clauText is not valid - error1 ";
							}

							check_pass = false;
							validVisit[i] = true;

							if (1 == multiple_flag && itCount < schemaUT->GetItemListSize() - 1 &&
								schemaUT->GetItemList(itCount + 1).ToString() == "%multiple_off") {
								multiple_flag = 0;
								multiple_run = 0;
							}
						}
						else if (1 == multiple_flag && itCount < schemaUT->GetItemListSize() - 1 &&
							schemaUT->GetItemList(itCount + 1).ToString() == "%multiple_off") {
							multiple_flag = 0;
							multiple_run = 0;
							check_pass = false;
						}
						else {
							std::cout << "clauText is not valid6" << ENTER;
							return false;
						}

						if (1 == multiple_flag) {
							multiple_run = 1;
							itCount--; i--;
						}
					}

					if (Option::Order_::ON == order && check_pass) {
						ct_itCount++;
					}
					itCount++;
				}
				else { // usertype
					// log
					if (log_on) {
						std::cout << ENTER << "<usertype> ";
						std::cout << "[depth] : " << depth << " ";
						std::cout << "[~th] : " << utCount << " ";
						std::cout << "[schema] : " << schemaUT->GetUserTypeList(utCount)->GetName().ToString() << " ";
					}

					// off -> no order? : slow??
					if (order == Option::Order_::OFF) {
						bool pass = false;
						bool use_onemore = false;
						std::tuple<bool, Option> temp3;
						int count_iter = 0;
						int check_justone = 0;

						for (long long j = 0; j < clautextUT->GetUserTypeListSize(); ++j) {
							if (count_iter > 0 && 0 == multiple_flag) {
								break;
							}

							if (mark2[j]) {
								continue;
							}

							if (log_on) {
								std::cout << ENTER << "\t" << "[clau ~th] : " << j << " "
									<< "[clautext] : " << clautextUT->GetUserTypeList(j)->GetName().ToString() << ENTER;
							}

							temp3 = _Check(schema_eventUT, schemaUT->GetUserTypeList(utCount), clautextUT->GetUserTypeList(j), wiz::load_data::LoadData::GetRealDir(clautextUT->GetUserTypeList(j)->GetName().ToString(), clautextUT->GetUserTypeList(j), &builder));

							if (mark2[j] == false && (std::get<0>(temp3) || std::get<1>(temp3).required == Option::Required_::OPTIONAL_)) {
								count_iter++;

								if (log_on) {
									std::cout << " { " << ENTER;
								}
								if (std::get<1>(temp3).empty_ut == Option::EmptyUT_::ON && 0 == clautextUT->GetUserTypeList(j)->GetIListSize()) {
									validVisit[i] = true;
								}
								else if (Check(schema_eventUT, schemaUT->GetUserTypeList(utCount), clautextUT->GetUserTypeList(j), depth + 1, log_on, is_optional || std::get<1>(temp3).required == Option::Required_::OPTIONAL_)) {
									//
								}
								else if (std::get<1>(temp3).required == Option::Required_::OPTIONAL_) {
									validVisit[i] = true;
								}
								else {
									std::cout << "clauText is not valid8" << ENTER;
									return false;
								}

								if (log_on) {
									std::cout << " } " << ENTER;
								}

								// visit vector? chk?
								validVisit[i] = true;
								varOptionVisit[i] = std::get<1>(temp3);
								mark2[j] = true;

								pass = true;

								// check id, total id!
								if (std::get<1>(temp3).id == Option::Id_::ID) {
									const std::string key_1 = clautextUT->GetUserTypeList(j)->GetName().ToString();
									const std::string key_2 = "ut_name";
									const std::pair<std::string, std::string> key(key_1, key_2);

									if (check_id.find(key) == check_id.end()) {
										check_id.insert(key);
									}
									else {
										std::cout << "clauText is not valid - ID9" << ENTER;
										return false;
									}
								}
								else if (std::get<1>(temp3).id == Option::Id_::TOTAL_ID) {
									if (0 == depth) {
										std::cout << "ERROR schema is not valid5" << ENTER;
										return false;
									}

									const std::string key_1 = clautextUT->GetUserTypeList(j)->GetName().ToString();
									const std::string key_2 = wiz::load_data::LoadData::GetRealDir(schemaUT->GetName().ToString(), schemaUT, &builder);
									const std::string key_3 = "ut_name";

									std::tuple<std::string, std::string, std::string> key(key_1, key_2, key_3);

									if (check_total_id.find(key) == check_total_id.end()) {
										check_total_id.insert(key);
									}
									else {
										std::cout << "clauText is not valid _ ID10" << ENTER;
										return false;
									}
								}

								// check justone, (onemore)
								if (std::get<1>(temp3).onemore == Option::OneMore_::JUSTONE) { // justone -> only for name! , not for value!
									if (check_justone > 0) {

										std::cout << "clauText is not valid, justone is set, but not justone. 3" << ENTER;
										return false;
									}
									else {
										check_justone++;
										use_ut_order_on = true;
										count_ct_ut++; //ct_utCount++;
										use_onemore = true;
									}
								}
								else {
									use_ut_order_on = true;
									count_ct_ut++; //ct_utCount++;
								}
							}
						}
						if (false == pass) {
							Option var_option = OptionFrom(schemaUT->GetUserTypeList(utCount)->GetName().ToString());

							if (var_option.required == Option::Required_::OPTIONAL_) {
								validVisit[i] = true;
								utCount++;
								break;
							}
							else {
								std::cout << "clauText is not valid9 " << schemaUT->GetUserTypeList(utCount)->GetName().ToString() << ENTER;
								return false;
							}
						}
					}
					else if (order == Option::Order_::ON) {
						if (!chk_ct_ut)
						{
							std::cout << "chk_ct_ut is false" << ENTER;

							std::tuple<bool, Option> temp = _Check(schema_eventUT,
								schemaUT->GetUserTypeList(utCount), nullptr, "");

							is_optional = std::get<1>(temp).required == Option::Required_::OPTIONAL_;


							if (2 == multiple_run) {
								utCount++;
								continue;
							}
							else {
								return is_optional; //... optional..... - ToDo : removal??
							}
						}

						if (log_on) {
							std::cout << "[clau ~th] : " << ct_utCount << " "
								<< "[clautext] : " << clautextUT->GetUserTypeList(ct_utCount)->GetName().ToString() << ENTER;
						}

						check_pass = true;
						int check_justone = 0;


						std::tuple<bool, Option> temp = _Check(schema_eventUT,
							schemaUT->GetUserTypeList(utCount), clautextUT->GetUserTypeList(ct_utCount),
							wiz::load_data::LoadData::GetRealDir(clautextUT->GetUserTypeList(ct_utCount)->GetName().ToString(), clautextUT->GetUserTypeList(ct_utCount), &builder)
						);

						if (mark2[ct_utCount]) {
							std::cout << "mark2" << ENTER;
							return false;
						}
						else if (std::get<0>(temp)) {
							if (log_on) {
								std::cout << " { " << ENTER;
							}


							if (std::get<1>(temp).empty_ut == Option::EmptyUT_::ON && 0 == clautextUT->GetUserTypeList(ct_utCount)->GetIListSize()) {
								check_pass = true;
								validVisit[i] = true;
							}
							else if (Check(schema_eventUT, schemaUT->GetUserTypeList(utCount), clautextUT->GetUserTypeList(ct_utCount), depth + 1, log_on, is_optional || std::get<1>(temp).required == Option::Required_::OPTIONAL_)) {
								validVisit[i] = true;

								// check id, total id!
								if (std::get<1>(temp).id == Option::Id_::ID) {
									const std::string key_1 = clautextUT->GetUserTypeList(ct_utCount)->GetName().ToString();
									const std::string key_2 = "ut_name";
									const std::pair<std::string, std::string> key(key_1, key_2);

									if (check_id.find(key) == check_id.end()) {
										check_id.insert(key);
									}
									else {
										std::cout << "clauText is not valid - ID11" << ENTER;
										return false;
									}
								}
								else if (std::get<1>(temp).id == Option::Id_::TOTAL_ID) {
									if (0 == depth) {
										std::cout << "ERROR schema is not valid6" << ENTER;
										return false;
									}

									const std::string key_1 = clautextUT->GetUserTypeList(ct_utCount)->GetName().ToString();
									const std::string key_2 = wiz::load_data::LoadData::GetRealDir(schemaUT->GetName().ToString(), schemaUT, &builder);
									const std::string key_3 = "ut_name";

									std::tuple<std::string, std::string, std::string> key(key_1, key_2, key_3);

									if (check_total_id.find(key) == check_total_id.end()) {
										check_total_id.insert(key);
									}
									else {
										std::cout << "clauText is not valid _ ID12" << ENTER;
										return false;
									}
								}

								// check justone, (onemore)
								if (std::get<1>(temp).onemore == Option::OneMore_::JUSTONE) { // justone -> only for name! , not for value!
									//
								}
								else if (std::get<1>(temp).onemore == Option::OneMore_::ONEMORE) {
									std::cout << "clauText is not valid, in order_on no onemore! 2" << ENTER;
									return false;
								}
								else {
									//
								}
							}
							else if (std::get<1>(temp).required == Option::Required_::OPTIONAL_) {
								if (i == schemaUT->GetIListSize() - 1) {
									throw "clauText is not valid - error3 ";
								}
								check_pass = false;
								validVisit[i] = true;
							}
							else {
								std::cout << "clauText is not valid11" << ENTER;
								return false;
							}
							if (log_on) {
								std::cout << " } " << ENTER;
							}
						}
						else if (std::get<1>(temp).required == Option::Required_::OPTIONAL_) {
							if (i == schemaUT->GetIListSize() - 1) {
								throw "clauText is not valid - error4 ";
							}

							check_pass = false;
							validVisit[i] = true;

							if (1 == multiple_flag &&
								schemaUT->GetItemList(itCount).ToString() == "%multiple_off") {
								multiple_flag = 0;
								multiple_run = 0;
								//ct_utCount--;
							}
						}
						else if (1 == multiple_flag &&
							schemaUT->GetItemList(itCount).ToString() == "%multiple_off") {
							multiple_flag = 0;
							multiple_run = 0;
							check_pass = false;
						}
						else {
							std::cout << "clauText is not valid12" << ENTER;
							return false;
						}

						if (1 == multiple_flag) {
							multiple_run = 2;
							utCount--;
							i--;
						}
					}
					if (Option::Order_::ON == order && check_pass) {
						ct_utCount++;
					}
					utCount++;
				}
			}


			if (use_it_order_on) { // order_off?
				ct_itCount += count_ct_it;
			}
			if (use_ut_order_on) {
				ct_utCount += count_ct_ut;
			}


			if (multiple_flag && 2 == multiple_run) {
				//utCount++;
			}
			else if (multiple_flag && 1 == multiple_run) {
				//itCount++;
			}
			else if (multiple_flag) {
				std::cout << "multiple_flag is wrong.. " << ENTER;
				return false;
			}

			if (ct_itCount != clautextUT->GetItemListSize()) {
				std::cout << "clauText is not valid13 : " << ct_itCount << " " << clautextUT->GetItemListSize() << ENTER;
				return false;
			}
			if (ct_utCount != clautextUT->GetUserTypeListSize()) {
				std::cout << "clauText is not valid14 : " << ct_utCount << " " << clautextUT->GetUserTypeListSize() << ENTER;
				return false;
			}

			if (itCount != schemaUT->GetItemListSize()) {
				std::cout << "clauText is not valid15 : " << itCount << ENTER;
				return false;
			}

			if (utCount != schemaUT->GetUserTypeListSize()) {
				std::cout << "clauText is not valid16 : " << utCount << " " << schemaUT->GetUserTypeListSize() << ENTER;
				return false;
			}

			for (long long i = 0; i < validVisit.size(); ++i) {
				if (false == validVisit[i]) {
					std::cout << "clauText is not valid17" << ENTER;
					return false;
				}
			}

			return true;
		}

		inline bool _Validate(const wiz::load_data::UserType* schema, const wiz::load_data::UserType* clautext)
		{
			// 1. $로 시작하는 것들을 따로 분리해서 별도의 UserType에 저장한다.
				// 1-1. $Event -> 따로 뽑아낸 다음 Event로 rename?
				// 1-2. $변수들 -> 따로 뽑아낸 다음 변수들로 rename?
			// 2. $로 시작하지는 않는 것들은 data check 내용이다?
				// 2-1. %로 시작하는 것을 찾아서 각각 수행한다. 
			wiz::load_data::UserType schema_eventUT;
			wiz::load_data::UserType schemaUT;
			int itemCount = 0;
			int utCount = 0;

			for (int i = 0; i < schema->GetIListSize(); ++i) {
				if (schema->IsItemList(i)) { // itemtype
					const std::string name = wiz::ToString(schema->GetItemList(itemCount).GetName());
					if (name.size() > 1 && name[0] == '$') {
						schema_eventUT.AddItem(name.substr(1), schema->GetItemList(itemCount).Get(0));
					}
					else {
						schemaUT.AddItem(name, schema->GetItemList(itemCount).Get(0));
					}
					itemCount++;
				}
				else { // usertype
					const std::string name = wiz::ToString(schema->GetUserTypeList(utCount)->GetName());
					wiz::load_data::UserType temp = *schema->GetUserTypeList(utCount);

					if (name.size() > 1 && name[0] == '$') {
						temp.SetName(wiz::ToString(temp.GetName()).substr(1));
						schema_eventUT.AddUserTypeItem(std::move(temp));
					}
					else {
						schemaUT.AddUserTypeItem(std::move(temp));
					}
					utCount++;
				}
			}

			// for log?
			bool log_on = false;
			const bool chk = Check(&schema_eventUT, &schemaUT, clautext, 0, log_on);

			return chk;
		}
	}

void ClauText::MStyleTest(wiz::load_data::UserType* pUt)
{
#ifdef _MSC_VER
	wiz::StringBuilder builder(1024);
	std::vector<wiz::load_data::ItemType<wiz::load_data::UserType*>> utVec;
	std::vector<MData> mdVec;
	//std::vector<vector<MData>> mdVec2;
	wiz::load_data::ItemType<wiz::load_data::UserType*> global;
	wiz::load_data::UserType* utTemp = pUt;
	std::vector<wiz::load_data::UserType*> utVec2;
	std::vector<int> idxVec; //for store before idx
	std::vector<std::string> strVec; // for value..
	int braceNum = 0;
	int idx = 0;
	bool isFirst = true;
	bool isReDraw = true;
	int sizeOfWindow = 30;
	size_t Start = 0;
	size_t End = 0;
	int state = 0;

	global.Push(utTemp);

	utVec.push_back(global);

	utVec2.push_back(utTemp);

	system("cls");

	int count_userType = 0;
	int count_item = 0;

	while (true) {
		if (isFirst) {
			mdVec = std::vector<MData>();
			count_userType = 0;
			count_item = 0;

			for (int h = 0; h < utVec[braceNum].size(); ++h) {
				for (int i = 0; i < utVec[braceNum].Get(h)->GetUserTypeListSize(); ++i) {
					MData mdTemp{ true, wiz::ToString(utVec[braceNum].Get(h)->GetUserTypeList(i)->GetName()), h };
					if (mdTemp.varName.empty()) {
						mdTemp.varName = " ";
					}

					mdVec.push_back(mdTemp);
					count_userType++;
				}
			}
			for (int h = 0; h < utVec[braceNum].size(); ++h) {
				for (int i = 0; i < utVec[braceNum].Get(h)->GetItemListSize(); ++i) {
					MData mdTemp{ false, wiz::ToString(utVec[braceNum].Get(h)->GetItemList(i).GetName()), h };
					if (mdTemp.varName.empty()) {
						mdTemp.varName = " ";
					}
					mdVec.push_back(mdTemp);
					count_item++;
				}
			}
			isFirst = false;
		}
		if (isReDraw) {
			setcolor(0, 0);
			system("cls");

			End = std::min(Start + sizeOfWindow - 1, mdVec.size() - 1);
			if (mdVec.empty()) {
				End = Start - 1;
			}
			// draw mdVec and cursor - chk!!
			else {
				for (int i = Start; i <= End; ++i) {
					if (mdVec[i].isDir) { setcolor(0, 10); }
					else { setcolor(0, 7); }
					if (false == mdVec[i].varName.empty()) {
						std::cout << "  " << mdVec[i].varName;
					}
					else
					{
						std::cout << "  " << " ";
					}
					if (i != mdVec.size() - 1) { std::cout << std::endl; }
				}

				gotoxy(0, idx - Start);

				setcolor(0, 12);
				std::cout << "●";
				setcolor(0, 0);
				gotoxy(0, 0);
			}

			isReDraw = false;
		}

		// std::move and chk enterkey. - todo!!
		{
			FFLUSH();
			char ch = GETCH();
			FFLUSH();
		
			if ('q' == ch) { return; }

			// todo - add, remove, save
			if (strVec.empty() && Start <= End && idx > 0 && ('w' == ch || 'W' == ch))
			{
				// draw mdVec and cursor - chk!!
				if (idx == Start) {
					system("cls");

					int count = 0;
					int newEnd = Start - 1;
					int newStart = std::max(0, newEnd - sizeOfWindow + 1);

					Start = newStart; End = newEnd;
					idx--;

					for (int i = Start; i <= End; ++i) {
						if (mdVec[i].isDir) { setcolor(0, 10); }
						else { setcolor(0, 7); }
						std::cout << "  " << mdVec[i].varName;
						if (mdVec[i].varName.empty()) { std::cout << " "; }
						if (i != mdVec.size() - 1) { std::cout << std::endl; }
						count++;
					}
					gotoxy(0, idx - Start);
					setcolor(0, 12);
					std::cout << "●";
					setcolor(0, 0);
				}
				else {
					gotoxy(0, idx - Start);
					setcolor(0, 0);
					std::cout << "  ";
					idx--;

					gotoxy(0, idx - Start);
					setcolor(0, 12);
					std::cout << "●";
					setcolor(0, 0);
				}
			}
			else if (
				strVec.empty() && Start <= End && (idx < mdVec.size() - 1)
				&& ('s' == ch || 'S' == ch)
				)
			{
				if (idx == End) {
					system("cls");

					int count = 0;
					size_t newStart = End + 1;
					size_t newEnd = std::min(newStart + sizeOfWindow - 1, mdVec.size() - 1);

					Start = newStart; End = newEnd;
					idx++;

					for (int i = Start; i <= End; ++i) {
						if (mdVec[i].isDir) { setcolor(0, 10); }
						else { setcolor(0, 7); }
						std::cout << "  " << mdVec[i].varName;
						if (i != mdVec.size() - 1) { std::cout << std::endl; }
						count++;
					}
					gotoxy(0, 0);
					setcolor(0, 12);
					std::cout << "●";
					setcolor(0, 0);
				}
				else {
					gotoxy(0, idx - Start);
					setcolor(0, 0);
					std::cout << "  ";
					idx++;

					gotoxy(0, idx - Start);
					setcolor(0, 12);
					std::cout << "●";
					setcolor(0, 0);
				}
			}
			if (!strVec.empty() && Start <= End && idx > 0 && ('w' == ch || 'W' == ch))
			{
				// draw mdVec and cursor - chk!!
				if (idx == Start) {
					system("cls");

					int count = 0;
					int newEnd = Start - 1;
					int newStart = std::max(0, newEnd - sizeOfWindow + 1);

					Start = newStart; End = newEnd;
					idx--;

					for (int i = Start; i <= End; ++i) {
						setcolor(0, 7);
						std::cout << "  " << strVec[i];
						if (i != strVec.size() - 1) { std::cout << std::endl; }
						count++;
					}
					gotoxy(0, idx - Start);
					setcolor(0, 12);
					std::cout << "●";
					setcolor(0, 0);
				}
				else {
					gotoxy(0, idx - Start);
					setcolor(0, 0);
					std::cout << "  ";
					idx--;

					gotoxy(0, idx - Start);
					setcolor(0, 12);
					std::cout << "●";
					setcolor(0, 0);
				}
			}
			else if (
				!strVec.empty() && Start <= End && (idx < strVec.size() - 1)
				&& ('s' == ch || 'S' == ch)
				)
			{
				if (idx == End) {
					setcolor(0, 0);
					system("cls");

					int count = 0;
					size_t newStart = End + 1;
					size_t newEnd = std::min(newStart + sizeOfWindow - 1, strVec.size() - 1);

					Start = newStart; End = newEnd;
					idx++;

					for (int i = Start; i <= End; ++i) {
						setcolor(0, 7);
						std::cout << "  " << strVec[i];
						if (i != strVec.size() - 1) { std::cout << std::endl; }
						count++;
					}
					gotoxy(0, 0);
					setcolor(0, 12);
					std::cout << "●";
					setcolor(0, 0);
				}
				else {
					gotoxy(0, idx - Start);
					setcolor(0, 0);
					std::cout << "  ";
					idx++;

					gotoxy(0, idx - Start);
					setcolor(0, 12);
					std::cout << "●";
					setcolor(0, 0);
				}
			}
			else if ('\r' == ch || '\n' == ch) {
				/// To Do..
				gotoxy(0, 0);
				if (count_userType == 0 && count_item == 0)
				{
					//
				}
				else if (state == 0 && strVec.empty() && idx < count_userType) { // utVec[braceNum].Get(mdVec[idx].no)->GetUserTypeListSize()) {
					setcolor(0, 0);
					system("cls");

					// usertypelist
					braceNum++;
					idxVec.push_back(idx); /// idx?

					if (braceNum >= utVec.size()) {
						utVec.push_back(wiz::load_data::ItemType<wiz::load_data::UserType*>());
						utVec2.push_back(nullptr);
					}

					wiz::load_data::ItemType< wiz::load_data::UserType*> ref;
					ref.Push(nullptr);
					std::string strTemp = mdVec[idxVec[braceNum - 1]].varName;
					if (strTemp == " " || strTemp == "_")
					{
						strTemp = "";
					}

					if (utVec[braceNum - 1].Get(mdVec[idxVec[braceNum - 1]].no)->GetUserTypeItemRef(idxVec[braceNum - 1], ref.Get(0)))
					{
						//
					}
					utVec[braceNum] = ref;

					utVec2[braceNum - 1]->GetUserTypeItemRef(idxVec[braceNum - 1], ref.Get(0));
					utVec2[braceNum] = ref.Get(mdVec[idxVec[braceNum - 1]].no);

					Start = 0;
					idx = 0;
					isFirst = true;
					isReDraw = true;
				}
				else
				{
					if (
						!mdVec.empty() &&
						strVec.empty()
						)
					{
						setcolor(0, 0);
						system("cls");

						std::string strTemp = mdVec[idx].varName;
						if (strTemp == " " || strTemp == "_") { strTemp = ""; }
						const int count = 1; // utVec[braceNum].Get(mdVec[idx].no)->GetItem(strTemp).size();
						setcolor(0, 7);

						for (int i = 0; i < count; ++i) {
							setcolor(0, 7);

							auto x = utVec[braceNum].Get(mdVec[idx].no)->GetItemList(idx - count_userType);
							std::string temp = wiz::ToString(x.Get(0));
							std::cout << "  " << temp;
							strVec.push_back(temp);
							//}
							if (i != count - 1) { std::cout << std::endl; }
						}
					}

					if (state == 0) {
						gotoxy(0, 0);
						state = 1;

						idxVec.push_back(idx);

						idx = 0;
						Start = 0;

						setcolor(0, 12);
						std::cout << "●";
						setcolor(0, 0);
					}
					else if (state == 1) { /// cf) state = 2;
						gotoxy(0, idx); /// chk..
						Start = idx;
					}


					// idx = 0;
					End = std::min(Start + sizeOfWindow - 1, strVec.size() - 1);
					if (strVec.empty()) { End = Start - 1; }


					// chk
					count_userType = 0;
					count_item = 1;
				}
			}
			else if (0 == state && 'f' == ch)
			{
				std::string temp;
				int x = 0;
				system("cls");
				setcolor(0, 7);
				std::cout << "row input : ";
				std::cin >> temp;
				FFLUSH();

				if (wiz::load_data::Utility::IsInteger(temp)) {
					x = stoi(temp); // toInt
					if (x < 0) { x = 0; }
					else if (x >= mdVec.size()) {
						x = mdVec.size() - 1;
					}

					// chk!! ToDo ?
					idx = x;
					x = std::max(0, x - sizeOfWindow / 2);
					Start = x;
					isReDraw = true; /// chk!! To Do - OnlyRedraw? Reset? // int + changed?
				}
				else
				{
					isReDraw = true; /// OnlyDraw = true?, no search?
				}
			}
			else if (0 == state && '1' == ch)
			{
				int index = idx;
				std::string temp = mdVec[idx].varName;

				for (int i = idx - 1; i >= 0; --i) {
					if (mdVec[i].varName == temp) {
						index = i;
					}
					else
					{
						break;
					}
				}

				idx = index;
				Start = std::max(0, idx - sizeOfWindow / 2);
				isReDraw = true;
			}
			else if (0 == state && '2' == ch)
			{
				int index = idx;
				std::string temp = mdVec[idx].varName;

				for (int i = idx + 1; i < mdVec.size(); ++i) {
					if (mdVec[i].varName == temp) {
						index = i;
					}
					else
					{
						break;
					}
				}

				idx = index;
				Start = std::max(0, idx - sizeOfWindow / 2);
				isReDraw = true;

			}
			else {
				if ('q' == ch) { return; } // quit
				else if ('b' == ch && braceNum > 0 && strVec.empty() && state == 0) {  // back
					braceNum--;

					setcolor(0, 0);
					system("cls");

					isFirst = true;
					isReDraw = true;
					//Start = idxVec.back();

					idx = idxVec.back();
					idxVec.pop_back();

					if (0 <= idx - sizeOfWindow / 2)
					{
						Start = idx - sizeOfWindow / 2;
					}
					else {
						Start = 0;
					}
				}
				else if ('b' == ch && !idxVec.empty()) /// state == 1 ?
				{
					idx = idxVec.back();
					idxVec.pop_back();

					if (0 <= idx - sizeOfWindow / 2)
					{
						Start = idx - sizeOfWindow / 2;
					}
					else {
						Start = 0;
					}

					state = 0;
					setcolor(0, 0);
					system("cls");
					strVec.clear();

					isFirst = true;
					isReDraw = true;
				}
				else if ('e' == ch) {
					setcolor(0, 0);
					system("cls");
					setcolor(7, 0);

					std::cout << "edit mode" << std::endl;
					std::cout << "add - a, change - c, remove - r, save - s" << std::endl;
					
					//GETCH(); // why '\0' or 0?
					ch = GETCH();
					FFLUSH();

					if ('a' == ch) { // add
						int select = -1;
						std::string var;
						std::string val;

						setcolor(0, 7);
						// need more test!!
						std::cout << "add UserType : 1, add Item : 2, add usertype that name is \"\": 3 your input : ";
						std::cin >> select;
						FFLUSH();

						// add userType?
						if (1 == select) {
							// name of UserType.
							std::cout << "what is new UserType name? : ";
							std::cin >> var;
							FFLUSH();
							utVec2[braceNum]->AddUserTypeItem(wiz::load_data::UserType(var));
						}
						// addd Item?
						else if (2 == select) {
							// var, val /// state에 따라?
							std::cout << "var : ";
							std::cin >> var;
							std::cout << "val : ";
							FFLUSH();
							std::getline(std::cin, val);
							utVec2[braceNum]->AddItem(var, val);
						}
						else if (3 == select)
						{
							utVec2[braceNum]->AddUserTypeItem(wiz::load_data::UserType(""));
						}
					}
					else if ('c' == ch && Start <= End) { // change var or value
														  // idx
						if (idx < count_userType) {
							std::string temp;
							setcolor(0, 7);
							std::cout << "change userType name : ";

							FFLUSH();
							std::getline(std::cin, temp);

							int count = 0;
							for (int h = 0; h < utVec[braceNum].size(); ++h) {
								for (int i = 0; i < utVec[braceNum].Get(h)->GetUserTypeListSize(); ++i) {
									if (count == idx) {
										utVec[braceNum].Get(h)->GetUserTypeList(i)->SetName(temp);
									}
									count++;
								}
							}
						}
						else {
							std::string temp;
							setcolor(0, 7);

							std::string name, value;
							if (1 == state) { // val
								std::cout << "change val : " << std::endl;
								std::getline(std::cin, temp);

								value = temp;

								int count = 0;
								count_userType = 0; // int ~ ?
								for (int h = 0; h < utVec[braceNum].size(); ++h) {
									for (int i = 0; i < utVec[braceNum].Get(h)->GetUserTypeListSize(); ++i) {
										count++;
										count_userType++;
									}
								}
								for (int h = 0; h < utVec[braceNum].size(); ++h) {
									for (int i = 0; i < utVec[braceNum].Get(h)->GetItemListSize(); ++i) {
										if (idxVec.back() == count) {
											utVec[braceNum].Get(h)->GetItemList(i).Get(idx) = value;
										}
										count++;
									}
								}
								idx = idxVec.back();
								idxVec.pop_back();
								// max!
								if (0 <= idx - sizeOfWindow / 2)
								{
									Start = idx - sizeOfWindow / 2;
								}
								else {
									Start = 0;
								}
								strVec.clear();
								state = 0;
							}
							else if (0 == state) { // var
								std::cout << "change var : " << std::endl;
								std::cin >> temp;
								FFLUSH();
								name = temp;
								int count = 0;
								for (int h = 0; h < utVec[braceNum].size(); ++h) {
									for (int i = 0; i < utVec[braceNum].Get(h)->GetUserTypeListSize(); ++i) {
										count++;
									}
								}
								for (int h = 0; h < utVec[braceNum].size(); ++h) {
									for (int i = 0; i < utVec[braceNum].Get(h)->GetItemListSize(); ++i) {
										if (idx == count) {
											utVec[braceNum].Get(h)->GetItemList(i).SetName(name);
										}
										count++;
									}
								}
							}
						}
					}
					else if ('r' == ch && Start <= End) { // remove
						if (idx < count_userType)
						{
							int count = 0;
							for (int h = 0; h < utVec[braceNum].size(); ++h) {
								for (int i = 0; i < utVec[braceNum].Get(h)->GetUserTypeListSize(); ++i) {
									if (count == idx) {
										std::string temp = mdVec[idx].varName;
										if (temp == " " || temp == "_") {
											temp = "";
										}
										utVec[braceNum].Get(h)->RemoveUserTypeList(temp);
									}
									count++;
								}
							}
							idx = 0;
							Start = 0;
						}
						else {
							if (state == 0) {
								int count = 0;
								int count_ut = 0;
								for (int h = 0; h < utVec[braceNum].size(); ++h) {
									for (int i = 0; i < utVec[braceNum].Get(h)->GetUserTypeListSize(); ++i) {
										count++;
										count_ut++;
									}
								}
								for (int h = 0; h < utVec[braceNum].size(); ++h) {
									for (int i = 0; i < utVec[braceNum].Get(h)->GetItemListSize(); ++i) {
										if (count == idx) {
											std::string temp = mdVec[idx].varName;
											if (temp == " " || temp == "_") { temp = ""; }

											utVec[braceNum].Get(h)->RemoveItemList(temp);
										}
										count++;
									}
								}
								idx = 0;
								Start = 0;
							}
							else if (1 == state)
							{
								int count = 0;
								int count_ut = 0;
								for (int h = 0; h < utVec[braceNum].size(); ++h) {
									for (int i = 0; i < utVec[braceNum].Get(h)->GetUserTypeListSize(); ++i) {
										count++;
										count_ut++;
									}
								}
								for (int h = 0; h < utVec[braceNum].size(); ++h) {
									for (int i = 0; i < utVec[braceNum].Get(h)->GetItemListSize(); ++i) {
										if (count == idxVec.back()) {
											utVec[braceNum].Get(h)->GetItemList(count - count_ut).Remove(0);
											if (utVec[braceNum].Get(h)->GetItemList(count - count_ut).size() == 0) {
												utVec[braceNum].Get(h)->GetItemList(count - count_ut).Remove();
												utVec[braceNum].Get(h)->RemoveEmptyItem();
											}
										}
										count++;
									}
								}

								idxVec.pop_back();
								idx = 0;
								// max!
								if (0 <= idx - sizeOfWindow / 2)
								{
									Start = idx - sizeOfWindow / 2;
								}
								else {
									Start = 0;
								}
								strVec.clear();
								state = 0;
							}
						}
					}
					else if ('s' == ch) { // save total data.
						std::string temp;

						setcolor(0, 7);
						std::cout << "save file name : ";
						FFLUSH();
						std::getline(std::cin, temp);

						wiz::load_data::LoadData::SaveWizDB(*utTemp, temp, "1");
					}

					if (1 == state)
					{
						idxVec.back();
						idxVec.pop_back();
						idx = 0;
						// max!
						if (0 <= idx - sizeOfWindow / 2)
						{
							Start = idx - sizeOfWindow / 2;
						}
						else {
							Start = 0;
						}
						strVec.clear();
						state = 0;
					}

					/// else if( l? reload?
					isFirst = true; // 
					isReDraw = true; //
				}
				else if ('t' == ch && braceNum == 0) { // pass???
					isFirst = true;
					isReDraw = true;
					setcolor(0, 0);
					system("cls");

					setcolor(7, 0);
					std::cout << "text edit mode" << std::endl;

					// Add, AddUserType, Set, Remove, RemoveAll ?.
					std::string temp;
					FFLUSH();
					std::getline(std::cin, temp);

					wiz::StringTokenizer tokenizer(temp, "|", &builder, 1);
					std::vector<std::string> strVecTemp;

					while (tokenizer.hasMoreTokens()) {
						strVecTemp.push_back(tokenizer.nextToken());
					}
					if (!strVecTemp.empty()) {
						try {
							if ("add" == strVecTemp[0])
							{
								if (false == wiz::load_data::LoadData::AddData(*utTemp, strVecTemp[1], strVecTemp[2], strVecTemp[3], ExcuteData(), nullptr))
								{
									std::cout << "fail to add" << std::endl; /// To Do to following code.
								}
							}
							else if ("addusertype" == strVecTemp[0])
							{
								wiz::load_data::LoadData::AddUserType(*utTemp, strVecTemp[1], strVecTemp[2], strVecTemp[3], strVecTemp[4], ExcuteData(), nullptr);
							}
							else if ("set" == strVecTemp[0])
							{
								wiz::load_data::LoadData::SetData(*utTemp, strVecTemp[1], strVecTemp[2], strVecTemp[3], strVecTemp[4], ExcuteData(), nullptr);
							}
							else if ("remove" == strVecTemp[0])
							{
								wiz::load_data::LoadData::Remove(*utTemp, strVecTemp[1], strVecTemp[2], strVecTemp[3], ExcuteData(), nullptr);
							}
							//else if ("removenonameitem" == strVecTemp[0])
							//{
							//	wiz::load_data::LoadData::RemoveNoNameItem(*utTemp, strVecTemp[1], strVecTemp[2]);
							//}
							else if ("removeall" == strVecTemp[0])
							{
								wiz::load_data::LoadData::Remove(*utTemp, strVecTemp[1], strVecTemp[2], ExcuteData(), nullptr);
							}
							else if ("searchitem" == strVecTemp[0])
							{
								std::cout << wiz::load_data::LoadData::SearchItem(*utTemp, strVecTemp[1], strVecTemp[2], ExcuteData(), nullptr) << std::endl;
								GETCH();
							}
							else if ("searchusertype" == strVecTemp[0])
							{
								std::cout << wiz::load_data::LoadData::SearchUserType(*utTemp, strVecTemp[1], strVecTemp[2], ExcuteData(), nullptr) << std::endl;
								GETCH();
							}
						}
						catch (std::exception& e) {}
						catch (wiz::Error& e) {}
						catch (const char* e) {}
						catch (const std::string& e) {}
					}
					//
					idx = 0;
					Start = 0;
				}
			}
		}
	}
#endif
}


template <class T> // T has clear method, default constructor.
class Node
{
public:
	T * data;
	Node* left;
	Node* right;
public:
	static T* f(T* ptr = nullptr) {
		T* result = nullptr;
		if (nullptr == ptr) {
			result = new T();
		}
		else {
			ptr->clear();
			result = ptr;
		}

		return result;
	}
};

std::string ClauText::excute_module(const std::string& MainStr, wiz::load_data::UserType* _global, const ExcuteData& excuteData, Option& opt, int chk)
{
	wiz::Map2<std::string, std::pair<std::vector<std::string>, bool>>* __map = opt._map;
	opt._map = Node<wiz::Map2<std::string, std::pair<std::vector<std::string>, bool>>>::f(__map);
	wiz::Map2<std::string, std::pair<std::vector<std::string>, bool>>& _map = *opt._map;
																		   //
	wiz::load_data::UserType& global = *_global;
	//std::vector<std::thread*> waits;
	wiz::Map2<std::string, wiz::load_data::UserType>* _objectMap = opt.objectMap;
	opt.objectMap = Node<wiz::Map2<std::string, wiz::load_data::UserType>>::f(_objectMap);
	wiz::Map2<std::string, wiz::load_data::UserType>& objectMap = *opt.objectMap;

	wiz::Map2<std::string, wiz::load_data::UserType>* _moduleMap = opt.moduleMap;
	opt.moduleMap = Node<wiz::Map2<std::string, wiz::load_data::UserType>>::f(_moduleMap);
	wiz::Map2<std::string, wiz::load_data::UserType>& moduleMap = *opt.moduleMap;

	wiz::Map2<std::string, wiz::load_data::UserType>* objectMapPtr = nullptr;
	wiz::Map2<std::string, wiz::load_data::UserType>* moduleMapPtr = nullptr;

	std::string* _module_value = opt.module_value;
	opt.module_value = Node<std::string>::f(_module_value);
	std::string& module_value = *opt.module_value;

	// data, event load..
	wiz::ArrayStack<EventInfo>* _eventStack = opt.eventStack;
	opt.eventStack = Node<wiz::ArrayStack<EventInfo>>::f(_eventStack);
	wiz::ArrayStack<EventInfo>& eventStack = *opt.eventStack;

	wiz::Map2<std::string, int>* _convert = opt.convert;
	opt.convert = Node<wiz::Map2<std::string, int>>::f(_convert);
	wiz::Map2<std::string, int>& convert = *opt.convert;

	std::vector<wiz::load_data::UserType*>* __events = opt._events;
	opt._events = Node<std::vector<wiz::load_data::UserType*>>::f(__events);
	std::vector<wiz::load_data::UserType*>& _events = *opt._events;

	wiz::load_data::UserType* _events_ = opt.events;
	opt.events = Node<wiz::load_data::UserType>::f(_events_);
	wiz::load_data::UserType& events = *opt.events;

	wiz::load_data::UserType* eventPtr = nullptr;

	wiz::load_data::UserType* _Main = opt.Main;
	opt.Main = Node<wiz::load_data::UserType>::f(_Main);
	wiz::load_data::UserType& Main = *opt.Main;
	
	wiz::StringBuilder* _builder = opt.builder;
	opt.builder = Node<wiz::StringBuilder>::f(_builder);
	wiz::StringBuilder& builder = *opt.builder;
	

	// start from Main.
	if (excuteData.chkInfo == false) { /// chk smartpointer.
		if (global.GetUserTypeItem("Main").empty() && MainStr.empty())
		{
			std::cout << "do not exist Main" << std::endl;
			return "ERROR -1";
		}

		wiz::load_data::UserType* _Main = nullptr;

		if (MainStr.empty()) {
			_Main = global.GetCopyUserTypeItem("Main")[0];
			Main.LinkUserType(_Main);
			global.RemoveUserTypeList("Main");
		}
		else {
			wiz::load_data::LoadData::LoadDataFromString(MainStr, Main);
		}


		EventInfo info;
		info.eventUT = Main.GetUserTypeList(0);
		info.userType_idx.push(0);
		std::pair<std::string, std::string> id_data = std::make_pair<std::string, std::string>("id", wiz::ToString(info.eventUT->GetUserTypeItem("$call")[0]->GetItem("id")[0].Get(0)));
		info.parameters.insert(
			id_data
		);
		info.id = info.parameters["id"];

		eventStack.push(info);

		// $load_only_data - sequential?
		{
			wiz::load_data::UserType* val = nullptr;
			auto x = info.eventUT->GetUserTypeItem("$load_only_data");
			if (!x.empty()) {
				for (int i = 0; i < x.size(); ++i) {
					val = x[i];

					wiz::load_data::UserType ut;
					std::string fileName = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(0)->ToString(), ExcuteData(), &builder);
					fileName = wiz::String::substring(fileName, 1, fileName.size() - 2);
					std::string dirName = val->GetUserTypeList(1)->ToString();
					wiz::load_data::UserType* utTemp;

					if (dirName == "/./" || dirName == "root") {
						utTemp = &global;
					}
					else {
						dirName = wiz::load_data::LoadData::ToBool4(nullptr, global, dirName, ExcuteData(), &builder);
						utTemp = global.GetUserTypeItem(dirName)[0];
					}

					std::string option;

					if (val->GetUserTypeListSize() >= 3) {
						option = val->GetUserTypeList(2)->ToString();
					}

					if (wiz::load_data::LoadData::LoadDataFromFile(fileName, ut)) {
						{
							for (int i = 0; i < ut.GetCommentListSize(); ++i) {
								utTemp->PushComment(std::move(ut.GetCommentList(i)));
							}
							int item_count = 0;
							int userType_count = 0;

							for (int i = 0; i < ut.GetIListSize(); ++i) {
								if (ut.IsItemList(i)) {
									utTemp->AddItem(std::move(ut.GetItemList(item_count).GetName()),
										std::move(ut.GetItemList(item_count).Get(0)));
									item_count++;
								}
								else {
									utTemp->AddUserTypeItem(std::move(*ut.GetUserTypeList(userType_count)));
									userType_count++;
								}
							}
						}

						//	auto _Main = ut.GetUserTypeItem("Main");
						//	if (!_Main.empty())
						//	{
						// error!
						//		std::cout << "err" << std::endl;

						//			return "ERROR -2"; /// exit?
						//		}
					}
					else {
						// error!
					}
				}
			}
		}

		// $load_only_data2 - parallel
		{
			wiz::load_data::UserType* val = nullptr;
			auto x = info.eventUT->GetUserTypeItem("$load_only_data2");
			if (!x.empty()) {
				for (int i = 0; i < x.size(); ++i) {
					val = x[i];

					wiz::load_data::UserType ut;
					std::string fileName = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(0)->ToString(), ExcuteData(), &builder);
					fileName = wiz::String::substring(fileName, 1, fileName.size() - 2);
					std::string dirName = val->GetUserTypeList(1)->ToString();
					wiz::load_data::UserType* utTemp;


					if (dirName == "/./" || dirName == "root") {
						utTemp = &global;
					}
					else {
						dirName = wiz::load_data::LoadData::ToBool4(nullptr, global, dirName, ExcuteData(), &builder);
						utTemp = global.GetUserTypeItem(dirName)[0];
					}

					std::string option;

					if (val->GetUserTypeListSize() >= 3) {
						option = val->GetUserTypeList(2)->ToString();
					}

					int lex_thr_num = 4;
					int parse_thr_num = 4;

					if (val->GetUserTypeListSize() >= 3) {
						lex_thr_num = stoi(val->GetUserTypeList(2)->ToString());
					}
					if (val->GetUserTypeListSize() >= 4) {
						parse_thr_num = stoi(val->GetUserTypeList(3)->ToString());
					}


					if (wiz::load_data::LoadData::LoadDataFromFile3(fileName, ut, parse_thr_num - 1, lex_thr_num)) {
						{
							for (int i = 0; i < ut.GetCommentListSize(); ++i) {
								utTemp->PushComment(std::move(ut.GetCommentList(i)));
							}
							int item_count = 0;
							int userType_count = 0;

							for (int i = 0; i < ut.GetIListSize(); ++i) {
								if (ut.IsItemList(i)) {
									utTemp->AddItem(std::move(ut.GetItemList(item_count).GetName()),
										std::move(ut.GetItemList(item_count).Get(0)));
									item_count++;
								}
								else {
									utTemp->AddUserTypeItem(std::move(*ut.GetUserTypeList(userType_count)));
									userType_count++;
								}
							}
						}

						//	auto _Main = ut.GetUserTypeItem("Main");
						//	if (!_Main.empty())
						//	{
						// error!
						//		std::cout << "err" << std::endl;

						//			return "ERROR -2"; /// exit?
						//		}
					}
					else {
						// error!
					}
				}
			}
		}


		if (nullptr == excuteData.pModule) {
			moduleMapPtr = &moduleMap;
		}
		else {
			moduleMapPtr = excuteData.pModule;
		}

		if (nullptr == excuteData.pObjectMap) {
			objectMapPtr = &objectMap;
		}
		else {
			objectMapPtr = excuteData.pObjectMap;
		}

		if (nullptr == excuteData.pEvents) {
			_events = global.GetCopyUserTypeItem("Event");
			for (int i = 0; i < _events.size(); ++i) {
				events.LinkUserType(_events[i]);
			}
			global.RemoveUserTypeList("Event");
			eventPtr = &events;
		}
		else {
			eventPtr = excuteData.pEvents;
		}

		// event table setting
		for (int i = 0; i < eventPtr->GetUserTypeListSize(); ++i)
		{
			auto x = eventPtr->GetUserTypeList(i)->GetItem("id");
			if (!x.empty()) {
				//std::cout <<	x[0] << std::endl;
				auto temp = std::pair<std::string, int>(wiz::ToString(x[0].Get(0)), i);
				convert.insert(temp);
			}
			else {
				// error
			}
		}

		const int no = convert.at(info.id);
		for (int i = 0; i < eventPtr->GetUserTypeList(no)->GetUserTypeListSize(); ++i) {
			if (eventPtr->GetUserTypeList(no)->GetUserTypeList(i)->GetName() == "$local") {
				for (int j = 0; j < eventPtr->GetUserTypeList(no)->GetUserTypeList(i)->GetItemListSize(); ++j) {
					std::string name = wiz::ToString(eventPtr->GetUserTypeList(no)->GetUserTypeList(i)->GetItemList(j).Get(0));
					std::string value = "";
					std::pair<std::string, std::string> temp(name, value);
					info.locals.insert(temp);
				}
				break;
			}
		}
	}
	else {
		eventStack.push(excuteData.info);
	}

	// main loop
	while (!eventStack.empty())
	{
		// 
		EventInfo info = eventStack.top();
		std::string str;
		wiz::ArrayMap<std::string, std::string>::iterator x;
		for (int i = 0; i < info.parameters.size(); ++i) {
			if ((x = info.parameters.find("id")) != info.parameters.end()) {
				str = x->second;
				break;
			}
		}

		int no = convert.at(str);

		int state = 0;

		if (info.userType_idx.size() == 1 && info.userType_idx[0] >= eventPtr->GetUserTypeList(no)->GetUserTypeListSize())
		{
			if (eventStack.size() == 1)
			{
				module_value = eventStack.top().return_value;
			}

			eventStack.pop();
			continue;
		}

		{ /// has bug!! WARNNING!!
			wiz::load_data::UserType* val = nullptr;
			if (eventStack.top().userType_idx.size() == 1) {
				if (eventPtr->GetUserTypeList(no)->GetUserTypeListSize() > eventStack.top().userType_idx.top()) {
					val = eventPtr->GetUserTypeList(no)->GetUserTypeList(eventStack.top().userType_idx.top());

					if (eventStack.top().userType_idx.top() >= 1 && val->GetName() == "$else"
						&& wiz::ToString(eventPtr->GetUserTypeList(no)->GetUserTypeList(eventStack.top().userType_idx.top() - 1)->GetName()) != "$if") {
						return "ERROR not exist $if, front $else.";
					}
					if (eventStack.top().userType_idx.top() == 0 && val->GetName() == "$else") {
						return "ERROR not exist $if, front $else.";
					}
				}
				else {
					val = nullptr;
				}
			}
			else
			{
				// # of userType_idx == nowUT.size() + 1, and nowUT.size() == conditionStack.size()..
				while (!eventStack.top().nowUT.empty() && eventStack.top().nowUT.top()->GetUserTypeListSize() <= eventStack.top().userType_idx.top())
				{
					eventStack.top().nowUT.pop();
					eventStack.top().userType_idx.pop();
					eventStack.top().conditionStack.pop();
				}

				if (!eventStack.top().nowUT.empty() && eventStack.top().nowUT.top()->GetUserTypeListSize() > eventStack.top().userType_idx.top()) {
					val = eventStack.top().nowUT.top()->GetUserTypeList(eventStack.top().userType_idx.top());

					if (eventStack.top().userType_idx.top() >= 1 && val->GetName() == "$else"
						&& wiz::ToString(eventStack.top().nowUT.top()->GetUserTypeList(eventStack.top().userType_idx.top() - 1)->GetName()) != "$if") {
						return "ERROR not exist $if, front $else.";
					}
					if (eventStack.top().userType_idx.top() == 0 && val->GetName() == "$else") {
						return "ERROR not exist $if, front $else.";
					}
				}
				else // same to else if( eventSTack.top().nowUT.empty() ), also same to else if ( 1 == eventStack.top().userType_idx.size() )
				{
					if (eventPtr->GetUserTypeList(no)->GetUserTypeListSize() > eventStack.top().userType_idx.top()) {
						val = eventPtr->GetUserTypeList(no)->GetUserTypeList(eventStack.top().userType_idx.top());

						if (eventStack.top().userType_idx.top() >= 1 && val->GetName() == "$else"
							&& wiz::ToString(eventPtr->GetUserTypeList(no)->GetUserTypeList(eventStack.top().userType_idx.top() - 1)->GetName()) != "$if") {
							return "ERROR not exist $if, front $else.";
						}
						if (eventStack.top().userType_idx.top() == 0 && val->GetName() == "$else") {
							return "ERROR not exist $if, front $else.";
						}
					}
					else {
						val = nullptr;
					}
				}
			}

			while (val != nullptr)
			{
				// ToDo -$filter, $map, $reduce?
				// list = { base_tax } // /root/ X , root/ O
				// $filter = { utDir = { /./test } 
				// condition = { AND = { EXSITITEMBYVAL = { base_tax root/list } COMP> = { ~~ / 5.000 ~ 0 } } } 
				// recursive = { false or true } } // return UserType.?
				/** it has bug?
				if ("$for_each" == val->GetName()) {
				ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
				_excuteData.chkInfo = true;
				_excuteData.info = eventStack.top();
				_excuteData.pObjectMap = objectMapPtr;
				_excuteData.pEvents = eventPtr;
				_excuteData.pModule = moduleMapPtr;

				const std::string eventID = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(0)->ToString(), _excuteData, &builder);
				const std::string dir = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(1)->ToString(), _excuteData, &builder);
				const std::string condition = val->GetUserTypeList(2)->ToString();

				wiz::load_data::UserType* event = nullptr;
				std::string parameter = "id = " + eventID + " ";
				// find event
				{
				for (int i = 0; i < eventPtr->GetUserTypeListSize(); ++i) {
				if (eventID == eventPtr->GetUserTypeList(i)->GetItem("id")[0].Get(9)) {
				event = eventPtr->GetUserTypeList(i);
				break;
				}
				}
				}
				// find parameter and validate?
				bool pass = false;
				{
				for (int i = 0; i < event->GetUserTypeListSize(); ++i) {
				if ("$parameter" == event->GetUserTypeList(i)->GetName()) {
				if (1 == event->GetUserTypeList(i)->GetItemListSize()) {
				parameter = parameter + event->GetUserTypeList(i)->GetItemList(0).Get(0);
				}
				else {
				pass = true;
				break;
				}
				break;
				}
				}
				}
				if (pass) {
				eventStack.top().userType_idx.top()++;
				break;
				}
				// chk loop and condition! chk do not use ""
				{
				int count = 0;
				eventStack.top().return_value = "";
				wiz::load_data::UserType* ut = wiz::load_data::UserType::Find(&global, dir, &builder).second[0];
				for (int i = 0; i < ut->GetItemListSize(); ++i) {
				std::string _condition = condition;

				_condition = wiz::String::replace(_condition, "~~~", ut->GetItemList(i).Get(0)); //
				_condition = wiz::String::replace(_condition, "////", dir);
				_condition = wiz::String::replace(_condition, "///", wiz::_toString(i));
				// ToDo - chk : empty name!!
				if (ut->GetItemList(i).GetName().empty()) {
				_condition = wiz::String::replace(_condition, "~~", "^"); // chk..
				}
				else
				{
				_condition = wiz::String::replace(_condition, "~~", ut->GetItemList(i).GetName());
				}

				_condition = wiz::load_data::LoadData::ToBool4(ut, global, _condition, _excuteData, &builder);

				wiz::load_data::Condition _cond(_condition, ut, &global, &builder);

				while (_cond.Next());

				if (_cond.Now().size() != 1 || "TRUE" != _cond.Now()[0]) // || cond.Now().size()  != 1
				{
				//std::std::cout << cond.Now()[0] << std::endl;
				continue;
				}


				wiz::load_data::UserType eventsTemp = events;

				ExcuteData _excuteData;
				_excuteData.depth = excuteData.depth;
				_excuteData.pEvents = &eventsTemp;
				_excuteData.pModule = moduleMapPtr;
				_excuteData.pObjectMap = objectMapPtr;
				_excuteData.noUseInput = excuteData.noUseInput;
				_excuteData.noUseOutput = excuteData.noUseOutput;

				wiz::load_data::LoadData::AddData(eventsTemp, "/root", "Event = { id = NONE $call = { " + parameter + " = { " + ut->GetItemList(i).ToString() + " } } }", "TRUE", _excuteData, &builder);

				const std::string return_value = excute_module("Main = { $call = { id = NONE } }", &global, _excuteData, opt, 0);

				wiz::load_data::UserType return_value_ut;

				wiz::load_data::LoadData::LoadDataFromString(return_value, return_value_ut);

				ut->GetItemList(i).SetName(return_value_ut.GetItemList(0).GetName());
				ut->GetItemList(i).Set(0, return_value_ut.GetItemList(0).Get(0));

				if (count != 0) {
				eventStack.top().return_value = eventStack.top().return_value + " ";
				}

				eventStack.top().return_value = eventStack.top().return_value + return_value;

				count++;
				}
				}

				eventStack.top().userType_idx.top()++;
				break;
				}
				else
				*/
				if ("$cond" == val->GetName()) {
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					std::string cond = val->ToString();


					cond = wiz::load_data::LoadData::DoCondition(global, cond, _excuteData, &builder);

					eventStack[eventStack.size() - 1].return_value = cond;

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$copy" == val->GetName()) { // to = { } from = { } option = { 1 : internal data, default, 2 : total data }
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					const std::string to = val->GetUserTypeList(0)->ToString();
					const std::string from = val->GetUserTypeList(1)->ToString();
					std::string option = "1";	// 1 : internal
												// 2 : total.
					if (val->GetUserTypeListSize() >= 3) {
						option = val->GetUserTypeList(2)->ToString();
					}

					wiz::load_data::UserType* to_ut = wiz::load_data::UserType::Find(&global, to, &builder).second[0];
					wiz::load_data::UserType* from_ut = wiz::load_data::UserType::Find(&global, from, &builder).second[0];

					if (to_ut == from_ut) {
						std::cout << "to_ut == from_ut\n";

						eventStack.top().userType_idx.top()++;
						break;
					}
					if ("1" == option) {
						int itCount = 0;
						int utCount = 0;
						for (int i = 0; i < from_ut->GetIListSize(); ++i) {
							if (from_ut->IsItemList(i)) {
								to_ut->AddItemList(from_ut->GetItemList(itCount));
								itCount++;
							}
							else {
								to_ut->AddUserTypeItem(*from_ut->GetUserTypeList(utCount));
								utCount++;
							}
						}
					}
					else {
						to_ut->AddUserTypeItem(*from_ut);
					}
					
					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$iterate" == val->GetName()) { // very slow? why??
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					std::string dir = val->GetUserTypeList(0)->ToString();
					std::vector<std::string> events; // event_ids

					for (int i = 0; i < val->GetUserTypeList(1)->GetItemListSize(); ++i) {
						events.push_back(wiz::ToString(val->GetUserTypeList(1)->GetItemList(i).Get(0)));
					}

					std::string recursive = "FALSE";

					if (val->GetUserTypeListSize() >= 3) {
						recursive = val->GetUserTypeList(2)->ToString();
						recursive = wiz::load_data::LoadData::ToBool4(nullptr, global, recursive, _excuteData, &builder);
					}

					std::string before_value;

					if (val->GetUserTypeListSize() >= 4) {
						before_value = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(3)->ToString(), _excuteData, &builder);
					}
					wiz::load_data::LoadData::Iterate(global, dir, events, recursive, before_value, _excuteData, &builder);


					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$iterateA" == val->GetName()) { // very slow? why??
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					std::string dir = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(0)->ToString(), _excuteData, &builder);
					std::vector<std::string> events; // event_ids

					for (int i = 0; i < val->GetUserTypeList(1)->GetItemListSize(); ++i) {
						events.push_back(wiz::ToString(val->GetUserTypeList(1)->GetItemList(i).Get(0)));
					}

					std::string recursive = "FALSE";

					if (val->GetUserTypeListSize() >= 3) {
						recursive = val->GetUserTypeList(2)->ToString();
						recursive = wiz::load_data::LoadData::ToBool4(nullptr, global, recursive, _excuteData, &builder);
					}

					std::string before_value;

					if (val->GetUserTypeListSize() >= 4) {
						before_value = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(3)->ToString(), _excuteData, &builder);
					}
					wiz::load_data::LoadData::Iterate(global, dir, events, recursive, before_value, _excuteData, &builder);


					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$iterate2" == val->GetName()) { // very slow? why??
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					std::string dir = val->GetUserTypeList(0)->ToString();
					std::vector<std::string> events; // event_ids

					for (int i = 0; i < val->GetUserTypeList(1)->GetItemListSize(); ++i) {
						events.push_back(wiz::ToString(val->GetUserTypeList(1)->GetItemList(i).Get(0)));
					}

					std::string recursive = "FALSE";

					if (val->GetUserTypeListSize() >= 3) {
						recursive = val->GetUserTypeList(2)->ToString();
						recursive = wiz::load_data::LoadData::ToBool4(nullptr, global, recursive, _excuteData, &builder);
					}
					wiz::load_data::LoadData::Iterate2(global, dir, events, recursive, _excuteData, &builder);


					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$iterate3" == val->GetName()) { //
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					std::string dir = val->GetUserTypeList(0)->ToString();
					std::string name = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(1)->ToString(), _excuteData, &builder);
					std::vector<std::string> events; // event_ids

					for (int i = 0; i < val->GetUserTypeList(2)->GetItemListSize(); ++i) {
						events.push_back(wiz::ToString(val->GetUserTypeList(2)->GetItemList(i).Get(0)));
					}

					std::string recursive = "FALSE";

					if (val->GetUserTypeListSize() >= 4) {
						recursive = val->GetUserTypeList(3)->ToString();
						recursive = wiz::load_data::LoadData::ToBool4(nullptr, global, recursive, _excuteData, &builder);
					}

					std::string before_value;

					if (val->GetUserTypeListSize() >= 5) {
						before_value = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(4)->ToString(), _excuteData, &builder);
					}
					wiz::load_data::LoadData::Iterate3(global, dir, name, events, recursive, before_value, _excuteData, &builder);


					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$riterate" == val->GetName()) { // very slow? why??
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					std::string dir = val->GetUserTypeList(0)->ToString();
					std::vector<std::string> events; // event_ids

					for (int i = 0; i < val->GetUserTypeList(1)->GetItemListSize(); ++i) {
						events.push_back(wiz::ToString(val->GetUserTypeList(1)->GetItemList(i).Get(0)));
					}

					std::string recursive = "FALSE";

					if (val->GetUserTypeListSize() >= 3) {
						recursive = val->GetUserTypeList(2)->ToString();
						recursive = wiz::load_data::LoadData::ToBool4(nullptr, global, recursive, _excuteData, &builder);
					}
					wiz::load_data::LoadData::RIterate(global, dir, events, recursive, _excuteData, &builder);


					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$while" == val->GetName()) {
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					const std::string condition = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(0)->ToString(), _excuteData, &builder);

					if ("TRUE" == condition) {
						eventStack.top().conditionStack.push("TRUE");
						eventStack.top().nowUT.push(val->GetUserTypeList(1));
						eventStack.top().userType_idx.push(0);
						break;
					}
					else {
						eventStack.top().userType_idx.top()++;
						break;
					}
				}
				else if ("$do" == val->GetName()) { // chk? - need example!
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;
					_excuteData.noUseInput = excuteData.noUseInput; //// check!
					_excuteData.noUseOutput = excuteData.noUseOutput;


					wiz::load_data::UserType subGlobal;
					wiz::load_data::LoadData::LoadDataFromString(val->GetUserTypeList(1)->ToString(), subGlobal);
					wiz::load_data::UserType inputUT;
					wiz::load_data::LoadData::LoadDataFromString(wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(0)->ToString(), _excuteData, &builder), inputUT);


					wiz::load_data::LoadData::AddData(subGlobal, "/./", inputUT.ToString(), "TRUE", _excuteData, &builder);

					{
						ExcuteData _excuteData;
						_excuteData.noUseInput = excuteData.noUseInput;
						_excuteData.noUseOutput = excuteData.noUseOutput;

						Option opt;
						eventStack.top().return_value = excute_module("", &subGlobal, _excuteData, opt, 0); // return ?
					}

					eventStack.top().userType_idx.top()++;
					break;
				}
				// add option! for "".support eu3, eu4.
				else if ("$replace_datatype1" == val->GetName()) { // name
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					std::string rex = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(0)->ToString(), _excuteData, &builder);
					rex = rex.substr(1, rex.size() - 2);
					std::vector<std::string> sval;
					std::vector<std::string> scondition;
					std::string start_dir = "root";

					if (val->GetUserTypeListSize() >= 3)
					{
						const int n = (val->GetUserTypeListSize() - 3); // chk
						for (int i = 0; i < n; i = i + 2) { // chk
							sval.push_back(val->GetUserTypeList(1 + i)->ToString());

							scondition.push_back(val->GetUserTypeList(2 + i)->ToString());
						}
					}
					if (val->GetUserTypeListSize() >= 4) {
						start_dir = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(val->GetUserTypeListSize() - 2)->ToString(), _excuteData, &builder);
					}
					bool recursive = true;
					if (val->GetUserTypeListSize() >= 5) {
						recursive = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(val->GetUserTypeListSize() - 1)->ToString(), _excuteData, &builder) == "TRUE" ? true : false;
					}
					wiz::load_data::LoadData::ReplaceDataType1(global, rex, sval, scondition, start_dir, _excuteData, recursive, &builder);

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$replace_datatype1_2" == val->GetName()) { //val
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					std::string rex = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(0)->ToString(), _excuteData, &builder);
					rex = rex.substr(1, rex.size() - 2);
					std::vector<std::string> sval;
					std::vector<std::string> scondition;
					std::string start_dir = "root";

					if (val->GetUserTypeListSize() >= 3)
					{
						const int n = (val->GetUserTypeListSize() - 3); // chk
						for (int i = 0; i < n; i = i + 2) { // chk
							sval.push_back(val->GetUserTypeList(1 + i)->ToString());

							scondition.push_back(val->GetUserTypeList(2 + i)->ToString());
						}
					}

					if (val->GetUserTypeListSize() >= 4) {
						start_dir = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(val->GetUserTypeListSize() - 2)->ToString(), _excuteData, &builder);
					}
					bool recursive = true;
					if (val->GetUserTypeListSize() >= 5) {
						recursive = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(val->GetUserTypeListSize() - 1)->ToString(), _excuteData, &builder) == "TRUE" ? true : false;
					}

					wiz::load_data::LoadData::ReplaceDataType1_2(global, rex, sval, scondition, start_dir, _excuteData, recursive, &builder);

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$replace_datatype2" == val->GetName()) { // usertype name
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					std::string rex = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(0)->ToString(), _excuteData, &builder);
					rex = rex.substr(1, rex.size() - 2);
					//std::cout << val->GetUserTypeList(1)->ToString() << std::endl;
					std::vector<std::string> sval;
					std::vector<std::string> scondition;
					std::string start_dir = "root";

					if (val->GetUserTypeListSize() >= 3)
					{
						const int n = (val->GetUserTypeListSize() - 3); // chk
						for (int i = 0; i < n; i = i + 2) { // chk
							sval.push_back(val->GetUserTypeList(1 + i)->ToString());

							scondition.push_back(val->GetUserTypeList(2 + i)->ToString());
						}
					}

					if (val->GetUserTypeListSize() >= 4) {
						start_dir = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(val->GetUserTypeListSize() - 1)->ToString(), _excuteData, &builder);
					}
					bool recursive = true;
					if (val->GetUserTypeListSize() >= 5) {
						recursive = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(val->GetUserTypeListSize() - 2)->ToString(), _excuteData, &builder) == "TRUE" ? true : false;
					}
					//std::cout << scondition << std::endl;
					//std::cout << "sval " << sval << std::endl;
					wiz::load_data::LoadData::ReplaceDataType2(global, rex, sval, scondition, start_dir, _excuteData, recursive, &builder);

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$replace_datetype" == val->GetName()) {
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					std::string sval = val->GetUserTypeList(0)->ToString();

					std::string scondition = "TRUE";
					std::string start_dir = "root";

					if (val->GetUserTypeListSize() >= 2)
					{
						scondition = val->GetUserTypeList(1)->ToString();
					}
					if (val->GetUserTypeListSize() >= 3) {
						start_dir = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(2)->ToString(), _excuteData, &builder);
					}
					bool recursive = true;
					if (val->GetUserTypeListSize() >= 4) {
						recursive = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(3)->ToString(), _excuteData, &builder) == "TRUE" ? true : false;
					}
					wiz::load_data::LoadData::ReplaceDateType(global, sval, scondition, start_dir, _excuteData, recursive, &builder);

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$replace_datetype2" == val->GetName()) {
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					std::string sval = val->GetUserTypeList(0)->ToString();
					std::string scondition = "TRUE";
					std::string start_dir = "root";


					if (val->GetUserTypeListSize() >= 2)
					{
						scondition = val->GetUserTypeList(1)->ToString();
					}
					if (val->GetUserTypeListSize() >= 3) {
						start_dir = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(2)->ToString(), _excuteData, &builder);
					}
					bool recursive = true;
					if (val->GetUserTypeListSize() >= 4) {
						recursive = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(3)->ToString(), _excuteData, &builder) == "TRUE" ? true : false;
					}
					wiz::load_data::LoadData::ReplaceDateType2(global, sval, scondition, start_dir, _excuteData, recursive, &builder);

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$remove_usertype_total" == val->GetName()) { //// has bug?
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					std::string ut_name = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(0)->ToString(), _excuteData, &builder);
					std::string condition = "TRUE";
					std::string start_dir = "root";

					if (val->GetUserTypeListSize() >= 2)
					{
						condition = val->GetUserTypeList(1)->ToString();
					}
					if (val->GetUserTypeListSize() >= 3) {
						start_dir = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(2)->ToString(), _excuteData, &builder);
					}
					bool recursive = false;

					wiz::load_data::LoadData::RemoveUserTypeTotal(global, ut_name, condition, start_dir, _excuteData, recursive, &builder);

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$replace_item" == val->GetName()) {
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;
					// chk nullptr vs val?
					std::string svar = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(0)->ToString(), _excuteData, &builder);
					std::string sval = val->GetUserTypeList(1)->ToString();
					std::string scondition = "TRUE";
					std::string start_dir = "root";

					if (val->GetUserTypeListSize() >= 3)
					{
						scondition = val->GetUserTypeList(2)->ToString();
					}
					if (val->GetUserTypeListSize() >= 4) {
						start_dir = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(3)->ToString(), _excuteData, &builder);
					}
					bool recursive = true;
					if (val->GetUserTypeListSize() >= 5) {
						recursive = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(4)->ToString(), _excuteData, &builder) == "TRUE" ? true : false;
					}
					wiz::load_data::LoadData::ReplaceItem(global, svar, sval, scondition, start_dir, _excuteData, recursive, &builder);

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$edit_mode" == val->GetName()) // chk!!
				{
#ifdef _MSC_VER
					if (excuteData.noUseInput || excuteData.noUseOutput) {
						eventStack.top().userType_idx.top()++;
						break;
					}
					MStyleTest(&global);

					system("cls");

					gotoxy(0, 0);
					setcolor(7, 0);
#endif
					eventStack.top().userType_idx.top()++;
					break;
				}
				// lint
				else if ("$lint" == val->GetName()) 
				{
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;


					std::string schemaName = val->GetUserTypeList(0)->ToString();
					std::string clauName = val->GetUserTypeList(1)->ToString();

					bool pass = wiz::Lint::_Validate(wiz::load_data::UserType::Find(&global, "/./" + schemaName, &builder).second[0],
						wiz::load_data::UserType::Find(&global, "/./" + clauName, &builder).second[0]);

					eventStack.top().return_value = pass ? "TRUE" : "FALSE";

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$save" == val->GetName()) // save data, event, main!
				{
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;
					//todo
					// "filename" save_option(0~2)
					std::string fileName = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(0)->ToString(), _excuteData, &builder);
					fileName = wiz::String::substring(fileName, 1, fileName.size() - 2);
					std::string option = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(1)->ToString(), _excuteData, &builder);

					wiz::load_data::LoadData::SaveWizDB(global, fileName, option, "");
					wiz::load_data::LoadData::SaveWizDB(Main, fileName, option, "APPEND");
					wiz::load_data::LoadData::SaveWizDB(events, fileName, option, "APPEND");

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$save_data_only" == val->GetName())
				{
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;
					//todo
					// "filename" save_option(0~2)
					std::string fileName = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(0)->ToString(), _excuteData, &builder);
					if (fileName.back() == '\"' && fileName.size() >= 2 && fileName[0] == fileName.back()) {
						fileName = wiz::String::substring(fileName, 1, fileName.size() - 2);
					}
					std::string option = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(1)->ToString(), _excuteData, &builder);

					wiz::load_data::LoadData::SaveWizDB(global, fileName, option, "");

					eventStack.top().userType_idx.top()++;
					break;
				}

				else if ("$save_data_only2" == val->GetName())
				{
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;
					//todo
					// "filename" save_option(0~2)
					std::string dirName = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(0)->ToString(), _excuteData, &builder);
					std::string fileName = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(1)->ToString(), _excuteData, &builder);
					
					if (fileName.back() == '\"' && fileName.size() >= 2 && fileName[0] == fileName.back()) {
						fileName = wiz::String::substring(fileName, 1, fileName.size() - 2);
					}
					std::string option = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(2)->ToString(), _excuteData, &builder);

					// todo - for? auto x = global.GetUserTypeItem(dirName);
					wiz::load_data::UserType* utTemp;

					utTemp = wiz::load_data::UserType::Find(&global, dirName, &builder).second[0];

					wiz::load_data::LoadData::SaveWizDB(*utTemp, fileName, option, "");


					eventStack.top().userType_idx.top()++;
					break;
				}
				/// module name or object name -> must "~" .
				else if ("$register_module" == val->GetName())
				{
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					std::string moduleFileName = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(0)->ToString(), _excuteData, &builder);
					moduleFileName = wiz::String::substring(moduleFileName, 1, moduleFileName.size() - 2);

					wiz::load_data::UserType moduleUT;
					wiz::load_data::LoadData::LoadDataFromFile(moduleFileName, moduleUT);

					//moduleMapPtr.insert(std::make_pair(moduleFileName, moduleUT));
					(*moduleMapPtr)[moduleFileName] = std::move(moduleUT);

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$call_registered_module" == val->GetName())
				{
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;
					_excuteData.noUseInput = excuteData.noUseInput;
					_excuteData.noUseOutput = excuteData.noUseOutput;

					std::string moduleFileName = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(0)->ToString(), _excuteData, &builder);
					std::string input;

					if (val->GetUserTypeListSize() >= 2) {
						input = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(1)->ToString(), _excuteData, &builder);
					}

					moduleFileName = wiz::String::substring(moduleFileName, 1, moduleFileName.size() - 2);

					wiz::load_data::UserType moduleUT = (*moduleMapPtr).at(moduleFileName);
					wiz::load_data::LoadData::AddData(moduleUT, "", input, "TRUE", _excuteData, &builder);

					{
						ExcuteData _excuteData;
						_excuteData.noUseInput = excuteData.noUseInput;
						_excuteData.noUseOutput = excuteData.noUseOutput;
						
						Option opt;
						eventStack.top().return_value = excute_module("", &moduleUT, _excuteData, opt, 0);
					}
					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$module" == val->GetName())
				{
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					std::string moduleFileName = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(0)->ToString(), _excuteData, &builder);
					std::string input;

					if (val->GetUserTypeListSize() >= 2) {
						input = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(1)->ToString(), _excuteData, &builder);
					}

					moduleFileName = wiz::String::substring(moduleFileName, 1, moduleFileName.size() - 2);

					wiz::load_data::UserType moduleUT;
					wiz::load_data::LoadData::LoadDataFromFile(moduleFileName, moduleUT);
					wiz::load_data::LoadData::AddData(moduleUT, "", input, "TRUE", _excuteData, &builder);

					{
						ExcuteData _excuteData;
						_excuteData.noUseInput = excuteData.noUseInput;
						_excuteData.noUseOutput = excuteData.noUseOutput;

						Option opt;
						eventStack.top().return_value = excute_module("", &moduleUT, _excuteData, opt, 0);
					}

					eventStack.top().userType_idx.top()++;
					break;
				}
				// todo - register module from file
				// todo	- register module from std::string
				// todo - call registered module.  $registered_module = { name = { ~ } input = { input = { n = 1 } } }

				else if ("$register_object" == val->GetName()) {
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					std::string objectFileName = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(0)->ToString(), _excuteData, &builder);

					objectFileName = wiz::String::substring(objectFileName, 1, objectFileName.size() - 2);

					wiz::load_data::UserType objectUT;
					wiz::load_data::LoadData::LoadDataFromFile(objectFileName, objectUT);

					//objectMapPtr.insert(std::make_pair(objectFileName, data));
					(*objectMapPtr)[objectFileName] = std::move(objectUT);

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$copy_object" == val->GetName()) {
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					std::string objectNameA = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(0)->ToString(), _excuteData, &builder);
					std::string objectNameB = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(1)->ToString(), _excuteData, &builder);

					objectNameA = wiz::String::substring(objectNameA, 1, objectNameA.size() - 2);
					objectNameB = wiz::String::substring(objectNameB, 1, objectNameB.size() - 2);

					(*objectMapPtr)[objectNameB] = (*objectMapPtr).at(objectNameA);

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$register_object_from_string" == val->GetName()) {
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					std::string objectName = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(0)->ToString(), _excuteData, &builder);
					std::string objectData = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(1)->ToString(), _excuteData, &builder);
					objectName = wiz::String::substring(objectName, 1, objectName.size() - 2);
					objectData = wiz::String::substring(objectData, 1, objectData.size() - 2);
					wiz::load_data::UserType objectUT;
					wiz::load_data::LoadData::LoadDataFromString(objectData, objectUT); // error chk?

																						//objectMapPtr.insert(std::make_pair(objectFileName, data));
					(*objectMapPtr)[objectName] = std::move(objectUT);

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$call_registered_object" == val->GetName()) {
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					std::string objectFileName = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(0)->ToString(), _excuteData, &builder);
					objectFileName = wiz::String::substring(objectFileName, 1, objectFileName.size() - 2);
					std::string parameter;
					wiz::load_data::UserType objectUT = objectMapPtr->at(objectFileName);

					parameter = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(1)->ToString(), _excuteData, &builder);

					std::string data = " Event = { id = NONE $call = { " + parameter + "  } } ";

					wiz::load_data::LoadData::AddData(objectUT, "/./", data, "TRUE", _excuteData, &builder);
					{
						ExcuteData _excuteData;
						_excuteData.noUseInput = excuteData.noUseInput;
						_excuteData.noUseOutput = excuteData.noUseOutput;
						Option _opt;
						
						eventStack.top().return_value = excute_module(" Main = { $call = { id = NONE } } ", &objectUT, _excuteData, _opt, 0);
					}
					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$call_registered_object2" == val->GetName()) {
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					std::string objectFileName = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(0)->ToString(), _excuteData, &builder);
					objectFileName = wiz::String::substring(objectFileName, 1, objectFileName.size() - 2);
					std::string parameter;

					parameter = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(1)->ToString(), _excuteData, &builder);

					std::string data = " Event = { id = NONE $call = { " + parameter + "  } } ";

					wiz::load_data::UserType objectUT = objectMapPtr->at(objectFileName);
					wiz::load_data::UserType objectUT2 = objectUT;
					wiz::load_data::LoadData::AddData(objectUT, "/./", data, "TRUE", _excuteData, &builder);
					{
						ExcuteData _excuteData;
						_excuteData.noUseInput = excuteData.noUseInput;
						_excuteData.noUseOutput = excuteData.noUseOutput;

						Option _opt;
						eventStack.top().return_value = excute_module(" Main = { $call = { id = NONE } } ", &objectUT, _excuteData, _opt, 0);
					}
					{
						std::vector<wiz::load_data::UserType*> _events;
						wiz::load_data::UserType events;

						_events = objectUT2.GetCopyUserTypeItem("Event");
						for (int i = 0; i < _events.size(); ++i) {
							events.LinkUserType(_events[i]);
						}

						wiz::load_data::LoadData::AddData(objectUT, "/./", events.ToString(), "TRUE", _excuteData, &builder);
					}
					(*objectMapPtr)[objectFileName] = std::move(objectUT);

					eventStack.top().userType_idx.top()++;
					break;
				}
				/// object of class?
				else if ("$object" == val->GetName()) { // "fileName"
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					std::string objectFileName = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(0)->ToString(), _excuteData, &builder);
					objectFileName = wiz::String::substring(objectFileName, 1, objectFileName.size() - 2);
					std::string parameter;

					parameter = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(1)->ToString(), _excuteData, &builder);


					wiz::load_data::UserType objectUT;
					wiz::load_data::LoadData::LoadDataFromFile(objectFileName, objectUT);

					std::string data = objectUT.ToString();

					data = data + "Event = { id = NONE $call = { " + parameter + "  } } ";

					objectUT.Remove();
					wiz::load_data::LoadData::LoadDataFromString(data, objectUT);
					{
						ExcuteData _excuteData;
						_excuteData.noUseInput = excuteData.noUseInput;
						_excuteData.noUseOutput = excuteData.noUseOutput;

						Option opt;
						eventStack.top().return_value = excute_module(" Main = { $call = { id = NONE } } ", &objectUT, _excuteData, opt, 0);
					}
					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$object_from_string" == val->GetName()) {
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					std::string data = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(0)->ToString(), _excuteData, &builder);
					std::string parameter;

					parameter = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(1)->ToString(), _excuteData, &builder);


					wiz::load_data::UserType objectUT;
					wiz::load_data::LoadData::LoadDataFromString(data, objectUT);

					std::string str = objectUT.ToString();

					str = str + " Event = { id = NONE $call = { " + parameter + "  } } ";

					objectUT.Remove();
					wiz::load_data::LoadData::LoadDataFromString(str, objectUT);
					{
						ExcuteData _excuteData;
						_excuteData.noUseInput = excuteData.noUseInput;
						_excuteData.noUseOutput = excuteData.noUseOutput;
						Option _opt;
						eventStack.top().return_value = excute_module(" Main = { $call = { id = NONE } } ", &objectUT, _excuteData, _opt, 0);
					}
					eventStack.top().userType_idx.top()++;
					break;
				}

				// todo - register object from file.
				//		~.ToString() + "Main = { $call = { id = 0 } } Event = { id = 0 $call = { id = " + id_val + " " + param_name1 + " = " + param_val1 + "  } } "
				// todo - register object from std::string.
				// todo - call registered object.  $registered_object = { name = { "ex2.txt" } parameter = { id = 1 i = 1 j = 1 } }  


				else if ("$option" == val->GetName()) // first
				{
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					eventStack.top().option = wiz::load_data::LoadData::ToBool4(nullptr, global, val->ToString(), _excuteData, &builder);

					eventStack.top().userType_idx.top()++;
					break;
				}
				// done - ($push_back-insert!) $pop_back, $push_front, $pop_front ($front?, $back?)
				else if ("$pop_back" == val->GetName()) {
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					std::string dir = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(0)->ToString(), _excuteData, &builder);
					wiz::load_data::UserType* ut = nullptr;
					auto finded = wiz::load_data::UserType::Find(&global, dir, &builder);
					ut = finded.second[0];

					std::string condition = "TRUE";
					if (val->GetUserTypeListSize() >= 2) {
						condition = val->GetUserTypeList(1)->ToString();
					}
					wiz::load_data::LoadData::Remove(global, dir, ut->GetIListSize() - 1, condition, _excuteData, &builder);

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$push_front" == val->GetName()) {
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					std::string value = val->GetUserTypeList(1)->ToString();
					std::string dir;
					if (val->GetUserTypeList(0)->GetItemListSize() > 0) {
						dir = wiz::ToString(val->GetUserTypeList(0)->GetItemList(0).Get(0));
						dir = wiz::load_data::LoadData::ToBool4(nullptr, global, dir, _excuteData, &builder);
					}
					else ///val->Ge
					{
						dir = std::string(val->GetUserTypeList(0)->ToString());
						dir = wiz::load_data::LoadData::ToBool4(nullptr, global, dir, _excuteData, &builder);
					}


					value = wiz::load_data::LoadData::ToBool4(nullptr, global, value, _excuteData, &builder);

					std::string condition = "TRUE";
					if (val->GetUserTypeListSize() >= 3) {
						condition = val->GetUserTypeList(2)->ToString();
					}
					wiz::load_data::LoadData::AddDataAtFront(global, dir, value, condition, _excuteData, &builder);

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$pop_front" == val->GetName()) {
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					std::string dir = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(0)->ToString(), _excuteData, &builder);

					std::string condition = "TRUE";
					if (val->GetUserTypeListSize() >= 2) {
						condition = val->GetUserTypeList(1)->ToString();
					}
					wiz::load_data::LoadData::Remove(global, dir, 0, condition, _excuteData, &builder);

					eventStack.top().userType_idx.top()++;
					break;
				}
				/*
				else if ("$wait" == val->GetName()) {
				for (int i = 0; i < waits.size(); ++i) {
				waits[i]->join();
				delete waits[i];
				}
				waits.resize(0);

				eventStack.top().userType_idx.top()++;
				break;
				}
				*/
				
				// todo - remove??
				else if ("$call_lambda" == val->GetName())
				{
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					std::string data = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(0)->ToString(), _excuteData, &builder);
					data = data.substr(1, data.size() - 2);
					std::string parameter; // = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(1)->ToString(), _excuteData, &builder);

					wiz::load_data::UserType dataUT;

					wiz::load_data::LoadData::LoadDataFromString(data, dataUT);

					{
						std::string id = dataUT.GetUserTypeItem("Event")[0]->GetItem("id")[0].Get(0).ToString();
						{
							EventInfo info;
							for (int j = 0; j < val->GetItemListSize(); ++j) {
								if (val->GetUserTypeList(1)->GetItemListSize() > 0) {
									std::string temp = wiz::ToString(val->GetUserTypeList(1)->GetItemList(j).Get(0));
									std::pair<std::string, std::string> temp2(wiz::ToString(val->GetUserTypeList(1)->GetItemList(j).GetName()), temp);
									info.parameters.insert(temp2);
								}
							}

							for (auto& x : info.parameters) {
								parameter += x.first;
								parameter += " = ";
								parameter += "^" + x.second;
							}
						}
						{
							for (int j = 0; j < val->GetUserTypeList(1)->GetUserTypeListSize(); ++j) {
								if (val->GetUserTypeList(1)->GetUserTypeListSize() > 0) {
									std::string temp = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(1)->GetUserTypeList(j)->ToString(), _excuteData, &builder);
									auto temp2 = std::pair<std::string, std::string>(wiz::ToString(val->GetUserTypeList(1)->GetUserTypeList(j)->GetName()), temp);
									info.parameters.insert(temp2);
								}
							}
							for (auto& x : info.parameters) {
								parameter += x.first;
								parameter += " = { ";
								parameter += "^" + x.second;
								parameter += " } ";
							}
						}

						data += " Event = { id = NONE $call = { id = " + id + " " + parameter + "  } } ";

						wiz::load_data::UserType objectUT;
						wiz::load_data::LoadData::LoadDataFromString(data, objectUT);
						Option opt;
						eventStack.top().return_value = excute_module(" Main = { $call = { id = NONE } } ", &objectUT, ExcuteData(), opt, 0);
					}

					eventStack.top().userType_idx.top()++;
					break;
				}
				
				else if ("$call" == val->GetName()) {
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					if (!val->GetItem("id").empty()) {
						info.id = wiz::ToString(val->GetItem("id")[0].Get(0));
					}
					else {
						//// removal?
						info.id = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeItem("id")[0]->ToString(), _excuteData, &builder);
					}

					info.eventUT = eventPtr->GetUserTypeList(no);
					info.userType_idx.clear();
					info.userType_idx.push(0);
					info.return_value.clear();
					info.nowUT.clear();

					EventInfo info2; //
					info2 = info;

					if (info.id != eventStack.top().id) {
						info.parameters.clear();
					}
					info.conditionStack.clear();

					//
					if (info.id != eventStack.top().id) {
						for (int j = 0; j < val->GetItemListSize(); ++j) {
							if (val->GetItemListSize() > 0) {
								_excuteData.info = info2;
								std::string temp = wiz::ToString(val->GetItemList(j).Get(0));
								auto temp2 = std::pair<std::string, std::string>(wiz::ToString(val->GetItemList(j).GetName()), temp);

								info.parameters.insert(temp2);
							}
						}
						for (int j = 0; j < val->GetUserTypeListSize(); ++j) {
							if (val->GetUserTypeListSize() > 0) {
								_excuteData.info = info2;
								std::string temp = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(j)->ToString(), _excuteData, &builder);
								auto temp2 = std::pair<std::string, std::string>(wiz::ToString(val->GetUserTypeList(j)->GetName()), temp);
								info.parameters.insert(temp2);
							}
						}
						eventStack.top().userType_idx.top()++;
					}
					else { // recursive call!
						if (val->GetItemListSize() > 0) {
							for (int j = 0; j < val->GetItemListSize(); ++j) {
								_excuteData.info = info;
								_excuteData.info.parameters = info2.parameters;

								std::string temp = wiz::ToString(val->GetItemList(j).Get(0));

								wiz::ArrayMap<std::string, std::string>::iterator x;
								if ((x = info.parameters.find(wiz::ToString(val->GetItemList(j).GetName()))) != info.parameters.end())
								{
									x->second = temp;
								}
							}
						}
						if (val->GetUserTypeListSize() > 0) {
							for (int j = 0; j < val->GetUserTypeListSize(); ++j) {
								_excuteData.info = info;
								_excuteData.info.parameters = info2.parameters;

								std::string temp = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(j)->ToString(), _excuteData, &builder);

								wiz::ArrayMap<std::string, std::string>::iterator x;
								if ((x = info.parameters.find(wiz::ToString(val->GetUserTypeList(j)->GetName()))) != info.parameters.end())
								{
									x->second = temp;
								}
							}
						}

						eventStack.top().userType_idx.top()++;

						// remove remove_now_event_stack_a?
						if ("REMOVE_IF_CALL_ONESELF_EVENT" == eventStack.top().option) //
						{
							eventStack.pop();
						}
					}

					if (false == eventStack.empty() && eventStack.top().option == "REMOVE_IF_CALL_ANY_EVENT")
					{
						eventStack.pop();
					}


					info.locals.clear();
					const int no = convert.at(info.id);
					for (int i = 0; i < eventPtr->GetUserTypeList(no)->GetUserTypeListSize(); ++i) {
						if (eventPtr->GetUserTypeList(no)->GetUserTypeList(i)->GetName() == "$local") {
							for (int j = 0; j < eventPtr->GetUserTypeList(no)->GetUserTypeList(i)->GetItemListSize(); ++j) {
								std::string name = wiz::ToString(eventPtr->GetUserTypeList(no)->GetUserTypeList(i)->GetItemList(j).Get(0));
								std::string value = "";
								std::pair<std::string, std::string> temp(name, value);
								info.locals.insert(temp);
							}
							break;
						}
					}
					/*
					if (waits.size() >= 4) {
					for (int i = 0; i < waits.size(); ++i) {
					waits[i]->join();
					delete waits[i]; // chk ?
					}
					waits.resize(0);
					}
					*/

					/*if (false == val->GetItem("option").empty() && val->GetItem("option")[0].Get(0) == "USE_THREAD") {
					_excuteData.info = info;

					_excuteData.noUseInput = excuteData.noUseInput;
					_excuteData.noUseOutput = excuteData.noUseOutput;

					std::thread* A = new std::thread(excute_module, "", &global, _excuteData, opt, 0);

					waits.push_back(A);
					}*/
					//else {
					eventStack.push(info);
					//}

					break;
				}
				else if ("$call_by_data" == val->GetName()) {
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;
					_excuteData.noUseInput = excuteData.noUseInput; //// check!
					_excuteData.noUseOutput = excuteData.noUseOutput;


					std::string dir = val->GetItemList(0).ToString();
					wiz::load_data::UserType subGlobal;
					wiz::load_data::LoadData::LoadDataFromString(global.GetUserTypeItem(dir)[0]->ToString(), subGlobal);

					{
						ExcuteData _excuteData;
						_excuteData.noUseInput = excuteData.noUseInput;
						_excuteData.noUseOutput = excuteData.noUseOutput;
						Option _opt;
						eventStack.top().return_value = excute_module("", &subGlobal, _excuteData, _opt, 0);  // return ?
					}

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$call_by_data2" == val->GetName()) {
				ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
				_excuteData.chkInfo = true;
				_excuteData.info = eventStack.top();
				_excuteData.pObjectMap = objectMapPtr;
				_excuteData.pEvents = eventPtr;
				_excuteData.pModule = moduleMapPtr;
				_excuteData.noUseInput = excuteData.noUseInput; //// check!
				_excuteData.noUseOutput = excuteData.noUseOutput;


				std::string dir = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(0)->ToString(), _excuteData, &builder);

				wiz::load_data::UserType subGlobal;
				wiz::load_data::LoadData::LoadDataFromString(global.GetUserTypeItem(dir)[0]->ToString(), subGlobal);

				{
					ExcuteData _excuteData;
					_excuteData.noUseInput = excuteData.noUseInput;
					_excuteData.noUseOutput = excuteData.noUseOutput;
					Option _opt;
					eventStack.top().return_value = excute_module("", &subGlobal, _excuteData, _opt, 0);  // return ?
				}

				eventStack.top().userType_idx.top()++;
				break;
				}
				//// no $parameter.~
				else if ("$assign" == val->GetName()) /// -> assign2?
				{
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					std::pair<std::string, std::string> dir = wiz::load_data::LoadData::Find2(&global, wiz::ToString(val->GetItemList(0).Get(0)));
					std::string data = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(0)->ToString(), _excuteData, &builder);

					if (dir.first == "" && wiz::String::startsWith(dir.second, "$local."))
					{
						eventStack.top().locals[wiz::String::substring(dir.second, 7)] = data;
					}
					else {
						wiz::load_data::LoadData::SetData(global, dir.first, dir.second, data, "TRUE", _excuteData, &builder);
					}
					eventStack.top().userType_idx.top()++;
					break;
				}

				else if ("$assign2" == val->GetName())
				{
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;


					std::pair<std::string, std::string> dir = wiz::load_data::LoadData::Find2(&global, wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(0)->ToString(), _excuteData, &builder));
					std::string data = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(1)->ToString(), _excuteData, &builder);

					{
						if (dir.first == "" && wiz::String::startsWith(dir.second, "$local."))
						{
							eventStack.top().locals[wiz::String::substring(dir.second, 7)] = data;
						}
						else {
							wiz::load_data::LoadData::SetData(global, dir.first, dir.second, data, "TRUE", _excuteData, &builder);
						}
					}

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$assign_local" == val->GetName()) /// no condition, 
				{
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					std::pair<std::string, std::string> dir = wiz::load_data::LoadData::Find2(&global, wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(0)->ToString(), _excuteData, &builder));
					std::string data = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(1)->ToString(), _excuteData, &builder);

					{
						if (dir.first == "" && dir.second.size() > 1 && dir.second[0] == '@')
						{
							dir.second.erase(dir.second.begin());
						}
						if (dir.first == "" && wiz::String::startsWith(dir.second, "$local."))
						{
							eventStack.top().locals[wiz::String::substring(dir.second, 7)] = data;
						}
						else {
							// throw error??
						}
					}

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$assign_global" == val->GetName()) // 二쇱쓽!! dir=> dir/name ( dir= { name = val } } , @瑜??욎뿉 遺숈뿬???쒕떎. 
				{
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					std::pair<std::string, std::string> dir = wiz::load_data::LoadData::Find2(&global, 
						wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(0)->ToString(), _excuteData, &builder));
					std::string data = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(1)->ToString(), _excuteData, &builder);

					//std::string condition;
					//if (val->GetUserTypeListSize() >= 3) {
					//	condition = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(2)->ToString(), _excuteData, &builder);
					//}
					wiz::load_data::LoadData::SetData(global, dir.first, dir.second, data, "TRUE", _excuteData, &builder);

					// chk local?

					eventStack.top().userType_idx.top()++;
					break;
				}

				else if ("$assign_from_ut" == val->GetName()) {
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					std::pair<std::string, std::string> dir = wiz::load_data::LoadData::Find2(&global, wiz::ToString(val->GetItemList(0).Get(0)));
					std::string ut_str = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(0)->ToString(), _excuteData, &builder);
					wiz::load_data::UserType ut;
					wiz::load_data::LoadData::LoadDataFromString(ut_str, ut);

					// check! ExcuteData() ?
					std::string data = wiz::load_data::LoadData::ToBool4(nullptr, ut, val->GetUserTypeList(1)->ToString(), _excuteData, &builder);

					if (dir.first == "" && wiz::String::startsWith(dir.second, "$local."))
					{
						eventStack.top().locals[wiz::String::substring(dir.second, 7)] = data;
					}
					else {
						wiz::load_data::LoadData::SetData(global, dir.first, dir.second, data, "TRUE", _excuteData, &builder);
					}
					eventStack.top().userType_idx.top()++;
					break;
				}

				/// cf) insert3? - any position?
				else if ("$push_back" == val->GetName() || "$insert" == val->GetName() || "$insert2" == val->GetName())
				{
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					std::string value = val->GetUserTypeList(1)->ToString();
					std::string dir;
					if (val->GetUserTypeList(0)->GetItemListSize() > 0) {
						dir = wiz::ToString(val->GetUserTypeList(0)->GetItemList(0).Get(0));
						dir = wiz::load_data::LoadData::ToBool4(nullptr, global, dir, _excuteData, &builder);
					}
					else ///val->Ge
					{
						dir = std::string(val->GetUserTypeList(0)->ToString());
						dir = wiz::load_data::LoadData::ToBool4(nullptr, global, std::move(dir), _excuteData, &builder);
					}

					value = wiz::load_data::LoadData::ToBool4(nullptr, global, std::move(value), _excuteData, &builder);

					std::string condition = "TRUE";
					if (val->GetUserTypeListSize() >= 3) {
						condition = val->GetUserTypeList(2)->ToString();
					}
					
					wiz::load_data::LoadData::AddData(global, dir, value, condition, _excuteData, &builder);
					
					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$insert_noname_usertype" == val->GetName())
				{
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					std::string position = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(0)->ToString(), _excuteData, &builder);;
					std::string data = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(1)->ToString(), _excuteData, &builder);;
					std::string condition = val->GetUserTypeList(2)->ToString();

					wiz::load_data::LoadData::AddNoNameUserType(global, position, data, condition, _excuteData, &builder);

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$insert_by_idx" == val->GetName())
				{
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					std::string value = val->GetUserTypeList(2)->ToString();
					int idx = atoi(wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(1)->ToString(), _excuteData, &builder).c_str());
					std::string dir;
					if (val->GetUserTypeList(0)->GetItemListSize() > 0) {
						dir = wiz::ToString(val->GetUserTypeList(0)->GetItemList(0).Get(0));
						dir = wiz::load_data::LoadData::ToBool4(nullptr, global, dir, _excuteData, &builder);
					}
					else ///val->Ge
					{
						dir = std::string(val->GetUserTypeList(0)->ToString());
						dir = wiz::load_data::LoadData::ToBool4(nullptr, global, dir, _excuteData, &builder);
					}

					value = wiz::load_data::LoadData::ToBool4(nullptr, global, value, _excuteData, &builder);

					std::string condition = "TRUE";
					if (val->GetUserTypeListSize() >= 4) {
						condition = val->GetUserTypeList(3)->ToString();
					}
					wiz::load_data::LoadData::Insert(global, dir, idx, value, condition, _excuteData, &builder);

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$make" == val->GetName()) // To Do? - make2? or remake? 
													// cf) make empty ut??
				{
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					std::string dir;
					bool is2 = false;
					if (val->GetItemListSize() > 0) { // remove?
						dir = wiz::ToString(val->GetItemList(0).Get(0));
						dir = wiz::load_data::LoadData::ToBool4(nullptr, global, dir, _excuteData, &builder);
					}
					else // 
					{
						dir = std::string(val->GetUserTypeList(0)->ToString());
						dir = wiz::load_data::LoadData::ToBool4(nullptr, global, dir, _excuteData, &builder);
						is2 = true;
					}

					std::string name;
					for (int i = dir.size() - 1; i >= 0; --i)
					{
						if (dir[i] == '/') {
							name = wiz::String::substring(dir, i + 1);
							dir = wiz::String::substring(dir, 0, i - 1);
							break;
						}
					}
					if (dir.empty()) { dir = "."; }

					std::string condition = "TRUE";
					if (is2 && val->GetUserTypeListSize() >= 2) {
						condition = val->GetUserTypeList(1)->ToString();
					}
					else if (false == is2 && val->GetUserTypeListSize() >= 1) {
						condition = val->GetUserTypeList(0)->ToString();
					}

					wiz::load_data::LoadData::AddUserType(global, dir, name, "", condition, _excuteData, &builder);


					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$findIndex" == val->GetName()) // For list : { 1 2  3 4 5 }
				{
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					std::string dir = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(0)->ToString(), _excuteData, &builder);
					std::string value = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(1)->ToString(), _excuteData, &builder);

					wiz::load_data::UserType ut;
					wiz::load_data::LoadData::LoadDataFromString(wiz::load_data::UserType::Find(&global, dir, &builder).second[0]->ToString(), ut);

					for (int i = 0; i < ut.GetItemListSize(); ++i) {
						if (wiz::ToString(ut.GetItemList(i).Get(0)) == value) {
							eventStack.top().return_value = wiz::toStr(i);
							break;
						}
					}

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$remove" == val->GetName()) // remove by dir., remove all?
				{
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					std::string dir = std::string(wiz::ToString(val->GetItemList(0).Get(0)).c_str()); // item -> userType

					dir = wiz::load_data::LoadData::ToBool4(nullptr, global, dir, _excuteData, &builder);

					std::string condition = "TRUE";
					if (val->GetUserTypeListSize() >= 1) {
						condition = val->GetUserTypeList(0)->ToString();
					}
					wiz::load_data::LoadData::Remove(global, dir, condition, _excuteData, &builder);

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$remove2" == val->GetName()) // remove /dir/name 
													   // if name is empty, then chk!!
				{
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					std::string dir = std::string(wiz::ToString(val->GetUserTypeList(0)->ToString()).c_str()); // item -> userType
					dir = wiz::load_data::LoadData::ToBool4(nullptr, global, dir, _excuteData, &builder);
					std::string name;
					for (int i = dir.size() - 1; i >= 0; --i)
					{
						if (dir[i] == '/') {
							name = wiz::String::substring(dir, i + 1);
							dir = wiz::String::substring(dir, 0, i - 1);
							break;
						}
					}

					std::string condition = "TRUE";
					if (val->GetUserTypeListSize() >= 2) {
						condition = val->GetUserTypeList(1)->ToString();
					}
					wiz::load_data::LoadData::Remove(global, dir, name, condition, _excuteData, &builder);

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$remove3" == val->GetName()) /// remove itemlist by idx.
				{
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					std::string dir = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(0)->ToString(), _excuteData, &builder);
					std::string value = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(1)->ToString(), _excuteData, &builder);

					int idx = atoi(value.c_str());  // long long -> int?

					std::string condition = "TRUE";

					if (val->GetUserTypeListSize() >= 3) {
						condition = val->GetUserTypeList(2)->ToString();
					}

					wiz::load_data::LoadData::Remove(global, dir, idx, condition, _excuteData, &builder);
					// remove -> UserType::Find(&global, dir).second[0]->RemoveItemList(idx); /// change ?

					eventStack.top().userType_idx.top()++;
					break;
				}

				else if ("$setElement" == val->GetName())
				{
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					std::string dir = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(0)->ToString(), _excuteData, &builder);
					std::string idx = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(1)->ToString(), _excuteData, &builder);
					std::string value = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(2)->ToString(), _excuteData, &builder);

					int _idx = stoi(idx);
					wiz::load_data::UserType::Find(&global, dir, &builder).second[0]->SetItem(_idx, value);

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$swap" == val->GetName()) // $swap2
				{
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					std::string dir = std::string(wiz::ToString(wiz::ToString(val->GetItemList(0).Get(0)).c_str())); // + 0
					std::string value1 = val->GetUserTypeList(0)->ToString();
					std::string value2 = val->GetUserTypeList(1)->ToString();

					value1 = wiz::load_data::LoadData::ToBool4(nullptr, global, value1, _excuteData, &builder);
					value2 = wiz::load_data::LoadData::ToBool4(nullptr, global, value2, _excuteData, &builder);
					if (value1 != value2) {
						int x = atoi(value1.c_str());
						int y = atoi(value2.c_str());

						std::string temp = wiz::ToString(wiz::load_data::UserType::Find(&global, dir, &builder).second[0]->GetItemList(x).Get(0));
						std::string temp2 = wiz::ToString(wiz::load_data::UserType::Find(&global, dir, &builder).second[0]->GetItemList(y).Get(0));

						wiz::load_data::LoadData::SetData(global, dir, x, temp2, "TRUE", _excuteData, &builder);
						wiz::load_data::LoadData::SetData(global, dir, y, temp, "TRUE", _excuteData, &builder);
					}

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$print" == val->GetName()) /// has many bugs..!?, for print list or print item?.
				{
					if (excuteData.noUseOutput) {
						eventStack.top().userType_idx.top()++;
						break;
					}
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					if (val->GetUserTypeListSize() == 1
						&& val->GetUserTypeList(0)->GetItemListSize() == 1)
					{
						std::string listName = wiz::ToString(val->GetUserTypeList(0)->GetItemList(0).Get(0));

						if (listName.size() >= 2 && listName[0] == '\"' && listName.back() == '\"')
						{
							listName = wiz::String::substring(listName, 1, listName.size() - 2);
							/*std::string data;
							int count = 0;
							for (int _i = 0; _i < listName.size(); ++_i) {
								data += listName[_i];
								if (listName[_i] < 0) {
									count++;
									if (count == 2) {
										data += "\0";
										count = 0;
									}
								}
							}
							std::cout << data;*/
							std::cout << listName;
						}
						else if (listName.size() == 2 && listName[0] == '\\' && listName[1] == 'n')
						{
							std::cout << '\n';
						}
						else if (wiz::String::startsWith(listName, "$local.")
							|| wiz::String::startsWith(listName, "$parameter.")
							)
						{
							std::string temp = wiz::load_data::LoadData::ToBool4(nullptr, global, listName, _excuteData, &builder);
							if (temp.empty()) {
								std::cout << "EMPTY";
							}
							else {
								std::cout << temp;
							}
						}
						else if (wiz::String::startsWith(listName, "/") && listName.size() > 1)
						{
							std::string temp = wiz::load_data::LoadData::ToBool4(nullptr, global, listName, _excuteData, &builder);
							if (temp != listName) // chk 
							{
								std::cout << temp;
							}
							else {
								wiz::load_data::UserType* ut = wiz::load_data::UserType::Find(&global, listName, &builder).second[0];
								if (ut->GetItemListSize() == 0 && wiz::ToString(ut->GetItemList(0).GetName()).empty()) {
									std::cout << wiz::ToString(ut->GetItemList(0).Get(0));
								}
							}
						}
						else
						{
							auto x = wiz::load_data::UserType::Find(&global, listName, &builder);
							if (x.first) {
								wiz::load_data::UserType* ut = x.second[0];
								std::cout << ut->ToString();
							}
							else {
								std::cout << listName;
							}
						}
					}
					// ?
					else if (val->GetUserTypeListSize() == 1
						&& val->GetUserTypeList(0)->GetItemListSize() == 0
						&& val->GetUserTypeList(0)->GetUserTypeListSize() == 1)
					{
						std::string temp = val->GetUserTypeList(0)->ToString();

						std::string name = wiz::load_data::LoadData::ToBool4(nullptr, global,
							temp, _excuteData, &builder);
						std::cout << name;
					}
					else
					{
						std::string start = val->GetUserTypeList(1)->ToString();
						std::string last = val->GetUserTypeList(2)->ToString();

						start = wiz::load_data::LoadData::ToBool4(nullptr, global, start, _excuteData, &builder);
						last = wiz::load_data::LoadData::ToBool4(nullptr, global, last, _excuteData, &builder);

						std::string listName = wiz::ToString(val->GetUserTypeList(0)->GetItemList(0).Get(0));
						int _start = atoi(start.c_str());
						int _last = atoi(last.c_str());
						wiz::load_data::UserType* ut = wiz::load_data::UserType::Find(&global, listName, &builder).second[0];
						for (int i = _start; i <= _last; ++i)
						{
							if (i != _start) { std::cout << " "; }
							std::cout << wiz::ToString(ut->GetItemList(i).Get(0));
						}
					}

					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$print2" == val->GetName()) /// for print usertype.ToString();
				{
					if (excuteData.noUseOutput) {
						eventStack.top().userType_idx.top()++;
						break;
					}
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					std::string dir = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(0)->ToString(), _excuteData, &builder);
					auto x = wiz::load_data::UserType::Find(&global, dir, &builder);

					for (int idx = 0; idx < x.second.size(); ++idx) {
						std::cout << x.second[idx]->ToString();
						if (idx < x.second.size() - 1) {
							std::cout << "\n";
						}
					}

					eventStack.top().userType_idx.top()++;
					break;
				}
				// comment copy??
				else if ("$load" == val->GetName())
				{
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					// load data and events from other file! 
					// itemlist -> usertypelist.tostring?
					for (int i = 0; i < val->GetItemListSize(); ++i) {
						wiz::load_data::UserType ut;
						std::string fileName = wiz::ToString(val->GetItemList(i).Get(0));
						fileName = wiz::String::substring(fileName, 1, fileName.size() - 2);

						if (wiz::load_data::LoadData::LoadDataFromFile(fileName, ut)) {
							{
								int item_count = 0;
								int userType_count = 0;

								for (int i = 0; i < ut.GetIListSize(); ++i) {
									if (ut.IsItemList(i)) {
										global.AddItem(std::move(ut.GetItemList(item_count).GetName()),
											std::move(ut.GetItemList(item_count).Get(0)));
										item_count++;
									}
									else {
										global.AddUserTypeItem(std::move(*ut.GetUserTypeList(userType_count)));
										userType_count++;
									}
								}
							}

							auto _Main = ut.GetUserTypeItem("Main");
							if (!_Main.empty())
							{
								// error!
								std::cout << "err" << std::endl;

								return "ERROR -2"; /// exit?
							}
						}
						else {
							// error!
						}
					}

					{
						convert.clear();
						auto _events = global.GetCopyUserTypeItem("Event");
						for (int i = 0; i < _events.size(); ++i) {
							eventPtr->LinkUserType(_events[i]);
						}
						global.RemoveUserTypeList("Event");

						// event table setting
						for (int i = 0; i < eventPtr->GetUserTypeListSize(); ++i)
						{
							auto x = eventPtr->GetUserTypeList(i)->GetItem("id");
							if (!x.empty()) {
								//std::cout <<	x[0] << std::endl;
								auto temp = std::pair<std::string, int>(wiz::ToString(x[0].Get(0)), i);
								convert.insert(temp);
							}
							else {
								// error
							}
						}

						// update no
						no = convert[str];
					}

					eventStack.top().userType_idx.top()++;
					break;

				}
				else if ("$load_only_data" == val->GetName()) // $load2?
				{
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					// to do, load data and events from other file!
					wiz::load_data::UserType ut;
					std::string fileName = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(0)->ToString(), _excuteData, &builder);
					
					if (fileName.back() == '\"' && fileName.size() >= 2 && fileName.back() == fileName[0]) {
						fileName = wiz::String::substring(fileName, 1, fileName.size() - 2);
					}
					std::string dirName = val->GetUserTypeList(1)->ToString();
					wiz::load_data::UserType* utTemp;


					if (dirName == "/./" || dirName == "root") {
						utTemp = &global;
					}
					else {
						dirName = wiz::load_data::LoadData::ToBool4(nullptr, global, dirName, ExcuteData(), &builder);
						utTemp = global.GetUserTypeItem(dirName)[0];
					}

					if (wiz::load_data::LoadData::LoadDataFromFile(fileName, ut)) {
						{
							for (int i = 0; i < ut.GetCommentListSize(); ++i) {
								utTemp->PushComment(std::move(ut.GetCommentList(i)));
							}
							int item_count = 0;
							int userType_count = 0;

							for (int i = 0; i < ut.GetIListSize(); ++i) {
								if (ut.IsItemList(i)) {
									utTemp->AddItem(std::move(ut.GetItemList(item_count).GetName()),
										std::move(ut.GetItemList(item_count).Get(0)));
									item_count++;
								}
								else {
									utTemp->AddUserTypeItem(std::move(*ut.GetUserTypeList(userType_count)));
									userType_count++;
								}
							}
						}

						//	auto _Main = ut.GetUserTypeItem("Main");
						//	if (!_Main.empty())
						//	{
						// error!
						//		std::cout << "err" << std::endl;

						//			return "ERROR -2"; /// exit?
						//		}
					}
					else {
						// error!
					}

					eventStack.top().userType_idx.top()++;
					break;

				}
				else if ("$load_only_data2" == val->GetName()) // $load2?
				{
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					// to do, load data and events from other file!
					wiz::load_data::UserType ut;
					std::string fileName = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(0)->ToString(), _excuteData, &builder);
					if (fileName.back() == '\"' && fileName.size() >= 2 && fileName.back() == fileName[0]) {
						fileName = wiz::String::substring(fileName, 1, fileName.size() - 2);
					}
					std::string dirName = val->GetUserTypeList(1)->ToString();
					wiz::load_data::UserType* utTemp;


					if (dirName == "/./" || dirName == "root") {
						utTemp = &global;
					}
					else {
						dirName = wiz::load_data::LoadData::ToBool4(nullptr, global, dirName, ExcuteData(), &builder);
						utTemp = global.GetUserTypeItem(dirName)[0];
					}


					int lex_thr_num = 0;
					int parse_thr_num = 0;

					if (val->GetUserTypeListSize() >= 3) {
						lex_thr_num = stoi(val->GetUserTypeList(2)->ToString());
					}
					if (val->GetUserTypeListSize() >= 4) {
						parse_thr_num = stoi(val->GetUserTypeList(3)->ToString());
					}


					if (wiz::load_data::LoadData::LoadDataFromFile3(fileName, ut, parse_thr_num - 1, lex_thr_num)) {
						{
						
							for (int i = 0; i < ut.GetCommentListSize(); ++i) {
								utTemp->PushComment(std::move(ut.GetCommentList(i)));
							}
							int item_count = 0;
							int userType_count = 0;

							for (int i = 0; i < ut.GetIListSize(); ++i) {
								if (ut.IsItemList(i)) {
									utTemp->AddItem(std::move(ut.GetItemList(item_count).GetName()),
										std::move(ut.GetItemList(item_count).Get(0)));
									item_count++;
								}
								else {
									utTemp->AddUserTypeItem(std::move(*ut.GetUserTypeList(userType_count)));
									userType_count++;
								}
							}
							
						}
						
						//	auto _Main = ut.GetUserTypeItem("Main");
						//	if (!_Main.empty())
						//	{
						// error!
						//		std::cout << "err" << std::endl;

						//			return "ERROR -2"; /// exit?
						//		}
					}
					else {
						// error!
					}

					eventStack.top().userType_idx.top()++;
					break;

				}
				else if ("$load_only_data3" == val->GetName()) // $load2?
				{
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					// to do, load data and events from other file!
					wiz::load_data::UserType ut;
					std::string fileName = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(0)->ToString(), _excuteData, &builder);
					if (fileName.back() == '\"' && fileName.size() >= 2 && fileName.back() == fileName[0]) {
						fileName = wiz::String::substring(fileName, 1, fileName.size() - 2);
					}
					std::string dirName = val->GetUserTypeList(1)->ToString();
					wiz::load_data::UserType* utTemp;


					if (dirName == "/./" || dirName == "root") {
						utTemp = &global;
					}
					else {
						dirName = wiz::load_data::LoadData::ToBool4(nullptr, global, dirName, ExcuteData(), &builder);
						utTemp = global.GetUserTypeItem(dirName)[0];
					}


					int lex_thr_num = 0;
					int parse_thr_num = 0;

					if (val->GetUserTypeListSize() >= 3) {
						lex_thr_num = stoi(val->GetUserTypeList(2)->ToString());
					}
					if (val->GetUserTypeListSize() >= 4) {
						parse_thr_num = stoi(val->GetUserTypeList(3)->ToString());
					}


					if (wiz::load_data::LoadData::LoadDataFromFile4(fileName, ut, parse_thr_num - 1, lex_thr_num)) {
						{

							for (int i = 0; i < ut.GetCommentListSize(); ++i) {
								utTemp->PushComment(std::move(ut.GetCommentList(i)));
							}
							int item_count = 0;
							int userType_count = 0;

							for (int i = 0; i < ut.GetIListSize(); ++i) {
								if (ut.IsItemList(i)) {
									utTemp->AddItem(std::move(ut.GetItemList(item_count).GetName()),
										std::move(ut.GetItemList(item_count).Get(0)));
									item_count++;
								}
								else {
									utTemp->AddUserTypeItem(std::move(*ut.GetUserTypeList(userType_count)));
									userType_count++;
								}
							}

						}

						//	auto _Main = ut.GetUserTypeItem("Main");
						//	if (!_Main.empty())
						//	{
						// error!
						//		std::cout << "err" << std::endl;

						//			return "ERROR -2"; /// exit?
						//		}
					}
					else {
						// error!
					}

					eventStack.top().userType_idx.top()++;
					break;

				}
				else if ("$load_only_data4" == val->GetName()) // $load2?
				{
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					// to do, load data and events from other file!
					wiz::load_data::UserType ut;
					std::string fileName = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(0)->ToString(), _excuteData, &builder);
					if (fileName.back() == '\"' && fileName.size() >= 2 && fileName.back() == fileName[0]) {
						fileName = wiz::String::substring(fileName, 1, fileName.size() - 2);
					}
					std::string dirName = val->GetUserTypeList(1)->ToString();
					wiz::load_data::UserType* utTemp;


					if (dirName == "/./" || dirName == "root") {
						utTemp = &global;
					}
					else {
						dirName = wiz::load_data::LoadData::ToBool4(nullptr, global, dirName, ExcuteData(), &builder);
						utTemp = global.GetUserTypeItem(dirName)[0];
					}


					int lex_thr_num = 0;
					int parse_thr_num = 0;

					if (val->GetUserTypeListSize() >= 3) {
						lex_thr_num = stoi(val->GetUserTypeList(2)->ToString());
					}
					if (val->GetUserTypeListSize() >= 4) {
						parse_thr_num = stoi(val->GetUserTypeList(3)->ToString());
					}

					int a = clock();
					if (wiz::load_data::LoadData::LoadDataFromFile5_2(fileName, ut, lex_thr_num, parse_thr_num)) {
						{
							int b = clock();
							std::cout << b - a << "ms" << "\n";

							for (int i = 0; i < ut.GetCommentListSize(); ++i) {
								utTemp->PushComment(std::move(ut.GetCommentList(i)));
							}
							int item_count = 0;
							int userType_count = 0;

							for (int i = 0; i < ut.GetIListSize(); ++i) {
								if (ut.IsItemList(i)) {
									utTemp->AddItem(std::move(ut.GetItemList(item_count).GetName()),
										std::move(ut.GetItemList(item_count).Get(0)));
									item_count++;
								}
								else {
									utTemp->AddUserTypeItem(std::move(*ut.GetUserTypeList(userType_count)));
									userType_count++;
								}
							}

						}

						//	auto _Main = ut.GetUserTypeItem("Main");
						//	if (!_Main.empty())
						//	{
						// error!
						//		std::cout << "err" << std::endl;

						//			return "ERROR -2"; /// exit?
						//		}
					}
					else {
						// error!
					}

					eventStack.top().userType_idx.top()++;
					break;

				}

#ifdef USE_FAST_LOAD_DATA

				else if ("$fast_load" == val->GetName())
				{
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					// to do, load data and events from other file!
					for (int i = 0; i < val->GetItemListSize(); ++i) {
						wiz::load_data::UserType ut;
						std::string fileName = val->GetItemList(i).Get(0);
						fileName = wiz::String::substring(fileName, 1, fileName.size() - 2);

						if (wiz::load_data::LoadData::FastLoadDataFromFile(fileName, ut)) {
							{
								int item_count = 0;
								int userType_count = 0;

								for (int i = 0; i < ut.GetIListSize(); ++i) {
									if (ut.IsItemList(i)) {
										global.AddItem(std::move(ut.GetItemList(item_count).GetName()),
											std::move(ut.GetItemList(item_count).Get(0)));
										item_count++;
									}
									else {
										global.AddUserTypeItem(std::move(*ut.GetUserTypeList(userType_count)));
										userType_count++;
									}
								}
							}

							auto _Main = ut.GetUserTypeItem("Main");
							if (!_Main.empty())
							{
								// error!
								std::cout << "err" << std::endl;

								return "ERROR -2"; /// exit?
							}
						}
						else {
							// error!
						}
					}

					{
						convert.clear();
						auto _events = global.GetCopyUserTypeItem("Event");
						for (int i = 0; i < _events.size(); ++i) {
							eventPtr->LinkUserType(_events[i]);
						}
						global.RemoveUserTypeList("Event");

						// event table setting
						for (int i = 0; i < eventPtr->GetUserTypeListSize(); ++i)
						{
							auto x = eventPtr->GetUserTypeList(i)->GetItem("id");
							if (!x.empty()) {
								//std::cout <<	x[0] << std::endl;
								convert.insert(std::pair<std::string, int>(x[0].Get(0), i));
							}
							else {
								// error
							}
						}

						// update no
						no = convert[str];
					}

					eventStack.top().userType_idx.top()++;
					break;

				}
				else if ("$fast_load_only_data" == val->GetName()) // $load2?
				{
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					// to do, load data and events from other file!
					wiz::load_data::UserType ut;
					std::string fileName = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(0)->ToString(), _excuteData, &builder);
					fileName = wiz::String::substring(fileName, 1, fileName.size() - 2);
					std::string dirName = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(1)->ToString(), _excuteData, &builder);
					wiz::load_data::UserType* utTemp = global.GetUserTypeItem(dirName)[0];



					if (wiz::load_data::LoadData::FastLoadDataFromFile(fileName, ut)) {
						{
							for (int i = 0; i < ut.GetCommentListSize(); ++i) {
								utTemp->PushComment(std::move(ut.GetCommentList(i)));
							}
							int item_count = 0;
							int userType_count = 0;

							for (int i = 0; i < ut.GetIListSize(); ++i) {
								if (ut.IsItemList(i)) {
									utTemp->AddItem(std::move(ut.GetItemList(item_count).GetName()),
										std::move(ut.GetItemList(item_count).Get(0)));
									item_count++;
								}
								else {
									utTemp->AddUserTypeItem(std::move(*ut.GetUserTypeList(userType_count)));
									userType_count++;
								}
							}
						}

						//	auto _Main = ut.GetUserTypeItem("Main");
						//	if (!_Main.empty())
						//	{
						// error!
						//		std::cout << "err" << std::endl;

						//			return "ERROR -2"; /// exit?
						//		}
					}
					else {
						// error!
					}

					eventStack.top().userType_idx.top()++;
					break;

				}
#endif
				else if ("$load_json" == val->GetName()) // $load2?
				{
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					// to do, load data and events from other file!
					wiz::load_data::UserType ut;
					std::string fileName = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(0)->ToString(), _excuteData, &builder);
					fileName = wiz::String::substring(fileName, 1, fileName.size() - 2);
					std::string dirName = val->GetUserTypeList(1)->ToString();
					wiz::load_data::UserType* utTemp = global.GetUserTypeItem(dirName)[0];


					if (dirName == "/./" || dirName == "root") {
						utTemp = &global;
					}
					else {
						dirName = wiz::load_data::LoadData::ToBool4(nullptr, global, dirName, ExcuteData(), &builder);
						utTemp = global.GetUserTypeItem(dirName)[0];
					}


					if (wiz::load_data::LoadData::LoadDataFromFileWithJson(fileName, ut)) {
						{
							//for (int i = 0; i < ut.GetCommentListSize(); ++i) {
							//	utTemp->PushComment(std::move(ut.GetCommentList(i)));
							//}

							wiz::load_data::UserType* _ut = ut.GetUserTypeList(0);

							int item_count = 0;
							int userType_count = 0;

							for (int i = 0; i < _ut->GetIListSize(); ++i) {
								if (_ut->IsItemList(i)) {
									utTemp->AddItem(std::move(_ut->GetItemList(item_count).GetName()),
										std::move(_ut->GetItemList(item_count).Get(0)));
									item_count++;
								}
								else {
									utTemp->AddUserTypeItem(std::move(*_ut->GetUserTypeList(userType_count)));
									userType_count++;
								}
							}
						}

						//	auto _Main = ut.GetUserTypeItem("Main");
						//	if (!_Main.empty())
						//	{
						// error!
						//		std::cout << "err" << std::endl;

						//			return "ERROR -2"; /// exit?
						//		}
					}
					else {
						// error!
					}

					eventStack.top().userType_idx.top()++;
					break;

				}
				else if ("$load_json2" == val->GetName()) // 
				{
				ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
				_excuteData.chkInfo = true;
				_excuteData.info = eventStack.top();
				_excuteData.pObjectMap = objectMapPtr;
				_excuteData.pEvents = eventPtr;
				_excuteData.pModule = moduleMapPtr;

				// to do, load data and events from other file!
				wiz::load_data::UserType ut;
				std::string fileName = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(0)->ToString(), _excuteData, &builder);
				fileName = wiz::String::substring(fileName, 1, fileName.size() - 2);
				std::string dirName = val->GetUserTypeList(1)->ToString();
				wiz::load_data::UserType* utTemp = global.GetUserTypeItem(dirName)[0];


				if (dirName == "/./" || dirName == "root") {
					utTemp = &global;
				}
				else {
					dirName = wiz::load_data::LoadData::ToBool4(nullptr, global, dirName, ExcuteData(), &builder);
					utTemp = global.GetUserTypeItem(dirName)[0];
				}


				if (wiz::load_data::LoadData::LoadDataFromFileFastJson(fileName, ut, 0, 0)) {
					{
						//for (int i = 0; i < ut.GetCommentListSize(); ++i) {
						//	utTemp->PushComment(std::move(ut.GetCommentList(i)));
						//}

						wiz::load_data::UserType* _ut = &ut;

						int item_count = 0;
						int userType_count = 0;

						for (int i = 0; i < _ut->GetIListSize(); ++i) {
							if (_ut->IsItemList(i)) {
								utTemp->AddItem(std::move(_ut->GetItemList(item_count).GetName()),
									std::move(_ut->GetItemList(item_count).Get(0)));
								item_count++;
							}
							else {
								utTemp->AddUserTypeItem(std::move(*_ut->GetUserTypeList(userType_count)));
								userType_count++;
							}
						}
					}

					//	auto _Main = ut.GetUserTypeItem("Main");
					//	if (!_Main.empty())
					//	{
					// error!
					//		std::cout << "err" << std::endl;

					//			return "ERROR -2"; /// exit?
					//		}
				}
				else {
					// error!
				}

				eventStack.top().userType_idx.top()++;
				break;

				}
				else if ("$load_html" == val->GetName()) // $load2?
				{
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					// to do, load data and events from other file!
					wiz::load_data::UserType ut;
					std::string fileName = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(0)->ToString(), _excuteData, &builder);
					fileName = wiz::String::substring(fileName, 1, fileName.size() - 2);
					std::string dirName = val->GetUserTypeList(1)->ToString();
					wiz::load_data::UserType* utTemp;


					if (dirName == "/./" || dirName == "root") {
						utTemp = &global;
					}
					else {
						dirName = wiz::load_data::LoadData::ToBool4(nullptr, global, dirName, ExcuteData(), &builder);
						utTemp = global.GetUserTypeItem(dirName)[0];
					}


					if (wiz::load_data::LoadData::LoadDataFromFileWithHtml(fileName, ut)) {
						{
							for (int i = 0; i < ut.GetCommentListSize(); ++i) {
								utTemp->PushComment(std::move(ut.GetCommentList(i)));
							}
							int item_count = 0;
							int userType_count = 0;

							for (int i = 0; i < ut.GetIListSize(); ++i) {
								if (ut.IsItemList(i)) {
									utTemp->AddItem(std::move(ut.GetItemList(item_count).GetName()),
										std::move(ut.GetItemList(item_count).Get(0)));
									item_count++;
								}
								else {
									utTemp->AddUserTypeItem(std::move(*ut.GetUserTypeList(userType_count)));
									userType_count++;
								}
							}
						}

						//	auto _Main = ut.GetUserTypeItem("Main");
						//	if (!_Main.empty())
						//	{
						// error!
						//		std::cout << "err" << std::endl;

						//			return "ERROR -2"; /// exit?
						//		}
					}
					else {
						// error!
					}

					eventStack.top().userType_idx.top()++;
					break;

				}
				else if ("$clear_screen" == val->GetName())
				{
					system("cls");
					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$_getch" == val->GetName())
				{
					if (excuteData.noUseInput) {
						eventStack.top().userType_idx.top()++;
						break;
					}

					GETCH();
					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$input" == val->GetName())
				{
					if (excuteData.noUseInput) {
						eventStack.top().userType_idx.top()++;
						break;
					}
					std::string str;
					std::cin >> str;
					eventStack.top().return_value = str;
					eventStack.top().userType_idx.top()++;
					break;
				}
				// line
				else if ("$input2" == val->GetName()) {
					if (excuteData.noUseInput) { // when no use input?
						eventStack.top().userType_idx.top()++;
						break;
					}
					std::string str;
					std::getline(std::cin, str);
					eventStack.top().return_value = str;
					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$return" == val->GetName())
				{
					//// can $return = { a b c }
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					eventStack.top().userType_idx.top()++;
					if (eventStack.size() > 1)
					{
						std::string temp = wiz::load_data::LoadData::ToBool4(nullptr, global, val->ToString(), _excuteData, &builder);
						/// if temp just one
						eventStack[eventStack.size() - 2].return_value = temp;
					}

					if (eventStack.size() == 1)
					{
						std::string temp = wiz::load_data::LoadData::ToBool4(nullptr, global, val->ToString(), _excuteData, &builder);

						module_value = temp;
					}

					eventStack.pop();
					break;
				}
				
				else if ("$return_data" == val->GetName()) { // for functional programming??
					eventStack.top().userType_idx.top()++;

					if (eventStack.size() > 1)
					{
						std::string temp = val->ToString();
						/// if temp just one
						eventStack[eventStack.size() - 2].return_value = temp;
					}

					if (eventStack.size() == 1)
					{
						std::string temp = val->ToString();

						module_value = temp;
					}

					eventStack.top().userType_idx.top()++;
					break;
				}
				
				else if ("$parameter" == val->GetName())
				{
					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$local" == val->GetName())
				{
					eventStack.top().userType_idx.top()++;
					break;
				}
				// make sort stable.
				else if ("$sort" == val->GetName()) {
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					std::vector<SortInfo> siVec;
					wiz::load_data::UserType* utTemp =
						wiz::load_data::UserType::Find(&global,
							wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(0)->ToString(), _excuteData, &builder), &builder).second[0];

					std::vector<wiz::load_data::Type*> temp;


					int item_count = 0, ut_count = 0;
					for (int i = 0; i < utTemp->GetIListSize(); ++i) {
						if (utTemp->IsItemList(i)) {
							temp.push_back(&(utTemp->GetItemList(item_count)));
							siVec.emplace_back(wiz::ToString(utTemp->GetItemList(item_count).GetName()), 1, i, &builder);
							item_count++;
						}
						else {
							temp.push_back(utTemp->GetUserTypeList(ut_count));
							siVec.emplace_back(wiz::ToString(utTemp->GetUserTypeList(ut_count)->GetName()), 2, i, &builder);
							ut_count++;
						}
					}

					std::sort(siVec.begin(), siVec.end());


					wiz::load_data::UserType ut;
					for (int i = 0; i < temp.size(); ++i)
					{
						if (siVec[i].iElement == 1) {
							ut.AddItem(siVec[i].data, static_cast<wiz::load_data::ItemType<WIZ_STRING_TYPE>*>(temp[siVec[i].idx])->Get(0));
						}
						else {
							ut.AddUserTypeItem(*(static_cast<wiz::load_data::UserType*>(temp[siVec[i].idx])));
						}
					}

					utTemp->Remove();

					//cf) chk? *utTemp = ut;
					wiz::load_data::LoadData::AddData(*(utTemp), "", ut.ToString(), "TRUE", _excuteData, &builder);


					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$sort2" == val->GetName()) { // colName -> just one! ? 
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					/// condition = has just one? in one usertype!
					std::vector<SortInfo> siVec;
					wiz::load_data::UserType* utTemp =
						wiz::load_data::UserType::Find(&global,
							wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(0)->ToString(), _excuteData, &builder), &builder).second[0];
					const std::string colName = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(1)->ToString(), _excuteData, &builder);

					std::vector<wiz::load_data::Type*> temp;


					int item_count = 0, ut_count = 0;
					for (int i = 0; i < utTemp->GetIListSize(); ++i) {
						if (utTemp->IsItemList(i)) {
							//
							item_count++;
						}
						else {
							temp.push_back(utTemp->GetUserTypeList(ut_count));
							if (utTemp->GetUserTypeList(ut_count)->GetItem(colName).empty())
							{
								siVec.emplace_back("", 2, ut_count, &builder);
							}
							else {
								siVec.emplace_back(wiz::ToString(utTemp->GetUserTypeList(ut_count)->GetItem(colName)[0].Get(0)), 2, ut_count, &builder);
							}
							ut_count++;
						}
					}

					std::sort(siVec.begin(), siVec.end());


					wiz::load_data::UserType ut;
					for (int i = 0; i < temp.size(); ++i)
					{
						if (siVec[i].iElement == 1) {
							//
						}
						else {
							ut.AddUserTypeItem(*(static_cast<wiz::load_data::UserType*>(temp[siVec[i].idx])));
						}
					}

					utTemp->RemoveUserTypeList();

					//cf) chk? *utTemp = ut;
					wiz::load_data::LoadData::AddData(*(utTemp), "", ut.ToString(), "TRUE", _excuteData, &builder);


					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$sort2_dsc" == val->GetName()) { // colName -> just one! ? 
														   /// condition = has just one? in one usertype!
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					std::vector<SortInfo2> siVec;
					wiz::load_data::UserType* utTemp =
						wiz::load_data::UserType::Find(&global,
							wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(0)->ToString(), _excuteData, &builder), &builder).second[0];
					const std::string colName = wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(1)->ToString(), _excuteData, &builder);
					// $it?
					std::vector<wiz::load_data::Type*> temp;


					int item_count = 0, ut_count = 0;
					for (int i = 0; i < utTemp->GetIListSize(); ++i) {
						if (utTemp->IsItemList(i)) {
							//
							item_count++;
						}
						else {
							temp.push_back(utTemp->GetUserTypeList(ut_count));
							if (utTemp->GetUserTypeList(ut_count)->GetItem(colName).empty())
							{
								siVec.emplace_back("", 2, ut_count, &builder);
							}
							else {
								siVec.emplace_back(wiz::ToString(utTemp->GetUserTypeList(ut_count)->GetItem(colName)[0].Get(0)), 2, ut_count, &builder);
							}
							ut_count++;
						}
					}

					std::sort(siVec.begin(), siVec.end());


					wiz::load_data::UserType ut;
					for (int i = 0; i < temp.size(); ++i)
					{
						if (siVec[i].iElement == 1) {
							//
						}
						else {
							ut.AddUserTypeItem(*(static_cast<wiz::load_data::UserType*>(temp[siVec[i].idx])));
						}
					}

					utTemp->RemoveUserTypeList();

					//cf) chk? *utTemp = ut;
					wiz::load_data::LoadData::AddData(*(utTemp), "", ut.ToString(), "TRUE", _excuteData, &builder);


					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$shell_mode" == val->GetName()) {
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					// check?
					if (excuteData.noUseInput || excuteData.noUseOutput) {
						eventStack.top().userType_idx.top()++;
						break;
					}
					
					ShellMode(global);

					eventStack.top().userType_idx.top()++;
					break;
				}
				// removal?
				else if ("$stable_sort" == val->GetName()) {
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					// todo
					// todo
					std::vector<SortInfo> siVec;
					wiz::load_data::UserType* utTemp =
						wiz::load_data::UserType::Find(&global,
							wiz::load_data::LoadData::ToBool4(nullptr, global, val->GetUserTypeList(0)->ToString(), _excuteData, &builder), &builder).second[0];

					std::vector<wiz::load_data::Type*> temp;


					int item_count = 0, ut_count = 0;
					for (int i = 0; i < utTemp->GetIListSize(); ++i) {
						if (utTemp->IsItemList(i)) {
							temp.push_back(&(utTemp->GetItemList(item_count)));
							siVec.emplace_back(wiz::ToString(utTemp->GetItemList(item_count).GetName()), 1, i, &builder);
							item_count++;
						}
						else {
							temp.push_back(utTemp->GetUserTypeList(ut_count));
							siVec.emplace_back(wiz::ToString(utTemp->GetUserTypeList(ut_count)->GetName()), 2, i, &builder);
							ut_count++;
						}
					}

					std::stable_sort(siVec.begin(), siVec.end());


					wiz::load_data::UserType ut;
					for (int i = 0; i < temp.size(); ++i)
					{
						if (siVec[i].iElement == 1) {
							ut.AddItem(siVec[i].data, static_cast<wiz::load_data::ItemType<std::string>*>(temp[siVec[i].idx])->Get(0));
						}
						else {
							ut.AddUserTypeItem(*(static_cast<wiz::load_data::UserType*>(temp[siVec[i].idx])));
						}
					}

					utTemp->Remove();

					wiz::load_data::LoadData::AddData(*(utTemp), "", ut.ToString(), "TRUE", _excuteData, &builder);


					eventStack.top().userType_idx.top()++;
					break;
				}
				else if ("$if" == val->GetName()) // ToDo!!
				{
					ExcuteData _excuteData; _excuteData.depth = excuteData.depth;
					_excuteData.chkInfo = true;
					_excuteData.info = eventStack.top();
					_excuteData.pObjectMap = objectMapPtr;
					_excuteData.pEvents = eventPtr;
					_excuteData.pModule = moduleMapPtr;

					std::string temp = val->GetUserTypeList(0)->ToString();

					/*
					std::pair<std::vector<std::string>, bool> x;
					if (_map.end() == _map.find(temp)) {
					x = wiz::load_data::LoadData::ToBool4_A(nullptr, global, temp, _excuteData, &builder);
					_map.insert(std::make_pair(temp, x));
					}
					else {
					x = _map[temp];
					}
					*/
					temp = wiz::load_data::LoadData::ToBool4(nullptr, global, temp, _excuteData, &builder);


					if (!eventStack.top().conditionStack.empty())
					{
						if ("TRUE" == temp && eventStack.top().conditionStack.top() == "FALSE")
						{
							temp = "FALSE";
						}
						else if ("FALSE" == temp && eventStack.top().conditionStack.top() == "FALSE")
						{
							temp = "FALSE";
						}
						else if (!eventStack.top().nowUT.empty() && eventStack.top().userType_idx.top() + 1 < eventStack.top().nowUT.top()->GetUserTypeListSize()
							&& (eventStack.top().nowUT.top()->GetUserTypeList(eventStack.top().userType_idx.top() + 1)->GetName() == "$else"))
						{
							eventStack.top().conditionStack.push(temp);
						}
						else if ("TRUE" == temp)
						{
							eventStack.top().conditionStack.push(temp);
						}
					}
					else
					{
						if (eventStack.top().userType_idx.top() + 1 < eventPtr->GetUserTypeList(no)->GetUserTypeListSize() &&
							eventPtr->GetUserTypeList(no)->GetUserTypeList(eventStack.top().userType_idx.top() + 1)->GetName() == "$else")
						{
							eventStack.top().conditionStack.push(temp);
						}
						else if ("TRUE" == temp)
						{
							eventStack.top().conditionStack.push(temp);
						}
					}

					if ("TRUE" == temp)
					{
						eventStack.top().nowUT.push(val->GetUserTypeList(1));
						//val = eventStack.top().nowUT.top()->GetUserTypeList(0); 
						eventStack.top().userType_idx.top()++;
						eventStack.top().userType_idx.push(0);
						//eventStack.top().state.push(1);
						//state = 1;
						break;
					}//
					else if ("FALSE" == temp)
					{
						eventStack.top().userType_idx.top()++;
						break;
					}
					else
					{
						// debug..
						std::cout << "Error Debug : " << temp << std::endl;
						return "ERROR -3";
					}
				}
				else if ("$else" == val->GetName())
				{
					if (!eventStack.top().conditionStack.empty() && "FALSE" == eventStack.top().conditionStack.top())
					{
						eventStack.top().conditionStack.top() = "TRUE";
						eventStack.top().nowUT.push(val->GetUserTypeList(0));
						//val = eventStack.top().nowUT.top()->GetUserTypeList(0); // empty chk
						eventStack.top().userType_idx.top()++;
						eventStack.top().userType_idx.push(0);
						//eventStack.top().state.push(2);
						//state = 2; //
						break;
					}
					else
					{
						eventStack.top().userType_idx.top()++;
						break;
					}
				}
				else { //
					std::cout << "it does not work. : " << wiz::ToString(val->GetName()) << std::endl;

					eventStack.top().userType_idx.top()++;
					break;
				}
			}
		}
	}
	/*
	for (int i = 0; i < waits.size(); ++i) {
	waits[i]->join();
	delete waits[i];
	}
	waits.resize(0);
	*/

	if (1 == chk && !events.empty()) {
		auto _events = events.GetCopyUserTypeItem("Event");
		for (int i = 0; i < _events.size(); ++i) {
			_global->LinkUserType(_events[i]);
		}
	}
	return module_value;
}

// rename?
bool IsEmpty(std::vector<int>& chk_brace, const std::string& str)
{
	for (int i = 0; i < str.size(); ++i) {
		if ('{' == str[i]) {
			chk_brace.push_back(0);
		}
		else if ('}' == str[i]) {
			if (chk_brace.empty()) {
				chk_brace.push_back(1);
				return false;
			}
			else {
				chk_brace.pop_back();
			}
		}
	}

	return chk_brace.empty();
}
void SaveWithOutEvent(std::ostream& stream, wiz::load_data::UserType* ut, int depth)
{
	int itemListCount = 0;
	int userTypeListCount = 0;

	for (int i = 0; i < ut->GetCommentListSize(); ++i) {
		for (int k = 0; k < depth; ++k) {
			stream << "\t";
		}
		stream << (ut->GetCommentList(i));

		if (i < ut->GetCommentListSize() - 1 || 0 == ut->GetIListSize()) {
			stream << "\n";
		}
	}

	for (int i = 0; i < ut->GetIListSize(); ++i) {
		//std::cout << "ItemList" << endl;
		if (ut->IsItemList(i)) {
			for (int j = 0; j < ut->GetItemList(itemListCount).size(); j++) {
				std::string temp;
				for (int k = 0; k < depth; ++k) {
					temp += "\t";
				}
				if (ut->GetItemList(itemListCount).GetName() != "") {
					temp += wiz::ToString(ut->GetItemList(itemListCount).GetName());
					temp += "=";
				}
				temp += wiz::ToString(ut->GetItemList(itemListCount).Get(j));
				if (j != ut->GetItemList(itemListCount).size() - 1) {
					temp += "\n";
				}
				stream << temp;
			}
			if (i != ut->GetIListSize() - 1) {
				stream << "\n";
			}
			itemListCount++;
		}
		else if (ut->IsUserTypeList(i)) {
			if (ut->GetUserTypeList(userTypeListCount)->GetName() == "Event"
				|| ut->GetUserTypeList(userTypeListCount)->GetName() == "Main") {
				userTypeListCount++;
				continue;
			}

			// std::cout << "UserTypeList" << endl;
			for (int k = 0; k < depth; ++k) {
				stream << "\t";
			}

			if (ut->GetUserTypeList(userTypeListCount)->GetName() != "") {
				stream << wiz::ToString(ut->GetUserTypeList(userTypeListCount)->GetName()) << "=";
			}

			stream << "{\n";

			SaveWithOutEvent(stream, ut->GetUserTypeList(userTypeListCount), depth + 1);
			stream << "\n";

			for (int k = 0; k < depth; ++k) {
				stream << "\t";
			}
			stream << "}";
			if (i != ut->GetIListSize() - 1) {
				stream << "\n";
			}

			userTypeListCount++;
		}
	}
}

// check $~ ??
void ClauText::ShellMode(wiz::load_data::UserType& global) {
	std::vector<int> chk_brace;
	std::string command;
	std::string totalCommand;

	while (true)
	{
		std::cout << "<< : "; //
		std::getline(std::cin, command);

		if (command.empty()) {
			continue;
		}

		// Evnent = {
		//		$call = { id = 0 } # tab or 들여쓰기!!
		if (!command.empty() && '$' == command[0]) {
			if ("$print" == command) {
				std::cout << ">> : global" << std::endl;
				//cout << global.ToString() << endl;
				global.Save1(std::cout);
				std::cout << std::endl;
			}
			else if ("$print_data_only" == command) {
				std::cout << ">> : global" << std::endl;
				SaveWithOutEvent(std::cout, &global, 0);
				std::cout << std::endl;
			}
			else if ("$exit" == command) {
				break;
			}
			else if ("$M" == command) {
#ifdef _MSC_VER
				MStyleTest(&global);  // for windows!
				system("cls"); // for windows!

				gotoxy(0, 0);
				setcolor(7, 0);
#endif
				std::cout << ">> : $M end" << std::endl;
			}
			else if (wiz::String::startsWith(command, "$call"))
			{
				wiz::load_data::UserType test;

				try {
					if (wiz::load_data::LoadData::LoadDataFromString(command, test))
					{
						try {
							wiz::load_data::UserType ut = global;
							const std::string id = wiz::ToString(test.GetItemList(0).Get(0));
							Option opt;
							const std::string result = excute_module("Main = { $call = { id = " + id + "} }", &ut, ExcuteData(), opt, 1);

							global = std::move(ut);
							std::cout << std::endl << "excute result is : " << result << std::endl;
						}
						catch (...) // any exception..
						{
							std::cout << ">> : $call id or excute module error" << std::endl;
						}
					}
					else {
						std::cout << ">> : $call Error" << std::endl;
					}
				}
				catch (...) {
					std::cout << ">> : $call load data from string error" << std::endl;
				}
			}
			else if (wiz::String::startsWith(command, "$load"))
			{
				wiz::load_data::UserType test;

				if (wiz::load_data::LoadData::LoadDataFromString(command, test))
				{
					try {
						const std::string name = wiz::ToString(test.GetItemList(0).Get(0));
						const std::string result = wiz::String::substring(name, 1, name.size() - 2);

						if (wiz::load_data::LoadData::LoadDataFromFile(result, global)) {}
						else {
							std::cout << ">> : load data from file error" << std::endl;
						}
					}
					catch (...) // any exception..
					{
						std::cout << ">> : load error" << std::endl;
					}
				}
				else {
					std::cout << ">> : $load syntax Error" << std::endl;
				}
			}
			else if (wiz::String::startsWith(command, "$save_event_only"))
			{
				wiz::load_data::UserType test;

				if (wiz::load_data::LoadData::LoadDataFromString(command, test))
				{
					std::ofstream outFile;

					try {
						const std::string name = wiz::ToString(test.GetItemList(0).Get(0));
						const std::string result = wiz::String::substring(name, 1, name.size() - 2);

						outFile.open(result);
						if (outFile.fail()) {
							//
						}
						else {
							for (int i = 0; i < global.GetUserTypeListSize(); ++i) {
								if (global.GetUserTypeList(i)->GetName() == "Event") {
									outFile << "Event = {\n";
									global.GetUserTypeList(i)->Save1(outFile, 1);
									outFile << "\n}\n";
								}
							}
							outFile.close();
						}
					}
					catch (...) // any exception..
					{
						if (outFile.is_open()) {
							outFile.close();
						}
						std::cout << ">> : $save_event_only error" << std::endl;
					}
				}
				else {
					std::cout << ">> : $save_event_only syntax Error" << std::endl;
				}
			}

			else if (wiz::String::startsWith(command, "$save_data_only"))
			{
				wiz::load_data::UserType test;

				if (wiz::load_data::LoadData::LoadDataFromString(command, test))
				{
					std::ofstream outFile;

					try {
						const std::string name = wiz::ToString(test.GetItemList(0).Get(0));
						const std::string result = wiz::String::substring(name, 1, name.size() - 2);

						outFile.open(result);
						if (outFile.fail()) {
							//
						}
						else {
							SaveWithOutEvent(outFile, &global, 0);
							outFile.close();
						}
					}
					catch (...) // any exception..
					{
						if (outFile.is_open()) {
							outFile.close();
						}
						std::cout << ">> : $save_data_only error" << std::endl;
					}
				}
			}
			else if (wiz::String::startsWith(command, "$save"))
			{
				wiz::load_data::UserType test;
				if (wiz::load_data::LoadData::LoadDataFromString(command, test))
				{
					const std::string name = wiz::ToString(test.GetItemList(0).Get(0));
					const std::string result = wiz::String::substring(name, 1, name.size() - 2);

					if (wiz::load_data::LoadData::SaveWizDB(global, result, "1")) {

					}
					else {
						std::cout << ">> : $save error" << std::endl;
					}
				}
				else {
					std::cout << ">> : $save syntax Error" << std::endl;
				}
			}
			else if ("$cls" == command) {
				system("cls"); // for windows!
			}
		}
		else {
			if (IsEmpty(chk_brace, command)) {
				command.append("\n");

				totalCommand.append(command);
				if (wiz::load_data::LoadData::LoadDataFromString(totalCommand, global)) {
					std::cout << ">> : Data Added!" << std::endl;
				}
				else {
					std::cout << ">> : Error : loaddata from string " << std::endl;
				}
				command = "";
				totalCommand = "";
			}
			else {
				if (chk_brace[0] == 1) {
					std::cout << ">> : Error in command, reset command" << std::endl;
					totalCommand = "";
					command = "";
					chk_brace.clear();
				}
				else {
					command.append("\n");

					totalCommand.append(command);
					command = "";
				}
			}
		}
	}
}

}

