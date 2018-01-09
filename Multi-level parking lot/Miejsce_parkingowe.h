#ifndef MIEJSCE_PARKINGOWE_H_INCLUDED
#define MIEJSCE_PARKINGOWE_H_INCLUDED

class Auto;     //deklaracje zapowiadające (aby kompilator wiedział, że
class Bilet;    //nazwy Auto i Bilet oznaczają nazwy klas)

class Miejsce_parkingowe
{
    const int dlugosc, szerokosc;
    Auto* pojazd;
    Bilet* bilet;

public:

    Miejsce_parkingowe(const Auto* poj = 0, const Bilet* bil = 0);
    Miejsce_parkingowe(const Miejsce_parkingowe& ob);   //konstruktor kopiujący - potrzebujemy go, ponieważ składniki
                                                        //klasy: "pojazd" oraz "bilet" mogą przechowywać czasem adresy obiektów zaalokowanych
                                                        //dynamicznie, a takich adresów nie należy kopiować, natomiast zrobić kopię
                                                        //obiektów przez nich wskazywanych (inaczej moglibyśmy doprowadzić
                                                        //w pewnych przypadkach do naruszenia ochrony pamięci - tzw. segmentation fault)

    Miejsce_parkingowe& operator=(const Miejsce_parkingowe& ob);    //przeładowany operator przypisania - potrzebujemy go z tego
                                                                    //samego powodu co konstruktora kopiującego

    int get_dlugosc() const;
    int get_szerokosc() const;

    const Auto* get_pojazd() const;
    const Bilet* get_bilet() const;

    void ustaw_pojazd_bilet(const Auto* poj, const Bilet* bil); //pojazd i bilet musimy ustawić razem, tak aby oba były
    bool usun_pojazd_bilet();                                   //jednocześnie ustawione, usuwamy je też razem

    bool czy_zmiesci_sie_auto(const Auto& poj) const;

    ~Miejsce_parkingowe();
};

#endif // MIEJSCE_PARKINGOWE_H_INCLUDED
