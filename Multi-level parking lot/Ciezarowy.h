#ifndef CIEZAROWY_H_INCLUDED
#define CIEZAROWY_H_INCLUDED

#include <string>
#include "Auto.h"

class Ciezarowy : public Auto
{
    int ilosc_przyczep;
    std::string zawartosc;

public:

    Ciezarowy(std::string nzw = "", int szer = 0, int dlug = 0, std::string rejstr = "",
              std::string zawart = "", int przyczepy = 0, const Kierowca& wlasc = Kierowca());

    int get_ilosc_przyczep() const;
    void set_ilosc_przyczep(int ilosc_prz);

    std::string get_zawartosc() const;
    void set_zawartosc(std::string zawart);

    //Deklarujemy własne wersje funkcji wirtualnych odziedziczonych
    //od klasy Auto, tym razem nie są one już czysto wirtualne (brakuje "= 0")
    virtual void przedstaw_sie() const;
    virtual void zapal_silnik() const;
    virtual void zgas_silnik() const;
    virtual void przyspiesz() const;
    virtual void zwolnij() const;

    virtual Auto* virt_konstr() const;
    virtual Auto* virt_konstr_kop() const;
};

#endif // CIEZAROWY_H_INCLUDED
