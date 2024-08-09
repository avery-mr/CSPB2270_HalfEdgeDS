CSPB 2270 Final Project
Half Edge Data Structure
Mitch Avery
08/09/2024

Initial notes:
I spent a significant amount of time trying to render anything.  I naivly planned on using openGL and GLFW to set up a primitive rendering pipeline but it was far too complicated to set up with my limited understanding.  I had to move the goalposts up quite a bit and settle on trying to just build a few simple primitive shapes.  

I initially decided to use classes rather than simple structs in anticipation of encapsulating behavior and methods specific to each element (for example, vertices could have their own methods for setting shader info or normal direction, faces could have their own methods for collapsing, etc.).  Given the simplicity of my data structure I found it much easier to incorporate all necessary methods in a single mesh class, so if I were to start over now I would probably employ structs for vertices, edges, faces, and halfedges.  

I opted to spend my limited time focusing on the data structure itself and important functions rather than implementing safeguards against potential problems such as bad meshes etc.  Therefore, this projects assumes the meshes are manifold and that all faces have the same orientation (all half edge loops around faces flow in the same direction in relation to the face.  

For simplicity I decided to build this data structure to only create and allow triangles.  This way my createFace() function would take 3 and only 3 verticies.  

I did not bother at this point to control access to any particular data, thus all data and methods are public and accessible.  
I added additional data to some of the classes in anticipation of future functionality, or simply because it seemed like it might be helpful.  For example, I'm not completely sure including and index for each vertex and face is entirely necessary.  On the one hand it makes retrieval from their vector O(1), but it also means that deleting, adding, or otherwise rearranging would potentially mean restructuring or reassigning indices.  
Other attributs such as HalfEdge::isBorder are not currently useful at all in my project, but I could forsee usefulness, although it might be an attribute better suited for undirected edges.


Issues:
Modern OpenGL is REALLY hard.  Shaders, etc, entire pipeline I can't learn in 3 days.
Vintage OpenGL and associated libraries have very little documentation to learn quickly.
Few other options for other graphics libraries in C++.

Storing half edge data in vectors - seems appropriate but running into issues.
Dynamically increasing vector size and reallocating more space is destroying some pointers.
Problem seems to be fixed if I manually reserve enough space in each vector, but that solution isn't scalable for much larger meshes. Not sure what the solution is there except for maybe not storing pointers directly in a vector that may be changing size.