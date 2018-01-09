#include <iostream>
#include <vector>   //naglowek dla kontenera vector
#include <fstream>  //naglowek dla zdefiniowania strumienia czytajacego z pliku
using namespace std;

//funkcja zamieniajaca wspolrzedna w postaci nr wiersza i kolumny na index w liniowej reprezetnacji sudoku
int coordtoidx(int row, int column)
{
    //ogolnie jesli mamy tablice dwuwymiarowa, np. w naszym przypadku bylo by to int sudoku[9][9] gdyby nie wymagana liniowa reprezentacja planszy,
    //i odwolujemy sie np. do elementu sudoku[4][7] to kompilator zamienia to na indeks (tablice wielowymiarowe sa przechowywane w komputerze
    //wlasnie w reprezetnacji liniowej, wiersz za wierszem) o numerze 9 * 4 + 7 - wzor wyglada tak: ILOSC_KOLUMN * numer_wiersza + numer_kolumny
    return 9 * row + column;
}

//funkcja zapisujaca wiersz o numerze row do wektora row_elements
void extract_row(const vector<int>& sudoku, int row, vector<int>& row_elements)
{
    //parametr sudoku tej funkcji (podobnie jak w ponizszych funkcjach) jest zdefiniowany jako stala (const) referencja (&)
    //do obiektu typu vector<int>; referencja - aby przeslac przez referencje a nie wartosc i przez to uniknac zbednego kopiowania elementow,
    //a stala - aby funkcja nie zmienila elementow tego wektora (to nie jest jej zadanie)

    //parametr row_elements tej funkcji (podobnie jak w ponizszych funkcjach) jest zdefiniowany jako referencja (&)
    //do obiektu typu vector<int>; referencja - aby przeslac przez referencje a nie wartosc i przez to umozliwic tej funkcji modyfikacje elementow
    //tego wektora

    row_elements.resize(9); //zmieniamy rozmiar wektora tak aby pomiescil 9 elementow

    for(int i = 0 ; i < 9 ; ++i)
        row_elements[i] = sudoku[coordtoidx(row, i)];   //do wektora przypisujemy elementy danego wiersza, korzystajac z funkcji coordtoidx,
                                                        //ktora zwraca nam odpowiedni index do glownego wektora zawierajacego liniowa reprezetnacje planszy sudoku
}

//funkcja analogiczna do powyzszej, ale zapisujaca nie wiersz a kolumne do wektora column_elements
void extract_col(const vector<int>& sudoku, int column, vector<int>& column_elements)
{
    column_elements.resize(9);

    for(int i = 0 ; i < 9 ; ++i)
        column_elements[i] = sudoku[coordtoidx(i, column)];
}

//funkcja zapisujaca subsquare o wspolrzednych gornego lewego elementu [row, column] do wektora subsquare_elements
void extract_subsquare(const vector<int>& sudoku, int row, int column, vector<int>& subsquare_elements)
{
    subsquare_elements.resize(9);

    for(int i = row, subsq_index = 0 ; i < row+3 ; ++i) //iterujemy po indeskach wierszy poczawszy od wartosci row
        for(int j = column ; j < column+3 ; ++j, ++subsq_index) //iterujemy po indeksach kolumn poczawszy od wartosci column
            subsquare_elements[subsq_index] = sudoku[coordtoidx(i, j)]; //zapisujemy element o wspolrzednych [i, j] do naszego wetkora
}

//funkcja sprawdzajaca czy liczba o wartosci number wystepuje tylko raz w wektorze numbers
bool once(int number, const vector<int>& numbers)
{
    bool found = false;

    for(unsigned i = 0 ; i < numbers.size() ; ++i)
    {
        if(numbers[i] == number)    //jesli element wektora jest rowny naszej liczbie number
        {
            if(found == false)  //i jesli jest to jej pierwsze wystapienie
                found = true;   //to ustawiamy flage na wartosc true
            else    //jesli nie jest to pierwsze wystapienie naszej liczby
                return false;   //to funkcja zwraca wartosc false
        }
    }

    return found;   //jesli funkcja nie zakonczyla sie w petli for to zwracamy wartosc bool found:
                    //- jest ona rowna true, gdy nasza liczba wystepuje w wektorze tylko raz
                    //- jest ona rowna false, gdy nasza liczba w ogole nie wystepuje w wektorze
}

//glowna funkcja sprawdzajaca czy wektor sudoku przechowuje reprezentacje sudoku, ktora jest prawidlowa
bool check_sudoku(const vector<int>& sudoku)
{
    vector<int> output_vector;  //output_vector przeznaczony jest dla funkcji, ktore zwracaja swoj wynik dzialania jako wektor

    for(int i = 0 ; i < 9 ; ++i)    //dla kazdego wiersza i kolumny:
    {
        extract_col(sudoku, i, output_vector);  //zapisujemy obecna kolumne do output_vector

        for(int number = 1 ; number <= 9 ; ++number)
        {
            if(once(number, output_vector) == false)    //i sprawdzamy czy kazda liczba od 1-9 wystepuje tam tylko raz
                return false;
        }

        extract_row(sudoku, i, output_vector);  //zapisujemy obecny wiersz do output_vector

        for(int number = 1 ; number <= 9 ; ++number)
        {
            if(once(number, output_vector) == false)    //i sprawdzamy czy kazda liczba od 1-9 wystepuje tam tylko raz
                return false;
        }
    }

    for(int row = 0 ; row < 9 ; row += 3)   //dla wartosci row rownych: 0, 3, 6
    {
        for(int column = 0 ; column < 9 ; column += 3)  //dla wartosci column rownych: 0, 3, 6
        {
            extract_subsquare(sudoku, row, column, output_vector);  //zapisujemy dany subsquare do output_vector

            for(int number = 1 ; number <= 9 ; ++number)
            {
                if(once(number, output_vector) == false)    //i sprawdzamy czy kazda liczba od 1-9 wystepuje tam tylko raz
                    return false;
            }
        }
    }

    return true;    //jesli funkcja nie zakonczyla sie w powyzszych petlach, oznacza to, ze rozpatrywane sudoku jest prawidlowe,
                    //zwracamy wiec wartosc true
}

int main()
{
    string filename;
    getline(cin, filename, '\n');   //funkcja wczytujaca do zmiennej tekstowej typu string o nazwie filename cala linie wprowadzona przez uzytkownika

    ifstream in_stream(filename.c_str());   //definicja strumienia czytajacego z pliku o nazwie wprowadzonej przez uzytkownika
                                            //funkcja c_str zamienia string na C-string, wymagany w konstruktorze obiektu ifstream

    vector<int> sudoku(81); //definicja wektora sudoku przechowujacego liniowa reprezentacje planszy sudoku

    for(int i = 0 ; i < 81 ; ++i)
        in_stream >> sudoku[i]; //wczytujemy 81 liczb z pliku do naszego wektora

    in_stream.close();  //zamykamy strumien czytajacy z pliku

    if(check_sudoku(sudoku))    //jesli plansza sudoku jest prawidlowa,
        cout << "valid";    //wypisujemy "valid"
    else    //jesli plansza sudoku nie jest prawidlowa,
        cout << "invalid";  //wypisujemy "invalid"

    return 0;
}
