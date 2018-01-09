#ifndef OSOBA_H_INCLUDED
#define OSOBA_H_INCLUDED

#include <iostream>
#include <string>

class Osoba
{
    std::string imie_nazwisko;

public:

    Osoba(std::string i_n = "");

    std::string get_imie_nazwisko() const;
    void set_imie_nazwisko(std::string i_n);
};

std::ostream& operator<<(std::ostream& str_wy, const Osoba& ob);    //umieszczamy tu deklarację przeładowanego operatora przypisania,
                                                                    //aby pliki włączające ten nagłówek wiedziały, że taki istnieje dla tej klasy

#endif // OSOBA_H_INCLUDED
