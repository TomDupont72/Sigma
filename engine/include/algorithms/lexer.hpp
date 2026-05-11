#include "domain/token.hpp"
#include <vector>
#include <string>

using namespace std;

vector<Token> lexer(string expression);

void parseDigit(string &expression, vector<Token> &res, int &index, int expressionLength);

void parseAlpha(string &expression, vector<Token> &res, int &index, int expressionLength);