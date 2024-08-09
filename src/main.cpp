#include "../include/HE_Mesh.h"
#include <vector>
#include <iostream>


//  this data structure assumes mesh is manifold and all half edges are oriented consistently



int main() {

    Mesh mesh;
    float edgeLength = 2.0;
    mesh.clearMeshData();
    mesh.buildSimpleSquare();

    std::cout << "vertices: " << mesh.getVertCount() << std::endl;
    std::cout << "faces: " << mesh.getFaceCount() << std::endl;
    std::cout << "halfEdges: " << mesh.halfEdges.size() << std::endl;
    mesh.printVertCoords();
    mesh.printHalfEdgeData();

    //std::cout << "Face index: " << mesh.halfEdges[1].face->idx << std::endl;

    mesh.bindHalfEdgePairs();

    for (int i = 0; i < mesh.halfEdges.size(); i++) {
        if (mesh.halfEdges[1].next) {
            std::cout << "has next" << std::endl;

        }
        else {
            std::cout << "no next" << std::endl;
        }
    }
    for (int i = 0; i < mesh.halfEdges.size(); i++) {
        if (mesh.halfEdges[i].twin) {
            std::cout << "has twin" << std::endl;

        }
        else {
            std::cout << "no twin" << std::endl;
        }
    }

    for (int i = 0; i < mesh.halfEdges.size(); i++) {
        if (mesh.halfEdges[i].isBorder) {
            std::cout << "border edge" << std::endl;

        }
        else {
            std::cout << "internal edge" << std::endl;
        }
    }



    return 0;
}
