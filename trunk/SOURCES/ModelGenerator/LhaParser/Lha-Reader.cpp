/*******************************************************************************
 *                                                                             *
 * Cosmos:(C)oncept et (O)utils (S)tatistique pour les (Mo)deles               *
 * (S)tochastiques                                                             *
 *                                                                             *
 * Copyright (C) 2009-2012 LSV & LACL                                          *
 * Authors: Paolo Ballarini & Hilal Djafri                                     *
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
 *******************************************************************************
 */

#include "lex.lha.c"

#include "Lha-parser.tab.hh"
#include "Lha-Reader.hpp"
#include "Lha_gmlparser.hpp"

#include "string.h"

#include <string>
#include <sstream>
#include <fstream>
#include <set>




using namespace std;

Lha_Reader::Lha_Reader() {
    trace_scanning = false;
    trace_parsing = false;
	
}

Lha_Reader::~Lha_Reader() {
}

int Lha_Reader::parse(string& expr) {
	
	
	
	
    IndexDist["UNIFORM"] = 1;
    IndexDist["EXPONENTIAL"] = 2;
    IndexDist["DETERMINISTIC"] = 3;
    IndexDist["LOGNORMAL"] = 4;
    IndexDist["TRIANGLE"] = 5;
    IndexDist["GEOMETRIC"] = 6;
	
    scan_expression(expr);
	
    lha::Lha_parser parser(*this);
	
    parser.set_debug_level(trace_parsing);
	
    int res = parser.parse();
    scan_end();
    return res;
}

int Lha_Reader::parse_file(parameters& P) {
    string str;
	MyLha.ConfidenceLevel=P.Level;
	
    ifstream file(P.PathLha.c_str(), ios::in);
    if (file) {
		
		
        while (!file.eof()) {
			
            string str2;
            getline(file, str2);
            str = str + "\n" + str2;
        }
		
        int x = parse(str);
        if (x) cout << "Parsing LHA description file failed" << endl;
		
		
        return x;
    } else {
        cout << "Can't open : " << P.PathLha << endl;
        return 1;
    }
}
int Lha_Reader::parse_gml_file(parameters& P) {
	MyLha.ConfidenceLevel=P.Level;
    ifstream ifile(P.PathLha.c_str());
    if(ifile){
        //cout << "parse GML:" << filename << endl;
        ModelHandlerPtr handlerPtr(new MyLhaModelHandler(&MyLha));
        ExpatModelParser parser = ExpatModelParser(handlerPtr);
        parser.parse_file(P.PathLha);
        //cout << "end parse GML"<< endl;
        return 0;
    }else{
        cout << "File " << P.PathLha << " does not exist!" << endl;
        exit(EXIT_FAILURE);
    }
    
	
}

string Lha_Reader::InvRelOp(string& str) {
    if (str == "<=") return ">=";
    if (str == ">=") return "<=";
	cerr << "Fail to inverse RelOp"<< endl;
	exit(EXIT_FAILURE);
}

void
Lha_Reader::error(const lha::location& l, const std::string& m) {
    std::cerr << l << ": " << m << std::endl;
}

void
Lha_Reader::error(const std::string& m) {
    std::cerr << m << std::endl;
}

void Lha_Reader::view() {
	
	
}

