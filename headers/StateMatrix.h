#ifndef __STATEMATRIX__
#define __STATEMATRIX__

#include <iostream>
#include <exception>

class StateMatrix
{
	char** M_coeffs; // possible values 'e': empty
					 //                 'w': wood (forest in the original model)
					 //					'f': fire
					 //                 'a': ash

	int M_col;
	int M_row;

	long forest_count;
	long fire_count;
	long ash_count;

	void inc_dec(bool, long&);
	void modStateCount(bool, int, int);

public:
	//

	StateMatrix();
	~StateMatrix();

	StateMatrix( int, int );
	StateMatrix( const StateMatrix& );

	//

	void empty();

	void randomize();

	// Getter & Setter
	// ---------------
	int row() const { return M_row; }
	int col() const { return M_col; }

	int forest() const { return forest_count; }
	int fire() const { return fire_count; }
	int ash() const { return forest_count; }

	auto c_matrixptr(){ return &M_coeffs; }

	// .. Assimilées

	int size() const { return M_row * M_col; }

	// operators

	char operator()( int, int );
	void setCoef(char, int, int);

};

#endif
