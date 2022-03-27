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

void Game::ProcessInput(float delta_time) {

}

void Game::Update(float delta_time) {

}

void Game::Render() {

}

void Game::SetKey(size_t key, bool state) {
	keys_[key] = state;
}
