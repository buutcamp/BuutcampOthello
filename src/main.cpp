/*#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_opengl.h>

#include <imgui.h>
#include <examples/imgui_impl_sdl.h>
#include <examples/imgui_impl_opengl2.h> */

#include "main.h"
#include "othello.h"

std::vector<std::string> PrgParams;
int ParseParameter(str param_text);

int main(int argc, char* argv[])
{
    int game_type = LocalGame;
    bool parameter_error = false;

    for (int i = 0; i < argc; ++i)
        PrgParams.push_back(argv[i]);

    for (const auto& entry : PrgParams) {
        if(entry == "-s") {
            std::cout << "Init as Server!" << std::endl;
            game_type = ServerGame;
        } else if(entry == "-c") {
            std::cout << "Init as Client!" << std::endl;
            game_type = ClientGame;
        } else if(entry == "-?") {
            std::cout << PrgParams[0] << "'s help:" << std::endl;
            std::cout << PrgParams[0] << " -?  This menu." << std::endl;
            std::cout << PrgParams[0] << " -c  Start game as client." << std::endl;
            std::cout << PrgParams[0] << " -s  Start game as server and will be game master for client's settings." << std::endl;
            parameter_error = true;     //Handle help as invalid parameter
        } else {
            if(ParseParameter(entry) != 0) {
                std::cout << "Unknown parameter [" << entry << "]" << std::endl;
                parameter_error = true;
            }
        }
    }
    if(parameter_error == true) {
        return 0;
    }

    Game game(Black, game_type);

    game.InitSdl();
    game.InitImgui();
    game.OthelloInit();

    while (game.gameRunning())
    {
        if((game.resetGame() || game.changeBoardsize()))
            game.OthelloInit();

        game.handleEvents();
        game.update();
        game.gameOver();

    }
    //clean up everything
    game.clean();
	return 0;

}

int ParseParameter(str param_text)
{
    if(param_text == PrgParams[0])
        return 0;
    return -1;
}