void Lha_Reader::WriteFile(parameters& P) {
	string Pref = P.tmpPath;
	
    string loc;
	
    //loc = Pref + "../SOURCES/Cosmos/LHA.cpp";
    loc = Pref + "/LHA.cpp";
	
	ofstream LhaCppFile(loc.c_str(), ios::out | ios::trunc);
	
    /*loc = Pref + "../SOURCES/ModelGenerator/LHA_orig.cpp";
	 ifstream LhaCppFile_orig(loc.c_str(), ios::in);
	 
	 while (!LhaCppFile_orig.eof()) {
	 
	 string LineFile;
	 getline(LhaCppFile_orig, LineFile);
	 LhaCppFile << LineFile << endl;
	 }
	 LhaCppFile_orig.close();
	 */
	
	LhaCppFile << "#include \"LHA_orig.hpp\"" << endl;
	LhaCppFile << "#include \"markingImpl.hpp\"" << endl;
	LhaCppFile << "#include <math.h>" << endl;
	LhaCppFile << "#include <float.h>" << endl;
	LhaCppFile << "#include \"LHA.hpp\"" << endl;
	
	
	for (map<string,double>::iterator it= MyLha.LhaRealConstant.begin(); it!= MyLha.LhaRealConstant.end() ; it++) {
		LhaCppFile << "    const double " << it->first << "=" << it->second << ";" << endl;
	}
	
	LhaCppFile << "struct Variables {\n";
	for(size_t v =0 ; v< MyLha.NbVar ; v++){
		LhaCppFile << "\tdouble "<< MyLha.VarLabel[v] << ";\n";
	}
	LhaCppFile << "};\n";
	
	LhaCppFile << "void LHA::resetVariables(){\n";
	for(size_t v= 0 ; v < MyLha.NbVar; v++){
		LhaCppFile << "\tVars->"<< MyLha.VarLabel[v] << "= 0;\n";
	}
	LhaCppFile << "};\n";
	
	LhaCppFile << "void LHA_ORIG::printState(){\n";
	LhaCppFile << "\tcerr << \"Location:\"<< LocLabel[CurrentLocation] << endl;\n";
	for(size_t v= 0 ; v < MyLha.NbVar; v++){
		LhaCppFile << "\tcerr << \"" << MyLha.LocLabel[v] << " = \" << Vars->"<< MyLha.VarLabel[v] << " << endl;\n";
	}
	LhaCppFile << "};\n";

		
    LhaCppFile << "LHA::LHA(){" << endl;
	
	
    LhaCppFile << "    NbLoc =" << MyLha.NbLoc << ";" << endl;
    LhaCppFile << "    NbVar =" << MyLha.NbVar << ";" << endl;
    for (set<unsigned int>::iterator it = MyLha.InitLoc.begin(); it != MyLha.InitLoc.end(); it++)
        LhaCppFile << "    InitLoc.insert(" << (*it) << ");" << endl;
    for (set<unsigned int>::iterator it = MyLha.FinalLoc.begin(); it != MyLha.FinalLoc.end(); it++)
        LhaCppFile << "    FinalLoc.insert(" << (*it) << ");" << endl;
	
	if(P.CountTrans)
		LhaCppFile << "    EdgeCounter = vector<int>("<<MyLha.Edge.size()<<",0);"<< endl;
    LhaCppFile << "    vector<LhaEdge> ve(" << MyLha.Edge.size() << ");" << endl;
    LhaCppFile << "    Edge= ve;\n" << endl;
	
	if(P.StringInSpnLHA){
		for (map <string, int>::iterator it = MyLha.LocIndex.begin(); it != MyLha.LocIndex.end(); it++)
			LhaCppFile << "    LocIndex[\"" << (*it).first << "\"]=" << (*it).second << ";" << endl;
		
		LhaCppFile << "\n    vector<string> vlstr(NbLoc);" << endl;
		LhaCppFile << "    LocLabel= vlstr;" << endl;
		LhaCppFile << "    StrLocProperty= vlstr;\n" << endl;
		
		LhaCppFile << "    vector < vector <string> >  vestr(" << MyLha.Edge.size() << ");" << endl;
		LhaCppFile << "    ConstraintsRelOp= vestr;" << endl;
		LhaCppFile << "    ConstraintsConstants= vestr;" << endl;
		
		
		LhaCppFile << "    vector < vector< vector <string> > > mvestr(" << MyLha.Edge.size() << ");" << endl;
		
		LhaCppFile << "    ConstraintsCoeffs= mvestr;\n" << endl;
		
		for (size_t i = 0; i < MyLha.LocLabel.size(); i++)
			LhaCppFile << "    LocLabel[" << i << "]=\"" << MyLha.LocLabel[i] << "\";" << endl;
		
		for (size_t i = 0; i < MyLha.StrLocProperty.size(); i++)
			LhaCppFile << "    StrLocProperty[" << i << "]=\"" << MyLha.StrLocProperty[i] << "\";" << endl;
	}
	
    for (map <string, int>::iterator it = MyLha.EdgeIndex.begin(); it != MyLha.EdgeIndex.end(); it++)
        LhaCppFile << "    EdgeIndex[\"" << (*it).first << "\"]=" << (*it).second << ";" << endl;
	
    for (size_t i = 0; i < MyLha.Edge.size(); i++) {
        LhaCppFile << "    Edge[" << i << "].Index=" << MyLha.Edge[i].Index << ";" << endl;
        LhaCppFile << "    Edge[" << i << "].Source=" << MyLha.Edge[i].Source << ";" << endl;
        LhaCppFile << "    Edge[" << i << "].Target=" << MyLha.Edge[i].Target << ";" << endl;
        if (MyLha.EdgeActions[i].size() < 1)
            LhaCppFile << "    Edge[" << i << "].Type= Auto ;" << endl;
        else
            LhaCppFile << "    Edge[" << i << "].Type= Synch;" << endl;
		
    }
	
    for (size_t i = 0; i < MyLha.Edge.size(); i++) {
		
        size_t NbC = MyLha.ConstraintsRelOp[i].size();
		
		if(P.StringInSpnLHA){
			LhaCppFile << "\n    {" << endl;
			LhaCppFile << "    vector <string> vcstr(" << NbC << ");" << endl;
			LhaCppFile << "    ConstraintsRelOp[" << i << "]=vcstr;" << endl;
			LhaCppFile << "    ConstraintsConstants[" << i << "]=vcstr;" << endl;
			LhaCppFile << "    vector <string>  vcvstr(NbVar, \"\");" << endl;
			LhaCppFile << "    vector < vector <string> > v2cvstr(" << NbC << ",vcvstr);" << endl;
			LhaCppFile << "    ConstraintsCoeffs[" << i << "]=v2cvstr;" << endl;
			LhaCppFile << "    }" << endl;
			
			for (size_t c = 0; c < NbC; c++) {
				LhaCppFile << "    ConstraintsRelOp[" << i << "][" << c << "]=\"" << MyLha.ConstraintsRelOp[i][c] << "\";" << endl;
				LhaCppFile << "    ConstraintsConstants[" << i << "][" << c << "]=\"" << MyLha.ConstraintsConstants[i][c] << "\";" << endl;
				for (size_t v = 0; v < MyLha.NbVar; v++)
					LhaCppFile << "    ConstraintsCoeffs[" << i << "][" << c << "][" << v << "]=\"" << MyLha.ConstraintsCoeffs[i][c][v] << "\";" << endl;
			}
		}
    }
	
    LhaCppFile << "\tVars = new Variables;" << endl;
	LhaCppFile << "\ttempVars = new Variables;" << endl;
	LhaCppFile << "\tresetVariables();" << endl;
	
	if(P.StringInSpnLHA){
		LhaCppFile << "    vector<string> VarStr(NbVar);" << endl;
		LhaCppFile << "    VarLabel=VarStr;" << endl;
	}
	
    for (size_t v = 0; v < MyLha.NbVar; v++) {
		if(P.StringInSpnLHA){
			LhaCppFile << "    VarLabel[" << v << "]=\"" << MyLha.VarLabel[v] << "\";" << endl;
			LhaCppFile << "    VarIndex[\"" << MyLha.VarLabel[v] << "\"]=" << v << ";" << endl;
		}
	}
	
	if(P.StringInSpnLHA){
		LhaCppFile << "\n    vector<string> vStr(NbVar);" << endl;
		LhaCppFile << "    vector< vector <string > > vvStr(NbLoc,vStr);" << endl;
		LhaCppFile << "    StrFlow=vvStr;" << endl;
	}
    
	if(P.StringInSpnLHA){
		for (size_t l = 0; l < MyLha.NbLoc; l++)
			for (size_t v = 0; v < MyLha.NbVar; v++) {
				LhaCppFile << "    StrFlow[" << l << "][" << v << "]=\"" << MyLha.StrFlow[l][v] << "\";" << endl;
			}
	}
	
    LhaCppFile << "\n    vector< set < int > > vset(NbLoc);" << endl;
    LhaCppFile << "    Out_S_Edges =vset;" << endl;
    LhaCppFile << "    Out_A_Edges =vset;" << endl;
    for (size_t e = 0; e < MyLha.Edge.size(); e++) {
        if (MyLha.EdgeActions[e].size() < 1)
            LhaCppFile << "    Out_A_Edges[" << MyLha.Edge[e].Source << "].insert(" << e << ");" << endl;
        else
            LhaCppFile << "    Out_S_Edges[" << MyLha.Edge[e].Source << "].insert(" << e << ");" << endl;
    }
	
	if(P.StringInSpnLHA){
		LhaCppFile << "\n    vector< set <string> > vStrSet(" << MyLha.Edge.size() << ");" << endl;
		LhaCppFile << "    EdgeActions=vStrSet;" << endl;
	}
	
    LhaCppFile << "    vector< set<int> > vSetInt(" << MyLha.TransitionIndex.size() << ");" << endl;
    LhaCppFile << "    vector < vector < set <int> > > vvSetInt(NbLoc,vSetInt);" << endl;
	
    LhaCppFile << "    ActionEdges=vvSetInt;" << endl;
    for (size_t e = 0; e < MyLha.Edge.size(); e++) {
        for (set<string>::iterator it = MyLha.EdgeActions[e].begin(); it != MyLha.EdgeActions[e].end(); it++) {
            if(P.StringInSpnLHA)LhaCppFile << "    EdgeActions[" << e << "].insert(\"" << *it << "\");" << endl;
            LhaCppFile << "    ActionEdges[" << MyLha.Edge[e].Source << "][" << MyLha.TransitionIndex[*it] << "].insert(" << e << ");" << endl;
        }
    }
    LhaCppFile << "    if(true){" << endl;
    LhaCppFile << "    			vector<double> vL(" << MyLha.LinearForm.size() << ",0);" << endl;
    LhaCppFile << "    			LinForm=vL;" << endl;
    LhaCppFile << "    			OldLinForm=vL;" << endl;
    LhaCppFile << "    			vector<double> vF(" << MyLha.LhaFuncArg.size() << ",0);" << endl;
    LhaCppFile << "    			LhaFunc=vF;" << endl;
	if(P.CountTrans){
		LhaCppFile << "    			vector<double> vA(" << MyLha.Algebraic.size() + MyLha.Edge.size() << ",0);" << endl;
	} else {
		LhaCppFile << "    			vector<double> vA(" << MyLha.Algebraic.size() << ",0);" << endl;
    }
	
	LhaCppFile << "    			FormulaVal=vA;" << endl;
    LhaCppFile << "    }" << endl;
    LhaCppFile << "}\n" << endl;
	
	
	LhaCppFile << "void LHA_ORIG::DoElapsedTimeUpdate(double DeltaT,const abstractMarking& Marking) {\n";
    for (size_t v = 0; v < MyLha.NbVar ; v++) {
        LhaCppFile <<  "\tVars->"<< MyLha.VarLabel[v] << " += GetFlow("<<v<<", CurrentLocation, Marking) * DeltaT;\n";
    }
	LhaCppFile << "}\n";
	
	
    LhaCppFile << "double LHA::GetFlow(int v, int loc,const abstractMarking& Marking){" << endl;
    LhaCppFile << "    switch(v){" << endl;
    for (size_t x = 0; x < MyLha.NbVar; x++) {
		
        LhaCppFile << "    case " << x << ":" << endl;
        LhaCppFile << "         switch(loc){" << endl;
        for (size_t l = 0; l < MyLha.NbLoc; l++) {
            LhaCppFile << "         case " << l << ":" << endl;
            if (MyLha.FuncFlow[l][x] != "")
                LhaCppFile << "             return " << MyLha.FuncFlow[l][x] << ";" << endl;
            else
                LhaCppFile << "             return " << 0.0 << ";" << endl;
            LhaCppFile << "             break;" << endl;
        }
        LhaCppFile << "       }" << endl;
        LhaCppFile << "       break;" << endl;
    }
    LhaCppFile << "	}\n" << endl;
    LhaCppFile << "}\n" << endl;
	
    LhaCppFile << "bool LHA::CheckLocation(int loc,const abstractMarking& Marking){" << endl;
    LhaCppFile << "    switch(loc){" << endl;
    for (size_t l = 0; l < MyLha.NbLoc; l++) {
        LhaCppFile << "     case " << l << ":" << endl;
        LhaCppFile << "         return " << MyLha.FuncLocProperty[l] << ";" << endl;
        LhaCppFile << "         break;" << endl;
    }
    LhaCppFile << "    }" << endl;
	
	
    LhaCppFile << "}\n" << endl;
	
    LhaCppFile << "bool LHA::CheckEdgeContraints(int ed,const abstractBinding& b){" << endl;
    LhaCppFile << "    switch(ed){" << endl;
    for (size_t e = 0; e < MyLha.Edge.size(); e++) {
        LhaCppFile << "    case " << e << ": {" << endl;
        for (size_t c = 0; c < MyLha.ConstraintsRelOp[e].size(); c++) {
            LhaCppFile << "         if(!( ";
            for (size_t v = 0; v < MyLha.NbVar; v++) {
                if (MyLha.ConstraintsCoeffs[e][c][v] != "")
                    LhaCppFile << "+(" << MyLha.ConstraintsCoeffs[e][c][v] << ")*Vars->" << MyLha.VarLabel[v];
				
            }
            LhaCppFile << MyLha.ConstraintsRelOp[e][c] << MyLha.ConstraintsConstants[e][c] << ")) return false;" << endl;
			
			
        }
        LhaCppFile << "         return true; " << endl;
        LhaCppFile << "         break;" << endl;
        LhaCppFile << "     }" << endl;
    }
    LhaCppFile << "    }" << endl;
	
	
    LhaCppFile << "}\n" << endl;
	
	
    LhaCppFile << "t_interval LHA::GetEdgeEnablingTime(int ed,const abstractMarking& Marking){" << endl;
    LhaCppFile << "    switch(ed){" << endl;
    for (size_t e = 0; e < MyLha.Edge.size(); e++) {
        LhaCppFile << "     case " << e << ":" << endl;
		
        //LhaCppFile << "         return GetEdgeEnablingTime_" << e << "( Marking);" << endl;
		LhaCppFile << "         {" << endl;
		
		LhaCppFile << "             t_interval EnablingT;\n" << endl;
        LhaCppFile << "             EnablingT.first=CurrentTime;" << endl;
        LhaCppFile << "             EnablingT.second=DBL_MAX;\n" << endl;
        LhaCppFile << "             t_interval EmptyInterval;\n" << endl;
        LhaCppFile << "             EmptyInterval.first=0;" << endl;
        LhaCppFile << "             EmptyInterval.second=-1;\n" << endl;
		
        LhaCppFile << "             double SumAF;" << endl;
        LhaCppFile << "             double SumAX;" << endl;
		
		
		
        LhaCppFile << "\n" << endl;
		
		
        for (size_t c = 0; c < MyLha.ConstraintsRelOp[e].size(); c++) {
			
            LhaCppFile << "             SumAF=";
            for (size_t v = 0; v < MyLha.NbVar; v++)
                if (MyLha.ConstraintsCoeffs[e][c][v] != "")
                    LhaCppFile << "+(" << MyLha.ConstraintsCoeffs[e][c][v] << ")*GetFlow(" << v << "," << MyLha.Edge[e].Source << ", Marking)";
            LhaCppFile << ";\n             SumAX=";
            for (size_t v = 0; v < MyLha.NbVar; v++)
                if (MyLha.ConstraintsCoeffs[e][c][v] != "")
                    LhaCppFile << "+(" << MyLha.ConstraintsCoeffs[e][c][v] << ")*Vars->" << MyLha.VarLabel[v];
            LhaCppFile << ";\n" << endl;
			
            string RelOp = MyLha.ConstraintsRelOp[e][c];
			
            LhaCppFile << "             if(SumAF==0){" << endl;
            LhaCppFile << "                  if(!(SumAX" << MyLha.ConstraintsRelOp[e][c] << MyLha.ConstraintsConstants[e][c] << "))" << endl;
            LhaCppFile << "                      return EmptyInterval;" << endl;
            LhaCppFile << "             }" << endl;
            LhaCppFile << "             else{" << endl;
            LhaCppFile << "                  double t=CurrentTime+(" << MyLha.ConstraintsConstants[e][c] << "-SumAX)/(double)SumAF;" << endl;
            if (RelOp == "==") {
				
                LhaCppFile << "                  if(t>=EnablingT.first && t<=EnablingT.second){" << endl;
                LhaCppFile << "                      EnablingT.first=t; EnablingT.second=t;" << endl;
                LhaCppFile << "                  }" << endl;
                LhaCppFile << "                  else return EmptyInterval;" << endl;
				
            } else {
                LhaCppFile << "                  if(SumAF>0){" << endl;
                if (RelOp == "<=") {
                    LhaCppFile << "                     if(EnablingT.second>t) EnablingT.second=t;" << endl;
                    LhaCppFile << "                     if(EnablingT.second<EnablingT.first) return EmptyInterval;" << endl;
					
                } else {
                    LhaCppFile << "                     if(EnablingT.first<t) EnablingT.first=t;" << endl;
                    LhaCppFile << "                     if(EnablingT.second<EnablingT.first) return EmptyInterval;" << endl;
                }
				
                LhaCppFile << "                      }" << endl;
				
                LhaCppFile << "                  else{" << endl;
                RelOp = InvRelOp(RelOp);
                if (RelOp == "<=") {
                    LhaCppFile << "                     if(EnablingT.second>t) EnablingT.second=t;" << endl;
                    LhaCppFile << "                     if(EnablingT.second<EnablingT.first) return EmptyInterval;" << endl;
					
                } else {
                    LhaCppFile << "                     if(EnablingT.first<t) EnablingT.first=t;" << endl;
                    LhaCppFile << "                     if(EnablingT.second<EnablingT.first) return EmptyInterval;" << endl;
                }
				
                LhaCppFile << "                      }" << endl;
            }
            LhaCppFile << "             }" << endl;
			
        }
		
        LhaCppFile << "             return EnablingT;" << endl;
		LhaCppFile << "         }"<< endl;
		
		
		//LhaCppFile << "         }" << endl;
        //LhaCppFile << "         break;" << endl;
		
	}
    
	LhaCppFile << "    }" << endl;
	
    LhaCppFile << "}\n" << endl;
	
	
	
    LhaCppFile << "void LHA::DoEdgeUpdates(int ed,const abstractMarking& Marking){" << endl;
    LhaCppFile << "    switch(ed){" << endl;
    for (size_t e = 0; e < MyLha.Edge.size(); e++) {
        LhaCppFile << "     case " << e << ":" << endl;
		LhaCppFile << "         {"<< endl;
		
		int k = 0;
		if(P.CountTrans)
			LhaCppFile << "         EdgeCounter[" << e << "]++ ;"<< endl;
        for (size_t v = 0; v < MyLha.NbVar; v++)
            if (MyLha.FuncEdgeUpdates[e][v] != "") {
                k++;
                break;
            }
        if (k > 0) {
            for (size_t v = 0; v < MyLha.NbVar; v++)
                if (MyLha.FuncEdgeUpdates[e][v] != ""){
                    LhaCppFile << "         tempVars->" << MyLha.VarLabel[v] << "=" << MyLha.FuncEdgeUpdates[e][v] << ";" << endl;
				}else{
					LhaCppFile << "         tempVars->" << MyLha.VarLabel[v] << "=Vars->" << MyLha.VarLabel[v] << ";" << endl;
				}
			LhaCppFile << "\tstd::swap(Vars,tempVars);\n";
			
		}
		LhaCppFile << endl;
		for (map<string, int>::iterator it = MyLha.LinearForm.begin(); it != MyLha.LinearForm.end(); it++) {
			LhaCppFile << "         OldLinForm[" << (*it).second << "]=LinForm[" << (*it).second << "];" << endl;
			
		}
		
		//LhaCppFile << "         DoEdgeUpdates_" << e << "( Marking);" << endl;
		LhaCppFile << "         }"<< endl;
		LhaCppFile << "         break;" << endl;
	}
	LhaCppFile << "    }" << endl;
	
	
	LhaCppFile << "}\n" << endl;
	
	
	
	/*for (int e = 0; e < MyLha.Edge.size(); e++) {
	 LhaCppFile << "void LHA::DoEdgeUpdates_" << e << "(abstractMarking& Marking){" << endl;
	 
	 LhaCppFile << "    }\n" << endl;
	 }*/
	
	LhaCppFile << "void LHA::UpdateLinForm(const abstractMarking& Marking){" << endl;
	for (map<string, int>::iterator it = MyLha.LinearForm.begin(); it != MyLha.LinearForm.end(); it++) {
		LhaCppFile << "    LinForm[" << (*it).second << "]=" << (*it).first << ";" << endl;
		
	}
	LhaCppFile << "    }\n" << endl;
	
	LhaCppFile << "void LHA::UpdateLhaFunc(double& Delta ){" << endl;
	for (size_t i = 0; i < MyLha.LhaFuncArg.size(); i++) {
		/*if (MyLha.LhaFuncType[i] == "Last")
		 LhaCppFile << "    LhaFunc[" << i << "]=LinForm[" << MyLha.LhaFuncArg[i] << "];" << endl;
		 else {*/
		if (MyLha.LhaFuncType[i] == "Min")
			LhaCppFile << "    LhaFunc[" << i << "]=Min(LhaFunc[" << i << "],LinForm[" << MyLha.LhaFuncArg[i] << "],OldLinForm[" << MyLha.LhaFuncArg[i] << "]);" << endl;
		else {
			if (MyLha.LhaFuncType[i] == "Max")
				LhaCppFile << "    LhaFunc[" << i << "]=Max(LhaFunc[" << i << "],LinForm[" << MyLha.LhaFuncArg[i] << "],OldLinForm[" << MyLha.LhaFuncArg[i] << "]);" << endl;
			else {
				if (MyLha.LhaFuncType[i] == "Integral")
					LhaCppFile << "    LhaFunc[" << i << "]=Integral(LhaFunc[" << i << "], CurrentTime, Delta, OldLinForm[" << MyLha.LhaFuncArg[i] << "],LinForm[" << MyLha.LhaFuncArg[i] << "]);" << endl;
				
			}
		}
		//}
		
	}
	LhaCppFile << "\n    }\n" << endl;
	
	LhaCppFile << "void LHA::UpdateLhaFuncLast(){" << endl;
	for (size_t i = 0; i < MyLha.LhaFuncArg.size(); i++) {
		if (MyLha.LhaFuncType[i] == "Last")
			LhaCppFile << "    LhaFunc[" << i << "]=LinForm[" << MyLha.LhaFuncArg[i] << "];" << endl;
	}
	LhaCppFile << "\n    }\n" << endl;
	
	
	
	LhaCppFile << "void LHA::UpdateFormulaVal(){\n" << endl;
	for(size_t i=0;i<MyLha.Algebraic.size();i++){
		LhaCppFile << "    OldFormulaVal=FormulaVal["<<i<<"];" << endl;
		LhaCppFile << "    FormulaVal["<<i<<"]=" << MyLha.Algebraic[i] << ";\n" << endl;
	}
	if (P.CountTrans) {
		for(size_t i=0;i<MyLha.Edge.size();i++){
			size_t j = i+ MyLha.Algebraic.size();
			LhaCppFile << "    FormulaVal["<<j<<"]= EdgeCounter[" << i << "];\n";
		}
	}
	LhaCppFile << "}\n" << endl;
	
	
	LhaCppFile.close();
	
	
}
