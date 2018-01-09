#include <iostream>
#include "Osobowy.h"

using namespace std;

Osobowy::Osobowy(string nzw, int szer, int dlug, string rejstr,
                 string typ_nadw, const Kierowca& wlasc)
                 : Auto(nzw, szer, dlug, rejstr, wlasc),
                   typ_nadwozia(typ_nadw)

{

}

string Osobowy::get_typ_nadwozia() const
{
    return typ_nadwozia;
}

void Osobowy::set_typ_nadwozia(string nowy_typ_nadw)
{
    typ_nadwozia = nowy_typ_nadw;
}

//Przy definicji funkcji wirtualnych poza ciałem klasy pomijamy słowo kluczowe "virtual"
void Osobowy::przedstaw_sie() const
{
    cout << "[Jestem samochodem osobowym marki " << get_nazwa() << ",\nmoja rejestracja: "
         << get_rejestracja() << ", typ nadwozia: " << typ_nadwozia << ".]\n";
}

void Osobowy::zapal_silnik() const
{
    cout << "[Samochod osobowy marki " << get_nazwa();

    if(!get_wlasciciel().get_imie_nazwisko().empty())    //jeżeli samochód ma właściciela to drukujemy jego imie i nazwisko
    {
        cout << "\nnalezacy do: " << get_wlasciciel().get_imie_nazwisko();
    }

    cout << " odpala silnik!]\n";
}

void Osobowy::zgas_silnik() const
{
    cout << "[Samochod osobowy marki " << get_nazwa();

    if(!get_wlasciciel().get_imie_nazwisko().empty())    //jeżeli samochód ma właściciela to drukujemy jego imie i nazwisko
    {
        cout << "\nnalezacy do: " << get_wlasciciel().get_imie_nazwisko();
    }

    cout << " gasi silnik!]\n";
}

void Osobowy::przyspiesz() const
{
    cout << "[Samochod osobowy marki " << get_nazwa() << " przyspiesza!]\n";
}

void Osobowy::zwolnij() const
{
    cout << "[Samochod osobowy marki " << get_nazwa() << " zwalnia!]\n";
}

Auto* Osobowy::virt_konstr() const
{
    return new Osobowy;
}

Auto* Osobowy::virt_konstr_kop() const
{
    return new Osobowy(*this);  //do pracy startuje konstruktor kopiujący klasy Osobowy, który
                                //zostaje wygenerowany automatycznie, a dodatkowo wywołuje on
                                //konstruktor kopiujący z klasy bazowej Auto, który też zostaje
                                //wygenerowany automatycznie przez kompilator
}
