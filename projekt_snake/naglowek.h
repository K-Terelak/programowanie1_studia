#pragma once
#include <SFML/Graphics.hpp>


extern struct SnakeSegment;

extern const int snake_rozmiar;
extern int snake_kierunek;
extern int snake_predkosc;

extern bool czyKoniec;
extern int punkty;

extern int punktX;
extern int punktY;

extern const int screenWidth;
extern const int screenHeight;

/*
* wyswietla okno, w ktorym mozna dodac nazwe uzytkownika
*/
void dodajNazweUzytkownika(int);

/*
* wyswietla okno w ktorym wyswietlana jest tablica najlepszych wynikow
*/
void pokazTabliceWynikow();

/*
* po zakonczonej rozgrywce, czeka az uzytkownika nacisnie:
* 'SPACJA' - nowa gra
* 'ENTER' - wyswietla *pokazTabliceWynikow*
*/
void koniecGry(sf::Event);

/*
* sterowanie postacia:
* 'W' - gora,
* 'S' - dol,
* 'A' - lewo,
* 'D' - prawo
*/
void sterowanie(sf::Event);

/*
*  przesuwa postac na planszy
* oraz sprawdza czy postac nie wykryla kolizji
*/
void chodzenie();

/*
* sprawdza czy po zakonczonej grze zapisac wynik,
* jesli tak otwiera 'dodajNickname'
*/
void czyDobryWynik();
