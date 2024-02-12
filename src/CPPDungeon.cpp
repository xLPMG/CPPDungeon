#include "constants.hpp"
#include "entities/Player.hpp"

#include <SDL.h>
#include <chrono>

using namespace cppdungeon;

static struct
{
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *texture, *debug;
  u32 *pixels;
  bool quit;
} state;

entities::Player player;

void update()
{
  player.update();
}

void render(u32 *pixels)
{
  player.render(pixels);
}

static void present() {
    void *px;
    int pitch;
    SDL_LockTexture(state.texture, NULL, &px, &pitch);
    {
        for (usize y = 0; y < SCREEN_HEIGHT; y++) {
            memcpy(
                &((u8*) px)[y * pitch],
                &state.pixels[y * SCREEN_WIDTH],
                SCREEN_WIDTH * 4);
        }
    }
    SDL_UnlockTexture(state.texture);

    SDL_SetRenderTarget(state.renderer, NULL);
    SDL_SetRenderDrawColor(state.renderer, 0, 0, 0, 0xFF);
    SDL_SetRenderDrawBlendMode(state.renderer, SDL_BLENDMODE_NONE);

    SDL_RenderClear(state.renderer);
    SDL_RenderCopyEx(
        state.renderer,
        state.texture,
        NULL,
        NULL,
        0.0,
        NULL,
        SDL_FLIP_VERTICAL);

    SDL_SetTextureBlendMode(state.debug, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(state.renderer, state.debug, NULL, NULL);
    SDL_RenderPresent(state.renderer);
}

int main(int argc, char *argv[])
{
  ASSERT(
      !SDL_Init(SDL_INIT_VIDEO),
      "SDL failed to initialize: %s",
      SDL_GetError());

  state.window =
      SDL_CreateWindow(
          "raycast",
          SDL_WINDOWPOS_CENTERED_DISPLAY(0),
          SDL_WINDOWPOS_CENTERED_DISPLAY(0),
          1280,
          720,
          0);

  ASSERT(state.window, "failed to create SDL window: %s\n", SDL_GetError());

  state.renderer =
      SDL_CreateRenderer(
          state.window,
          -1,
          SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  state.texture =
      SDL_CreateTexture(
          state.renderer,
          SDL_PIXELFORMAT_ABGR8888,
          SDL_TEXTUREACCESS_STREAMING,
          SCREEN_WIDTH,
          SCREEN_HEIGHT);
  state.debug =
      SDL_CreateTexture(
          state.renderer,
          SDL_PIXELFORMAT_ABGR8888,
          SDL_TEXTUREACCESS_TARGET,
          128,
          128);

  state.pixels = (u32*)malloc(SCREEN_WIDTH * SCREEN_HEIGHT * 4);

  //auto lastTick = std::chrono::high_resolution_clock::now();
  f32 dt = 0;
  /////////////////
  //  MAIN LOOP  //
  /////////////////
  while (!state.quit)
  {
    SDL_Event ev;
    while (SDL_PollEvent(&ev))
    {
      switch (ev.type)
      {
      case SDL_QUIT:
        state.quit = true;
        break;
      default:
        break;
      }
    }

    if (state.quit)
    {
      break;
    }

    const u8 *keystate = SDL_GetKeyboardState(NULL);

    if (keystate[SDL_SCANCODE_RIGHT] || keystate[SDL_SCANCODE_D])
    {
      player.move(Direction::RIGHT);
    }

    if (keystate[SDL_SCANCODE_LEFT] || keystate[SDL_SCANCODE_A])
    {
      player.move(Direction::LEFT);
    }

    if (keystate[SDL_SCANCODE_UP] || keystate[SDL_SCANCODE_W])
    {
      player.move(Direction::UP);
    }

    if (keystate[SDL_SCANCODE_DOWN] || keystate[SDL_SCANCODE_S])
    {
      player.move(Direction::DOWN);
    }
    update();
    memset(state.pixels, 0, SCREEN_WIDTH * SCREEN_HEIGHT * 4);
    render(state.pixels);
    present();
  }
  SDL_DestroyTexture(state.debug);
  SDL_DestroyTexture(state.texture);
  SDL_DestroyRenderer(state.renderer);
  SDL_DestroyWindow(state.window);
  return 0;
}