#include "spn.hpp" 

#include <iostream>
#include <string>
#include <string.h>
#include <sstream>
#include <fstream>
#include <utility>
#include <stdlib.h>
#include <time.h>
#include <set>
#include <vector>
#include <math.h>
#include <float.h>
#include <limits.h> 
#include "RareEvent.hpp"

SPN::SPN() {
}

SPN::~SPN() {
}

SPN::SPN(const SPN & orig) {

}

set<int, less <int> > SPN::enabledTrans() {
    set<int, less<int> > eTrans;
    for (int i = 0; i < (this->tr); i++) {
        if (IsEnabled(i)) {
            eTrans.insert(i);
        }
    }
    this->enTrans = eTrans;
    return eTrans;
}

void SPN::reset() {
    Marking = initMarking;
}

void SPN::setMarking(vector<int>& M) {
    Marking = M;
}

void SPN::EnabledDisabledTr() {

    PossiblyEnabled = new Dim2;
    PossiblyDisabled = new Dim2;
    //-------------- Rare Event -----------------
    /*vector<int> AllEnabled;
      for (int i =0; i< tr; i++){ AllEnabled.push_back(i); };*/
    //------------- /Rare Event -----------------

    for (int t1 = 0; t1 < tr; t1++) {
        Dim1 V;
        Dim1 Vinhib;
        set<int> INt1;
        for (int p = 0; p < pl; p++) {
            if (inArcs[t1][p] > 0) {
                INt1.insert(p);
            }
        }
        for (int t2 = 0; t2 < tr; t2++)
            if (t1 != t2) {
                int size = INt1.size();
                set<int> INt1t2 = INt1;
                bool B = true;
                int p = 0;
                while (B && p < pl) {
                    if (inArcs[t2][p] > 0) {
                        INt1t2.insert(p);
                        if (size == INt1t2.size()) {
                            B = false;
                            V.push_back(t2);
                        } else size = INt1t2.size();
                    }
                    p++;
                }

                size = INt1.size();
                set<int> INt1t2Inhib = INt1;
                B = true;
                p = 0;
                while (B && p < pl) {
                    if (inhibArcs[t2][p] > 0) {
                        INt1t2Inhib.insert(p);
                        if (size == INt1t2Inhib.size()) {
                            B = false;
                            Vinhib.push_back(t2);
                        } else size = INt1t2Inhib.size();
                    }
                    p++;
                }

            }


        (*PossiblyDisabled).push_back(V);
        (*PossiblyEnabled).push_back(Vinhib);
    }
    for (int t1 = 0; t1 < tr; t1++) {
        Dim1 V = (*PossiblyEnabled)[t1];
        set<int> OUTt1;
        for (int p = 0; p < pl; p++)
            if (outArcs[t1][p] > 0)
                OUTt1.insert(p);
        for (int t2 = 0; t2 < tr; t2++)
            if (t1 != t2) {
                int size = OUTt1.size();
                set<int> OUTt1INt2 = OUTt1;
                bool B = true;
                int p = 0;
                while (B && p < pl) {
                    if (inArcs[t2][p] > 0) {
                        OUTt1INt2.insert(p);
                        if (size == OUTt1INt2.size()) {
                            B = false;
                            V.push_back(t2);
                        } else size = OUTt1INt2.size();
                    }
                    p++;
                }
            }
	//-------------- Rare Event -----------------
	(*PossiblyEnabled)[t1] = V;    //Ligne original
	//(*PossiblyEnabled)[t1]=AllEnabled;
	//------------- /Rare Event -----------------
    }


}

Dim1 SPN::PossiblyEn(int t) {
    return (*PossiblyEnabled)[t];
}

Dim1 SPN::PossiblyDis(int t) {
    return (*PossiblyDisabled)[t];
}

vector<int> SPN::getMarking() {
    return Marking;
}

double SPN::min(double x1, double x2) {
    if (x1 < x2) return x1;
    else return x2;
}

double SPN::max(double x1, double x2) {
    if (x1 > x2) return x1;
    else return x2;
}

void SPN::Msimple(){
  vector<int> tab;
  for(vector<spn_place>::iterator it=Place.begin(); it != Place.end(); it++){
    //cout << (*it).label<< " : " << (*it).label.substr(0,3) << endl;
    if((*it).label.substr(0,4).compare(" RE_") == 0){
      tab.push_back((*it).Id);
      //cout << ":"<<(*it).label << endl;
    }
  }
  Msimpletab = tab;
}

