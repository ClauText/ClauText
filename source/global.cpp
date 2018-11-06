
#define _CRT_SECURE_NO_WARNINGS

#include "wiz/global.h"
#include "wiz/load_data_utility.h"

namespace wiz {
	inline std::string Remove(const std::string& str)
	{
		if (str.size() >= 2 && str.front() == str.back() && str.back() == '\"') {
			return str.substr(1, str.size() - 2);
		}
		return str;
	}

	long long checkDelimiter(const char* start, const char* last, const std::vector<std::string>& delimiter)
	{
		const int delim_size = delimiter.size();

		for (int delim_num = 0; delim_num < delim_size; ++delim_num) {
			bool pass = true;

			// size check
			if (start + delimiter[delim_num].size() - 1 > last) {
				continue;
			}

			for (const char* x = start; x <= start + delimiter[delim_num].size() - 1; ++x) {
				if (*x == delimiter[delim_num][x - start]) {

				}
				else {
					pass = false;
					break;
				}
			}

			if (pass) {
				return delim_num;
			}
		}

		return -1;
	}

	int DataType::GetType()const {
		if (change) {
			if (wiz::load_data::Utility::IsInteger(str_value)) {
				type = 3; 
				int_value = ToInt();
			}
			else if (wiz::load_data::Utility::IsDouble(str_value)) {
				type = 5;
				float_value = ToFloat();
			}
			else {
				throw "error in gettype in datatype";
			}
		}
		change = false;

		return type;
	}

	void DataType::SetInt(long long val)
	{
		this->type = 3;
		this->int_value = val;
		this->str_value = wiz::_toString(val);
		change = false;
	}

	void DataType::SetFloat(long double val)
	{
		this->type = 5;
		this->float_value = val;
		this->str_value = wiz::_toString(val);
		change = false;
	}

	DataType::DataType(const char* cstr)
	{
		this->str_value = std::string(cstr);
		
		if (USE_REMOVE_IN_DATATYPE) {
			this->str_value = Remove(str_value);
		}
		/* // #ifdef DataTypeDebug?
		this->change = true;

		if (wiz::load_data::Utility::IsInteger(this->str_value)) {
			this->type = 3;
			this->int_value = ToInt();
		}
		else if (wiz::load_data::Utility::IsDouble(this->str_value)) {
			this->type = 5;
			this->float_value = ToFloat();
		}
		else {
			this->type = 1;
		}
		*/

		this->change = true; // false;
	}
	DataType::DataType(const std::string& str)
	{
		this->str_value = str;

		if (USE_REMOVE_IN_DATATYPE) {
			this->str_value = Remove(str_value);
		}

		/*
		this->change = true;

		if (wiz::load_data::Utility::IsInteger(this->str_value)) {
			this->type = 3;
			this->int_value = ToInt();
		}
		else if (wiz::load_data::Utility::IsDouble(this->str_value)) {
			this->type = 5;
			this->float_value = ToFloat();
		}
		else {
			this->type = 1;
		}
		*/
		this->change = true; // false;
	}


	bool DataType::operator==(const DataType& type) const
	{
		return type.str_value == this->str_value;
	}
	bool DataType::operator==(const char* cstr) const
	{
		return cstr == this->str_value;
	}
	bool DataType::operator==(const std::string& str) const
	{
		return str == this->str_value;
	}

	bool DataType::operator!=(const DataType& type) const
	{
		return type.str_value != this->str_value;
	}
	bool DataType::operator!=(const char* cstr) const
	{
		return cstr != this->str_value;
	}
	bool DataType::operator!=(const std::string& str) const
	{
		return str != this->str_value;
	}

	DataType DataType::operator+(const char* cstr) const
	{
		return DataType(cstr + this->str_value);
	}
	DataType DataType::operator+(const std::string& str) const
	{
		return DataType(str + this->str_value);
	}

	DataType& DataType::operator+=(const DataType& type) 
	{
		this->str_value += type.str_value;

		this->change = true;
		return *this;
	}
	DataType& DataType::operator+=(const char* cstr) 
	{
		(*this) += DataType(cstr);
		return *this;
	}
	DataType& DataType::operator+=(const std::string& str)
	{
		(*this) += DataType(str);
		return *this;
	}

	bool operator==(const char* cstr, const DataType& type)
	{
		return type == cstr;
	}
	bool operator==(const std::string& str, const DataType& type)
	{
		return type == str;
	}

	bool operator!=(const char* cstr, const DataType& type)
	{
		return type != cstr;
	}
	bool operator!=(const std::string& str, const DataType& type)
	{
		return type != str;
	}

	DataType operator+(const char* cstr, const DataType& type)
	{
		return type + cstr;
	}
	DataType operator+(const std::string& str, const DataType& type)
	{
		return type + str;
	}
}

