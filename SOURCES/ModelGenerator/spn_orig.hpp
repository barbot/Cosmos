#ifndef _SPN_HPP
#define	_SPN_HPP
#include <set>
#include <utility>
#include <string>
#include <vector>
#include <map>
#include "tab.hpp"
using namespace std;

enum TransType {
    Timed, unTimed
};

struct _trans {
    unsigned int Id;
    string label;
    TransType transType; // Time or unTimed
    string DistType;
    vector<string> DistParams;
    string priority;
    string weight;
    bool MarkingDependent;
};
typedef struct _trans spn_trans;

struct _place {
    unsigned int Id;
    string label;
};
typedef struct _place spn_place;

typedef vector <int> Dim1;
typedef vector <int> IntVector;
typedef vector <Dim1> Dim2;
typedef vector <IntVector> IntMatrix;

class SPN {
public:

    // Modification evenements rares.
  vector <double> Rate_Table;
  vector <double> Origine_Rate_Table;
  double Rate_Sum;
  double Origine_Rate_Sum;
  vector <int> Msimpletab;
  TAB gammaprob;
  

    SPN();
    void Load();
    SPN(const SPN& orig);
    virtual ~SPN();
    string Path;
    int pl; // Number of places
    int tr; // Number of transitions
    set<int, less<int> > enTrans; // the set of current enabled transition
    IntMatrix* PossiblyEnabled; //a matrix, where a row t of PossiblyEnabled refers to transitions that may be enabled after firing t
    IntMatrix* PossiblyDisabled; //a matrix, where a row t of PossiblyDisabled refers to transitions that may be disabled after firing t

    vector <int> Marking; // Current marking
    vector <int> initMarking; //initial marking
    IntMatrix inArcs; // input arcs
    IntMatrix outArcs; //output arcs
    IntMatrix inhibArcs; // inhibitor arcs

    vector <spn_trans> Transition; //contains all the transitions of the Petri net
    vector <spn_place> Place; //contains all the places of the Petri net
    vector < vector<int> > PredT; // to be deleted

    map <string, int> PlaceIndex; // for a given place label return its index among {0, 1, ..., pl-1}
    map <string, int> TransitionIndex; // for a given transition label return its index among {0, 1, ..., tr-1}

    //  map <int, string> PlaceLabel;// for a given place index return its label
    //  map <int, string> TransitionLabel;// for a given transition index return its label



    double min(double, double); //return the minimum of two numbers
    double max(double, double); //return the maximum of two numbers

  void Msimple();


    vector<int> getMarking(); //return  the current marking

    set<int, less <int> > enabledTrans(); // return the set of enabled transitions


    void EnabledDisabledTr(); // contruct for each transition t, the vector of transitions that may be enabled (PossiblyEnabled) and the vector of transitions that may be disabled (PossiblyDisabled) after firing t
    Dim1 PossiblyEn(int); // for a given transition t, return the vector of transitions that may be enabled after firing t  (it is the row t of PossiblyEnabled)
    Dim1 PossiblyDis(int); // for a given transition t, return the vector of transitions that may be disabled after firing t  (it is the row t of PossiblyDisabled)


    void reset(); // set the marking to the initial marking
    void setMarking(vector<int>&); // set the marking to a given marking



    void fire(int); // fire a given transition
    void unfire(int); // unfire a given transition
    bool IsEnabled(int); // Check if a given transition is enabled
    vector<double> GetDistParameters(int); // compute the the parameters value of a given distribution 
   //------------ Rare Event ---------------------
  vector<double> GetDistParametersOrigin(int);
  //------------ /Rare Event -------------------

    double GetWeight(int); // compute the the weight value of a given transition
    double GetPriority(int); // compute the the priority value of a given transition


    //private:
    //};
    //#endif	/* _SPN_HPP */