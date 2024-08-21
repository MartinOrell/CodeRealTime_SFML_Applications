#pragma once

#include <SFML/Graphics.hpp>

class Gui{
    public:
        enum Event{Void, End, DiceRoll};

        Gui();
        void setDiceValue(int);
        Event update();
    private:
        
        sf::RenderWindow _window;
        sf::Font _font;
        sf::Text _instructionText;
        sf::Text _diceText;
        sf::RectangleShape _diceRect;
};