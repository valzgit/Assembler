#ifndef TOBRACANJA_H
#define TOBRACANJA_H

#include "TabelaSimbola.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class TabelaSimbola;
class Obracanje
{
public:
    Obracanje(string naziv);

    int bss = 0;
    int text = 0;
    int data = 0;
    int und = 0;

    vector<Simbol> simboli;
    vector<string> znaci;
    int value = 0;

    string naziv;
};
class TabelaObracanja
{
public:
    TabelaObracanja(string section, TabelaSimbola *ts);

    void dodajobracanje(string naziv);
    void dodajuObracanjeBroj(int naziv, string nobr, string znak);
    void dodajuObracanje(Simbol *naziv, string nobr, string znak);
    void staviSekciju(string section)
    {
        if (section != "")
            this->section = section;
        else
            this->section = ".UND";
    }

    void ocisti(string naziv, Section *sekcija, RelocationTable *rt, TabelaObracanja *to);
    bool proveri(string proslediime);

    bool kraj(TabelaSimbola *ts);
    string section;
    TabelaSimbola *ts;
    vector<Obracanje> obracanja;
};

#endif