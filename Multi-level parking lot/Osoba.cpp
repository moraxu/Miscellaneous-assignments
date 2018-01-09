#include "Osoba.h"

using namespace std;

Osoba::Osoba(string i_n)   : imie_nazwisko(i_n)
{

}

string Osoba::get_imie_nazwisko() const
{
    return imie_nazwisko;
}

void Osoba::set_imie_nazwisko(string i_n)
{
    imie_nazwisko = i_n;
}

ostream& operator<<(ostream& str_wy, const Osoba& ob)
{
    str_wy << "Nazywam sie " << ob.get_imie_nazwisko() << ".";

    return str_wy;
}
