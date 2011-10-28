#ifndef _SPN_GML_HPP
#define	_SPN_GML_HPP

#include "expatmodelparser.hh"
#include "modelhandler.hh"
#include "Gspn-Reader.hpp"
#include <map>

class MyModelHandler: public ModelHandler
{
private:
	int countPl;
	int countTr;
	bool ParsePl;
	map<int,bool> IsPlace;
	map<int,int> Gml2Place;
	map<int,int> Gml2Trans;
	GSPN* MyGspn;
public:
    MyModelHandler(GSPN* ) ;
	//~MyModelHandler() { }
	
	
    void on_read_model(const XmlString& formalismUrl) ;
	
    void on_read_model_attribute(const Attribute& attribute)	;
	
    void on_read_node(const XmlString& id,
					  const XmlString& nodeType,
					  const AttributeMap& attributes,
					  const XmlStringList& references) ;
	
    void on_read_arc(const XmlString& id,
					 const XmlString& arcType,
					 const XmlString& source,
					 const XmlString& target,
					 const AttributeMap& attributes,
					 const XmlStringList& references);
};








#endif

