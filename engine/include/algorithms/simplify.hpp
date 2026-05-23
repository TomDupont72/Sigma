#pragma once

#include "domain/tree.hpp"
#include <vector>
#include <string>
#include <map>

using namespace std;

Node * normalize(Node * node);

Node * simplify(Node * node);

Node * simplifyOperation(Node * node, string value);

void collectTerms(Node * node, vector<Node *>& terms, string value);

tuple<map<string, float>, map<string, Node *>, float> createMappings(vector<Node *> terms, string value);

tuple<map<string, float>, map<string, Node *>, float> createProductMappings(vector<Node *> terms, string value);

tuple<Node*, float> createPowerMapping(Node* node);

vector<Node *> constructChildren(map<string, float> sumMapping, map<string, Node *> symbolicMapping, float sumNumber, string value);

vector<Node *> constructPowerChildren(map<string, float> mapping, map<string, Node *> symbolicMapping, float number);

Node* constructPower(Node* base, float exponent);

Node * applyRewriteRules(Node * node);
