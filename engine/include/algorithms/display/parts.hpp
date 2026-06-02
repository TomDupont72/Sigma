#pragma once

#include "domain/tree.hpp"
#include <string>

using namespace std;

string displaySum(Node *node);

string displayProduct(Node *node);

string displayPower(Node *node);

string displayUnaryFunction(Node *node, string name);

string displayExpression(Node *node, int parentPriority);

int priority(Node *node);
