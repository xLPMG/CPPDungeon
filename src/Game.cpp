#include "Game.hpp"
#include "constants.hpp"
#include <iostream>

#include <SDL.h>
#include <chrono>

void cppdungeon::Game::handleEvents()
{
  SDL_Event ev;
  while (SDL_PollEvent(&ev))
  {
    switch (ev.type)
    {
    case SDL_QUIT:
      running = false;
      break;
    default:
      break;
    }
  }

  const u8 *keystate = SDL_GetKeyboardState(NULL);

  bool moveRight = keystate[SDL_SCANCODE_RIGHT] || keystate[SDL_SCANCODE_D];
  bool moveLeft = keystate[SDL_SCANCODE_LEFT] || keystate[SDL_SCANCODE_A];
  bool moveUp = keystate[SDL_SCANCODE_UP] || keystate[SDL_SCANCODE_W];
  bool moveDown = keystate[SDL_SCANCODE_DOWN] || keystate[SDL_SCANCODE_S];
  bool sprinting = keystate[SDL_SCANCODE_LSHIFT];
  i8 x = moveRight - moveLeft;
  i8 y = moveDown - moveUp;
  m_player->move(&x, &y, sprinting, &m_dt);
}

void cppdungeon::Game::update()
{
  m_player->update(&m_dt);
}

void cppdungeon::Game::render()
{
  SDL_RenderClear(renderer);
  m_player->render(renderer);
  SDL_RenderPresent(renderer);
}

void cppdungeon::Game::cleanup()
{
  SDL_DestroyTexture(debug);
  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
}

int cppdungeon::Game::start(int width, int height)
{
  ASSERT(
      !SDL_Init(SDL_INIT_VIDEO),
      "SDL failed to initialize: %s",
      SDL_GetError());

  window =
      SDL_CreateWindow(
          "dungeon",
          SDL_WINDOWPOS_CENTERED_DISPLAY(0),
          SDL_WINDOWPOS_CENTERED_DISPLAY(0),
          width,
          height,
          0);

  ASSERT(window, "failed to create SDL window: %s\n", SDL_GetError());

  renderer =
      SDL_CreateRenderer(
          window,
          -1,
          SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  texture =
      SDL_CreateTexture(
          renderer,
          SDL_PIXELFORMAT_ABGR8888,
          SDL_TEXTUREACCESS_STREAMING,
          SCREEN_WIDTH,
          SCREEN_HEIGHT);
  debug =
      SDL_CreateTexture(
          renderer,
          SDL_PIXELFORMAT_ABGR8888,
          SDL_TEXTUREACCESS_TARGET,
          128,
          128);

  m_player = new cppdungeon::entities::Player(renderer);
  
  return 0;
}