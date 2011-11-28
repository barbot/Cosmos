#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include <sstream>
#include <unistd.h>

#include <boost/program_options.hpp>

//#include "directsim.hpp"
#include "./LhaParser/Lha-Reader.hpp"
#include "./GspnParser/Gspn-Reader.hpp"
#include "server.hpp"

#ifdef __APPLE__
#include <mach-o/dyld.h>

void FindPathMac(SimParam& P) {
	
	char path[1024];
	uint32_t size = sizeof(path);
	if (_NSGetExecutablePath(path, &size) != 0){
		printf("buffer too small; need size %u\n", size);
		exit(0);
	}
	
	P.Path=path;
	std::string::size_type t = P.Path.find_last_of("/");
	P.Path = P.Path.substr(0, t);
	P.Path.append("/");
	//cout << "Unknow Cosmos directory" << endl;
	//exit(1);
}


#define FindPath FindPathMac
#elif __linux__
void FindPathLinux(SimParam& P) {
	
	char path[1024];
	char link[512];
	sprintf(link, "/proc/%d/exe", getpid());
	int sz = readlink(link, path, 1024);
	if (sz >= 0) {
		path[sz] = 0;
		P.Path = path;
		std::string::size_type t = P.Path.find_last_of("/");
		P.Path = P.Path.substr(0, t);
		P.Path.append("/");
	}
}

#define FindPath FindPathLinux
#else
#error "Operating system not supported"
#endif

using namespace std;
namespace po = boost::program_options;

void ViewParameters(SimParam& P) {
	cout << "Confidence interval width:      " << P.Width << endl;
	cout << "Confidence interval level:      " << P.Level << endl;
	cout << "Maximum number of trajectories: " << P.MaxRuns << endl;
	cout << "Batch size:                     " << P.Batch << endl;
	cout << "Rare Event:                     " << P.RareEvent << endl;
	cout << "Double Important Sampling:      " << P.DoubleIS << endl;
	cout << "Number of parallel execution:   " << P.Njob << endl;
	cout << "Read GML file as input:         " << P.GMLinput << endl;  
}



bool ParseBuild( SimParam& P) {
	Gspn_Reader gReader;
	
	/*if(P.GMLinput){
		GspnFileName.append(".gml");
	}else {
		GspnFileName.append(".gspn");
	}*/
	
	cout << "Start Parsing " << P.PathGspn << endl;
	int parseresult;
	
	if(P.GMLinput){
		parseresult = gReader.parse_gml_file(P.PathGspn);
	}else {
		parseresult = gReader.parse_file(P.PathGspn);
	}
	
	
	if (!parseresult) {
		gReader.MyGspn.Path = P.PathGspn.substr(0, P.PathGspn.find_last_of("."));
		gReader.WriteFile(P.Path);
	} else {
		Gspn_Reader gr;
		gReader = gr;
		return false;
	}
	
	
	Lha_Reader lReader;
	
	
	lReader.MyLha.TransitionIndex = gReader.MyGspn.TransId;
	lReader.MyLha.PlaceIndex = gReader.MyGspn.PlacesId;
	
	
	Gspn_Reader gr;
	gReader = gr;
	
	
	/*if(P.GMLinput){
		LhaFileName.append(".lha.gml");
	}else {
		LhaFileName.append(".lha");
	}*/
	
	cout << "Start Parsing " << P.PathLha << endl;
	
	if(P.GMLinput){
		parseresult = lReader.parse_gml_file(P.PathLha);
	}else {
		parseresult = lReader.parse_file(P.PathLha);
	}
	
	if (!parseresult) {
		lReader.WriteFile(P.Path);
		
	} else {
		Lha_Reader lr;
		lr = lReader;
		
		return false;
	}
	Lha_Reader lr;
	lr = lReader;
	cout << "Parsing OK.\n" << endl;
	cout << "Start building ... " << endl;
	string cmd;
	
	cmd = "make -s -C " + P.Path + ".. sim";
	if (system(cmd.c_str())) return false;
	
	cout << "Building OK.\n" << endl;
	
	return true;
}

void DirectSim(SimParam& P) {
	
	if (ParseBuild(P)) {
		
		LauchServer(P);
		
	}
}

