#include "Gui.h"

#include <SFML/Window/Event.hpp>
#include <iostream>

using namespace gui;

UI::UI(int goal)
: _window{ { 1920u, 1080u }, "Race" }
{
    _goal = goal;
    _font.loadFromFile("res/fonts/arial.ttf");

    _window.setFramerateLimit(144);

    _trackWidth = _window.getSize().x;
    _trackHeight = 500.f;
    _trackYPos = (_window.getSize().y-_trackHeight)/2.f;

    _startWidth = 50.f;
    _goalWidth = 100.f;
    _raceWidth = _window.getSize().x-_startWidth-_goalWidth+1;//+1 is for goal touch on finish

    _edgeLineWidth = 50.f;
    _edgeLineHeight = 20.f;
    _edgeLineXOffset = 20.f;
    _edgeLineYOffset = 10.f;

    _middleLineWidth = 200.f;
    _middleLineHeight = 20.f;
    _middleLineXOffset = 50.f;
    _middleLineYPos = _trackYPos+(_trackHeight-_middleLineHeight)/2.f;

    _startLineWidth = 20.f;
    _goalLineWidth = 20.f;
}

void UI::addRacer(int id, std::string filename){
    sf::RectangleShape racerShape;
    racerShape.setSize(sf::Vector2f(100.f,100.f));
    racerShape.setFillColor(sf::Color(255,255,255));
    racerShape.setPosition(sf::Vector2f(10.f, 10.f));

    auto texture = std::make_unique<sf::Texture>();
    if(texture->loadFromFile(filename))
    {
        racerShape.setTexture(&(*texture));
        _textures.push_back(std::move(texture));
    }
    else{
        std::cout << "Failed to load from file " << filename << std::endl;
    }

    Racer racer;
    racer.id = id;
    racer.shape = racerShape;
    _racers.push_back(racer);
}

void UI::initPrint(){
    initRacers();   
    initTrack();
    initText();
}

void UI::initText(){
    _displayText.setFont(_font);
    _displayText.setCharacterSize(100);
    _goalText.setFillColor(sf::Color(255,255,255));
}

void UI::setText(std::string text){
    _displayText.setString(text);
    sf::FloatRect textRect = _displayText.getLocalBounds();
    float x = textRect.left + textRect.width/2.f;
    float y = textRect.top + textRect.height/2.f;
    _displayText.setOrigin(x,y);
    float xPos = _window.getView().getSize().x/2.f;
    float yPos = (_window.getView().getSize().y - _trackHeight)/4.f;
    _displayText.setPosition(xPos,yPos);
}

gui::Event UI::update(std::vector<int> stepPositions){
    auto returnEvent = gui::Event::Void;
    for (auto event = sf::Event{}; _window.pollEvent(event);)
    {
        switch(event.type){
            case sf::Event::Closed:
                _window.close();
                return Event::Exit;
            case sf::Event::MouseButtonReleased:
                returnEvent = gui::Event::Click;
                break;
        }
    }

    _window.clear(sf::Color(100,100,100));

    for(int i = 0; i < _track.size(); i++){
        _window.draw(_track.at(i));
    }
    for(int i = 0; i < _racers.size(); i++){
        float x = _startWidth - _racers.at(i).shape.getSize().x + (stepPositions.at(i)*_raceWidth)/_goal;
        float y = _racers.at(i).shape.getPosition().y;
        _racers.at(i).shape.setPosition(sf::Vector2f(x,y));
        _window.draw(_racers.at(i).shape);
    }
    _window.draw(_goalText);
    _window.draw(_displayText);
    _window.display();
    return returnEvent;
}

