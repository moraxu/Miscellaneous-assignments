#ifndef OSOBOWY_H_INCLUDED
#define OSOBOWY_H_INCLUDED

#include <string>
#include "Auto.h"

class Osobowy : public Auto
{
    std::string typ_nadwozia;

public:

    Osobowy(std::string nzw = "", int szer = 0, int dlug = 0, std::string rejstr = "",
            std::string typ_nadw = "", const Kierowca& wlasc = Kierowca());

    std::string get_typ_nadwozia() const;
    void set_typ_nadwozia(std::string nowy_typ_nadw);

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

#endif // OSOBOWY_H_INCLUDED
