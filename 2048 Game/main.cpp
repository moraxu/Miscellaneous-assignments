#include <iostream> //dla IO
#include <fstream>  //dla IO na plikach
#include <string>   //dla zmiennych tekstowych
#include <vector>   //dla wektorow
#include <cstdlib>  //dla funkcji srand i rand
#include <ctime>    //dla funkcji time
#include <utility>  //dla struktury pair
using namespace std;

void print_board(const vector<vector<int> >& board)
{
    for(int i = 0 ; i < 4 ; ++i)    //petla drukujaca zawartosc planszy na ekranie
    {
        for(int j = 0 ; j < 4 ; ++j)
        {
            cout << board[i][j] << "\t";
        }
        cout << "\n";
    }
}

void draw_tile_on_board(vector<vector<int> >& board)    //funkcja losujaca kafelek 2 na planszy
{
    vector<pair<int, int> > free_spots; //wektor przechowujacy pare liczb - wspolrzedne kafelka na planszy

    for(int i = 0 ; i < 4 ; ++i)
    {
        for(int j = 0 ; j < 4 ; ++j)
        {
            if(board[i][j] == 0)    //do tego wektora dodajemy wspolrzedne wszystkich pustych kafelkow
                free_spots.push_back(make_pair(i, j));
        }
    }

    int drawn_index = rand() % free_spots.size();   //nastepnie losujemy indeks tego wektora

    board[free_spots[drawn_index].first][free_spots[drawn_index].second] = 2;   //z wylosowanego indeksu pobieramy wspolrzedne, pod
                                                                                //ktorymi znajdzie sie nowo wylosowany kafelek z wartoscia 2
}

bool game_can_be_continued(const vector<vector<int> >& board)   //funkcja sprawdzajaca czy gra moze byc kontynuowana (czy nie wystapil game over)
{
    for(int i = 0 ; i < 4 ; ++i)
    {
        for(int j = 0 ; j < 4 ; ++j)
        {
            if( (board[i][j] == 0)  //jezeli natrafilismy na pusty kafelek
                    ||
                (i > 0 && board[i-1][j] == board[i][j]) //lub kafelek wyzej ma taka sama wartosc jak aktualny
                    ||
                (i < 3 && board[i+1][j] == board[i][j]) //lub kafelek nizej ma taka sama wartosc jak aktualny
                    ||
                (j < 3 && board[i][j+1] == board[i][j]) //lub kafelek na prawo ma taka sama wartosc jak aktualny
                    ||
                (j > 0 && board[i][j-1] == board[i][j]))    //lub kafelek na lewo ma taka sama wartosc jak aktualny
                    return true;    //to zwracamy true - gra moze byc kontynuowana
        }
    }

    return false;   //w przeciwnym wypadku zwracamy false - wystapil game over
}

bool shift_blank_tiles_up(vector<vector<int> >& board, int j)   //funkcja wywolywana w przypadku ruchu "W" - przesuwa w gore
{                                                               //kafelki z wartosciami, a w dol puste kafelki
    bool move_occurred = false;

    for(int i = 0, next_starting_point = 1 ; next_starting_point < 4 && i < 3 ; ++i, ++next_starting_point)
    {
        if(board[i][j] == 0)    //jezeli napotkalismy pusty kafelek to...
        {
            bool break_the_loop = false;

            //...przebiegamy nastepne kafelki, az do konca - w tym przypadku, w ruchu "W" - kolumny,
            //a w ruchu "A" i "D" - do konca wiersza, dopoki nie napotkamy...

            for( ; !break_the_loop && next_starting_point < 4 ; ++next_starting_point)
            {
                if(board[next_starting_point][j] != 0)  //...niepustego kafelka - wtedy...
                {
                    board[i][j] = board[next_starting_point][j];    //...przenosimy ten kafelek na miejsce pustego
                    board[next_starting_point][j] = 0;

                    break_the_loop = true;  //konczymy wewnetrzna petle for (potrzebna nam flaga typu bool, poniewaz nie mozna uzywac instrukcji break)
                    move_occurred = true;   //jesli przesunelismy jakikolwiek kafelek, zwracamy true pod koniec funkcji
                    --next_starting_point;  //musimy zdekrementowac ta zmienna, poniewaz zwiekszamy ja tez w zewnetrznej petli for - byla by ona wtedy
                                            //zwiekszona dwukrotnie
                }
            }
        }
    }

    return move_occurred;
}

