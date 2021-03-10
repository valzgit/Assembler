#include "Tokeni.h"

Tokeni::Tokeni(ifstream &in, TabelaSimbola *ts, RelocationTable *rt, Section *s, TabelaObracanja *to, ofstream &out)
{
    string line;
    this->ts = ts;
    this->to = to;
    this->rt = rt;
    this->s = s;
    while (getline(in, line))
    {
        vector<string> tokens;
        tokens = skiniBlanko(line);
        if (tsekcija == ".end" && tokens.size() != 0)
        {
            cout << endl
                 << "GRESKA NA LINIJI " << line << endl;
            break;
        }
        //for(int i=0;i<tokens.size();i++){
        //cout<<tokens[i]<<endl;}
        if (obradiTokene(tokens) == false)
        {
            cout << endl
                 << "GRESKA NA LINIJI " << line << endl;
            break;
        }
    }

    // proveriti da li se zavrsava sa .end
    string tekst = "";
    if (tsekcija == ".end" && to->kraj(ts) && ts->kraj(&tekst))
    {

        out << *ts << endl;
        out << endl
            << *rt << endl;
        out << endl
            << *s << endl;
    }
    else
    {
        if (tekst == "" && tsekcija == ".end")
            cout << "Obracanja nisu razresena!" << endl;
        else if (tsekcija != ".end")
        {
            cout << ".end ne postoji!" << endl;
        }
        else
            cout << tekst << endl;
    }
};

map<string, int> Tokeni::instructionOpCode =
    {
        {"halt", 0},
        {"iret", 1},
        {"ret", 2},
        {"int", 3},
        {"call", 4},
        {"jmp", 5},
        {"jeq", 6},
        {"jne", 7},
        {"jgt", 8},
        {"push", 9},
        {"pop", 10},
        {"xchg", 11},
        {"mov", 12},
        {"add", 13},
        {"sub", 14},
        {"mul", 15},
        {"div", 16},
        {"cmp", 17},
        {"not", 18},
        {"and", 19},
        {"or", 20},
        {"xor", 21},
        {"test", 22},
        {"shl", 23},
        {"shr", 24}};

map<string, int> Tokeni::instructionNumOperands =
    {{"halt", 0},
     {"iret", 0},
     {"ret", 0},
     {"jmp", 1},
     {"jeq", 1},
     {"jne", 1},
     {"jgt", 1},
     {"call", 1},
     {"int", 1},
     {"push", 1},
     {"pop", 1},
     {"add", 2},
     {"sub", 2},
     {"mul", 2},
     {"div", 2},
     {"cmp", 2},
     {"xchg", 2},
     {"and", 2},
     {"or", 2},
     {"xor", 2},
     {"not", 2},
     {"test", 2},
     {"mov", 2},
     {"shl", 2},
     {"shr", 2}};

vector<string> Tokeni::skiniBlanko(string line)
{
    vector<string> niz;
    while (!line.empty() && line.size() > 0)
    {
        if (line.at(0) == '\n')
        {
            line.resize(0);
            break;
        }
        if (line.at(0) == ' ' || line.at(0) == '\t' || line.at(0) == ',' || line.at(0) == '+' || line.at(0) == '-')
        {
            int i = 0;
            while (i < line.length() && (line.at(i) == ' ' || line.at(i) == '\t' || line.at(i) == ',' || line.at(i) == '+' || line.at(i) == '-'))
            {
                if (line.at(i) == ',')
                {
                    niz.push_back(",");
                }
                if (line.at(i) == '+')
                {
                    niz.push_back("+");
                }
                if (line.at(i) == '-')
                {
                    niz.push_back("-");
                }
                i++;
            }
            line.erase(0, i);
        }
        if (line.size() == 0)
            break;
        if (line.at(0) == '#')
            break;
        int brojac = 0;
        while (!(line.at(brojac) == ' ' || line.at(brojac) == '\t' || line.at(brojac) == ',' || line.at(brojac) == '+' || line.at(brojac) == '-' || line.at(brojac) == '\n'))
        {
            brojac++;
            if (brojac == line.size())
                break;
        }
        if (brojac > 0)
        {
            niz.push_back(line.substr(0, brojac));
            line.erase(0, brojac);
        }
        else if (brojac <= 0)
        {
            break;
        }
    }

    return niz;
}

string Tokeni::tsekcija = "";
string Tokeni::tlabela = "";

