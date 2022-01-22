#include "naglowek.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>

using namespace std;
using namespace sf;


const Color bg_color = Color::Color(5, 3, 15);
const Color txt_color = Color::Color(255, 152, 10);
const Color txt_color_alpha_50 = Color::Color(255, 152, 10, 75);

// sortowanie tabeli wynikow
template <typename A, typename B>
multimap<B, A> flip_map(map<A, B>& src) {

	multimap<B, A> dst;

	for (typename map<A, B>::const_iterator it = src.begin(); it != src.end(); ++it)
		dst.insert(pair<B, A>(it->second, it->first));

	return dst;
}

/*
* zapisuje 10 najlepszych wynikow do pliku wynik.txt
* sprawdza czy podana 'nazwaUzytkownika' nie jest juz zajeta
* jesli jest zajeta zwraca false, w przeciwnym razie true(oraz zapisuje do pliku 10 najlepszych wynikow)
*/
bool zapiszDoPliku(string nazwaUzytkownika, int wynik) {
	map<string, int> my_map = {};

	ifstream inPlik;
	inPlik.open("wyniki.txt");
	string str;
	// dodanie do mapy nazwy uzytkownikow oraz ich wyniki
	while (getline(inPlik, str)) {
		// dzieli linie z pliku na 'nazweUzytkownika' oraz jego 'wynik'
		string nazwa = (str.substr(0, str.find(" ")));
		int wynik = stoi(str.substr(str.find(" ") + 1, str.size() - 1));
		my_map[nazwa] = wynik;
		if (nazwaUzytkownika == nazwa) return false;
	}
	inPlik.close();

	my_map[nazwaUzytkownika] = wynik;

	multimap<int, string> reverseTest = flip_map(my_map);

	//zapis 
	ofstream outPlik;
	int licznik = 0;
	outPlik.open("wyniki.txt");
	for (multimap<int, string>::const_reverse_iterator it = reverseTest.rbegin(); it != reverseTest.rend(); ++it) {
		if (licznik >= 10) { // zeby zapisalo tylko 10 wynikow
			break;
		}
		else {
			outPlik << it->second << " " << it->first << "\n";
			licznik++;
		}
	}
	outPlik.close();

	return true;

}


void dodajNazweUzytkownika(int wynik) {

	RenderWindow window(sf::VideoMode(500, 250), "Brawo");

	string nickname = "anonim";

	Font font;
	font.loadFromFile("PressStart2P-Regular.ttf");

	Text txtInfo;
	txtInfo.setFont(font);
	txtInfo.setCharacterSize(14);
	txtInfo.setFillColor(txt_color);
	txtInfo.setLineSpacing(1.5);
	txtInfo.setPosition(10, 10);
	txtInfo.setString("No niezle, calkiem dobry wynik.\nPodaj swoj nick to zapiszemy ciebie\nna liste najlepszych graczy!\n(max 10 znakow)");

	Text txt;
	txt.setFont(font);
	txt.setCharacterSize(32);
	txt.setFillColor(txt_color);
	txt.setPosition(140, 150);
	txt.setString(nickname);


	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {


			window.clear(bg_color);

			window.draw(txtInfo);

			/*
			* uzytkownik podaje znaki a-z
			* max 10 znakow
			*/
			if (event.type == Event::TextEntered) {

				if (event.text.unicode >= 'a' && event.text.unicode <= 'z' && nickname.size() < 10) { // a-z
					nickname += event.text.unicode;
					txt.setString(nickname);
				}
				else if (event.text.unicode == 8) { // backspace - usuwa ostatni znak
					if (!nickname.empty()) {
						nickname.pop_back();
						txt.setString(nickname);
					}
				}
				// centruje text w oknie po wpisaniu znaku
				FloatRect textRect = txt.getLocalBounds();
				txt.setOrigin(textRect.left + textRect.width / 2.0f, 0);
				txt.setPosition(500 / 2.0f, 150);
			}
			window.draw(txt);

			Text txtExit;
			txtExit.setFont(font);
			txtExit.setString("Press 'ENTER' to save & exit.");
			txtExit.setCharacterSize(12);
			txtExit.setFillColor(txt_color_alpha_50);
			txtExit.setOrigin(txtExit.getGlobalBounds().width / 2, txtExit.getGlobalBounds().height / 2);
			txtExit.setPosition(500 / 2, 200);

			/*
			* jesli uzytkownik nacisnie enter
			* wywoluje sie funkcja 'zapiszDoPliku' ktora zwraca true/false
			* jestli true, zamyka okno(zapisano uzytkownika)
			* jesli false, informuje uzytkownika ze podana nazwaUzytkownika jest zajeta
			*/ 
			if (event.key.code == Keyboard::Enter) {
				if (zapiszDoPliku(nickname, wynik)) {
					window.close();
				}
				else {
					txtExit.setString("Zajeta nazwa uzytkownika");
				}
			}

			if (event.type == Event::Closed) {
				window.close();
			}
			window.draw(txtExit);

			window.display();
		}
	}
}