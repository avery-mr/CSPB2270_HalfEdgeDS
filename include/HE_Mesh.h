#ifndef HE_EDGE_H
#define HE_EDGE_H

#include <vector>
#include <fstream>
#include <sstream>
#include <string>



class Vert;
class Edge;
class HalfEdge;
class Face;

class Vert {
public:
	float x, y, z;
	HalfEdge* hEdge;
	int idx;  //maybe give each vert a unique index?
	Vert(float x, float y, float z, int idx);
};

class Edge {
public:
	HalfEdge* hEdge;
	bool isBorder;

	Edge();
};

class Face {
public:
	HalfEdge* edge;
	int idx;

	Face();
};

class HalfEdge {
public:
	Vert* vert;  // pointer to the vertex that half-edge starts from
	HalfEdge* twin; // pointer to the twin half-edge
	HalfEdge* next; // pointer to the next half-edge around the face
	Face* face; //pointer to associated face
	Edge* edge; //pointer to associated undirected edge

	HalfEdge();
};

class Mesh {
public:
	std::string name;
	std::vector<Vert> vertices;
	std::vector<HalfEdge> halfEdges;
	std::vector<Edge> edges;
	std::vector<Face> faces;
	
	Mesh(const std::string& name);

	// CLEAR ALL ABOVE VECTORS
	void clearMeshData();

	//INITIALIZE VERTEX WITH COORDINATE AND INDEX DATA
	void createVert(float x, float y, float z, int idx);

	//INITIALIZE HALF EDGE WITH ASSOCIATED VERTEZ AND FACE DATA
	HalfEdge* createHalfEdge(Vert* vert, Face* face);

	//INITIALIZE TRIANGLE FACE WITH 3 VERTICES AND ASSIGN INDEX
	void createFace(int vert1, int vert2, int vert3, int idx);

	//Find all twin half edges and point them to eachother.  Implement after building mesh.
	void bindHalfEdgePairs();

	int getVertCount() const;
	int getFaceCount() const;


	// DEBUG METHODS ------------------
	void printVertCoords();
	void printHalfEdgeData();
	void printMeshData();
	// ---------------------------------

	

	// Find all neighbor vertices of a vertex
	std::vector<Vert*> getAdjacentVerts(int vertIdx);

	// Find all adjacent faces of a vertex
	std::vector<Face*> getAdjacentFaces(int vertIdx);


	// BUILD PRIMITIVE MESH METHODS
	void buildTriangle();
	void buildSimpleSquare();
	void buildPlane(float edgeLength);
	void buildCube(float edgeLength);
	void buildPyramid(float baseEdgeLength, float height);


	// EXPORT TO OBJ MAYBE??
	void exportToOBJ(const std::string& filename);

	// ---------------------- TO DO -----------------------------------
	// remove face and combine 3 associated vertices to point at center of face
	void collapseFace(Face* face);
	// 
	// delete vertex and its associated edges and faces
	void deleteVertex(int vertIdx);
	void deleteFace(Face* face);
};
#endif