#include <stdio.h>
#include <string>

#include <SDL.h>
#include <glad/glad.h>
#include <fmt/core.h>
#ifdef __EMSCRIPTEN__
	#include <emscripten.h>
#endif

const int SCREEN_WIDTH = 512;
const int SCREEN_HEIGHT = 448;

SDL_Window* window = NULL;
SDL_GLContext context;
bool quit = false;

bool init() {
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fmt::print("SDL could not initialize! SDL Error: {}\n", SDL_GetError());
		success = false;
	} else {
    #ifdef __EMSCRIPTEN__
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    #else
    	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    #endif

		window = SDL_CreateWindow(
            "C++ And WebAssembly",
            SDL_WINDOWPOS_UNDEFINED, 
            SDL_WINDOWPOS_UNDEFINED, 
            SCREEN_WIDTH, SCREEN_HEIGHT, 
            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
        );
		if (window == nullptr) {
			fmt::print("Window could not be created! SDL Error: {}\n", SDL_GetError());
			success = false;
		} else {
            context = SDL_GL_CreateContext(window);
			if (context == NULL) {
				fmt::print("OpenGL context could not be created! SDL Error: {}\n", SDL_GetError());
				success = false;
			} else {
                if (gladLoadGLLoader(SDL_GL_GetProcAddress) > 0) {
                    fmt::print("GL version: {}\n", (const char *)glGetString(GL_VERSION));
                }
				if (SDL_GL_SetSwapInterval(1) < 0) {
					fmt::print("Warning: Unable to set VSync! SDL Error: {}\n", SDL_GetError());
				}
			}
		}
	}

	return success;
};

void initGL() {
	glClearColor(0.0, 1.0, 1.0, 1.0);
};

void update() {
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT);
}

void mainloop() {
	            SDL_Event event;
			while (SDL_PollEvent(&event) != 0) {
                switch (event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYUP:
                    switch (event.key.keysym.sym) {
                    case 'w':
                        fmt::print("You pressed W key\n");
                        break;
                    case 'a':
                        fmt::print("You pressed A key\n");
                        break;
                    case 's':
                        fmt::print("You pressed S key\n");
                        break;
                    case 'd':
                        fmt::print("You pressed D key\n");
                        break;
                    default:
                        break;
                    }
                default:
                    break;
                }
			}

            update();

			render();

			SDL_GL_SwapWindow(window);
}

void close() {
    SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

extern "C" {
#ifdef __EMSCRIPTEN__
	EMSCRIPTEN_KEEPALIVE
#endif
	void sayHello() {
		fmt::print("Hello!\n");
	}
}

extern "C" {
	int main (int argc, char* args[]) {
		if (!init()) {
			fmt::print("Failed to initialize!\n");
		} else {
			sayHello();

			initGL();

		#ifdef __EMSCRIPTEN__
			emscripten_set_main_loop(mainloop, 0, true);
		#else
			while (!quit) {
				mainloop();
			}
		#endif
		}
		close();

		return EXIT_SUCCESS;
	}
}