void UI::initTrack(){

    sf::RectangleShape track;
    track.setSize(sf::Vector2f(_window.getSize().x, _trackHeight));
    track.setPosition(sf::Vector2f(0.0, _trackYPos));
    track.setFillColor(sf::Color(0,0,0));
    _track.push_back(track);

    float goalXPos = _window.getSize().x - _goalWidth;

    //Upper edgeLines
    float y = _trackYPos + _edgeLineYOffset;
    addLines(_startWidth + _startLineWidth, goalXPos, _edgeLineXOffset, y, _edgeLineWidth, _edgeLineHeight);

    //Middle Lines
    addLines(_startWidth + _startLineWidth, goalXPos, _middleLineXOffset, _middleLineYPos, _middleLineWidth, _middleLineHeight);

    //Lower edgeLines
    y = _trackYPos+_trackHeight-_edgeLineHeight-_edgeLineYOffset;
    addLines(_startWidth + _startLineWidth, goalXPos, _edgeLineXOffset, y, _edgeLineWidth, _edgeLineHeight);

    //Start Line
    sf::RectangleShape startLine;
    startLine.setSize(sf::Vector2f(_startLineWidth, _trackHeight));
    startLine.setPosition(sf::Vector2f(_startWidth,_trackYPos));
    startLine.setFillColor(sf::Color(190,190,190));
    _track.push_back(startLine);

    //Goal Line
    sf::RectangleShape goalLine;
    goalLine.setSize(sf::Vector2f(_goalLineWidth,_trackHeight));
    goalLine.setPosition(sf::Vector2f(_window.getSize().x - _goalWidth, _trackYPos));
    goalLine.setFillColor(sf::Color(190,190,190));
    _track.push_back(goalLine);

    //Goal Text
    int goalTextXOffset = 5.f;
    _goalText.setFont(_font);
    _goalText.setString("GOAL");
    _goalText.setCharacterSize((int)(_goalWidth-goalTextXOffset));
    _goalText.setFillColor(sf::Color(255,255,255));

    float textXPos = goalXPos+_goalLineWidth + _goalText.getCharacterSize() + goalTextXOffset;
    float textYPos = _trackYPos + (_trackHeight-_goalText.getLocalBounds().width)/2.f;
    _goalText.setPosition(sf::Vector2f(textXPos, textYPos));
    _goalText.rotate(90.f);
    
    
}

void UI::initRacers(){
     //Sort racers by id to give them correct positions
    std::sort(_racers.begin(),_racers.end(),[](Racer &a, Racer &b){return a.id<b.id;});

    std::vector<float> racerBottomOffsetList; //Distance racer is from the bottom

    float extHeight = _racers.front().shape.getSize().y; //extHeight used to extend the trackheight used in the calculation, making racers closer to the edges
    if(_racers.size() == 1){
        racerBottomOffsetList.push_back(_trackHeight/2);
    }
    else{
        for(int i = 0; i < _racers.size();i++){
            float offset = (i+1)*(_trackHeight+extHeight)/float(_racers.size()+1);
            racerBottomOffsetList.push_back(offset);
        }
    }

    for(int i = 0; i < _racers.size(); i++){
        float x = _racers.at(i).shape.getPosition().x;
        float y = _trackYPos + _trackHeight + extHeight/2.f - racerBottomOffsetList.at(i)-_racers.at(i).shape.getSize().y/2.f;
        _racers.at(i).shape.setPosition(sf::Vector2f(x,y));
    }
}

void UI::addLines(float xStartPos, float xEndPos, float xOffset, float y, float lineWidth, float lineHeight){
    float x = xStartPos + xOffset/2.f;
    while(x < xEndPos){
        if(x >= xEndPos -lineWidth-xOffset){
            //Last line
            sf::RectangleShape edgeLine2;
            edgeLine2.setSize(sf::Vector2f(xEndPos - x, lineHeight));
            edgeLine2.setPosition(sf::Vector2f(x, y));
            edgeLine2.setFillColor(sf::Color(190,190,190));
            _track.push_back(edgeLine2);
            break;
        }

        sf::RectangleShape edgeLine;
        edgeLine.setSize(sf::Vector2f(lineWidth, lineHeight));
        edgeLine.setPosition(sf::Vector2f(x, y));
        edgeLine.setFillColor(sf::Color(190,190,190));
        _track.push_back(edgeLine);
        x += lineWidth + xOffset;
    }
}