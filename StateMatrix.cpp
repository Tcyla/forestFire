#include "headers/StateMatrix.h"

StateMatrix::StateMatrix(): M_col(0), M_row(0){}

StateMatrix::StateMatrix(int row, int col)
{
    
    M_col = col;
    M_row = row;

    forest_count = 0;
	fire_count = 0;
	ash_count = 0;
    
    M_coeffs = new char*[M_row];
    for (int i = 0; i < M_row; ++i)
    {
        M_coeffs[i] = new char[M_col];
    }
     
}

StateMatrix::~StateMatrix()
{
    for(int i = 0 ; i < M_row ; ++i)
    {
        delete [] M_coeffs[i];
    } 
    delete [] M_coeffs; 
}

StateMatrix::StateMatrix(const StateMatrix& M): M_col(M.M_col), M_row(M.M_row)
{
    M_coeffs = new char*[M_row];
    for (int i = 0; i < M_row; ++i)
    {
        M_coeffs[i] = new char[M_col];
        for (int j = 0; j < M_col; ++j)
        {
            M_coeffs[i][j] = M.M_coeffs[i][j];
        }
    }
}

void StateMatrix::empty()
{
    forest_count = 0;
	fire_count = 0;
	ash_count = 0;

    for (int i = 0; i < M_row; ++i)
    {
        for(int j = 0; j < M_col; ++j)
        {
            M_coeffs[i][j] = 'e';
        }
    }

}

void StateMatrix::randomize()
{
    forest_count = 0;
	fire_count = 0;
	ash_count = 0;

    srand(time(0));
    for (int i = 0; i < M_row; ++i)
    {
        for(int j = 0; j < M_col; ++j)
        {
            if ( rand() % 100 < 50 )
            {
                M_coeffs[i][j] = 'w'; // square (i,j) is a forest 
                                      // with probability p = 0.7
                ++forest_count;
            }
            else
                M_coeffs[i][j] = 'e'; // square (i,j) is empty 
                                      // with probability (1-p) = 0.3
        }
    }
}

char StateMatrix::operator()(int i, int j)
{
		if ( i < 0 || j < 0 || i >= M_row || j >= M_col )
			throw "operator(): Out of range"; 
			//throw std::out_of_range();
		return M_coeffs[i][j];
}

void StateMatrix::inc_dec(bool inc, int& count)
{
    if (inc) // if inc true add 1
        ++count;
    else // else remove one
        --count;
}

void StateMatrix::modStateCount(bool inc, int i, int j)
{
/*****************************************************************************
 **
 *****************************************************************************/
    if ( (*this)(i,j) == 'w' )
    {
        inc_dec(inc, forest_count);
    }else if ( (*this)(i,j) == 'f' )
    {
        inc_dec(inc, fire_count);
    }else if ( (*this)(i,j) == 'a' )
    {
        inc_dec(inc, ash_count);
    }
}

void StateMatrix::setCoef(char c, int i, int j)
{
    if ( i < 0 || j < 0 || i >= M_row || j >= M_col )
			throw "setCoef: (i,j) is out of range"; 
    else
    {
        if ( (c == 'e') |  (c == 'w') |  (c == 'f') |  (c == 'a') )
        {
            modStateCount(false, i, j);
            M_coeffs[i][j] = c;
            modStateCount(true, i, j);
        }
        else
        {
            throw "setCoef: char argument should be 'e', 'w', 'f' or 'c'";
        }
    }
}    

std::ostream& operator<<(std::ostream& o, StateMatrix& M)
{
	for( int i = 0; i < M.row() ; ++i)
	{
		for ( int j = 0 ; j < M.col() ; ++j)
		{
			if ( M(i,j) != 'e')
				o << M(i,j);
			else
				o << " ";
		}
		o << std::endl;
	}
	return o; 
}
