/*
 *  numericalSolver.cpp
 *  Cosmos
 *
 *  Created by Benoit Barbot on 05/12/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "numericalSolver.hpp"

#include <iostream>
#include <fstream>
#include <time.h>
#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_expression.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/operation_sparse.hpp>
#include <boost/numeric/ublas/operation.hpp>
#include <boost/numeric/ublas/vector.hpp>

#define BOOST_UBLAS_NDEBUG

using namespace std;


numericalSolver::numericalSolver(){
    cerr << "Initialising the simulator" << endl;
    cerr << "Reading Matrix" << endl;
    time_t start, endt;
    time(&start);
	inputMat();
    time(&endt);
    cerr << "time for input:" << endt-start << endl;
    
}


void numericalSolver::sparseProd(boostmat::vector<double> *result,boostmat::vector<double> *vect, boostmat::compressed_matrix<double> *mat){
    //*result = boostmat::zero_vector<double> (vect->size());
    
    //boostmat::axpy_prod((const boostmat::compressed_matrix<double,boostmat::row_major>) *mat,(const boostmat::vector<double>) *vect,*result,true);

    
    //boostmat::axpy_prod(*mat, *vect, *result, boostmat::row_major );
    
    //boostmat::sparse_prod<double>(mat, vect);
    
    //boostmat::sparse_prod(mat, vect, result);
    
    
    
    for(boostmat::compressed_matrix<double>::iterator1 it = mat->begin1(); it!= mat->end1(); it++){
        for(boostmat::compressed_matrix<double>::iterator2 it2 = it.begin(); it2!= it.end(); it2++){
            //cerr << "iteration: " << it.index1() << ":" << it2.index2() << endl;
            (*result) (it.index1()) += ((*vect) (it2.index2()) * (*it2));
        };        
    };
}

void numericalSolver::initVect(int nT){
    time_t start, endt;
    time(&start);

    
	T=nT;
	circularvect = new vector< boostmat::vector<double> > (nT+1, boostmat::zero_vector<double> (finalVector->size()));
	(*circularvect)[0] = *finalVector;
    time(&endt);
    cerr << "time for allocation:" << difftime(endt, start) << endl;
    
	//cerr << "itervect:" << 0 << ":"<< (*circularvect)[0] << endl;
    
    //int test=0;
    boostmat::vector<double> itervect = boostmat::vector<double>(*finalVector);
    //boostmat::vector<double> itervect2(finalVector->size());
	for(int i=1; i<=nT ; i++){
        sparseProd(&((*circularvect)[i]), &((*circularvect)[i-1]), transitionsMatrix);
        
        //boostmat::sparse_prod(transitionsMatrix, (*circularvect)[i] , (*circularvect)[i-1] );
        
        
		//cerr << "itervect:" << i << ":"<< (*circularvect)[i] << endl;
	}
	nbVect = nT;
	matOffset = nT;
    time(&endt);
    cerr << "time for precalculation:" << difftime(endt, start) << endl;
    
	cerr << "Starting the simulation" << endl;
}

void numericalSolver::reset(){
	matOffset=T;
	//cerr << (*circularvect)[matOffset] << endl;

}

boostmat::vector<double> numericalSolver::getVect(){
	return (*circularvect)[matOffset];
}

double numericalSolver::getMu(int i){
	//cerr << "indice" << matOffset << endl;
	if (matOffset>=0) return (*circularvect)[matOffset][i];
	else return 0.0;
}

void numericalSolver::stepVect(){
	matOffset--;
	//cerr << " # "<< matOffset <<" # ";
}

void numericalSolver::previousVect(){
	matOffset++;
}










