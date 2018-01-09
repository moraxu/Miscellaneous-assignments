#ifndef BUS_H_INCLUDED
#define BUS_H_INCLUDED

#include <string>
#include "Auto.h"

class Bus : public Auto
{
    int ilosc_bagaznikow;

public:

    Bus(std::string nzw = "", int szer = 0, int dlug = 0, std::string rejstr = "",
        int ilosc_bagaz = 0, const Kierowca& wlasc = Kierowca());

    int get_ilosc_bagaznikow() const;
    void set_ilosc_bagaznikow(int ilosc_bagaz);

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

#endif // BUS_H_INCLUDED
