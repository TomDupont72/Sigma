#pragma once

#include "domain/tree.hpp"
#include <sstream>
#include<vector>

using namespace std;

string join(const vector<string>& values, const string& separator);

string joinSum(const vector<string>& parts);

bool isNumber(const string& s);

string numberToString(float value);

void printTree(Node* node, string prefix = "", bool isLast = true);

float operation(float a, float b, string value);
