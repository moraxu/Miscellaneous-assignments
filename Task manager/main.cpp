#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <limits>
#include <algorithm>
#include <string>
#include <vector>
#include <cstdlib>

using namespace std;

//----------------------------------------------------------------------------------------
class Zadanie   //klasa reprezentująca pojedyncze zdadanie do wykonania
{
public:

    //------------------------------------
    //Dane składowe:
    string tresc, data_terminu, data_wykonania, data_poczatkowa;
    int czas_na_wykonanie, priorytet, procent_postepu;

    //tresc             - treść zadania
    //data_terminu      - data terminu (deadline)
    //data_wykonania    - data, w której spodziewamy się zakończyć zadanie (data_poczatkowa + czas_na_wykonanie)
    //data_poczatkowa   - data dodania (utworzenia) zadania
    //czas_na_wykonanie - czas w dniach potrzebny na wykonanie zadania
    //priorytet         - priorytet zadania (0-10), im większy tym ważniejsze zadanie
    //procent_postepu   - liczba procentowa oznaczająca postęp wykonania zadania (0-100)

    Zadanie(string tresc = "", string data_terminu = "", string data_poczatkowa = "",
            int czas_na_wykonanie = 0, int priorytet = 0, int procent_postepu = 0)  //domyślny konstruktor

