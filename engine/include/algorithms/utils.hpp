#pragma once

#include "domain/tree.hpp"
#include <sstream>
#include <vector>

using namespace std;

string join(const vector<string> &values, const string &separator);

string joinSum(const vector<string> &parts);

bool isNumber(const string &s);

string numberToString(float value);

void printTree(Node *node, string prefix = "", bool isLast = true);

float operation(float a, float b, string value);

vector<Node *> copyWithoutIndex(const vector<Node *> &children, int indexToRemove);

int binom(int n, int k);

Node *canonicalExpression(Node *node, string boundVariable);

bool isInverseOf(Node *node, Node *base);

bool isExactDivision(Node *node, Node *numerator, Node *denominator);
