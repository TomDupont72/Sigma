#include "domain/token.hpp"
#include "domain/tree.hpp"
#include <vector>

using namespace std;

class Parser
{
    private:
        vector<Token> tokens;
        size_t pos;

    public:
        Parser(const vector<Token>& tokens);

        Node* parseExpression();
        Node* parseTerm();
        Node* parseFactor();

        Token currentToken();
        void advance();
};
