#ifndef __VIEW__
#define __VIEW__

#include <iostream>
#include <memory>
#include <SDL2/SDL.h>

#include "ForestFire.h"


/*
 * To do:
 * - setFire()
 * - Loc indic°
 *
 */

enum Direction { Up, Down, Left, Right};

class View
{
	private:
		int M_pixSize;
		SDL_Rect M_view;

        ForestFire* forestfire;

	public:

		View(int, int);
		~View();

		void setScale( int );
		void setScale( Direction, int );
		void moveView( Direction );

		void setFire(int x, int y)
		{
			forestfire -> setFire(y/M_pixSize,x/M_pixSize);
		}

        void getView(std::shared_ptr<SDL_Rect[]> forest, int& forest_size,
					 std::shared_ptr<SDL_Rect[]> fire, int& fire_size,
					 std::shared_ptr<SDL_Rect[]> ash, int& ash_size)
		{
			forestfire -> getState(M_view, M_pixSize,
						  		   forest, forest_size,
								   fire, fire_size,
						  		   ash, ash_size);
		}

		void getLoc();

		void next()
		{
			forestfire -> step();
		}

};


#endif