bool shift_blank_tiles_down(vector<vector<int> >& board, int j) //funkcja wywolywana w przypadku ruchu "S" - przesuwa w dol
{                                                               //kafelki z wartosciami, a w gore puste kafelki
    bool move_occurred = false;

    for(int i = 3, next_starting_point = 2 ; next_starting_point >= 0 && i >= 1 ; --i, --next_starting_point)
    {
        if(board[i][j] == 0)
        {
            bool break_the_loop = false;

            for( ; !break_the_loop && next_starting_point >= 0 ; --next_starting_point)
            {
                if(board[next_starting_point][j] != 0)
                {
                    board[i][j] = board[next_starting_point][j];
                    board[next_starting_point][j] = 0;

                    break_the_loop = true;
                    move_occurred = true;   //jesli przesunelismy jakikolwiek kafelek, zwracamy true pod koniec funkcji
                    ++next_starting_point;
                }
            }
        }
    }

    return move_occurred;
}

bool shift_blank_tiles_left(vector<vector<int> >& board, int i) //funkcja wywolywana w przypadku ruchu "A" - przesuwa w lewo
{                                                               //kafelki z wartosciami, a w prawo puste kafelki
    bool move_occurred = false;

    for(int j = 0, next_starting_point = 1 ; next_starting_point < 4 && j < 3 ; ++j, ++next_starting_point)
    {
        if(board[i][j] == 0)
        {
            bool break_the_loop = false;

            for( ; !break_the_loop && next_starting_point < 4 ; ++next_starting_point)
            {
                if(board[i][next_starting_point] != 0)
                {
                    board[i][j] = board[i][next_starting_point];
                    board[i][next_starting_point] = 0;

                    break_the_loop = true;
                    move_occurred = true;   //jesli przesunelismy jakikolwiek kafelek, zwracamy true pod koniec funkcji
                    --next_starting_point;
                }
            }
        }
    }

    return move_occurred;
}

bool shift_blank_tiles_right(vector<vector<int> >& board, int i)    //funkcja wywolywana w przypadku ruchu "D" - przesuwa w prawo
{                                                                   //kafelki z wartosciami, a w lewo puste kafelki
    bool move_occurred = false;

    for(int j = 3, next_starting_point = 2 ; next_starting_point >= 0 && j >= 1 ; --j, --next_starting_point)
    {
        if(board[i][j] == 0)
        {
            bool break_the_loop = false;

            for( ; !break_the_loop && next_starting_point >= 0 ; --next_starting_point)
            {
                if(board[i][next_starting_point] != 0)
                {
                    board[i][j] = board[i][next_starting_point];
                    board[i][next_starting_point] = 0;

                    break_the_loop = true;
                    move_occurred = true;   //jesli przesunelismy jakikolwiek kafelek, zwracamy true pod koniec funkcji
                    ++next_starting_point;
                }
            }
        }
    }

    return move_occurred;
}

bool shift_tiles_up(vector<vector<int> >& board)    //funkcja glowna, wywolywana w przypadku ruchu "W"
{
    bool move_occurred = false; //zmienna oznaczajaca czy wystapil jakis ruch na planszy - jesli nie, to
                                //plansza nie jest ponownie drukowana

    for(int j = 0 ; j < 4 ; ++j)
    {
        move_occurred |= shift_blank_tiles_up(board, j);    //przesuwamy wszystkie kafelki z wartoscia na gore, a puste na dol

        for(int i = 0 ; i < 3 ; ++i)    //nastepnie laczymy sasiednie kafelki z taka sama wartoscia w jeden - jesli jest to mozliwe
        {
            if(board[i][j] != 0 && board[i][j] == board[i+1][j])
            {
                board[i][j] += board[i][j];
                board[i+1][j] = 0;
                ++i;

                move_occurred |= true;
            }
        }

        move_occurred |= shift_blank_tiles_up(board, j);    //ponownie wykonujemy ta operacje co na poczatku petli, gdyby po polaczeniu
                                                            //kafelkow, pomiedzy nimi znalazly sie kafelki puste
    }

    return move_occurred;
}