void SPN::Load(){
    Path ="philosophe";
    gammaprob.load();
    pl =26;
    tr =16;
    IntVector Null_Pl_Vector(pl, 0);
    IntMatrix Null_PlxTr_Matrix(tr, Null_Pl_Vector);
    inArcs = Null_PlxTr_Matrix;
    outArcs = Null_PlxTr_Matrix;
    inhibArcs = Null_PlxTr_Matrix;
    Marking = Null_Pl_Vector;;
    inArcs[0][0]=1;
    inArcs[0][3]=1;
    inArcs[0][20]=1;
    inArcs[0][24]=1;

    inArcs[1][1]=1;
    inArcs[1][19]=1;
    inArcs[1][21]=1;
    inArcs[1][24]=1;

    inArcs[2][2]=1;
    inArcs[2][22]=1;

    inArcs[3][4]=1;
    inArcs[3][7]=1;
    inArcs[3][20]=1;
    inArcs[3][24]=1;

    inArcs[4][3]=1;
    inArcs[4][5]=1;
    inArcs[4][21]=1;
    inArcs[4][24]=1;

    inArcs[5][6]=1;
    inArcs[5][22]=1;

    inArcs[6][8]=1;
    inArcs[6][11]=1;
    inArcs[6][20]=1;
    inArcs[6][24]=1;

    inArcs[7][7]=1;
    inArcs[7][9]=1;
    inArcs[7][21]=1;
    inArcs[7][24]=1;

    inArcs[8][10]=1;
    inArcs[8][22]=1;

    inArcs[9][12]=1;
    inArcs[9][15]=1;
    inArcs[9][20]=1;
    inArcs[9][24]=1;

    inArcs[10][11]=1;
    inArcs[10][13]=1;
    inArcs[10][21]=1;
    inArcs[10][24]=1;

    inArcs[11][14]=1;
    inArcs[11][22]=1;

    inArcs[12][16]=1;
    inArcs[12][19]=1;
    inArcs[12][20]=1;
    inArcs[12][24]=1;

    inArcs[13][15]=1;
    inArcs[13][17]=1;
    inArcs[13][21]=1;
    inArcs[13][24]=1;

    inArcs[14][18]=1;
    inArcs[14][22]=1;


    outArcs[0][1]=1;
    outArcs[0][21]=1;

    outArcs[1][2]=1;
    outArcs[1][22]=1;

    outArcs[2][0]=1;
    outArcs[2][3]=1;
    outArcs[2][19]=1;
    outArcs[2][20]=1;
    outArcs[2][23]=1;
    outArcs[2][24]=2;

    outArcs[3][5]=1;
    outArcs[3][21]=1;

    outArcs[4][6]=1;
    outArcs[4][22]=1;

    outArcs[5][3]=1;
    outArcs[5][4]=1;
    outArcs[5][7]=1;
    outArcs[5][20]=1;
    outArcs[5][23]=1;
    outArcs[5][24]=2;

    outArcs[6][9]=1;
    outArcs[6][21]=1;

    outArcs[7][10]=1;
    outArcs[7][22]=1;

    outArcs[8][7]=1;
    outArcs[8][8]=1;
    outArcs[8][11]=1;
    outArcs[8][20]=1;
    outArcs[8][23]=1;
    outArcs[8][24]=2;

    outArcs[9][13]=1;
    outArcs[9][21]=1;

    outArcs[10][14]=1;
    outArcs[10][22]=1;

    outArcs[11][11]=1;
    outArcs[11][12]=1;
    outArcs[11][15]=1;
    outArcs[11][20]=1;
    outArcs[11][23]=1;
    outArcs[11][24]=2;

    outArcs[12][17]=1;
    outArcs[12][21]=1;

    outArcs[13][18]=1;
    outArcs[13][22]=1;

    outArcs[14][15]=1;
    outArcs[14][16]=1;
    outArcs[14][19]=1;
    outArcs[14][20]=1;
    outArcs[14][23]=1;
    outArcs[14][24]=2;

    outArcs[15][25]=1;

















    Marking[0]=1;
    Marking[3]=1;
    Marking[4]=1;
    Marking[7]=1;
    Marking[8]=1;
    Marking[11]=1;
    Marking[12]=1;
    Marking[15]=1;
    Marking[16]=1;
    Marking[19]=1;
    Marking[20]=5;
    Marking[24]=5;

   initMarking = Marking;

    vector <spn_trans> t(16);
    Transition = t;
    vector <spn_place> p(26);
    Place = p;
    Place[3].Id =3;
    Place[3].label =" Fork1";
    PlaceIndex[" Fork1"] =3;
    Place[7].Id =7;
    Place[7].label =" Fork2";
    PlaceIndex[" Fork2"] =7;
    Place[11].Id =11;
    Place[11].label =" Fork3";
    PlaceIndex[" Fork3"] =11;
    Place[15].Id =15;
    Place[15].label =" Fork4";
    PlaceIndex[" Fork4"] =15;
    Place[19].Id =19;
    Place[19].label =" Fork5";
    PlaceIndex[" Fork5"] =19;
    Place[0].Id =0;
    Place[0].label =" Philo1_Think";
    PlaceIndex[" Philo1_Think"] =0;
    Place[2].Id =2;
    Place[2].label =" Philo1_eat";
    PlaceIndex[" Philo1_eat"] =2;
    Place[1].Id =1;
    Place[1].label =" Philo1_fork";
    PlaceIndex[" Philo1_fork"] =1;
    Place[4].Id =4;
    Place[4].label =" Philo2_Think";
    PlaceIndex[" Philo2_Think"] =4;
    Place[6].Id =6;
    Place[6].label =" Philo2_eat";
    PlaceIndex[" Philo2_eat"] =6;
    Place[5].Id =5;
    Place[5].label =" Philo2_fork";
    PlaceIndex[" Philo2_fork"] =5;
    Place[8].Id =8;
    Place[8].label =" Philo3_Think";
    PlaceIndex[" Philo3_Think"] =8;
    Place[10].Id =10;
    Place[10].label =" Philo3_eat";
    PlaceIndex[" Philo3_eat"] =10;
    Place[9].Id =9;
    Place[9].label =" Philo3_fork";
    PlaceIndex[" Philo3_fork"] =9;
    Place[12].Id =12;
    Place[12].label =" Philo4_Think";
    PlaceIndex[" Philo4_Think"] =12;
    Place[14].Id =14;
    Place[14].label =" Philo4_eat";
    PlaceIndex[" Philo4_eat"] =14;
    Place[13].Id =13;
    Place[13].label =" Philo4_fork";
    PlaceIndex[" Philo4_fork"] =13;
    Place[16].Id =16;
    Place[16].label =" Philo5_Think";
    PlaceIndex[" Philo5_Think"] =16;
    Place[18].Id =18;
    Place[18].label =" Philo5_eat";
    PlaceIndex[" Philo5_eat"] =18;
    Place[17].Id =17;
    Place[17].label =" Philo5_fork";
    PlaceIndex[" Philo5_fork"] =17;
    Place[25].Id =25;
    Place[25].label =" Puit";
    PlaceIndex[" Puit"] =25;
    Place[22].Id =22;
    Place[22].label =" RE_nEat";
    PlaceIndex[" RE_nEat"] =22;
    Place[24].Id =24;
    Place[24].label =" RE_nFork";
    PlaceIndex[" RE_nFork"] =24;
    Place[23].Id =23;
    Place[23].label =" RE_nServ";
    PlaceIndex[" RE_nServ"] =23;
    Place[20].Id =20;
    Place[20].label =" RE_nThink";
    PlaceIndex[" RE_nThink"] =20;
    Place[21].Id =21;
    Place[21].label =" RE_nWait";
    PlaceIndex[" RE_nWait"] =21;
    Transition[2].Id =2;
    Transition[2].label ="Philo1_eat";
    TransitionIndex["Philo1_eat"]=2;
    Transition[0].Id =0;
    Transition[0].label ="Philo1_fork1";
    TransitionIndex["Philo1_fork1"]=0;
    Transition[1].Id =1;
    Transition[1].label ="Philo1_fork2";
    TransitionIndex["Philo1_fork2"]=1;
    Transition[5].Id =5;
    Transition[5].label ="Philo2_eat";
    TransitionIndex["Philo2_eat"]=5;
    Transition[3].Id =3;
    Transition[3].label ="Philo2_fork1";
    TransitionIndex["Philo2_fork1"]=3;
    Transition[4].Id =4;
    Transition[4].label ="Philo2_fork2";
    TransitionIndex["Philo2_fork2"]=4;
    Transition[8].Id =8;
    Transition[8].label ="Philo3_eat";
    TransitionIndex["Philo3_eat"]=8;
    Transition[6].Id =6;
    Transition[6].label ="Philo3_fork1";
    TransitionIndex["Philo3_fork1"]=6;
    Transition[7].Id =7;
    Transition[7].label ="Philo3_fork2";
    TransitionIndex["Philo3_fork2"]=7;
    Transition[11].Id =11;
    Transition[11].label ="Philo4_eat";
    TransitionIndex["Philo4_eat"]=11;
    Transition[9].Id =9;
    Transition[9].label ="Philo4_fork1";
    TransitionIndex["Philo4_fork1"]=9;
    Transition[10].Id =10;
    Transition[10].label ="Philo4_fork2";
    TransitionIndex["Philo4_fork2"]=10;
    Transition[14].Id =14;
    Transition[14].label ="Philo5_eat";
    TransitionIndex["Philo5_eat"]=14;
    Transition[12].Id =12;
    Transition[12].label ="Philo5_fork1";
    TransitionIndex["Philo5_fork1"]=12;
    Transition[13].Id =13;
    Transition[13].label ="Philo5_fork2";
    TransitionIndex["Philo5_fork2"]=13;
    Transition[15].Id =15;
    Transition[15].label ="Puittrans";
    TransitionIndex["Puittrans"]=15;
    Transition[0].transType = Timed;
    Transition[0].DistType = "EXPONENTIAL";
    Transition[0].DistParams.push_back(" 1" );
    Transition[0].priority = "1";
    Transition[0].MarkingDependent = 0;
    Transition[0].weight = "1 ";

    Transition[1].transType = Timed;
    Transition[1].DistType = "EXPONENTIAL";
    Transition[1].DistParams.push_back(" 100" );
    Transition[1].priority = "1";
    Transition[1].MarkingDependent = 0;
    Transition[1].weight = "1 ";

    Transition[2].transType = Timed;
    Transition[2].DistType = "EXPONENTIAL";
    Transition[2].DistParams.push_back(" 10" );
    Transition[2].priority = "1";
    Transition[2].MarkingDependent = 0;
    Transition[2].weight = "1 ";

    Transition[3].transType = Timed;
    Transition[3].DistType = "EXPONENTIAL";
    Transition[3].DistParams.push_back(" 1" );
    Transition[3].priority = "1";
    Transition[3].MarkingDependent = 0;
    Transition[3].weight = "1 ";

    Transition[4].transType = Timed;
    Transition[4].DistType = "EXPONENTIAL";
    Transition[4].DistParams.push_back(" 100" );
    Transition[4].priority = "1";
    Transition[4].MarkingDependent = 0;
    Transition[4].weight = "1 ";

    Transition[5].transType = Timed;
    Transition[5].DistType = "EXPONENTIAL";
    Transition[5].DistParams.push_back(" 10" );
    Transition[5].priority = "1";
    Transition[5].MarkingDependent = 0;
    Transition[5].weight = "1 ";

    Transition[6].transType = Timed;
    Transition[6].DistType = "EXPONENTIAL";
    Transition[6].DistParams.push_back(" 1" );
    Transition[6].priority = "1";
    Transition[6].MarkingDependent = 0;
    Transition[6].weight = "1 ";

    Transition[7].transType = Timed;
    Transition[7].DistType = "EXPONENTIAL";
    Transition[7].DistParams.push_back(" 100" );
    Transition[7].priority = "1";
    Transition[7].MarkingDependent = 0;
    Transition[7].weight = "1 ";

    Transition[8].transType = Timed;
    Transition[8].DistType = "EXPONENTIAL";
    Transition[8].DistParams.push_back(" 10" );
    Transition[8].priority = "1";
    Transition[8].MarkingDependent = 0;
    Transition[8].weight = "1 ";

    Transition[9].transType = Timed;
    Transition[9].DistType = "EXPONENTIAL";
    Transition[9].DistParams.push_back(" 1" );
    Transition[9].priority = "1";
    Transition[9].MarkingDependent = 0;
    Transition[9].weight = "1 ";

    Transition[10].transType = Timed;
    Transition[10].DistType = "EXPONENTIAL";
    Transition[10].DistParams.push_back(" 100" );
    Transition[10].priority = "1";
    Transition[10].MarkingDependent = 0;
    Transition[10].weight = "1 ";

    Transition[11].transType = Timed;
    Transition[11].DistType = "EXPONENTIAL";
    Transition[11].DistParams.push_back(" 10" );
    Transition[11].priority = "1";
    Transition[11].MarkingDependent = 0;
    Transition[11].weight = "1 ";

    Transition[12].transType = Timed;
    Transition[12].DistType = "EXPONENTIAL";
    Transition[12].DistParams.push_back(" 1" );
    Transition[12].priority = "1";
    Transition[12].MarkingDependent = 0;
    Transition[12].weight = "1 ";

    Transition[13].transType = Timed;
    Transition[13].DistType = "EXPONENTIAL";
    Transition[13].DistParams.push_back(" 100" );
    Transition[13].priority = "1";
    Transition[13].MarkingDependent = 0;
    Transition[13].weight = "1 ";

    Transition[14].transType = Timed;
    Transition[14].DistType = "EXPONENTIAL";
    Transition[14].DistParams.push_back(" 10" );
    Transition[14].priority = "1";
    Transition[14].MarkingDependent = 0;
    Transition[14].weight = "1 ";

    Transition[15].transType = Timed;
    Transition[15].DistType = "EXPONENTIAL";
    Transition[15].DistParams.push_back(" 0 *  Marking[ 0 ] " );
    Transition[15].priority = "1";
    Transition[15].MarkingDependent = 1;
    Transition[15].weight = "1 ";

    vector < vector <int> > vec(16);
    PredT = vec;
    PredT[0].push_back(0);
    PredT[0].push_back(3);
    PredT[0].push_back(20);
    PredT[0].push_back(24);
    PredT[1].push_back(1);
    PredT[1].push_back(19);
    PredT[1].push_back(21);
    PredT[1].push_back(24);
    PredT[2].push_back(2);
    PredT[2].push_back(22);
    PredT[3].push_back(4);
    PredT[3].push_back(7);
    PredT[3].push_back(20);
    PredT[3].push_back(24);
    PredT[4].push_back(3);
    PredT[4].push_back(5);
    PredT[4].push_back(21);
    PredT[4].push_back(24);
    PredT[5].push_back(6);
    PredT[5].push_back(22);
    PredT[6].push_back(8);
    PredT[6].push_back(11);
    PredT[6].push_back(20);
    PredT[6].push_back(24);
    PredT[7].push_back(7);
    PredT[7].push_back(9);
    PredT[7].push_back(21);
    PredT[7].push_back(24);
    PredT[8].push_back(10);
    PredT[8].push_back(22);
    PredT[9].push_back(12);
    PredT[9].push_back(15);
    PredT[9].push_back(20);
    PredT[9].push_back(24);
    PredT[10].push_back(11);
    PredT[10].push_back(13);
    PredT[10].push_back(21);
    PredT[10].push_back(24);
    PredT[11].push_back(14);
    PredT[11].push_back(22);
    PredT[12].push_back(16);
    PredT[12].push_back(19);
    PredT[12].push_back(20);
    PredT[12].push_back(24);
    PredT[13].push_back(15);
    PredT[13].push_back(17);
    PredT[13].push_back(21);
    PredT[13].push_back(24);
    PredT[14].push_back(18);
    PredT[14].push_back(22);
    EnabledDisabledTr();
    Msimple();
vector <double> Rate_Table_init (tr);
Rate_Table = Rate_Table_init;
Origine_Rate_Table = Rate_Table_init;
}

