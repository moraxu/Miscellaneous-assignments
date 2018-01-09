#ifndef PARKING_H_INCLUDED
#define PARKING_H_INCLUDED

#include <vector>
#include <string>
#include "Miejsce_parkingowe.h"

class Auto;
class Bilet;

class Parking
{
    std::vector< std::vector<Miejsce_parkingowe> > wektor;  //wektor wektorów typu vector<Miejsce_parkingowe>

public:

    Parking(unsigned ilosc_poziomow, unsigned ilosc_miejsc);

    const std::vector< std::vector<Miejsce_parkingowe> >& zwroc_wektor() const;   //zwraca referencję do stałego wektora

    bool wjedz_na_poziom(unsigned nr_poziomu, const Auto& poj, int oplata);
    bool wyjedz(std::string rejestracja);
};

#endif // PARKING_H_INCLUDED
