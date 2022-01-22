#include "naglowek.h"
#include <SFML/Graphics.hpp>
#include <list>
#include <string> 
#include <iostream>
#include <fstream>

using namespace std;
using namespace sf;


struct SnakeSegment {
	int x;
	int y;
};

const int snake_rozmiar = 25;
int snake_kierunek = 3;
int snake_predkosc = 110;

bool czyKoniec = false;
int punkty = 0;

int punktX = 300;
int punktY = 400;

const int screenWidth = 700;
const int screenHeight = 600;

const Color snake_color = Color::Color(255, 152, 10);
const Color bg_color = Color::Color(5, 3, 15);
const Color food_color = Color::Color(187, 3, 230);
const Color txt_color = Color::Color(255, 255, 255);
list<SnakeSegment> snake = { {350,300}, {375,300}, {400,300}, {425,300},{450,300 } };


void czyDobryWynik() {

	ifstream infile;
	infile.open("wyniki.txt");
	int lastScore = 0;
	int index = 0;
	string str;
	// sprawdza ostatni wynik, oraz liczy ile wynikow znajduje sie obecnie w tabeli wynikow
	while (getline(infile, str)) {
		index++;
		lastScore = stoi(str.substr(str.find(" ") + 1, str.size() - 1));
	}

	// jesli obecny wynik jest wiekszy od ostatniego wyniku z tabeli wynikow, albo w tabeli  jest mniej niz 10 zapisanych osob otwiera okno *dodajNazweUzytkownika*
	if (punkty > lastScore || index < 10) { 
		dodajNazweUzytkownika(punkty);
	}
}

void koniecGry(Event event) {
	if (czyKoniec && event.key.code == Keyboard::Space) { // 'SPACE' - od nowa
		punkty = 0;
		punktX = 300;
		punktY = 400;
		snake_kierunek = 3;
		snake = { {350,300}, {375,300}, {400,300}, {425,300},{450,300 } };
		czyKoniec = false;
		snake_predkosc = 110;
	}
	else if (czyKoniec && event.key.code == Keyboard::Enter) { // 'ENTER' - pokaz tablice wynikow
		pokazTabliceWynikow();
	}
}


void sterowanie(Event event) {

	switch (event.key.code) {
	case Keyboard::W:
		if (snake_kierunek != 2) {
			snake_kierunek = 0;
		}
		break;
	case Keyboard::A:
		if (snake_kierunek != 1) {
			snake_kierunek = 3;
		}
		break;
	case Keyboard::S:
		if (snake_kierunek != 0) {
			snake_kierunek = 2;
		}
		break;
	case Keyboard::D:
		if (snake_kierunek != 3) {
			snake_kierunek = 1;
		}
		break;
	}
}


void chodzenie() {
	switch (snake_kierunek) {
	case 0: // gora
		snake.push_front({ snake.front().x, snake.front().y - snake_rozmiar });
		break;
	case 1: // prawo
		snake.push_front({ snake.front().x + snake_rozmiar, snake.front().y });
		break;
	case 2: // dol
		snake.push_front({ snake.front().x, snake.front().y + snake_rozmiar });
		break;
	case 3: // lewo
		snake.push_front({ snake.front().x - snake_rozmiar , snake.front().y });
		break;
	}

	// sprawdza czy nie wyszedl poza mape
	if (snake.front().x <0 || snake.front().x + snake_rozmiar > screenWidth) {
		czyKoniec = true;
		czyDobryWynik();
	}
	if (snake.front().y <0 || snake.front().y + snake_rozmiar > screenHeight) {
		czyKoniec = true;
		czyDobryWynik();
	}

	// sprawdza czy nie wjechal sam w siebie
	for (list<SnakeSegment>::iterator i = snake.begin(); i != snake.end(); i++) {
		if (i != snake.begin() && i->x == snake.front().x && i->y == snake.front().y) {
			czyKoniec = true;
			czyDobryWynik();
		}
	}

	// usun koniec
	if (!czyKoniec) {
		snake.pop_back();
	}
}


