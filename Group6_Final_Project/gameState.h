#pragma once
#include <memory>

class Game;

class GameState {
public:
    virtual ~GameState() = default;

    virtual void handleInput() = 0;
    virtual void update() = 0;
    virtual void draw() = 0;

    virtual bool isRunning() const = 0;
    virtual std::unique_ptr<GameState> getNextState() = 0;
};