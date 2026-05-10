#include <iostream>
#include <vector>
#include "../../domain/tree.hpp"
#include "../../domain/function.hpp"

using namespace std;

int main()
{
    Node *a = new Node("a", {});
    Node *b = new Node("b", {});

    vector<Node *> children = {a, b};

    Plus p(children);

    cout << p.display();

    return 0;
};