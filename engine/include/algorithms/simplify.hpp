#include "domain/tree.hpp"

Node * simplify(Node * node);

Node * simplifyPlus(Node * node);

void collectSumTerms(Node * node, vector<Node *>& terms);