bool SPN::IsEnabled(int t){
    switch(t){
     case 0:
       return IsEnabled_t0();
       break;
     case 1:
       return IsEnabled_t1();
       break;
     case 2:
       return IsEnabled_t2();
       break;
     case 3:
       return IsEnabled_t3();
       break;
     case 4:
       return IsEnabled_t4();
       break;
     case 5:
       return IsEnabled_t5();
       break;
     case 6:
       return IsEnabled_t6();
       break;
     case 7:
       return IsEnabled_t7();
       break;
     case 8:
       return IsEnabled_t8();
       break;
     case 9:
       return IsEnabled_t9();
       break;
     case 10:
       return IsEnabled_t10();
       break;
     case 11:
       return IsEnabled_t11();
       break;
     case 12:
       return IsEnabled_t12();
       break;
     case 13:
       return IsEnabled_t13();
       break;
     case 14:
       return IsEnabled_t14();
       break;
     case 15:
       return IsEnabled_t15();
       break;
   }
}

bool SPN::IsEnabled_t0() {
    if (Marking[0] < 1) return false;
    if (Marking[3] < 1) return false;
    if (Marking[20] < 1) return false;
    if (Marking[24] < 1) return false;
    return true;
}

bool SPN::IsEnabled_t1() {
    if (Marking[1] < 1) return false;
    if (Marking[19] < 1) return false;
    if (Marking[21] < 1) return false;
    if (Marking[24] < 1) return false;
    return true;
}