bool Tokeni::obradiTokene(vector<string> tokens)
{
    using namespace std;

    regex instrukcija{"(halt|iret|ret|jmp|jeq|jgt|jne|call|int|push|pop|add|sub|mul|div|cmp|xchg|and|or|xor|not|test|mov|shl|shr)(b|w)?"};
    regex labela{"([a-zA-Z][a-zA-Z0-9_]*):"};
    regex direktiva{"\\.(byte|word|equ|skip)"};
    regex symbol{("^([a-zA-Z][a-zA-Z0-9_]*)$")};
    regex zvezdicasymbol{("^\\*([a-zA-Z][a-zA-Z0-9_]*)$")};
    regex op_sym$_value{"(\\$)([a-zA-Z][a-zA-Z0-9_]*)"};
    regex bukvregistar{"^(r[0-7](h|l)?)$"};
    regex registar{"^(\\%r[0-7](h|l)?)$"};
    regex memregistar{"^(\\(\\%r[0-7](h|l)?\\))$"};
    regex zvezdicaprocenatregistar{"^(\\*\\%r[0-7](h|l)?)$"};
    regex zzprz{"^(\\*\\(\\%r[0-7](h|l)?\\))$"};
    regex relsimbol{"^(\\*)([a-zA-Z][a-zA-Z0-9_]*)\\(\\%r[0-7](h|l)?\\)$"};
    regex lzprz{"^([a-zA-Z][a-zA-Z0-9_]*)\\(\\%r[0-7](h|l)?\\)$"};
    regex relpc{"^(\\*)([a-zA-Z][a-zA-Z0-9_]*)\\(\\%pc\\)$"};
    regex relpcbezzvezdice{"^([a-zA-Z][a-zA-Z0-9_]*)\\(\\%pc\\)$"};
    regex relr7{"^(\\*)([a-zA-Z][a-zA-Z0-9_]*)\\(\\%r7(h|l)?\\)$"};
    regex sekcija{"^(\\.section)$"};
    regex section{"(\\.)(text|data|bss)\\:"};
    regex sectionbezdv{"(\\.)(text|data|bss)"};
    smatch match;

    for (int i = 0; i < tokens.size(); i++)
    {
        string input = tokens[i];
        if (input == ".end")
        {
            string starasekcija = Tokeni::tsekcija;
            Tokeni::tsekcija = input;
            int slc = locationcounter;
            Tokeni::tsekcija = tokens[i];
            locationcounter = 0;
            Simbol *s = new Simbol(tsekcija, tsekcija, "section", locationcounter, "local", 0);
            if (!ts->dodajNoviSimbol(*s))
            {
                cout << "Dva puta definisana sekcija!" << endl;
                return false;
            }
            ts->definisiSekciju(*s, starasekcija, slc);
            if (++i < tokens.size())
            {
                cout << ".end nije poslednji a trebalo bi da bude!" << endl;
                return false;
            }
            return true;
        }
        else if (input == ".global")
        {
            if (tsekcija != "")
            {
                cout << ".GLOBAL ne sme da bude van UND direktive!" << endl;
                return false;
            }
            bool simbol = false;
            i += 1;
            for (; i < tokens.size(); i++)
            {
                if (!simbol && regex_match(tokens[i], symbol))
                {
                    simbol = true;
                    Simbol *s = new Simbol(tokens[i], ".UND", "label", locationcounter, "global", 0);
                    if (!ts->dodajNoviSimbol(*s))
                    {
                        cout << "Vec definisan simbol!" << endl;
                        return false;
                    };
                }
                else if (simbol && tokens[i] == ",")
                {
                    simbol = false;
                }
                else
                {
                    cout << "Neispravan format " << input << endl;
                    return false;
                }
            }
            if (simbol == false)
            {
                cout << "Nedovrsena " << input << " direktiva!" << endl;
                return false;
            }
        }
        else if (input == ".extern")
        {
            if (tsekcija != "")
            {
                cout << ".extern se ne sme nalaziti unutar drugih sekcija!" << endl;
                return false;
            }
            bool simbol = false;
            i += 1;
            for (; i < tokens.size(); i++)
            {
                if (!simbol && regex_match(tokens[i], symbol))
                {
                    simbol = true;
                    Simbol *s = new Simbol(tokens[i], ".UND", "label", locationcounter, "global", 0, 0, true);
                    if (!ts->dodajNoviSimbol(*s))
                    {
                        cout << "Vec definisan simbol!" << endl;
                        return false;
                    };
                }
                else if (simbol && tokens[i] == ",")
                {
                    simbol = false;
                }
                else
                {
                    cout << "Neispravan format " << input << endl;
                    return false;
                }
            }
            if (simbol == false)
            {
                cout << "Nedovrsena " << input << " direktiva!" << endl;
                return false;
            }
        }
        else if (regex_match(input, labela))
        {
            Tokeni::tlabela = input;
            string pom = tlabela;
            pom.replace(pom.length() - 1, 1, "");
            Simbol *si = new Simbol(pom, tsekcija, "label", locationcounter, "local", 0);
            if (!ts->dodajNoviSimbol(*si))
            {
                //postoji u tabeli
                if (ts->isLabelaDefined(*si))
                {
                    cout << "Druga definicija iste labele!" << endl;
                    return false;
                }
                if (ts->dohvatiSimbol(si->naziv)->ex == true)
                {
                    cout << "SIMBOL JE EXTERN!" << si->pomeraj << " " << si->naziv << " " << si->sekcija << endl;
                    return false;
                }
            }
            bool dobro;
            dobro = ts->definisiSimbol(*si, s, rt, to);
            if (!dobro)
            {
                cout << "SIMBOL JE EXTERN!" << si->pomeraj << " " << si->naziv << " " << si->sekcija << endl;

                return false;
            }
        }
        else if (regex_match(input, sekcija))
        {
            i++;
            if (i != tokens.size() && regex_match(tokens[i], section))
            {
                string starasekcija = Tokeni::tsekcija;
                int slc = locationcounter;
                Tokeni::tsekcija = tokens[i];
                locationcounter = 0;
                //cout << *rt;
                rt->staviSekciju(Tokeni::tsekcija);
                to->staviSekciju(Tokeni::tsekcija);
                s->novaSekcija(Tokeni::tsekcija);
                Simbol *s = new Simbol(tsekcija, tsekcija, "section", locationcounter, "local", 0);
                if (!ts->dodajNoviSimbol(*s))
                {
                    cout << "Dva puta definisana sekcija!" << endl;
                    return false;
                }
                ts->definisiSekciju(*s, starasekcija, slc);
            }
            else
                return false;
        }
        else if (regex_match(input, direktiva))
        {
            if (Tokeni::tsekcija == "")
            {
                cout << "Direktive se ne smeju nalaziti u .UND  sekciji!" << endl;
                return false;
            }
            if (tokens[i] == ".equ")
            { //moze prvo da stoji + ili - uz labelu koja se navodi!
                if (tokens[i + 2] != ",")
                {
                    cout << "Pogresan format direktive .equ!" << endl;
                    return false;
                }
                Simbol *si = new Simbol(tokens[i + 1], tsekcija, "label", locationcounter, "local", 0);
                ts->dodajNoviSimbol(*si);
                if (ts->dohvatiSimbol(si->naziv)->definisan == true)
                {
                    cout << "SIMBOL JE VEC DEFINISAN!" << si->pomeraj << " " << si->naziv << " " << si->sekcija << endl;
                    return false;
                }
                if (ts->dohvatiSimbol(si->naziv)->ex == true)
                {
                    cout << "SIMBOL JE EXTERN!" << si->pomeraj << " " << si->naziv << " " << si->sekcija << endl;
                    return false;
                }

                to->dodajobracanje(si->naziv);
                string proslediime = si->naziv;
                i += 3;

                bool simbol = false;
                int b2 = 0;
                string znak = "+";
                int zbir = 0;
                if (tokens[i] == "+" || tokens[i] == "-")
                {
                    znak = tokens[i];
                    i++;
                }
                for (; i < tokens.size(); i++)
                {
                    if (!simbol && (regex_match(tokens[i], symbol) || proveribroj(tokens[i]) || regex_match(tokens[i], sectionbezdv)))
                    {
                        simbol = true;
                        if (!proveribroj(tokens[i]) && !regex_match(tokens[i], sectionbezdv))
                        {
                            b2++;

                            dodajequ(tokens[i], proslediime, znak);
                        }
                        else if (proveribroj(tokens[i]))
                        {
                            dodajequ(tokens[i], proslediime, znak, true);
                        }
                    }

                    else if (simbol && (tokens[i] == "+" || tokens[i] == "-"))
                    {
                        znak = tokens[i];
                        simbol = false;
                    }
                    else
                    {
                        cout << "Neispravan format .equ direktive" << endl;
                        return false;
                    }
                }
                if (to->proveri(proslediime))
                {
                    bool dobro;
                    dobro = ts->definisiSimbol(*ts->dohvatiSimbol(proslediime), s, rt, to);
                    if (!dobro)
                    {
                        return false;
                    }
                }

                if (simbol == false)
                {
                    cout << "Nedovrsena .equ direktiva!";
                    return false;
                }
            }
            else if (tokens[i] == ".byte" || tokens[i] == ".word")
            {
                regex op_dec{("^([0-9]+)$")};
                regex op_bin{("^0b([0-1]+)$")};
                regex op_hex{("^(0[xX][0-9a-fA-F]+)$")};
                string prostor = tokens[i];
                bool simbol = false;
                i += 1;

                for (; i < tokens.size(); i++)
                {
                    string znak = "+";
                    if (tokens[i] == "+" || tokens[i] == "-")
                    {
                        znak = tokens[i];
                        i++;
                    }
                    if (!simbol && (regex_match(tokens[i], symbol) || proveribroj(tokens[i]) || regex_match(tokens[i], sectionbezdv)))
                    {
                        simbol = true;
                        if (regex_match(tokens[i], symbol))
                        {
                            if (prostor == ".byte")
                                gurnisimbol(tokens[i], false, true);
                            else
                                gurnisimbol(tokens[i], false, false);
                        }
                        else if (proveribroj(tokens[i]))
                        {
                            if (regex_match(tokens[i], op_dec))
                            {
                                if (prostor == ".byte")
                                {

                                    if (znak == "+")
                                    {
                                        s->dodajBajt(stoi(tokens[i]));
                                    }
                                    else
                                    {
                                        s->dodajBajt(-stoi(tokens[i]));
                                    }
                                }
                                else
                                {
                                    if (znak == "+")
                                    {
                                        s->dodajDvaBajta(stoi(tokens[i]));
                                    }
                                    else
                                    {
                                        s->dodajDvaBajta(-stoi(tokens[i]));
                                    }
                                }
                            }
                            else if (regex_match(tokens[i], op_hex))
                            {
                                if (prostor == ".byte")
                                {
                                    if (znak == "+")
                                        s->dodajBajt(hextodec(tokens[i]));
                                    else
                                        s->dodajBajt(-hextodec(tokens[i]));
                                }
                                else
                                {
                                    if (znak == "+")
                                        s->dodajDvaBajta(hextodec(tokens[i]));
                                    else
                                        s->dodajDvaBajta(-hextodec(tokens[i]));
                                }
                            }
                            else
                            {
                                if (prostor == ".byte")
                                {
                                    if (znak == "+")
                                        s->dodajBajt(bintodec(tokens[i]));
                                    else
                                        s->dodajBajt(-bintodec(tokens[i]));
                                }
                                else
                                {
                                    if (znak == "+")
                                        s->dodajDvaBajta(bintodec(tokens[i]));
                                    else
                                        s->dodajDvaBajta(-bintodec(tokens[i]));
                                }
                            }
                        }
                        else
                        { //dont care hahahahah
                            if (prostor == ".byte")
                            {
                                s->dodajBajt(0);
                            }
                            else
                            {
                                s->dodajDvaBajta(0);
                            }
                        }
                        if (prostor == ".byte")
                        {
                            locationcounter += 1;
                        }
                        else
                        {
                            locationcounter += 2;
                        }
                    }
                    else if (simbol && tokens[i] == ",")
                    {
                        simbol = false;
                    }
                    else
                    {
                        cout << "Neispravan format " << input << endl;
                        return false;
                    }
                }
                if (simbol == false)
                {
                    cout << "Nedovrsena " << input << " direktiva!" << endl;
                    return false;
                }
            }
            else if (tokens[i] == ".skip")
            {
                regex op_dec{("^([0-9]+)$")};
                regex op_bin{("^0b([0-1]+)$")};
                regex op_hex{("^(0[xX][0-9a-fA-F]+)$")};
                if (tokens.size() != i + 2)
                {
                    cout << "Nekoretkna .skip direktiva";
                    return false;
                }
                i++;
                if (!proveribroj(tokens[i]))
                {
                    cout << "Parametar .skip direktive nije literal!";
                    return false;
                }
                else
                {
                    if (regex_match(tokens[i], op_dec))
                    {
                        locationcounter += stoi(tokens[i]);
                        s->dodajViseBajtova(stoi(tokens[i]), 0);
                    }
                    else if (regex_match(tokens[i], op_hex))
                    {
                        locationcounter += hextodec(tokens[i]);
                        s->dodajViseBajtova(hextodec(tokens[i]), 0);
                    }
                    else
                    {
                        locationcounter += bintodec(tokens[i]);
                        s->dodajViseBajtova(bintodec(tokens[i]), 0);
                    }
                }
            }
            else
            {
                cout << "Inkorektna sintaksa direktive" << endl;
                return false;
            }
        }
        else if (regex_match(input, instrukcija))
        {
            if (Tokeni::tsekcija != ".text:")
            {
                cout << "Instrukcije se nalaze u pogresnoj sekciji!" << endl;
                return false;
            }
            smatch match;
            regex_match(tokens[i], match, instrukcija);
            string icode = match[1];
            int size = instructionNumOperands[icode];
            //cout<<"Velicina od "<<icode<<" je "<<size<<endl;
            if (size == 0 && tokens.size() - 1 - i == 0)
            {
                //cout<<"0 adresna instrukcija"<<endl;
                s->dodajBajt(instructionConvertor(instructionOpCode[icode], 0, true));
            }
            else if (size == 1 && tokens.size() - 1 - i == 1)
            {
                //cout<<"1 adresna instrukcija"<<endl;
                s->dodajBajt(instructionConvertor(instructionOpCode[icode], 1));
                i++;
                if (testirajSkokove(icode, tokens[i]))
                { //cout<<"Usao u skok";
                }
                else if (testirajPusheve(icode, tokens[i]))
                { //cout<<"Usao u push/pop";
                }
                else
                {
                    cout << "Neispravna jednoadresna funkcija!" << endl;
                    return false;
                }
            }
            else if (size == 2 && tokens.size() - 1 - i == 3)
            {
                //cout<<"2 adresna instrukcija"<<endl;
                int zblb = 0;
                int *n = &zblb;
                s->dodajBajt(instructionConvertor(instructionOpCode[icode], 2, dalijejedanbajt(tokens[i], tokens[i + 1], tokens[i + 3], n)));
                i += 3;
                if (zblb == -1)
                {
                    cout << "Registar h/l se koristi uz instrukciju velicine word! " << endl;
                    return false;
                }
                if (zblb == -2)
                {
                    cout << "Registar h/l se koristi uz registar r7 pri relativnom adresiranju!" << endl;
                    return false;
                }
                if (zblb == -3)
                {
                    cout << "JMP byte!" << endl;
                    return false;
                }
                if (tokens[i - 1] != ",")
                {
                    cout << "Los format dvoadresne insturkcije!" << endl;
                    return false;
                }
                else if (icode != "shr" && (!proveriSrcParametar(icode, tokens[i - 2]) || !proveriDstParametar(icode, tokens[i])))
                {
                    cout << "Parametri dvoadresne instrukcije nisu dobrog oblika!" << endl;
                    return false;
                }
                else if (icode == "shr" && (!proveriDstParametar(icode, tokens[i - 2]) || !proveriSrcParametar(icode, tokens[i])))
                {
                    cout << "Parametri dvoadresne instrukcije nisu dobrog oblika!" << endl;
                    return false;
                }
                else
                {
                    //cout<<"Dvoadresna instrukcija ima PEDIGREE najvise klase!";
                }
            }
            else
            {
                cout << "Nesipravno napisana instrukcija! " << endl;
                return false;
            }
        }
        else
        {
            cout << "Neadekvatna instrukcija!" << endl;
            return false;
        }
    }
    return true;
}

