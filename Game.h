#pragma once

enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

class Game {
public:
    Game(size_t width, size_t height);
    ~Game();
    // initialize game state (load all shaders/textures/levels)
    void Init();
    // game loop
    void ProcessInput(float delta_time);
    void Update(float delta_time);
    void Render();

    void SetKey(size_t key, bool state);
private:
    // game state
    GameState    state_;
    bool         keys_[1024];
    size_t width_, height_;
};