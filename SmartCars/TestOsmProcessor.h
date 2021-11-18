#pragma once
#ifndef TESTOSMPROCESSOR_H
#define TESTOSMPROCESSOR_H

#include "OsmProcessor.h"
#include <vector>
#include <iostream>
#include <string>
#include "rue.h"
#include "noeud.h"
//#include "graphics.h"

using namespace std;
/**

Class Designed to do Tests on the OsmProcessor Class

*/
class TestOsmProcessor
{
public:
	TestOsmProcessor(OsmProcessor* osm) : d_osmProcessor{ osm } { retrieveNumberOfNodes(); };
	~TestOsmProcessor() = default;
	void testParsingOsm();
	void tracePlan();

private:
	OsmProcessor* d_osmProcessor;
	vector<int> d_nbNodesPerRoad;

	//-------- methods ---------
	void retrieveNumberOfNodes();
	
};

#endif