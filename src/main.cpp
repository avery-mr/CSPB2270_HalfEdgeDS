#include "../include/HE_Mesh.h"
#include <vector>
#include <iostream>


//  this data structure assumes mesh is manifold and all half edges are oriented consistently



int main() {

    
    float edgeLength = 2.0;
    Mesh triangle("Triangle01");
    Mesh square("Square01");
    Mesh cube("Cube01");
    Mesh pyramid("Pyramid01");
    

    triangle.buildTriangle();
    square.buildSimpleSquare();
    cube.buildCube(4.0f);
    pyramid.buildPyramid(2.0f, 2.0f);


    triangle.printMeshData();
    square.printMeshData();
    cube.printMeshData();
    pyramid.printMeshData();
   
    triangle.exportToOBJ("obj/triangle.obj");
    square.exportToOBJ("obj/square.obj");
    cube.exportToOBJ("obj/cube.obj");
    pyramid.exportToOBJ("obj/pyramid.obj");


    return 0;
}
