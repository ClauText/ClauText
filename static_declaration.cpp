#define _CRT_SECURE_NO_WARNINGS

#define ARRAY_QUEUE std::deque // chk?
#define VECTOR std::vector

#include <iostream>

#ifdef _MSC_VER 
#include <conio.h>
#endif

#include "wiz/STRINGBUILDER.H"

#include "wiz/load_data.h"

#include "wiz/cpp_string.h"
const std::vector<std::string> wiz::StringTokenizer::whitespaces = { " ", "\t", "\r", "\n" };

