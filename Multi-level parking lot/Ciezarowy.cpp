#include <iostream>
#include "Ciezarowy.h"

using namespace std;

Ciezarowy::Ciezarowy(string nzw, int szer, int dlug, string rejstr,
                     string zawart, int przyczepy, const Kierowca& wlasc)
                        : Auto(nzw, szer, dlug, rejstr, wlasc),
                          ilosc_przyczep(przyczepy), zawartosc(zawart)

{

}

int Ciezarowy::get_ilosc_przyczep() const
{
    return ilosc_przyczep;
}

void Ciezarowy::set_ilosc_przyczep(int ilosc_prz)
{
    ilosc_przyczep = ilosc_prz;
}

string Ciezarowy::get_zawartosc() const
{
    return zawartosc;
}

void Ciezarowy::set_zawartosc(string zawart)
{
    zawartosc = zawart;
}

//Przy definicji funkcji wirtualnych poza ciałem klasy pomijamy słowo kluczowe "virtual"
void Ciezarowy::przedstaw_sie() const
{
    cout << "[Jestem ciezarowka marki " << get_nazwa() << ",\nmoja rejestracja: "
         << get_rejestracja() << ", przewoze: " << zawartosc << ".]\n";
}

void Ciezarowy::zapal_silnik() const
{
    cout << "[Ciezarowka marki " << get_nazwa();

    if(!get_wlasciciel().get_imie_nazwisko().empty())    //jeżeli samochód ma właściciela to drukujemy jego imie i nazwisko
    {
        cout << "\nnalezaca do: " << get_wlasciciel().get_imie_nazwisko();
    }

    cout << " odpala silnik!]\n";
}

void Ciezarowy::zgas_silnik() const
{
    cout << "[Ciezarowka marki " << get_nazwa();

    if(!get_wlasciciel().get_imie_nazwisko().empty())    //jeżeli samochód ma właściciela to drukujemy jego imie i nazwisko
    {
        cout << "\nnalezaca do: " << get_wlasciciel().get_imie_nazwisko();
    }

    cout << " gasi silnik!]\n";
}

void Ciezarowy::przyspiesz() const
{
    cout << "[Ciezarowka marki " << get_nazwa() << " przyspiesza!]\n";
}

void Ciezarowy::zwolnij() const
{
    cout << "[Ciezarowka marki " << get_nazwa() << " zwalnia!]\n";
}


Auto* Ciezarowy::virt_konstr() const
{
    return new Ciezarowy;
}

Auto* Ciezarowy::virt_konstr_kop() const
{
    return new Ciezarowy(*this);    //do pracy startuje konstruktor kopiujący klasy Ciezarowy, który
                                    //zostaje wygenerowany automatycznie, a dodatkowo wywołuje on
                                    //konstruktor kopiujący z klasy bazowej Auto, który też zostaje
                                    //wygenerowany automatycznie przez kompilator
}