bool SPN::IsEnabled_t2() {
    if (Marking[2] < 1) return false;
    if (Marking[22] < 1) return false;
    return true;
}

bool SPN::IsEnabled_t3() {
    if (Marking[4] < 1) return false;
    if (Marking[7] < 1) return false;
    if (Marking[20] < 1) return false;
    if (Marking[24] < 1) return false;
    return true;
}

bool SPN::IsEnabled_t4() {
    if (Marking[3] < 1) return false;
    if (Marking[5] < 1) return false;
    if (Marking[21] < 1) return false;
    if (Marking[24] < 1) return false;
    return true;
}

bool SPN::IsEnabled_t5() {
    if (Marking[6] < 1) return false;
    if (Marking[22] < 1) return false;
    return true;
}

bool SPN::IsEnabled_t6() {
    if (Marking[8] < 1) return false;
    if (Marking[11] < 1) return false;
    if (Marking[20] < 1) return false;
    if (Marking[24] < 1) return false;
    return true;
}

bool SPN::IsEnabled_t7() {
    if (Marking[7] < 1) return false;
    if (Marking[9] < 1) return false;
    if (Marking[21] < 1) return false;
    if (Marking[24] < 1) return false;
    return true;
}

bool SPN::IsEnabled_t8() {
    if (Marking[10] < 1) return false;
    if (Marking[22] < 1) return false;
    return true;
}

