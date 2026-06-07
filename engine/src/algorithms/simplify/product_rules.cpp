#include "algorithms/simplify/internal.hpp"
#include "algorithms/display.hpp"
#include "algorithms/utils.hpp"
#include "domain/tree.hpp"
#include <string>
#include <vector>

using namespace std;

ProductIndex buildProductIndex(vector<Node *> terms)
{
    ProductIndex index;

    for (size_t i = 0; i < terms.size(); i++)
    {
        Node *term = terms[i];

        if (term->value == "sum")
        {
            Node *variable = term->children[0];
            Node *body = term->children[3];

            string key = "#|" + displayExpression(canonicalExpression(body, variable->value), 0);

            index.productsByVariableAndBody[key].push_back(i);
        }
    }

    return index;
}
