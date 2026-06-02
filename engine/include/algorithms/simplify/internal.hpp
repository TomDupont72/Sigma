#pragma once

#include "domain/tree.hpp"
#include "types/tokenTypes.hpp"
#include <map>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

Node *simplifyOperation(Node *node, string value);

void collectTerms(Node *node, vector<Node *> &terms, string value);

tuple<map<string, float>, map<string, Node *>, float> createMappings(vector<Node *> terms, string value);

tuple<map<string, float>, map<string, Node *>, float> createProductMappings(vector<Node *> terms, string value);

vector<Node *> combineProductPowers(vector<Node *> children);

tuple<Node *, Node *> createPowerMapping(Node *node);

vector<Node *> constructChildren(map<string, float> sumMapping, map<string, Node *> symbolicMapping, float sumNumber, string value);

vector<Node *> constructPowerChildren(map<string, float> mapping, map<string, Node *> symbolicMapping, float number);

Node *constructPower(Node *base, Node *exponent);

Node *applyRewriteRules(Node *node);

Node *rewriteSumRules(Node *node);

Node *rewriteTrigIdentity(Node *node, SumIndex index);

Node *rewriteAdjacentSums(Node *node, SumIndex index);

SumIndex buildSumIndex(vector<Node *> terms);
