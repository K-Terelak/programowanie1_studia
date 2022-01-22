#include "naglowek.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>

using namespace std;
using namespace sf;


const Color bg_color = Color::Color(14, 5, 33);
const Color txt_color = Color::Color(255, 152, 10);
const Color txt_color_alpha_50 = Color::Color(255, 152, 10, 75);

Font font;
ifstream infile;
Event event;


void pokazTabliceWynikow() {
	RenderWindow window(VideoMode(500, 500), "Tabela wynikow!");
	while (window.isOpen()) {
		while (window.pollEvent(event)) {

			if (event.type == Event::Closed || event.type == Event::KeyPressed) {
				window.close();
			}
			window.clear(bg_color);

			infile.open("wyniki.txt");

			for (int i = 1; i <= 10; i++) {
				string STRING;
				getline(infile, STRING);
				font.loadFromFile("PressStart2P-Regular.ttf");
				Text txt;
				txt.setFont(font);
				txt.setString(to_string(i) + ": " + STRING);
				txt.setCharacterSize(24);
				txt.setFillColor(txt_color);
				txt.setOrigin(txt.getGlobalBounds().width / 2, txt.getGlobalBounds().height / 2);
				txt.setPosition(500 / 2, i * 40);
				window.draw(txt);
			}
			infile.close();

			Text txtExit;
			txtExit.setFont(font);
			txtExit.setString("Press any key to exit.");
			txtExit.setCharacterSize(12);
			txtExit.setFillColor(txt_color_alpha_50);
			txtExit.setOrigin(txtExit.getGlobalBounds().width / 2, txtExit.getGlobalBounds().height / 2);
			txtExit.setPosition(500 / 2, 450);
			window.draw(txtExit);

			window.display();
		}
	}
}