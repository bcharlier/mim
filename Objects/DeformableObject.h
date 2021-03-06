#ifndef DEFORMABLEOBJECT_H
#define DEFORMABLEOBJECT_H

#include "Utils/Vec3D.h"
#include <queue>

template <class faces>
class DeformableObject {
public:

    DeformableObject() : normalDirection(1.) {}

    DeformableObject(std::vector <Vec3Df> &vertices, std::vector < faces > &triangles) : vertices(vertices),
                                                                                         triangles(triangles),
                                                                                         normalDirection(1.) {
        update();
    }

    ~DeformableObject() {}

    std::vector <Vec3Df> &getVertices() { return vertices; }

    const std::vector <Vec3Df> &getVertices() const { return vertices; }

    std::vector <faces> &getTriangles() { return triangles; }

    const std::vector <faces> &getTriangles() const { return triangles; }

    unsigned int getVerticesNb() { return vertices.size(); }

    void draw();

    void draw(std::vector<bool> &selected, std::vector<bool> &fixed);

    void recomputeNormals();

    void update();

    void clear();

    typedef std::priority_queue <std::pair<float, int>, std::deque<
            std::pair < float, int>> , std::greater <std::pair<float, int>> >
    FacesQueue;

    void invertNormal() { normalDirection *= -1; }

protected:
    void init();

    void computeBB();

    void collectOneRing(std::vector <std::vector<unsigned int>> &oneRing) const;

    void computeTriangleNormals();

    Vec3Df computeTriangleNormal(int t);

    void computeVerticesNormals();

    void glTriangle(unsigned int i);

    std::vector <Vec3Df> vertices;
    std::vector <faces> triangles;

    std::vector <Vec3Df> normals;
    std::vector <Vec3Df> verticesNormals;

    Vec3Df BBMin;
    Vec3Df BBMax;
    float radius;

    int normalDirection;
};

#endif // DEFORMABLEOBJECT_H
