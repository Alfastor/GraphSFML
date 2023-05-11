#include<cmath>
#include<SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#pragma warning(disable : 4996)

using namespace sf;

// Определение специальных клавиш
#define DELETE_KEY 8
#define ENTER_KEY 13
#define ESCAPE_KEY 27


// Класс текст-бокса
class Textbox {
public:
	Textbox(int size, sf::Color color, bool sel) { // Конструктор класса
		textbox.setCharacterSize(size);
		textbox.setColor(color);
		isSelected = sel;

		// Проверка на то, выбран ли текст-бокс при его создании и вставка соответствующего знака если это так
		if (isSelected)
			textbox.setString("_");
		else
			textbox.setString("");
	}

	// Функция для установки шрифта
	void setFont(sf::Font& fonts) {
		textbox.setFont(fonts);
	}

	// Функция для установки позиции
	void setPosition(sf::Vector2f point) {
		textbox.setPosition(point);
	}

	// Установка ограничения на количество символов
	void setLimit(bool ToF, int lim) {
		hasLimit = ToF;
		limit = lim - 1;
	}

	// Переключение между выбором и не-выбором текст-бокса
	void setSelected(bool sel) {
		isSelected = sel;

		// Когда текст-бокс не выбран - удалить знак "_"
		if (!sel) {
			std::string t = text.str();
			std::string newT = "";
			for (int i = 0; i < t.length(); i++) {
				newT += t[i];
			}
			textbox.setString(newT);
		}
	}

	std::string getText() {
		return text.str();
	}

	void drawTo(sf::RenderWindow& window) {
		window.draw(textbox);
	}

	// Функция для записи данных в текст-бокс
	void typedOn(sf::Event input) {
		if (isSelected) {
			int charTyped = input.text.unicode;

			if (charTyped < 128) {
				if (hasLimit) {
					// Проверка на лимит ввода
					if (text.str().length() <= limit) {
						inputLogic(charTyped);
					}
					// Позволяет удалять символы 
					else if (text.str().length() > limit && charTyped == DELETE_KEY) {
						deleteLastChar();
					}
				}
				// Если лимит отсутствует, то просто ввести данные
				else {
					inputLogic(charTyped);
				}
			}
		}
	}
private:
	sf::Text textbox;

	std::ostringstream text;
	bool isSelected = false;
	bool hasLimit = false;
	int limit = 0;

	// Функция для удаления последнего знака
	void deleteLastChar() {
		std::string t = text.str();
		std::string newT = "";
		for (int i = 0; i < t.length() - 1; i++) {
			newT += t[i];
		}
		text.str("");
		text << newT;
		textbox.setString(text.str() + "_");
	}

	// Получение ввода пользователя
	void inputLogic(int charTyped) {

		// Если введенная клавиша не явлется специальной - ввести знак
		if (charTyped != DELETE_KEY && charTyped != ENTER_KEY && charTyped != ESCAPE_KEY) {
			text << static_cast<char>(charTyped);
		}
		// Если нажат backspace - удалить последний знак
		else if (charTyped == DELETE_KEY) {
			if (text.str().length() > 0) {
				deleteLastChar();
			}
		}
		// Запись текста в текст-бокс
		textbox.setString(text.str() + "_");
	}
}; 

// Класс кнопки
class Button {
public:
	Button(std::string btnText, sf::Vector2f buttonSize, int charSize, sf::Color bgColor, sf::Color textColor) { // Конструктор класса
		button.setSize(buttonSize);
		button.setFillColor(bgColor);

		btnWidth = buttonSize.x;
		btnHeight = buttonSize.y;

		text.setString(btnText);
		text.setCharacterSize(charSize);
		text.setColor(textColor);
	}

	// Функция для установки шрифта
	void setFont(sf::Font& fonts) {
		text.setFont(fonts);
	}

	// Функция для установки цвета самой кнопки
	void setBackColor(sf::Color color) {
		button.setFillColor(color);
	}

	// Функция для установки цвета текста
	void setTextColor(sf::Color color) {
		text.setColor(color);
	}

	// Функция для установки позиции
	void setPosition(sf::Vector2f point) {
		button.setPosition(point);

		// Ставит текст кнопки в её центр
		float xPos = (point.x + btnWidth / 2) - (text.getLocalBounds().width / 2);
		float yPos = (point.y + btnHeight / 2.2) - (text.getLocalBounds().height / 2);
		text.setPosition(xPos, yPos);
	}

	// Функция для вывода кнопки в окно
	void drawTo(sf::RenderWindow& window) {
		window.draw(button);
		window.draw(text);
	}

	// Функция для проверки на то, наведена ли в данный момент времени мышка на кнопку
	bool isMouseOver(sf::RenderWindow& window) {
		int mouseX = sf::Mouse::getPosition(window).x;
		int mouseY = sf::Mouse::getPosition(window).y;

		int btnPosX = button.getPosition().x;
		int btnPosY = button.getPosition().y;

		int btnxPosWidth = button.getPosition().x + btnWidth;
		int btnyPosHeight = button.getPosition().y + btnHeight;

		if (mouseX < btnxPosWidth && mouseX > btnPosX && mouseY < btnyPosHeight && mouseY > btnPosY) {
			return true;
		}
		return false;
	}
private:
	sf::RectangleShape button;
	sf::Text text;

