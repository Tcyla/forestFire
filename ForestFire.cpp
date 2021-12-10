#include "headers/ForestFire.h"

// Private
// -------

char ForestFire::get_torified_coeffs( int i ,  int j)
{
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
void set_null(SDL_Rect* rect, int& count)
{
	SDL_Rect* tmp = rect;
	rect = nullptr;
	count = 0;
	//delete [] tmp;
}

void setRectSize(int pixSize, SDL_Rect* state, int state_size)
{
	state[state_size].h = pixSize;
	state[state_size].w = pixSize;
}

void ForestFire::getState(SDL_Rect view, int pixSize,
						  SDL_Rect* forest, int& forest_size,
						  SDL_Rect* fire, int& fire_size,
						  SDL_Rect* ash, int& ash_size)
{

	forest_size = 0;

	/*set_null(forest, forest_size);
	set_null(fire, fire_size);
	set_null(ash, ash_size);*/


	SDL_Rect* forest_tmp = new SDL_Rect[M_matrix -> forest()];
	fire = new SDL_Rect[M_matrix -> fire()];
	ash = new SDL_Rect[M_matrix -> ash()];


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
				forest_tmp[forest_size].h = pixSize;
				forest_tmp[forest_size].w = pixSize;
				forest_tmp[forest_size].x = j * pixSize;
				forest_tmp[forest_size].y = i * pixSize;
				++forest_size;
							
				// std::cout << "forest_tmp[" << forest_size - 1 <<"] = {\n"
				// 		  << "                 x: " << forest_tmp[forest_size - 1].x << ",\n"
				// 		  << "                 y: " << forest_tmp[forest_size - 1].y << ",\n" 
				// 		  << "                 w: " << forest_tmp[forest_size - 1].w << ",\n" 
				// 		  << "                 h: " << forest_tmp[forest_size - 1].h << "\n"
				// 		  << "                          }\n";
			
			}else if ( state == 'f' )
			{
				
				setRectSize(pixSize, fire, fire_size);
				fire[fire_size].x = j * (pixSize + 1);
				fire[fire_size].y = i * (pixSize + 1);
				++fire_size;

			}else if ( state == 'a' )
			{
				
				setRectSize(pixSize, ash, ash_size);
				ash[ash_size].x = j * (pixSize + 1);
				ash[ash_size].y = i * (pixSize + 1);
				++ash_size;
			
			}

			
		}
	}

	SDL_Rect* tmp = forest;
	forest = nullptr;
	delete [] tmp;

	forest = forest_tmp;
}

// step()
// ------

void ForestFire::step()
{
	char state;
	for (int i = 0; i < M_row; ++i) 
	{
		for (int j = 0 ; j < M_col ; ++j) 
		{
			state = rule(i,j);
			M_matrixBuffer -> setCoef(state, i, j);
		}
	}

	swap_buffer();
	++M_curentTime;
}


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


std::ostream& operator<<(std::ostream& o, ForestFire& g)
{ 
	return o << std::endl << *g.M_matrix << std::endl 
			 << "t = " << g.M_curentTime << std::endl;
}

