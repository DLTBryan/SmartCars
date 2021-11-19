#include "OsmProcessor.h"

#ifndef XMLCheckResult
#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); exit(a_eResult); }
#endif

OsmProcessor::OsmProcessor(string fileName) {

	XMLError eResult = d_map.LoadFile(fileName.c_str());
	XMLCheckResult(eResult);

	d_fileName = fileName;
	d_rootNode = d_map.FirstChildElement("osm");

	if (d_rootNode == nullptr) {
		exit(XML_ERROR_FILE_READ_ERROR);
	}

	firstParsing();
}

void OsmProcessor::firstParsing() {

	XMLElement* currentWay = d_rootNode->FirstChildElement("way");

	if (currentWay == nullptr) {
		exit(XML_ERROR_PARSING_ELEMENT);
	}

	while (currentWay != nullptr) {
		XMLElement* currentTag = currentWay->FirstChildElement("tag");
		
		Rue* rue = new Rue(currentWay->Attribute("id"));
		
		while (currentTag != nullptr) {
			if (currentTag->Attribute("k") != nullptr && currentTag->Attribute("v") != nullptr) {
				string currentKAttribute(currentTag->Attribute("k"));
				string currentVAttribute(currentTag->Attribute("v"));
				
				if (isValidRoad(currentKAttribute, currentVAttribute)) {
			
					processKandVAttribute(currentWay, rue);
			
					linkRueNodes(rue);

					//cout << rue->noeuds().size() << endl;

					d_ways.push_back(rue); //addinf the newly created rue in the vector of ways (rues)
					break;
				}
			}

			currentTag = currentTag->NextSiblingElement("tag");
		}
		currentWay = currentWay->NextSiblingElement("way");
	}
	
	cout << "Finished First Parsing" << endl;
	cout << d_ways.at(1)->noeuds().size() << endl;

	printRue(1);
	linkNeighbooringNodes();
	cout << "--------------------------------------------------------" << endl;
	printRue(1);
	
	cout << d_ways.at(1)->noeuds().at(d_ways.at(1)->noeuds().size() - 2)->voisins().at(1)->voisins().size() << endl;
	cout << d_ways.at(1)->noeuds().at(d_ways.at(1)->noeuds().size() - 2)->voisins().at(1)->id() << endl;
	cout << d_ways.at(1)->noeuds().at(d_ways.at(1)->noeuds().size() - 2)->voisins().at(1)->voisins().size() << endl;
	cout << d_ways.at(1)->noeuds().at(d_ways.at(1)->noeuds().size() - 2)->voisins().at(1)->voisins().at(1)->voisins().at(0)->id() << endl;
	cout << d_ways.at(1)->noeuds().at(d_ways.at(1)->noeuds().size() - 2)->voisins().at(1)->voisins().at(1)->voisins().at(1)->id() << endl;

	cout << d_ways.at(1)->noeuds().at(d_ways.at(1)->noeuds().size() - 1)->voisins().at(1)->voisins().size() << endl;
	cout << d_ways.at(1)->noeuds().at(d_ways.at(1)->noeuds().size() - 1)->voisins().at(1)->id() << endl;
	cout << "First voisin " << d_ways.at(1)->noeuds().at(d_ways.at(1)->noeuds().size() - 1)->voisins().at(1)->voisins().at(0)->id() << endl;
	cout << "Second voisin " << d_ways.at(1)->noeuds().at(d_ways.at(1)->noeuds().size() - 1)->voisins().at(1)->voisins().at(1)->id() << endl;
	
}

void OsmProcessor::linkNeighbooringNodes() {
	//TODO Finir 
	//TODO peut être mettre les rues parents du noeud ?

	Rue* currentRue = nullptr;
	int j = 0;
	Noeud* n;
	 
	vector<int> tempIndex;

	for (int i = 0; i < d_ways.size(); i++) { //iterate over all the rues
		currentRue = d_ways.at(i);

		for (int j = 0; j < currentRue->noeuds().size(); j++) { //iterate over the array of nodes of the current rue 
			n = currentRue->noeuds().at(j);
			if (!n->isProcessed()) {
				updateNode(n, i);
				n->processed(true);
			}
			
		}
		
	}
}

void OsmProcessor::updateNode(Noeud* nodeToUpdate, int currentRueIndex) {
	Rue* currentRue = nullptr;

	Noeud* tempNode = nullptr;
	vector<Noeud*> tempNodes; // put the nodes to be linked to
	
	string id = nodeToUpdate->id();
	int index;

	for (int i = 0; i < d_ways.size(); i++) {
		if (currentRueIndex != i) {
			
			currentRue = d_ways.at(i);
			tempNodes = currentRue->noeuds();
			auto node = find_if(tempNodes.begin(), tempNodes.end(), [&](const Noeud* noeud) {
				return noeud->id() == id;
			});
			
			if (node != tempNodes.end()) {
				index = node - tempNodes.begin();
				
				for (Noeud* n: tempNodes.at(index)->voisins()) {
					nodeToUpdate->ajouteVoisin(n);
				}
				
				tempNode = tempNodes.at(index);
				//tempNode->~Noeud(); //TODO: find way to deallocate memory of the previous node
				tempNodes.at(index) = nodeToUpdate;
				
			}
		}
	}
	
}

