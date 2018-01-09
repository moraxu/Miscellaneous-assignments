#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

//-----------------------------------------------------------
//Deklaracje funkcji:
bool czy_spacja(char znak);
bool czy_poprawna_data(const string& data);
void wprowadzanie_daty(string& data);
void otworz_plik(ifstream& str_wej, string& nazwa_pliku);
void wydrukuj_statystyki(ifstream& str_wej, const string& data_poczatkowa, const string& data_koncowa);
//-----------------------------------------------------------
int main()
{
    //-----------------------------------------------------------
    //Na poczatku user podaje nazwe pliku, oraz zakres dat

    string nazwa_pliku;
    ifstream str_wej;

    otworz_plik(str_wej, nazwa_pliku);

    string data_poczatkowa, data_koncowa;

    cout << "\n\t1) DATA POCZATKOWA:\n\n";
    wprowadzanie_daty(data_poczatkowa);

    cout << "\n\t2) DATA KONCOWA:\n\n";
    wprowadzanie_daty(data_koncowa);

    //-----------------------------------------------------------
    //Czesc wlasciwa programu czyli przetwarzanie pliku

    wydrukuj_statystyki(str_wej, data_poczatkowa, data_koncowa);

    return EXIT_SUCCESS;
}
//------------------------------------------------------------------------------------------------------------------
bool czy_spacja(char znak)  //funkcja sprawdzajaca czy dany znak jest spacja, jej adres przyda nam sie pozniej
{
    return (znak == ' ');
}
//------------------------------------------------------------------------------------------------------------------
bool czy_poprawna_data(const string& data)  //funkcja sprawdzajaca czy podana data jest poprawna
{
    const int dni_w_miesiacu[12] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    //tablica, w ktorej zapisano ile kazdy miesiac ma dni

    istringstream str_czyt(data);   //strumien czytajacy ze stringu do zmiennych

    int dzien, miesiac, rok, godzina, minuta;
    char temp;

    str_czyt >> rok >> temp >> miesiac >> temp >> dzien >> godzina >> temp >> minuta;

    if(miesiac > 0 && miesiac < 13 &&
       dzien > 0 && dzien <= dni_w_miesiacu[miesiac-1] &&
       godzina >= 0 && godzina < 24 &&
       minuta >= 0 && minuta < 60)    //jezeli format daty i godziny jest prawidlowy...
    {
        return true; //...to zwracamy wartosc true
    }
    else
    {
        return false;
    }
}
//------------------------------------------------------------------------------------------------------------------
void wprowadzanie_daty(string& data)
{
    while(true) //petla wykonuje sie dopoki az wprowadzimy poprawna date
    {
        cout << "Wprowadz date w formacie RRRR-MM-DD GG:MM >>> ";
        getline(cin, data, '\n');

        if(!czy_poprawna_data(data))
        {
            cout << "\tNiepoprawny format daty!\n";
        }
        else
        {
            return;
        }
    }
}
//------------------------------------------------------------------------------------------------------------------
void otworz_plik(ifstream& str_wej, string& nazwa_pliku)
{
    while(true) //petla wykonuje sie dopoki az wprowadzimy poprawna sciezke do pliku
    {
        cout << "Wprowadz nazwe pliku do wczytania >>> ";
        getline(cin, nazwa_pliku, '\n');

        str_wej.open(nazwa_pliku.c_str());

        if(!str_wej.is_open())
        {
            cout << "\tNiepoprawna nazwa pliku!\n";
        }
        else
        {
            return;
        }
    }
}
//------------------------------------------------------------------------------------------------------------------
void wydrukuj_statystyki(ifstream& str_wej, const string& data_poczatkowa, const string& data_koncowa)
{
    unsigned long zmodyfikowane_katalogi = 0,
                  zmodyfikowane_pliki = 0,
                  laczna_liczba_bajtow = 0;

    while(str_wej.peek() != EOF)    //czytamy z pliku dopoki ostatni znak nie jest koncem pliku
    {
        string temp, rok_miesiac_dzien;
        str_wej >> rok_miesiac_dzien;

        if(rok_miesiac_dzien.find('-') == string::npos) //jezeli pierwszy wyraz wczytany z pliku nie zawiera "-" to nie jest to data...
        {
            getline(str_wej, temp, '\n'); //...a wiec ignorujemy dalsza linie, i przechodzimy do nastepnej (jesli taka jest)
        }
        else
        {
            string godzina_minuta;
            str_wej >> godzina_minuta;

            string akt_data = rok_miesiac_dzien+' '+godzina_minuta;

            if(akt_data < data_poczatkowa || akt_data > data_koncowa)   //porownanie leksykograficzne (slownikowe) czy
                                                                        //aktualna data znajduje sie w zadanym przedziale
            {
                getline(str_wej, temp, '\n');   //...jesli sie nie znajduje, to podobnie jak powyzej, przechodzimy do nast. linii
            }
            else
            {
                char znak;
                str_wej >> znak;

                if(znak == '<') //jesli nast. znak po dacie i godzinie to "<", oznacza to, ze plik jest katalogiem
                {
                    ++zmodyfikowane_katalogi;       //w takim przypadku inkrementujemy odpowiedni licznik
                    getline(str_wej, temp, '\n');   //i pomijamy reszte aktualnie wczytywanej linii
                }
                else
                {
                    ++zmodyfikowane_pliki;  //jeśli nast. wczytywany znak nie jest "<"

                    str_wej.putback(znak);  //to odkladamy go z powrotem do strumienia

                    string rozmiar_pliku;
                    getline(str_wej, rozmiar_pliku, '\n');  //wczytujemy pozostala czesc linii do stringu

                    rozmiar_pliku.erase(rozmiar_pliku.find_last_of(' '));   //usuwamy ostatni wyraz (wyraz to ciag liter niezawierajacy spacji),
                                                                            //najczesciej bedzie to po prostu nazwa pliku, ale moze sie tak zdazyc,
                                                                            //ze plik sklada sie z dwoch lub wiecej wyrazow - wtedy program niepoprawnie
                                                                            //obsluzy taki wpis w pliku tekstowym; niestety sposob formatowania wyjscia
                                                                            //z komendy "dir" nie zostal do tego przystosowany

                    rozmiar_pliku.erase(remove_if(rozmiar_pliku.begin(), rozmiar_pliku.end(), czy_spacja), rozmiar_pliku.end());

                    //funkcja odpowiedzialna za usuniecie spacji wewnatrz stringu, tak aby cyfry bezposrednio do siebie przylegaly
                    //i tym samym tworzyly "zwarta" liczbe, uzywa adresu naszej funkcji "czy_spacja", aby sprawdzic czy dany znak
                    //stringu jest znakiem spacji - jesli tak, to usuwa go ze stringu

                    istringstream str_czyt(rozmiar_pliku);  //zapisujemy string przechowujacy liczbe do strumienia czytajacego z obiektu typu string
                    unsigned long bajty;
                    str_czyt >> bajty;  //strumieniem czytajacym zapisujemy wartosc do zmiennej "bajty"

                    laczna_liczba_bajtow += bajty;  //inkrementacja odpowiedniego licznika
                }
            }
        }
    }

    cout << "\n\nLiczba zmodyfikowanych katalogow w podanym przedziale czasowym: " << zmodyfikowane_katalogi
         << "\nLiczba zmodyfikowanych plikow w podanym przedziale czasowym: " << zmodyfikowane_pliki
         << "\nLaczna liczba bajtow zmodyfikowanych plikow w podanym przedziale czasowym: " << laczna_liczba_bajtow << '\n';
}
//------------------------------------------------------------------------------------------------------------------
