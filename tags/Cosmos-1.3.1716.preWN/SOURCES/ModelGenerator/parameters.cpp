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
 * file parameters.cpp                                                         *
 * Created by Benoit Barbot on 08/02/12.                                       *
 *******************************************************************************
 */

#include "parameters.hpp"
#include <iostream>
#include <cstdlib>
#include <getopt.h>

parameters::parameters():
    verbose(2),
	updatetime(0.1),
	seed(0),
	Njob(1),

    epsilon(0.000001),
	Level(0.99),
	Width(0.001),
	Batch(1000),
	MaxRuns(2000000),

	tmpPath("tmp"),
	tmpStatus(0),
	Path(""),
	PathGspn(""),
	PathLha(""),

	RareEvent(false),
	DoubleIS(false),
	BoundedRE(0),
    horizon(100),
    BoundedContinuous(false),

	CountTrans(false),
	StringInSpnLHA(false),

	GMLinput(false),
    computeStateSpace(false),
	alligatorMode(false),

	gcccmd("g++"),
	gccflags("-O3"),

	prismPath("prism/bin/prism"),
    dataoutput(""),
	dataraw(""),
	dataPDFCDF(""),

	HaslFormulas(vector<HaslFormulasTop*>(0)),
	HaslFormulasname(vector<string>(0)),
	nbAlgebraic(0)
    //prismPath = "/import/barbot/prism-4.0.1-linux64/bin/prism";
{}

void parameters::View() {
	cout << "Confidence interval width:      " << Width << endl;
	cout << "Confidence interval level:      " << Level << endl;
	cout << "Maximum number of trajectories: " << MaxRuns << endl;
	cout << "Batch size:                     " << Batch << endl;
	cout << "Rare Event:                     " << RareEvent << endl;
	cout << "Double Important Sampling:      " << DoubleIS << endl;
	cout << "Number of parallel execution:   " << Njob << endl;
	cout << "Read GML file as input:         " << GMLinput << endl;
    for (size_t i =0; i<HaslFormulas.size() ; i++) {
        cout << "Hasl Formula " << i << ": " << HaslFormulasname[i] << endl;
    }
}

void parameters::usage(){
    cout << "usage: Cosmos [-ghsr] [-vb arg] ";
    cout << "[--alligator-mode] ";
    cout << "[--level arg] [--width arg] [--batch arg] [--max-run arg] ";
    cout << "[--set-Horizon arg] [--njob arg] ";
    cout << "gspn_file lha_file" << endl;
    
    cout << "General options:" << endl;
    cout << "\t-v,--verbose arg\tset the verbose level"<< endl;
	cout << "\t--update-time\t set the time in second beetween two updates of the display"<< endl;
    cout << "\t-h,--help \tdisplay this message" << endl;
    cout << "\t--njob    \tset the number of parralel thread"<< endl;
	cout << "\t--gppcmd  \tset the C++ compiler (default g++)"<< endl;
	cout << "\t--gppflags\tset the C++ compiler flags (default -O3)"<< endl;
    cout << "Option of simulation:" << endl;
    cout << "\t--level \tset the confidence level for the simulation (default=0.99)"<< endl;
    cout << "\t--width \tset the width of the confidence interval (default=0.001)"<< endl;
    cout << "\t--batch \tset the size of batch of simulation (default=1000)"<< endl;
    cout << "\t--max-run \tset the maximal number of run (default=2000000)" << endl;
	cout << "\t--seed \tSpecify the seed for the random generator, 0 allow to take a random value";
	
    cout << "Miscellaneous options:" << endl;
    cout << "\t-g,--gmlinput \tuse gml file format for input file"<< endl;
    cout << "\t--alligator-mode \toutput easy to parse result"<< endl;
	cout << "\t--count-transition \tAdd a Hasl formula for wich count the number of time each transition occurs"<< endl;
	cout << "\t--tmp-path arg \tPath to the temporary directory by default ./tmp/"<< endl;
	cout << "\t--bin-path arg \tPath to the binary of cosmos (guess automatically)"<< endl;
	cout << "\t--tmp-status arg \tDo not remove or do not rebuild tmp directory: 0 default->rebuild,destroy; 1->do not build;2->do not destroy;3-> do not build nor destroy"<<endl;
	cout << "\t--debug-string \tAdd transition and place name to the compile file for debuging"<< endl;
	cout << "\t-d,--outputdata \tOutput successive result in the blank separated file format"<< endl;
	cout << "\t--output-raw \tOutput the result of each trajectory in a file for debug purpose"<< endl;
	cout << "\t--output-PDFCDF \tOutput the result of CDF or PDF formula in gnuplot file format"<< endl;
	
}