void OsmProcessor::linkRueNodes(Rue* rue) {
	//TODO: Link nodes with next and previous
	Noeud* temp = rue->noeuds().at(0);

	temp->ajouteVoisin(rue->noeuds().at(1));//first node only has only 1 neighboor on the same street (next)

	for (int i = 1; i < rue->noeuds().size() - 1; i++) {
		temp = rue->noeuds().at(i);
		temp->ajouteVoisin(rue->noeuds().at(i - 1)); //adding next voisin
		temp->ajouteVoisin(rue->noeuds().at(i + 1)); //adding previous voisin
		
	}

	temp = rue->noeuds().at(rue->noeuds().size() - 1);
	temp->ajouteVoisin(rue->noeuds().at(rue->noeuds().size() - 2));;//last node only has only 1 neighboor on the same street (previous)

}


void OsmProcessor::processKandVAttribute(XMLElement* currentWay, Rue* currentRue) {
	
	XMLElement* currentNode = currentWay->FirstChildElement("nd");
	
	while (currentNode != nullptr) {
		
		Noeud* node = makeNode(currentNode->Attribute("ref"), currentWay);

		currentRue->ajouteNoeud(node);
		currentNode = currentNode->NextSiblingElement("nd");
		
	}

	
}


Noeud* OsmProcessor::makeNode(string idNode, XMLElement* currentWay) {
	XMLElement* currentNode = d_rootNode->FirstChildElement("node");
	
	Noeud* node = nullptr;

	while (currentNode != nullptr) {

		if (currentNode->Attribute("id") != nullptr) {

			if (string(currentNode->Attribute("id")) == idNode) {

				if (currentNode->Attribute("lat") != nullptr && currentNode->Attribute("lon")) {

					node = new Noeud(stof(currentNode->Attribute("lat")),
									 stof(currentNode->Attribute("lon")),
									 idNode);
					
					findNodeOnOtherRoutes(node, currentWay);
					//cout << "idNode => " << idNode << endl;
				}
				
			}

		}
		currentNode = currentNode->NextSiblingElement("node");
	}
	
	return node; 
}


void OsmProcessor::findNodeOnOtherRoutes(Noeud* node, XMLElement* way) {
	
	XMLElement* currentWay = d_rootNode->FirstChildElement("way");
	XMLElement* currentNd;
	XMLElement* currentTag;

	while (currentWay != nullptr){
		
		if (currentWay->Attribute("id") != way->Attribute("id")) {
			currentTag = currentWay->FirstChildElement("tag");

			while (currentTag != nullptr) {

				if (currentTag->Attribute("k") != nullptr && currentTag->Attribute("v")) {
					string currentKAttribute(currentTag->Attribute("k"));
					string currentVAttribute(currentTag->Attribute("v"));
					//strcmp(currentTag->Attribute("k"), "highway") == 0 && strcmp(currentTag->Attribute("v"), "primary") == 0

					if (isValidRoad(currentKAttribute, currentVAttribute)) {

						currentNd = currentWay->FirstChildElement("nd");

						while (currentNd != nullptr && strcmp(node->id().c_str(), currentNd->Attribute("ref")) != 0) {
							currentNd = currentWay->NextSiblingElement("nd");
						}

						if (currentNd != nullptr) { //found in a way
							
							if (currentNd->PreviousSiblingElement("nd") != nullptr) { //adding previous 
								node->addIdNeighbooringNode(currentNd->PreviousSiblingElement("nd")->Attribute("ref")); //id
							}
							
							if (currentNd->NextSiblingElement("nd") != nullptr) { //adding next
								node->addIdNeighbooringNode(currentNd->NextSiblingElement("nd")->Attribute("ref")); //id
							}
						}
						break; //break upper tag while loop ==> avoiding useless iterations
					}
					
				}
				currentTag = currentTag->NextSiblingElement("tag");
			}
		}
		currentWay = currentWay->NextSiblingElement("way");
	}

}


void OsmProcessor::printRue(int index) {

	int i = 1;

	for (Noeud* n : d_ways.at(index)->noeuds()) {
		cout << "id Rue => " << n->id() << endl;
		cout << "nb voisins => " << n->voisins().size() << endl;
		cout << "Voisins : " << endl;
		for (Noeud* nVoisin : n->voisins()) {
			cout << "id  voisin " << i << " => " << nVoisin->id() << endl;
			i++;
		}
		i = 0;
		cout << "------------" << endl;
	}
}

bool OsmProcessor::isValidRoad(string attributeK, string attributeV) {
	//return attributeK == "highway" && attributeV == "primary"
	return attributeK == "highway" && (attributeV == "primary" || attributeV == "secondary");
}