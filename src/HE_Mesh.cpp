#include <iostream>
#include "../include/HE_Mesh.h"



Vert::Vert(float x, float y, float z, int idx) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->idx = idx;
	this->hEdge = nullptr;
}

Edge::Edge() {
	this->hEdge = nullptr;
	isBorder = true;
}

Face::Face() {
	this->edge = nullptr;
	this->idx = -1;
}

HalfEdge::HalfEdge() {
	this->vert = nullptr;
	this->twin = nullptr;
	this->next = nullptr;
	this->face = nullptr;
	this->edge = nullptr;
}

Mesh::Mesh(const std::string& name) {
	// for debugging: reserving space in each vector to avoid having to reallocate space and potentially screw up pointers
	// using arbitrary small values that can hold data for simple primitives only
	this->name = name;
	vertices.reserve(20);
	halfEdges.reserve(100);
	edges.reserve(50);
	faces.reserve(20);
}

void Mesh::clearMeshData() {
	vertices.clear();
	halfEdges.clear();
	edges.clear();
	faces.clear();
}

void Mesh::createVert(float x, float y, float z, int idx) {
	Vert newVert(x, y, z, idx);
	vertices.push_back(newVert);
	//std::cout << "Created Vertex: " << idx << " at " << &vertices.back() << std::endl;

	return;
}

HalfEdge* Mesh::createHalfEdge(Vert* vert, Face* face) {
	halfEdges.push_back(HalfEdge());
	HalfEdge* newHE = &halfEdges.back();
	newHE->vert = vert;
	newHE->face = face;
	return &halfEdges.back();
}

void Mesh::createFace(int vert1, int vert2, int vert3, int idx) {
	faces.push_back(Face());
	Face* face = &faces.back();

	//std::cout << "Vertex 1: " << &vertices[vert1] << std::endl;
	//std::cout << "Vertex 2: " << &vertices[vert2] << std::endl;
	//std::cout << "Vertex 3: " << &vertices[vert3] << std::endl;

	HalfEdge* he1 = createHalfEdge(&vertices[vert1], face);
	HalfEdge* he2 = createHalfEdge(&vertices[vert2], face);
	HalfEdge* he3 = createHalfEdge(&vertices[vert3], face);

	// set next pointers
	he1->next = he2;
	he2->next = he3;
	he3->next = he1;

	face->edge = he1;
	face->idx = idx;
	// create edges, link half-edges to them
	edges.push_back(Edge());
	Edge* edge1 = &edges.back();
	edge1->hEdge = he1;
	he1->edge = edge1;

	edges.push_back(Edge());
	Edge* edge2 = &edges.back();
	edge2->hEdge = he2;
	he2->edge = edge2;

	edges.push_back(Edge());
	Edge* edge3 = &edges.back();
	edge3->hEdge = he3;
	he3->edge = edge3;

	// set twin pointers to null for now
	he1->twin = nullptr;
	he2->twin = nullptr;
	he3->twin = nullptr;

	// link vertices to one of their outgoing half edges
	vertices[vert1].hEdge = he1;
	vertices[vert2].hEdge = he2;
	vertices[vert3].hEdge = he3;

	return;
}

void Mesh::bindHalfEdgePairs() {
	//std::cout << "running bind pairs...." << std::endl;
	for (auto& he1 : halfEdges) { // loop through all halfEdges
		bool foundTwin = false; 
		if (he1.twin == nullptr) {  // if twin already set, no need to look again
			//std::cout << " --- Checking vert " << he1.vert->idx << " ---> " << he1.next->vert->idx << std::endl;
			for (auto& he2 : halfEdges) {  // loop through all halfEdges a second time to find matching opposite
				//std::cout << " --------- " << he2.vert->idx << " ---> " << he2.next->vert->idx << std::endl;
				if (he1.vert == he2.next->vert && he2.vert == he1.next->vert) {
					//std::cout << "congratulations, its twins!" << std::endl;
					he1.twin = &he2;  // set he1 twin to he2 and vice versa
					he2.twin = &he1;
					foundTwin = true;  // ran into segment fault here, forgot to end the loop
					break;
				}
			}
		}
		else {
			foundTwin = true;  // if halfEdge twin already set, make sure fount twin flag is true so isBorder doesn't get set
		}
		if (foundTwin) {
			he1.edge->isBorder = false;
			
		}
	}
	return;
}