bool SPN::IsEnabled_t9() {
    if (Marking[12] < 1) return false;
    if (Marking[15] < 1) return false;
    if (Marking[20] < 1) return false;
    if (Marking[24] < 1) return false;
    return true;
}

bool SPN::IsEnabled_t10() {
    if (Marking[11] < 1) return false;
    if (Marking[13] < 1) return false;
    if (Marking[21] < 1) return false;
    if (Marking[24] < 1) return false;
    return true;
}

bool SPN::IsEnabled_t11() {
    if (Marking[14] < 1) return false;
    if (Marking[22] < 1) return false;
    return true;
}

bool SPN::IsEnabled_t12() {
    if (Marking[16] < 1) return false;
    if (Marking[19] < 1) return false;
    if (Marking[20] < 1) return false;
    if (Marking[24] < 1) return false;
    return true;
}

bool SPN::IsEnabled_t13() {
    if (Marking[15] < 1) return false;
    if (Marking[17] < 1) return false;
    if (Marking[21] < 1) return false;
    if (Marking[24] < 1) return false;
    return true;
}

bool SPN::IsEnabled_t14() {
    if (Marking[18] < 1) return false;
    if (Marking[22] < 1) return false;
    return true;
}

bool SPN::IsEnabled_t15() {
    return true;
}

