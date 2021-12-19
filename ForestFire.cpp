#include "headers/ForestFire.h"

// Private
// -------

char ForestFire::get_torified_coeffs( int i ,  int j)
{
	// The grid is a torus. This function returns 
	//
	int x,y;

	if ( i == -1 )
		x = M_row - 1;
	else if ( i == M_row )
		x = 0;
	else
		x = i;

	if ( j == -1 )
		y = M_col - 1;
	else if ( j == M_col )
		y = 0;
	else
		y = j;

	return (*M_matrix)(x,y);
}

char ForestFire::rule(int i, int j)
{
	/*
	 * rules are:
	 * 1) if a fire tile is in the neighbourhood of a forest tile then
	 *    the forest tile is set on fire.
	 * 2) if fire then ash.
	 */

	char res;
	try
	{
		res = (*M_matrix)(i,j);
	}
	catch (const char* e)
	{
		throw e;
	}
	// table of the surrounding cells
	char tab[8] = {
		get_torified_coeffs(i-1, j-1), get_torified_coeffs(i-1, j),
		get_torified_coeffs(i-1, j+1), get_torified_coeffs(i, j-1),
		get_torified_coeffs(i, j+1), get_torified_coeffs(i+1, j-1),
		get_torified_coeffs(i+1, j), get_torified_coeffs(i+1, j+1)
	};

	// counting the living cells
	bool fire_around = false;
	for (int k = 0; k < 8; ++k)
	{
		if ( tab[k] == 'f' )
		{
			fire_around = true;
			break;
		}
	}

	// checking if the cell lives
	if ( (*M_matrix)(i,j) == 'w' && fire_around )
	{
		res = 'f';
	}else if ( (*M_matrix)(i,j) == 'f' )
	{
		res = 'a';
	}

	return res;
}

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



void setRectAttr(int pixSize, std::unique_ptr<SDL_Rect[]>& state, int& state_size, int i, int j, SDL_Rect view) noexcept
{
	(state.get())[state_size].h = pixSize;
	(state.get())[state_size].w = pixSize;
	(state.get())[state_size].x = (j - view.x) * pixSize;
	(state.get())[state_size].y = (i - view.y) * pixSize ;
	++state_size;

}


void ForestFire::getState(SDL_Rect view, int pixSize,
						  std::unique_ptr<SDL_Rect[]>& forest, int& forest_size,
						  std::unique_ptr<SDL_Rect[]>& fire, int& fire_size,
						  std::unique_ptr<SDL_Rect[]>& ash, int& ash_size)
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

void ForestFire::step_thread_handler(int current_thread, int max_threads)
{
	char state;
	for (int i = 0; i < M_row; ++i)
	{
		for (int j = 0 ; j < M_col ; ++j)
		{
			if ( j % max_threads == current_thread)
			{
				state = rule(i,j);
				M_matrixBuffer -> setCoef(state, i, j);
			}
		}
	}

}

void ForestFire::step()
{
	int max_threads = 8;
	std::vector<std::thread> thread_list; // empty vector of thread

	for (int i = 0 ; i < max_threads ; ++i)
	{
		// execute ForestFire::rule() on max_threads different threads  
		thread_list.push_back(std::thread(
			[this](int i, int max_threads)
			{
				ForestFire::step_thread_handler(i, max_threads);
			}, i, max_threads
		));
	}

	for (auto& th : thread_list) th.join(); // waiting for all the threads to join
	
	swap_buffer();
	++M_curentTime;
}


[[deprecated]]
void ForestFire::run(int end)
{
	while (M_curentTime <= end)
	{
		std::cout << "\033c" << *this << std::endl;
		this -> step();
		SDL_Delay(1000);
	}
}

void ForestFire::reset()
{
	M_matrix -> randomize();
	M_curentTime = 0;
}

[[deprecated]]
std::ostream& operator<<(std::ostream& o, ForestFire& g)
{
	return o << std::endl << *g.M_matrix << std::endl
			 << "t = " << g.M_curentTime << std::endl;
}