	int btnWidth;
	int btnHeight;
};

// Функция, рисующая графики
void DrawFunc(int f, int a, int b, float c)
{
	const int width = 800;
	const int height = 800;

	int x0 = width / 2; // Нулевая точка по x
	int y0 = height / 2; // Нулевая точка по y 
	float addots = 800; // Дополнительное кол-во точек в графике (по факту то, насколько график плавный, чем больше точек тем он плавнее)
	int size = 50; 
	int left = -10; // Область определения графика слева и справо
	int right = 10;
	int amount = (left * (-1) + right) * addots + 1; // Формула для рассчёта нужного количества точек, чтобы вывести график на экран
	float y;

	// Размер и цвет точки
	CircleShape dot(2);
	dot.setFillColor(Color::Black);

	RenderWindow window(VideoMode(width, height), "User Input Function Graph");

	RectangleShape line[40];

	// Цикл для отображения единичных отрезков
	for (int i = 0; i < 40; i++) {

		line[i].setSize(Vector2f(1, 20));
		line[i].setFillColor(Color::Black);

		if (i < 20) {
			if (i < 10)
				line[i].setPosition(x0 - (i + 1) * size, y0 - 10);
			else
				line[i].setPosition(x0 + (i - 9) * size, y0 - 10);
		}
		else {
			line[i].setRotation(90);
			if (i < 30)
				line[i].setPosition(x0 + 10, y0 + (i - 30) * size);
			else
				line[i].setPosition(x0 + 10, y0 + (i - 29) * size);
		}
	}

	// Ось X
	RectangleShape OsX(Vector2f(width, 1));
	OsX.setFillColor(Color::Black);
	OsX.setPosition(0, y0);

	// Ось Y
	RectangleShape OsY(Vector2f(1, height));
	OsY.setFillColor(Color::Black);
	OsY.setPosition(x0, 0);

	// Основной цикл
	while (window.isOpen())
	{
		Event event;

		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed) // Позволяет закрыть программу
			{
				window.close();
			}
		}

		window.clear(Color::White);
		window.draw(OsX);
		window.draw(OsY);

		for (int i = 0; i < 40; i++) // Отображение единичных отрезков
		{
			if (i != 19 && i != 20)
			{
				window.draw(line[i]);
			}
		}

		for (int i = 0; i < amount; i++) // Цикл для вывода графика на экран
		{
			float x = left + i / addots;

			// Число f определяет, какой график будет выведен
			if (f == 1) {
				y = (((x + a) * (x + a)) * c) + b; //парабола
			}
			else if (f == 2) {
				y = (((x + a)*(x + a)*(x + a))*c) + b;//x в степени 3
			}
			else if (f == 3) {
				y = ((1/(x+a)) * c) + b;//гипербола
			}
			else if (f == 4) {
				y = ((x+a) * c) + b;//прямая
			}
			else if (f == 5) {
				y = (sin(x+a) * c) + b;//синусоида
			}
			else if (f == 6) {
				y = (cos(x + a) * c) + b;//косинусоида
			}
			else if (f == 7) {
				y = (tan(x + a) * c) + b;//тангенсоида
			}
			else if (f == 8) {
				y = (sqrt(x+a) * c) + b;//корень
			}
			else if (f == 9) {
				y = (pow(a,x) * c) + b;//корень
			}

			float x1 = x0 + x * size; // Координаты точки выводимой на экран
			float y1 = y0 - y * size;

			dot.setPosition(x1 - 1, y1 - 1);
			window.draw(dot);
		}
		window.display();
	}
}