bool shift_tiles_down(vector<vector<int> >& board)  //patrz opis funkcji shift_tiles_up
{
    bool move_occurred = false;

    for(int j = 0 ; j < 4 ; ++j)
    {
        move_occurred |= shift_blank_tiles_down(board, j);

        for(int i = 3 ; i > 0 ; --i)
        {
            if(board[i][j] != 0 && board[i][j] == board[i-1][j])
            {
                board[i][j] += board[i][j];
                board[i-1][j] = 0;
                --i;

                move_occurred |= true;
            }
        }

        move_occurred |= shift_blank_tiles_down(board, j);
    }

    return move_occurred;
}

bool shift_tiles_right(vector<vector<int> >& board) //patrz opis funkcji shift_tiles_up
{
    bool move_occurred = false;

    for(int i = 0 ; i < 4 ; ++i)
    {
        move_occurred |= shift_blank_tiles_right(board, i);

        for(int j = 3 ; j > 0 ; --j)
        {
            if(board[i][j] != 0 && board[i][j] == board[i][j-1])
            {
                board[i][j] += board[i][j];
                board[i][j-1] = 0;
                --j;

                move_occurred |= true;
            }
        }

        move_occurred |= shift_blank_tiles_right(board, i);
    }

    return move_occurred;
}

bool shift_tiles_left(vector<vector<int> >& board)  //patrz opis funkcji shift_tiles_up
{
    bool move_occurred = false;

    for(int i = 0 ; i < 4 ; ++i)
    {
        move_occurred |= shift_blank_tiles_left(board, i);

        for(int j = 0 ; j < 3 ; ++j)
        {
            if(board[i][j] != 0 && board[i][j] == board[i][j+1])
            {
                board[i][j] += board[i][j];
                board[i][j+1] = 0;
                ++j;

                move_occurred |= true;
            }
        }

        move_occurred |= shift_blank_tiles_left(board, i);
    }

    return move_occurred;
}

bool shift_tiles_on_board(vector<vector<int> >& board, char user_move)  //funkcja, ktora na podstawie ruchu podanego przez usera
{                                                                       //wywoluje odpowiednia funkcje pomocnicza
    if(user_move == 'w')
    {
        return shift_tiles_up(board);
    }
    else if(user_move == 's')
    {
        return shift_tiles_down(board);
    }
    else if(user_move == 'a')
    {
        return shift_tiles_left(board);
    }
    else if(user_move == 'd')
    {
        return shift_tiles_right(board);
    }

    //funkcja zwraca true jesli wystapil jakis ruch na planszy, w przeciwnym wypadku zwraca false
    return false;
}

int main()
{
    srand(time(0)); //zainicjalizowanie generatora liczb pseudolosowych tzw. ziarnem - u nas jest to liczba sekund od 1970 r.,
                    //dzieki temu przy kazdym ponownym uruchomieniu programu wylosowane liczby beda rozne

    cout << "enter initial configuration file name:\n";
    string file_name;
    getline(cin, file_name, '\n');

    vector<vector<int> > board(4);  //definicja naszej planszy liczb calkowitych o rozmiarze 4x4

    for(int i = 0 ; i < 4 ; ++i)
        board[i].resize(4);

    ifstream in_str(file_name.c_str());

    if(in_str.is_open())    //jesli udalo sie otworzyc plik podany przez usera
    {
        for(int i = 0 ; i < 4 ; ++i)
        {
            for(int j = 0 ; j < 4 ; ++j)
            {
                in_str >> board[i][j];  //to wczytujemy z niego liczby (kafelki) do naszej planszy
            }
        }
    }
    else    //jesli nie, to w domyslnie zainicjalizowanej planszy zerami ustawiamy ostatni kafelek na 2
    {
        cout << "file not found, using default start configuration\n";
        board[3][3] = 2;
    }

    while(game_can_be_continued(board)) //dopoki nie wystapil game over
    {
        print_board(board); //drukujemy plansze

        cout << "\n";

        bool move_occurred = false;
        do
        {
            char user_move;
            cin >> user_move;   //user wprowadza ruch

            move_occurred = shift_tiles_on_board(board, user_move); //a funkcja shift_tiles_on_board
                                                                    //zwraca true, jesli wystapil jakis ruch na planszy

        }while(!move_occurred); //petla do...while jest kontynuowana az user wprowadzi ruch, ktory wywola jakis ruch na planszy (kafelki sie przesuna)

        draw_tile_on_board(board);  //losujemy kafelek 2 na ktoryms wolnym miejscu na planszy

        cout << "\n";
    }

    cout << "\n\ngame over\n";

    return 0;
}
