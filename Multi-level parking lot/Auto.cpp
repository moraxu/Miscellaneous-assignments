#include "Auto.h"

using namespace std;

Auto::Auto(string nzw, int szer, int dlug, string rejstr, const Kierowca& wlasc)
            : szerokosc(szer), dlugosc(dlug), nazwa(nzw), wlasciciel(wlasc), rejestracja(rejstr)
{

}

Kierowca Auto::get_wlasciciel() const
{
    return wlasciciel;
}

void Auto::set_wlasciciel(const Kierowca& wlasc)
{
    wlasciciel = wlasc;
}

int Auto::get_szerokosc() const
{
    return szerokosc;
}

void Auto::set_szerokosc(int nowa_szer)
{
    szerokosc = nowa_szer;
}

int Auto::get_dlugosc() const
{
    return dlugosc;
}

void Auto::set_dlugosc(int nowa_dlug)
{
    dlugosc = nowa_dlug;
}

string Auto::get_rejestracja() const
{
    return rejestracja;
}

void Auto::set_rejestracja(string rejestr)
{
    rejestracja = rejestr;
}

string Auto::get_nazwa() const
{
    return nazwa;
}

void Auto::set_nazwa(string nowa_nazwa)
{
    nazwa = nowa_nazwa;
}

//Przy definicji funkcji wirtualnych poza ciałem klasy pomijamy słowo kluczowe "virtual"
Auto::~Auto()
{
    //puste ciało wirtualnego destruktora
}
