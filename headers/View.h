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

        void getView(std::unique_ptr<SDL_Rect[]> &forest, long& forest_size,
					 std::unique_ptr<SDL_Rect[]> &fire, long& fire_size,
					 std::unique_ptr<SDL_Rect[]> &ash, long& ash_size)
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
