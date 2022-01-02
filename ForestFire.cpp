#include "headers/ForestFire.h"
#include "rule.cu"

// Private
// -------

void ForestFire::swap_buffer()
{
	StateMatrix* tmp = M_matrix;
	M_matrix = M_matrixBuffer;
	M_matrixBuffer = tmp;
}

// Public
// ------

// Init

ForestFire::ForestFire(int row, int col)
{
	M_row = row;
	M_col = col;

	try
	{
		M_matrix = new StateMatrix(M_row, M_col);
		M_matrix -> randomize();
		M_matrixBuffer = new StateMatrix(M_row, M_col);
	}
	catch ( std::bad_alloc& e )
	{
		throw "bad_alloc";
	}

}

/*********************************************************************
 *                                                                   *
 *                              getState()                           *
 *                                                                   *
 *********************************************************************/



void setRectAttr(int pixSize, std::unique_ptr<SDL_Rect[]>& state, long& state_size, int i, int j, SDL_Rect view) noexcept
{
	(state.get())[state_size].h = pixSize;
	(state.get())[state_size].w = pixSize;
	(state.get())[state_size].x = (j - view.x) * pixSize;
	(state.get())[state_size].y = (i - view.y) * pixSize ;
	++state_size;

}


void ForestFire::getState(SDL_Rect view, int pixSize,
						  std::unique_ptr<SDL_Rect[]>& forest, long& forest_size,
						  std::unique_ptr<SDL_Rect[]>& fire, long& fire_size,
						  std::unique_ptr<SDL_Rect[]>& ash, long& ash_size)
{

	forest_size = 0;
	std::unique_ptr<SDL_Rect[]> forest_tmp(new SDL_Rect[M_matrix -> forest()]);
	fire_size = 0;
	std::unique_ptr<SDL_Rect[]> fire_tmp(new SDL_Rect[M_matrix -> fire()]);
	ash_size = 0;
	std::unique_ptr<SDL_Rect[]> ash_tmp(new SDL_Rect[M_matrix -> ash()]);
	
	char state;
	for ( int i = view.y ; i < (view.y + view.h) ; ++i )
	{
		for( int j = view.x ; j < (view.x + view.w) ; ++j )
		{

			try
			{
				state = (*M_matrix)(i,j);
			}
			catch(const char* e)
			{
				std::cerr << e << '\n';
			}

			if ( state == 'w' )
			{

				setRectAttr(pixSize, forest_tmp, forest_size, i, j, view);
				
			}else if ( state == 'f' )
			{

				setRectAttr(pixSize, fire_tmp, fire_size, i, j, view);

			}else if ( state == 'a' )
			{

				setRectAttr(pixSize, ash_tmp, ash_size, i, j, view);

			}
		}

	}

	forest = std::move(forest_tmp);
	fire = std::move(fire_tmp);
	ash = std::move(ash_tmp);

}

void ForestFire::setFire(int i, int j)
{
    M_matrix -> setCoef('f', i, j);
}


void ForestFire::step()
{
	rule_applied( M_matrixBuffer -> c_matrixptr(), M_matrixBuffer -> row(), M_matrixBuffer -> col() );
	swap_buffer();
	++M_curentTime;
}


void ForestFire::reset()
{
	M_matrix -> randomize();
	M_curentTime = 0;
}

