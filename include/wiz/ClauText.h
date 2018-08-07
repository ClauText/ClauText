
#ifndef CLAUTEXT_H
#define CLAUTEXT_H


#define ARRAY_QUEUE std::deque // chk?
#define VECTOR std::vector

#include <iostream>
#include <vector>
#include <map>
#include <utility>
#include <algorithm>
#include <string>
#include <ctime>
#include <cstdlib>

#include <regex> 

#ifdef _MSC_VER 
#include <conio.h> // for windows, _getch function.
#endif

#include "global.h"
#include "STRINGBUILDER.H"

#include "queues.h" // no use?

#ifdef USE_FAST_LOAD_DATA // REMOVE!

#include "wiz_load_data_user_type.h"
#include "wiz_load_data.h"
#include "utility.h"

#endif

#include "load_data.h"
#include "stacks.h"
#include "cpp_string.h"

class MData
{
public:
	bool isDir;
	std::string varName;
	int no; /// for UserType that has same name.!, rename?
public:
	explicit MData(const bool isDir = false, const std::string& varName = "", const int no = 0) : isDir(isDir), varName(varName), no(no)
	{

	}
};


#endif