void SPN::fire(int t){
   switch(t){
     case 0: {
       fire_t0();
       break;
     } 
     case 1: {
       fire_t1();
       break;
     } 
     case 2: {
       fire_t2();
       break;
     } 
     case 3: {
       fire_t3();
       break;
     } 
     case 4: {
       fire_t4();
       break;
     } 
     case 5: {
       fire_t5();
       break;
     } 
     case 6: {
       fire_t6();
       break;
     } 
     case 7: {
       fire_t7();
       break;
     } 
     case 8: {
       fire_t8();
       break;
     } 
     case 9: {
       fire_t9();
       break;
     } 
     case 10: {
       fire_t10();
       break;
     } 
     case 11: {
       fire_t11();
       break;
     } 
     case 12: {
       fire_t12();
       break;
     } 
     case 13: {
       fire_t13();
       break;
     } 
     case 14: {
       fire_t14();
       break;
     } 
     case 15: {
       fire_t15();
       break;
     } 
   }
}

void SPN::unfire(int t){
   switch(t){
     case 0: {
       unfire_t0();
       break;
     } 
     case 1: {
       unfire_t1();
       break;
     } 
     case 2: {
       unfire_t2();
       break;
     } 
     case 3: {
       unfire_t3();
       break;
     } 
     case 4: {
       unfire_t4();
       break;
     } 
     case 5: {
       unfire_t5();
       break;
     } 
     case 6: {
       unfire_t6();
       break;
     } 
     case 7: {
       unfire_t7();
       break;
     } 
     case 8: {
       unfire_t8();
       break;
     } 
     case 9: {
       unfire_t9();
       break;
     } 
     case 10: {
       unfire_t10();
       break;
     } 
     case 11: {
       unfire_t11();
       break;
     } 
     case 12: {
       unfire_t12();
       break;
     } 
     case 13: {
       unfire_t13();
       break;
     } 
     case 14: {
       unfire_t14();
       break;
     } 
     case 15: {
       unfire_t15();
       break;
     } 
   }
}

void SPN::fire_t0() {
    Marking[0] -= 1;
    Marking[1] += 1;
    Marking[3] -= 1;
    Marking[20] -= 1;
    Marking[21] += 1;
    Marking[24] -= 1;
}

void SPN::unfire_t0() {
    Marking[0] += 1;
    Marking[1] -= 1;
    Marking[3] += 1;
    Marking[20] += 1;
    Marking[21] -= 1;
    Marking[24] += 1;
}

void SPN::fire_t1() {
    Marking[1] -= 1;
    Marking[2] += 1;
    Marking[19] -= 1;
    Marking[21] -= 1;
    Marking[22] += 1;
    Marking[24] -= 1;
}

void SPN::unfire_t1() {
    Marking[1] += 1;
    Marking[2] -= 1;
    Marking[19] += 1;
    Marking[21] += 1;
    Marking[22] -= 1;
    Marking[24] += 1;
}

void SPN::fire_t2() {
    Marking[0] += 1;
    Marking[2] -= 1;
    Marking[3] += 1;
    Marking[19] += 1;
    Marking[20] += 1;
    Marking[22] -= 1;
    Marking[23] += 1;
    Marking[24] += 2;
}

void SPN::unfire_t2() {
    Marking[0] -= 1;
    Marking[2] += 1;
    Marking[3] -= 1;
    Marking[19] -= 1;
    Marking[20] -= 1;
    Marking[22] += 1;
    Marking[23] -= 1;
    Marking[24] -= 2;
}

void SPN::fire_t3() {
    Marking[4] -= 1;
    Marking[5] += 1;
    Marking[7] -= 1;
    Marking[20] -= 1;
    Marking[21] += 1;
    Marking[24] -= 1;
}

void SPN::unfire_t3() {
    Marking[4] += 1;
    Marking[5] -= 1;
    Marking[7] += 1;
    Marking[20] += 1;
    Marking[21] -= 1;
    Marking[24] += 1;
}

