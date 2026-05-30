#pragma once

#include "domain/tree.hpp"

using namespace std;

Node *derive(Node *node, string variable);

Node *deriveNumber(Node *node);

Node *deriveIdentifier(Node *node, string value);

Node *derivePlus(Node *node, string variable);

Node *deriveMultiply(Node *node, string variable);

Node *derivePower(Node *node, string variable);
