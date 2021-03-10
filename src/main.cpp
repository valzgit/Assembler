#include <iostream>
#include <string>
#include "Asembler.h"

using namespace std;
int main(int argc, char **argv)
{
    if (argc < 3 || argc > 3)
    {
        cout << "Prvi operand mora biti output fajl, a drugi input fajl!" << endl;
        return 1;
    }
    else
    {
        string input = argv[2];
        string output = argv[1];

        Asembler a = Asembler(input, output);
        a.generisiTokene();
    }

    return 0;
}