bool Tokeni::testirajSkokove(string icode, string token)
{
    regex symbol{("^([a-zA-Z][a-zA-Z0-9_]*)$")};
    regex zvezdicasymbol{("^\\*([a-zA-Z][a-zA-Z0-9_]*)$")};
    regex bukvregistar{"^(r[0-7](h|l)?)$"};
    regex zvezdicaprocenatregistar{"^(\\*\\%r[0-7](h|l)?)$"};
    regex zzprz{"^(\\*\\(\\%r[0-7](h|l)?\\))$"};
    regex relsimbol{"^(\\*)([a-zA-Z][a-zA-Z0-9_]*)\\(\\%r[0-7](h|l)?\\)$"};
    regex relpc{"^(\\*)([a-zA-Z][a-zA-Z0-9_]*)\\(\\%pc\\)$"};
    regex relr7{"^(\\*)([a-zA-Z][a-zA-Z0-9_]*)\\(\\%r7(h|l)?\\)$"};
    regex relsimbolbroj{"^(\\*)([0-9]+)\\(\\%r[0-7](h|l)?\\)$"};
    regex relsimbolbrojhex{"^(\\*)(0[xX][0-9a-fA-F]+)\\(\\%r[0-7](h|l)?\\)$"};
    regex relsimbolbrojbin{"^(\\*)0b([0-1]+)\\(\\%r[0-7](h|l)?\\)$"};
    regex op_dec{("^([0-9]+)$")};
    regex op_bin{("^0b([0-1]+)$")};
    regex op_hex{("^(0[xX][0-9a-fA-F]+)$")};
    regex op_deczvezda{("^(\\*[0-9]+)$")};
    regex op_deczvezdabin{("^(\\*0b([0-1]+))$")};
    regex op_deczvezdahex{("^(\\*(0[xX][0-9a-fA-F]+))$")};
    regex skinipc{("\\(\\%pc\\)")};
    regex skinireg{("\\(\\%r[0-7](h|l)?\\)")};
    if ((icode == "int" || icode == "call" || icode == "jmp" || icode == "jne" || icode == "jgt" || icode == "jeq") &&
        ((regex_match(token, symbol) && !regex_match(token, bukvregistar)) || regex_match(token, zzprz) ||
         regex_match(token, zvezdicaprocenatregistar) || regex_match(token, relsimbol) || regex_match(token, relpc) || regex_match(token, relr7) || (regex_match(token, zvezdicasymbol) && !regex_match(token, bukvregistar)) || regex_match(token, relsimbolbrojbin) || regex_match(token, relsimbolbrojhex) || regex_match(token, relsimbolbroj) || regex_match(token, op_dec) || regex_match(token, op_bin) || regex_match(token, op_hex) || regex_match(token, op_deczvezdabin) || regex_match(token, op_deczvezdahex) || regex_match(token, op_deczvezda)))
    {
        string kopija = token;
        operandConvertorJumps(kopija, true);

        return true;
    }

    else
        return false;
}
bool Tokeni::testirajPusheve(string icode, string token)
{
    if (icode == "push")
        return proveriSrcParametar(icode, token, true);
    else if (icode == "pop")
        return proveriDstParametar(icode, token, true);
    else
        return false;
}
bool Tokeni::proveriSrcParametar(string icode, string token, bool izpusha)
{
    regex symbol{("^([a-zA-Z][a-zA-Z0-9_]*)$")};
    regex op_sym$_value{"(\\$)([a-zA-Z][a-zA-Z0-9_]*)"};
    regex bukvregistar{"^(r[0-7](h|l)?)$"};
    regex registar{"^(\\%r[0-7](h|l)?)$"};
    regex memregistar{"^(\\(\\%r[0-7](h|l)?\\))$"};
    regex lzprz{"^([a-zA-Z][a-zA-Z0-9_]*)\\(\\%r[0-7](h|l)?\\)$"};
    regex relpcbezzvezdice{"^([a-zA-Z][a-zA-Z0-9_]*)\\(\\%pc\\)$"};
    regex op_dec{("^([0-9]+)$")};
    regex op_bin{("^0b([0-1]+)$")};
    regex op_hex{("^(0[xX][0-9a-fA-F]+)$")};
    regex op_dec${("^(\\$[0-9]+)$")};
    regex op_dec$hex{("^(\\$(0[xX][0-9a-fA-F]+))$")};
    regex op_dec$bin{("^(\\$0b([0-1]+))$")};
    regex bzprz{"^([0-9]+)\\(\\%(r[0-7]|pc)(h|l)?\\)$"};
    regex bzprzhex{"^(0[xX][0-9a-fA-F]+)\\(\\%(r[0-7]|pc)(h|l)?\\)$"};
    regex bzprzbin{"^(0b([0-1]+))\\(\\%(r[0-7]|pc)(h|l)?\\)$"};
    regex skinipc{("\\(\\%pc\\)")};
    regex skinireg{("\\(\\%r[0-7](h|l)?\\)")};
    regex psw{"(\\%psw)"};
    if ((izpusha == true && regex_match(token, psw)) ||
        (regex_match(token, symbol) && !regex_match(token, bukvregistar)) ||
        regex_match(token, registar) || regex_match(token, memregistar) || regex_match(token, bzprz) || regex_match(token, bzprzbin) || regex_match(token, bzprzhex) ||
        regex_match(token, op_sym$_value) || regex_match(token, lzprz) || regex_match(token, relpcbezzvezdice) || regex_match(token, op_dec) || regex_match(token, op_bin) || regex_match(token, op_hex) || regex_match(token, op_dec$bin) || regex_match(token, op_dec$hex) || regex_match(token, op_dec$))
    {
        string pom = token;
        operandConvertorJumps(pom);
        return true;
    }

    else
        return false;
}
bool Tokeni::proveriDstParametar(string icode, string token, bool izpusha)
{
    regex symbol{("^([a-zA-Z][a-zA-Z0-9_]*)$")};
    regex bukvregistar{"^(r[0-7](h|l)?)$"};
    regex registar{"^(\\%r[0-7](h|l)?)$"};
    regex memregistar{"^(\\(\\%r[0-7](h|l)?\\))$"};
    regex lzprz{"^([a-zA-Z][a-zA-Z0-9_]*)\\(\\%r[0-7](h|l)?\\)$"};
    regex relpcbezzvezdice{"^([a-zA-Z][a-zA-Z0-9_]*)\\(\\%pc\\)$"};
    regex op_dec{("([0-9]+)")};
    regex op_bin{("^0b([0-1]+)$")};
    regex op_hex{("^(0[xX][0-9a-fA-F]+)$")};
    regex bzprz{"^([0-9]+)\\(\\%(r[0-7]|pc)(h|l)?\\)$"};
    regex bzprzhex{"^(0[xX][0-9a-fA-F]+)\\(\\%(r[0-7]|pc)(h|l)?\\)$"};
    regex bzprzbin{"^(0b([0-1]+))\\(\\%(r[0-7]|pc)(h|l)?\\)$"};
    regex skinipc{("\\(\\%pc\\)")};
    regex skinireg{("\\(\\%r[0-7](h|l)?\\)")};
    regex psw{"(\\%psw)"};
    if ((izpusha == true && regex_match(token, psw)) ||
        (regex_match(token, symbol) && !regex_match(token, bukvregistar)) ||
        regex_match(token, registar) || regex_match(token, memregistar) || regex_match(token, bzprz) || regex_match(token, bzprzbin) || regex_match(token, bzprzhex) || regex_match(token, lzprz) || regex_match(token, relpcbezzvezdice) || regex_match(token, op_dec) || regex_match(token, op_bin) || regex_match(token, op_hex))
    {
        string pom = token;
        operandConvertorJumps(pom);

        return true;
    }

    else
        return false;
}
bool Tokeni::proveribroj(string token)
{
    regex op_dec{("^([0-9]+)$")};
    regex op_bin{("^0b([0-1]+)$")};
    regex op_hex{("^(0[xX][0-9a-fA-F]+)$")};
    return (regex_match(token, op_dec) || regex_match(token, op_bin) || regex_match(token, op_hex));
}
int Tokeni::hextodec(string token)
{
    token = token.substr(2, token.size() - 2);
    int len = token.size();
    int base = 1;
    int temp = 0;
    for (int i = len - 1; i >= 0; i--)
    {
        if (token.at(i) >= '0' && token.at(i) <= '9')
        {
            temp += (token.at(i) - 48) * base;
            base = base * 16;
        }
        else if (token.at(i) >= 'A' && token.at(i) <= 'F')
        {
            temp += (token.at(i) - 55) * base;
            base = base * 16;
        }

        else if (token.at(i) >= 'a' && token.at(i) <= 'f')
        {
            temp += (token.at(i) - 87) * base;
            base = base * 16;
        }
    }

    return temp;
}
int Tokeni::bintodec(string token)
{
    token = token.substr(2, token.size() - 2);
    int len = token.size();
    int base = 1;
    int temp = 0;
    for (int i = len - 1; i >= 0; i--)
    {
        temp += (token.at(i) - 48) * base;
        base = base * 2;
    }
    return temp;
}
short Tokeni::instructionConvertor(int broj, int adresna, bool jedanbajt)
{

    cout << locationcounter << " Location " << s->counter << " Counter" << endl;
    poslinstrukcija = s->counter;
    if (jedanbajt)
    {
        locationcounter++;
        return (broj << 3);
    }
    else
    {
        locationcounter++;
        broj = broj << 1;
        broj += 1;
        return broj << 2;
    }
    return 0;
}
char Tokeni::operandConvertorJumps(string token, bool skok)
{
    regex symbol{("^([a-zA-Z][a-zA-Z0-9_]*)$")};
    regex zvezdicasymbol{("^\\*([a-zA-Z][a-zA-Z0-9_]*)$")};
    regex zvezdicaprocenatregistar{"^(\\*\\%r[0-7](h|l)?)$"};
    regex zzprz{"^(\\*\\(\\%r[0-7](h|l)?\\))$"};
    regex relsimbol{"^(\\*)([a-zA-Z][a-zA-Z0-9_]*)\\(\\%r[0-7](h|l)?\\)$"};
    regex relpc{"^(\\*)([a-zA-Z][a-zA-Z0-9_]*)\\(\\%pc\\)$"};
    regex relsimbolbroj{"^(\\*)([0-9]+)\\(\\%r[0-7](h|l)?\\)$"};
    regex relsimbolbrojhex{"^(\\*)(0[xX][0-9a-fA-F]+)\\(\\%r[0-7](h|l)?\\)$"};
    regex relsimbolbrojbin{"^(\\*)0b([0-1]+)\\(\\%r[0-7](h|l)?\\)$"};
    regex op_dec{("^([0-9]+)$")};
    regex op_bin{("^0b([0-1]+)$")};
    regex op_hex{("^(0[xX][0-9a-fA-F]+)$")};
    regex op_deczvezda{("^(\\*[0-9]+)$")};
    regex op_deczvezdabin{("^(\\*0b([0-1]+))$")};
    regex op_deczvezdahex{("^(\\*(0[xX][0-9a-fA-F]+))$")};
    regex skinipc{("\\(\\%pc\\)")};
    regex skinireg{("\\(\\%r[0-7](h|l)?\\)")};
    regex op_sym$_value{"(\\$)([a-zA-Z][a-zA-Z0-9_]*)"};
    regex bukvregistar{"^(r[0-7](h|l)?)$"};
    regex registar{"^(\\%r[0-7](h|l)?)$"};
    regex memregistar{"^(\\(\\%r[0-7](h|l)?\\))$"};
    regex lzprz{"^([a-zA-Z][a-zA-Z0-9_]*)\\(\\%r[0-7](h|l)?\\)$"};
    regex lzprzh{"^([a-zA-Z][a-zA-Z0-9_]*)\\(\\%r7h\\)$"}; //lzprz mozda ne moze high low ali boli nas uvo
    regex lzprzl{"^([a-zA-Z][a-zA-Z0-9_]*)\\(\\%r7l\\)$"};
    regex relpcbezzvezdice{"^([a-zA-Z][a-zA-Z0-9_]*)\\(\\%pc\\)$"};
    regex op_dec${("^(\\$[0-9]+)$")};
    regex op_dec$hex{("^(\\$(0[xX][0-9a-fA-F]+))$")};
    regex op_dec$bin{("^(\\$0b([0-1]+))$")};
    regex bzprz{"^([0-9]+)\\(\\%(r[0-7]|pc)(h|l)?\\)$"};
    regex bzprzhex{"^(0[xX][0-9a-fA-F]+)\\(\\%(r[0-7]|pc)(h|l)?\\)$"};
    regex bzprzbin{"^(0b([0-1]+))\\(\\%(r[0-7]|pc)(h|l)?\\)$"};
    regex psw{"(\\%psw)"};
    int x = 0;

    if (regex_match(token, symbol) && skok)
    {
        x += 0;
        x = x << 5;
        s->dodajBajt(x);
        locationcounter += 1;
        gurnisimbol(token, false);
        locationcounter += 2;
    }
    else if (regex_match(token, zvezdicasymbol) && skok)
    {
        x += 4;
        x = x << 5;
        s->dodajBajt(x);
        locationcounter += 1;
        string pom = token.substr(1, token.length());
        gurnisimbol(pom, false);
        locationcounter += 2;
    }
    else if (regex_match(token, zvezdicaprocenatregistar) && skok)
    {
        x += 1;
        x = x << 4;
        char pom = brojregistra(token);
        x += pom;
        x = x << 1;
        x += nizivisi(token);

        s->dodajBajt(x);
        locationcounter += 1;
    }
    else if (regex_match(token, zzprz) && skok)
    {
        x += 2;
        x = x << 4;
        char pom = brojregistra(token);
        x += pom;
        x = x << 1;
        x += nizivisi(token);
        s->dodajBajt(x);
        locationcounter += 1;
    }
    else if (regex_match(token, relsimbol) && skok)
    {
        string pomstring = token.substr(1, token.length());
        pomstring = regex_replace(pomstring, skinipc, "");
        pomstring = regex_replace(pomstring, skinireg, "");
        x += 3;
        x = x << 4;
        char pom = brojregistra(token);
        x += pom;
        x = x << 1;
        x += nizivisi(token);
        s->dodajBajt(x);
        locationcounter += 1;
        if (pom == 7)
        {
            gurnisimbol(pomstring, true);
        }
        else
        {
            gurnisimbol(pomstring, false);
        }

        locationcounter += 2;
    }
    else if (regex_match(token, relpc) && skok)
    {
        string pomstring = token.substr(1, token.length());
        pomstring = regex_replace(pomstring, skinipc, "");
        pomstring = regex_replace(pomstring, skinireg, "");
        x += 3;
        x = x << 4;
        char pom = 7;
        x += pom;
        x = x << 1;
        x += 0;
        s->dodajBajt(x);
        locationcounter += 1;
        gurnisimbol(pomstring, true);
        locationcounter += 2;
    }
    else if ((regex_match(token, relsimbolbroj) || regex_match(token, relsimbolbrojbin) || regex_match(token, relsimbolbrojhex)) && skok)
    {
        int tip = 0;
        if (regex_match(token, relsimbolbrojhex))
        {
            tip = 2;
        }
        if (regex_match(token, relsimbolbroj))
        {
            tip = 1;
        }
        if (regex_match(token, relsimbolbrojbin))
        {
            tip = 3;
        }
        x += 3;
        x = x << 4;
        char pom = brojregistra(token);
        x += pom;
        x = x << 1;
        x += nizivisi(token);
        s->dodajBajt(x);
        locationcounter += 1;

        string pomstring = token.substr(1, token.length() - 1);
        pomstring = regex_replace(pomstring, skinireg, "");
        if (tip == 1)
        {

            s->dodajDvaBajta(stoi(pomstring));
        }
        else if (tip == 2)
        {
            s->dodajDvaBajta(hextodec(pomstring));
        }
        else
        {
            s->dodajDvaBajta(bintodec(pomstring));
        }
        locationcounter += 2;
    }
    else if (proveribroj(token) && skok)
    {

        x = 0;
        s->dodajBajt(x);
        locationcounter++;

        if (regex_match(token, op_dec))
        {
            s->dodajDvaBajta(stoi(token));
        }
        if (regex_match(token, op_hex))
        {
            s->dodajDvaBajta(hextodec(token));
        }
        if (regex_match(token, op_bin))
        {
            s->dodajDvaBajta(bintodec(token));
        }

        locationcounter += 2;
    }
    else if ((regex_match(token, op_deczvezda) || regex_match(token, op_deczvezdahex) || regex_match(token, op_deczvezdabin)) && skok)
    {
        int tip = 0;
        if (regex_match(token, op_deczvezdahex))
        {
            tip = 2;
        }
        if (regex_match(token, op_deczvezda))
        {
            tip = 1;
        }
        if (regex_match(token, op_deczvezdabin))
        {
            tip = 3;
        }
        string pomstring = token.substr(1, token.length());
        x = 4;
        x = x << 5;
        s->dodajBajt(x);
        locationcounter++;

        if (tip == 1)
        {
            s->dodajDvaBajta(stoi(pomstring));
        }
        else if (tip == 2)
        {
            s->dodajDvaBajta(hextodec(pomstring));
        }
        else
        {
            s->dodajDvaBajta(bintodec(pomstring));
        }

        locationcounter += 2;
    }
    else if (regex_match(token, symbol) && !regex_match(token, bukvregistar) && !skok)
    {
        x += 4;
        x = x << 5;
        s->dodajBajt(x);
        locationcounter += 1;

        if ((s->dohvatiBajt(poslinstrukcija) & 4) != 0)
        {
            gurnisimbol(token);
            locationcounter += 2;
        }
        else
        {
            gurnisimbol(token, false, true);

            locationcounter += 1;
        }
    }
    else if ((regex_match(token, bzprzhex) || regex_match(token, bzprz) || regex_match(token, bzprzbin)) && !skok)
    {
        int tip = 0;
        if (regex_match(token, bzprzhex))
        {
            tip = 2;
        }
        if (regex_match(token, bzprz))
        {
            tip = 1;
        }
        if (regex_match(token, bzprzbin))
        {
            tip = 3;
        }
        string pomstring = token.substr(0, token.length());
        pomstring = regex_replace(pomstring, skinipc, "");
        pomstring = regex_replace(pomstring, skinireg, "");
        x += 3;
        x = x << 4;
        char pom = brojregistra(token);

        x += pom;
        x = x << 1;
        x += nizivisi(token);
        s->dodajBajt(x);
        locationcounter++;

        if (tip == 1)
        {
            s->dodajDvaBajta(stoi(pomstring));
        }
        else if (tip == 2)
        {
            s->dodajDvaBajta(hextodec(pomstring));
        }
        else
        {
            s->dodajDvaBajta(bintodec(pomstring));
        }

        locationcounter += 2;
    }
    else if (regex_match(token, op_sym$_value) && !skok)
    {
        string pom = token;
        pom = pom.substr(1, token.length());
        x += 0;
        x = x << 5;
        s->dodajBajt(x);
        locationcounter += 1;
        if ((s->dohvatiBajt(poslinstrukcija) & 4) != 0)
        {
            gurnisimbol(pom);
            locationcounter += 2;
        }
        else
        {
            gurnisimbol(pom, false, true);
            locationcounter += 1;
        }
    }
    else if ((regex_match(token, relpcbezzvezdice) || regex_match(token, lzprz)) && !skok)
    {
        string pomstring = token.substr(0, token.length()); // pisalo je 1
        pomstring = regex_replace(pomstring, skinipc, "");
        pomstring = regex_replace(pomstring, skinireg, "");
        x += 3;
        x = x << 4;
        char pom = brojregistra(token);
        x += pom;
        x = x << 1;
        x += nizivisi(token);
        s->dodajBajt(x);
        locationcounter += 1;
        if ((s->dohvatiBajt(poslinstrukcija) & 4) != 0)
        {
            if (pom == 7)
            {
                gurnisimbol(pomstring, true);
            }
            else
            {
                gurnisimbol(pomstring);
            }

            locationcounter += 2;
        }
        else
        {
            gurnisimbol(pomstring, false, true);
            locationcounter += 1;
        }
    }
    else if (proveribroj(token) && !skok)
    {
        x = 4;
        s->dodajBajt(x);
        locationcounter++;
        int brojzadodati = 0;
        if (regex_match(token, op_dec))
            brojzadodati = stoi(token);
        else if (regex_match(token, op_hex))
            brojzadodati = hextodec(token);
        else
            brojzadodati = bintodec(token);
        if ((s->dohvatiBajt(poslinstrukcija) & 4) != 0)
        {
            s->dodajDvaBajta(brojzadodati);
            locationcounter += 2;
        }
        else
        {
            s->dodajBajt(brojzadodati);
            locationcounter++;
        }
    }
    else if ((regex_match(token, op_dec$) || regex_match(token, op_dec$bin) || regex_match(token, op_dec$hex)) && !skok)
    {

        x = 0;
        s->dodajBajt(x);
        locationcounter++;

        if ((s->dohvatiBajt(poslinstrukcija) & 4) != 0)
        {
            string pomstring = token.substr(1, token.length() - 1);
            if (regex_match(token, op_dec$))
            {
                s->dodajDvaBajta(stoi(pomstring));
            }
            else if (regex_match(token, op_dec$hex))
            {
                s->dodajDvaBajta(hextodec(pomstring));
            }
            else
            {
                s->dodajDvaBajta(bintodec(pomstring));
            }

            locationcounter += 2;
        }
        else
        {
            string pomstring = token.substr(1, token.length() - 1);
            if (regex_match(token, op_dec$))
            {
                s->dodajBajt(stoi(pomstring));
            }
            else if (regex_match(token, op_dec$hex))
            {
                s->dodajBajt(hextodec(pomstring));
            }
            else
            {
                s->dodajBajt(bintodec(pomstring));
            }
            locationcounter++;
        }
    }
    else if ((regex_match(token, registar) || regex_match(token, psw)) && !skok)
    {
        x = 1;
        x = x << 4;
        char pom = brojregistra(token);
        x += pom;
        x = x << 1;
        x += nizivisi(token);
        s->dodajBajt(x);
        locationcounter += 1;
    }
    else if (regex_match(token, memregistar) && !skok)
    {
        x = 2;
        x = x << 4;
        char pom = brojregistra(token);
        x += pom;
        x = x << 1;
        x += nizivisi(token);
        s->dodajBajt(x);
        locationcounter += 1;
    }
    else
    {
        return -1;
    }

    return 0;
}