void SPN::fire_t4() {
    Marking[3] -= 1;
    Marking[5] -= 1;
    Marking[6] += 1;
    Marking[21] -= 1;
    Marking[22] += 1;
    Marking[24] -= 1;
}

void SPN::unfire_t4() {
    Marking[3] += 1;
    Marking[5] += 1;
    Marking[6] -= 1;
    Marking[21] += 1;
    Marking[22] -= 1;
    Marking[24] += 1;
}

void SPN::fire_t5() {
    Marking[3] += 1;
    Marking[4] += 1;
    Marking[6] -= 1;
    Marking[7] += 1;
    Marking[20] += 1;
    Marking[22] -= 1;
    Marking[23] += 1;
    Marking[24] += 2;
}

void SPN::unfire_t5() {
    Marking[3] -= 1;
    Marking[4] -= 1;
    Marking[6] += 1;
    Marking[7] -= 1;
    Marking[20] -= 1;
    Marking[22] += 1;
    Marking[23] -= 1;
    Marking[24] -= 2;
}

void SPN::fire_t6() {
    Marking[8] -= 1;
    Marking[9] += 1;
    Marking[11] -= 1;
    Marking[20] -= 1;
    Marking[21] += 1;
    Marking[24] -= 1;
}

void SPN::unfire_t6() {
    Marking[8] += 1;
    Marking[9] -= 1;
    Marking[11] += 1;
    Marking[20] += 1;
    Marking[21] -= 1;
    Marking[24] += 1;
}

void SPN::fire_t7() {
    Marking[7] -= 1;
    Marking[9] -= 1;
    Marking[10] += 1;
    Marking[21] -= 1;
    Marking[22] += 1;
    Marking[24] -= 1;
}

void SPN::unfire_t7() {
    Marking[7] += 1;
    Marking[9] += 1;
    Marking[10] -= 1;
    Marking[21] += 1;
    Marking[22] -= 1;
    Marking[24] += 1;
}

void SPN::fire_t8() {
    Marking[7] += 1;
    Marking[8] += 1;
    Marking[10] -= 1;
    Marking[11] += 1;
    Marking[20] += 1;
    Marking[22] -= 1;
    Marking[23] += 1;
    Marking[24] += 2;
}

void SPN::unfire_t8() {
    Marking[7] -= 1;
    Marking[8] -= 1;
    Marking[10] += 1;
    Marking[11] -= 1;
    Marking[20] -= 1;
    Marking[22] += 1;
    Marking[23] -= 1;
    Marking[24] -= 2;
}

void SPN::fire_t9() {
    Marking[12] -= 1;
    Marking[13] += 1;
    Marking[15] -= 1;
    Marking[20] -= 1;
    Marking[21] += 1;
    Marking[24] -= 1;
}

void SPN::unfire_t9() {
    Marking[12] += 1;
    Marking[13] -= 1;
    Marking[15] += 1;
    Marking[20] += 1;
    Marking[21] -= 1;
    Marking[24] += 1;
}

void SPN::fire_t10() {
    Marking[11] -= 1;
    Marking[13] -= 1;
    Marking[14] += 1;
    Marking[21] -= 1;
    Marking[22] += 1;
    Marking[24] -= 1;
}

void SPN::unfire_t10() {
    Marking[11] += 1;
    Marking[13] += 1;
    Marking[14] -= 1;
    Marking[21] += 1;
    Marking[22] -= 1;
    Marking[24] += 1;
}

void SPN::fire_t11() {
    Marking[11] += 1;
    Marking[12] += 1;
    Marking[14] -= 1;
    Marking[15] += 1;
    Marking[20] += 1;
    Marking[22] -= 1;
    Marking[23] += 1;
    Marking[24] += 2;
}

void SPN::unfire_t11() {
    Marking[11] -= 1;
    Marking[12] -= 1;
    Marking[14] += 1;
    Marking[15] -= 1;
    Marking[20] -= 1;
    Marking[22] += 1;
    Marking[23] -= 1;
    Marking[24] -= 2;
}

void SPN::fire_t12() {
    Marking[16] -= 1;
    Marking[17] += 1;
    Marking[19] -= 1;
    Marking[20] -= 1;
    Marking[21] += 1;
    Marking[24] -= 1;
}

void SPN::unfire_t12() {
    Marking[16] += 1;
    Marking[17] -= 1;
    Marking[19] += 1;
    Marking[20] += 1;
    Marking[21] -= 1;
    Marking[24] += 1;
}

void SPN::fire_t13() {
    Marking[15] -= 1;
    Marking[17] -= 1;
    Marking[18] += 1;
    Marking[21] -= 1;
    Marking[22] += 1;
    Marking[24] -= 1;
}

void SPN::unfire_t13() {
    Marking[15] += 1;
    Marking[17] += 1;
    Marking[18] -= 1;
    Marking[21] += 1;
    Marking[22] -= 1;
    Marking[24] += 1;
}

