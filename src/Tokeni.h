#ifndef TOKENI_H
#define TOKENI_H
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <regex>
#include "TObracanja.h"
#include "RTabela.h"

#include "Section.h"

using namespace std;
class Tokeni
{
public:
    Tokeni(ifstream &in, TabelaSimbola *ts, RelocationTable *rt, Section *s, TabelaObracanja *to, ofstream &out);
    int locationcounter = 0;
    int poslinstrukcija = 0;
    TabelaSimbola *ts;
    TabelaObracanja *to;
    static string tsekcija;
    static string tlabela;
    RelocationTable *rt;
    Section *s;
    vector<string> skiniBlanko(string line);
    bool obradiTokene(vector<string> tokens);

    bool testirajSkokove(string icode, string token);
    bool testirajPusheve(string icode, string token);
    bool proveriSrcParametar(string icode, string token, bool izpusha = false);
    bool proveriDstParametar(string icode, string token, bool izpusha = false);
    bool proveribroj(string token);
    int hextodec(string token);
    int bintodec(string token);
    short instructionConvertor(int broj, int adresna, bool jedanbajt = false);
    char operandConvertorJumps(string token, bool skok = false);
    char brojregistra(string token);
    char nizivisi(string token);
    void gurnisimbol(string token, bool skok = false, bool jedanbajt = false);
    static map<string, int> instructionNumOperands;
    static map<string, int> instructionOpCode;
    bool dalijejedanbajt(string icode, string token1, string token2, int *n);
    void dodajequ(string tok, string proslediime, string znak, bool broj = false);
};

#endif