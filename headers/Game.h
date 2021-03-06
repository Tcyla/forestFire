#ifndef __GAME__
#define __GAME__

#include <iostream>
#include <memory>
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
		std::unique_ptr<SDL_Rect[]> forest; 
		long forest_size;
		//
		std::unique_ptr<SDL_Rect[]> fire; 
		long fire_size;
		//
		std::unique_ptr<SDL_Rect[]> ash; 
		long ash_size;

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