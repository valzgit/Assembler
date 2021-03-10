#include "Asembler.h"
#include "Tokeni.h"

#include "RTabela.h"
#include "TObracanja.h"
#include "Section.h"

Asembler::Asembler(string in, string out)
{
    this->in = in;
    this->out = out;
}

void Asembler::generisiTokene()
{
    ifstream ins;
    ofstream outs;
    try
    {
        ins.open(in);
        outs.open(out);

        TabelaSimbola *ts = new TabelaSimbola();
        TabelaObracanja *to = new TabelaObracanja(".UND", ts);
        RelocationTable *rt = new RelocationTable();
        Section *s = new Section();
        Tokeni t = Tokeni(ins, ts, rt, s, to, outs);
        if (outs.is_open())
        {
            outs.close();
        }
        if (ins.is_open())
        {
            ins.close();
        }
        if (ts != nullptr)
            delete (ts);
        if (to != nullptr)
            delete (to);
        if (rt != nullptr)
            delete (rt);
        if (s != nullptr)
            delete (s);
    }
    catch (const ifstream::failure &e)
    {
        cout << "Greska pri otvaranju ili citanju iz fajla!";
    }
}