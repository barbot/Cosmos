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
	
	IndexDist["UNIFORM"] = 1;
	IndexDist["EXPONENTIAL"] = 2;
	IndexDist["DETERMINISTIC"] = 3;
	IndexDist["LOGNORMAL"] = 4;
	IndexDist["TRIANGLE"] = 5;
	IndexDist["GEOMETRIC"] = 6;
	IndexDist["ERLANG"] = 7;
	IndexDist["GAMMA"] =8;
	
	
}

Gspn_Reader::~Gspn_Reader() {
}

int Gspn_Reader::parse(string& expr) {
	
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
		
		int x = parse(str);
		
		if (x) cout << "Parsing GSPN Description file failed" << endl;
		
		return x;
	} else {
		cout << "Can't open : " << filename << endl;
		return 1;
	}
}

//#include "Gspn_gmlparser.cpp";
int Gspn_Reader::parse_gml_file(parameters &P) {
	ifstream ifile(P.PathGspn.c_str());
	if(ifile){
		//cout << "parse GML:" << filename << endl;
		ModelHandlerPtr handlerPtr(new MyModelHandler(&MyGspn,P.RareEvent,P.verbose));
		ExpatModelParser parser = ExpatModelParser(handlerPtr);
		parser.parse_file(P.PathGspn);
		//cout << "end parse GML:"<< MyGspn.pl << endl;
		if (P.RareEvent)addSinkTrans();
		return 0;
	}else{
		cout << "File " << P.PathGspn << " does not exist!" << endl;
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


void Gspn_Reader::addSinkTrans(){
	(MyGspn.outArcs)[MyGspn.tr-1][MyGspn.pl-1]=1;
}

void Gspn_Reader::EnabledDisabledTr(vector< set<int> > &PossiblyEnabled,
                                    vector< set<int> > &PossiblyDisabled,
                                    vector< set<int> > &FreeMarkDepT) {
	
	for (size_t t1 = 0; t1 < MyGspn.tr; t1++) {
		set<int> S;
		set<int> Sinhib;
		set<int> INt1;
		for (size_t p = 0; p < MyGspn.pl; p++) {
			if (MyGspn.inArcs[t1][p] > 0) {
				INt1.insert(p);
			}
		}
		for (size_t t2 = 0; t2 < MyGspn.tr; t2++)
			if (t1 != t2) {
				size_t size = INt1.size();
				set<int> INt1t2 = INt1;
				bool B = true;
				size_t p = 0;
				while (B && p < MyGspn.pl) {
					if (MyGspn.inArcs[t2][p] > 0) {
						INt1t2.insert(p);
						if (size == INt1t2.size()) {
							B = false;
							S.insert(t2);
						} else size = INt1t2.size();
					}
					p++;
				}
				
				size = INt1.size();
				set<int> INt1t2Inhib = INt1;
				B = true;
				p = 0;
				while (B && p < MyGspn.pl) {
					if (MyGspn.inhibArcs[t2][p] > 0) {
						INt1t2Inhib.insert(p);
						if (size == INt1t2Inhib.size()) {
							B = false;
							Sinhib.insert(t2);
						} else size = INt1t2Inhib.size();
					}
					p++;
				}
				
			}
		
		
		PossiblyDisabled.push_back(S);
		PossiblyEnabled.push_back(Sinhib);
	}
	for (size_t t1 = 0; t1 < MyGspn.tr; t1++) {
		set<int> S = PossiblyEnabled[t1];
		set<int> Sinhib = PossiblyDisabled[t1];
		set<int> OUTt1;
		for (size_t p = 0; p < MyGspn.pl; p++)
			if (MyGspn.outArcs[t1][p] > 0)
				OUTt1.insert(p);
		for (size_t t2 = 0; t2 < MyGspn.tr; t2++)
			if (t1 != t2) {
				size_t size = OUTt1.size();
				set<int> OUTt1INt2 = OUTt1;
				bool B = true;
				size_t p = 0;
				while (B && p < MyGspn.pl) {
					if (MyGspn.inArcs[t2][p] > 0) {
						OUTt1INt2.insert(p);
						if (size == OUTt1INt2.size()) {
							B = false;
							S.insert(t2);
						} else size = OUTt1INt2.size();
					}
					p++;
				}
				size = OUTt1.size();
				set<int> OUTt1t2Inhib = OUTt1;
				B = true;
				p = 0;
				while (B && p < MyGspn.pl) {
					if (MyGspn.inhibArcs[t2][p] > 0) {
						OUTt1t2Inhib.insert(p);
						if (size == OUTt1t2Inhib.size()) {
							B = false;
							Sinhib.insert(t2);
						} else size = OUTt1t2Inhib.size();
					}
					p++;
				}
				
			}
		PossiblyEnabled[t1] = S;
		PossiblyDisabled[t1] = Sinhib;
	}
	
	set<int> MarkDepT;
	for (size_t t = 0; t < MyGspn.tr; t++)
		if (MyGspn.transitionStruct[t].markingDependant)
			MarkDepT.insert(t);
	
	for (size_t t = 0; t < MyGspn.tr; t++) {
		set<int> S;
		for (set<int>::iterator it = MarkDepT.begin(); it != MarkDepT.end(); it++) {
			if ((PossiblyEnabled[t].find((*it)) == PossiblyEnabled[t].end()) &&
				(PossiblyDisabled[t].find((*it)) == PossiblyDisabled[t].end()))
				S.insert((*it));
		}
		FreeMarkDepT.push_back(S);
	}
}

void Gspn_Reader::writeEnabledDisabled(ofstream &SpnF){
	vector< set<int> > PossiblyEnabled;
	vector< set<int> > PossiblyDisabled;
	vector< set<int> > FreeMarkDepT;
	
	EnabledDisabledTr(PossiblyEnabled,PossiblyDisabled,FreeMarkDepT);
	
	
	SpnF << "\tPossiblyEnabled = vector< set<int> >("<< MyGspn.tr << ");"<< endl;
	for (size_t t = 0; t < MyGspn.tr; t++) {
		if(PossiblyEnabled[t].size()>0){
			SpnF << "\t{\n\tint PE[]= {";
			for (set<int>::iterator it = PossiblyEnabled[t].begin(); it != PossiblyEnabled[t].end(); it++) {
				//SpnF << "\tPossiblyEnabled[" << t << "].insert( " << *it << " );"<< endl;
				if(it != PossiblyEnabled[t].begin())SpnF << ", ";
				SpnF << *it;
			}
			SpnF << "};" << endl << "\t\tPossiblyEnabled[" << t << "] = set<int>(PE,PE+"<< PossiblyEnabled[t].size()<< ");\n\t}" << endl;
		}
	}
	SpnF << endl;
	
	
	SpnF << "\tPossiblyDisabled = vector< set<int> >("<< MyGspn.tr << ");"<< endl;
	for (size_t t = 0; t < MyGspn.tr; t++) {
		if(PossiblyDisabled[t].size()>0){
			SpnF << "\t{\n\tint PE[]= {";
			for (set<int>::iterator it = PossiblyDisabled[t].begin(); it != PossiblyDisabled[t].end(); it++) {
				//SpnF << "\tPossiblyEnabled[" << t << "].insert( " << *it << " );"<< endl;
				if(it != PossiblyDisabled[t].begin())SpnF << ", ";
				SpnF << *it;
			}
			SpnF << "};" << endl << "\t\tPossiblyDisabled[" << t << "] = set<int>(PE,PE+"<< PossiblyDisabled[t].size()<< ");\n\t}" << endl;
		}
	}
	SpnF << endl;
	
	
	SpnF << "\tFreeMarkDepT = vector< set<int> >("<< MyGspn.tr << ");"<< endl;
	for (size_t t = 0; t < MyGspn.tr; t++) {
		if(FreeMarkDepT[t].size()>0){
			SpnF << "\t{\n\tint PE[]= {";
			for (set<int>::iterator it = FreeMarkDepT[t].begin(); it != FreeMarkDepT[t].end(); it++) {
				//SpnF << "\tPossiblyEnabled[" << t << "].insert( " << *it << " );"<< endl;
				if(it != FreeMarkDepT[t].begin())SpnF << ", ";
				SpnF << *it;
			}
			SpnF << "};" << endl << "\t\tFreeMarkDepT[" << t << "] = set<int>(PE,PE+"<< FreeMarkDepT[t].size()<< ");\n\t}" << endl;
		}
	}
	SpnF << endl;
}

void Gspn_Reader::printloot(ofstream& fs, size_t domain, size_t nesting ){
	const colorDomain& dom = MyGspn.colDoms[domain];
	if(nesting == dom.colorClassIndex.size()){
		stringstream mult;
		mult << "x.mult";
		for (size_t count = 0 ; count < dom.colorClassIndex.size(); count++ ) {
			mult << "[c" << count << "]";
		}
		fs << "\t\tif(" << mult.str() << ")\n\t\t";
		fs << "\t\t\tout << "<< mult.str();
		fs << "<< \"<\"";
		for (size_t count = 0 ; count < dom.colorClassIndex.size(); count++ ) {
			if( count > 0)fs << " << \",\"";
			fs << " << Color_"<< MyGspn.colClasses[dom.colorClassIndex[count]].name << "_names[c" << count << "]";
		}
		fs << "<< \"> \";\n";
		return;
	}
	fs << "\tfor(size_t c" << nesting << " = 0 ; c"<<nesting<<"< Color_";
	fs << MyGspn.colClasses[dom.colorClassIndex[nesting]].name << "_Total; c";
	fs << nesting << "++ )\n";
	printloot(fs, domain, nesting+1);
}

void Gspn_Reader::writeMarkingClasse(ofstream &SpnCppFile,ofstream &header, parameters &P){
	SpnCppFile << "#include \"marking.hpp\"\n";
	SpnCppFile << "#include \"markingImpl.hpp\"\n";
	header << "#include <string.h>\n";
	
	for (vector<colorClass>::const_iterator it = MyGspn.colClasses.begin();
		 it != MyGspn.colClasses.end(); ++it ) {
		header << "enum " << it->cname() << " {\n";
		for (vector<color>::const_iterator it2 = it->colors.begin();
			 it2 != it->colors.end(); ++it2 ) {
			header << "\tColor_" << it->name << "_" << it2->name << ",\n";
		}
		header << "\tColor_" << it->name << "_Total,\n";
		header << "\tColor_"<< it->name << "_All\n};\n";
		header << "extern const char *Color_"<< it->name << "_names[];\n";
	}
	
	
	
	for (vector<colorDomain>::const_iterator it = MyGspn.colDoms.begin()+1;
		 it != MyGspn.colDoms.end(); ++it ) {
		
		//token class def
		header << "\nstruct " << it->tokname() << "{\n";
		vector<size_t>::const_iterator itcol;
		for (itcol = it->colorClassIndex.begin(); itcol != it->colorClassIndex.end() ; ++itcol ) {
			header << "\t" << MyGspn.colClasses[*itcol].cname() << " c" << itcol - it->colorClassIndex.begin() << ";\n";
		}
		header << "\tint mult;\n";
		
		header << "\t" << it->tokname() << "( ";
		for (itcol = it->colorClassIndex.begin(); itcol != it->colorClassIndex.end() ; ++itcol ) {
			header << " "<<MyGspn.colClasses[*itcol].cname() << " cv" << itcol - it->colorClassIndex.begin()<< " = ("<< MyGspn.colClasses[*itcol].cname() << ")0, " ;
		}
		header << "int v =1) {\n";
		for (itcol = it->colorClassIndex.begin(); itcol != it->colorClassIndex.end() ; ++itcol ) {
			size_t pos = itcol - it->colorClassIndex.begin();
			header << "\t\tc" << pos << "= cv"<<pos <<";\n";
		}
		header << "\t\tmult = v;\n\t}\n";
		
		
		header << "\t" << it->tokname() << "( ";
		for (itcol = it->colorClassIndex.begin(); itcol != it->colorClassIndex.end() ; ++itcol ) {
			header << "const "<<MyGspn.colClasses[*itcol].name << "_Token& cv" << itcol - it->colorClassIndex.begin()<< ", " ;
		}
		header << "int v =1) {\n";
		for (itcol = it->colorClassIndex.begin(); itcol != it->colorClassIndex.end() ; ++itcol ) {
			size_t pos = itcol - it->colorClassIndex.begin();
			header << "\t\tc" << pos << "= cv";
			header << pos << ".c0" << ";\n";
		}
		header << "\t\tmult = v;\n\t}\n";
		
		header << "\t" << it->tokname() << " operator * (size_t v){\n";
		header << "\t\tmult *= v;\n\t\treturn *this;\n\t}\n";
		
		
		header << "\tvoid iter() {\n";
		for (itcol = it->colorClassIndex.begin(); itcol != it->colorClassIndex.end() ; ++itcol ) {
			size_t pos = itcol - it->colorClassIndex.begin();
			header << "\t\tif( c" << pos << "< ("<< MyGspn.colClasses[*itcol].cname() << ")(Color_";
			header << MyGspn.colClasses[*itcol].name << "_Total - 1) )";
			header << "{ c"<< pos << " = ("<< MyGspn.colClasses[*itcol].cname() << ")(c"<< pos <<"+ 1); return;};\n";
			header << "c"<< pos << " = ("<< MyGspn.colClasses[*itcol].cname() << ")(0);\n";
		}
		header << "\t}\n";
		
		header << "\tvoid print(){\n\t\tstd::cerr << mult << \"<\" <<";
		for (itcol = it->colorClassIndex.begin(); itcol != it->colorClassIndex.end() ; ++itcol ) {
			if(itcol != it->colorClassIndex.begin())header << "<< \" , \" << ";
			header << " Color_"<< MyGspn.colClasses[*itcol].name << "_names[c" << itcol - it->colorClassIndex.begin() << "]";
		}
		header << " << \">\";\n";
		header << "\t}\n";
		
		
		
		if (it->colorClassIndex.size()==1 && MyGspn.colClasses[it->colorClassIndex[0]].isCircular) {
			header << "\t" << it->tokname() << " next(int i)const {\n";
			header << "\t\t" << it->tokname() << " x(("<< MyGspn.colClasses[it->colorClassIndex[0]].cname() << ")((c0 +i) % Color_";
			header << MyGspn.colClasses[it->colorClassIndex[0]].name << "_Total), ";
			header << " mult);\n\t\treturn x;}\n";
		}
		
		header << "\tbool islast()const {\n\t\treturn (";
		for (itcol = it->colorClassIndex.begin(); itcol != it->colorClassIndex.end() ; ++itcol ) {
			size_t pos = itcol - it->colorClassIndex.begin();
			if (pos > 0)header << " && ";
			header << " c" << pos << "== ("<< MyGspn.colClasses[*itcol].cname() << ")(Color_";
			header << MyGspn.colClasses[*itcol].name << "_Total -1) ";
		}
		header << " );\n\t}\n";
		
		header << "\tbool operator > (const int x){\n";
		header << "\t\treturn mult > x ;\n\t}\n";
		header << "\tbool operator < (const int x){\n";
		header << "\t\treturn mult < x ;\n\t}\n";
		
		header << "};\n";
		
		
		
		stringstream colorArgsName;
		stringstream colorArrayAccess;
		stringstream colorArrayAccess2;
		stringstream allCondition;
		stringstream forLoop;
		
		header << "struct " << it->cname() << " {\n\tint mult";
		for (vector<size_t>::const_iterator it2 = it->colorClassIndex.begin();
			 it2 != it->colorClassIndex.end(); ++it2 ) {
			header << "[ Color_" << MyGspn.colClasses[*it2].name << "_Total ]";
			size_t countCol = it2 - it->colorClassIndex.begin();
			if(countCol > 0){
				colorArgsName << ",";
				allCondition << " && ";
			}
			colorArgsName << MyGspn.colClasses[ *it2 ].cname() << " c" << countCol;
			colorArrayAccess << "[c" << countCol << "]";
			colorArrayAccess2 << "[i" << countCol << "]";
			allCondition << "c" << countCol << " != Color_" << MyGspn.colClasses[*it2].name << "_All";
			forLoop << "\t\t\tfor( int i" <<
			countCol <<"= ( c" << countCol << " == Color_" << MyGspn.colClasses[*it2].name << "_All ? 0 : c" << countCol << ");";
			forLoop << "i" << countCol <<"< ( c" << countCol << " == Color_" << MyGspn.colClasses[*it2].name << "_All ? Color_"<< MyGspn.colClasses[*it2].name <<"_Total : c" << countCol << "+1);";
			forLoop << "i" << countCol << "++)\n";
			
			
		}
		header << ";\n";
		header << "\t" << it->cname() << "(size_t v =0) { fill( (int*)mult ,((int*)mult) + sizeof(mult)/sizeof(int), v );}"<< endl;
		header << "\t" << it->cname() << "(" << colorArgsName.str() << ") {\n";
		header << "\t\t" << "memset(&mult,0 , sizeof(mult));\n";
		header << "\t\t" << "if(" << allCondition.str() << ")\n";
		header << "\t\t\t" << "mult" << colorArrayAccess.str() << " = 1 ;\n";
		header << "\t\telse{\n";
		header << forLoop.str() << "\t\t\t\tmult" << colorArrayAccess2.str() << " = 1 ;\n";
		
		header << "\t\t}\n";
		header << "\t}\n";
		header << "\t" << it->cname() << "& operator = (const " << it->cname() << "& x){\n";
		header << "\t\tcopy((int*)x.mult,(int*)x.mult + sizeof(mult)/sizeof(int),(int*)mult);\n\t\treturn *this;\n\t}\n";
		
		header << "\tbool operator == (const " << it->cname() << "& x){\n";
		header << "\t\treturn  equal((int*)mult, ((int*)mult) + sizeof(mult)/sizeof(int), (int*)x.mult);\n\t}\n";
		
		header << "\tbool operator < (const " << it->cname() << "& x){\n";
		header << "\t\treturn  equal((int*)mult, ((int*)mult) + sizeof(mult)/sizeof(int), (int*)x.mult,std::less<int>());\n\t}\n";
		
		header << "\tbool operator > (const " << it->cname() << "& x){\n";
		header << "\t\treturn  equal((int*)mult, ((int*)mult) + sizeof(mult)/sizeof(int), (int*)x.mult,std::greater<int>());\n\t}\n";
		
		header << "\t" << it->cname() << " operator * (int v){\n";
		header << "\t\tfor(size_t count = 0 ; count < sizeof(mult)/sizeof(int);count++) ((int*)mult)[count]*= v;\n\t\treturn *this;\n\t}\n";
		
		header << "\t" << it->cname() << "& operator += (const " << it->cname() << "& x){\n";
		header << "\t\tfor(size_t count = 0 ; count < sizeof(mult)/sizeof(int);count++)";
		header << "\n\t\t\t((int*)mult)[count]+= ((int*)x.mult)[count] ;\n";
		header << "\t\treturn *this;\n\t}\n";
		
		header << "\t" << it->cname() << "& operator += (const " << it->tokname() << "& x){\n";
		header << "\t\tmult";
		for (vector<size_t>::const_iterator it2 = it->colorClassIndex.begin();
			 it2 != it->colorClassIndex.end(); ++it2 ) {
			header << "[ x.c" << it2- it->colorClassIndex.begin() << " ]" ;
		}
		header << " += x.mult;\n\t\treturn *this;\n\t}\n";
		
		header << "\t" << it->cname() << "& operator + (const " << it->tokname() << "& x){\n";
		header << "\t\t"<< it->cname()<< " d(*this);\n\t\td+=x;\n ";
		header << "\t\treturn *this;\n}\n";
		
		header << "\t" << it->cname() << "& operator -= (const " << it->tokname() << "& x){\n";
		header << "\t\tmult";
		for (vector<size_t>::const_iterator it2 = it->colorClassIndex.begin();
			 it2 != it->colorClassIndex.end(); ++it2 ) {
			header << "[ x.c" << it2- it->colorClassIndex.begin() << " ]" ;
		}
		header << " -= x.mult;\n\t\treturn *this;\n\t}\n";
		
		header << "\tbool operator < (const " << it->tokname() << "& x)const{\n";
		header << "\t\treturn mult";
		for (vector<size_t>::const_iterator it2 = it->colorClassIndex.begin();
			 it2 != it->colorClassIndex.end(); ++it2 ) {
			header << "[ x.c" << it2- it->colorClassIndex.begin() << " ]" ;
		}
		header << " < x.mult;\n\t}\n";
		
		header << "\tbool operator >= (const " << it->tokname() << "& x)const{\n";
		header << "\t\treturn mult";
		for (vector<size_t>::const_iterator it2 = it->colorClassIndex.begin();
			 it2 != it->colorClassIndex.end(); ++it2 ) {
			header << "[ x.c" << it2- it->colorClassIndex.begin() << " ]" ;
		}
		header << " >= x.mult;\n\t}\n";
		
		header << "\t" << it->cname() << " operator + (const " << it->cname() << "& x)const{\n";
		header << "\t\t"<< it->cname() << " returnval = *this; returnval+= x;\n";
		header << "\t\treturn returnval;\n\t}\n";
		
		header << "\t" << it->cname() << "& operator -= (const " << it->cname() << "& x){\n";
		header << "\t\tfor(size_t count = 0 ; count < sizeof(mult)/sizeof(int);count++)";
		header << "\n\t\t\t((int*)mult)[count]-= ((int*)x.mult)[count] ;\n";
		header << "\t\treturn *this;\n\t}\n";
		
		header << "\t" << it->cname() << " operator - (const " << it->cname() << "& x)const{\n";
		header << "\t\t"<< it->cname() << " returnval = *this; returnval-= x;\n";
		header << "\t\treturn returnval;\n\t}\n";
		
		header << "\tint card (void){\n";
		header << "\tint acc=0;\n";
		header << "\t\tfor(size_t count = 0 ; count < sizeof(mult)/sizeof(int);count++)";
		header << "\n\t\t\tacc += ((int*)mult)[count] ;\n";
		header << "\t\treturn acc;\n\t}\n";
		
		header << "};\n";
		//end of domain class definition
		
		
		header << it->cname() << " operator + (const " << it->tokname() << "& t1 ,const " << it->tokname() << "& t2 )\n\n;";
		
		SpnCppFile << "" << it->cname() << " operator + (const " << it->tokname() << "& t1 ,const " << it->tokname() << "& t2 ){\n";
		SpnCppFile << "\t"<< it->cname() << " d; d += t1; d+=t2 ;\n";
		SpnCppFile << "\treturn d;\n}\n";
		
		
		SpnCppFile << "std::ostream& operator << (std::ostream& out, const " << it->cname();
		SpnCppFile << "& x) {\n";
		printloot(SpnCppFile, it - MyGspn.colDoms.begin(), 0);
		SpnCppFile << "\treturn out;\n}\n";
		
		SpnCppFile << "inline bool contains(const "<<it->cname() << "& d1, const " << it->cname() << "& d2){";
		SpnCppFile << "\treturn (d1-d2) > -1;\n";
		SpnCppFile << "}\n";
		
		SpnCppFile << "inline bool contains(const "<<it->cname() << "& d1, const " << it->tokname() << "& tok){";
		SpnCppFile << "\treturn d1 >= tok;\n";
		SpnCppFile << "}\n";
	}
	
	header << "struct abstractBindingImpl {\n";
	for (vector<colorVariable>::const_iterator colvar = MyGspn.colVars.begin() ; colvar != MyGspn.colVars.end(); ++colvar) {
		header << "\t" << MyGspn.colDoms[colvar->type].tokname() << " " << colvar->name << ";\n";
	}
	header << "};\n";
	
	header << "class abstractMarkingImpl {\n";
	header << "public:\n";
	
	for (vector<place>::const_iterator plit = MyGspn.placeStruct.begin();
		 plit!= MyGspn.placeStruct.end(); ++plit) {
		
		header << "\t"<< MyGspn.colDoms[plit->colorDom].cname() << " _PL_"<< plit->name << ";\n";
		
	}
	writeVariable(header);
	header << "};\n";
	
	SpnCppFile << "\n";
	SpnCppFile << "void abstractMarking::resetToInitMarking(){\n";
	for (vector<place>::const_iterator plit = MyGspn.placeStruct.begin();
		 plit!= MyGspn.placeStruct.end(); ++plit) {
		SpnCppFile << "\tP->_PL_"<< plit->name << " =" <<
		MyGspn.Marking[MyGspn.PlacesId[plit->name]] << ";\n";
	}
	SpnCppFile << "}\n";
	SpnCppFile << "\n";
	SpnCppFile << "\n";
	SpnCppFile << "abstractMarking::abstractMarking() {\n";
	SpnCppFile << "\tP= new abstractMarkingImpl;\n";
	SpnCppFile << "\tresetToInitMarking();\n";
	SpnCppFile << "}\n";
	SpnCppFile << "\n";
	SpnCppFile << "abstractMarking::abstractMarking(const std::vector<int>& m) {\n";
	SpnCppFile << "\tP = new abstractMarkingImpl;\n";
	SpnCppFile << "\tsetVector(m);\n";
	SpnCppFile << "}\n";
	SpnCppFile << "abstractMarking::abstractMarking(const abstractMarking& m) {\n";
	SpnCppFile << "\tP= new abstractMarkingImpl;\n";
	SpnCppFile << "\t*this = m;\n";
	SpnCppFile << "};\n";
	SpnCppFile << "\n";
	SpnCppFile << "abstractMarking& abstractMarking::operator = (const abstractMarking& m) {\n";
	SpnCppFile << "\t*P = *(m.P);\n";
	SpnCppFile << "\treturn *this;\n";
	SpnCppFile << "}\n";
	SpnCppFile << "\n";
	SpnCppFile << "abstractMarking::~abstractMarking() {\n";
	SpnCppFile << "\tdelete(P);\n";
	SpnCppFile << "}\n";
	SpnCppFile << "\n";
	SpnCppFile << "void abstractMarking::print(ostream &s)const{\n";
	if(P.StringInSpnLHA){
		//SpnCppFile << "\tstd::cerr << \"Marking:\"<< std::endl;\n";
		for (vector<place>::const_iterator plit = MyGspn.placeStruct.begin();
			 plit!= MyGspn.placeStruct.end(); ++plit) {
			SpnCppFile << "\ts << \""<< plit->name <<": \" << P->_PL_"<< plit->name << " << \"\\t\" ;\n";
		}
	}
	SpnCppFile << "}\n";
	SpnCppFile << "\n";
	SpnCppFile << "int abstractMarking::getNbOfTokens(int p)const {\n";
	if(MyGspn.isColored()){
		SpnCppFile << "\texit(EXIT_FAILURE);\n";
	}else{
		SpnCppFile << "\tswitch (p) {\n";
		for (vector<place>::const_iterator plit = MyGspn.placeStruct.begin();
			 plit!= MyGspn.placeStruct.end(); ++plit) {
			SpnCppFile << "\t\tcase "<< MyGspn.PlacesId[plit->name] << ": return P->_PL_"<< plit->name << ";\n";
		}
		SpnCppFile << "     }\n";
	}
	SpnCppFile << "}\n";
	SpnCppFile << "\n";
	SpnCppFile << "std::vector<int> abstractMarking::getVector()const {\n";
	if(MyGspn.isColored()){
		SpnCppFile << "\texit(EXIT_FAILURE);\n";
	}else{
		SpnCppFile << "\tstd::vector<int> v("<<MyGspn.pl << ");\n";
		for (vector<place>::const_iterator plit = MyGspn.placeStruct.begin();
			 plit!= MyGspn.placeStruct.end(); ++plit) {
			SpnCppFile << "\tv[" << MyGspn.PlacesId[plit->name] <<"] = P->_PL_" << plit->name << ";\n";
		}
		SpnCppFile << "     return v;\n";
	}
	SpnCppFile << "}\n";
	SpnCppFile << "\n";
	SpnCppFile << "void abstractMarking::setVector(const std::vector<int>&v) {\n";
	if(MyGspn.isColored()){
		SpnCppFile << "\texit(EXIT_FAILURE);\n";
	}else{
		for (vector<place>::const_iterator plit = MyGspn.placeStruct.begin();
			 plit!= MyGspn.placeStruct.end(); ++plit) {
			SpnCppFile << "\tP->_PL_" << plit->name << " = v[" << MyGspn.PlacesId[plit->name] << "];\n";
		}
	}
	SpnCppFile << "};"<<endl<<endl;
	
	SpnCppFile << "bool abstractBinding::next() {\n";
	SpnCppFile << "\tidcount++;\n";
	for (vector<colorVariable>::const_iterator colvar = MyGspn.colVars.begin() ; colvar != MyGspn.colVars.end(); ++colvar) {
		SpnCppFile << "\tif(P->" << colvar->name << ".mult >= 0){\n";
		SpnCppFile << "\t\tif (! P->"<< colvar->name << ".islast()){";
		SpnCppFile << "\tP->"<< colvar->name << ".iter(); return true; };\n";
		SpnCppFile << "\t\tP->"<< colvar->name << " = "<< MyGspn.colDoms[colvar->type].tokname() << "();\n";
		SpnCppFile << "\t}\n";
	}
	SpnCppFile << "\treturn false;\n};\n";
	
	
	SpnCppFile << "abstractBinding::abstractBinding() {\n";
	if(MyGspn.isColored())SpnCppFile << " P= new abstractBindingImpl;\n";
	SpnCppFile << "       idcount=0;\n";
	SpnCppFile << "}\n";
	
	SpnCppFile << "abstractBinding::abstractBinding(const abstractBinding& m) {\n";
	if(MyGspn.isColored())SpnCppFile << " P= new abstractBindingImpl;\n";
	if(MyGspn.isColored())SpnCppFile << " *P = *m.P;\n";
	SpnCppFile << "\tidcount = m.idcount;\n";
	SpnCppFile << "}\n";
	
	SpnCppFile << "abstractBinding::~abstractBinding() {\n";
	if(MyGspn.isColored())SpnCppFile << " delete P;\n";
	SpnCppFile << "}\n";
	
	SpnCppFile << "abstractBinding& abstractBinding::operator = (const abstractBinding& m) {\n";
	if(MyGspn.isColored())SpnCppFile << " *P = *m.P;\n";
	SpnCppFile << "\tidcount = m.idcount;\n";
	SpnCppFile << "       return *this;\n";
	SpnCppFile << "}\n";
	
	SpnCppFile << "void abstractBinding::print()const{\n";
	//SpnCppFile << "\tstd::cerr << \"Binding:\"<< std::endl;\n";
	for (vector<colorVariable>::const_iterator colvar = MyGspn.colVars.begin() ; colvar != MyGspn.colVars.end(); ++colvar) {
		SpnCppFile << "\tstd::cerr << \"\\t"<< colvar->name <<": \";";
		SpnCppFile << "P->"<< colvar->name << ".print();\n\tcerr << endl;\n";
	}
	SpnCppFile << "}\n";
	
	SpnCppFile << "int abstractBinding::id()const{\n";
	/*for (vector<colorVariable>::const_iterator colvar = MyGspn.colVars.begin() ; colvar != MyGspn.colVars.end(); ++colvar) {
	 SpnCppFile << "\tstd::cerr << \"\\t"<< colvar->name <<": \";";
	 SpnCppFile << "P->"<< colvar->name << ".print();\n\tcerr << endl;\n";
	 }*/
	SpnCppFile << "\treturn idcount;\n}\n";
	
}

void Gspn_Reader::writeTransition(ofstream & spnF, bool bstr){
	
	for (size_t t=0; t < MyGspn.tr; t++ ) {
		spnF << "\tTransition["<<t<<"].Id =" << t << ";" <<endl;
		if (MyGspn.transitionStruct[t].type==Timed) {
			spnF << "\tTransition["<<t<<"].transType = Timed;" <<endl;
			spnF << "\tTransition["<<t<<"].DistTypeIndex = "<< MyGspn.transitionStruct[t].dist.name << ";" <<endl;
			if (bstr) {
				for (size_t j = 0; j < MyGspn.transitionStruct[t].dist.Param.size(); j++) {
					spnF << "\tTransition[" << t << "].DistParams.push_back(\" " << MyGspn.transitionStruct[t].dist.Param[j] << "\" );" << endl;
				}
			}
		}else{
			spnF << "\tTransition["<<t<<"].transType = unTimed;" <<endl;
			spnF << "\tTransition["<<t<<"].DistTypeIndex = DETERMINISTIC ;" <<endl;
		}
		spnF << "\tTransition["<<t<<"].MarkingDependent = "<< MyGspn.transitionStruct[t].markingDependant << ";" <<endl;
		
		if (bstr) {
			spnF << "\tTransition["<<t<<"].label = \""<< MyGspn.transitionStruct[t].label << "\";"<< endl;
			spnF << "\tTransition["<<t<<"].priority = \""<< MyGspn.transitionStruct[t].priority << "\";"<< endl;
			spnF << "\tTransition["<<t<<"].weight = \""<< MyGspn.transitionStruct[t].weight << "\";"<< endl;
		}
		
		
		spnF << "\t{abstractBinding bl;\n";
		for (size_t it=0; it < MyGspn.colVars.size(); ++it) {
			if( MyGspn.transitionStruct[t].varDomain.count(it)==0){
				spnF<< "\tbl.P->" << MyGspn.colVars[it].name<<".mult = -1;\n";
			}
		}
		spnF << "\tdo{\n";
		if(MyGspn.transitionStruct[t].guard.compare("")==0)spnF << "\t\t{\n";
		else spnF << "\t\tif(" << MyGspn.transitionStruct[t].guard << "){\n";
		spnF << "\t\t\tbl.idcount = Transition["<<t<<"].bindingList.size();\n";
		spnF << "\t\t\tTransition["<<t<<"].bindingList.push_back( bl );\n";
		spnF << "\t\t}\n\t}while(bl.next());}\n";
		
	}
}

void Gspn_Reader::writeVariable(ofstream & spnF){
	for (size_t v = 0 ; v < MyGspn.colVars.size(); v++) {
		spnF << "\t" << MyGspn.colDoms[MyGspn.colVars[v].type].cname();
		spnF << " " << MyGspn.colVars[v].name << ";\n";
	}
	
}

void Gspn_Reader::WriteFile(parameters& P){
	
	vector<string> placeNames(MyGspn.pl);
	for (vector<place>::const_iterator plit = MyGspn.placeStruct.begin();
		 plit!= MyGspn.placeStruct.end(); ++plit) {
		int k = MyGspn.PlacesId[plit->name];
		placeNames[k] = plit->name;
	}
	
	
	
	string Pref = P.tmpPath;
	string loc;
	
	//loc = Pref + "../SOURCES/Cosmos/spn.cpp";
	loc = Pref + "/spn.cpp";
	//loc= "/Users/barbot/Documents/Cosmos/SOURCES/Cosmos/spn.cpp";
	ofstream SpnCppFile(loc.c_str(), ios::out | ios::trunc);
	// ouverture en écriture avec effacement du SpnCppFile ouvert
	//cout << loc << endl;
	string headerloc = Pref + "/markingImpl.hpp";
	ofstream header(headerloc.c_str(), ios::out | ios::trunc);
	header << "#ifndef _MarkingImpl_HPP" << endl;
	header << "#define    _MarkingImpl_HPP" << endl;
	
	//SpnCppFile << "#include \"spn_orig.hpp\"" << endl;
	SpnCppFile << "#include \"spn.hpp\"" << endl;
	
	
	//------------- Writing constant--------------------------------------------
	for (map<string,double>::iterator it= MyGspn.RealConstant.begin();
		 it!= MyGspn.RealConstant.end() ; it++) {
		SpnCppFile << "\tconst double "<<it->first<<"="<<it->second << ";" << endl;
	}
	for (vector<place>::const_iterator plit = MyGspn.placeStruct.begin();
		 plit!= MyGspn.placeStruct.end(); ++plit) {
		int k = MyGspn.PlacesId[plit->name];
		SpnCppFile << "\tconst int _nb_Place_"<< plit->name << "=" << k << ";" << endl;
	}
	
	
	
	SpnCppFile << "#include \"lumpingfun.cpp\"" << endl;
	SpnCppFile << "#include <iostream>" << endl;
	
	//------------- Writing Marking type and header ----------------------------
	writeMarkingClasse(SpnCppFile,header,P);
	header << "#endif" << endl;
	header.close();
	
	//------------- Writing Color name -----------------------------------------
	
	for (vector<colorClass>::const_iterator it = MyGspn.colClasses.begin();
		 it != MyGspn.colClasses.end(); ++it ) {
		SpnCppFile << "const char *Color_"<< it->name << "_names[Color_" << it->name << "_Total] = {\n";
		for (vector<color>::const_iterator it2 = it->colors.begin();
			 it2 != it->colors.end(); ++it2 ) {
			SpnCppFile << "\"" << it2->name << "\",";
		}
		SpnCppFile << "\n};\n";
	}
	
	
	//--------------- Writing implementation of SPN ----------------------------
	SpnCppFile << "SPN::SPN():" << endl;
	SpnCppFile << "pl(" << MyGspn.pl << "), ";
	SpnCppFile << "tr(" << MyGspn.tr << "), ";
	SpnCppFile << "ParamDistr(3), TransitionConditions(" << MyGspn.tr <<",0){" << endl;
	SpnCppFile << "    Path =\"" << MyGspn.Path << "\";" << endl;
	
	writeEnabledDisabled(SpnCppFile);
	
	if(P.localTesting){
		SpnCppFile << "\tsetConditionsVector();"<< endl;
		SpnCppFile << "\tinitTransitionConditions = TransitionConditions;" << endl;
	}
	
	
	SpnCppFile << "    vector <spn_trans> t(" << MyGspn.tr << ");" << endl;
	SpnCppFile << "    Transition = t;" << endl;
	
	
	SpnCppFile << "    vector <spn_place> p(" << MyGspn.pl << ");" << endl;
	SpnCppFile << "    Place = p;" << endl;
	
	
	
	for (vector<place>::const_iterator plit = MyGspn.placeStruct.begin();
		 plit!= MyGspn.placeStruct.end(); ++plit) {
		int k = MyGspn.PlacesId[plit->name];
		SpnCppFile << "    Place[" << k << "].Id =" << k << ";" << endl;
		if(P.StringInSpnLHA){
			SpnCppFile << "    Place[" << k << "].label =\" " << plit->name << "\";" << endl;
		}
	}
	
	writeTransition(SpnCppFile,P.StringInSpnLHA);
	
	
	//-------------- Rare Event -----------------
	if(P.RareEvent || P.computeStateSpace>0){
		SpnCppFile << "    Msimple();" << endl;
		SpnCppFile << "vector <double> Rate_Table_init (tr);" << endl;
		SpnCppFile << "Rate_Table = Rate_Table_init;" << endl;
		SpnCppFile << "Origine_Rate_Table = Rate_Table_init;" << endl;
	}
	//------------- /Rare Event -----------------
	
	SpnCppFile << "}\n" << endl;
	
	
	SpnCppFile << "bool SPN::IsEnabled(size_t t, const abstractBinding& b)const {" << endl;
	if(P.localTesting){
		SpnCppFile << "\treturn (TransitionConditions[t]==0);" << endl;
	} else {
		SpnCppFile << "    switch(t){" << endl;
		for (size_t t = 0; t < MyGspn.tr; t++) {
			SpnCppFile << "     case " << t << ":" << endl;
			for (size_t p = 0; p < MyGspn.pl; p++) {
				if (MyGspn.inArcs[t][p] > 0) {
					
					if (MyGspn.inArcsStr[t][p] == " ")
						SpnCppFile << "    if (!(contains(Marking.P->_PL_" << placeNames[p] <<" , " << MyGspn.inArcs[t][p] << "))) return false;" << endl;
					
					else {
						SpnCppFile << "    if ( !(" << MyGspn.inArcsStr[t][p] << " < 1)) " << endl;
						SpnCppFile << "        if (!(contains(Marking.P->_PL_" << placeNames[p] <<" , " << MyGspn.inArcsStr[t][p] << "))) return false;" << endl;
					}
				}
				if (MyGspn.inhibArcs[t][p] > 0) {
					
					if (MyGspn.inhibArcsStr[t][p] == " ")
						SpnCppFile << "    if (Marking.P->_PL_" << placeNames[p] <<" >= " << MyGspn.inhibArcs[t][p] << ") return false;" << endl;
					
					else {
						SpnCppFile << "    if ( !(" << MyGspn.inhibArcsStr[t][p] << " < 1) ) " << endl;
						SpnCppFile << "        if (contain(Marking.P->_PL_" << placeNames[p] <<" , " << MyGspn.inhibArcsStr[t][p] << ")) return false;" << endl;
						
					}
				}
			}
			SpnCppFile << "    return true;" << endl;
			//SpnCppFile << "       return IsEnabled_t" << i << "();" << endl;
			SpnCppFile << "       break;" << endl;
		}
		SpnCppFile << "   }" << endl;
	}
	SpnCppFile << "}\n" << endl;
	
	
	SpnCppFile << "void SPN::fire(size_t t, const abstractBinding& b){" << endl;
	SpnCppFile << "\tlastTransition = t;" << endl;
	SpnCppFile << "\tswitch(t){" << endl;
	for (size_t t = 0; t < MyGspn.tr; t++) {
		SpnCppFile << "\t\tcase " << t << ": {" << endl;
		//SpnCppFile << "       unfire_t" << t << "();" << endl;
		for (size_t p = 0; p < MyGspn.pl; p++) {
			if (MyGspn.inArcs[t][p] > 0) {
				if (MyGspn.inArcsStr[t][p] == " "){
					int decrement = MyGspn.inArcs[t][p];
					if(P.localTesting)for (size_t t2 = 0; t2 < MyGspn.tr; t2++) {
						if (MyGspn.inArcs[t2][p] > 0) {
							if (MyGspn.inArcsStr[t2][p] == " "){
								int seuil = MyGspn.inArcs[t2][p];
								if(seuil-decrement >0){
									SpnCppFile << "\t\t\tif(Marking.P->_PL_" << placeNames[p] <<" < "<< seuil+decrement;
									SpnCppFile << " && Marking.P->_PL_" << placeNames[p] <<" >=" << seuil <<")";
									SpnCppFile << "TransitionConditions["<< t2 <<"]++ ;" << endl;
								} else
									SpnCppFile << "\t\t\tif(Marking.P->_PL_" << placeNames[p] <<" < "<< seuil+decrement << ")TransitionConditions["<< t2 <<"]++ ;" << endl;
							}else {
								string seuil = MyGspn.inArcsStr[t2][p];
								SpnCppFile << "\t\t\tif(Marking.P->_PL_" << placeNames[p] <<" >= "<< seuil;
								SpnCppFile << " && Marking.P->_PL_" << placeNames[p] <<" < " << seuil <<"+"<< decrement <<")";
								SpnCppFile << "TransitionConditions["<< t2 <<"]++ ;" << endl;
							}
						}
						
						if (MyGspn.inhibArcs[t2][p] > 0) {
							if (MyGspn.inhibArcsStr[t2][p] == " "){
								int seuil = MyGspn.inhibArcs[t2][p];
								if(seuil-decrement >0){
									SpnCppFile << "\t\t\tif(Marking.P->_PL_" << placeNames[p] <<" < "<< seuil+decrement;
									SpnCppFile << " && Marking.P->_PL_" << placeNames[p] <<" >=" << seuil <<")";
									SpnCppFile << "TransitionConditions["<< t2 <<"]-- ;" << endl;
								} else
									SpnCppFile << "\t\t\tif(Marking.P->_PL_" << placeNames[p] <<" < "<< seuil+decrement << ")TransitionConditions["<< t2 <<"]-- ;" << endl;
							}else {
								string seuil = MyGspn.inhibArcsStr[t2][p];
								SpnCppFile << "\t\t\tif(Marking.P->_PL_" << placeNames[p] <<" < "<< seuil <<"+"<< decrement;
								SpnCppFile << " && Marking.P->_PL_" << placeNames[p] <<" >=" << seuil <<")";
								SpnCppFile << "TransitionConditions["<< t2 <<"]-- ;" << endl;
							}
						}
					}
					SpnCppFile << "\t\t\tMarking.P->_PL_" << placeNames[p] <<" -= " << decrement << ";"<< endl;
				} else {
					string decrement = MyGspn.inArcsStr[t][p];
					if(P.localTesting)for (size_t t2 = 0; t2 < MyGspn.tr; t2++) {
						if (MyGspn.inArcs[t2][p] > 0) {
							if (MyGspn.inArcsStr[t2][p] == " "){
								int seuil = MyGspn.inArcs[t2][p];
								SpnCppFile << "\t\t\tif(Marking.P->_PL_" << placeNames[p] <<" < "<< seuil <<"+"<< decrement;
								SpnCppFile << " && Marking.P->_PL_" << placeNames[p] <<" >=" << seuil <<")";
								SpnCppFile << "TransitionConditions["<< t2 <<"]++ ;" << endl;
							}else {
								string seuil = MyGspn.inArcsStr[t2][p];
								SpnCppFile << "\t\t\tif(Marking.P->_PL_" << placeNames[p] <<" < "<< seuil <<"+" << decrement;
								SpnCppFile << " && Marking.P->_PL_" << placeNames[p] <<" >=" << seuil <<")";
								SpnCppFile << "TransitionConditions["<< t2 <<"]++ ;" << endl;
							}
						}
						
						if (MyGspn.inhibArcs[t2][p] > 0) {
							if (MyGspn.inhibArcsStr[t2][p] == " "){
								int seuil = MyGspn.inhibArcs[t2][p];
								SpnCppFile << "\t\t\tif(Marking.P->_PL_" << placeNames[p] <<" < "<< seuil <<"+"<<decrement;
								SpnCppFile << " && Marking.P->_PL_" << placeNames[p] <<" >=" << seuil <<")";
								SpnCppFile << "TransitionConditions["<< t2 <<"]-- ;" << endl;
							}else {
								string seuil = MyGspn.inhibArcsStr[t2][p];
								SpnCppFile << "\t\t\tif(Marking.P->_PL_" << placeNames[p] <<" < "<< seuil << "+" <<decrement;
								SpnCppFile << " && Marking.P->_PL_" << placeNames[p] <<" >=" << seuil <<")";
								SpnCppFile << "TransitionConditions["<< t2 <<"]-- ;" << endl;
							}
						}
					}
					SpnCppFile << "\t\t\tMarking.P->_PL_" << placeNames[p] <<" -= " << decrement << ";"<< endl;
				}
			}
			
			if (MyGspn.outArcs[t][p] > 0) {
				if (MyGspn.outArcsStr[t][p] == " "){
					int increment = MyGspn.outArcs[t][p];
					if(P.localTesting)for (size_t t2 = 0; t2 < MyGspn.tr; t2++) {
						if (MyGspn.inArcs[t2][p] > 0) {
							if (MyGspn.inArcsStr[t2][p] == " "){
								int seuil = MyGspn.inArcs[t2][p];
								if(seuil-increment >0){
									SpnCppFile << "\t\t\tif(Marking.P->_PL_" << placeNames[p] <<" < "<< seuil;
									SpnCppFile << " && Marking.P->_PL_" << placeNames[p] <<"+"<<increment<<" >=" << seuil <<")";
									SpnCppFile << "TransitionConditions["<< t2 <<"]-- ;" << endl;
								} else
									SpnCppFile << "\t\t\tif(Marking.P->_PL_" << placeNames[p] <<" < "<< seuil << ")TransitionConditions["<< t2 <<"]-- ;" << endl;
							}else {
								string seuil = MyGspn.inArcsStr[t2][p];
								SpnCppFile << "\t\t\tif(Marking.P->_PL_" << placeNames[p] <<" < "<< seuil;
								SpnCppFile << " && Marking.P->_PL_" << placeNames[p] <<"+"<<increment<<" >=" << seuil <<")";
								SpnCppFile << "TransitionConditions["<< t2 <<"]-- ;" << endl;
							}
						}
						
						if (MyGspn.inhibArcs[t2][p] > 0) {
							if (MyGspn.inhibArcsStr[t2][p] == " "){
								int seuil = MyGspn.inhibArcs[t2][p];
								if(seuil-increment >0){
									SpnCppFile << "\t\t\tif(Marking.P->_PL_" << placeNames[p] <<" < "<< seuil;
									SpnCppFile << " && Marking.P->_PL_" << placeNames[p] <<"+"<<increment<<" >=" << seuil <<")";
									SpnCppFile << "TransitionConditions["<< t2 <<"]++ ;" << endl;
								} else
									SpnCppFile << "\t\t\tif(Marking.P->_PL_" << placeNames[p] <<" < "<< seuil << ")TransitionConditions["<< t2 <<"]++ ;" << endl;
							}else {
								string seuil = MyGspn.inhibArcsStr[t2][p];
								SpnCppFile << "\t\t\tif(Marking.P->_PL_" << placeNames[p] <<" < "<< seuil;
								SpnCppFile << " && Marking.P->_PL_" << placeNames[p] <<"+"<<increment<<" >=" << seuil <<")";
								SpnCppFile << "TransitionConditions["<< t2 <<"]++ ;" << endl;
							}
						}
					}
					SpnCppFile << "\t\t\tMarking.P->_PL_" << placeNames[p] <<" += " << increment << ";"<< endl;
				} else {
					string increment = MyGspn.outArcsStr[t][p];
					if(P.localTesting)for (size_t t2 = 0; t2 < MyGspn.tr; t2++) {
						if (MyGspn.inArcs[t2][p] > 0) {
							if (MyGspn.inArcsStr[t2][p] == " "){
								int seuil = MyGspn.inArcs[t2][p];
								SpnCppFile << "\t\t\tif(Marking.P->_PL_" << placeNames[p] <<" < "<< seuil;
								SpnCppFile << " && Marking.P->_PL_" << placeNames[p] <<"+"<<increment<<" >=" << seuil <<")";
								SpnCppFile << "TransitionConditions["<< t2 <<"]-- ;" << endl;
							}else {
								string seuil = MyGspn.inArcsStr[t2][p];
								SpnCppFile << "\t\t\tif(Marking.P->_PL_" << placeNames[p] <<" < "<< seuil;
								SpnCppFile << " && Marking.P->_PL_" << placeNames[p] <<"+"<<increment<<" >=" << seuil <<")";
								SpnCppFile << "TransitionConditions["<< t2 <<"]-- ;" << endl;
							}
						}
						
						if (MyGspn.inhibArcs[t2][p] > 0) {
							if (MyGspn.inhibArcsStr[t2][p] == " "){
								int seuil = MyGspn.inhibArcs[t2][p];
								SpnCppFile << "\t\t\tif(Marking.P->_PL_" << placeNames[p] <<" < "<< seuil;
								SpnCppFile << " && Marking.P->_PL_" << placeNames[p] <<"+"<<increment<<" >=" << seuil <<")";
								SpnCppFile << "TransitionConditions["<< t2 <<"]++ ;" << endl;
							}else {
								string seuil = MyGspn.inhibArcsStr[t2][p];
								SpnCppFile << "\t\t\tif(Marking.P->_PL_" << placeNames[p] <<" < "<< seuil;
								SpnCppFile << " && Marking.P->_PL_" << placeNames[p] <<"+"<<increment<<" >=" << seuil <<")";
								SpnCppFile << "TransitionConditions["<< t2 <<"]++ ;" << endl;
							}
						}
					}
					SpnCppFile << "\t\t\tMarking.P->_PL_" << placeNames[p] <<" += " << increment << ";"<< endl;
				}
			}
		}
		//
		SpnCppFile << "       break;" << endl;
		SpnCppFile << "     } " << endl;
	}
	SpnCppFile << "\t}" << endl;
	SpnCppFile << "}" << endl;
	
	SpnCppFile << "void SPN::unfire(size_t t ,const abstractBinding& b){" << endl;
	if(P.RareEvent || P.computeStateSpace){
		SpnCppFile << "   switch(t){" << endl;
		for (size_t t = 0; t < MyGspn.tr; t++) {
			SpnCppFile << "     case " << t << ": {" << endl;
			//SpnCppFile << "       unfire_t" << t << "();" << endl;
			for (size_t p = 0; p < MyGspn.pl; p++) {
				if (MyGspn.inArcs[t][p] > 0) {
					if (MyGspn.inArcsStr[t][p] == " ")
						SpnCppFile << "    Marking.P->_PL_" << placeNames[p] <<" += " << MyGspn.inArcs[t][p] << ";" << endl;
					else
						SpnCppFile << "    Marking.P->_PL_" << placeNames[p] <<" += " << MyGspn.inArcsStr[t][p] << ";" << endl;
				}
				
				if (MyGspn.outArcs[t][p] > 0) {
					if (MyGspn.outArcsStr[t][p] == " ")
						SpnCppFile << "    Marking.P->_PL_" << placeNames[p] <<" -= " << MyGspn.outArcs[t][p] << ";" << endl;
					else
						SpnCppFile << "    Marking.P->_PL_" << placeNames[p] <<" -= " << MyGspn.outArcsStr[t][p] << ";" << endl;
				}
			}
			//
			SpnCppFile << "       break;" << endl;
			SpnCppFile << "     } " << endl;
		}
		
		SpnCppFile << "   }" << endl;
	}
	SpnCppFile << "}\n" << endl;
	
	SpnCppFile << "const set<int>* SPN::PossiblyEn()const {" << endl;
	if (false && P.localTesting)SpnCppFile << "\treturn &newlyEnabled;" << endl;
	else SpnCppFile << "\treturn &(PossiblyEnabled[lastTransition]);" << endl;
	SpnCppFile << "}" << endl;
	
	SpnCppFile << "const set<int>* SPN::PossiblyDis()const {" << endl;
	if (false &&P.localTesting)SpnCppFile << "\treturn &newlyDisabled;" << endl;
	else SpnCppFile << "\treturn &(PossiblyDisabled[lastTransition]);" << endl;
	SpnCppFile << "}" << endl;
	
	SpnCppFile << "const set<int>* SPN::FreeMarkingDependant()const {" << endl;
	SpnCppFile << "\treturn &(FreeMarkDepT[lastTransition]);" << endl;
	SpnCppFile << "}" << endl;
	
	
	SpnCppFile << "void SPN::setConditionsVector(){" << endl;
	if(P.localTesting)for (size_t t = 0; t < MyGspn.tr; t++) {
		SpnCppFile << "\tTransitionConditions["<< t <<"]=0;" << endl;
		for (size_t p = 0; p < MyGspn.pl; p++) {
			if (MyGspn.inArcs[t][p] > 0) {
				
				if (MyGspn.inArcsStr[t][p] == " ")
					SpnCppFile << "\tif (Marking.P->_PL_" << placeNames[p] <<" < " << MyGspn.inArcs[t][p] << ")TransitionConditions["<< t <<"]++;" << endl;
				
				else {
					SpnCppFile << "\tif ( " << MyGspn.inArcsStr[t][p] << "> 0) " << endl;
					SpnCppFile << "\t\tif (Marking.P->_PL_" << placeNames[p] <<" < " << MyGspn.inArcsStr[t][p] << ")TransitionConditions["<< t <<"]++;" << endl;
				}
			}
			if (MyGspn.inhibArcs[t][p] > 0) {
				
				if (MyGspn.inhibArcsStr[t][p] == " ")
					SpnCppFile << "\tif (Marking.P->_PL_" << placeNames[p] <<" >= " << MyGspn.inhibArcs[t][p] << ")TransitionConditions["<< t <<"]++;" << endl;
				
				else {
					SpnCppFile << "\tif ( " << MyGspn.inhibArcsStr[t][p] << " > 0 ) " << endl;
					SpnCppFile << "\t\tif (Marking.P->_PL_" << placeNames[p] <<" >= " << MyGspn.inhibArcsStr[t][p] << ")TransitionConditions["<< t <<"]++;" << endl;
					
				}
			}
		}
    }
	
	SpnCppFile << "}" << endl;
	
	SpnCppFile << "void SPN::GetDistParameters(size_t t, const abstractBinding&)const {" << endl;
	//-------------- /Rare Event -------------------------
	SpnCppFile << "   switch(t){" << endl;
	for (size_t t = 0; t < MyGspn.tr; t++) {
		if (MyGspn.transitionStruct[t].type == Timed) {
			SpnCppFile << "     case " << t << ": {" << endl;
			//SpnCppFile << "       vector<double> P(" << MyGspn.Dist[t].Param.size() << ");" << endl;
			if (MyGspn.transitionStruct[t].singleService)
				for (size_t i = 0; i < MyGspn.transitionStruct[t].dist.Param.size(); i++) {
					
					SpnCppFile << "       ParamDistr[" << i << "]= ( double ) " << MyGspn.transitionStruct[t].dist.Param[i] << ";" << endl;
				} else {
					SpnCppFile << "       double EnablingDegree = INT_MAX; " << endl;
					bool AtLeastOneMarkDepArc = false;
					for (size_t p = 0; p < MyGspn.pl; p++)
						if (MyGspn.inArcs[t][p] > 0) {
							if (MyGspn.inArcsStr[t][p] == " ")
								SpnCppFile << "       EnablingDegree=min(floor(Marking.P->_PL_" << placeNames[p] <<"/(double)(" << MyGspn.inArcs[t][p] << ")),EnablingDegree);" << endl;
							else {
								AtLeastOneMarkDepArc = true;
								SpnCppFile << "       if(" << MyGspn.inArcsStr[t][p] << ">0)" << endl;
								SpnCppFile << "              EnablingDegree=min(floor(Marking.P->_PL_" << placeNames[p] <<"/(double)(" << MyGspn.inArcsStr[t][p] << ")),EnablingDegree);" << endl;
							}
							
						}
					if (AtLeastOneMarkDepArc) {
						if (MyGspn.transitionStruct[t].nbServers >= INT_MAX) {
							SpnCppFile << "       if(EnablingDegree < INT_MAX ) ParamDistr[0] = EnablingDegree * ( " << MyGspn.transitionStruct[t].dist.Param[0] << " );" << endl;
							SpnCppFile << "       else ParamDistr[0] = " << MyGspn.transitionStruct[t].dist.Param[0] << " ;" << endl;
						} else {
							SpnCppFile << "       if(EnablingDegree < INT_MAX ) P[0] = min(EnablingDegree , " << MyGspn.transitionStruct[t].nbServers << " ) * ( " << MyGspn.transitionStruct[t].dist.Param[0] << " );" << endl;
							SpnCppFile << "       else ParamDistr[0] = " << MyGspn.transitionStruct[t].dist.Param[0] << " ;" << endl;
						}
					} else {
						if (MyGspn.transitionStruct[t].nbServers >= INT_MAX)
							SpnCppFile << "        ParamDistr[0] = EnablingDegree  * ( " << MyGspn.transitionStruct[t].dist.Param[0] << " );" << endl;
						else
							SpnCppFile << "        ParamDistr[0] = min(EnablingDegree , " << MyGspn.transitionStruct[t].nbServers << " ) * ( " << MyGspn.transitionStruct[t].dist.Param[0] << " );" << endl;
					}
				}
			//SpnCppFile << "       return P;" << endl;
			SpnCppFile << "       break;" << endl;
			SpnCppFile << "     }" << endl;
		}
	}
	SpnCppFile << "   }" << endl;
	SpnCppFile << "}\n" << endl;
	
	
	/////////////////////////////////////////
	
	SpnCppFile << "double SPN::GetPriority(size_t t)const {" << endl;
	SpnCppFile << "   switch(t){" << endl;
	for (size_t t = 0; t < MyGspn.tr; t++)
		if(MyGspn.transitionStruct[t].priority != "1"){
		
		SpnCppFile << "     case " << t << ": {" << endl;
		
		SpnCppFile << "       return (double)" << MyGspn.transitionStruct[t].priority << ";" << endl;
		SpnCppFile << "       break;" << endl;
		SpnCppFile << "     } " << endl;
		
	}
	SpnCppFile << "\tdefault:" << endl;
	SpnCppFile << "\t\treturn (double)1;" << endl;
	SpnCppFile << "   }" << endl;
	SpnCppFile << "}\n" << endl;
	
	
	/////////////////////////////////////////
	SpnCppFile << "double SPN::GetWeight(size_t t)const{" << endl;
	SpnCppFile << "   switch(t){" << endl;
	for (size_t t = 0; t < MyGspn.tr; t++)
		if(MyGspn.transitionStruct[t].weight != "1"){
		
		SpnCppFile << "     case " << t << ":" << endl;
		
		SpnCppFile << "       return (double)" << MyGspn.transitionStruct[t].weight << ";" << endl;
		SpnCppFile << "       break;" << endl;
		
	}
	SpnCppFile << "\tdefault:" << endl;
	SpnCppFile << "\t\treturn (double)1;" << endl;
	SpnCppFile << "   }" << endl;
	SpnCppFile << "}\n" << endl;
	
	SpnCppFile << "void SPN::Msimple(){"<<endl;
	SpnCppFile << "\tvector<int> tab;"<<endl;
	for(map<string,int>::iterator it=MyGspn.PlacesId.begin(); it != MyGspn.PlacesId.end(); it++){
		if(it->first.substr(0,3).compare("RE_") == 0)
			SpnCppFile << "\t\ttab.push_back("<< it->second << ");" << endl;
	}
	SpnCppFile << "\tMsimpletab = tab;\n}"<< endl;
	
	SpnCppFile << "void SPN::reset() {"<< endl;
	SpnCppFile << "\tMarking.resetToInitMarking();"<< endl;
	SpnCppFile << "\tTransitionConditions = initTransitionConditions;"<< endl;
	SpnCppFile << "}"<< endl<< endl;
	
	/*
	SpnCppFile << "double SPN::min(double x1, double x2) {"<< endl;
	SpnCppFile << "\tif (x1 < x2) return x1;"<< endl;
	SpnCppFile << "\telse return x2;"<< endl;
	SpnCppFile << "}"<< endl<< endl;
	
	SpnCppFile << "double SPN::max(double x1, double x2) {"<< endl;
	SpnCppFile << "\tif (x1 > x2) return x1;"<< endl;
	SpnCppFile << "\telse return x2;"<< endl;
	SpnCppFile << "}"<< endl<< endl;
	*/
	
	SpnCppFile.close();
	
}
