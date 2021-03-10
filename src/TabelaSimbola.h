#ifndef TABELASIMBOLA_H
#define TABELASIMBOLA_H
#include <map>
#include <string>
#include "Simbol.h"
#include "Section.h"
#include "RTabela.h"

class TabelaObracanja;

using namespace std;
class TabelaSimbola
{
public:
    map<string, Simbol> symbols;
    TabelaSimbola();
    bool kraj(string *tekst);
    bool dodajNoviSimbol(Simbol &s);

    int dohvatiVrednost(Simbol &s, string sekcija);
    bool isLabelaDefined(Simbol &s);
    bool isGlobal(Simbol &s);
    bool definisiSimbol(Simbol &s);
    int counter;
    bool definisiSekciju(Simbol &s, string starasekcija, int slc);
    bool definisiSimbol(Simbol &s, Section *sekcija, RelocationTable *rt, TabelaObracanja *to);

    int svakakoDohvatiVrednost(Simbol &s);

    Simbol *dohvatiSimbol(string naziv);

    friend ostream &operator<<(ostream &os, const TabelaSimbola &st);

    void obelezi(string naziv, int gde, char znak, bool jedanbajt, bool skok = false);

private:
};

#endif