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
 * file BatchR.cpp created by Benoit Barbot.                                   *
 *******************************************************************************
 */

#include "BatchR.hpp"
#include <unistd.h>
#include <math.h>
#include <float.h>

using namespace std;


//Initialize the batch with zeros
BatchR::BatchR(int i) {
    I =0;
    Isucc=0;
    TableLength=i;
    IsBernoulli= vector<bool>(i,true);
    Mean=vector<double>(i,0.0);
    M2=vector<double>(i,0.0);
}

BatchR::~BatchR() {
}

//Add a simulation to the batch
//If the simulation is a success then The Mean and second Moment are updated
void BatchR::addSim(SimOutput *Result){
    I++;
    if (Result->first) {
        Isucc++;
        double Dif=0;
        
        for(int i =0; i< TableLength; i++){
            
            if (Result->second[i] * (1 - Result->second[i]) != 0){
                IsBernoulli[i] = false;
            }
            
            Dif = Result->second[i] - Mean[i];
            Mean[i] += Dif / Isucc;
            
            Dif = pow(Result->second[i], 2) - M2[i];
            M2[i] += Dif / Isucc;
        }
    }
}

void BatchR::unionR(BatchR *batch){
    double Dif;
    
    I += batch->I;
    Isucc += batch->Isucc;
    
    for(int i =0; i< TableLength; i++){
        IsBernoulli[i] = IsBernoulli[i] && batch->IsBernoulli[i];
        
        Dif = batch->Mean[i] - Mean[i];
        Mean[i] += batch->Isucc * Dif / Isucc;
        
        Dif = batch->M2[i] - M2[i];
        M2[i] += batch->Isucc * Dif / Isucc;
    }
}

//This function write a batch on the standart output it is suposed to
//be read by the function BatchR::inputR
void BatchR::outputR() {
    int size;
    size = write(STDOUT_FILENO,reinterpret_cast<char*>(&I),sizeof(int));
    size = write(STDOUT_FILENO,reinterpret_cast<char*>(&Isucc),sizeof(int));

    for(int i =0; i< TableLength; i++){
        bool tmpbool = IsBernoulli[i];
        size = write(STDOUT_FILENO,reinterpret_cast<char*>(&tmpbool),sizeof(bool));
        size = write(STDOUT_FILENO,reinterpret_cast<char*>(&Mean[i]),sizeof(double));
        size = write(STDOUT_FILENO,reinterpret_cast<char*>(&M2[i]),sizeof(double));
    }
    
    //print();
    fflush(stdout);
}

//Read a batch from a file.
void BatchR::inputR(FILE* f) {
    double read;
    bool readb;
    int readi;
    int size;
    
    size = fread(reinterpret_cast<char*>( &readi ), sizeof readi ,1, f);
    I=readi;
    size = fread(reinterpret_cast<char*>( &readi ), sizeof readi ,1, f);
    Isucc=readi;

    for(int i =0; i< TableLength; i++){
        size = fread(reinterpret_cast<char*>( &readb ), sizeof readb ,1, f);
        IsBernoulli[i]=readb;
        size = fread(reinterpret_cast<char*>( &read ), sizeof read ,1, f);
        Mean[i]=read;
        size = fread(reinterpret_cast<char*>( &read ), sizeof read ,1, f);
        M2[i]=read;
    }
    fflush(stdout);
}

//Print human readable version of batch on stdout.
void BatchR::print(){
    cerr << "I:\t" << I << endl << "Isucc:\t" << Isucc << endl;
    for(int i =0; i< TableLength; i++){
        cerr << "Mean:\t" << Mean[i] << endl << "M2:\t" << M2[i] << endl << "IsBernoulli:\t" << IsBernoulli[i] << endl;
    }
}