// trzeba uruchamiac Release x64, bo cos sfml sie psuje ;/
int main() {

	RenderWindow window(VideoMode(screenWidth, screenHeight), "Snake");

	// sfml petla
	while (window.isOpen()) {

		// opoznienie
		sf::sleep(sf::Time(sf::milliseconds(snake_predkosc)));


		sf::Event event;
		while (window.pollEvent(event)) {

			if (event.type == sf::Event::Closed) {
				window.close();
			}
			else if (event.type == sf::Event::KeyPressed) {

				// koniec gry menu wyboru
				koniecGry(event);

				// sterowanie WSAD
				sterowanie(event);

			}
		}

		// jesli gra sie zakonczy, wyswietla sie menu *koniecGry*
		if (!czyKoniec) {

			window.clear(bg_color);

			// przesun snake'a i sprawdz czy nie koniec gry 
			chodzenie();

			bool ustawPunkt = false;

			if (snake.front().x == punktX && snake.front().y == punktY) {

				if (snake_predkosc >= 80) {
					snake_predkosc -= 1; // zwieksz predkosc, jesli zdobedzie punkt,
				}
				punkty++;

				// ustawia punkt na planszy
				while (!ustawPunkt)
				{
					punktX = (rand() % (screenWidth / 25)) * 25;
					punktY = (rand() % (screenHeight / 25)) * 25;

					if (snake.front().x != punktX && snake.front().y != punktY) {
						ustawPunkt = true;
					}

					for (auto s : snake) {
						if (s.x != punktX && s.y != punktY && ustawPunkt) {
							ustawPunkt = true;
						}
						else {
							ustawPunkt = false;
						}
					}
				}
				snake.push_back({ snake.back().x, snake.back().y }); // dodaj dlugosc postaci
			}


			// postac
			for (auto s : snake) {
				sf::RectangleShape part;
				part.setPosition(s.x, s.y);
				part.setSize(sf::Vector2f{ snake_rozmiar, snake_rozmiar });
				part.setFillColor(snake_color);
				window.draw(part);
			}

			// punkt na planszy 
			sf::RectangleShape food_shape;
			food_shape.setPosition(punktX, punktY);
			food_shape.setFillColor(food_color);
			food_shape.setSize(sf::Vector2f{ snake_rozmiar, snake_rozmiar });
			window.draw(food_shape);

		}
		else {
			Font font;
			Text txt;
			font.loadFromFile("PressStart2P-Regular.ttf");
			txt.setFont(font);
			txt.setString("Score: " + to_string(punkty));
			txt.setCharacterSize(32);
			txt.setFillColor(txt_color);
			txt.setOrigin(txt.getGlobalBounds().width / 2, txt.getGlobalBounds().height / 2);
			txt.setPosition(screenWidth / 2, screenHeight / 2);
			window.draw(txt);

			Text txt2;
			txt2.setFont(font);
			txt2.setString("Press \'Space\' to play again");
			txt2.setCharacterSize(15);
			txt2.setFillColor(txt_color);
			txt2.setOrigin(txt2.getGlobalBounds().width / 2, txt2.getGlobalBounds().height / 2);
			txt2.setPosition(screenWidth / 2, (screenHeight / 2) + 100);
			window.draw(txt2);

			Text txt3;
			txt3.setFont(font);
			txt3.setString("Press \'Enter\' to see leaderboard");
			txt3.setCharacterSize(15);
			txt3.setFillColor(txt_color);
			txt3.setOrigin(txt3.getGlobalBounds().width / 2, txt3.getGlobalBounds().height / 2);
			txt3.setPosition(screenWidth / 2, (screenHeight / 2) + 135);
			window.draw(txt3);
		}

		window.display();
	}

	return 0;
}


