#include "std.h"
#include "bbsys.h"
#include "userlibs.h"
#include "../MultiLang/MultiLang.h"

#include <Windows.h>

static std::vector<HMODULE> _mods;

struct Str {
	char* p;
	int size;
};

static Str _strs[256];
static int _nextStr;
static std::string libFile;
static std::string funcName;

static void libNotFound() {
	RTEX(std::format(MultiLang::userlib_not_found, libFile).c_str());
}

static void procNotFound() {
	RTEX(std::format(MultiLang::userlib_function_not_found, funcName).c_str());
}

void _bbLoadLibs(char* p) {

	std::string home;

	libFile = p;
	if (const char* t = getenv("blitzpath")) home = t;
	while (*p) {
		HMODULE mod = LoadLibrary(p);
		if (!mod && home.size()) {
			mod = LoadLibrary((home + "/userlibs/" + p).c_str());
		}
		p += strlen(p) + 1;
		if (mod) {
			_mods.push_back(mod);
			while (*p) {
				funcName = p;
				void* proc = GetProcAddress(mod, p);
				p += strlen(p) + 1;
				void* ptr = *(void**)p;
				p += 4;
				*(void**)ptr = proc ? proc : procNotFound;
			}
		}
		else {
			while (*p) {
				p += strlen(p) + 1;
				void* ptr = *(void**)p;
				p += 4;
				*(void**)ptr = libNotFound;
			}
		}
		++p;
	}
}

const char* _bbStrToCStr(BBStr* str) {

	Str& t = _strs[_nextStr++ & 255];

	int size = str->size();

	if (!t.p || t.size < size) {
		delete[] t.p;
		t.p = new char[size + 1];
		t.size = size;
	}

	memcpy(t.p, str->data(), size);
	t.p[size] = 0;
	delete str;
	return t.p;
}

BBStr* _bbCStrToStr(const char* str) {
	return new BBStr(str);
}

bool userlibs_create() {
	return true;
}

void userlibs_destroy() {
	for (; _mods.size(); _mods.pop_back()) FreeLibrary(_mods.back());
}

void userlibs_link(void(*rtSym)(const char*, void*)) {
	rtSym("_bbLoadLibs", _bbLoadLibs);
	rtSym("_bbStrToCStr", _bbStrToCStr);
	rtSym("_bbCStrToStr", _bbCStrToStr);
}