/*******************************************************************************
 *                                                                             *
 * Cosmos:(C)oncept et (O)utils (S)tatistique pour les (Mo)deles               *
 * (S)tochastiques                                                             *
 *                                                                             *
 * Copyright (C) 2009-2012 LSV & LACL                                          *
 * Authors: Paolo Ballarini Benoît Barbot & Hilal Djafri                       *
 * Website: http://www.lsv.ens-cachan.fr/Software/cosmos                       *
 *                                                                             *
 * This program is free software; you can redistribute it and/or modify        *
 * it under the terms of the GNU General Public License as published by        *
 * the Free Software Foundation; either version 3 of the License, or           *
 * (at your option) any later version.                                         *
 *                                                                             *
 * This program is distributed in the hope that it will be useful,             *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of              *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               *
 * GNU General Public License for more details.                                *
 *                                                                             *
 * You should have received a copy of the GNU General Public License along     *
 * with this program; if not, write to the Free Software Foundation, Inc.,     *
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.                 *
 * file SimulatorRE.cpp created by Benoit Barbot on 09/11/11.                  *
 *******************************************************************************
 */

#include "EventsQueue.hpp"
#include <iostream>
#include <set>
#include <math.h>
#include <float.h>
#include <boost/random.hpp>
#include <boost/generator_iterator.hpp>
#include <boost/math/distributions/normal.hpp>
#include <boost/math/distributions/binomial.hpp>
#include <time.h>
#include "marking.hpp"
//#include "RareEvent.hpp"

#include "SimulatorRE.hpp"

using namespace std;

SimulatorRE::SimulatorRE(bool b) {
	doubleIS_mode=b;
	muprob.inputVect();
}

SimulatorRE::SimulatorRE(){
	SimulatorRE(false);
}

void SimulatorRE::InitialEventsQueue() {
	
	Initialized = true;
	
	
	Event E;
	N.Rate_Sum = 0;
	N.Origine_Rate_Sum = 0;
	
	for (size_t t = 0; t < N.tr; t++) {
		// Loop over all binding here
		abstractBinding b;

		if (N.IsEnabled(t,b)) {
			GenerateEvent(E, t, b);
			(*EQ).insert(E);
		}
	}
}

void SimulatorRE::returnResultTrue(){
	
	A.UpdateLinForm(N.Marking);
	A.UpdateLhaFuncLast();
	A.UpdateFormulaVal();
	Result.first = true;
	Result.second = vector<double>(A.FormulaVal);
    for(size_t i = 0; i< A.FormulaVal.size() ; i++){
        Result.second[i] *= A.Likelihood;
    }
	if(verbose>3)cerr << "---------------\n TRUE \n------\n";
}

void SimulatorRE::returnResultFalse(){
	Result.first = false;
	Result.second[0] =0.0;
	if(verbose>3)cerr << "---------------\n FALSE \n------\n";
}

void SimulatorRE::updateSPN(const int E1_transitionNum,const abstractBinding& b ){
	Event F;
    //check if the current transition is still enabled
	for(vector<abstractBinding>::const_iterator bindex = N.Transition[E1_transitionNum].bindingList.begin() ;
		bindex != N.Transition[E1_transitionNum].bindingList.end() ; ++bindex){
		bool Nenabled = N.IsEnabled(E1_transitionNum, *bindex);
		bool NScheduled = EQ->isScheduled(E1_transitionNum, bindex->id());
		
		if (Nenabled && NScheduled) {
			GenerateEvent(F, E1_transitionNum, *bindex);
			EQ->replace(F); //replace the transition with the new generated time
		} else if (Nenabled && !NScheduled) {
			GenerateEvent(F, E1_transitionNum, *bindex);
			EQ->insert(F);
		} else if (!Nenabled && NScheduled) {
			EQ->remove(E1_transitionNum,bindex->id() );
		}
	}
	
	// Possibly adding Events corresponding to newly enabled-transitions
	const set<int>* net = N.PossiblyEn();
	for (set<int>::iterator it = net->begin(); it != net->end(); it++) {
		for(vector<abstractBinding>::const_iterator bindex = N.Transition[*it].bindingList.begin() ;
			bindex != N.Transition[*it].bindingList.end() ; ++bindex){
			if (N.IsEnabled(*it,*bindex)) {
				if (!EQ->isScheduled((*it),bindex->id())) {
					GenerateEvent(F, (*it), *bindex);
					(*EQ).insert(F);
				} else {
					if (N.Transition[(*it)].MarkingDependent) {
						GenerateEvent(F, (*it),*bindex);
						(*EQ).replace(F);
					}
				}
			}
		}
	}
	
	// Possibly removing Events corresponding to newly disabled-transitions
	const set<int>* ndt = N.PossiblyDis();
	for (set<int>::iterator it = ndt->begin(); it != ndt->end(); it++) {
		for(vector<abstractBinding>::const_iterator bindex = N.Transition[*it].bindingList.begin() ;
			bindex != N.Transition[*it].bindingList.end() ; ++bindex){
			if (EQ->isScheduled(*it, bindex->id())) {
				if (!N.IsEnabled(*it, *bindex ))
					EQ->remove(*it,bindex->id());
				else {
					if (N.Transition[(*it)].MarkingDependent) {
						GenerateEvent(F, (*it),*bindex);
						EQ->replace(F);
					}
				}
			}
		}
	}
	
	N.Rate_Sum = 0;
	N.Origine_Rate_Sum = 0;
	
	for (size_t it = 0 ; it < EQ->getSize() ; it++) {
		size_t tr = EQ->InPosition(it).transition;
		if(tr != N.tr-1){
			if(N.IsEnabled(tr,EQ->InPosition(it).binding)){
				GenerateEvent(F, tr ,EQ->InPosition(it).binding );
				EQ->replace(F);
			}else {
				EQ->remove(tr, EQ->InPosition(it).binding.id());
			}
		}; 
	};
	
	abstractBinding bpuit;
	GenerateEvent(F, (N.tr-1), bpuit);
	if(!doubleIS_mode){
		EQ->replace(F);
	}
	
};

