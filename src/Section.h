#ifndef SECTION_H
#define SECTION_H
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <stdint.h>

using namespace std;
class SectionTwo
{
public:
    int dodajBajt(char val)
    {
        data.insert(std::pair<int, char>(counter, val));
        counter++;
        return counter - 1;
    }
    void dodajViseBajtova(int number, char val);
    int dodajDvaBajta(short val);

    void promeniBajt(int position, char val)
    {
        data[position] += val;
    }
    unsigned char dohvatiBajt(int position)
    {
        return data.at(position);
    }
    void promeniDvaBajta(int position, short val);
    string name;
    string sl;
    int size;
    int counter = 0;
    map<int, unsigned char> data;
    friend ostream &operator<<(ostream &os, const SectionTwo &s);
};

class Section
{

public:
    Section()
    {
        pocetne.push_back(counter);
        nazivi.push_back(".UND");
        SectionTwo *s = new SectionTwo();
        s->counter = counter;
        sekcije.push_back(s);
    }
    int dodajBajt(char val)
    {
        counter++;
        return sekcije.at(sekcije.size() - 1)->dodajBajt(val);
    }
    void dodajViseBajtova(int number, char val);
    int dodajDvaBajta(short val);

    void promeniBajt(int position, char val);
    unsigned char dohvatiBajt(int position);
    void promeniDvaBajta(int position, short val);
    void novaSekcija(string naziv)
    {
        pocetne.push_back(counter);
        nazivi.push_back(naziv);
        SectionTwo *s = new SectionTwo();
        s->counter = counter;
        sekcije.push_back(s);
    }
    static string napravistring(unsigned char second);

    int counter = 0;
    vector<SectionTwo *> sekcije;
    vector<int> pocetne;
    vector<string> nazivi;

    friend ostream &operator<<(ostream &os, const Section &s);
};
#endif