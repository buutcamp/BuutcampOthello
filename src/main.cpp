
/*#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_opengl.h>

#include <imgui.h>
#include <examples/imgui_impl_sdl.h>
#include <examples/imgui_impl_opengl2.h> */

#include "main.h"
#include "othello.h"

//SDL_Window* window;
//SDL_GLContext gl_context;

int main(void)
{
    Game game(White);
    game.InitSdl();
    game.InitImgui();

    game.OthelloInit();

    uint64_t ticksLast = SDL_GetPerformanceCounter();

    SDL_Event event;
    while (true)
    {
        while (SDL_PollEvent(&event))
        {
            // SDL events are passed to imgui
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                return 0;
        }
        game.update();
    }
	return 0;
}