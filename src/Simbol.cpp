#include "Simbol.h"

Simbol::Simbol(string naziv, string sekcija, string seklab, int pomeraj, string lokalna, int velicina, int broj, bool ex)
{

    this->pomeraj = pomeraj;
    this->lokalna = lokalna;
    this->seklab = seklab;
    this->broj = broj;
    this->sekcija = sekcija;
    this->definisan = false;
    this->velicina = velicina;
    this->jeEqu = false;
    this->naziv = naziv;
    this->ex = ex;
}
void Simbol::dodajobracanje(string obracanje)
{ //?
}
void Simbol::definisi()
{
    definisan = true;
}

ostream &operator<<(ostream &os, const Simbol s)
{

    string definisan = "";
    if (s.definisan)
    {
        definisan = "yes";
    }
    else
        definisan = "not";
    os << s.broj << "\t\t" << s.naziv;
    string padding = "";
    if (s.naziv.length() < 25)
    {
        for (int i = 0; i < 25 - s.naziv.length(); i++)
        {
            padding += " ";
        }
    }
    os << "\t\t" << padding << s.sekcija;
    if (s.sekcija == ".UND")
    {
        os << ":";
    }
    os << "\t\t\t" << s.seklab << "\t\t   " << s.pomeraj << "\t\t\t" << s.lokalna << "\t\t" << s.velicina << "\t\t\t" << definisan;

    return os;
}