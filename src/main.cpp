#include <iostream>

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_opengl.h>

#include <imgui.h>
#include <examples/imgui_impl_sdl.h>
#include <examples/imgui_impl_opengl2.h>

#include "main.h"
#include "othello.h"

SDL_Window* window;
SDL_GLContext gl_context;

void InitSdl()
{
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Othello", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL);
    if (!window)
        throw std::runtime_error("Failed to create SDL window");

    gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);

    SDL_GL_SetSwapInterval(1);
}

void InitImgui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(window, &gl_context);
    ImGui_ImplOpenGL2_Init();
}

int main(void)
{
	InitSdl();
    InitImgui();

    OthelloInit();

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

        // prepare new frame
        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();

        // game specific logic and rendering goes here
        int width, height;
        SDL_GetWindowSize(window, &width, &height);

        uint64_t ticksNow = SDL_GetPerformanceCounter();
        float deltaTime = (ticksNow - ticksLast)*1000 / (float)SDL_GetPerformanceFrequency();
        ticksLast = ticksNow;

        OthelloFrame(deltaTime);
        OthelloRender(width, height);
        
        // let imgui handle rest of the rendering process
        ImGui::Render();

        glViewport(0, 0, width, height);
        //glClearColor(1, 0, 0, 1);
        //glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

        SDL_GL_SwapWindow(window);
    }
	return 0;
}