void SimulatorRE::updateLikelihood(int E1_transitionNum){
    //cerr << "initialised?:\t" << E1_transitionNum << endl;
	if(doubleIS_mode){	
		A.Likelihood = A.Likelihood * 
		(N.Origine_Rate_Table[E1_transitionNum] / N.Origine_Rate_Sum) * 
		((N.Rate_Sum-N.Rate_Table[N.tr-1]) / N.Rate_Table[E1_transitionNum]);
	}else{
		A.Likelihood = A.Likelihood * 
		//
		//(N.Origine_Rate_Table[E1_transitionNum] / 1.0) *
		(N.Origine_Rate_Table[E1_transitionNum] / N.Origine_Rate_Sum) *
		(N.Rate_Sum / N.Rate_Table[E1_transitionNum]);
	}
}

bool SimulatorRE::transitionSink(int i){
    return (i==N.tr-1);
}

void SimulatorRE::GenerateDummyEvent(Event& E, int Id) {
    E.transition = Id;
    E.time = 0.0;
    E.priority = N.GetPriority(Id);
    E.weight = 0.0;
}
	
void SimulatorRE::GenerateEvent(Event& E,const int Id,const abstractBinding& b) {
	
    double t = A.CurrentTime;
    if (N.Transition[Id].transType == Timed) {
        getParams(Id, b);
        t += GenerateTime(N.Transition[Id].DistTypeIndex, N.ParamDistr);
		
		N.Rate_Table[Id] = N.ParamDistr[0];
		N.Origine_Rate_Table[Id] = N.ParamDistr[1];
		N.Rate_Sum = N.Rate_Sum + N.ParamDistr[0];
		N.Origine_Rate_Sum = N.Origine_Rate_Sum + N.ParamDistr[1];
		
    }
	double w=0.0;
	if (N.Transition[Id].DistTypeIndex > 2) {
		N.ParamDistr[0]= N.GetWeight(Id);
		w = GenerateTime(2, N.ParamDistr);
		//vector<double> wParam(1, N.GetWeight(Id));
		//w = GenerateTime(2, wParam);
    }
	
    E.transition = Id;
    E.time = t;
    E.priority = N.GetPriority(Id);
    E.weight = w;
	E.binding = b;
	
}

void SimulatorRE::reset(){
	Simulator::reset();
	N.Origine_Rate_Sum=0;
	N.Rate_Sum=0;
}

void SimulatorRE::getParams(const int Id,const abstractBinding& b){
	
	N.GetDistParameters(Id,b);
	N.ParamDistr[1]=N.ParamDistr[0];
	N.ParamDistr[0]= ComputeDistr( Id, b, N.ParamDistr[0]);
}


double SimulatorRE::mu(){
	
	vector<int> vect (N.Msimpletab.size());
	for(size_t i=0; i< N.Msimpletab.size();i++){
		vect[i] = N.Marking.getNbOfTokens(N.Msimpletab[i]);
		if(verbose>3)cerr << i << " : " << N.Msimpletab[i] << " : " << N.Marking.getNbOfTokens(N.Msimpletab[i]) << endl;
	};
	
    N.lumpingFun(&vect);
    //cerr << "test(";
    int i = muprob.findHash(&vect);
    if(i<0){
        cerr << "state:(";
        for (size_t j =0; j < vect.size(); j++) {
            cerr << vect[j] << ",";
        }
        cerr << ") ->" << i << endl;
       // exit(EXIT_FAILURE);
    }
	if(verbose>3) cerr << ((*muprob.muvect)[i]) << endl;
    //cerr << ")" << endl;
	return((*muprob.muvect)[i]);
}

double SimulatorRE::ComputeDistr(const int t , const abstractBinding& b, double origin_rate){
	
	double mux = mu();
	if( mux==0.0 || mux==1.0) return(origin_rate);
    
	if(t== N.tr-1){
		if(N.Origine_Rate_Sum >= N.Rate_Sum){
			return( (N.Origine_Rate_Sum - N.Rate_Sum)  );
		}else{ 
			return 0.0 ;};
	}; 
	
	double distr;
	N.fire(t,b);
	distr = origin_rate *( mu() / mux);
	N.unfire(t,b);
	return(distr);
}

