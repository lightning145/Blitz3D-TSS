#ifndef MULTILANG
#define MULTILANG

namespace MultiLang {
#ifdef CHINESE_SIMP // Simplified Chinese
	#include "SimplifiedChinese.cpp"
#elif defined CHINESE_TRAD // Traditional Chinese
	#include "TraditionalChinese.cpp"
#else // English
	#include "English.cpp"
#endif
};

#endif