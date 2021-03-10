#ifndef RTABELA_H
#define RTABELA_H
#include "Simbol.h"
#include <string>
#include <iostream>
#include <vector>
#include <map>

using namespace std;

class Relocation
{
public:
    Relocation(short pomeraj, string relokTip, Simbol &s, string sekcija);

    friend ostream &operator<<(ostream &os, const Relocation &r);

    Simbol s;
    short pomeraj;
    string relokTip;
    string sekcija;
};
class RelocationTableTwo
{
public:
    RelocationTableTwo(string section);
    friend ostream &operator<<(ostream &os, const RelocationTableTwo &rt);

    string section;
    vector<Relocation> relocations;
};

class RelocationTable
{
public:
    RelocationTable();
    static string napravistring(short pomeraj);
    string dohvSekciju() { return section; }
    void staviSekciju(string section)
    {

        this->section = section;
        mapa.insert(std::pair<string, RelocationTableTwo *>(section, new RelocationTableTwo(section)));
        sekcije.push_back(section);
    }
    friend ostream &operator<<(ostream &os, const RelocationTable &rt);
    void ocisti(int broj, int ts);
    map<string, RelocationTableTwo *> mapa = {{".UND", new RelocationTableTwo(".UND")}};
    short vratibroj(int broj);
    string section = ".UND";

    vector<string> sekcije;

    void dodaj(Relocation *rel)
    {
        (mapa.at(section)->relocations).push_back(*rel);
    }
};

#endif