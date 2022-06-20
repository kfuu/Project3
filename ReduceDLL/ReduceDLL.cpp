#include "ReduceDLL.h"
#include "../MapDLL/FileManager.h"

#include <vector>
#include <map>

REDUCEDLL_API void reduce(string key, string ints, string outputdirectory, string outputfile)
{
	int sum = 0;
	for (int i = 0; i < ints.size(); i++) {
		if (ints[i] == '1') {
			sum += 1;
		}
	}

	reduce_export(key, sum, outputdirectory, outputfile);
}

REDUCEDLL_API vector<string> collectData(string inputdirectory, string inputfile) {
    FileManager fm(inputdirectory, inputfile, 'r'); // we are reading in the intermediate file here.
    fm.openfileforreading();
    fm.readFile();

    vector<string> fileData = fm.fileContent;
	return fileData;
}

REDUCEDLL_API void sort(string inputdirectory, string inputfile, vector<string> fileData) {
	FileManager fm(inputdirectory, inputfile, 'w');
	fm.openfileforwriting();

	vector<tuple<string, string>> tokenList;
	map<string, vector<string>> frequencyMap;
	string sortedString;

	// first, convert string data to tupled data <string, int>
	for (string t : fileData) {
		string temp = t.erase(0, 1); // erase first parens
		temp.erase(temp.size() - 1); // erase last parens

		string leftString = "";
		string rightString = "";
		bool onLeftSide = true;

		for (int i = 0; i < temp.size(); i++) {
			if (temp[i] != ',' && onLeftSide) {
				leftString += temp[i];
			}

			if (temp[i] == ',') {
				onLeftSide = false;
			}

			if (temp[i] != ',' && !(onLeftSide)) {
				rightString += temp[i];
			}
		}
		tokenList.emplace_back(leftString, rightString);
	}

	for (tuple<string, string> token : tokenList) {
		string word = get<0>(token);
		string count = get<1>(token);

		if (!(frequencyMap.count(word))) {
			vector<string> v;
			v.push_back(count);
			frequencyMap.insert(pair<string, vector<string>>(word, v));
		}
		else {
			frequencyMap[word].push_back(count);

		}
	}

	map<string, vector<string>>::iterator it;
	for (it = frequencyMap.begin(); it != frequencyMap.end(); it++) {
		string word = it->first;
		string listOfOnes = "[";

		for (int n = 0; n < it->second.size(); n++) {
			listOfOnes += it->second[n];
			if (n + 1 != it->second.size()) {
				listOfOnes += ", ";
			}
		}
		listOfOnes += "]";

		sortedString += "(" + word + ", " + listOfOnes + "), ";
	}

	fm.writetoFile(sortedString);
}

REDUCEDLL_API void parse_each(string inputdirectory, string inputfile, string outputdirectory, string outputfile, vector<string> fileData) {
	FileManager fm(inputdirectory, inputfile, 'r');
	fm.openfileforreading();
	fm.readFile();

	string read_string = fileData[0];

	string leftString = "";
	string rightString = "";
	bool onLeftSide = true;

	for (int i = 0; i < read_string.size(); i++) {
		if (read_string[i] == '(') { // skips over the left parens
			onLeftSide = true;
		}
		else if (read_string[i] == ',' && read_string[i - 1] != ')' && onLeftSide) { // case of when we're at the comma inside the tuple
			onLeftSide = false;
			i++; // skip the space after comma
		}
		else if (read_string[i] == ')') { // when we reach the closed parens
			reduce(leftString, rightString, outputdirectory, outputfile);

			// reset
			leftString = "";
			rightString = "";
			onLeftSide = true;
		}
		else if (read_string[i] != ',' && read_string[i] != ' ' && onLeftSide) { // when we're on left side of tuple and it's not a , or ' '
			leftString += read_string[i];
		}
		else if (!(onLeftSide)) { // when we're on right side of tuple (including commas and spaces)
			rightString += read_string[i];
		}

	}

	// If reach here, then everything was outputted successfully.
	FileManager fm2(outputdirectory, "SUCCESS.txt", 'w');
	fm2.openfileforwriting();
	fm2.writetoFile("SUCCESS");
}

REDUCEDLL_API void reduce_export(string key, int n, string outputdirectory, string outputfile) {
	FileManager fm(outputdirectory, outputfile, 'a');
	fm.openfileforappending();

	string result = key + ": " + to_string(n) + "\n";

	fm.appendtofile(result);
}

REDUCEDLL_API void execute(string inputdirectory, string inputfile, string outputdirectory, string outputfile)
{
	vector<string> fileData = collectData(inputdirectory, inputfile);
	sort(inputdirectory, inputfile, fileData);
	parse_each(inputdirectory, inputfile, outputdirectory, outputfile, fileData);
}