void SPN::fire_t14() {
    Marking[15] += 1;
    Marking[16] += 1;
    Marking[18] -= 1;
    Marking[19] += 1;
    Marking[20] += 1;
    Marking[22] -= 1;
    Marking[23] += 1;
    Marking[24] += 2;
}

void SPN::unfire_t14() {
    Marking[15] -= 1;
    Marking[16] -= 1;
    Marking[18] += 1;
    Marking[19] -= 1;
    Marking[20] -= 1;
    Marking[22] += 1;
    Marking[23] -= 1;
    Marking[24] -= 2;
}

void SPN::fire_t15() {
    Marking[25] += 1;
}

void SPN::unfire_t15() {
    Marking[25] -= 1;
}

vector<double> SPN::GetDistParameters(int t){
   vector<double> P(2);
   double origin_rate = (SPN::GetDistParametersOrigin(t))[0];
   P[0]= ComputeDistr( *this ,t, origin_rate);
   P[1]= origin_rate;
   return P;
}
 
vector<double> SPN::GetDistParametersOrigin(int t){
   switch(t){
     case 0: {
       vector<double> P(1);
       P[0]= ( double ) 1;
       return P;
       break;
     }
     case 1: {
       vector<double> P(1);
       P[0]= ( double ) 100;
       return P;
       break;
     }
     case 2: {
       vector<double> P(1);
       P[0]= ( double ) 10;
       return P;
       break;
     }
     case 3: {
       vector<double> P(1);
       P[0]= ( double ) 1;
       return P;
       break;
     }
     case 4: {
       vector<double> P(1);
       P[0]= ( double ) 100;
       return P;
       break;
     }
     case 5: {
       vector<double> P(1);
       P[0]= ( double ) 10;
       return P;
       break;
     }
     case 6: {
       vector<double> P(1);
       P[0]= ( double ) 1;
       return P;
       break;
     }
     case 7: {
       vector<double> P(1);
       P[0]= ( double ) 100;
       return P;
       break;
     }
     case 8: {
       vector<double> P(1);
       P[0]= ( double ) 10;
       return P;
       break;
     }
     case 9: {
       vector<double> P(1);
       P[0]= ( double ) 1;
       return P;
       break;
     }
     case 10: {
       vector<double> P(1);
       P[0]= ( double ) 100;
       return P;
       break;
     }
     case 11: {
       vector<double> P(1);
       P[0]= ( double ) 10;
       return P;
       break;
     }
     case 12: {
       vector<double> P(1);
       P[0]= ( double ) 1;
       return P;
       break;
     }
     case 13: {
       vector<double> P(1);
       P[0]= ( double ) 100;
       return P;
       break;
     }
     case 14: {
       vector<double> P(1);
       P[0]= ( double ) 10;
       return P;
       break;
     }
     case 15: {
       vector<double> P(1);
       P[0]= ( double ) 0 *  Marking[ 0 ] ;
       return P;
       break;
     }
   }
}

double SPN::GetPriority(int t){
   switch(t){
     case 0: {
       return (double)1;
       break;
     } 
     case 1: {
       return (double)1;
       break;
     } 
     case 2: {
       return (double)1;
       break;
     } 
     case 3: {
       return (double)1;
       break;
     } 
     case 4: {
       return (double)1;
       break;
     } 
     case 5: {
       return (double)1;
       break;
     } 
     case 6: {
       return (double)1;
       break;
     } 
     case 7: {
       return (double)1;
       break;
     } 
     case 8: {
       return (double)1;
       break;
     } 
     case 9: {
       return (double)1;
       break;
     } 
     case 10: {
       return (double)1;
       break;
     } 
     case 11: {
       return (double)1;
       break;
     } 
     case 12: {
       return (double)1;
       break;
     } 
     case 13: {
       return (double)1;
       break;
     } 
     case 14: {
       return (double)1;
       break;
     } 
     case 15: {
       return (double)1;
       break;
     } 
   }
}

double SPN::GetWeight(int t){
   switch(t){
     case 0:
       return (double)1;
       break;
     case 1:
       return (double)1;
       break;
     case 2:
       return (double)1;
       break;
     case 3:
       return (double)1;
       break;
     case 4:
       return (double)1;
       break;
     case 5:
       return (double)1;
       break;
     case 6:
       return (double)1;
       break;
     case 7:
       return (double)1;
       break;
     case 8:
       return (double)1;
       break;
     case 9:
       return (double)1;
       break;
     case 10:
       return (double)1;
       break;
     case 11:
       return (double)1;
       break;
     case 12:
       return (double)1;
       break;
     case 13:
       return (double)1;
       break;
     case 14:
       return (double)1;
       break;
     case 15:
       return (double)1;
       break;
   }
}
