#include "RTabela.h"
#include <string>

RelocationTableTwo::RelocationTableTwo(string section)
{
    this->section = section;
}

Relocation::Relocation(short pomeraj, string relokTip, Simbol &s, string sekcija) : s(s)
{
    this->pomeraj = pomeraj;
    this->sekcija = sekcija;
    this->relokTip = relokTip;
}

ostream &operator<<(ostream &os, const RelocationTableTwo &rt)
{
    os << "#.ret" << rt.section;
    if (rt.section == ".UND")
        os << ":";
    os << endl;
    os << "#ofset\t\ttip\t\t\tvr[" << rt.section;
    if (rt.section == ".UND")
        os << ":";
    os << "]" << endl;
    for (Relocation r : rt.relocations)
    {
        os << r << endl;
    }
    os << endl;
    return os;
}
ostream &operator<<(ostream &os, const Relocation &r)
{

    string s = RelocationTable::napravistring(r.pomeraj);
    os << s << "\t\t" << r.relokTip << "\t\t" << r.s.broj << "\t\t"
       << "[" << r.sekcija << "]";
    return os;
}
string RelocationTable::napravistring(short pomeraj)
{
    char str[10];
    sprintf(str, "%04X", pomeraj);
    string s(str);
    return s;
}
ostream &operator<<(ostream &os, const RelocationTable &r)
{

    for (int i = 0; i < r.sekcije.size(); i++)
    {
        string pom = r.sekcije[i];

        os << *r.mapa.at(pom) << endl;
    }
    return os;
}

void RelocationTable::ocisti(int broj, int ts)
{
    for (int i = 0; i < sekcije.size(); i++)
    {
        string pom = sekcije[i];

        RelocationTableTwo *rtt = mapa.at(pom);

        for (int i = rtt->relocations.size() - 1; i >= 0; i--)
        {
            if (rtt->relocations[i].s.broj == broj)
            {
                if (section == rtt->relocations[i].sekcija)
                {
                    rtt->relocations.erase(rtt->relocations.begin() + i);
                }
                else
                {
                    rtt->relocations[i].s.broj = ts;
                }
            }
        }
    }
}

short RelocationTable::vratibroj(int broj)
{
    for (int i = 0; i < sekcije.size(); i++)
    {
        string pom = sekcije[i];

        RelocationTableTwo *rtt = mapa.at(pom);
        for (int i = 0; i < rtt->relocations.size(); i++)
        {
            if (rtt->relocations[i].s.broj == broj)
            {
                if (section == rtt->relocations[i].sekcija)
                {
                    return rtt->relocations[i].s.pomeraj;
                }
            }
        }
    }
    return 0;
}

RelocationTable::RelocationTable()
{
    sekcije.push_back(".UND");
}
