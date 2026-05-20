#pragma once

#include "domain/tree.hpp"
#include <vector>
#include <string>
#include <map>

using namespace std;

Node * simplify(Node * node);

Node * simplifyOperation(Node * node, string value);

void collectTerms(Node * node, vector<Node *>& terms, string value);

tuple<map<string, float>, map<string, Node *>, float> createMappings(vector<Node *> terms, string value);

vector<Node *> constructChildren(map<string, float> sumMapping, map<string, Node *> symbolicMapping, float sumNumber, string value);
