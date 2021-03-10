#include "TObracanja.h"
#include <map>
using namespace std;
TabelaSimbola::TabelaSimbola()
{
    counter = 0;
    Simbol *s = new Simbol(".UND", ".UND", "section", 0, "local", 0);
    dodajNoviSimbol(*s);
    definisiSimbol(*s);
}
bool TabelaSimbola::dodajNoviSimbol(Simbol &s)
{
    if (symbols.find(s.naziv) == symbols.end())
    {
        s.broj = counter;
        counter++;
        symbols.insert(std::pair<string, Simbol>(s.naziv, s));
        return true;
    }
    else
    {
        return false;
    }
}
Simbol *TabelaSimbola::dohvatiSimbol(string naziv)
{
    auto it = symbols.find(naziv);
    if (it != symbols.end())
    {

        return &it->second;
    }
    else
        return nullptr;
}
void TabelaSimbola::obelezi(string naziv, int gde, char znak, bool jedanbajt, bool skok)
{
    auto it = symbols.find(naziv);
    it->second.pozicije.push_back(gde);
    it->second.znaci.push_back(znak);
    it->second.velicine.push_back(jedanbajt);
    it->second.skok.push_back(skok);
}
bool TabelaSimbola::definisiSimbol(Simbol &s, Section *sekcija, RelocationTable *rt, TabelaObracanja *to)
{
    string lokalna = "global";
    int broj = 0;

    auto it = symbols.find(s.naziv);

    if (it != symbols.end())
    {
        if (it->second.definisan == true || it->second.ex == true)
            return false;

        definisiSimbol(s);
        broj = it->second.broj;
        lokalna = it->second.lokalna;
    }

    for (int i = 0; i < it->second.pozicije.size(); i++)
    {
        bool skok = it->second.skok.at(i);
        int poz = it->second.pozicije.at(i);
        bool jedanbajt = it->second.velicine.at(i);

        int ofs = it->second.pomeraj;
        char znak = it->second.znaci.at(i);

        if (skok)
        {

            ofs -= rt->vratibroj(broj);
        }

        if (jedanbajt)
        {
            if (znak == '+')

                sekcija->promeniBajt(poz, ofs);
            else
            {
                sekcija->promeniBajt(poz, -ofs);
            }
        }
        else
        {
            if (znak == '+')
            {
                sekcija->promeniDvaBajta(poz, ofs);
            }
            else
            {
                sekcija->promeniDvaBajta(poz, -ofs);
            }
        }
    }

    while (it->second.pozicije.size() != 0)
    {
        it->second.pozicije.pop_back();
        it->second.znaci.pop_back();
        it->second.velicine.pop_back();
        it->second.skok.pop_back();
    }

    if (lokalna == "local")
    {
        rt->ocisti(broj, dohvatiSimbol(rt->section)->broj);
    }
    to->ocisti(s.naziv, sekcija, rt, to);
    return true;
}

bool TabelaSimbola::isLabelaDefined(Simbol &s)
{
    auto it = symbols.find(s.naziv);
    if (it != symbols.end())
    {
        return it->second.definisan;
    }
    else
        return false;
}
bool TabelaSimbola::definisiSekciju(Simbol &s, string starasekcija, int slc)
{
    auto it = symbols.find(s.naziv);
    it->second.definisan = true;
    it = symbols.find(starasekcija);
    if (it != symbols.end())
    {
        it->second.velicina = slc;
    }
    return true;
}
int TabelaSimbola::dohvatiVrednost(Simbol &s, string sekcija)
{
    auto it = symbols.find(s.naziv);
    if (it != symbols.end())
    {
        if (it->second.sekcija == sekcija)
        {
            return it->second.pomeraj;
        }
    }
    return -1;
}
int TabelaSimbola::svakakoDohvatiVrednost(Simbol &s)
{
    auto it = symbols.find(s.naziv);
    if (it != symbols.end())
    {
        return it->second.pomeraj;
    }
    else
        return -1;
}
bool TabelaSimbola::isGlobal(Simbol &s)
{
    auto it = symbols.find(s.naziv);
    if (it != symbols.end())
    {
        return (it->second.lokalna == "global");
    }
    else
        return false;
}
ostream &operator<<(ostream &os, const TabelaSimbola &st)
{
    os << "#id\t\t#name\t\t                    #section\t\t#type\t\t#offset\t\t  #scope\t\t#size\t\t#Defined" << endl;
    for (auto const &sym : st.symbols)
    {
        os << sym.second << endl;
    }
    return os;
}
bool TabelaSimbola::definisiSimbol(Simbol &s)
{

    auto it = symbols.find(s.naziv);
    if (it != symbols.end())
    {
        it->second.sekcija = s.sekcija;
        it->second.definisan = true;
        it->second.pomeraj = s.pomeraj;
    }
    return true;
}

bool TabelaSimbola::kraj(string *tekst)
{
    bool kraj = true;
    for (auto const &sym : symbols)
    {
        if (sym.second.definisan == false)
        {
            if (sym.second.ex == false)
            {
                *tekst += "Simbol " + sym.second.naziv + " nije definisan!!\n";
                kraj = false;
            }
        }
    }

    return kraj;
}
