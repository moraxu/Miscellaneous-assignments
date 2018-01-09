#include <iostream>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <vector>

using namespace std;

//szczegoly dzialania AVL tree:
//http://en.wikipedia.org/wiki/AVL_tree

//--------------------------------------------------------------------------------------------
class AVL   //klasa reprezentujaca zbalansowane drzewo AVL
{
    struct node //klasa reprezentujaca wezel w drzewie AVL
    {
        int key;
        int height;
        node* left, *right;

        node()
        {
            left = right = 0;
        }

        node(int k, node* l = 0, node* r = 0, int h = 1)
        {
            key = k;
            left = l;
            right = r;
            height = h;
        }
    };

    node* root; //korzen drzewa AVL

    //nasz posredni destruktor zwalniajacy pamiec przyznana wezlom drzewa
    void clear(node* start)
    {
        if(start != 0)
        {
            clear(start->left);
            clear(start->right);
            delete start;
        }
    }

    //funkcja przeprowadzajaca rotacje w prawo
    //drzewa o korzeniu y
    node* rightRotate(node* y)
    {
        node* x1 = y->left;
        node* x2 = x1->right;

        //przeprowadzamy rotacje
        x1->right = y;
        y->left = x2;

        //uaktualniamy wysokosci
        y->height = max(height(y->left), height(y->right)) + 1;
        x1->height = max(height(x1->left), height(x1->right)) + 1;

        //zwracamy nowy korzen
        return x1;
    }

    //funkcja przeprowadzajaca rotacje w lewo
    //drzewa o korzeniu x
    node* leftRotate(node* x)
    {
        node* y1 = x->right;
        node* y2 = y1->left;

        //przeprowadzamy rotacje
        y1->left = x;
        x->right = y2;

        //uaktualniamy wysokosci
        x->height = max(height(x->left), height(x->right)) + 1;
        y1->height = max(height(y1->left), height(y1->right)) + 1;

        //zwracamy nowy korzen
        return y1;
    }

    //zwracamy tzw. balance factor dla wezla N
    int getBalance(node* N) const
    {
        if(N == 0)
            return 0;

        return height(N->left) - height(N->right);
    }

    //zwracamy wysokosc drzewa o korzeniu N
    int height(node* N) const
    {
        if(N == 0)
            return 0;

        return N->height;
    }

    node* insert(node* N, int key)
    {
        //na poczatku szukamy miejsca do wstawienia nowego elementu (wezla)
        if (N == 0)
            return(new node(key));

        if(key < N->key)
            N->left = insert(N->left, key);
        else
            N->right = insert(N->right, key);

        //pozniej uaktualniamy wysokosc aktualnego wezla (przez ktory
        //przeszlismy aby wstawic nowy element)
        N->height = max(height(N->left), height(N->right)) + 1;

        //nastepnie dowiadujemy sie o wartosc balance factor
        //dla aktualnego wezla, aby sprawdzic czy drzewo o tym
        //korzeniu jest poprawnie zbalansowane
        int balance = getBalance(N);

        //jesli nie jest zbalansowane, mamy 4 przypadki do rozwazenia:

        //Left Left
        if(balance > 1 && key < N->left->key)
            return rightRotate(N);

        //Right Right
        if (balance < -1 && key > N->right->key)
            return leftRotate(N);

        //Left Right
        if (balance > 1 && key > N->left->key)
        {
            N->left =  leftRotate(N->left);
            return rightRotate(N);
        }

        //Right Left
        if (balance < -1 && key < N->right->key)
        {
            N->right = rightRotate(N->right);
            return leftRotate(N);
        }

        return N;
    }

    //funkcja zwracajaca wezel z najmniejsza wartoscia (najbardziej na lewo)
    node* minValueNode(node* N)
    {
        node* current = N;

        while (current->left != 0)
            current = current->left;

        return current;
    }

