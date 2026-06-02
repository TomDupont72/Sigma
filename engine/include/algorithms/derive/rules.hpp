#pragma once

#include "domain/tree.hpp"
#include <string>

using namespace std;

Node *deriveNumber(Node *node);

Node *deriveIdentifier(Node *node, string value);

Node *derivePlus(Node *node, string variable);

Node *deriveMultiply(Node *node, string variable);

Node *derivePower(Node *node, string variable);

Node *deriveLn(Node *node, string variable);

Node *deriveExp(Node *node, string variable);

Node *deriveSin(Node *node, string variable);

Node *deriveCos(Node *node, string variable);