void Command(string str, SimParam& P) {
	
	if (str == "sim") {
		
		cout << "Type the path of the files: ";
		string filename;
		getline(cin, filename);
		DirectSim( P);
	}
	else if (str == "width") {
		string st;
		cout << "Type the confidence width: ";
		getline(cin, st);
		
		double y = StrToDbl(st);
		if (y <= 0 || y >= 1)
			cout << "width should be in ]0,1[" << endl;
		else {
			P.Width = y;
			cout << "New width: " << y << endl;
		}
		
	} else if (str == "level") {
		string st;
		cout << "Type the confidence level: ";
		getline(cin, st);
		
		double y = StrToDbl(st);
		if (y <= 0 || y >= 1)
			cout << "level should be in ]0,1[" << endl;
		else {
			P.Level = y;
			cout << "New level: " << y << endl;
		}
		
	} else if (str == "batch") {
		string st;
		cout << "Type batch size: ";
		getline(cin, st);
		
		int y = StrToInt(st);
		if (y > P.MaxRuns || y < 1)
			cout << "batch size  should be in [1,maxpaths]" << endl;
		else {
			P.Batch = y;
			cout << "New batch size: " << y << endl;
		}
	} else if (str == "maxpaths") {
		string st;
		cout << "Type maximum number of paths: ";
		getline(cin, st);
		
		long int y = StrToLongInt(st);
		if (y < P.Batch)
			cout << "maxpaths  should be >  Batch size" << endl;
		else {
			P.MaxRuns = y;
			cout << "New maxpaths: " << y << endl;
		}
	} else if (str == "rareevent") {
		string st;
		cout << "Type true for Rare Event acceleration: ";
		getline(cin, st);
		
		if (st == "true")
			P.RareEvent = true;
		else {
			P.RareEvent = false;
		}
		
	} else if (str == "GMLinput") {
		string st;
		cout << "Type true to use GML input: ";
		getline(cin, st);
		
		if (st == "true")
			P.GMLinput = true;
		else {
			P.GMLinput = false;
		}
		
	} else if (str == "doubleIS") {
		string st;
		cout << "Type true for double Important Sampling: ";
		getline(cin, st);
		
		if (st == "true") {
			P.RareEvent = true;
			P.DoubleIS = true;
		} else {
			P.DoubleIS = false;
			P.RareEvent = true;
		}
	} else if (str == "Njob") {
		string st;
		cout << "Number of parallel execution: ";
		getline(cin, st);
		P.Njob = StrToInt(st);
		
	} else if (str == "params") {
		
		ViewParameters(P);
	} else if (str == "help") {
		cout << "Allowed commands\n" << endl;
		cout << "\tsim" << endl;
		cout << "\twidth" << endl;
		cout << "\tlevel" << endl;
		cout << "\tbatch" << endl;
		cout << "\tmaxpaths" << endl;
		cout << "\trareevent" << endl;
		cout << "\tdoubleIS" << endl;
		cout << "\tparams" << endl;
		cout << "\thelp" << endl;
		cout << "\tstop" << endl;
		cout << "\tNjob" << endl;
		cout << "\tGMLinput" << endl;
	} else
		cout << "Unknown command, for more information type 'help':\n" << endl;
}


void LoadSimParam(SimParam& P) {
	P.Level = 0.99;
	P.Width = 0.001;
	P.Batch =   1000;
	P.MaxRuns = 2000000;
	P.RareEvent = false;
	P.DoubleIS = false;
	P.Njob = 1;
	P.GMLinput = false;
}

int main2(int argc, char** argv) {
	SimParam P;
	
	LoadSimParam(P);
	
	
	string st = argv[0];
	if (st == "./CosmosGPP") P.Path = "";
	else if (st == "CosmosGPP")FindPath(P);
	else P.Path.assign(st.begin(), st.end() - 9);
	
	if (argc > 1) {
		st = argv[1];
		DirectSim(P);
		return (EXIT_SUCCESS);
	}
	
	
	while (true) {
		string str;
		cout << "COSMOS: ";
		getline(cin, str);
		if (str == "stop" || cin.eof())
			break;
		else Command(str, P);
		
	}
	
	return (EXIT_SUCCESS);
}

int main(int argc, char** argv) {
	SimParam P;
	
	LoadSimParam(P);
	
	// Declare the supported options.
	po::options_description desc("Allowed options");
	desc.add_options()
    ("help", "produce help message")
	("GspnFile",po::value(&(P.PathGspn)),"Path to the Gspn")
	("LhaFile",po::value(&(P.PathLha)),"Path to the Lha")
	("GMLinput,g",po::bool_switch(&(P.GMLinput)),"Change input file format")
	("RareEvent,r",po::bool_switch(&(P.RareEvent)),"Use Rare Event acceleration")
	("DoubleIS,d",po::bool_switch(&(P.DoubleIS)),"Use Rare Event acceleration with double Important Sampling")
	("level",po::value(&(P.Level)),"Set Confidence interval level")
	("width",po::value(&(P.Width)),"Set Confidence interval width")
	("batch",po::value(&(P.Batch)),"Set Batch Size")
	("max-run",po::value(&(P.MaxRuns)),"Set the maximum number of run")
	("njob",po::value(&(P.Njob)),"Set the number of parallel simulation")	
	;
	
	po::positional_options_description p;
	p.add("GspnFile", 1);
	p.add("LhaFile", 2);
	
	po::variables_map vm;
	po::store(po::command_line_parser(argc, argv).
			  options(desc).positional(p).run(), vm);
	//po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);    
	
	if (vm.count("help")) {
		cout << desc << "\n";
		return 1;
	}
	
	//ViewParameters(P);
	
	/*if (vm.count("compression")) {
		cout << "Compression level was set to " 
		<< vm["compression"].as<int>() << ".\n";
	} else {
		cout << "Compression level was not set.\n";
	}*/
	
	string st = argv[0];
	if (st == "./CosmosGPP") P.Path = "";
	else if (st == "CosmosGPP")FindPath(P);
	else P.Path.assign(st.begin(), st.end() - 9);
	
	DirectSim(P);
	
	
	return (EXIT_SUCCESS);
}