char Tokeni::brojregistra(string token)
{
    regex r0{"(r0)"};
    regex r1{"(r1)"};
    regex r2{"(r2)"};
    regex r3{"(r3)"};
    regex r4{"(r4)"};
    regex r5{"(r5)"};
    regex r6{"(r6)"};
    regex r7{"(r7)"};
    regex pc{"(pc)"};
    regex psw{"(psw)"};

    if (regex_search(token, r0))
    {
        return 0;
    }
    else if (regex_search(token, r1))
    {
        return 1;
    }
    else if (regex_search(token, r2))
    {
        return 2;
    }
    else if (regex_search(token, r3))
    {
        return 3;
    }
    else if (regex_search(token, r4))
    {
        return 4;
    }
    else if (regex_search(token, r5))
    {
        return 5;
    }
    else if (regex_search(token, r6))
    {
        return 6;
    }
    else if (regex_search(token, r7))
    {
        return 7;
    }
    else if (regex_search(token, pc))
    {
        return 7;
    }
    else if (regex_search(token, psw))
    {
        return 15;
    }
    else
    {
        cout << "GRESKA PRI BIRANJU REGISTRA!" << token << endl;
    }
    return -1;
}
char Tokeni::nizivisi(string token)
{
    regex bukvregistarh{"(r[0-7]h)"};
    regex bukvregistarl{"(r[0-7]l)"};
    if (regex_match(token, bukvregistarh))
    {
        return 1;
    }
    else
        return 0;
}
void Tokeni::gurnisimbol(string token, bool skok, bool jedanbajt)
{

    Simbol *si = new Simbol(token, tsekcija, "label", locationcounter, "local", 0); // prvi bool je bio true
    ts->dodajNoviSimbol(*si);

    if (!ts->isGlobal(*si))
    {
        if (ts->isLabelaDefined(*si))
        { //definisan
            int pom = ts->dohvatiVrednost(*si, tsekcija);

            if (pom == -1)
            { //definisan ali u drugoj sekciji
                if (skok == false)
                {
                    if (!jedanbajt)
                        s->dodajDvaBajta(ts->svakakoDohvatiVrednost(*si));
                    else
                        s->dodajBajt(ts->svakakoDohvatiVrednost(*si));

                    Simbol *sekcija = ts->dohvatiSimbol(si->naziv);
                    Relocation *rel = new Relocation(locationcounter, "R_386_64", *ts->dohvatiSimbol(sekcija->sekcija), tsekcija);
                    rt->dodaj(rel);
                }
                else
                {
                    s->dodajDvaBajta(ts->svakakoDohvatiVrednost(*si) - 2); //komplikovana razlika

                    Simbol *sekcija = ts->dohvatiSimbol(si->naziv);
                    Relocation *rel = new Relocation(locationcounter, "R_386_PC64", *ts->dohvatiSimbol(sekcija->sekcija), tsekcija);
                    rt->dodaj(rel);
                }

                //realok table
            }
            else
            { //definisan u istoj sekciji

                if (skok)
                { //ako jeste skok
                    s->dodajDvaBajta(pom - (locationcounter + 2));
                }
                else
                { //ako nije skok
                    if (!jedanbajt)
                    {
                        s->dodajDvaBajta(pom);
                    }
                    else
                    {
                        s->dodajBajt(pom);
                    }
                }
            }
        }
        else
        { //lokalan, nije definisan
            //obracanje unapred

            if (skok)
            {
                ts->obelezi(si->naziv, s->counter, '+', jedanbajt, true);

                Relocation *rel = new Relocation(locationcounter, "R_386_PC64", *ts->dohvatiSimbol(si->naziv), tsekcija);
                rt->dodaj(rel);
                s->dodajDvaBajta(-2);
            }
            else
            {
                ts->obelezi(si->naziv, s->counter, '+', jedanbajt);

                Relocation *rel = new Relocation(locationcounter, "R_386_64", *ts->dohvatiSimbol(si->naziv), tsekcija);
                rt->dodaj(rel);
                s->dodajDvaBajta(0);
            }
        }
    }
    else
    {
        //jeste globalna
        //if (!ts->isLabelaDefined(*si))
        //  ts->obelezi(si->name, s->counter, '+', jedanbajt);
        if (skok)
        {
            s->dodajDvaBajta(-2);

            Relocation *rel = new Relocation(locationcounter, "R_386_PC64", *ts->dohvatiSimbol(si->naziv), tsekcija);
            rt->dodaj(rel);
        }
        else
        { //globalan i nije skok

            if (!jedanbajt)
            {
                s->dodajDvaBajta(0);

                Relocation *rel = new Relocation(locationcounter, "R_386_64", *ts->dohvatiSimbol(si->naziv), tsekcija);
                rt->dodaj(rel);
            }
            else
            {
                s->dodajBajt(0);

                Relocation *rel = new Relocation(locationcounter, "R_386_64", *ts->dohvatiSimbol(si->naziv), tsekcija);
                rt->dodaj(rel);
            }
        }
        //relok tabela
    }
}

