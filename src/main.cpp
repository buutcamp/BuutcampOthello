/*#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_opengl.h>

#include <imgui.h>
#include <examples/imgui_impl_sdl.h>
#include <examples/imgui_impl_opengl2.h> */

#include "main.h"
#include "othello.h"

int main(void)
{
    Game game(White);
    
    game.InitSdl();
    game.InitImgui();
    game.OthelloInit();

    while (game.gameRunning())
    {
        if(game.resetGame() || game.changeBoardsize())
            game.OthelloInit();

        game.handleEvents();
        game.update();   
        
    }
    //clean up everything
    game.clean();
    return 0;
}

