#include "TObracanja.h"

TabelaObracanja::TabelaObracanja(string section, TabelaSimbola *ts)
{
    this->section = section;
    this->ts = ts;
}

Obracanje::Obracanje(string naziv)
{
    this->naziv = naziv;
}

void TabelaObracanja::ocisti(string naziv, Section *sekcija, RelocationTable *rt, TabelaObracanja *to)
{
    int i = 0;
    while (i < obracanja.size())
    {
        int j = obracanja[i].simboli.size();
        j--;
        while (j >= 0)
        {

            if (obracanja[i].simboli[j].naziv == naziv)
            {

                if (obracanja[i].znaci[j] == "+")
                {
                    if (ts->dohvatiSimbol(naziv)->sekcija == ".text:")
                    {

                        obracanja[i].text++;
                    }
                    else if (ts->dohvatiSimbol(naziv)->sekcija == ".bss:")
                    {

                        obracanja[i].bss++;
                    }
                    else if (ts->dohvatiSimbol(naziv)->sekcija == ".data:")
                    {

                        obracanja[i].data++;
                    }
                    obracanja[i].value += ts->dohvatiSimbol(naziv)->pomeraj;
                }
                else
                {
                    if (ts->dohvatiSimbol(naziv)->sekcija == ".text:")
                    {

                        obracanja[i].text--;
                    }
                    else if (ts->dohvatiSimbol(naziv)->sekcija == ".bss:")
                    {

                        obracanja[i].bss--;
                    }
                    else if (ts->dohvatiSimbol(naziv)->sekcija == ".data:")
                    {

                        obracanja[i].data--;
                    }

                    obracanja[i].value -= ts->dohvatiSimbol(naziv)->pomeraj;
                }

                obracanja[i].und--;

                if (proveri(obracanja[i].naziv))
                {
                    ts->definisiSimbol(*ts->dohvatiSimbol(obracanja[i].naziv), sekcija, rt, to);
                }
            }
            j--;
        }
        i++;
    }
}
void TabelaObracanja::dodajobracanje(string naziv)
{

    obracanja.push_back(*(new Obracanje(naziv)));
}

void TabelaObracanja::dodajuObracanje(Simbol *simbol, string nobr, string znak)
{

    int i = 0;
    while (i < obracanja.size())
    {
        if (obracanja[i].naziv == nobr)
        {

            obracanja[i].simboli.push_back(*simbol);
            obracanja[i].znaci.push_back(znak);

            if (ts->isLabelaDefined(*simbol))
            {
                if (simbol->sekcija == ".bss:")
                {
                    if (znak == "+")
                        obracanja[i].bss++;
                    else
                        obracanja[i].bss--;
                }

                if (simbol->sekcija == ".text:")
                {
                    if (znak == "+")
                        obracanja[i].text++;
                    else
                        obracanja[i].text--;
                }

                if (simbol->sekcija == ".data:")
                {
                    if (znak == "+")
                        obracanja[i].data++;
                    else
                        obracanja[i].data--;
                }

                if (znak == "+")
                {
                    obracanja[i].value += simbol->pomeraj;
                }
                else
                {
                    obracanja[i].value -= simbol->pomeraj;
                }
            }
            else
            {
                obracanja[i].und++;
            }
        }
        i++;
    }
}
void TabelaObracanja::dodajuObracanjeBroj(int naziv, string nobr, string znak)
{

    int i = 0;
    while (i < obracanja.size())
    {
        if (obracanja[i].naziv == nobr)
        {

            if (znak == "+")
            {

                obracanja[i].value += naziv;
            }
            else
            {
                obracanja[i].value -= naziv;
            }
            return;
        }
        i++;
    }
}
bool TabelaObracanja::proveri(string proslediime)
{

    for (int i = 0; i < obracanja.size(); i++)
    {
        if (obracanja[i].naziv == proslediime)
        {
            int pom = 0;
            int pom2 = 0;
            pom = obracanja[i].text | obracanja[i].data | obracanja[i].bss;
            pom2 = obracanja[i].text + obracanja[i].data + obracanja[i].bss;
            if (pom == pom2 && (pom == 1 || pom == 0) && obracanja[i].und == 0)
            {
                ts->dohvatiSimbol(proslediime)->pomeraj = obracanja[i].value;
                return true;
            }
        }
    }
    return false;
}

bool TabelaObracanja::kraj(TabelaSimbola *ts)
{
    for (int i = 0; i < obracanja.size(); i++)
    {
        int pom = 0;
        int pom2 = 0;
        pom = obracanja[i].text | obracanja[i].data | obracanja[i].bss;
        pom2 = obracanja[i].text + obracanja[i].data + obracanja[i].bss;

        if (!(pom == pom2 && (pom == 1 || pom == 0) && obracanja[i].und == 0) || ((ts->dohvatiSimbol(obracanja.at(i).naziv))->pomeraj < 0) || (ts->dohvatiSimbol((ts->dohvatiSimbol(obracanja.at(i).naziv))->sekcija)->velicina < (ts->dohvatiSimbol(obracanja.at(i).naziv))->pomeraj))
        {
            cout << obracanja.at(i).naziv << " je kriticno obracanje bss:" << obracanja.at(i).bss << " text: " << obracanja.at(i).text << " data: " << obracanja.at(i).data << " und: " << obracanja.at(i).und << endl;
            cout << obracanja.at(i).naziv << " offset je: " << (ts->dohvatiSimbol(obracanja.at(i).naziv))->pomeraj << " a velicina sekcije u kojoj se nalazi je " << ts->dohvatiSimbol((ts->dohvatiSimbol(obracanja.at(i).naziv))->sekcija)->velicina << endl;
            return false;
        }
    }

    return true;
}