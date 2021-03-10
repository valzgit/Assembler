#include "Section.h"

void SectionTwo::dodajViseBajtova(int number, char val)
{
    for (int i = 0; i < number; i++)
    {
        dodajBajt(val);
    }
}

int SectionTwo::dodajDvaBajta(short val)
{
    int pom = counter;
    char MASK = 255;

    unsigned char value = (unsigned char)val & MASK;
    data.insert(std::pair<int, char>(counter, value));
    counter++;
    value = (val >> 8) & MASK;
    data.insert(std::pair<int, char>(counter, value));
    counter++;
    return pom;
}
void SectionTwo::promeniDvaBajta(int position, short val)
{

    unsigned short stvarnavrednost = ((unsigned char)(data.at(position + 1)));
    stvarnavrednost <<= 8;
    stvarnavrednost += ((unsigned char)(data.at(position)));
    short vr = stvarnavrednost;
    vr += val;

    char MASK = 255;
    unsigned char value = (unsigned char)(vr & MASK); //donji bajt
    data[position] = value;
    value = (vr >> 8) & MASK; //gornji bajt
    data[position + 1] = value;
}

ostream &operator<<(ostream &os, const SectionTwo &s)
{

    for (auto it : s.data)
    {
        string out = Section::napravistring(it.second);
        os << out << "\t";
    }
    os << endl;
    return os;
}
string Section::napravistring(unsigned char second)
{
    char str[10];
    sprintf(str, "%02X", second);
    string out(str);
    return out;
}
ostream &operator<<(ostream &os, const Section &s)
{

    for (int i = 0; i < s.sekcije.size(); i++)
    {
        os << "#" << s.nazivi.at(i);
        if (s.nazivi.at(i) == ".UND")
            os << ":";
        os << endl;
        os << *s.sekcije.at(i) << endl;
    }

    return os;
}

void Section::dodajViseBajtova(int number, char val)
{
    sekcije.at(sekcije.size() - 1)->dodajViseBajtova(number, val);
    counter += number;
}

int Section::dodajDvaBajta(short val)
{
    int pom = sekcije.at(sekcije.size() - 1)->dodajDvaBajta(val);
    counter += 2;
    return pom;
}

void Section::promeniBajt(int position, char val)
{
    for (int i = 0; i < pocetne.size(); i++)
    {
        if (position >= pocetne.at(i) && position < sekcije.at(i)->counter)
        {
            sekcije.at(i)->promeniBajt(position, val);
        }
    }
}

void Section::promeniDvaBajta(int position, short val)
{
    for (int i = 0; i < pocetne.size(); i++)
    {
        if (position >= pocetne.at(i) && position < sekcije.at(i)->counter)
        {
            sekcije.at(i)->promeniDvaBajta(position, val);
        }
    }
}

unsigned char Section::dohvatiBajt(int position)
{

    for (int i = 0; i < pocetne.size(); i++)
    {
        if (position >= pocetne.at(i) && position < sekcije.at(i)->counter)
        {
            return sekcije.at(i)->dohvatiBajt(position);
        }
    }
    return 0;
}