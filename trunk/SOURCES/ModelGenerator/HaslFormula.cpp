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
 * file parameters.hpp                                                         *
 * Created by Benoit Barbot on 12/12/12.                                       *
 *******************************************************************************
 */

#include <iostream>
#include "HaslFormula.hpp"
#include <boost/math/distributions/normal.hpp>
#include <boost/math/distributions/binomial.hpp>
#include <limits>

ConfInt::ConfInt(){
	mean = 0;
	low = - std::numeric_limits<double>::infinity();
	up = std::numeric_limits<double>::infinity();
}

/**
 * Symetric confidence interval
 * @param meanArg is the middle of the confidence interval
 * @param width is the width of the confidence interval
 */
ConfInt::ConfInt(double meanArg,double width){
	mean = meanArg;
	low = meanArg - width/2;
	up = meanArg + width/2;
}

/**
 * Asymetric confidence interval.
 * @param meanArg is the mean of the confidence interval it is not necessary
 * the middle of the confidence interval if the distribution is not symetric 
 * and is not approximated by the normal distribution wich is symetric.
 * @param lowArg the Lower bound of the confidence interval.
 * @param upArg the Upper bound of the confidence interval.
 */
ConfInt::ConfInt(double meanArg,double lowArg,double upArg){
	mean = meanArg;
	low = lowArg;
	up = upArg;
}

ConfInt::~ConfInt(){}

double ConfInt::width(){
	return (up-low);
}

HaslFormulasTop::HaslFormulasTop(double l){
	TypeOp = PROBABILITY;
	Level =l;
	Value =0;
	Algebraic = 0;
	left = NULL;
	right = NULL;
}

HaslFormulasTop::HaslFormulasTop(double l,double v){
	TypeOp = CONSTANT;
	Level = 1;
	Value = v;
	Algebraic = 0;
	left = NULL;
	right = NULL;
}

HaslFormulasTop::HaslFormulasTop(size_t al,double l){
	TypeOp = EXPECTANCY;
	Level = l;
	Value = quantile(boost::math::normal() , 0.5 + l / 2.0);
	Algebraic = al;
	left = NULL;
	right = NULL;
}

HaslFormulasTop::HaslFormulasTop(HaslType t, HaslFormulasTop* l,HaslFormulasTop* r){
	TypeOp = t;
	Algebraic = 0;
	Level = 1;
	Value = 0;
	left = l;
	right = r;
}

HaslFormulasTop::~HaslFormulasTop(){
	if(!left) delete left;
	if(!right) delete right;
}

/*HaslFormulasTop::HaslFormulasTop(const HaslFormulasTop& ht){
	TypeOp = ht.TypeOp;
	Algebraic = ht.Algebraic;
	Level = ht.Level;
	Value = ht.Value;
	if(ht.left)left = new HaslFormulasTop(*ht.left);
	else left = NULL;
	if(ht.right)right = new HaslFormulasTop(*ht.right);
	else right = NULL;
}*/

////////////////////////////////////////////////////////////////////////////
// Some remarks about the estimation of the confidence interval adopted here
// Let l=ConfLevel, the confidence level
// l=1-alpha
// Let w=ConfWidth, the size of the confidence interval
//
// Let mu the value to estimate, and x the estimation of mu
// then Prob(x-w/2 <= mu <= x+w/2) = 1-alpha
//
// The confidence interval is given by :
// [x-z(1-alpha/2) * StandardDeviation~ / sqrt(NumberOfObservations) ,
//         x+z(1-alpha/2) * StandardDeviation~ / sqrt(NumberOfObservations)]
//
// z(1-alpha/2)=z(1-(1-l)/2) = z(0.5+l/2)
//
// StandartDeviation~ = sqrt( Variance +1/n)
// This correction come from the Chows and Robbin algorithm to ensure
// The correctness of the stopping condition.
//
////////////////////////////////////////////////////////////////////////////
ConfInt* HaslFormulasTop::eval(BatchR &batch){
	switch (TypeOp) {
		case PROBABILITY:
		{
			double mean = (double)batch.Isucc / (double)batch.I;
			double l = boost::math::binomial_distribution<>::find_lower_bound_on_p(batch.I,batch.Isucc, (1-Level)/2);
			double u = boost::math::binomial_distribution<>::find_upper_bound_on_p(batch.I,batch.Isucc, (1-Level)/2);
			
			return new ConfInt(mean,l,u);
		}
			
		case EXPECTANCY:
		case CDF_PART:
		case PDF_PART:
		{
			double mean = batch.Mean[Algebraic]/batch.Isucc;
			double m2 = batch.M2[Algebraic]/batch.Isucc;
			double variance = m2 - mean * mean;
			
			variance += 1.0/batch.Isucc;
			//Here the +1 come from the Chows and Robbin algorithm
			
			double width = 2 * Value * sqrt(variance/batch.Isucc);
			
			return new ConfInt(mean,width);
		}
			
		case CONSTANT:
			return new ConfInt(Value,0);
			
		case HASL_PLUS:
		{
			ConfInt* lci = left->eval(batch);
			ConfInt* rci = right->eval(batch);
			
			double mean = lci->mean+rci->mean;
			double low = lci->low+rci->low;
			double up = lci->up +rci->up;
			
			delete lci;
			delete rci;
			
			return new ConfInt(mean,low,up);
		}
			
		case HASL_TIME:
		{
			ConfInt* lci = left->eval(batch);
			ConfInt* rci = right->eval(batch);
			
			double mean = lci->mean*rci->mean;
			double low = lci->low*rci->low;
			double up = lci->up *rci->up;
			
			delete lci;
			delete rci;
			
			return new ConfInt(mean,low,up);
		}
			
		case HASL_DIV:
		{
			ConfInt* lci = left->eval(batch);
			ConfInt* rci = right->eval(batch);
			
			if(rci->low * rci->up<=0)
				return new ConfInt();
			
			double mean = lci->mean / rci->mean;
			double low,up;
			
			if(rci->low > 0){
				low = lci->low / rci->up;
				up  = lci->up  / rci->low;
			}else{
				low = lci->low / rci->low;
				up  = lci->up  / rci->up;
			}
			
			delete lci;
			delete rci;
			
			return new ConfInt(mean,low,up);
		}
			
		default:
			std::cerr << "Fail to parse Hasl Formula"<< std::endl;
			exit(EXIT_FAILURE);
	}
}
