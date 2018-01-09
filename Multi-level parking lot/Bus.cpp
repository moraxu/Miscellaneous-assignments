#include <iostream>
#include "Bus.h"

using namespace std;

Bus::Bus(string nzw, int szer, int dlug, string rejstr,
         int ilosc_bagaz, const Kierowca& wlasc)
            : Auto(nzw, szer, dlug, rejstr, wlasc),
              ilosc_bagaznikow(ilosc_bagaz)

{

}

int Bus::get_ilosc_bagaznikow() const
{
    return ilosc_bagaznikow;
}

void Bus::set_ilosc_bagaznikow(int ilosc_bagaz)
{
    ilosc_bagaznikow = ilosc_bagaz;
}

//Przy definicji funkcji wirtualnych poza ciałem klasy pomijamy słowo kluczowe "virtual"
void Bus::przedstaw_sie() const
{
    cout << "[Jestem busem marki " << get_nazwa() << ",\nmoja rejestracja: "
         << get_rejestracja() << ", mam: " << ilosc_bagaznikow << " bagaznikow.]\n";
}

void Bus::zapal_silnik() const
{
    cout << "[Bus marki " << get_nazwa();

    if(!get_wlasciciel().get_imie_nazwisko().empty())    //jeżeli samochód ma właściciela to drukujemy jego imie i nazwisko
    {
        cout << "\nnalezacy do: " << get_wlasciciel().get_imie_nazwisko();
    }

    cout << " odpala silnik!]\n";
}

void Bus::zgas_silnik() const
{
    cout << "[Bus marki " << get_nazwa();

    if(!get_wlasciciel().get_imie_nazwisko().empty())    //jeżeli samochód ma właściciela to drukujemy jego imie i nazwisko
    {
        cout << "\nnalezacy do: " << get_wlasciciel().get_imie_nazwisko();
    }

    cout << " gasi silnik!]\n";
}

void Bus::przyspiesz() const
{
    cout << "[Bus marki " << get_nazwa() << " przyspiesza!]\n";
}

void Bus::zwolnij() const
{
    cout << "[Bus marki " << get_nazwa() << " zwalnia!]\n";
}

Auto* Bus::virt_konstr() const
{
    return new Bus;
}

Auto* Bus::virt_konstr_kop() const
{
    return new Bus(*this);  //do pracy startuje konstruktor kopiujący klasy Bus, który
                            //zostaje wygenerowany automatycznie, a dodatkowo wywołuje on
                            //konstruktor kopiujący z klasy bazowej Auto, który też zostaje
                            //wygenerowany automatycznie przez kompilator
}
