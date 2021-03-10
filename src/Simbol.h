#ifndef SIMBOL_H
#define SIMBOL_H
#include <queue>
#include <iterator>
#include <list>
#include <iostream>
#include <string>

using namespace std;

class Simbol
{
public:
    Simbol(string naziv, string sekcija, string seklab, int pomeraj, string lokalna, int velicina, int broj = 0, bool ex = false);
    string naziv;
    bool definisan;
    int velicina;
    int broj;
    int pomeraj;
    bool jeEqu;
    string seklab;
    string sekcija;
    bool ex;
    string lokalna;
    vector<int> pozicije;
    vector<char> znaci;
    vector<bool> velicine;
    vector<bool> skok;
    friend ostream &operator<<(ostream &os, const Simbol s);
    void dodajobracanje(string obracanje);
    void definisi();
};

#endif