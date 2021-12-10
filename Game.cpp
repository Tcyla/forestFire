#include "headers/Game.h"


Game::Game(const char* title, int xpos, int ypos, int width, int height, int flags)
{

// SDL Initialisation
// ------------------

// attempt to initialize SDL
	if(SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
	    // init the window
		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);

		if(window != 0) // window init success
		{
			renderer = SDL_CreateRenderer(window, -1, 0);

			if(renderer != 0) // renderer init success
			{
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			}
			else
			{
				throw "renderer init fail";
			}
		}
		else
		{
			throw "window init fail";
		}
	}
	else
	{
		throw "SDL init fail";
	}
	std::cout << "SDL OK" << std::endl; 


    // ForestFire Initialisation
    // -------------------------

    try 
    {

		view = new View(width, height);
		
		forest_size = 0;
		fire_size = 0;
		ash_size = 0;
		forest = new SDL_Rect[10];
		fire = new SDL_Rect[10];
		ash = new SDL_Rect[10];
		
		view -> getView(forest, forest_size, fire, fire_size, ash, ash_size);
		
		// for (int i = 0 ; i < forest_size ; ++i)
		// 	{
		// 		std::cerr << "forest[" << i << "] = {\n"
		// 				  << "                 x: " << forest[i].x << ",\n"
		// 				  << "                 y: " << forest[i].y << ",\n" 
		// 				  << "                 w: " << forest[i].w << ",\n" 
		// 				  << "                 h: " << forest[i].h << "\n"
		// 				  << "                       }\n";
		//  }
		
    }catch(const char* e)
    {
        throw e; 
    }

	std::cout << "Init OK" << std::endl; 

	M_isRunning = true; // everything init'd successfully, start the main loop
	pause = true;
}

void Game::render()
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer); // clear the renderer to the draw color
	// draw forest in green
    SDL_SetRenderDrawColor(renderer, 18, 148, 59, 255);
	SDL_RenderDrawRects(renderer, forest, forest_size);
	SDL_RenderFillRects(renderer, forest, forest_size);
	// draw fire in red
    SDL_SetRenderDrawColor(renderer, 148, 5, 3, 255);
	SDL_RenderDrawRects(renderer, fire, fire_size);
	SDL_RenderFillRects(renderer, fire, fire_size);
	
	// draw ash in grey
    SDL_SetRenderDrawColor(renderer, 148, 148, 148, 255);
	SDL_RenderDrawRects(renderer, ash, ash_size);
	SDL_RenderFillRects(renderer, ash, ash_size);

	// draw Position on side
	// SDL_SetRenderDrawColor(renderer, 0, 0, 0, 148);
	// SDL_RenderDrawRects(renderer, bigPosRect, bigPosRect_size);
	// SDL_SetRenderDrawColor(renderer, 255, 255, 255, 148);
	// SDL_RenderFillRects(renderer, bigPosRect, bigPosRect_size);
	// SDL_SetRenderDrawColor(renderer, 148, 0, 0, 148);
	// SDL_RenderDrawRects(renderer, bigPosRect, bigPosRect_size);
	// SDL_RenderFillRects(renderer, bigPosRect, bigPosRect_size);


	SDL_RenderPresent(renderer); // draw to the screen
}

void Game::update()
{
    if (!pause)
    {
        view -> next();

		try
		{
			view -> getView(forest, forest_size, fire, fire_size, ash, ash_size);
			
		}
		catch(const char* e)
		{
			std::cout << e << std::endl;
			M_isRunning = false;
		}
    }
}

void Game::handleKeyboardEvents(SDL_KeyboardEvent* event)
{
	switch (event->keysym.sym)
	{
		case SDLK_p:
		case SDLK_ESCAPE:
			pause = !pause;
			break;
		case SDLK_UP:
			break;
		case SDLK_DOWN:
			break;
		case SDLK_LEFT:
			break;
		case SDLK_RIGHT:
			break;
		default:
			break;
	}
}

void Game::handleMouseButtonEvent(SDL_MouseButtonEvent* event)
{
	switch (event->button)
	{
		case SDL_BUTTON_LEFT:
			view -> setFire(event -> x, event -> y);
			break;
		default:
			break;
	}
}

void Game::handleMouseWheelEvent(SDL_MouseWheelEvent* event)
{
	if ( (event -> y) > 0)
	{
		view -> setScale(Up, 5);
	}else if ((event -> y) < 0)
	{
		view -> setScale(Down, 5);
	}
}

void Game::handleEvents()
{
	SDL_Event event;
	if(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_QUIT:
				M_isRunning = false;
				break;
			case SDL_KEYUP:
				handleKeyboardEvents(&event.key);
				break;
			case SDL_MOUSEBUTTONUP:
				handleMouseButtonEvent(&event.button);
				break;
			case SDL_MOUSEWHEEL:
				handleMouseButtonEvent(&event.button);
				break;
			default:
			break;
		}
	}
}

Game::~Game()
{
	delete [] forest;
	delete [] fire;
	delete [] ash; 
    delete view;
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	std::cout << "Deletes OK" << std::endl; 
}