int Mesh::getVertCount() const {
	return vertices.size();
}

int Mesh::getFaceCount() const {
	return faces.size();
}

void Mesh::printVertCoords() {
	std::cout << "\nMesh vertex coordinates: " << std::endl;
	if (vertices.size() > 0) {
		for (auto& vert : vertices) {
			std::cout << vert.idx << ": " << vert.x << ", " << vert.y << ", " << vert.z << std::endl;
		}
	}
	else {
		std::cout << "No vertices to print...." << std::endl;
	}
}

void Mesh::printHalfEdgeData() {
	std::cout << "\nMesh half edge data: " << std::endl;
	if (halfEdges.size() > 0) {
		for (int i = 0; i < halfEdges.size(); i++) {
			std::cout << "HalfEdge " << i << " vertex = " << halfEdges[i].vert->idx << std::endl;
			std::cout << "HalfEdge " << i << " points to vertex " << halfEdges[i].next->vert->idx << std::endl;
		}
	}
	else {
		std::cout << "No half edges to print....." << std::endl;
	}
}

void Mesh::printMeshData() {
	std::cout << "\nName: " << name << std::endl;
	//std::cout << "\n ------- MAIN DATA -------" << std::endl;
	std::cout << "Vertex Count: " << vertices.size() << std::endl;
	std::cout << "Edge Count: " << edges.size() << std::endl;
	std::cout << "Half Edge Count: " << halfEdges.size() << std::endl;
	std::cout << "Face Count: " << faces.size() << std::endl;
	//std::cout << "\n------- ADDITIONAL DATA -------" << std::endl;
	int unpairedHE = 0;
	for (auto& he : halfEdges) {
		if (!he.twin) {
			unpairedHE++;
		}
	}
	std::cout << "Unpaired Half Edges: " << unpairedHE << std::endl;
	int borderCount = 0;
	for (auto& edge : edges) {
		if (edge.isBorder == true) {
			borderCount++;
		}
	}
	std::cout << "Open (Border) Edge Count: " << borderCount << std::endl;
	if (borderCount == 0) { std::cout << "Mesh is watertight" << std::endl; }
	else { std::cout << "Mesh is not watertight" << std::endl; }

}


std::vector<Vert*> Mesh::getAdjacentVerts(int vertIdx) {
	std::vector<Vert*> retVerts;
	Vert* v = &vertices[vertIdx];
	HalfEdge* startEdge = v->hEdge;
	HalfEdge* currentEdge = startEdge;
	do {
		retVerts.push_back(currentEdge->next->vert);
		if (currentEdge->twin) {   // check if edge is boundary.  if no twin half edge exists, exit the loop  (or use isBorder property)
			currentEdge = currentEdge->twin->next;   
		}
		else {
			break;
		}
		
	} while (currentEdge != startEdge);

	return retVerts;
}


std::vector<Face*> Mesh::getAdjacentFaces(int vertIdx) {
	std::vector<Face*> retFaces;
	Vert* v = &vertices[vertIdx];
	HalfEdge* startEdge = v->hEdge;
	HalfEdge* currentEdge = startEdge;
	do {
		retFaces.push_back(currentEdge->face);
		if (currentEdge->twin) {
			currentEdge = currentEdge->twin->next;
		}
		else {
			break;
		}

	} while (currentEdge != startEdge);

	return retFaces;
}

void Mesh::buildTriangle() {
	createVert(-2.0, -1.0, 0.0, 0);
	createVert(0.0, 2.0, 0.0, 1);
	createVert(2.0, -1.0, 0.0, 2);

	createFace(0, 1, 2, 0);

	bindHalfEdgePairs();
}

