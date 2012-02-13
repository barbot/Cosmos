/*
 *  untitled.cpp
 *  Cosmos
 *
 *  Created by Benoit Barbot on 01/12/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "stateSpace.hpp"
#include <deque>
#include <set>
#include <iostream>
#include <fstream>
#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_expression.hpp>
#include <boost/numeric/ublas/io.hpp>

#include "sparse_io.hpp"

namespace boostmat = boost::numeric::ublas;
//using namespace boost::numeric::ublas;

stateSpace::stateSpace(){
	nbState=0;
	nbTrans=0;
	N.Load();//load the GSPN
	A.Load(); //load the LHA
}

double stateSpace::findHash(const vector<int>* vect){
    hash_state::iterator it = S.find (vect);
    if (it != S.end ())
		return(it->second); // found
    else
		return(-1);
}

void stateSpace::add_state(vector<int> v){
    vector<int>* v2 = new vector<int>(v);
    S[v2] = nbState;
    findstate->push_back(*v2);
	nbState++;
    
}

void stateSpace::exploreStateSpace(){
	// apply a Dijkstra algorithm on the product of the SPN an the LHA to produce
	// the state space. The list of state is store in the hash table S and 
	// the transition list is stored in transitionList.

    cerr << "Exploring state space" << endl;
    
	stack<vector<int> ,vector<vector<int> > > toBeExplore;
	vector<int> init = N.initMarking;
	
	set <int, less<int> > ::iterator it;
	
	init.push_back( *(A.InitLoc.begin()) );
	toBeExplore.push(init);
    findstate = new vector<vector<int> >(0);
    add_state(init);
    
	while (!toBeExplore.empty()) {
		vector<int> place = toBeExplore.top();
		toBeExplore.pop();
		
		vector<int> currentstate = place;
		
		A.setCurrentLocation(place.back());
		place.pop_back();
		N.setMarking(place);
		
		set<int, less <int> > ent;
		ent = N.enabledTrans();
		set<int>::iterator it;
		for (it = ent.begin(); it != ent.end(); it++) {
			
			N.fire(*it);
			//cerr << "transition:" << *it << endl;
			vector<int> marking = N.Marking;
			N.unfire(*it);
			
			int SE = A.GetEnabled_S_Edges(A.CurrentLocation, *it , 0.0 , N.Marking, marking);
			if (SE > -1) {
				
				nbTrans++;
				marking.push_back( A.Edge[SE].Target );
				//vector<double> Param = N.GetDistParameters(*it);
				//transitionsList.push( make_pair(make_pair(currentstate, marking),Param[0] ));
				
				hash_state::iterator its = S.find (&marking);
				if (its == S.end ()){
					
					//cerr << "state:"<< nbState << " -> ";
					for (vector<int>::iterator it=marking.begin(); it!= marking.end() ; it++) {
						//cerr << *it << ":";
					}
					//cerr << endl;
					
					toBeExplore.push(marking);
                    add_state(marking);
				}
			}
			
		}
		
	}
    cerr << nbState << " states found" << endl 
        << nbTrans << " transitions found" << endl;
}

void stateSpace::buildTransitionMatrix()
{
    cerr << "Building transition matrix" << endl;
    
	// transform the transition list into a sparse transition probability matrix 
	boost::numeric::ublas::compressed_matrix<double> mat(nbState, nbState, nbTrans);
	
	cerr << "Exploring graph" << endl;
    
    for (int i=0; i<nbState; i++) {
        vector<int> place = (*findstate)[i];
		vector<int> currentstate = place;
		
		A.setCurrentLocation(place.back());
		place.pop_back();
		N.setMarking(place);
		
		set<int, less <int> > ent;
		ent = N.enabledTrans();
		set<int>::iterator it;
        mat (i,i) = 1.0;
		for (it = ent.begin(); it != ent.end(); it++) {
			
			N.fire(*it);
			vector<int> marking = N.Marking;
			N.unfire(*it);
			
			int SE = A.GetEnabled_S_Edges(A.CurrentLocation, *it , 0.0 , N.Marking, marking);
			if (SE > -1) {
				
				marking.push_back( A.Edge[SE].Target );
				vector<double> Param = N.GetDistParameters(*it);
				
				int j = findHash(&marking);
                mat (i,j) = Param[0];
			}
			
		}

        
    }
	    
    cerr << "Adding self loop" << endl;
	// Add self loop to ensure that mat is a probability matrix.
	typedef boost::numeric::ublas::compressed_matrix<double>::iterator1 it1_t;
	typedef boost::numeric::ublas::compressed_matrix<double>::iterator2 it2_t;
	
	for (it1_t it1 = mat.begin1(); it1 != mat.end1(); it1++)
	{
		double sum = 1.0;
		for (it2_t it2 = it1.begin(); it2 != it1.end(); it2++){
			//cerr << "non null:" << it2.index1() << ":" << it2.index2() << endl;
			if(it2.index1()!= it2.index2())sum -= *it2;
		}
		mat(it1.index1(),it1.index1())= sum;
	}
	
    cerr << " copping" << endl;
    
	transitionsMatrix = new boost::numeric::ublas::compressed_matrix<double>(mat);
    
	boost::numeric::ublas::vector<double> vect(nbState);
	for(hash_state::iterator it=S.begin();  it!=S.end() ; it++){
		if(A.isFinal(it->first->back())){
			vect(it->second)=1.0;
			//cerr << "final:" << it->second << endl;
		}else {
			vect(it->second)=0.0;
		}

	}
	
	finalVector = new boost::numeric::ublas::vector<double> (vect);
}

void stateSpace::printP(){
	cerr << "Probability transition matrix:" << endl;
	for(int i=0; i< transitionsMatrix->size1() ; i++){
		for(int j = 0;j< transitionsMatrix->size2() ; j++)
			cerr << (*transitionsMatrix)(i,j) << "\t";
		cerr << endl;
	}
	cerr << endl << "Final Vector"<< endl;
	for(int i=0; i< finalVector->size() ; i++){
		cerr << (*finalVector)(i);
		cerr << endl;
	}
}

void stateSpace::outputMat(){
    cerr << "Exporting the transition matrix" << endl;
    
	fstream outputFile;
	outputFile.open("matrixFile",fstream::out);
	
	outputFile << boostmat::io::sparse(*transitionsMatrix);
	outputFile << *finalVector << endl;
	
	for(hash_state::iterator it= S.begin() ; it != S.end(); it++){
		outputFile << "(";
		vector<int> vect = *(*it).first;
		for(int i =0; i< vect.size()-1; i++){
			if(i>0)outputFile << ",";
			outputFile << vect[i];
		}
		outputFile << ")=";
		outputFile << (*it).second << endl;
	}
		
	outputFile.close();
}

void stateSpace::inputVect(){
    string line;
	int poseq;
	string pos;
	string prob;
	ifstream myfile ("mu_table");
	if (myfile.is_open())
	{ 
        muvect = new vector<double>;
        int n=0;
		while ( myfile.good() )
		{
			getline (myfile,line);
			poseq = line.find("=");
			
			if(poseq > 0){
				pos = line.substr(1,poseq-2);
				prob = line.substr(poseq+1,line.size());
				
				vector<int> vect;
				int it = 0;
				while(it < pos.length()){
					int it2 = pos.find(",",it);
					if(it2 == -1) it2 = pos.length();
					//cout << "test:" << it<< ":" << it2 << endl;
					vect.push_back(atoi((pos.substr(it,it2-it)).c_str() ));
					it = it2+1;
				}
				
                muvect->push_back(atof(prob.c_str()));
				S[new vector<int>(vect)] = n;
                n++;
				
				
			}
		}
		myfile.close();
		nbState = n;
		
    }
	
	else cerr << "Unable to open file "<< "mu_table" << endl ; 
    
}

void stateSpace::inputMat(){
	fstream inputFile;
	inputFile.open("matrixFile",fstream::in);
	
	/*boostmat::matrix<double> m1;
	inputFile >> m1;
	nbState = m1.size1();*/
	boostmat::compressed_matrix<double , boostmat::row_major> m;
    inputFile >> boostmat::io::sparse(m);
    
	/*for (unsigned i = 0; i < nbState; ++ i)
        for (unsigned j = 0; j < nbState; ++ j)
            if(m1 (i,j) != 0.)  m (i, j) = m1 (i,j);*/
	transitionsMatrix = new boostmat::compressed_matrix<double>(m);
	
	//cerr << *transitionsMatrix << endl;
	
	boostmat::vector<double> v1;
	inputFile >> v1;
	finalVector = new boostmat::vector<double>(v1);
	
	string line;
	int poseq;
	string pos;
	string stateid;
	while ( inputFile.good() )
    {
		getline (inputFile,line);
		//cerr << line << endl;
		poseq = line.find("=");
		
		if(poseq > 0){
			pos = line.substr(1,poseq-2);
			stateid = line.substr(poseq+1,line.size());
			
			vector<int> vect;
			int it = 0;
			while(it < pos.length()){
				int it2 = pos.find(",",it);
				if(it2 == -1) it2 = pos.length();
				vect.push_back(atoi((pos.substr(it,it2-it)).c_str() ));
				it = it2+1;
			}
			
			S[new vector<int>(vect)] = (int)atoi(stateid.c_str());
			
		}
	}
	
	inputFile.close();
	
	cerr << "DTMC size:" << finalVector->size() << endl;
	
}