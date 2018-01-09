#include <iostream>
#include <string>
#include <iomanip>      //dla manipulatorów z parametrem
#include <vector>
#include <algorithm>    //dla funkcji find
#include <limits>       //dla klasy szablonowej numeric_limits
#include <ctime>
#include <cstdlib>

#include "Kierowca.h"
#include "Auto.h"
#include "Osobowy.h"
#include "Ciezarowy.h"
#include "Bus.h"
#include "Miejsce_parkingowe.h"
#include "Parking.h"

using namespace std;

//--------------------------------------------------------------------------------------------------------------------------------------
void zlicz_wystapienia_pojazdow(const vector< vector<Miejsce_parkingowe> >& wektor)
{
    //funkcja zliczająca wystąpienia poszczególnych typów pojazdów w całym parkingu wielopoziomowym (wektor),
    //korzystamu tu z tego, że klasa Auto (klasa bazowa dla innych klas pochodnych pojazdów) jest typem
    //polimorficznym, a więc posiadającym co najmniej 1 funkcję wirtualną, dzięki temu możemy
    //skorzystać z operatora rzutowania dynamic_cast działającym w czasie wykonywania się programu
    const Auto* poj;

    int ile_osobowych = 0, ile_ciezarowych = 0, ile_busow = 0;

    for(unsigned nr_poziomu = 0 ; nr_poziomu < wektor.size() ; ++nr_poziomu)
    {
        for(unsigned nr_miejsca = 0 ; nr_miejsca < wektor[nr_poziomu].size() ; ++nr_miejsca)
        {
            poj = wektor[nr_poziomu][nr_miejsca].get_pojazd();

            if(poj) //jeżeli miejsce jest zajęte, czyli jest tam jakieś auto...
            {
                //...to korzystamy z tzw. operatora rzutowania dynamic_cast,
                //w jego nawiasach kwadratowych znajduje się nazwa klasy
                //pochodnej na stały wskaźnik na którą rzutujemy stały wskaźnik "poj" typu const Auto*,
                //który tak naprawdę przechowuje adres jednego z obiektu klasy pochodnej od Auto

                //jeżeli rzutowanie się powiedzie (a więc np. w poniższym przypadku wskaźnik "poj"
                //będzie pokazywał na obiekt typu Osobowy), to operator ten zwróci odpowiedni adres
                //obiektu typu, na który rzutowaliśmy, jeśli rzutowanie się nie powiedzie
                //(a więc np. w poniższym przypadku wskaźnik "poj" NIE BĘDZIE pokazywał na obiekt typu Osobowy),
                //to operator zwróci adres zerowy 0

                if(dynamic_cast<const Osobowy*>(poj))
                {
                    ++ile_osobowych;
                }
                else if(dynamic_cast<const Ciezarowy*>(poj))
                {
                    ++ile_ciezarowych;
                }
                else if(dynamic_cast<const Bus*>(poj))  //(tak naprawdę to tutaj wystarczyłoby samo "else"
                {                                       //bo i tak pozostała nam już tylko ta jedna opcja)
                    ++ile_busow;
                }
            }
        }
    }

    cout << "\nNa parkingu wielopoziomowym jest:\n"
         << "\t-" << ile_osobowych << " samochodow osobowych,\n"
         << "\t-" << ile_ciezarowych << " samochodow ciezarowych,\n"
         << "\t-" << ile_busow << " busow.\n\n";

    //funkcję tą zdefiniowaliśmy jako zwykłą funkcję, a nie składową klasy Parking, ponieważ
    //typów samochodów może w przyszłości być więcej, a czasem nie mamy możliwości dostępu
    //do wewnętrznej implementacji danej klasy (tak często bywa w realnym programowaniu,
    //więc chodzi o dobry styl programowania, tutaj moglibyśmy równie dobrze uczynić tę funkcję
    //funkcją składową wspomnianej klasy) aby rozszerzyć zachowania jej funkcji składowych
}
//--------------------------------------------------------------------------------------------------------------------------------------

