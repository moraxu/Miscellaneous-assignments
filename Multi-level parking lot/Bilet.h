#ifndef BILET_H_INCLUDED
#define BILET_H_INCLUDED

#include <iostream>
#include <string>
#include <ctime>    //dla funkcji obsługujących czas

class Bilet
{
    const std::time_t czas_wjazdu;
    const int oplata_za_godz;
    const std::string nr_stanowiska;

    std::string zlacz_nr_stanowiska(unsigned poziom, unsigned miejsce) const;

public:

    Bilet(int oplata, unsigned poziom, unsigned miejsce);

    std::string get_czas_wjazdu() const;

    int get_oplata_za_godz() const;

    double aktualna_naleznosc() const;

    std::string get_nr_stanowiska() const;
};

std::ostream& operator<<(std::ostream& str_wy, const Bilet& ob);    //umieszczamy tu deklarację przeładowanego operatora przypisania,
                                                                    //aby pliki włączające ten nagłówek wiedziały, że taki istnieje dla tej klasy

#endif // BILET_H_INCLUDED
