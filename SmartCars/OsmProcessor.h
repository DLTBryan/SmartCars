#pragma once

#ifndef OSMPROCESSOR_H
#define OSMPROCESSOR_H

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "rue.h"
#include "tinyxml2.h"
#include "noeud.h"
#include <map>

using namespace std;
using namespace tinyxml2;

/**
*Class to parse .OSM FILE
*Parse all the ways where those ones are considered as primary highway
*------------------------------------------------------------------------------------
*A way is considered as primary highway where it has a subNode element named "Tag" 
*that constains the following attributes:
*	where k = 'highway'
*	and 
*	where v = 'primary'
*/
class OsmProcessor
{
public:
	OsmProcessor(string fileName);
	~OsmProcessor() = default;

	Rue* rue(string id) const;
	vector<Rue*> ways() const { return d_ways; };
	string fileName() const { return d_fileName; };
	void printRue(int index);

private:
	void init();
	/// <summary>
	/// Method of OsmProcessor class designed to parse the osm file 
	/// <see cref="OsmProcessor::processKandVAttributes"/>
	/// </summary>
	void firstParsing();
	void processKandVAttribute(XMLElement* currentWay, Rue* currentRue);
	Noeud* makeNode(string idNode, XMLElement* currentWay);
	void findNodeOnOtherRoutes(Noeud* node, XMLElement* currentWay);
	void linkRueNodes(Rue* rue);
	void linkNeighbooringNodes();
	void updateNode(Noeud* nodeToUpdate, int currentRueIndex);
	bool isValidRoad(string attributeK, string attributeV);

	//attributes
	XMLDocument d_map;
	XMLNode* d_rootNode;

	string d_fileName;

	vector<Rue*> d_ways;



};
#endif