void Mesh::buildSimpleSquare() {
	createVert(-1.0f, -1.0f, 0.0f, 0);
	createVert(-1.0f, 1.0f, 0.0f, 1);
	createVert(1.0f, 1.0f, 0.0f, 2);
	createVert(1.0f, -1.0f, 0.0f, 3);

	createFace(0, 1, 3, 0); 
	createFace(1, 2, 3, 1); 

	bindHalfEdgePairs();
}
void Mesh::buildPlane(float edgeLength) {
	float halfL = edgeLength / 2.0;

	createVert(-halfL, -halfL, -halfL, 0);
	createVert(halfL, -halfL, -halfL, 1);
	createVert(-halfL, halfL, -halfL, 2);
	createVert(halfL, halfL, -halfL, 3);

	createFace(0, 1, 2, 0); // Correct order
	createFace(0, 2, 3, 1); // Correct order

	bindHalfEdgePairs();
}
void Mesh::buildCube(float edgeLength) {
	float halfL = edgeLength / 2.0f;

	createVert(-halfL, -halfL, -halfL, 0);
	createVert(halfL, -halfL, -halfL, 1);
	createVert(-halfL, halfL, -halfL, 2);
	createVert(halfL, halfL, -halfL, 3);
	createVert(-halfL, -halfL, halfL, 4);
	createVert(halfL, -halfL, halfL, 5);
	createVert(-halfL, halfL, halfL, 6);
	createVert(halfL, halfL, halfL, 7);

	// face (-z)
	createFace(0, 2, 1, 0);
	createFace(1, 2, 3, 1);
	// face (+x)
	createFace(1, 3, 5, 2);
	createFace(5, 3, 7, 3);
	// face (+z)
	createFace(5, 7, 4, 4);
	createFace(4, 7, 6, 5);
	// face (-x)
	createFace(4, 6, 0, 6);
	createFace(0, 6, 2, 7);
	// face (+y)
	createFace(2, 6, 3, 8);
	createFace(3, 6, 7, 9);
	// face (-y)
	createFace(0, 1, 4, 10);
	createFace(4, 1, 5, 11);

	//bind half edges to their twins
	bindHalfEdgePairs();

	return;
}

void Mesh::buildPyramid(float baseEdgeLength, float height) {
	float halfL = baseEdgeLength / 2.0f;
	createVert(-halfL, -halfL, 0, 0); 
	createVert(halfL, -halfL, 0, 1); 
	createVert(halfL, halfL, 0, 2); 
	createVert(-halfL, halfL, 0, 3); 
	createVert(0, 0, height, 4);           

	//Base face
	createFace(0, 1, 2, 0);
	createFace(0, 2, 3, 1);

	// Side faces
	createFace(1, 0, 4, 2); 
	createFace(1, 4, 2, 3); 
	createFace(2, 4, 3, 4); 
	createFace(0, 3, 4, 5); 

	//bind half edges to their twins
	bindHalfEdgePairs();
}


// DID SOME COPYING AND PASTING FOR THIS PART FROM VARIOUS SOURCES
void Mesh::exportToOBJ(const std::string& filename) {
	std::ofstream file(filename);
	
	for (const auto& vert : vertices) {
		file << "v " << vert.x << " " << vert.y << " " << vert.z << "\n";
	}

	for (const auto& face : faces) {
		HalfEdge* startEdge = face.edge;
		HalfEdge* currentEdge = startEdge;
		std::vector<int> vIdx;

		do {
			// add 1 cuz obj indeces are 1 based
			vIdx.push_back(currentEdge->vert->idx + 1);
			currentEdge = currentEdge->next;
		} while (currentEdge != startEdge);

		// same thing, 
		file << "f";
		for (int idx : vIdx) {
			file << " " << idx;
		}
		file << "\n";
	}

	file.close();
	return;
}

// ---------------------- TO DO ---------------------------------------------
void Mesh::collapseFace(Face* face) {
	return;
}
void Mesh::deleteVertex(int vertIDx) {
	return;
}

void Mesh::deleteFace(Face* face) {
	return;
}

