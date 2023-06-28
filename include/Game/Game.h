#pragma once

class Game
{
private:

    enum class EnState
    {
        eInMenu,
        eInNameEntering,
        eInGame,
        eInManual,
        eInGameOver,
    };

    EnState eState;
public:

    Game() : eState(EnState::eInMenu) { }

};