            : tresc(tresc), data_terminu(data_terminu), data_poczatkowa(data_poczatkowa),
              czas_na_wykonanie(czas_na_wykonanie), priorytet(priorytet), procent_postepu(procent_postepu)
    {

    }

};
//----------------------------------------------------------------------------------------
bool porownanie_zadan(const Zadanie& pierwsze, const Zadanie& drugie)   //nasza własna funkcja używana do porównywania zadań
{                                                                       //(na samej górze znajdują się zadania o najbliższym deadlinie)
    if(pierwsze.data_terminu < drugie.data_terminu)
    {
        return true;
    }
    else if(pierwsze.data_terminu > drugie.data_terminu)
    {
        return false;
    }
    else /* if(pierwsze.data_terminu == drugie.data_terminu) */ //jeżeli deadliny są takie same, porównujemy priorytety zadań
    {
        if(pierwsze.priorytet >= drugie.priorytet)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}
//----------------------------------------------------------------------------------------
class Organizer //klasa reprezentująca organizer zadań
{
    //------------------------------------
    //Dane składowe:

    const string plik_zadan;    //zmienna przechowująca nazwę pliku z zadaniami

    string dzisiejsza_data;     //zmienna przechowująca dzisiejszą datę

    vector<Zadanie> lista_zadan;    //wektor obiektów typu "Zadanie", czyli nasza lista zadań do wykonania

    //------------------------------------
    //Funkcje składowe:

    void wczytaj_liste_zadan_z_dysku(); //funkcja wczytująca listę zadań z pliku
    bool zapisz_liste_zadan_na_dysk();  //funkcja zapisująca listę zadań do pliku

    void wyswietl_liste_zadan();

    void dodaj_zadanie();

    void aktualizuj_postep();
    void zmien_date_terminu();
    void zmien_priorytet();

    void sortuj_liste_zadan();  //funkcja wywoływana po modyfikacji listy zadań

    void uaktualnij_dzisiejsza_date();

    string wprowadzanie_daty_terminu(); //funkcja odpowiedzialna za pobranie daty terminu od usera, jest ona wywoływana
                                        //w dwóch innych funkcjach składowych, więc poprzez umieszczenie jej ciała
                                        //jednorazowo w osobnej funkcji, oszczędzamy na miejscu

    unsigned wprowadzanie_nr_zadania(); //funkcja odpowiedzialna za pobranie nr zadania od usera na jakim chce on
                                        //pracować, jest ona wywoływana w kilku innych funkcjach składowych (oszczędność miejsca)

public:
    Organizer(string plik_zadan);   //konstruktor obiektu, wymaga on przekazania nazwy pliku z listą zadań
    ~Organizer();   //destruktor obiektu, zapisuje on aktualną listę zadań do pliku

    bool operator()(const Zadanie& zadanie);    //tzw. przeładowany operator (), służy on do sprawdzenia czy
                                                //należy usunąć dane zadanie z listy, jest on potrzebny w ciele
                                                //funkcji składowej "usun_zalegle_zadania"

    string pobierz_dzisiejsza_date();   //funkcja zwracająca daną składową "dzisiejsza_data"

    void wywietl_menu();
};
//----------------------------------------------------------------------------------------
Organizer::Organizer(string plik_zadan) : plik_zadan(plik_zadan)
{
    uaktualnij_dzisiejsza_date();
    wczytaj_liste_zadan_z_dysku();
}
//----------------------------------------------------------------------------------------
Organizer::~Organizer()
{
    if(!zapisz_liste_zadan_na_dysk())
    {
        cout << "\tNastapilo niepowodzenie przy zapisywaniu listy zadan do pliku!";
    }
}
//----------------------------------------------------------------------------------------
void Organizer::wczytaj_liste_zadan_z_dysku()
{
    fstream str_czytajacy(plik_zadan.c_str(), ios::out | ios::in | ios::app);   //definicja strumienia mogącego zarówno zapisywać do i czytać
                                                                                //z pliku, tutaj będzie on tylko czytał, ale potrzebujemy
                                                                                //takiego rodzaju strumienia, aby przekazać do niego flagi
                                                                                //out, in i app - oznaczają one, że jeśli taki plik istnieje,
                                                                                //to zachowywana jest jego dotychczasowa zawartość, a jeśli
                                                                                //pliku nie ma na dysku, to jest on w tym momencie tworzony

    if(!str_czytajacy.is_open())    //kończymy program jeśli nie udało się otworzyć dostępu do pliku
    {                               //(np. nie mamy potrzebnych do tego uprawnień systemowych)

        cout << "\tNastapilo niepowodzenie przy otwieraniu pliku z lista zadan!";
        exit(EXIT_FAILURE);
    }

    while(str_czytajacy.peek() != EOF)  //dopóki nie napotkaliśmy końca pliku...
    {
        //...to czytamy z niego informacje o zadaniach, które są przechowywane w następującym formacie:
        //
        //TRESC_ZADANIA_1
        //DATA_POCZATKOWA_1 DATA_TERMINU_1 DATA_WYKONANIA_1 CZAS_NA_WYKONANIE_1 PRIORYTET_1 PROCENT_POSTEPU_1
        //TRESC_ZADANIA_2
        //DATA_POCZATKOWA_2 DATA_TERMINU_2 DATA_WYKONANIA_1 CZAS_NA_WYKONANIE_2 PRIORYTET_2 PROCENT_POSTEPU_2
        //...itd.

        Zadanie do_dodania;

        getline(str_czytajacy, do_dodania.tresc);       //wczytuje treść zadania (może zawierać spacje) aż do napotkania znaku nowej linii
        str_czytajacy >> do_dodania.data_poczatkowa     //wczytywanie pozostałych pól
                      >> do_dodania.data_terminu
                      >> do_dodania.data_wykonania
                      >> do_dodania.czas_na_wykonanie
                      >> do_dodania.priorytet
                      >> do_dodania.procent_postepu;

        str_czytajacy.ignore(); //ingorujemy znak nowej linii umieszczany po każdym wierszu w pliku

        lista_zadan.push_back(do_dodania);  //dodajemy wczytane zadanie do naszego wektora zadan
    }

    str_czytajacy.close();  //zamykamy strumień
}
//----------------------------------------------------------------------------------------
bool Organizer::zapisz_liste_zadan_na_dysk()
{
    ofstream str_piszacy(plik_zadan.c_str());   //definicja strumienia piszącego do pliku, plik taki przy otwarciu jest
                                                //automatycznie czyszczony (o to nam chodzi), a następnie zapisujemy
                                                //do niego całą naszą zawartość wektora zadan

    if(!str_piszacy.is_open())
    {
        return false;
    }

    for(unsigned i = 0 ; i < lista_zadan.size() ; ++i)
    {
        str_piszacy << lista_zadan[i].tresc << '\n'
                    << lista_zadan[i].data_poczatkowa << ' '
                    << lista_zadan[i].data_terminu << ' '
                    << lista_zadan[i].data_wykonania << ' '
                    << lista_zadan[i].czas_na_wykonanie << ' '
                    << lista_zadan[i].priorytet << ' '
                    << lista_zadan[i].procent_postepu << '\n';
    }

    str_piszacy.close();
    return true;
}
//----------------------------------------------------------------------------------------
void Organizer::wyswietl_liste_zadan()
{
    if(lista_zadan.empty())
    {
        cout << "\n\nLista zadan jest pusta.\n";
    }
    else
    {
        cout << "\n\nLista zadan:\n\n";

        for(unsigned i = 0 ; i < lista_zadan.size() ; ++i)
        {
            //Poniższa część pętli oblicza ilość dni, przez jaką jest już wykonywane zadanie,
            //służy ona do "rozebrania na części" dwóch dat - daty początkowej zadania i dzisiejszej,
            //a następnie oblicza różnicę pomiędzy ->ilością sekund<- ich dzielących (stąd dzielenie przez
            //86400 w ostatniej linijce zaznaczonego obszaru - tyle sekund ma dzień)
            //---------------------------------------------------------------------------------------------
            istringstream str_czyt(lista_zadan[i].data_poczatkowa + ' ' + dzisiejsza_data);

            //czas podzielony to struktura z biblioteki standardowej jezyka (naglowek <ctime>),
            //ktorej skladniki skladaja sie na aktualny czas, mozemy odczytac z niej np. godzine, dzien w miesiacu itp.
            struct tm czas_podzielony_poczatkowy,
                      czas_podzielony_koncowy;

            char temp;

            str_czyt >> czas_podzielony_poczatkowy.tm_year >> temp >> czas_podzielony_poczatkowy.tm_mon >> temp >> czas_podzielony_poczatkowy.tm_mday
                     >> czas_podzielony_koncowy.tm_year >> temp >> czas_podzielony_koncowy.tm_mon >> temp >> czas_podzielony_koncowy.tm_mday;

            //Wprowadzamy drobne poprawki do wartości przechowywanych w składnikach struktur, tak aby odpowiadały
            //formatowi opisywanemu przez standard języka (np. tm_year "zaczyna" liczyć lata od 1900 roku):
            czas_podzielony_poczatkowy.tm_year -= 1900;
            czas_podzielony_poczatkowy.tm_mon -= 1;
            czas_podzielony_poczatkowy.tm_sec = 0;
            czas_podzielony_poczatkowy.tm_min = 0;
            czas_podzielony_poczatkowy.tm_hour = 0;

            czas_podzielony_koncowy.tm_year -= 1900;
            czas_podzielony_koncowy.tm_mon -= 1;
            czas_podzielony_koncowy.tm_sec = 0;
            czas_podzielony_koncowy.tm_min = 0;
            czas_podzielony_koncowy.tm_hour = 0;

            time_t czas_kalendarzowy_poczatkowy = mktime(&czas_podzielony_poczatkowy),
                   czas_kalendarzowy_koncowy = mktime(&czas_podzielony_koncowy);

            int ile_wykonywane = (czas_kalendarzowy_koncowy - czas_kalendarzowy_poczatkowy) / 86400;
            //---------------------------------------------------------------------------------------------

            cout << '\t' << i+1 << ". "
                 << lista_zadan[i].tresc
                 << "\n\t--------------------------------------------------------\n\t"
                 << "data terminu: " << lista_zadan[i].data_terminu
                 << ", data poczatkowa: " << lista_zadan[i].data_poczatkowa
                 << "\n\tczas potrzebny na wykonanie: " << lista_zadan[i].czas_na_wykonanie << " dni,"
                 << (lista_zadan[i].data_wykonania > lista_zadan[i].data_terminu ? " (# dluzej niz deadline! #)" : "") << "\n\t"
                 << "(zadanie jest wykonywane juz " << ile_wykonywane << " dni),\n\t"
                 << "priorytet zadania: " << lista_zadan[i].priorytet
                 << ", procent ukonczenia: " << lista_zadan[i].procent_postepu << "%\n\n";
        }
    }

    cout << "\n";
}
//----------------------------------------------------------------------------------------
void Organizer::dodaj_zadanie()
{
    Zadanie nowe_zadanie;
    nowe_zadanie.data_poczatkowa = dzisiejsza_data;

    cout << "\n\tPodaj tresc zadania > ";
    getline(cin, nowe_zadanie.tresc);

    nowe_zadanie.data_terminu = wprowadzanie_daty_terminu();

    while(true)
    {
        cout << "\tPodaj czas potrzebny na wykonanie (w dniach) > ";

        cin >> nowe_zadanie.czas_na_wykonanie;

        if(cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n\t\tNiepoprawny format liczby!\n\n";
        }
        else
        {
            time_t czas_kalendarzowy = time(0); //wczytanie aktualnego czasu kalendarzowego
            struct tm* czas_podzielony = localtime(&czas_kalendarzowy); //konwersja z czasu kalendarzowego na tzw. czas podzielony

            //czas podzielony to struktura, ktorej skladniki skladaja sie na aktualny czas, mozemy odczytac z niej np. godzine, dzien w miesiacu itp.

            czas_podzielony->tm_mday += nowe_zadanie.czas_na_wykonanie;  //zwiększamy dzien o odpowiednią ilość dni
            mktime(czas_podzielony);    //wywolujemy funkcje mktime, ktora koryguje skladniki struktury
                                        //czas_podzielony - dzieki temu np. przy 32 dniu miesiaca, funkcja ta automatycznie
                                        //zmieni skladniki struktury tak, ze beda one opisywaly 1-szy dzien nastepnego miesiaca
                                        //(a moze i nawet roku - w przypadku 31 grudnia)

            int akt_rok = czas_podzielony->tm_year+1900,
                akt_miesiac = czas_podzielony->tm_mon+1,
                akt_dzien = czas_podzielony->tm_mday;

            ostringstream str_do_stringu;
            str_do_stringu.fill('0');

            str_do_stringu << akt_rok << '-' << setw(2) << akt_miesiac << '-' << setw(2) << akt_dzien;

            nowe_zadanie.data_wykonania = str_do_stringu.str();

            break;
        }
    }

    while(true)
    {
        cout << "\tPodaj priorytet (liczba 0-10, wieksza = wiekszy priorytet) > ";

        cin >> nowe_zadanie.priorytet;

        if(cin.fail() || nowe_zadanie.priorytet < 0 || nowe_zadanie.priorytet > 10)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n\t\tNiepoprawna wartosc priorytetu!\n\n";
        }
        else
        {
            break;
        }
    }

    lista_zadan.push_back(nowe_zadanie);

    sortuj_liste_zadan();
}
//----------------------------------------------------------------------------------------
void Organizer::aktualizuj_postep()
{
    wyswietl_liste_zadan();

    unsigned nr_zadania = wprowadzanie_nr_zadania();

    while(true)
    {
        cout << "\tWprowadz postep zadania w procentach (0-100) > ";

        cin >> lista_zadan[nr_zadania-1].procent_postepu;

        if(cin.fail() || lista_zadan[nr_zadania-1].procent_postepu < 0 || lista_zadan[nr_zadania-1].procent_postepu > 100)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n\t\tNiepoprawna wartosc liczbowa!\n\n";
        }
        else
        {
            break;
        }
    }

    if(lista_zadan[nr_zadania-1].procent_postepu == 100)    //jeśli ukończyliśmy zadanie to usuwamy go z listy
    {
        lista_zadan.erase(lista_zadan.begin()+nr_zadania-1);
    }
}
//----------------------------------------------------------------------------------------
void Organizer::zmien_date_terminu()
{
    wyswietl_liste_zadan();

    unsigned nr_zadania = wprowadzanie_nr_zadania();

    lista_zadan[nr_zadania-1].data_terminu = wprowadzanie_daty_terminu();

    sortuj_liste_zadan();
}
//----------------------------------------------------------------------------------------
void Organizer::zmien_priorytet()
{
    wyswietl_liste_zadan();

    unsigned nr_zadania = wprowadzanie_nr_zadania();

    while(true)
    {
        cout << "\tPodaj nowy priorytet (liczba 0-10, wieksza = wiekszy priorytet) > ";

        cin >> lista_zadan[nr_zadania-1].priorytet;

        if(cin.fail() || lista_zadan[nr_zadania-1].priorytet < 0 || lista_zadan[nr_zadania-1].priorytet > 10)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n\t\tNiepoprawna wartosc priorytetu!\n\n";
        }
        else
        {
            break;
        }
    }

    sortuj_liste_zadan();
}
//----------------------------------------------------------------------------------------
void Organizer::sortuj_liste_zadan()
{
    sort(lista_zadan.begin(), lista_zadan.end(), porownanie_zadan);
}
//----------------------------------------------------------------------------------------
void Organizer::uaktualnij_dzisiejsza_date()
{
    time_t czas_kalendarzowy = time(0); //wczytanie aktualnego czasu kalendarzowego
    struct tm* czas_podzielony = localtime(&czas_kalendarzowy); //konwersja z czasu kalendarzowego na tzw. czas podzielony

    //czas podzielony to struktura, ktorej skladniki skladaja sie na aktualny czas, mozemy odczytac z niej np. godzine, dzien w miesiacu itp.
    int akt_rok = czas_podzielony->tm_year+1900,
        akt_miesiac = czas_podzielony->tm_mon+1,
        akt_dzien = czas_podzielony->tm_mday;

    ostringstream str_do_stringu;
    str_do_stringu.fill('0');

    str_do_stringu << akt_rok << '-' << setw(2) << akt_miesiac << '-' << setw(2) << akt_dzien;

    dzisiejsza_data = str_do_stringu.str();
}
//----------------------------------------------------------------------------------------
string Organizer::wprowadzanie_daty_terminu()
{
    const int dni_w_miesiacu[12] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    //tablica, w ktorej zapisano ile kazdy miesiac ma dni

    while(true)
    {
        cout << "\tPodaj date konca terminu formacie RRRR-MM-DD > ";

        int rok, miesiac, dzien;
        char temp;

        cin >> rok >> temp >> miesiac >> temp >> dzien;

        if(cin.fail() || miesiac < 1 || miesiac > 12 ||     //jeśli user poda niepoprawną datę lub użyje błędnego formatu
           dzien < 1 || dzien > dni_w_miesiacu[miesiac-1])  //(np. wstawi gdzieś literę) i strumień będzie zawierał tzw. flagę błędu...
        {
            cin.clear();    //...to czyścimy tą flagę
            cin.ignore(numeric_limits<streamsize>::max(), '\n');    //i ignorujemy (odrzucamy) błędne dane wejściowe zalegające w strumieniu
            //(numeric_limits<streamsize>::max() to specjalna wartość przekazywana jako pierwszy argument do funkcji ignore,
            //który oznacza ilość znaków do zignorowania (wyjęcia ze strumienia wejściowego), wartość ta oznacza: "zignoruj
            //WSZYSTKIE zalegające w nim znaki aż do napotkania znaku nowej linii '\n' (drugi argument funkcji ignore)")

            cout << "\n\t\tNiepoprawny format daty!\n\n";
        }
        else
        {
            ostringstream str_do_stringu;

            str_do_stringu.fill('0'); //ustawiamy znak wypelniacz na '0' do wypelniania pustych miejsc
            //przy uzywaniu manipulatora setw(2) - dzieki temu np. data 2013-06-05 nie zostanie zapisana
            //jako 2013-6-5, jest to ważne z powodu sposobu leksykograficznego (słownikowego) jakiego używamy
            //do porównywania dat zapisanych w obiektach typu string!

            str_do_stringu << rok << '-' << setw(2) << miesiac << '-' << setw(2) << dzien;

            return str_do_stringu.str();
        }
    }
}
//----------------------------------------------------------------------------------------
unsigned Organizer::wprowadzanie_nr_zadania()
{
    unsigned nr_zadania;

    while(true) //pętla wczytująca do skutku poprawny nr zadania
    {
        cout << "\tPodaj nr zadania >>> ";
        cin >> nr_zadania;

        if(cin.fail() || nr_zadania < 1 || nr_zadania > lista_zadan.size())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n\t\tNiepoprawny nr zadania!\n\n";
        }
        else
        {
            return nr_zadania;
        }
    }
}
//----------------------------------------------------------------------------------------
string Organizer::pobierz_dzisiejsza_date()
{
    return dzisiejsza_data;
}
//----------------------------------------------------------------------------------------
void Organizer::wywietl_menu()
{
    while(true)
    {
        wyswietl_liste_zadan();

        cout << "------------------------------------------------------\n"
             << "MENU:\n\n"
             << "\t1. Dodaj nowe zadanie\n"
             << "\t2. Aktualizuj postep zadania\n"
             << "\t3. Zmien date terminu zadania\n"
             << "\t4. Zmien priorytet zadania\n"
             << "\t5. Wyjdz\n\n\n";

        unsigned opcja;

        while(true) //pętla wczytująca do skutku poprawny nr opcji
        {
            cout << "\tTwoj wybor >>> ";
            cin >> opcja;

            if(cin.fail() || opcja < 1 || opcja > 5)
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "\n\t\tNiepoprawny nr opcji!\n\n";
            }
            else
            {
                break;
            }
        }

        if(opcja == 1)
        {
            cin.ignore();  //na ENTER z cinu, następną operacją wejścia pokieruje funkcja getline,
                           //więc trzeba ten znak nowej linii wyrzucić ze strumienia wejściowego
                           //(inaczej getline od razu zakończyłoby swoją pracę po napotkaniu tego znaku)

            dodaj_zadanie();
        }
        else if(opcja == 2)
        {
            aktualizuj_postep();
        }
        else if(opcja == 3)
        {
            zmien_date_terminu();
        }
        else if(opcja == 4)
        {
            zmien_priorytet();
        }
        else /* if(opcja == 5) */
        {
            return;
        }
    }
}
//----------------------------------------------------------------------------------------
int main()
{
    //Ponieważ organizer zadań oparliśmy na klasach, cała funkcja main sprowadza
    //się tylko do wywołania funkcji składowej rysującej menu obiektu klasy "Organizer"

    Organizer moj_organizer("zadania.txt");

    cout << "Dzisiejsza data: " << moj_organizer.pobierz_dzisiejsza_date();

    moj_organizer.wywietl_menu();

    return EXIT_SUCCESS;
    //na końcu programu do pracy rusza destruktor obiektu "moj_organizer", który
    //zapisuje aktualną listę zadań z powrotem do pliku tekstowego na dysku
}
