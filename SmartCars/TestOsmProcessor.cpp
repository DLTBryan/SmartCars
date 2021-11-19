#include "TestOsmProcessor.h"

#ifndef XMLCheckResult
#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); exit(a_eResult); }
#endif

void TestOsmProcessor::retrieveNumberOfNodes() {
	XMLDocument map;
	XMLElement* rootNode;
	XMLElement* currentWay;
	XMLElement* currentTag;
	XMLElement* currentNode;
	int counter;

	XMLError eResult = map.LoadFile(d_osmProcessor->fileName().c_str());
	XMLCheckResult(eResult);
	rootNode = map.FirstChildElement("osm");

	if (rootNode == nullptr) {
		exit(XML_ERROR_FILE_READ_ERROR);
	}

	currentWay = rootNode->FirstChildElement("way");

	while (currentWay != nullptr) {
		currentTag = currentWay->FirstChildElement("tag");
		counter = 0;
		while (currentTag != nullptr) {
			
			if (currentTag->Attribute("k") != nullptr && currentTag->Attribute("v") != nullptr) {
				string currentKAttribute(currentTag->Attribute("k"));
				string currentVAttribute(currentTag->Attribute("v"));
				
				if (currentKAttribute == "highway" && currentVAttribute == "primary") {
					currentNode = currentWay->FirstChildElement("nd");
					while (currentNode != nullptr) {
						counter++;
						currentNode = currentNode->NextSiblingElement("nd");
					}
					d_nbNodesPerRoad.push_back(counter);
					break;
				}
			}
			currentTag = currentTag->NextSiblingElement("tag");
		}
		currentWay = currentWay->NextSiblingElement("way");
	}

}

void TestOsmProcessor::testParsingOsm() {
	
	int i = 0; 
	
	while(i < d_osmProcessor->ways().size() && d_osmProcessor->ways().at(i)->noeuds().size() == d_nbNodesPerRoad.at(i)){
		i++;
	}

	if (i >= d_osmProcessor->ways().size()) { //at the end so no anomaly found
		cout << "Roads correctly parsed" << endl;
	}
	else {
		cout << "[-] Problem found at Rue nb " << i + 1 << endl;
		cout << "Normaly there are " << d_nbNodesPerRoad.at(i) << " Nodes\n" << d_osmProcessor->ways().at(i)->noeuds().at(i) << " Nodes were found" << endl;
		exit(1);
 	}

}

void TestOsmProcessor::tracePlan() {
	vector<Rue*> rues = d_osmProcessor->ways();

	/*float xmin = rues.at(0)->noeuds().at(0)->x();
	float xmax = xmin;
	float ymin = rues.at(0)->noeuds().at(0)->y();
	float ymax = ymin;
	for (Rue *rue : rues) {
		for (Noeud* noeud : rue->noeuds()) {
			if (noeud->x() < xmin)
				xmin = noeud->x();
			if (noeud->x() > xmax)
				xmax = noeud->x();
			if (noeud->y() < ymin)
				ymin = noeud->y();
			if (noeud->y() > ymax)
				ymax = noeud->y();
		}
	}

	opengraphsize(1000, 800);
	setbkcolor(BLACK);
	cleardevice();
	setcolor(GREEN);

	for (Rue* rue : rues) {
		for (Noeud* node : rue->noeuds()) {
			circle((xmax - node->x()) * 1000 / (xmax - xmin), (ymax - node->y()) * 800 / (ymax - ymin), 5);
			std::cout << "VOISINS :" << std::endl;
			setcolor(RED);
			for (Noeud* noeudvoisin : node->voisins()) {
				std::cout << noeudvoisin->x() << ", " << noeudvoisin->y() << std::endl;
				line((xmax - node->x()) * 1000 / (xmax - xmin), (ymax - node->y()) * 800 / (ymax - ymin), (xmax - noeudvoisin->x()) * 1000 / (xmax - xmin), (ymax - noeudvoisin->y()) * 800 / (ymax - ymin));
			}
			setcolor(GREEN);
		}
	}

	getch();
	closegraph();*/
}