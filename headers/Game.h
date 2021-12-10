#ifndef __GAME__
#define __GAME__

#include <iostream>
#include <SDL2/SDL.h>

#include "View.h"
#include "ForestFire.h"


class Game
{
	private:

		SDL_Window* window;
		SDL_Renderer* renderer; 
		View* view;

		//
		SDL_Rect* forest; 
		int forest_size;
		//
		SDL_Rect* fire; 
		int fire_size;
		//
		SDL_Rect* ash; 
		int ash_size;

		bool M_isRunning;
		bool pause;

		void handleKeyboardEvents(SDL_KeyboardEvent*);
		void handleMouseButtonEvent(SDL_MouseButtonEvent*);
		void handleMouseWheelEvent(SDL_MouseWheelEvent*);
		

	public:
		
		Game( const char*, int, int, int, int, int );
		~Game();

		bool isRunning(){ return M_isRunning; }

		void render();
		void update();
		void handleEvents();
	
};

#endif