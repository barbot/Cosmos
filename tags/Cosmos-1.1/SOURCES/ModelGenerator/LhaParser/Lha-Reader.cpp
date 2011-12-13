#include "lex.lha.c"

#include "Lha-parser.tab.hh"
#include "Lha-Reader.hpp"

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

    scan_expression(expr);

    lha::Lha_parser parser(*this);

    parser.set_debug_level(trace_parsing);

    int res = parser.parse();
    scan_end();
    return res;
}

int Lha_Reader::parse_file(string &filename) {
    string str;

    ifstream file(filename.c_str(), ios::in);
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
        cout << "Can't open : " << filename << endl;
        return 1;
    }
}

string Lha_Reader::InvRelOp(string& str) {
    if (str == "<=") return ">=";
    if (str == ">=") return "<=";
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

void Lha_Reader::WriteFile(string& Pref) {

    string loc;

    loc = Pref + "../SOURCES/Cosmos/LHA.cpp";
    ofstream LhaCppFile(loc.c_str(), ios::out | ios::trunc);

    loc = Pref + "../SOURCES/ModelGenerator/LHA_orig.cpp";
    ifstream LhaCppFile_orig(loc.c_str(), ios::in);

    while (!LhaCppFile_orig.eof()) {

        string LineFile;
        getline(LhaCppFile_orig, LineFile);
        LhaCppFile << LineFile << endl;
    }
    LhaCppFile_orig.close();


    LhaCppFile << "void LHA::Load(){" << endl;


    LhaCppFile << "    NbLoc =" << MyLha.NbLoc << ";" << endl;
    LhaCppFile << "    NbVar =" << MyLha.NbVar << ";" << endl;
    for (set<unsigned int>::iterator it = MyLha.InitLoc.begin(); it != MyLha.InitLoc.end(); it++)
        LhaCppFile << "    InitLoc.insert(" << (*it) << ");" << endl;
    for (set<unsigned int>::iterator it = MyLha.FinalLoc.begin(); it != MyLha.FinalLoc.end(); it++)
        LhaCppFile << "    FinalLoc.insert(" << (*it) << ");" << endl;

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


    LhaCppFile << "    vector<LhaEdge> ve(" << MyLha.Edge.size() << ");" << endl;
    LhaCppFile << "    Edge= ve;\n" << endl;
    for (int i = 0; i < MyLha.LocLabel.size(); i++)
        LhaCppFile << "    LocLabel[" << i << "]=\"" << MyLha.LocLabel[i] << "\";" << endl;

    for (int i = 0; i < MyLha.StrLocProperty.size(); i++)
        LhaCppFile << "    StrLocProperty[" << i << "]=\"" << MyLha.StrLocProperty[i] << "\";" << endl;

    for (map <string, int>::iterator it = MyLha.EdgeIndex.begin(); it != MyLha.EdgeIndex.end(); it++)
        LhaCppFile << "    EdgeIndex[\"" << (*it).first << "\"]=" << (*it).second << ";" << endl;

    for (int i = 0; i < MyLha.Edge.size(); i++) {
        LhaCppFile << "    Edge[" << i << "].Index=" << MyLha.Edge[i].Index << ";" << endl;
        LhaCppFile << "    Edge[" << i << "].Source=" << MyLha.Edge[i].Source << ";" << endl;
        LhaCppFile << "    Edge[" << i << "].Target=" << MyLha.Edge[i].Target << ";" << endl;
        if (MyLha.EdgeActions[i].size() < 1)
            LhaCppFile << "    Edge[" << i << "].Type= Auto ;" << endl;
        else
            LhaCppFile << "    Edge[" << i << "].Type= Synch;" << endl;

    }

    for (int i = 0; i < MyLha.Edge.size(); i++) {

        int NbC = MyLha.ConstraintsRelOp[i].size();

        LhaCppFile << "\n    if (true){" << endl;
        LhaCppFile << "    vector <string> vcstr(" << NbC << ");" << endl;
        LhaCppFile << "    ConstraintsRelOp[" << i << "]=vcstr;" << endl;
        LhaCppFile << "    ConstraintsConstants[" << i << "]=vcstr;" << endl;
        LhaCppFile << "    vector <string>  vcvstr(NbVar, \"\");" << endl;
        LhaCppFile << "    vector < vector <string> > v2cvstr(" << NbC << ",vcvstr);" << endl;
        LhaCppFile << "    ConstraintsCoeffs[" << i << "]=v2cvstr;" << endl;
        LhaCppFile << "    }" << endl;

        for (int c = 0; c < NbC; c++) {
            LhaCppFile << "    ConstraintsRelOp[" << i << "][" << c << "]=\"" << MyLha.ConstraintsRelOp[i][c] << "\";" << endl;
            LhaCppFile << "    ConstraintsConstants[" << i << "][" << c << "]=\"" << MyLha.ConstraintsConstants[i][c] << "\";" << endl;
            for (int v = 0; v < MyLha.NbVar; v++)
                LhaCppFile << "    ConstraintsCoeffs[" << i << "][" << c << "][" << v << "]=\"" << MyLha.ConstraintsCoeffs[i][c][v] << "\";" << endl;




        }



    }



    LhaCppFile << "\n    vector<double> vdouble(NbVar);" << endl;
    LhaCppFile << "    vector<string> VarStr(NbVar);" << endl;
    LhaCppFile << "    Var=vdouble;" << endl;
    LhaCppFile << "    VarLabel=VarStr;" << endl;

    for (int v = 0; v < MyLha.NbVar; v++) {
        LhaCppFile << "    Var[" << v << "]=" << 0 << ";" << endl;
        LhaCppFile << "    VarLabel[" << v << "]=\"" << MyLha.VarLabel[v] << "\";" << endl;
        LhaCppFile << "    VarIndex[\"" << MyLha.VarLabel[v] << "\"]=" << v << ";" << endl;



    }

    LhaCppFile << "\n    vector<string> vStr(NbVar);" << endl;
    LhaCppFile << "    vector< vector <string > > vvStr(NbLoc,vStr);" << endl;
    LhaCppFile << "    StrFlow=vvStr;" << endl;
    for (int l = 0; l < MyLha.NbLoc; l++)
        for (int v = 0; v < MyLha.NbVar; v++) {
            LhaCppFile << "    StrFlow[" << l << "][" << v << "]=\"" << MyLha.StrFlow[l][v] << "\";" << endl;

        }

    LhaCppFile << "\n    vector< set < int > > vset(NbLoc);" << endl;
    LhaCppFile << "    Out_S_Edges =vset;" << endl;
    LhaCppFile << "    Out_A_Edges =vset;" << endl;
    for (int e = 0; e < MyLha.Edge.size(); e++) {
        if (MyLha.EdgeActions[e].size() < 1)
            LhaCppFile << "    Out_A_Edges[" << MyLha.Edge[e].Source << "].insert(" << e << ");" << endl;
        else
            LhaCppFile << "    Out_S_Edges[" << MyLha.Edge[e].Source << "].insert(" << e << ");" << endl;

    }

    LhaCppFile << "\n    vector< set <string> > vStrSet(" << MyLha.Edge.size() << ");" << endl;
    LhaCppFile << "    vector< set<int> > vSetInt(" << MyLha.TransitionIndex.size() << ");" << endl;
    LhaCppFile << "    vector < vector < set <int> > > vvSetInt(NbLoc,vSetInt);" << endl;

    LhaCppFile << "    EdgeActions=vStrSet;" << endl;
    LhaCppFile << "    ActionEdges=vvSetInt;" << endl;
    for (int e = 0; e < MyLha.Edge.size(); e++) {
        for (set<string>::iterator it = MyLha.EdgeActions[e].begin(); it != MyLha.EdgeActions[e].end(); it++) {
            LhaCppFile << "    EdgeActions[" << e << "].insert(\"" << *it << "\");" << endl;
            LhaCppFile << "    ActionEdges[" << MyLha.Edge[e].Source << "][" << MyLha.TransitionIndex[*it] << "].insert(" << e << ");" << endl;

        }
    }
    LhaCppFile << "    if(true){" << endl;
    LhaCppFile << "    			vector<double> vL(" << MyLha.LinearForm.size() << ",0);" << endl;
    LhaCppFile << "    			LinForm=vL;" << endl;
    LhaCppFile << "    			OldLinForm=vL;" << endl;
    LhaCppFile << "    			vector<double> vF(" << MyLha.LhaFuncArg.size() << ",0);" << endl;
    LhaCppFile << "    			LhaFunc=vF;" << endl;
    LhaCppFile << "    			M1Var=vF;" << endl;
    LhaCppFile << "    			M2Var=vF;" << endl;
    LhaCppFile << "    }" << endl;
    LhaCppFile << "}\n" << endl;




    LhaCppFile << "double LHA::GetFlow(int v, int loc, vector<int>& Marking){" << endl;
    LhaCppFile << "    switch(v){" << endl;
    for (int x = 0; x < MyLha.NbVar; x++) {

        LhaCppFile << "    case " << x << ":" << endl;
        LhaCppFile << "         switch(loc){" << endl;
        for (int l = 0; l < MyLha.NbLoc; l++) {
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

    LhaCppFile << "bool LHA::CheckLocation(int loc, vector<int>& Marking){" << endl;
    LhaCppFile << "    switch(loc){" << endl;
    for (int l = 0; l < MyLha.NbLoc; l++) {
        LhaCppFile << "     case " << l << ":" << endl;
        LhaCppFile << "         return " << MyLha.FuncLocProperty[l] << ";" << endl;
        LhaCppFile << "         break;" << endl;
    }
    LhaCppFile << "    }" << endl;


    LhaCppFile << "}\n" << endl;

    LhaCppFile << "bool LHA::CheckEdgeContraints(int ed, double DeltaT, vector<int>& Marking){" << endl;
    LhaCppFile << "    switch(ed){" << endl;
    for (int e = 0; e < MyLha.Edge.size(); e++) {
        LhaCppFile << "    case " << e << ": {" << endl;
        for (int c = 0; c < MyLha.ConstraintsRelOp[e].size(); c++) {
            LhaCppFile << "         if(!( ";
            for (int v = 0; v < MyLha.NbVar; v++) {
                if (MyLha.ConstraintsCoeffs[e][c][v] != "")
                    LhaCppFile << "+(" << MyLha.ConstraintsCoeffs[e][c][v] << ")*(Var[" << v << "]+DeltaT*GetFlow(" << v << "," << MyLha.Edge[e].Source << ", Marking))";

            }
            LhaCppFile << MyLha.ConstraintsRelOp[e][c] << MyLha.ConstraintsConstants[e][c] << ")) return false;" << endl;


        }
        LhaCppFile << "         return true; " << endl;
        LhaCppFile << "         break;" << endl;
        LhaCppFile << "     }" << endl;
    }
    LhaCppFile << "    }" << endl;


    LhaCppFile << "}\n" << endl;


    LhaCppFile << "t_interval LHA::GetEdgeEnablingTime(int ed, vector<int>& Marking){" << endl;
    LhaCppFile << "    switch(ed){" << endl;
    for (int e = 0; e < MyLha.Edge.size(); e++) {
        LhaCppFile << "     case " << e << ":" << endl;

        LhaCppFile << "         return GetEdgeEnablingTime_" << e << "( Marking);" << endl;
        LhaCppFile << "         break;" << endl;
    }
    LhaCppFile << "    }" << endl;


    LhaCppFile << "}\n" << endl;



    for (int e = 0; e < MyLha.Edge.size(); e++) {
        LhaCppFile << "t_interval LHA::GetEdgeEnablingTime_" << e << "(vector<int>& Marking){" << endl;
        LhaCppFile << "    t_interval EnablingT;\n" << endl;
        LhaCppFile << "    EnablingT.first=CurrentTime;" << endl;
        LhaCppFile << "    EnablingT.second=DBL_MAX;\n" << endl;
        LhaCppFile << "    t_interval EmptyInterval;\n" << endl;
        LhaCppFile << "    EmptyInterval.first=0;" << endl;
        LhaCppFile << "    EmptyInterval.second=-1;\n" << endl;

        LhaCppFile << "    double SumAF;" << endl;
        LhaCppFile << "    double SumAX;" << endl;



        LhaCppFile << "\n" << endl;


        for (int c = 0; c < MyLha.ConstraintsRelOp[e].size(); c++) {

            LhaCppFile << "    SumAF=";
            for (int v = 0; v < MyLha.NbVar; v++)
                if (MyLha.ConstraintsCoeffs[e][c][v] != "")
                    LhaCppFile << "+(" << MyLha.ConstraintsCoeffs[e][c][v] << ")*GetFlow(" << v << "," << MyLha.Edge[e].Source << ", Marking)";
            LhaCppFile << ";\n    SumAX=";
            for (int v = 0; v < MyLha.NbVar; v++)
                if (MyLha.ConstraintsCoeffs[e][c][v] != "")
                    LhaCppFile << "+(" << MyLha.ConstraintsCoeffs[e][c][v] << ")*Var[" << v << "]";
            LhaCppFile << ";\n" << endl;

            string RelOp = MyLha.ConstraintsRelOp[e][c];

            LhaCppFile << "    if(SumAF==0){" << endl;
            LhaCppFile << "         if(!(SumAX" << MyLha.ConstraintsRelOp[e][c] << MyLha.ConstraintsConstants[e][c] << "))" << endl;
            LhaCppFile << "             return EmptyInterval;" << endl;
            LhaCppFile << "    }" << endl;
            LhaCppFile << "    else{" << endl;
            LhaCppFile << "         double t=CurrentTime+(" << MyLha.ConstraintsConstants[e][c] << "-SumAX)/(double)SumAF;" << endl;
            if (RelOp == "==") {

                LhaCppFile << "         if(t>=EnablingT.first && t<=EnablingT.second){" << endl;
                LhaCppFile << "             EnablingT.first=t; EnablingT.second=t;" << endl;
                LhaCppFile << "         }" << endl;
                LhaCppFile << "         else return EmptyInterval;" << endl;

            } else {
                LhaCppFile << "         if(SumAF>0){" << endl;
                if (RelOp == "<=") {
                    LhaCppFile << "            if(EnablingT.second>t) EnablingT.second=t;" << endl;
                    LhaCppFile << "            if(EnablingT.second<EnablingT.first) return EmptyInterval;" << endl;

                } else {
                    LhaCppFile << "            if(EnablingT.first<t) EnablingT.first=t;" << endl;
                    LhaCppFile << "            if(EnablingT.second<EnablingT.first) return EmptyInterval;" << endl;
                }

                LhaCppFile << "             }" << endl;

                LhaCppFile << "         else{" << endl;
                RelOp = InvRelOp(RelOp);
                if (RelOp == "<=") {
                    LhaCppFile << "            if(EnablingT.second>t) EnablingT.second=t;" << endl;
                    LhaCppFile << "            if(EnablingT.second<EnablingT.first) return EmptyInterval;" << endl;

                } else {
                    LhaCppFile << "            if(EnablingT.first<t) EnablingT.first=t;" << endl;
                    LhaCppFile << "            if(EnablingT.second<EnablingT.first) return EmptyInterval;" << endl;
                }

                LhaCppFile << "             }" << endl;
            }
            LhaCppFile << "    }" << endl;





        }

        LhaCppFile << "    return EnablingT;\n}\n" << endl;
    }



    LhaCppFile << "void LHA::DoEdgeUpdates(int ed, vector<int>& Marking){" << endl;
    LhaCppFile << "    switch(ed){" << endl;
    for (int e = 0; e < MyLha.Edge.size(); e++) {
        LhaCppFile << "     case " << e << ":" << endl;

        LhaCppFile << "         DoEdgeUpdates_" << e << "( Marking);" << endl;
        LhaCppFile << "         break;" << endl;
    }
    LhaCppFile << "    }" << endl;


    LhaCppFile << "}\n" << endl;



    for (int e = 0; e < MyLha.Edge.size(); e++) {
        LhaCppFile << "void LHA::DoEdgeUpdates_" << e << "(vector<int>& Marking){" << endl;
        int k = 0;
        for (int v = 0; v < MyLha.NbVar; v++)
            if (MyLha.FuncEdgeUpdates[e][v] != "") {
                k++;
                break;
            }
        if (k > 0) {
            LhaCppFile << "    vector<double> TempVar(NbVar);" << endl;
            for (int v = 0; v < MyLha.NbVar; v++)
                if (MyLha.FuncEdgeUpdates[e][v] != "")
                    LhaCppFile << "    TempVar[" << v << "]=" << MyLha.FuncEdgeUpdates[e][v] << ";" << endl;

            for (int v = 0; v < MyLha.NbVar; v++)
                if (MyLha.FuncEdgeUpdates[e][v] != "") {
                    LhaCppFile << "    Var[" << v << "]=TempVar[" << v << "];" << endl;


                }

        }
        LhaCppFile << endl;
        for (map<string, int>::iterator it = MyLha.LinearForm.begin(); it != MyLha.LinearForm.end(); it++) {
            LhaCppFile << "    OldLinForm[" << (*it).second << "]=LinForm[" << (*it).second << "];" << endl;

        }
        LhaCppFile << "    }\n" << endl;
    }

    LhaCppFile << "void LHA::UpdateLinForm(vector<int>& Marking){" << endl;
    for (map<string, int>::iterator it = MyLha.LinearForm.begin(); it != MyLha.LinearForm.end(); it++) {
        LhaCppFile << "    LinForm[" << (*it).second << "]=" << (*it).first << ";" << endl;

    }
    LhaCppFile << "    }\n" << endl;

    LhaCppFile << "void LHA::UpdateLhaFunc(double& t0, double& Delta ){" << endl;
    for (int i = 0; i < MyLha.LhaFuncArg.size(); i++) {
        if (MyLha.LhaFuncType[i] == "Last")
            LhaCppFile << "    LhaFunc[" << i << "]=LinForm[" << MyLha.LhaFuncArg[i] << "];" << endl;
        else {
            if (MyLha.LhaFuncType[i] == "Min")
                LhaCppFile << "    LhaFunc[" << i << "]=Min(LhaFunc[" << i << "],LinForm[" << MyLha.LhaFuncArg[i] << "],OldLinForm[" << MyLha.LhaFuncArg[i] << "]);" << endl;
            else {
                if (MyLha.LhaFuncType[i] == "Max")
                    LhaCppFile << "    LhaFunc[" << i << "]=Max(LhaFunc[" << i << "],LinForm[" << MyLha.LhaFuncArg[i] << "],OldLinForm[" << MyLha.LhaFuncArg[i] << "]);" << endl;
                else {
                    if (MyLha.LhaFuncType[i] == "Integral")
                        LhaCppFile << "    LhaFunc[" << i << "]=LhaFunc["<<i<<"]+Integral(OldLinForm[" << MyLha.LhaFuncArg[i] << "],LinForm[" << MyLha.LhaFuncArg[i] << "],Delta);" << endl;
		    else {
		      if (MyLha.LhaFuncType[i] == "Mean"){
			  LhaCppFile <<"    if(Delta>0)"<<endl;	
			  LhaCppFile <<"    	LhaFunc[" << i << "]=(t0*LhaFunc["<<i<<"]+Integral(OldLinForm[" << MyLha.LhaFuncArg[i] << "],LinForm[" << MyLha.LhaFuncArg[i] << "],Delta))/(t0+Delta);" << endl;
		      }
		      else {
			if (MyLha.LhaFuncType[i] == "Var"){
			    LhaCppFile << "    if(Delta>0){"<<endl;
			    LhaCppFile << "        M1Var[" << i << "]=(t0*M1Var["<<i<<"]+Integral(OldLinForm[" << MyLha.LhaFuncArg[i] << "],LinForm[" << MyLha.LhaFuncArg[i] << "],Delta))/(t0+Delta);" << endl;
			    LhaCppFile << "        M2Var[" << i << "]=(t0*M2Var["<<i<<"]+IntegralP2(OldLinForm[" << MyLha.LhaFuncArg[i] << "],LinForm[" << MyLha.LhaFuncArg[i] << "],Delta))/(t0+Delta);" << endl;
			    LhaCppFile << "        LhaFunc[" << i << "]=M2Var["<<i<<"]-pow(M1Var["<<i<<"],2);"<< endl;
			    LhaCppFile << "    }"<<endl;
			  
			    }
			  }
			}
		    }

		}
	}
    }

    LhaCppFile << "\n    }\n" << endl;
    
    LhaCppFile << "void LHA::UpdateFormulaVal(){" << endl;
    LhaCppFile << "\n    OldFormulaVal=FormulaVal;" << endl;
    LhaCppFile << "    FormulaVal=" << MyLha.Algebraic << ";\n" << endl;
    LhaCppFile << "    }\n" << endl;


    LhaCppFile.close();


    loc = Pref + "../SOURCES/Cosmos/LHA.hpp";
    ofstream LhaHppFile(loc.c_str(), ios::out | ios::trunc);

    loc = Pref + "../SOURCES/ModelGenerator/LHA_orig.hpp";
    ifstream LhaHppFile_orig(loc.c_str(), ios::in);




    while (!LhaHppFile_orig.eof()) {
        string LineFile;
        getline(LhaHppFile_orig, LineFile);
        LhaHppFile << LineFile << endl;
    }

    LhaHppFile_orig.close();

    for (int e = 0; e < MyLha.Edge.size(); e++)
        LhaHppFile << "t_interval GetEdgeEnablingTime_" << e << "(vector<int>& );" << endl;

    for (int e = 0; e < MyLha.Edge.size(); e++)
        LhaHppFile << "void DoEdgeUpdates_" << e << "(vector<int>& );" << endl;

    LhaHppFile << "private:" << endl;

    LhaHppFile << "};" << endl;



    LhaHppFile << "#endif	/* _Lha_HPP */" << endl;

    LhaHppFile.close();

}