int main()
{
    srand(time(0)); //inicjalizujemy generator liczb pseudo-losowych

    cout << "Podaj swoje imie i nazwisko (oddzielajac je spacja) >>> ";
    string imie_nazwisko;
    getline(cin, imie_nazwisko);

    int ile_pkt_karnych;

    while(true) //pętla wczytująca do skutku ilość pkt karnych (pojawia się później w dalszej części funkcji main
                //w celu wczytania poprawnej wartości liczbowej - czyli zareagowania na błędne sytuacje, w których
                //user poda np. literę lub napis zamiast jednej liczby)
    {
        cout << "Podaj ilosc Twoich punktow karnych >>> ";
        cin >> ile_pkt_karnych;

        if(cin.fail())  //jeśli user poda coś, co nie jest liczbą (np. literę) i strumień będzie zawierał tzw. flagę błędu...
        {
            cin.clear();    //...to czyścimy tą flagę
            cin.ignore(numeric_limits<streamsize>::max(), '\n');    //i ignorujemy (odrzucamy) błędne dane wejściowe zalegające w strumieniu
            //(numeric_limits<streamsize>::max() to specjalna wartość przekazywana jako pierwszy argument do funkcji ignore,
            //który oznacza ilość znaków do zignorowania (wyjęcia ze strumienia wejściowego), wartość ta oznacza: "zignoruj
            //WSZYSTKIE zalegające w nim znaki aż do napotkania znaku nowej linii '\n' (drugi argument funkcji ignore)")

            cout << "\n\t\tNiepoprawna liczba!\n\n";
        }
        else
        {
            break;
        }
    }

    Kierowca user(imie_nazwisko, ile_pkt_karnych);

    cout << "\n" << user << "\n";   //testujemy przeładowany operator<< na rzecz obiektu typu Kierowca

    cout << "\nRozpoczynamy losowanie poszczegolnych wymiarow aut (z zakresu 5-25)...\n";

    int szerokosc_osobowego = rand() % 21 + 5,
        dlugosc_osobowego = rand() % 21 + 5,
        szerokosc_ciezarowego = rand() % 21 + 5,
        dlugosc_ciezarowego = rand() % 21 + 5,
        szerokosc_busa = rand() % 21 + 5,
        dlugosc_busa = rand() % 21 + 5;

    //manipulator setw(2) wypisuje następną liczbę na conajmniej dwóch znakach, poprzedzając niepotrzebne spacją,
    //dzięki temu uzyskujemy ładne wyrównanie do kolumn przy wypisywaniu liczb
    cout << "\n\tWymiary osobowych:   [szerokosc = " << setw(2) << szerokosc_osobowego << "][dlugosc = " << setw(2) << dlugosc_osobowego << "]"
         << "\n\tWymiary ciezarowych: [szerokosc = " << setw(2) << szerokosc_ciezarowego << "][dlugosc = " << setw(2) << dlugosc_ciezarowego << "]"
         << "\n\tWymiary busow:       [szerokosc = " << setw(2) << szerokosc_busa << "][dlugosc = " << setw(2) << dlugosc_busa << "]\n\n";


    unsigned ilosc_poziomow, ilosc_miejsc;

    //(user wprowadza liczby naturalne dodatnie)
    while(true)
    {
        cout << "Wprowadz ilosc poziomow parkingu >>> ";
        cin >> ilosc_poziomow;

        if(cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n\t\tNiepoprawna liczba!\n\n";
        }
        else
        {
            break;
        }
    }

    while(true)
    {
        cout << "Wprowadz ilosc miejsc na kazdym poziomie >>> ";
        cin >> ilosc_miejsc;

        if(cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n\t\tNiepoprawna liczba!\n\n";
        }
        else
        {
            break;
        }
    }

    Parking nasz_parking(ilosc_poziomow, ilosc_miejsc);

    //drukujemy wymiary parkingu korzystając z fukcji składowej zwroc_wektor()
    cout << "\n\tUtworzono parking o wymiarach "
         << nasz_parking.zwroc_wektor().size() << "x" << nasz_parking.zwroc_wektor()[0].size() << '\n';

    //------------------------------------------------------------------------------------
    //------------------------------------------------------------------------------------
    //------------------------------------------------------------------------------------
    //Interakcja z użytkownikiem:

    vector<string> zbior_rejestracji;   //w tym wektorze będziemy przechowywać wszystkie rejestracje,
                                        //aby wychwycić przypadki dodania przez użytkownika 2-gi raz
                                        //tej samej rejestracji
    int opcja;

    while(true) //w tej pętli użytkownik będzie podejmował decyzje, wjeżdział nowo tworzonymi
    {           //autami na parking, wyjeżdżał z niego itp.

        cout << "\n------------------------------------------------------\n"
             << "MENU:\n\n"
             << "\t1. Wjedz na parking\n"
             << "\t2. Wyjedz z parkingu\n"
             << "\t3. Zlicz rodzaje aut na calym parkingu\n"
             << "\t4. Zakoncz program\n\n\n"
             << "\tTwoj wybor >>> ";

        cin >> opcja;

        if(cin.fail() || opcja < 1 || opcja > 4)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n\t\tNiepoprawny nr opcji!\n\n";
        }
        else if(opcja == 1)
        {
            int oplata_za_godzine = 5;  //opłata zawsze będzie stała

            //===============================================================================================================================
            //Menu wybierające poziom:

            unsigned na_ktory_poziom;
            while(true)
            {
                cout << "\n\tNa ktory poziom chcesz wjechac (1-" << ilosc_poziomow << ") >>> ";
                cin >> na_ktory_poziom;

                if(cin.fail() || na_ktory_poziom < 1 || na_ktory_poziom > ilosc_poziomow)
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "\n\t\t\tNiepoprawny nr poziomu!\n";
                }
                else
                {
                    break;
                }
            }
            //===============================================================================================================================

            //###############################################################################################################################
            //Menu tworzące auto:

            int nr_auta;
            cout << "\n\t\tDostepne auta:\n"
                 << "\t\t1. Utworz samochod osobowy\n"
                 << "\t\t2. Utworz samochod ciezarowy\n"
                 << "\t\t3. Utworz bus\n\n";

            while(true)
            {
                cout << "\t\tTwoj wybor >>> ";
                cin >> nr_auta;

                if(cin.fail() || nr_auta < 1 || nr_auta > 3)
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "\n\t\t\tNiepoprawny nr opcji!\n\n";
                }
                else
                {
                    break;
                }
            }
            cin.ignore();   //usuwamy niewczytany znak nowej linii, ponieważ następną
                            //operacją wejścia pokieruje funkcja getline

            string rejestracja, marka;  //rejestracja i marka są wspólne dla wszystkich typów pojazdów w naszym programie
                                        //(należą do klasy bazowej Auto)

            //-----------------------------------------------------------------------------------
            //Pętla sprawdzająca czy nie podaliśmy przez przypadek istniejącej rejestracji:
            while(true)
            {
                cout << "\n\tWprowadz rejestracje auta (np. WA34LE2) >>> ";
                getline(cin, rejestracja);

                if(find(zbior_rejestracji.begin(), zbior_rejestracji.end(), rejestracja) != zbior_rejestracji.end())
                    cout << "\n\t\t\tRejestracja juz istnieje!\n";
                else
                    break;
            }
            //-----------------------------------------------------------------------------------

            cout << "\n\tWprowadz nazwe auta (np. Renault Clio) >>> ";
            getline(cin, marka);

            if(nr_auta == 1)    //<---- Konstrukcja samochodu osobowego
            {
                string typ_nadwozia;

                cout << "\n\tWprowadz typ nadwozia (np. kombi) >>> ";
                getline(cin, typ_nadwozia);

                Osobowy sam_osobowy(marka, szerokosc_osobowego, dlugosc_osobowego, rejestracja,
                                    typ_nadwozia, user);    //konstruujemy samochód osobowy

                if(nasz_parking.wjedz_na_poziom(na_ktory_poziom-1, sam_osobowy, oplata_za_godzine))    //jeżeli pomyślnie zaparkowaliśmy
                {
                    zbior_rejestracji.push_back(rejestracja);   //to dodajemy rejestrację do naszego wektora
                }
            }
            else if(nr_auta == 2)   //<---- Konstrukcja samochodu ciężarowego
            {
                string zawartosc;
                int ile_przyczep;

                cout << "\n\tWprowadz przewozona zawartosc (np. Coca Cola) >>> ";
                getline(cin, zawartosc);

                while(true)
                {
                    cout << "\n\tWprowadz liczbe przyczep ciezarowki >>> ";
                    cin >> ile_przyczep;

                    if(cin.fail())
                    {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "\n\t\tNiepoprawna liczba!\n";
                    }
                    else
                    {
                        break;
                    }
                }

                Ciezarowy sam_ciezarowy(marka, szerokosc_osobowego, dlugosc_osobowego, rejestracja,
                                        zawartosc, ile_przyczep, user);    //konstruujemy samochód ciężarowy

                if(nasz_parking.wjedz_na_poziom(na_ktory_poziom-1, sam_ciezarowy, oplata_za_godzine))    //jeżeli pomyślnie zaparkowaliśmy
                {
                    zbior_rejestracji.push_back(rejestracja);   //to dodajemy rejestrację do naszego wektora
                }
            }
            else //czyli: if(nr_auta == 3)  //<---- Konstrukcja busa
            {
                int ile_bagaznikow;

                while(true)
                {
                    cout << "\n\tWprowadz liczbe bagaznikow w busie >>> ";
                    cin >> ile_bagaznikow;

                    if(cin.fail())
                    {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "\n\t\tNiepoprawna liczba!\n";
                    }
                    else
                    {
                        break;
                    }
                }

                Bus autobus(marka, szerokosc_osobowego, dlugosc_osobowego, rejestracja,
                            ile_bagaznikow, user);    //konstruujemy bus

                if(nasz_parking.wjedz_na_poziom(na_ktory_poziom-1, autobus, oplata_za_godzine))    //jeżeli pomyślnie zaparkowaliśmy
                {
                    zbior_rejestracji.push_back(rejestracja);   //to dodajemy rejestrację do naszego wektora
                }
            }
            //###############################################################################################################################
        }
        else if(opcja == 2)
        {
            cin.ignore();   //usuwamy niewczytany znak nowej linii, ponieważ następną
                            //operacją wejścia pokieruje funkcja getline

            cout << "\n\tWprowadz rejestracje pojazdu >>> ";
            string rejestracja;
            getline(cin, rejestracja);

            if(nasz_parking.wyjedz(rejestracja))    //jeżeli znaleziono rejestrację na parkingu
            {
                //to usuwamy ją także z naszego wektora rejestracji:
                zbior_rejestracji.erase(find(zbior_rejestracji.begin(), zbior_rejestracji.end(), rejestracja));
            }
        }
        else if(opcja == 3)
        {
            zlicz_wystapienia_pojazdow(nasz_parking.zwroc_wektor());
        }
        else //czyli: if(opcja == 4)
        {
            return EXIT_SUCCESS;
        }
    }
}
