#ifndef CURVE_H
#define CURVE_H

#include "Objects/DeformableObject.h"
#include "Objects/Edge.h"


class Curve : public DeformableObject< Edge > {
public:

    Curve() {}

    ~Curve() {}

    std::vector <Vec3Df> &getVertices() { return vertices; }

    const std::vector <Vec3Df> &getVertices() const { return vertices; }

    std::vector <Edge> &getTriangles() { return triangles; }

    const std::vector < Edge > &getTriangles() const { return triangles; }

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


#endif // CURVE_H
