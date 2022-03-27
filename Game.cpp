#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Game.h"

Game::Game(size_t width, size_t height)
	: state_(GAME_ACTIVE)
	, keys_()
	, width_(width)
	, height_(height) {

}

Game::~Game() {

}

void Game::Init() {

}

void Game::ProcessInput(float dt) {

}

void Game::Update(float dt) {

}

void Game::Render() {

}
