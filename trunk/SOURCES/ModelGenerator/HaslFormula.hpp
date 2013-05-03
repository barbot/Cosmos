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

#

#ifndef __Cosmos__HaslFormula__
#define __Cosmos__HaslFormula__

#include "../Cosmos/BatchR.hpp"

enum HaslType {
	EXPECTANCY,
	BOUNDED_EXPECTANCY,
	PROBABILITY,
	PDF_PART,
	CDF_PART,
	CONSTANT,
	HASL_PLUS,
	HASL_TIME,
	HASL_DIV,
	RE_AVG
};
	
/**
 *
 * A class to manage Confidence interval.
 */
class ConfInt {
public:
	ConfInt(); //!< Interval representing \f$ \mathbb{R}  \f$.
	ConfInt(double,double); //!< Symetric interval.
	ConfInt(double,double,double); //!< Asymetric interval.
	~ConfInt();
	
	double mean; //!< Middle of the confidence interval.
	double low; //!< Lower bound of the confidence interval.
	double up; //!< Upper bound of the confidence interval.
	double width(void); //!< Width of the confidence interval.
};

/**
 * A class to manage and evaluate an HASL formula.
 * An HASL formula is either a probabilistic operator like PROB or EXPECTANCY
 * or combination of HASL formula.
 * The evaluation of an HASL formula require the first and second moment of
 * of the result of a simulation contain in a BatchResult object.
 * The resul of an evaluation is a confidence interval.
 */
class HaslFormulasTop {
public:
	HaslFormulasTop(double); //!< PROB operator use to compute \f$ \mathbb{P} \f$ .
	HaslFormulasTop(double,double); //!< Constant
	HaslFormulasTop(size_t,double); //!< Expectancy operator use to compute \f$ \mathbb{E} \f$ .
	HaslFormulasTop(HaslType , HaslFormulasTop*, HaslFormulasTop*); //!< Build a tree of operator.
//	HaslFormulasTop(const HaslFormulasTop&);
	
	~HaslFormulasTop();
	
	ConfInt* eval(BatchR&)const; //!< Evaluate the formula over the batch of simulation
	double bound()const; //!< Return a bound on the formula result
	HaslType TypeOp; //!< type of the HaslFormula.
	
protected:
  double Level; //!< The confidence level which should be used for the computation of the confidence interval
  double Value; //!< A double value, have different pupose depending of the type of the operator
  size_t Algebraic; //!< The index of the data in the table of result return by the simulator.
  HaslFormulasTop* left;
  HaslFormulasTop* right;
};



#endif /* defined(__Cosmos__HaslFormula__) */
