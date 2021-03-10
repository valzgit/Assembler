#ifndef ASEMBLER_H
#define ASEMBLER_H
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
class Asembler
{
public:
    Asembler(string in, string out);
    void generisiTokene();

private:
    string in;
    string out;
};

#endif