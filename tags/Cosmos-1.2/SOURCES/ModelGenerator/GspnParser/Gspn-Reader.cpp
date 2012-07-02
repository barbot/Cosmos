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

#include "lex.gspn.c"

#include "Gspn-parser.tab.hh"
#include "Gspn-Reader.hpp"
#include "Gspn_gmlparser.hpp"

#include "string.h"

#include <string>
#include <sstream>
#include <fstream>
#include <set>




using namespace std;

Gspn_Reader::Gspn_Reader() {
    trace_scanning = false;
    trace_parsing = false;

}

Gspn_Reader::~Gspn_Reader() {
}

int Gspn_Reader::parse(string& expr) {




    IndexDist["UNIFORM"] = 1;
    IndexDist["EXPONENTIAL"] = 2;
    IndexDist["DETERMINISTIC"] = 3;
    IndexDist["LOGNORMAL"] = 4;
    IndexDist["TRIANGLE"] = 5;
    IndexDist["GEOMETRIC"] = 6;

    scan_expression(expr);

    gspn::Gspn_parser parser(*this);

    parser.set_debug_level(trace_parsing);

    int res = parser.parse();
    scan_end();
    return res;
}

int Gspn_Reader::parse_file(string &filename) {
    string str;

    ifstream file(filename.c_str(), ios::in);
    if (file) {

        while (!file.eof()) {

            string str2;
            getline(file, str2);
            str = str + "\n" + str2;
        }
        file.close();
        /*filename.append(".mod");
        ifstream Modfile(filename.c_str(), ios::in);
        if (Modfile) {

            while (!Modfile.eof()) {

                string str2;
                getline(Modfile, str2);
                str = str + "\n" + str2;
            }

        }*/
        int x = parse(str);

        if (x) cout << "Parsing GSPN Description file failed" << endl;

        return x;
    } else {
        cout << "Can't open : " << filename << endl;
        return 1;
    }
}

//#include "Gspn_gmlparser.cpp";
int Gspn_Reader::parse_gml_file(string &filename,bool re) {
    ifstream ifile(filename.c_str());
    if(ifile){
	//cout << "parse GML:" << filename << endl;
        ModelHandlerPtr handlerPtr(new MyModelHandler(&MyGspn,re));
    ExpatModelParser parser = ExpatModelParser(handlerPtr);
    parser.parse_file(filename);
	//cout << "end parse GML:"<< MyGspn.pl << endl;
        if (re)addSinkTrans();
	return 0;
    }else{
        cout << "File " << filename << " does not exist!" << endl;
        exit(EXIT_FAILURE);
    }
    
}

void
Gspn_Reader::error(const gspn::location& l, const std::string& m) {
    std::cerr << l << ": " << m << std::endl;
}

void
Gspn_Reader::error(const std::string& m) {
    std::cerr << m << std::endl;
}

/*void Gspn_Reader::addSink(){
    //Add a place
    MyGspn.Marking.push_back(0);
    string Plname = "Puit";
    MyGspn.PlacesList.insert(Plname);
    MyGspn.PlacesId[Plname]=MyGspn.pl;
    MyGspn.pl++;
    
    //Add a transition
    string Trname = "Puittrans";
    MyGspn.TransList.insert(Trname);
    MyGspn.TransId[Trname]=MyGspn.tr;
    ProbabiliteDistribution& dist = *(new ProbabiliteDistribution);
    dist.name = "EXPONENTIAL";
    dist.Param.push_back("0");
    MyGspn.Dist.push_back(dist);
    MyGspn.tType.push_back(Timed);
    MyGspn.Priority.push_back("1");
    MyGspn.Weight.push_back("1");
    MyGspn.SingleService.push_back(true);
    MyGspn.MarkingDependent.push_back(true);
    MyGspn.NbServers.push_back(1);
    MyGspn.tr++;
}*/

void Gspn_Reader::addSinkTrans(){
    (MyGspn.outArcs)[MyGspn.tr-1][MyGspn.pl-1]=1;
}

