# CSPB 2270 Final Project: Half-Edge Data Structure

**Author:** Mitch Avery  
**Date:** 08/09/2024

## Project Overview

This project implements a basic half-edge data structure in C++. The original intent was to ultimately render primitive shapes using OpenGL as well as implement a very simple GUI, but due to time constraints and the complexity of the OpenGL pipeline, the project was refocused on constructing and traversing simple 3D meshes using the half-edge data structure.

## Initial Goals and Adjustments

I initially aimed to:
- Build several primitve 3d shapes using the half-edge data structure.
- Implement code to traverse a 3d data structure.
- Use OpenGL and GLFW to set up a basic rendering pipeline.

### Challenges and Revisions
- **Rendering Complexity:** Setting up modern OpenGL was more difficult than anticipated.  I had trouble finding enough documentation on older OpenGL or other graphics libraries and I eventually decided to dismiss the idea of rendering since I wasn't able to learn and implement the necessary shaders and pipeline within the available time. The project was adjusted to focus solely on the data structure, with a goal of building and manipulating simple primitive shapes.
- **Data Output:** In lieu of implementing my own rendering pipeline I opted to write the mesh data to .obj format to load and view in an external application. 
- **Data Structure Design:** I began with the idea of using classes for each element (vertices, edges, faces, half-edges) to encapsulate behavior. However, as the project progressed, I found it more straightforward to incorporate all necessary methods into a single `Mesh` class. If I were to start over, I would likely use structs for simplicity.
- **Functionality:** Most of the methods I wrote for my mesh class were intended to navigate and traverse the mesh, to debug any issues in the building process, and to ensure the mesh was ultimately built correctly and clean.  This required stitching together half edge pairs and ensuring the number of vertices, faces and open edges were appropriate.
## Data Structure Implementation
For simplicity's sake, this version of the half-edge data structure requires a few underlying assumptions to be true in order for it function correctly.  
### Assumptions
- **Manifold Meshes:** The data structure assumes that all meshes are manifold.
- **Consistent Orientation:** All faces are oriented such that half-edge loops around each face flow in the same direction relative to the face.
- **Triangles Only:** The current implementation only supports triangular faces. The `createFace()` function, therefore, requires exactly three vertices.
### Key Member Functions
- **createVert()** Creates a vertex class and assigns x, y and z coordinates as well as a unique index number, then appends it to the end of a vector;
- **createHalfEdge()** Creates a half edge starting from a vertex and associated with a single face, and appends it to the end of a vector.  The half-edge also contains pointers to the next half-edge, a twin/opposite half-edge, and a non-directional edge.
- **createFace()** Takes 3 vertices, creates half-edges between them that define the border of the face, and append the created face to the end of a vector with a unique index number.  This function also creates the necessary undirected edges. 
- **bindHalfEdgePairs()** Iterates through list of half-edges searching for twin pairs (where he1 points to he2 and he2 points to he1).  If no twin is found the associated undirected edge is labeled a border edge.
- **getAdjacentVerts** Takes a vertex index and returns a vector of adjacent vertices (that are within one edge-length distance);
- **build functions** Supply vertex coordinates and face creation order to build several very simple primitives.  Plane, Cube and Pyramid take variable parameters to control the size of the objects.



### Design Decisions
- **Public Data and Methods:** For simplicity, all data and methods in the project are public and accessible. This decision was made to focus on the core functionality rather than access control.
- **Additional Data:** Some additional data members, such as `Edge::isBorder`, were included in anticipation of future functionality. For instance, including indices for each vertex and face allows O(1) retrieval from their respective vectors, but may complicate operations like deletion or reordering.

## Issues and Challenges

### Mesh Data
- **Vertex Order** Supplying the coordinates of the vertices was rather straightforward, although I often had to draw out diagrams on paper or else use 3d software get a clear picture of what I was creating.  I am used to working with quadrilaterals so having to create even a simple cube with triangles became hard to visualize.  The real struggle was creating the faces with the vertices in the correct order to ensure the half-edge directions were consistent for every face and edge.  This took several attempts, diagrams and debugging to get right.

### Vector Storage
- **Pointer Invalidation:** Storing half-edge data in vectors caused issues when vectors resized, invalidating pointers. Reserving enough space in vectors mitigated this problem temporarily but is not a scalable solution. Alternative data structures like `std::list` or smart pointers may provide more robust solutions for dynamic mesh handling.

## Future Considerations
- **Improved Data Structures:** Future iterations of this project could explore more sophisticated data structures or libraries like `deque` for better memory management and pointer stability.
- **Learning OpenGL:** A more extended project timeline might allow for deeper exploration of OpenGL or other graphic libraries or rendering pipelines.
- **Additional Member Functions:** 3D meshes require manipulation.  These could include adding and removing vertices/faces, subdividing, scaling, rotating, etc.
