#pragma once

#ifdef REDUCEDLL_EXPORTS
#define REDUCEDLL_API __declspec(dllexport)
#else
#define REDUCEDLL_API __declspec(dllimport)
#endif

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

extern "C" REDUCEDLL_API void reduce(string key, string ints, string outputdirectory, string outputfile);
extern "C" REDUCEDLL_API void sort(string inputdirectory, string inputfile, vector<string> fileData);
extern "C" REDUCEDLL_API void parse_each(string inputdirectory, string inputfile, string outputdirectory, string outputfile, vector<string> fileData);
extern "C" REDUCEDLL_API vector<string> collectData(string inputdirectory, string inputfile);
extern "C" REDUCEDLL_API void reduce_export(string key, int n, string outputdirectory, string outputfile);

extern "C" REDUCEDLL_API void execute(string inputdirectory, string inputfile, string outputdirectory, string outputfile);