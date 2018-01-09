#ifndef KIEROWCA_H_INCLUDED
#define KIEROWCA_H_INCLUDED

#include <iostream>
#include <string>
#include "Osoba.h"

class Kierowca : public Osoba
{
    int ile_pkt_karnych;

public:

    Kierowca(std::string i_n = "", int pkt_karne = 0);

    int get_ile_pkt_karnych() const;
    void set_ile_pkt_karnych(int pkt_karne);
};

std::ostream& operator<<(std::ostream& str_wy, const Kierowca& ob); //umieszczamy tu deklarację przeładowanego operatora przypisania,
                                                                    //aby pliki włączające ten nagłówek wiedziały, że taki istnieje dla tej klasy

#endif // KIEROWCA_H_INCLUDED