void Tokeni::dodajequ(string token, string proslediime, string znak, bool broj)
{
    if (broj == false)
    {
        Simbol *si = new Simbol(token, tsekcija, "label", locationcounter, "local", 0);
        ts->dodajNoviSimbol(*si);
        to->dodajuObracanje(ts->dohvatiSimbol(token), proslediime, znak);
    }
    else
    {
        regex op_dec{("^([0-9]+)$")};
        regex op_bin{("^0b([0-1]+)$")};
        regex op_hex{("^(0[xX][0-9a-fA-F]+)$")};
        if (regex_match(token, op_dec))
            to->dodajuObracanjeBroj(stoi(token), proslediime, znak);
        else if (regex_match(token, op_bin))
        {
            to->dodajuObracanjeBroj(bintodec(token), proslediime, znak);
        }
        else
        {
            to->dodajuObracanjeBroj(hextodec(token), proslediime, znak);
        }
    }
}

bool Tokeni::dalijejedanbajt(string icode, string token1, string token2, int *n)
{
    regex bukvregistarh{"(r[0-7]h)"};
    regex bukvregistarl{"(r[0-7]l)"};
    regex instrukcijab{"(halt|iret|ret|jmp|jeq|jgt|jne|call|int|push|pop|add|sub|mul|div|cmp|xchg|and|or|xor|not|test|mov|shl|shr)b"};
    regex instrukcijaw{"(halt|iret|ret|jmp|jeq|jgt|jne|call|int|push|pop|add|sub|mul|div|cmp|xchg|and|or|xor|not|test|mov|shl|shr)w"};
    regex instrukcija{"(halt|iret|ret|jmp|jeq|jgt|jne|call|int|push|pop|add|sub|mul|div|cmp|xchg|and|or|xor|not|test|mov|shl|shr)(b|w)?"};
    regex instrukcijaJMPb{"(int|call|jmp|jeq|jne|jgt)b"};
    regex instrukcijaJMP{"(int|call|jmp|jeq|jne|jgt)(b|w)?"};
    regex lzprz{"^([a-zA-Z][a-zA-Z0-9_]*)\\(\\%r[0-7](h|l)?\\)$"};
    regex lzprzh{"^([a-zA-Z][a-zA-Z0-9_]*)\\(\\%r7h\\)$"};
    regex lzprzl{"^([a-zA-Z][a-zA-Z0-9_]*)\\(\\%r7l\\)$"};

    if (regex_match(icode, instrukcijaw) && (regex_search(token1, bukvregistarh) || regex_search(token1, bukvregistarl) || regex_search(token2, bukvregistarh) || regex_search(token2, bukvregistarl)))
    {
        *n = -1;
    }
    if ((regex_match(token1, lzprz) || regex_match(token2, lzprz)) && (regex_match(token1, lzprzh) || regex_match(token1, lzprzl) || regex_match(token2, lzprzh) || regex_match(token2, lzprzl)))
    {
        *n = -2;
    }
    if (regex_match(icode, instrukcijaJMPb))
    {
        *n = -3;
    }
    if (regex_search(token1, bukvregistarh) || regex_search(token1, bukvregistarl) || regex_search(token2, bukvregistarh) || regex_search(token2, bukvregistarl))
    {
        return true;
    }
    else if (regex_match(icode, instrukcijab))
    {
        return true;
    }
    else
        return false;
}
