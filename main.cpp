#include <SDL.h>
#include <vector>
#include <memory>

#include "Player.h"

int main(int argc, char* args[])
{
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_Window* window = SDL_CreateWindow("openGlNowe", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	SDL_GLContext context = SDL_GL_CreateContext(window);

	SDL_SetRelativeMouseMode(SDL_TRUE);

	glewExperimental = true;
	glewInit();
	
	glViewport(0, 0, 1280, 720);
	bool quit = false;

	SDL_Event event;
	
	Player player(glm::vec3(0.0f, 0.0f, 0.0f));

	glEnable(GL_DEPTH_TEST);
	
	while (!quit) {
		float deltaTime = 1 / 60.0f;

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				exit(0);
			} else if (event.type == SDL_MOUSEMOTION) {
				//player.camera->ProcessMouseMovement((float)event.motion.xrel, (float)event.motion.yrel);
				player.MouseMotion((float)event.motion.xrel, (float)event.motion.yrel);
			} else if (event.type == SDL_MOUSEWHEEL) {
				if (event.wheel.y > 0) {
					//player.getCamera()->ProcessMouseScroll((float)event.wheel.y);
				} else if (event.wheel.y < 0) {
					//player.getCamera()->ProcessMouseScroll((float)event.wheel.y);
				}
			} else if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
					case SDLK_ESCAPE: exit(0); break;
					case SDLK_UP:   SDL_SetWindowSize(window, 1920, 1080); 	glViewport(0, 0, 1920, 1080); break;
					case SDLK_DOWN: SDL_SetWindowSize(window, 1280, 720); 	glViewport(0, 0, 1280, 720); break;
				}
			}
		}

		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		player.Update(deltaTime);

		player.Draw();

		SDL_GL_SwapWindow(window);
	}
	return 0;
}

