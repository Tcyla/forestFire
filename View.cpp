#include "headers/View.h"


View::View(int i, int j)
{
    // bug when pixSize <= 3
	M_pixSize = 5;
	M_view.h = j/M_pixSize;
	M_view.w = i/M_pixSize;

	// Center
	M_view.x = 0;
	M_view.y = 0;

	forestfire = new ForestFire(i, j);
}

View::~View()
{
	delete forestfire;
}

void View::setScale( int s )
{

	if ( s < 20 )
	{
		M_pixSize = s;
	}

}

void View::setScale( Direction direction, int s )
{
	switch ( direction )
	{
		case Up:
			setScale(M_pixSize - s);
            std::cout << "Up\n";
			break;
		case Down:
			setScale(M_pixSize + s);
			std::cout << "Down\n";
			break;
		default:
			break;
	}

}

void View::moveView( Direction direction)
{
    int px = M_pixSize;
	switch ( direction )
	{
		case Up:
			if ( (M_view.y - px) > 0 )
			{
				M_view.y -= px ;
			}
			break;
		case Down:
			if ( (M_view.y + px) < (forestfire -> row()) )
			{
				M_view.y += px ;
			}
			break;
		case Left:
			if ( (M_view.x - px) > 0 )
			{
				M_view.x -= px ;
			}
			break;
		case Right:
			if ( (M_view.x + px) < (forestfire -> col()) )
			{
				M_view.x += px ;
			}
			break;
	}
}
