#include <iostream>
#include <limits>
#include <ctime>
#include <cctype>
#include <vector>
#include <iomanip>
#include <cstdlib>

using namespace std;

//-----------------------------------------------------------------------------------------------------
class saper
{
    vector< vector<int> > stan;         //wektor wektorow przechowujacy stan planszy (wartosc int moze przybrac 3 wartosci:
                                        //-1 - mina na polu, 0 - puste pole, wartosc od 1 do 8 - ilosc min na sasiednich polach)
    vector< vector<char> > plansza;     //wektor wektorow przechowujacy aktualny wyglad planszy

    const int WYM_X;
    const int WYM_Y;

    const int ILOSC_MIN;

    enum { MINA = -1, PUSTE = 0 };  //stale z wartoscia - nienazwany typ enum

    //Funkcje składowe:
    int wybierz_ilosc_min() const;
    void inicjalizuj_plansze();
    int ile_min_wokol(const int x, const int y) const;
    void odkryj_obszar(const int x, const int y);
    void ustaw_wspolrzedne(int& x, int& y) const;
    void umiesc_flage();
    bool odslon_pole();
    bool czy_wygrana() const;

public:
    saper(int wym_x = 10, int wym_y = 10);
    void rysuj_plansze(bool pokazac_miny = false) const;
    bool ruch_gracza();
};
//-----------------------------------------------------------------------------------------------------
int saper::wybierz_ilosc_min() const
{
    cout << "\n\t\tWitaj w grze saper!\n\n";

    int ile_min;

    bool poprawny_wybor = false;
    while(!poprawny_wybor)
    {
        cout << "Ile min umiescic na planszy (1-20)? >>> ";
        cin >> ile_min;

        if(cin.fail() || ile_min < 1 || ile_min > 20)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');    //jezeli w strumieniu wystapily bledy, to ta funkcja
                                                                    //usuwa z niego wszystkie zalegajace w nim bajty
            cout << "\n\t\tNieprawidlowa liczba min!\n\n";
        }
        else
        {
            poprawny_wybor = true;
        }
    }

    return ile_min;
}
//-----------------------------------------------------------------------------------------------------
void saper::inicjalizuj_plansze()
{
    srand(time(0)); //inicjalizacja ziarnem generatora liczb pseudolosowych
    for(int i = 0 ; i < ILOSC_MIN ; ++i)
    {
        bool czy_umieszczono = false;
        while(!czy_umieszczono)
        {
            int x = rand() % WYM_X, //losujemy dwie wspolrzedne
                y = rand() % WYM_Y;

            if(stan[x][y] != MINA)
            {
                czy_umieszczono = true; //jezeli na tym polu nie bylo miny to wychodzimy z petli while
                stan[x][y] = MINA;      //z powrotem do for
            }
        }
    }

    for(int x = 0 ; x < WYM_X ; ++x)
    {
        for(int y = 0 ; y < WYM_Y ; ++y)
        {
            plansza[x][y] = 'X';    //na planszy umieszczamy wszedzie 'X'

            if(stan[x][y] != MINA)  //w wektorze stanu planszy liczymy sasiednia ilosc min jesli pole nie jest mina
                stan[x][y] = ile_min_wokol(x, y);
        }
    }
}
//-----------------------------------------------------------------------------------------------------
int saper::ile_min_wokol(const int x, const int y) const
{
    int ile_wokol = 0;

    if(x > 0 && y > 0 && stan[x-1][y-1] == MINA)    //lewy gorny rog
        ++ile_wokol;

    if(y > 0 && stan[x][y-1] == MINA)   //na gorze
        ++ile_wokol;

    if(x < WYM_X-1 && y > 0 && stan[x+1][y-1] == MINA)  //prawy gorny rog
        ++ile_wokol;

    if(x > 0 && stan[x-1][y] == MINA)   //na lewo
        ++ile_wokol;

    if(x < WYM_X-1 && stan[x+1][y] == MINA) //na prawo
        ++ile_wokol;

    if(x > 0 && y < WYM_Y-1 && stan[x-1][y+1] == MINA)  //lewy dolny rog
        ++ile_wokol;

    if(y < WYM_Y-1 && stan[x][y+1] == MINA) //na dole
        ++ile_wokol;

    if(x < WYM_X-1 && y < WYM_Y-1 && stan[x+1][y+1] == MINA)    //prawy dolny rog
        ++ile_wokol;

    return ile_wokol;
}
//-----------------------------------------------------------------------------------------------------
void saper::odkryj_obszar(const int x, const int y) //funkcja odkrywajaca obszar, na ktorym nie ma min
{
    if(x < 0 || x >= WYM_X || y < 0 || y >= WYM_Y)  //sprawdzamy czy wspolrzedne sa OK
        return;

    if(plansza[x][y] == ' ' || plansza[x][y] == 'F' || stan[x][y] == MINA)
        return; //jezeli pole zostalo juz odkryte lub jest na nim mina/flaga to nic nie robimy

    plansza[x][y] = ' ';

    if(stan[x][y] != PUSTE)
    {
        plansza[x][y] = stan[x][y]+48;  //umieszczamy przy okazji na planszy liczbe informujaca
        return;                         //o ilosci sasiadujacych min z polem
    }

    odkryj_obszar(x-1, y-1);    //wywolania rekurencyjne, kazde odnosi sie do sasiedniego pola
    odkryj_obszar(x, y-1);      //(wszystkie wspolrzedne sa takie same jak w funkcji
    odkryj_obszar(x+1, y-1);    //skladowej o nazwie saper::ile_min_wokol)
    odkryj_obszar(x-1, y);
    odkryj_obszar(x+1, y);
    odkryj_obszar(x-1, y+1);
    odkryj_obszar(x, y+1);
    odkryj_obszar(x+1, y+1);
}
//-----------------------------------------------------------------------------------------------------
void saper::ustaw_wspolrzedne(int& x, int& y) const //funkcja pobierajaca od gracza dwie wspolrzedne
{
    int wsp_x,
        wsp_y;

    char litera;

    bool poprawny_wybor = false;
    while(!poprawny_wybor)
    {
        cout << "Podaj wspolrzedne pola (np. A1) >>> ";
        cin >> litera >> wsp_y;

        wsp_x = toupper(litera)-64; //zamieniamy ew. mala litere na duza a nastepnie na odpowiadajaca jej wspolrzedna X

        if(cin.fail() || wsp_x < 1 || wsp_x > WYM_X || wsp_y < 1 || wsp_y > WYM_Y)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n\t\tNiepoprawny format danych!\n\n";
        }
        else
        {
            poprawny_wybor = true;
        }
    }

    x = wsp_y-1;    //"odwrotne przypisania" z powodu stosowanego adresowania (jako A1, a nie 1A)
    y = wsp_x-1;
}
//-----------------------------------------------------------------------------------------------------
void saper::umiesc_flage()
{
    int x, y;
    ustaw_wspolrzedne(x, y);

    plansza[x][y] = 'F';
}
//-----------------------------------------------------------------------------------------------------
bool saper::odslon_pole()
{
    int x, y;
    ustaw_wspolrzedne(x, y);

    if(stan[x][y] == MINA)  //jezeli natrafilismy na mine to przegralismy - zwracamy false
        return false;

    if(plansza[x][y] == 'F')  //jezeli natrafilismy bezposrednio na flage, to odslaniamy tylko to pole
    {
        plansza[x][y] = (stan[x][y] != 0 ? stan[x][y]+48 : ' ');
        return true;
    }

    odkryj_obszar(x, y);    //ciag wywolan rekurencyjnych

    return true;    //jezeli funkcja zwraca true, to znaczy ze jeszcze nie przegralismy
}
//-----------------------------------------------------------------------------------------------------
bool saper::czy_wygrana() const
{
    bool wygrana = true;
    for(int x = 0; x < WYM_X; ++x)
    {
        for(int y = 0; y < WYM_Y; ++y)
        {
            //sprawdzamy czy plansza jest tak uzupelniona, ze pozwala nam na zwyciestwo
            wygrana &= ((plansza[x][y] != 'F' && plansza[x][y] != 'X' && stan[x][y] != MINA) ||
                       ((plansza[x][y] == 'X' || plansza[x][y] == 'F') && stan[x][y] == MINA));
        }
    }
    return wygrana;
}
//-----------------------------------------------------------------------------------------------------
saper::saper(int wym_x, int wym_y) : WYM_X(wym_x), WYM_Y(wym_y), ILOSC_MIN(wybierz_ilosc_min())
{
    stan.resize(WYM_X);     //zwiekszamy rozmiar wektorow przechowujacych inne wektory
    plansza.resize(WYM_X);

    for(int i = 0 ; i < WYM_X ; ++i)
    {
        stan[i].resize(WYM_Y, PUSTE);   //pozniej zwiekszamy kazdy z wektorow
        plansza[i].resize(WYM_Y);
    }

    inicjalizuj_plansze();  //wypelniamy nasze dwa wektory odpowiednimi wartosciami poczatkowymi
}
//-----------------------------------------------------------------------------------------------------
void saper::rysuj_plansze(bool pokazac_miny) const
{
    cout << '\n' << setw(3) << ' ';
    for(int x = 0 ; x < WYM_X ; ++x)
        cout << char('A'+x) << ' ';
    cout << '\n';

    for(int x = 0 ; x < WYM_X ; ++x)
    {
        cout << setw(2) << x+1 << ' ';
        for(int y = 0 ; y < WYM_Y ; ++y)
        {
            cout << ((pokazac_miny && stan[x][y] == MINA) ? '#' : plansza[x][y]) << ' ';
        }
        cout << '\n';
    }
}
//-----------------------------------------------------------------------------------------------------
bool saper::ruch_gracza()
{
    rysuj_plansze();

    int wybor;

    cout << "\n\nDostepne opcje: 1 - odslon pole, 2 - umiesc flage, 3 - koniec gry\n";

    bool poprawny_wybor = false;
    while(!poprawny_wybor)
    {
        cout << "Twoj wybor: ";
        cin >> wybor;

        if(cin.fail() || wybor < 1 || wybor > 3)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n\t\tNiepoprawny nr opcji!\n\n";
        }
        else
        {
            poprawny_wybor = true;
        }
    }

    if(wybor == 1)
    {
        if(!odslon_pole())
        {
            system("clear");
            cout << "Przegrales! Oto plansza z minami:\n";
            rysuj_plansze(true);

            return false;
        }

        if(czy_wygrana())
        {
            system("clear");
            cout << "Wygrales!\n";
            rysuj_plansze(true);

            return false;
        }

        system("clear");
        return true;
    }
    if(wybor == 2)
    {
        umiesc_flage();

        if(czy_wygrana())
        {
            system("clear");
            cout << "Wygrales!\n";
            rysuj_plansze(true);

            return false;
        }

        system("clear");
        return true;
    }
    if(wybor == 3)
    {
        system("clear");
        return false;
    }
}
//-----------------------------------------------------------------------------------------------------
int main()
{
    bool czy_dalej = true;

    while(czy_dalej)
    {
        saper moja_gra;

        while(moja_gra.ruch_gracza());  //dopoki nie przegramy, ta petla ciagle wywoluje funkcje skladowa
                                        //o nazwie saper::ruch_gracza

        int wybor;

        cout << "\n\nDostepne opcje: 1 - nowa plansza, 2 - koniec gry\n";

        bool poprawny_wybor = false;
        while(!poprawny_wybor)
        {
            cout << "Twoj wybor >>> ";
            cin >> wybor;

            if(cin.fail() || wybor < 1 || wybor > 2)
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "\n\t\tNiepoprawny nr opcji!\n\n";
            }
            else
            {
                poprawny_wybor = true;

                if(wybor == 2)
                    czy_dalej = false;
                else
                    system("clear");
            }
        }
    }

    return EXIT_SUCCESS;
}