    node* Remove(node* root, int key)
    {
        if (root == 0)
            return root;

        if(key < root->key)
        {
            root->left = Remove(root->left, key);
        }
        else if(key > root->key)
        {
            root->right = Remove(root->right, key);
        }
        else    //znalezlismy wezel do usuniecia
        {
            if((root->left == 0) || (root->right == 0)) //jesli ten wezel ma tylko jednego syna lub nie ma zadnych
            {
                node* temp = root->left ? root->left : root->right;

                if(temp == 0)    //jesli nie ma zadnych
                {
                    temp = root;
                    root = 0;
                }
                else    //jesli ma jednego syna
                {
                    *root = *temp;
                }

                delete temp;
            }
            else
            {
                //jesli wezel ma dwoje synow, to znajdujemy wezel z najmniejszym
                //elementem w jego prawym drzewie
                node* temp = minValueNode(root->right);

                root->key = temp->key;

                root->right = Remove(root->right, temp->key);
            }
        }

        //jesli drzewo mialo tylko jeden wezel to konczymy prace
        if (root == 0)
          return root;

        //uaktualniamy wysokosc aktualnego wezla
        root->height = max(height(root->left), height(root->right)) + 1;

        //nastepnie dowiadujemy sie o wartosc balance factor
        //dla aktualnego wezla, aby sprawdzic czy drzewo o tym
        //korzeniu jest poprawnie zbalansowane
        int balance = getBalance(root);

        //jesli nie jest zbalansowane, mamy 4 przypadki do rozwazenia:

        //Left Left
        if (balance > 1 && getBalance(root->left) >= 0)
            return rightRotate(root);

        //Left Right
        if (balance > 1 && getBalance(root->left) < 0)
        {
            root->left =  leftRotate(root->left);
            return rightRotate(root);
        }

        //Right Right
        if (balance < -1 && getBalance(root->right) <= 0)
            return leftRotate(root);

        //Right Left
        if (balance < -1 && getBalance(root->right) > 0)
        {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }

        return root;
    }

public:

    AVL()
    {
        root = 0;
    }

    ~AVL()
    {
        clear();
    }

    void clear()
    {
        clear(root);
        root = 0;
    }

    //funkcja tworzaca drzewo AVL z elementow wektora
    void Create(const vector<int>& vec_of_ints)
    {
        for(int i = 0 ; i < vec_of_ints.size() ; ++i)
            Insert(vec_of_ints[i]);
    }

    void Insert(int pValue)
    {
        root = insert(root, pValue);
    }

    bool Find(int pValue) const
    {
        node* current = root;

        while(current != 0)
        {
            if(pValue == current->key)
                return true;
            else if(pValue < current->key)
                current = current->left;
            else
                current = current->right;
        }

        return false;
    }

    bool Remove(int pValue)
    {
        if(Find(pValue))
        {
            root = Remove(root, pValue);
            return true;
        }
        else
        {
            return false;
        }
    }
};
//--------------------------------------------------------------------------------------------

int main()
{
    ifstream in_str("data.txt");

    if(!in_str.is_open())
    {
        cout << "Wystapil blad podczas otwierania pliku!\n";
        return EXIT_FAILURE;
    }

    vector<int> numbers;
    int temp;

    while(in_str >> temp)   //wczytujemy liczby z pliku do wektora
    {
        numbers.push_back(temp);
    }

    AVL avl_tree;
    avl_tree.Create(numbers);   //przekazujemy wektor z liczbami do naszego drzewa AVL

    cout << "(Legenda: 0-falsz, 1-prawda)\n";
    cout << "Istnieje element 12: " << avl_tree.Find(12) << "\n"
         << "Istnieje element 234: " << avl_tree.Find(234) << "\n"
         << "Istnieje element 600: " << avl_tree.Find(600) << "\n";

    avl_tree.Insert(600);
    avl_tree.Remove(234);

    cout << "Po modyfikacjach...\n";

    cout << "Istnieje element 600: " << avl_tree.Find(600) << "\n"
         << "Istnieje element 234: " << avl_tree.Find(234) << "\n";

    return EXIT_SUCCESS;
}