int main() {
	
	int w = 400; // ширина окна
	int h = 400; // высота окна
	int func; 
	int a;
	int b;
	float c;

	RenderWindow window1(VideoMode(w, h), "User input");

	// Объявление шрифта
	Font arial;
	arial.loadFromFile("arial.ttf");

	// Объявление прямоугольников
	RectangleShape box1({ 75,50 }); 
	box1.setPosition({ 25,25 });
	box1.setFillColor(Color::White);
	box1.setOutlineColor(Color::Black);
	box1.setOutlineThickness(5);

	RectangleShape box2({ 75,50 }); 
	box2.setPosition({ 25,125 });
	box2.setFillColor(Color::White);
	box2.setOutlineColor(Color::Black);
	box2.setOutlineThickness(5);

	RectangleShape box3({ 75,50 }); 
	box3.setPosition({ 25,225 });
	box3.setFillColor(Color::White);
	box3.setOutlineColor(Color::Black);
	box3.setOutlineThickness(5);

	RectangleShape box4({ 75,50 }); 
	box4.setPosition({ 25,325 });
	box4.setFillColor(Color::White);
	box4.setOutlineColor(Color::Black);
	box4.setOutlineThickness(5);

	// Объявление кнопок
	Button btn1("", { 75,50 }, 0, Color::Black, Color::Black);
	btn1.setPosition({ 25,25 });

	Button btn2("", { 75,50 }, 0, Color::Black, Color::Black);
	btn2.setPosition({ 25,125 });

	Button btn3("", { 75,50 }, 0, Color::Black, Color::Black);
	btn3.setPosition({ 25,225 });

	Button btn4("", { 75,50 }, 0, Color::Black, Color::Black);
	btn4.setPosition({ 25,325 });

	Button EnterButton("ENTER", { 120,75 }, 20, Color::Black, Color::White);
	EnterButton.setFont(arial);
	EnterButton.setPosition({ 250,300 });



	//Объявление текст-боксов
	Textbox textbox1(50, Color::Black, false);
	textbox1.setFont(arial);
	textbox1.setPosition({ 35.0f, 15.0f });
	textbox1.setLimit(true, 1);

	Textbox textbox2(50, Color::Black, false);
	textbox2.setFont(arial);
	textbox2.setPosition({ 35.0f, 115.0f });
	textbox2.setLimit(true, 2);

	Textbox textbox3(50, Color::Black, false);
	textbox3.setFont(arial);
	textbox3.setPosition({ 35.0f, 215.0f });
	textbox3.setLimit(true, 2);

	Textbox textbox4(50, Color::Black, false);
	textbox4.setFont(arial);
	textbox4.setPosition({ 35.0f, 315.0f });
	textbox4.setLimit(true, 2);

	Text TextF;
	TextF.setFont(arial);
	TextF.setString("Which function do\nyou want to display");
	TextF.setColor(Color::Black);
	TextF.setScale({ 0.65,0.65 });
	TextF.setPosition({ 115,25 });

	Text TextA;
	TextA.setFont(arial);
	TextA.setString("Number A");
	TextA.setColor(Color::Black);
	TextA.setScale({ 0.65,0.65 });
	TextA.setPosition({ 115,135 });

	Text TextB;
	TextB.setFont(arial);
	TextB.setString("Number B");
	TextB.setColor(Color::Black);
	TextB.setScale({ 0.65,0.65 });
	TextB.setPosition({ 115,235 });

	Text TextC;
	TextC.setFont(arial);
	TextC.setString("Number C");
	TextC.setColor(Color::Black);
	TextC.setScale({ 0.65,0.65 });
	TextC.setPosition({ 115,335 });

	// Основной цикл
	while (window1.isOpen())
	{
		Event event;

		//По нажатию кнопки backspace снимает выделение со всех текст-боксов
		if (Keyboard::isKeyPressed(Keyboard::Return))
		{
			textbox1.setSelected(false);
			textbox2.setSelected(false);
			textbox3.setSelected(false);
			textbox4.setSelected(false);
		}

		while (window1.pollEvent(event))
		{

			switch (event.type)
			{
			case Event::Closed: // Позволяет закрыть окно
				window1.close();
			case Event::TextEntered: // По нажатию любой кнопки на клавиатуре - пробует ввести данный символ во все текст-боксы сразу, но вводит только в те, что выделены пользователем в данный момент
				textbox1.typedOn(event);
				textbox2.typedOn(event);
				textbox3.typedOn(event);
				textbox4.typedOn(event);
			case Event::MouseButtonPressed: // Проверяет нажал ли кнопку мыши пользователь и если нажал - проверяет, находится ли мышка в данный момент на любой из кнопок, и если да то считает данную кнопку нажатой. После чего выделяет текст-бокс под которым данная кнопка находится (кнопки невидимые). Сделано это для того чтобы текст-боксы можно было выбирать нажатием мыши
				if (btn1.isMouseOver(window1)) {
					textbox1.setSelected(true);
				}
				else if (btn2.isMouseOver(window1)) {
					textbox2.setSelected(true);
				}
				else if (btn3.isMouseOver(window1)) {
					textbox3.setSelected(true);
				}
				else if (btn4.isMouseOver(window1)) {
					textbox4.setSelected(true);
				}
				else if (EnterButton.isMouseOver(window1)) { // Таким же образом считывает нажатие мышкой на кнопку, и вызывает функцию которая рисует график с введенными данными
					func = std::stoi(textbox1.getText());
					a = std::stoi(textbox2.getText()); // Перевод данных (полученных из текст-боксов) из std::string в int
					b = std::stoi(textbox3.getText());
					c = std::stof(textbox4.getText());
					DrawFunc(func, a, b, c);

				}

			}

			window1.clear(Color::White);


			window1.draw(box1);
			textbox1.drawTo(window1);
			window1.draw(box2);
			textbox2.drawTo(window1);
			window1.draw(box3);
			textbox3.drawTo(window1);
			window1.draw(box4);
			textbox4.drawTo(window1);

			EnterButton.drawTo(window1);

			window1.draw(TextF);
			window1.draw(TextA);
			window1.draw(TextB);
			window1.draw(TextC);

			window1.display();

		}
	}

}