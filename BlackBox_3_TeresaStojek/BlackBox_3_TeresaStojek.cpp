//kbhit() ??
#include <stdlib.h>
//#include <Windows.h>//Okazuje siê ¿e tego nie mo¿na u¿ywaæ, bo nie nale¿y do standardowych bibliotek c++
#include <conio.h>
#include <iostream>
#include <time.h>
#include <iomanip>
#include <stack>
using namespace std;
int n = 1;//numer strza³u
struct Punkt {
    int x, y;
};
struct stan {
    int etap;
    int rozmiar;
    int nKulek;
    int zaznKulki;
};

int los1do5() {
    return rand() % 5 + 1;
}
char cyfra(char n) {
    if (n < 10) return n + 48;// 0 to 48 znak tabeli ascii
    else return n - 10 + 97; // a to 97 znak ascii
}
int DajRozmiar(int etap) {
    int rozmiar;
    if (etap == 1) {
        rozmiar = 7;
    }
    else if (etap == 2) {
        rozmiar = 10;
    }
    else if (etap == 3) {
        rozmiar = 12;
    }
    return rozmiar;
}
int DajNKulek(int etap) {
    int liczbaKulek;
    if (etap == 1) {
        liczbaKulek = 3;
    }
    else if (etap == 2) {
        liczbaKulek = 5;
    }
    else if (etap == 3) {
        liczbaKulek = 8;
    }
    return liczbaKulek;
}
int liczZnaczki(char** tablica, char znaczek = 'O', int rozmiar = 7) {
    int odp = 0;
    for (int j = 0; j < rozmiar - 1; j++) {
        for (int k = 0; k < rozmiar - 1; k++) {
            if (tablica[j][k] == znaczek) odp += 1;
        }
    }
    return odp;
}
char symbol(int x, int y, int rozmiar) {
    rozmiar -= 1;
    if (x == 0 && y == 0) return 201;
    else if (x == 0 && y == rozmiar) return 200;
    else if (x == rozmiar && y == rozmiar) return 188;
    else if (x == rozmiar && y == 0) return 187;
    else if (x == 0 || x == rozmiar) return 186;
    else if (y == 0 || y == rozmiar) return 205;
    else return ' ';
}
void Odstep(int n) {
    for (int i = 0; i < n; i++)
    {
        cout << " ";
    }
}
void Wyswietl(Punkt p, char** tab, int n, int zaznaczoneKulki, int etap = 1, int trafione = -1) {
    //n to odstêp
    int liczbaKulek;
    const wchar_t pozioma = 205;
    int rozmiar = 7;
    if (etap == 1) {
        rozmiar = 7;
        liczbaKulek = 3;
    }
    else if (etap == 2) {
        rozmiar = 10;
        liczbaKulek = 5;
    }
    else if (etap == 3) {
        rozmiar = 12;
        liczbaKulek = 8;
    }
    else {
        cout << "B£¥D: PODANO NIEW£AŒCIWY ETAP";
    }
    for (int y = 0; y < rozmiar; y++) {
        if (y == 0) {
            cout << "zaznaczone: " << zaznaczoneKulki;
            Odstep(n - 13);

        }
        else if (y == 2) {
            cout << "ukryte: " << liczbaKulek;
            Odstep(n - 9);
        }
        else if (y == 4) {
            cout << "trafione: ";
            if (trafione == -1) {
                cout << '\?';
            }
            else {
                cout << trafione;
            }
            Odstep(n - 11);
        }
        else {
            Odstep(n);
        }
        for (int x = 0; x < rozmiar; x++) {
            if (p.x == x && p.y == y) cout << '@';
            else wcout << tab[x][y];

            if (x < rozmiar - 1 && (y == 0 || y == rozmiar - 1)) wcout << pozioma;
            else cout << ' ';
        }
        cout << endl;
    }
    cout << "ETAP " << etap << endl;
    cout << "kursor x:" << p.x << "  y:" << p.y << endl;
}
void PokazMenu() {

    cout << "Wybierz opcje naciskajac klawisz" << endl;
    cout << "   [esc]   Wyjscie z gry";
    cout << endl << endl;
    cout << "   [R]    Zresetuj etap";
    cout << endl << endl;
    cout << "   [M]    Wyjdz z menu";
    cout << endl << endl;
    cout << "   [2]    Etap 2";
    cout << endl << endl;
    cout << "   [3]    Etap 3";
}
Punkt StrzelLaserem(char** edytuj, char** niezmien, Punkt pocz, bool wyczysc, int rozm) {
    //info o parametrze wyczysc
    //Jeœli jest true, to miejsca gdzie wchodzi i wychodzi laser powinny zmieniaæ siê na znaki ramek w tych miejscach

    Punkt il = pocz;
    char kierunek;
    if (pocz.x == 0) kierunek = 'p';
    else if (pocz.x == rozm - 1) kierunek = 'l';
    else if (pocz.y == 0) kierunek = 'd';
    else if (pocz.y == rozm - 1) kierunek = 'g';

    int dx = 0;
    int dy = 0;

    if (kierunek == 'g') dy = -1;//Run-Time Check Failure #3 - The variable 'kierunek' is being used without being initialized.
    else if (kierunek == 'd') dy = 1;
    else if (kierunek == 'p') dx = 1;
    else if (kierunek == 'l') dx = -1;

    if (niezmien[il.x][il.y] == 'B') {//jeœli na brzegu, zrób krok do przodu
        il.x += dx;
        il.y += dy;
    }

    while (niezmien[il.x][il.y] != 'B') {//dopóki nie jest na brzegu
        bool hit = niezmien[il.x + dx][il.y + dy] == 'O';
        bool deflectPL = (dy == 0) && (niezmien[il.x][il.y + 1] == 'O' || niezmien[il.x][il.y - 1] == 'O');
        bool deflectGD = (dx == 0) && (niezmien[il.x + 1][il.y] == 'O' || niezmien[il.x - 1][il.y] == 'O');
        bool nowyKierunek = kierunek;
        if (deflectPL || deflectGD) {
            if (niezmien[il.x - dx][il.y - dy] == 'B') {//If an atom is at the edge of the grid
                if (wyczysc)  edytuj[pocz.x][pocz.y] = symbol(pocz.x, pocz.y, rozm);
                else edytuj[pocz.x][pocz.y] = 'R';
                return il;
                //any ray which is aimed into the grid directly beside it causes a reflection.
            }
            else {//Deflection - odbicie w bok - rykos
                /*if (wyczysc)  edytuj[pocz.x][pocz.y] = symbol(pocz.x, pocz.y, 7);
                else edytuj[pocz.x][pocz.y] = 'D';
                return il;*/
                il.x -= dx;
                il.y -= dy;
                if (dy == 0) {
                    if (niezmien[il.x + dx][il.y - 1] == 'O') {
                        kierunek = 'd';
                        dx = 0;
                        dy = 1;
                    }
                    else if (niezmien[il.x + dx][il.y + 1] == 'O') {
                        kierunek = 'g';//nie wchodzi do œrodka
                        dx = 0;
                        dy = -1;
                    }
                }
                else if (dx == 0) {
                    if (niezmien[il.x - 1][il.y + dy] == 'O') {
                        kierunek = 'p';
                        dx = 1;
                        dy = 0;
                    }
                    else if (niezmien[il.x + 1][il.y + dy] == 'O') {
                        kierunek = 'l';
                        dx = -1;
                        dy = 0;
                    }
                }//problem: nie zmienia wartoœci dx i dy
                continue;
            }
        }
        else if (hit) {//HIT
            if (wyczysc) edytuj[pocz.x][pocz.y] = symbol(pocz.x, pocz.y, rozm);
            else edytuj[pocz.x][pocz.y] = 'H';
            return il;
        }
        il.x += dx;
        il.y += dy;
    }

    if (wyczysc) {
        edytuj[pocz.x][pocz.y] = symbol(pocz.x, pocz.y, rozm);//
        edytuj[il.x][il.y] = symbol(il.x, il.y, rozm);//
        n -= 1;
    }
    else {
        edytuj[pocz.x][pocz.y] = cyfra(n);//piszê odpowiedni¹ CYFRÊ w miejscu oddania strza³u
        edytuj[il.x][il.y] = cyfra(n);//i w miejscu wyjœcia strza³u poza pude³ko
        n += 1;//nastêpna cyfra bêdzie o 1 wiêksza
    }
}
void LosujKulki(char** tablica, int rozmiar = 7) {
    int nKulek = 3;
    if (rozmiar == 7) nKulek = 3;
    else if (rozmiar == 10) nKulek = 5;
    else if (rozmiar == 12) nKulek = 8;
    rozmiar -= 2;
    for (int n = 0; n < nKulek; n++) {
        tablica[rand() % rozmiar + 1][rand() % rozmiar + 1] = 'O';
    }
}
void Resetuj(char** tab, char ramka = '=', int rozmiar = 7) {//aby cofn¹æ zmiany, wywo³aj 
    for (int i = 0; i < rozmiar; i++) {
        tab[i] = new char[rozmiar];
    }
    for (int x = 0; x < rozmiar; x++) {
        for (int y = 0; y < rozmiar; y++) {
            tab[x][y] = symbol(x, y, rozmiar);
        }
    }
    if (ramka != '=') {
        for (int x = 0; x < rozmiar; x++) {
            for (int y = 0; y < rozmiar; y++) {
                if (tab[x][y] != ' ') {
                    tab[x][y] = ramka;
                }
            }
        }
    }
}
void kasujStos(stack <Punkt>& stos) {
    while (!stos.empty()) {
        stos.pop();
    }
}
bool Undo(stack <Punkt>& Ustos, stack <Punkt>& Rstos, char** odp, char** klucz, int rozmiar) {
    if (Ustos.size() > 6 || Ustos.empty()) return false;
    char znak = odp[Ustos.top().x][Ustos.top().y];
    if (znak == 'o') {
        odp[Ustos.top().x][Ustos.top().y] = ' ';
    }
    else if (znak == ' ') {
        odp[Ustos.top().x][Ustos.top().y] = 'o';
    }
    else {
        StrzelLaserem(odp, klucz, Ustos.top(), true, rozmiar);
    }
    Rstos.push(Ustos.top());
    Ustos.pop();
    return true;
}
bool Redo(stack <Punkt>& Ustos, stack <Punkt>& Rstos, char** odp, char** klucz, int rozm) {
    if (Rstos.empty()) return false;
    if (odp[Rstos.top().x][Rstos.top().y] == 'o') {
        odp[Rstos.top().x][Rstos.top().y] = ' ';
    }
    else if (odp[Rstos.top().x][Rstos.top().y] == ' ') {
        odp[Rstos.top().x][Rstos.top().y] = 'o';
    }
    else {
        StrzelLaserem(odp, klucz, Rstos.top(), false, rozm);
    }
    Ustos.push(Rstos.top());
    Rstos.pop();
    return true;
}
void ZapiszRuch(Punkt p, stack <Punkt>& Ustos, char** tab) {
    if (Ustos.size() < 6) {
        Ustos.push(p);
    }
    else {
        stack <Punkt> temp;
        while (Ustos.size() > 1) {
            temp.push(Ustos.top());
            Ustos.pop();
        }
        Ustos.pop();
        while (Ustos.size() < 5) {
            Ustos.push(temp.top());
            temp.pop();
        }
        if (!temp.empty()) Ustos.push(temp.top());

    }
}
int DajWynik(char** odpowiedzi, char** klucz, int rozmiar) {//Napraw
    int odp = 0;
    for (int j = 0; j < rozmiar; j++) {
        for (int k = 0; k < rozmiar; k++) {
            if (odpowiedzi[j][k] == 'o' && klucz[j][k] != 'O') {
                odpowiedzi[j][k] = 'X';
            }
            else if (odpowiedzi[j][k] == 'o' && klucz[j][k] == 'O') {
                odpowiedzi[j][k] = 'O';
                odp += 1;
            }
        }
    }
    return odp;
}
int main()
{
    //SetConsoleOutputCP(852);//polskie kodowanie
    //
    //setlocale(LC_ALL, "pl");//w³aœciwie to znaki tabelek s¹ wa¿niejsze od polskich znaków

    //mo¿e rozwi¹zaniem by³oby w³¹czanie i wy³¹czanie polskiego kodowania w odpowiednich miejscach ?
    const int odstep = 16;
    char klawisz = 'a';
    cout << "---------THE BLACK BOX!------------\n";
    cout << "-------------a game----------------\n";
    cout << "---------By-Teresa-Stojek----------\n";
    cout << "-------------198-101---------------\n";

    //________________miejsce na eksperymentalny kod______________//

    //_______________koniec miejsca na eksperymentalny kod___________//

    _getch(); //ta funkcja zwraca naciœniêty klawisz
    printf("\033c");
    srand(time(NULL));
    Punkt kursor = { 1, 1 };

    char** odpowiedzi1 = new char* [7];
    Resetuj(odpowiedzi1, '=', 7);

    char** klucz1 = new char* [7];
    Resetuj(klucz1, 'B', 7);

    char** odpowiedzi2 = new char* [10];
    Resetuj(odpowiedzi2, 'B', 10);

    char** klucz2 = new char* [10];
    Resetuj(klucz2, 'B', 10);

    char** odpowiedzi3 = new char* [12];
    Resetuj(odpowiedzi2, 'B', 12);

    char** klucz3 = new char* [12];
    Resetuj(klucz3, 'B', 12);

    stack<Punkt> U1;
    stack<Punkt> R1;
    int liczbaKulek = 3;
    int wskazaneKulki = 0;
    int etap = 1;
    int trafione = 0;
    bool menu = false;
    Resetuj(odpowiedzi1);
    Resetuj(klucz1, 'B');
    while (liczZnaczki(klucz1, 'O') < liczbaKulek) {
        Resetuj(klucz1, 'B');
        //Losuj5x5(klucz1);
        LosujKulki(klucz1);
    }

    Resetuj(odpowiedzi1);
    Wyswietl(kursor, odpowiedzi1, odstep, wskazaneKulki, etap);
    while (1) {//g³ówna pêtla gry
        printf("\033c");//czyœci ekaran

        if (etap == 1) {
            Wyswietl(kursor, odpowiedzi1, odstep, wskazaneKulki, etap, trafione);
            wskazaneKulki = liczZnaczki(odpowiedzi1, 'o', 7);
        }
        else if (etap == 2) {
            Wyswietl(kursor, odpowiedzi2, odstep, wskazaneKulki, etap, trafione);
            wskazaneKulki = liczZnaczki(odpowiedzi2, 'o', 10);

        }
        else if (etap == 3) {
            Wyswietl(kursor, odpowiedzi3, odstep, wskazaneKulki, etap, trafione);
            wskazaneKulki = liczZnaczki(odpowiedzi3, 'o', 12);
        }

        //Zaznacz(2 * kursor.x + odstep, kursor.y);//zaznacza odpowiedni¹ pozycjê na planszy
        cout << endl;
        klawisz = _getch();
        if (klawisz == 'm' || menu) {
            menu = true;
            printf("\033c");//czyœci ekaran
            PokazMenu();
            klawisz = _getch();
            if (klawisz == 27) {//klawisz 27 to ESC - wyjœcie z gry
                delete[] odpowiedzi1;
                delete[] klucz1;
                delete[] odpowiedzi2;
                delete[] klucz2;
                delete[] odpowiedzi3;
                delete[] klucz3;
                return(0);
            }
            else if (klawisz == 'r') {//zresetuj etap
                klawisz = etap + 48; // 0 to 48 znak tabeli ascii
            }
            else if (klawisz == 'm') {//wyjdŸ z menu
                menu = false;
                printf("\033c");//czyœci ekaran
                Wyswietl(kursor, odpowiedzi1, odstep, wskazaneKulki, etap);
            }
            if (klawisz == '1') {
                menu = false;
                etap = 1;
                liczbaKulek = 3;
                wskazaneKulki = 0;
                n = 1;

                Resetuj(klucz1, 'B');
                Resetuj(odpowiedzi1, '=');
                while (liczZnaczki(klucz1, 'O') < liczbaKulek) {
                    Resetuj(klucz1, 'B');
                    //Losuj5x5(klucz1);
                    LosujKulki(klucz1);
                }
                printf("\033c");//czyœci ekaran                
                Wyswietl(kursor, odpowiedzi1, odstep, wskazaneKulki, etap);

            }
            else if (klawisz == '2') {
                menu = false;
                etap = 2;
                liczbaKulek = 5;
                wskazaneKulki = 0;
                kursor = { 0,0 };
                n = 1;

                Resetuj(klucz2, 'B', 10);
                Resetuj(odpowiedzi2, '=', 10);
                while (liczZnaczki(klucz2, 'O', 10) < liczbaKulek) {
                    Resetuj(klucz2, 'B', 10);
                    //Losuj5x5(klucz1);
                    LosujKulki(klucz2, 10);
                }

                printf("\033c");//czyœci ekaran
                Wyswietl(kursor, odpowiedzi2, odstep, wskazaneKulki, etap);
                //Nie mo¿na poszerzaæ raz stworzonej tablicy
            }
            else if (klawisz == '3') {
                menu = false;
                etap = 3;
                liczbaKulek = 8;
                wskazaneKulki = 0;
                kursor = { 0,0 };
                n = 1;
                Resetuj(klucz3, 'B', 12);
                Resetuj(odpowiedzi3, '=', 12);
                while (liczZnaczki(klucz3, 'O', 12) < liczbaKulek) {
                    Resetuj(klucz3, 'B', 12);

                    LosujKulki(klucz3, 12);
                }

                printf("\033c");//czyœci ekaran
                Wyswietl(kursor, odpowiedzi2, odstep, wskazaneKulki, etap);
            }
        }
        else if (klawisz == 's') {
            if (kursor.y < DajRozmiar(etap) - 1) kursor.y += 1;
        }
        else if (klawisz == 'w') {
            if (kursor.y > 0) kursor.y -= 1;
        }
        else if (klawisz == 'a') {
            if (kursor.x > 0) kursor.x -= 1;
        }
        else if (klawisz == 'd') {
            if (kursor.x < DajRozmiar(etap) - 1) kursor.x += 1;
        }
        else if (klawisz == 'h') {
            printf("\033c");//czyœci ekaran
            if (etap == 1)  Wyswietl(kursor, klucz1, odstep, wskazaneKulki, etap);
            else if (etap == 2) Wyswietl(kursor, klucz2, odstep, wskazaneKulki, etap);
            else if (etap == 3) Wyswietl(kursor, klucz3, odstep, wskazaneKulki, etap);
            klawisz = _getch();

            continue;
        }
        else if (klawisz == 'k') {
            printf("\033c");//czyœci ekaran
            if (etap == 1) {
                trafione = DajWynik(odpowiedzi1, klucz1, 7);
                Wyswietl(kursor, odpowiedzi1, odstep, wskazaneKulki, etap, trafione);
                Odstep(odstep);
            }
            else if (etap == 2) {
                trafione = DajWynik(odpowiedzi2, klucz2, 10);
                Wyswietl(kursor, odpowiedzi2, odstep, wskazaneKulki, etap, trafione);
                Odstep(odstep);
            }
            else if (etap == 3) {
                trafione = DajWynik(odpowiedzi3, klucz3, 12);
                Wyswietl(kursor, odpowiedzi3, odstep, wskazaneKulki, etap, trafione);
                Odstep(odstep);
            }
        }
        else if (klawisz == 'u') {
            Undo(U1, R1, odpowiedzi1, klucz1, 7);
            wskazaneKulki = liczZnaczki(odpowiedzi1, 'o');
            continue;
        }
        else if (klawisz == 'r') {
            Redo(U1, R1, odpowiedzi1, klucz1, 7);
            wskazaneKulki = liczZnaczki(odpowiedzi1, 'o');
        }
        else if (klawisz == 'o' && 0 < kursor.x < DajRozmiar(etap) && 0 < kursor.y < DajRozmiar(etap)) {
            if (etap == 1) {
                if (odpowiedzi1[kursor.x][kursor.y] == 'o') {
                    odpowiedzi1[kursor.x][kursor.y] = ' ';
                    ZapiszRuch(kursor, U1, odpowiedzi1);
                }
                else if (wskazaneKulki < liczbaKulek) {
                    odpowiedzi1[kursor.x][kursor.y] = 'o';
                    ZapiszRuch(kursor, U1, odpowiedzi1);
                }
            }
            else if (etap == 2) {
                if (odpowiedzi2[kursor.x][kursor.y] == 'o') {
                    odpowiedzi2[kursor.x][kursor.y] = ' ';
                    ZapiszRuch(kursor, U1, odpowiedzi2);
                }
                else if (wskazaneKulki < liczbaKulek) {
                    odpowiedzi2[kursor.x][kursor.y] = 'o';
                    ZapiszRuch(kursor, U1, odpowiedzi2);
                }

            } 
            else if (etap == 3) {
                if (odpowiedzi3[kursor.x][kursor.y] == 'o') {
                    odpowiedzi3[kursor.x][kursor.y] = ' ';
                    ZapiszRuch(kursor, U1, odpowiedzi3);
                }
                else if (wskazaneKulki < liczbaKulek) {
                    odpowiedzi3[kursor.x][kursor.y] = 'o';
                    ZapiszRuch(kursor, U1, odpowiedzi3);
                }
            }
            kasujStos(R1);
        }
        else if (klawisz == ' ' && (kursor.x == 0 || kursor.x == DajRozmiar(etap) - 1 || kursor.y == 0 || kursor.y == DajRozmiar(etap) - 1)) {

            //tam gdzie ramka (w klucz1u zaznaczona literk¹ b)
            //muszê wymyœleæ jak zrobiæ liczby dwucyfrowe
            //plansza[kursor.x][kursor.y] = 48 + n;// 0 to 48 znak tabeli ascii
            //n += 1;
            bool rog = ((kursor.x == 0 or kursor.x == DajRozmiar(etap) - 1) && (kursor.y == 0 or kursor.y == DajRozmiar(etap) - 1));
            if (!rog) {
                if (etap == 1) {
                    Punkt koniec = StrzelLaserem(odpowiedzi1, klucz1, kursor, false, 7);
                    ZapiszRuch(kursor, U1, odpowiedzi1);
                }
                else if (etap == 2) {
                    Punkt koniec = StrzelLaserem(odpowiedzi2, klucz2, kursor, false, 10);
                    ZapiszRuch(kursor, U1, odpowiedzi2);
                }
                else if (etap == 3) {
                    Punkt koniec = StrzelLaserem(odpowiedzi3, klucz3, kursor, false, 12);
                    ZapiszRuch(kursor, U1, odpowiedzi3);
                }
                cout << "LASERR";
                //dodajDoHistorii(historia, odpowiedzi1);
            }
            kasujStos(R1);
        }
    }
}

