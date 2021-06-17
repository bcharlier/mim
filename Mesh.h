#ifndef MESH_H
#define MESH_H

#include "DeformableObject.h"


class Mesh : public DeformableObject {
public:

    Mesh() {}

    ~Mesh() {}

    std::vector <Vec3Df> &getVertices() { return vertices; }

    const std::vector <Vec3Df> &getVertices() const { return vertices; }

    std::vector <Triangle> &getTriangles() { return triangles; }

    const std::vector <Triangle> &getTriangles() const { return triangles; }

    unsigned int getVerticesNb() { return vertices.size(); }

    void draw();

    void draw(std::vector<bool> &selected, std::vector<bool> &fixed);

    void recomputeNormals();

    void update();

    void clear();

    void invertNormal() { normalDirection *= -1; }

protected:
    void init();

    void computeBB();

    void collectOneRing(std::vector <std::vector<unsigned int>> &oneRing) const;

    void computeTriangleNormals();

    Vec3Df computeTriangleNormal(int t);

    void computeVerticesNormals();

    void glTriangle(unsigned int i);

    void sortFaces(FacesQueue &facesQueue);
};

#endif // MESH_H