void Gspn_Reader::WriteFile(string& Pref) {


    string loc;

    loc = Pref + "../SOURCES/Cosmos/spn.cpp";
    //loc= "/Users/barbot/Documents/Cosmos/SOURCES/Cosmos/spn.cpp";
	ofstream SpnCppFile(loc.c_str(), ios::out | ios::trunc); // ouverture en écriture avec effacement du SpnCppFile ouvert
	//cout << loc << endl;

    /*loc = Pref + "../SOURCES/ModelGenerator/spn_orig.cpp";
    //loc = "/Users/barbot/Documents/Cosmos/SOURCES/ModelGenerator/spn_orig.cpp";
	ifstream SpnCppFile_orig(loc.c_str(), ios::in);

	//cout << "Generating " << loc << endl;

    while (!SpnCppFile_orig.eof()) {
        string LineFile;
        getline(SpnCppFile_orig, LineFile);
        SpnCppFile << LineFile << endl;
    }
    SpnCppFile_orig.close();*/
	
	SpnCppFile << "#include \"spn_orig.hpp\"" << endl;
	
	SpnCppFile << "#ifndef _SPN_HPP" << endl;
	SpnCppFile << "#define	_SPN_HPP" << endl;
	
	SpnCppFile << "class SPN: " << endl << "public SPN_ORIG {" << endl;
	SpnCppFile << "public:"<< endl;
	SpnCppFile << "    void Load();" <<endl;
	SpnCppFile << "    bool IsEnabled(int);" << endl;
	SpnCppFile << "    void fire(int);" << endl; 
	SpnCppFile << "    void unfire(int);" << endl;
	SpnCppFile << "    vector<double> GetDistParameters(int);" << endl;
	//SpnCppFile << "    vector<double> GetDistParametersOrigin(int);" << endl;
	SpnCppFile << "    double GetPriority(int);" << endl;
	SpnCppFile << "    double GetWeight(int);" << endl;
    SpnCppFile << "    void lumpingFun(vector<int>*);" << endl;
	
	
	
	SpnCppFile << "private:" << endl;
	
    for (int t = 0; t < MyGspn.tr; t++) {
		
		SpnCppFile << "    void fire_t" << t << "();" << endl;
		SpnCppFile << "    void unfire_t" << t << "();" << endl;
        SpnCppFile << "    bool IsEnabled_t" << t << "();" << endl;
    }
	
    SpnCppFile << "};" << endl;
	
	
	SpnCppFile << "#endif	/* _SPN_HPP */" << endl;
	
	
	//SpnCppFile << "#include \"RareEvent.hpp\"" << endl;
	
    /*for (map<string,int>::iterator it= MyGspn.IntConstant.begin(); it!= MyGspn.IntConstant.end() ; it++) {
		SpnCppFile << "    const int " << it->first << "=" << it->second << ";" << endl;
		}*/
	for (map<string,double>::iterator it= MyGspn.RealConstant.begin(); it!= MyGspn.RealConstant.end() ; it++) {
		SpnCppFile << "    const double " << it->first << "=" << it->second << ";" << endl;
	}
	for (set<string>::iterator it = MyGspn.PlacesList.begin(); it != MyGspn.PlacesList.end(); it++) {
        int k = MyGspn.PlacesId[*it];
		SpnCppFile << "    const int _nb_Place_"<< *it << "=" << k << ";" << endl;
    }
    
    SpnCppFile << "#include \"lumpingfun.cpp\"" << endl;

    SpnCppFile << "void SPN::Load(){" << endl;
    SpnCppFile << "    Path =\"" << MyGspn.Path << "\";" << endl;

    //-------------- Rare Event -----------------
    //SpnCppFile << "    gammaprob.load();" << endl;
    //------------- /Rare Event -----------------

    SpnCppFile << "    pl =" << MyGspn.pl << ";" << endl;

    SpnCppFile << "    tr =" << MyGspn.tr << ";" << endl;

    SpnCppFile << "    IntVector Null_Pl_Vector(pl, 0);" << endl;


    SpnCppFile << "    IntMatrix Null_PlxTr_Matrix(tr, Null_Pl_Vector);" << endl;


    SpnCppFile << "    inArcs = Null_PlxTr_Matrix;" << endl;
    SpnCppFile << "    outArcs = Null_PlxTr_Matrix;" << endl;
    SpnCppFile << "    inhibArcs = Null_PlxTr_Matrix;" << endl;
    SpnCppFile << "    Marking = Null_Pl_Vector;;" << endl;
    for (int t = 0; t < MyGspn.tr; t++) {
        for (int p = 0; p < MyGspn.pl; p++)

            if (MyGspn.inArcs[t][p] > 0)
                SpnCppFile << "    inArcs[" << t << "][" << p << "]=" << MyGspn.inArcs[t][p] << ";" << endl;
        SpnCppFile << endl;

    }

    for (int t = 0; t < MyGspn.tr; t++) {
        for (int p = 0; p < MyGspn.pl; p++)
            if (MyGspn.outArcs[t][p] > 0)
                SpnCppFile << "    outArcs[" << t << "][" << p << "]=" << MyGspn.outArcs[t][p] << ";" << endl;
        SpnCppFile << endl;

    }

    for (int t = 0; t < MyGspn.tr; t++) {
        for (int p = 0; p < MyGspn.pl; p++)

            if (MyGspn.inhibArcs[t][p] > 0)
                SpnCppFile << "    inhibArcs[" << t << "][" << p << "]=" << MyGspn.inhibArcs[t][p] << ";" << endl;
        SpnCppFile << endl;

    }

    for (int p = 0; p < MyGspn.pl; p++) {
        if (MyGspn.Marking[p] > 0)
            SpnCppFile << "    Marking[" << p << "]=" << MyGspn.Marking[p] << ";" << endl;
    }
    SpnCppFile << "\n   initMarking = Marking;\n" << endl;




    SpnCppFile << "    vector <spn_trans> t(" << MyGspn.tr << ");" << endl;

    SpnCppFile << "    Transition = t;" << endl;


    SpnCppFile << "    vector <spn_place> p(" << MyGspn.pl << ");" << endl;
    SpnCppFile << "    Place = p;" << endl;


    for (set<string>::iterator it = MyGspn.PlacesList.begin(); it != MyGspn.PlacesList.end(); it++) {
        int k = MyGspn.PlacesId[*it];
        SpnCppFile << "    Place[" << k << "].Id =" << k << ";" << endl;
        SpnCppFile << "    Place[" << k << "].label =\" " << *it << "\";" << endl;
        SpnCppFile << "    PlaceIndex[\" " << *it << "\"] =" << k << ";" << endl;

    }

    for (set<string>::iterator it = MyGspn.TransList.begin(); it != MyGspn.TransList.end(); it++) {
        int k = MyGspn.TransId[*it];
        SpnCppFile << "    Transition[" << k << "].Id =" << k << ";" << endl;
        SpnCppFile << "    Transition[" << k << "].label =\"" << *it << "\";" << endl;
        SpnCppFile << "    TransitionIndex[\"" << *it << "\"]=" << k << ";" << endl;

    }




    for (int i = 0; i < MyGspn.tr; i++) {

        if (MyGspn.tType[i] == Timed) {
            SpnCppFile << "    Transition[" << i << "].transType = Timed;" << endl;
            SpnCppFile << "    Transition[" << i << "].DistType = \"" << MyGspn.Dist[i].name << "\";" << endl;

            for (int j = 0; j < MyGspn.Dist[i].Param.size(); j++) {
                SpnCppFile << "    Transition[" << i << "].DistParams.push_back(\" " << MyGspn.Dist[i].Param[j] << "\" );" << endl;
            }

        } else {
            SpnCppFile << "    Transition[" << i << "].transType = unTimed;" << endl;


        }
        SpnCppFile << "    Transition[" << i << "].priority = \"" << MyGspn.Priority[i] << "\";" << endl;
        SpnCppFile << "    Transition[" << i << "].MarkingDependent = " << MyGspn.MarkingDependent[i] << ";" << endl;
        SpnCppFile << "    Transition[" << i << "].weight = \"" << MyGspn.Weight[i] << " \";" << endl;
        SpnCppFile << endl;
    }

    SpnCppFile << "    vector < vector <int> > vec(" << MyGspn.tr << ");" << endl;
    SpnCppFile << "    PredT = vec;" << endl;
    for (int t = 0; t < MyGspn.tr; t++) {
        for (int p = 0; p < MyGspn.pl; p++)
            if (MyGspn.inArcs[t][p] > 0) SpnCppFile << "    PredT[" << t << "].push_back(" << p << ");" << endl;

        //SpnCppFile << endl;

    }
    SpnCppFile << "    EnabledDisabledTr();" << endl;

    //-------------- Rare Event -----------------
    SpnCppFile << "    Msimple();" << endl;
    SpnCppFile << "vector <double> Rate_Table_init (tr);" << endl;
    SpnCppFile << "Rate_Table = Rate_Table_init;" << endl;
    SpnCppFile << "Origine_Rate_Table = Rate_Table_init;" << endl;
    /*for(int i =0;i<MyGspn.tr;i++){
      SpnCppFile << "Origine_Rate_Table["<<i<<"]= ( double ) " <<  MyGspn.Dist[i].Param[0] << ";" << endl;
      };*/
    
    //------------- /Rare Event ----------------- 

    SpnCppFile << "}\n" << endl;
    





    SpnCppFile << "bool SPN::IsEnabled(int t){" << endl;
    SpnCppFile << "    switch(t){" << endl;
    for (int i = 0; i < MyGspn.tr; i++) {
        SpnCppFile << "     case " << i << ":" << endl;
        SpnCppFile << "       return IsEnabled_t" << i << "();" << endl;
        SpnCppFile << "       break;" << endl;
    }


    SpnCppFile << "   }" << endl;
    SpnCppFile << "}\n" << endl;

    for (int t = 0; t < MyGspn.tr; t++) {
        SpnCppFile << "bool SPN::IsEnabled_t" << t << "() {" << endl;
        for (int p = 0; p < MyGspn.pl; p++) {
            if (MyGspn.inArcs[t][p] > 0) {

                if (MyGspn.inArcsStr[t][p] == " ")
                    SpnCppFile << "    if (Marking[" << p << "] < " << MyGspn.inArcs[t][p] << ") return false;" << endl;

                else {
                    SpnCppFile << "    if ( " << MyGspn.inArcsStr[t][p] << "> 0) " << endl;
                    SpnCppFile << "        if (Marking[" << p << "] < " << MyGspn.inArcsStr[t][p] << ") return false;" << endl;
                }
            }
            if (MyGspn.inhibArcs[t][p] > 0) {

                if (MyGspn.inhibArcsStr[t][p] == " ")
                    SpnCppFile << "    if (Marking[" << p << "] >= " << MyGspn.inhibArcs[t][p] << ") return false;" << endl;

                else {
                    SpnCppFile << "    if ( " << MyGspn.inhibArcsStr[t][p] << " > 0 ) " << endl;
                    SpnCppFile << "        if (Marking[" << p << "] >= " << MyGspn.inhibArcsStr[t][p] << ") return false;" << endl;

                }
            }
        }
        SpnCppFile << "    return true;" << endl;
        SpnCppFile << "}\n" << endl;

    }



    SpnCppFile << "void SPN::fire(int t){" << endl;
    SpnCppFile << "   switch(t){" << endl;
    for (int t = 0; t < MyGspn.tr; t++) {
        SpnCppFile << "     case " << t << ": {" << endl;
        SpnCppFile << "       fire_t" << t << "();" << endl;
        SpnCppFile << "       break;" << endl;
        SpnCppFile << "     } " << endl;
    }
    SpnCppFile << "   }" << endl;
    SpnCppFile << "}\n" << endl;

    SpnCppFile << "void SPN::unfire(int t){" << endl;
    SpnCppFile << "   switch(t){" << endl;
    for (int t = 0; t < MyGspn.tr; t++) {
        SpnCppFile << "     case " << t << ": {" << endl;
        SpnCppFile << "       unfire_t" << t << "();" << endl;
        SpnCppFile << "       break;" << endl;
        SpnCppFile << "     } " << endl;
    }

    SpnCppFile << "   }" << endl;
    SpnCppFile << "}\n" << endl;

    for (int t = 0; t < MyGspn.tr; t++) {
      SpnCppFile << "void SPN::fire_t" << t << "() {" << endl;
      for (int p = 0; p < MyGspn.pl; p++) {
	if (MyGspn.inArcs[t][p] > 0) {
	  if (MyGspn.inArcsStr[t][p] == " ")
	    SpnCppFile << "    Marking[" << p << "] -= " << MyGspn.inArcs[t][p] << ";" << endl;
	  else
	    SpnCppFile << "    Marking[" << p << "] -= " << MyGspn.inArcsStr[t][p] << ";" << endl;
	}
	
	if (MyGspn.outArcs[t][p] > 0) {
	  if (MyGspn.outArcsStr[t][p] == " ")
	    SpnCppFile << "    Marking[" << p << "] += " << MyGspn.outArcs[t][p] << ";" << endl;
	  else
	    SpnCppFile << "    Marking[" << p << "] += " << MyGspn.outArcsStr[t][p] << ";" << endl;
	}
      }
      SpnCppFile << "}\n" << endl;
      
      SpnCppFile << "void SPN::unfire_t" << t << "() {" << endl;
      for (int p = 0; p < MyGspn.pl; p++) {
	if (MyGspn.inArcs[t][p] > 0) {
	  if (MyGspn.inArcsStr[t][p] == " ")
	    SpnCppFile << "    Marking[" << p << "] += " << MyGspn.inArcs[t][p] << ";" << endl;
	  else
	    SpnCppFile << "    Marking[" << p << "] += " << MyGspn.inArcsStr[t][p] << ";" << endl;
	}
	
	if (MyGspn.outArcs[t][p] > 0) {
	  if (MyGspn.outArcsStr[t][p] == " ")
	    SpnCppFile << "    Marking[" << p << "] -= " << MyGspn.outArcs[t][p] << ";" << endl;
	  else
	    SpnCppFile << "    Marking[" << p << "] -= " << MyGspn.outArcsStr[t][p] << ";" << endl;
	}
      }
      SpnCppFile << "}\n" << endl;

    }
    
    //-------------- Rare Event -------------------------
    /*SpnCppFile << "vector<double> SPN::GetDistParameters(int t){"<< endl;
    SpnCppFile << "   vector<double> P(2);" << endl;
    SpnCppFile << "   double origin_rate = (SPN::GetDistParametersOrigin(t))[0];"<< endl;
    SpnCppFile << "   P[0]= ComputeDistr( *this ,t, origin_rate);" << endl;
    SpnCppFile << "   P[1]= origin_rate;" << endl;

    SpnCppFile << "   return P;" << endl;
    SpnCppFile << "}\n " << endl;*/
    
    
    SpnCppFile << "vector<double> SPN::GetDistParameters(int t){" << endl;
    //-------------- /Rare Event -------------------------
    SpnCppFile << "   switch(t){" << endl;
    for (int t = 0; t < MyGspn.tr; t++) {
      if (MyGspn.tType[t] == Timed) {
	SpnCppFile << "     case " << t << ": {" << endl;
	SpnCppFile << "       vector<double> P(" << MyGspn.Dist[t].Param.size() << ");" << endl;
	if (MyGspn.SingleService[t])
	  for (int i = 0; i < MyGspn.Dist[t].Param.size(); i++) {
	    
	    SpnCppFile << "       P[" << i << "]= ( double ) " << MyGspn.Dist[t].Param[i] << ";" << endl;
	  } else {
	  SpnCppFile << "       double EnablingDegree = INT_MAX; " << endl;
	  bool AtLeastOneMarkDepArc = false;
	  for (int p = 0; p < MyGspn.pl; p++)
	    if (MyGspn.inArcs[t][p] > 0) {
	      if (MyGspn.inArcsStr[t][p] == " ")
		SpnCppFile << "       EnablingDegree=min(floor(Marking[" << p << "]/(double)(" << MyGspn.inArcs[t][p] << ")),EnablingDegree);" << endl;
	      else {
		AtLeastOneMarkDepArc = true;
		SpnCppFile << "       if(" << MyGspn.inArcsStr[t][p] << ">0)" << endl;
		SpnCppFile << "              EnablingDegree=min(floor(Marking[" << p << "]/(double)(" << MyGspn.inArcsStr[t][p] << ")),EnablingDegree);" << endl;
	      }
	      
	    }
	  if (AtLeastOneMarkDepArc) {
	    if (MyGspn.NbServers[t] >= INT_MAX) {
	      SpnCppFile << "       if(EnablingDegree < INT_MAX ) P[0] = EnablingDegree * ( " << MyGspn.Dist[t].Param[0] << " );" << endl;
	      SpnCppFile << "       else P[0] = " << MyGspn.Dist[t].Param[0] << " ;" << endl;
	    } else {
	      SpnCppFile << "       if(EnablingDegree < INT_MAX ) P[0] = min(EnablingDegree , " << MyGspn.NbServers[t] << " ) * ( " << MyGspn.Dist[t].Param[0] << " );" << endl;
	      SpnCppFile << "       else P[0] = " << MyGspn.Dist[t].Param[0] << " ;" << endl;
	    }
	  } else {
	    if (MyGspn.NbServers[t] >= INT_MAX)
	      SpnCppFile << "        P[0] = EnablingDegree  * ( " << MyGspn.Dist[t].Param[0] << " );" << endl;
	    else
	      SpnCppFile << "        P[0] = min(EnablingDegree , " << MyGspn.NbServers[t] << " ) * ( " << MyGspn.Dist[t].Param[0] << " );" << endl;
	  }
	}
	SpnCppFile << "       return P;" << endl;
	SpnCppFile << "       break;" << endl;
	SpnCppFile << "     }" << endl;
      }
    }
    SpnCppFile << "   }" << endl;
    SpnCppFile << "}\n" << endl;


/////////////////////////////////////////

SpnCppFile << "double SPN::GetPriority(int t){" << endl;
SpnCppFile << "   switch(t){" << endl;
for (int t = 0; t < MyGspn.tr; t++) {
  
  SpnCppFile << "     case " << t << ": {" << endl;
  
  SpnCppFile << "       return (double)" << MyGspn.Priority[t] << ";" << endl;
  SpnCppFile << "       break;" << endl;
  SpnCppFile << "     } " << endl;
  
 }
SpnCppFile << "   }" << endl;
SpnCppFile << "}\n" << endl;


/////////////////////////////////////////
SpnCppFile << "double SPN::GetWeight(int t){" << endl;
SpnCppFile << "   switch(t){" << endl;
for (int t = 0; t < MyGspn.tr; t++) {
  
        SpnCppFile << "     case " << t << ":" << endl;

        SpnCppFile << "       return (double)" << MyGspn.Weight[t] << ";" << endl;
        SpnCppFile << "       break;" << endl;

    }


    SpnCppFile << "   }" << endl;
    SpnCppFile << "}\n" << endl;


    SpnCppFile.close();

	/*
    loc = Pref + "../SOURCES/Cosmos/spn.hpp";
    ofstream SpnHppFile(loc.c_str(), ios::out | ios::trunc);
*/
    /*loc = Pref + "../SOURCES/ModelGenerator/spn_orig.hpp";
    ifstream SpnHppFile_orig(loc.c_str(), ios::in);

	//cout << "generating hpp"<< endl;


    while (!SpnHppFile_orig.eof()) {
        string LineFile;
        getline(SpnHppFile_orig, LineFile);
        SpnHppFile << LineFile << endl;
    }

    SpnHppFile_orig.close();*/
	
	/*
	SpnHppFile << "#ifndef _SPN_HPP" << endl;
	SpnHppFile << "#define	_SPN_HPP" << endl;
	
	SpnHppFile << "#include \"spn_orig.hpp\"" << endl;

	SpnHppFile << "class SPN: " << endl << "public SPN_ORIG {" << endl;

	
	SpnHppFile << "private:" << endl;

    for (int t = 0; t < MyGspn.tr; t++) {

		SpnHppFile << "    void fire_t" << t << "();" << endl;
		SpnHppFile << "    void unfire_t" << t << "();" << endl;
        SpnHppFile << "    bool IsEnabled_t" << t << "();" << endl;
    }

    SpnHppFile << "};" << endl;


*/
    //SpnHppFile << "#endif	/* _SPN_HPP */" << endl;

    //SpnHppFile.close();



}
