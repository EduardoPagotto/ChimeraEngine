enum SIDE { IS_COPLANAR = 0, IS_INFRONT, IS_BEHIND, IS_SPANNING };

class Vertex {
  public:
    float x;
    float y;
    float z;
    Vertex();
    Vertex(float x, float y, float z);
    ~Vertex();
    float* getPoint();
    void setPoint(float x, float y, float z);
};

class Polygon {
  public:
    int id;
    Vertex vertices[3];
    Vertex normal;
    Vertex color;
    Polygon();
    ~Polygon();
    Vertex* getVertices();
    void setVertices(Vertex p1, Vertex p2, Vertex p3);
    Vertex getNormals();
    void setNormals(Vertex n);
    void setColor(Vertex col);
    Vertex getColor();
};

class Octahedron {
  public:
    Octahedron();
    ~Octahedron();
    Vertex vertices[6];
    Polygon faces[8];
    Vertex* getVertices();
    void setVertices(Vertex* verts);
    Polygon* getFaces();
    void setFaces(Polygon* faces);
};

class Cube {
  public:
    Cube();
    ~Cube();
    Polygon faces[10];
    Vertex normals[10];
    Vertex* getNormal();
    Vertex colors[10];
    void setNormal(Vertex* normal);
    Polygon* getFaces();
    void setFaces(Polygon* face);
    Vertex* getColors();
    void setColors(Vertex* cols);
};

class List {
  public:
    int nextindex;
    Polygon* planes;
    int noofplanes;
    List();
    void Add(Polygon* plane);
    Polygon* Next();
};

class Node {
  public:
    Polygon partition;
    List polygons;
    Node* frontNode;
    Node* backNode;
    Node();
    void BuildTree(Node* rootNode, List polygons);
    SIDE classifyPolygon(Polygon* cPolygon);
    float classify(Vertex* plane, Vertex* eye);
    void DrawTree(Node* finaltree, Vertex* eyepoint, List* finalfaces);
};