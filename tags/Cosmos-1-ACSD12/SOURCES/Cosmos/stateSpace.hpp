/*
 *   *  Cosmos
 *
 *  Created by Benoit Barbot on 01/12/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

/*
 *	This class is used to compute the state space of the synchronized product
 * between the SPN and the LHA.
 *
 * This class also export and import this state space and the transition matrix
 *
 */

#include <set>
#include <utility>
#include <string>
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <stack>
#include <cstring>
#include "LHA.hpp"
#include "spn.hpp"

#include <iostream>
#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <boost/numeric/ublas/io.hpp>

//using namespace arma;
using namespace std;

#ifdef __GNUC__
#include <ext/hash_map>
#else
#include <hash_map>
#endif


#ifndef _STATESPACE_HPP
#define	_STATESPACE_HPP

using namespace __gnu_cxx;

struct eqstate{
	bool operator()(const vector<int>* t1,const vector<int>* t2) const {
		for(int it=0; it < t1->size() ; it++)
			if((*t1)[it] != (*t2)[it]) return false;
		return true;
	}
};

struct hashstate{
	int operator()(const vector<int>* t1) const {
		int h = 0;
		for(int it=0; it < t1->size() ; it++)
			h = h * 524287 + (*t1)[it];
		return h;
	}
};

/* 
 * The hash_map structure implement a mapping between a state as a vector
 * of integer and its indice.
 */
typedef hash_map<const vector<int>*, int , hashstate , eqstate > hash_state;

/*
 * This class manage the state space of the model. The state space is
 * stored as a mapping between the states and [0..nbState] the number of state.
 * This class can generate the state space by exploring it.
 *
 */
class stateSpace {
public:
	stateSpace();
	hash_state S;
	double findHash(const vector<int>* vect);	
    
	int nbState;
	int nbTrans;
	boost::numeric::ublas::compressed_matrix<double>* transitionsMatrix;
	boost::numeric::ublas::vector<double>* finalVector;
    vector<double>* muvect;
    
	void exploreStateSpace();
	void buildTransitionMatrix();
		
	SPN N; //The object representing the SPN
	LHA A; //The object representing the LHA
	
	void printP();
	void outputMat();
	void inputMat();
    void inputVect();
	
private:
    vector<vector<int> > *findstate;
    void add_state(vector<int> v);
};

#endif