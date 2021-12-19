#ifndef __FORESTFIRE__
#define __FORESTFIRE__

#include <iostream>
#include <memory>
#include <thread>
#include <vector>
#include <SDL2/SDL.h>

#include "StateMatrix.h"


class ForestFire
{
    private:
        StateMatrix* M_matrix;
		StateMatrix* M_matrixBuffer;
        int M_curentTime = 0;
        int M_row;
        int M_col;
	
		char get_torified_coeffs( int , int );
		
		char rule( int,  int );

		void swap_buffer();

		void step_thread_handler(int, int);
	
    public:

		ForestFire( int, int );
		~ForestFire()
		{
			delete M_matrix; 
			delete M_matrixBuffer;
		}

		friend std::ostream& operator<<(std::ostream&, ForestFire&);

		// Getters
		int row(){ return M_row; }
		int col(){ return M_col; }
		int currentTime() const { return M_curentTime; }
		void getState(SDL_Rect, int,
					  std::unique_ptr<SDL_Rect[]>&, int&,
					  std::unique_ptr<SDL_Rect[]>&, int&,
					  std::unique_ptr<SDL_Rect[]>&, int&); 
		
		// Setters
		void setFire(int, int);
		void step();
		
		void run(int);
		void reset();

		StateMatrix matrix() {return *M_matrix;}

};

std::ostream& operator<<( std::ostream&, ForestFire& );

#endif