void parameters::parseCommandLine(int argc, char** argv){
    int c;
    
    while (1)
    {
        static struct option long_options[] =
        {
            /* Options for the simulator*/
            {"level" , required_argument, 0, 'l'},
            {"width" , required_argument, 0, 'w'},
            {"batch" , required_argument, 0,  2 },
            {"max-run",required_argument, 0, 'm'},
			{"seed"  , required_argument, 0,  10},
            
            /* Options for the rare event engine */
            {"rareevent",   no_argument,        0, 'r'},
            {"boundedcountiniousRE",no_argument,0, 'c'},
            {"boundedRE",   required_argument,  0, 'b'},
            {"epsilon" ,    required_argument,  0, 'e'},
            {"set-Horizon", required_argument , 0,  1 },
            {"stateSpace" , no_argument ,       0, 's'},
			
            /* CosyVerif Options */
            {"gmlinput" ,      no_argument, 0, 'g' },
            {"alligator-mode", no_argument, 0, 'a' },
			
            /* Miscellaneous options */
            {"njob" , required_argument,	 0, 'n'},
			{"gppcmd",required_argument,	 0,  6 },
			{"gppflags",required_argument,	 0,  7 },
            {"verbose", required_argument,	 0, 'v'},
			{"update-time",required_argument,0, 'u'},
            {"outputdata", required_argument,0, 'd'},
			{"output-raw" , required_argument,0,  8 },
			{"output-PDFCDF",required_argument,0,11},
            {"help" , no_argument ,			 0, 'h'},
			{"count-transition", no_argument,0, 't'},
			{"debug-string", no_argument,	 0,  3 },
			{"tmp-path" , required_argument, 0,	 4 },
			{"tmp-status", required_argument,0,	 5 },
			{"bin-path", required_argument  ,0,	 9 },
            
            {0, 0, 0, 0}
        };
        /* getopt_long stores the option index here. */
        int option_index = 0;
        
        c = getopt_long (argc, argv, "ghscrb:v:d:",
                         long_options, &option_index);
        
        /* Detect the end of the options. */
        if (c == -1)
            break;
        
        switch (c)
        {
            case 'h':
                usage();
                exit(EXIT_SUCCESS);
                break;
                
            case 'v':verbose = atoi(optarg);
				if(verbose>=4)StringInSpnLHA = true;	
				break;
				
			case 'u':updatetime=atof(optarg);	break;
                
            case 'g':GMLinput = true;          break;
                
            case 'r':
				RareEvent = true;        
				StringInSpnLHA =true; // Need to know the name of place to find 
				// place begining with "RE_"
				break;
                
            case 'b':BoundedRE = atoi(optarg);
				StringInSpnLHA =true;
                RareEvent=true;
                break;
                
            case 'c':BoundedContinuous = true;
                RareEvent=true;
                break;
				
            case  1:horizon = atof(optarg);     break;
                
            case 's':computeStateSpace= true; 
				StringInSpnLHA = true;
				break;
                
            case 'a':alligatorMode = true;
				verbose =0;
				break;
				
            case  'l':Level = atof(optarg);     break;
            case  'w':Width = atof(optarg);     break;
            case  2  : Batch = atoi(optarg);      break;
            case  'm': MaxRuns = atoi(optarg);      break;
            case  'n': Njob = atoi(optarg);      break;
            case  'e': epsilon = atof(optarg);  break;
            case  'd': dataoutput = optarg; break;
			case  8  : dataraw = optarg; break;
			case  11 : dataPDFCDF = optarg; break;
			case  't': CountTrans = true;	break;
			case  3  : StringInSpnLHA = true; break;
			case  4  : tmpPath = optarg; break;
			case  9  : Path = optarg; break;
			case  5	 : tmpStatus = atoi(optarg); break;
			case  6  : gcccmd = optarg; break;
			case  7  : gccflags = optarg; break;
			case  10 : seed = atol(optarg); break;

            case '?':
                usage();
                exit(EXIT_FAILURE);
                
            default:
                usage();
                exit(EXIT_FAILURE);
        }
    }
	
    if (optind+2 == argc){
        PathGspn = argv[optind];
        PathLha  = argv[optind+1];
    }else{
		if (optind+2 > argc) {
			cout << "Two files are require." << endl;
		}else{
			cout << "Unrecognize option:"<<argv[optind+2]<<endl;
		}
        
        usage();
        exit(EXIT_FAILURE);
    }
}