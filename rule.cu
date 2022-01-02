char get_torified_coeffs( int i ,  int j, char** matrix, int row, int col )
{
	// The grid is a torus. This function returns 
	//
	int x,y;

	if ( i == -1 )
		x = row - 1;
	else if ( i == row )
		x = 0;
	else
		x = i;

	if ( j == -1 )
		y = col - 1;
	else if ( j == col )
		y = 0;
	else
		y = j;

	return (matrix)[x][y];
}

char rule(int i, int j, char** matrix, int row, int col )
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
		res = (matrix)[i][j];
	}
	catch (const char* e)
	{
		throw e;
	}
	// table of the surrounding cells
	char tab[8] = {
		get_torified_coeffs(i-1, j-1, matrix, row, col), get_torified_coeffs(i-1, j, matrix, row, col),
		get_torified_coeffs(i-1, j+1, matrix, row, col), get_torified_coeffs(i, j-1, matrix, row, col),
		get_torified_coeffs(i, j+1, matrix, row, col), get_torified_coeffs(i+1, j-1, matrix, row, col),
		get_torified_coeffs(i+1, j, matrix, row, col), get_torified_coeffs(i+1, j+1, matrix, row, col)
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
	if ( (matrix)[i][j] == 'w' && fire_around )
	{
		res = 'f';
	}else if ( (matrix)[i][j] == 'f' )
	{
		res = 'a';
	}

	return res;
}

__global__
void rule_application(char** matrix[], int row, int col )
{
    int nThreadRef;
    // int nThreadRef = cudathreadnumber
    int i = nThreadRef / col;
    int j = nThreadRef % col;
    (*matrix)[i][j] = rule( i, j, *matrix, row, col );
}

void rule_applied( char** matrix[], int row, int col )
{
    int nbThread = row * col;
    rule_application<<<nbThread>>>(char** matrix[], int row, int col); 
}