#include "Curve.h"
#include <algorithm>
#include <float.h>

void Curve::computeBB() {

    BBMin = Vec3Df(FLT_MAX, FLT_MAX, FLT_MAX);
    BBMax = Vec3Df(-FLT_MAX, -FLT_MAX, -FLT_MAX);

    for (unsigned int i = 0; i < vertices.size(); i++) {
        const Vec3Df &point = vertices[i];
        for (int v = 0; v < 3; v++) {
            float value = point[v];
            if (BBMin[v] > value) BBMin[v] = value;
            if (BBMax[v] < value) BBMax[v] = value;
        }
    }

    radius = (BBMax - BBMin).norm();
}

void Curve::update() {

    if ((vertices.size() >0) && ( triangles.size() == 0)) {
        triangles.clear();
        triangles.resize(vertices.size() - 1);

        for (unsigned int i = 0; i < vertices.size() - 1; i++) {
            triangles[i].setVertices(i, i+1);
        }
    }

    computeBB();
    recomputeNormals();
    std::cout << "Curve : " << vertices.size() << " vertices, " << triangles.size() << " edges " << std::endl;
}

void Curve::clear() {
    vertices.clear();

    triangles.clear();

    normals.clear();
    verticesNormals.clear();

}


void Curve::recomputeNormals() {

    computeTriangleNormals();
    computeVerticesNormals();

}

void Curve::computeTriangleNormals() {

    normals.clear();

    for (unsigned int i = 0; i < triangles.size(); i++) {
        normals.push_back(computeTriangleNormal(i));
    }

}

Vec3Df Curve::computeTriangleNormal(int id) {

    const Edge &t = triangles[id];
    Vec3Df tangent = (vertices[t.getVertex(1)] - vertices[t.getVertex(0)]);
    tangent.normalize();
    return tangent;

}


void Curve::computeVerticesNormals() {

    verticesNormals.clear();
    verticesNormals.resize(vertices.size(), Vec3Df(0., 0., 0.));

    for (unsigned int t = 0; t < triangles.size(); ++t) {
        Vec3Df const &tri_normal = normals[t];

        verticesNormals[triangles[t].getVertex(0)] += tri_normal;
        verticesNormals[triangles[t].getVertex(1)] += tri_normal;
    }

    for (unsigned int v = 0; v < verticesNormals.size(); ++v) {
        verticesNormals[v].normalize();
    }
}

void Curve::collectOneRing(std::vector <std::vector<unsigned int>> &oneRing) const {
    oneRing.resize(vertices.size());
    for (unsigned int i = 0; i < triangles.size(); i++) {
        const Edge &ti = triangles[i];
        for (unsigned int j = 0; j < 2; j++) {
            unsigned int vj = ti.getVertex(j);
            for (unsigned int k = 1; k < 2; k++) {
                unsigned int vk = ti.getVertex((j + k) % 2);
                if (std::find(oneRing[vj].begin(), oneRing[vj].end(), vk) == oneRing[vj].end())
                    oneRing[vj].push_back(vk);
            }
        }
    }
}

void Curve::glTriangle(unsigned int i) {

    const Edge &t = triangles[i];
    for (int j = 0; j < 2; j++) {
        glNormal(verticesNormals[t.getVertex(j)] * normalDirection);
        glVertex(vertices[t.getVertex(j)]);
    }

}


void Curve::draw(std::vector<bool> &selected, std::vector<bool> &fixed) {

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEPTH);

    glBegin(GL_LINES);

    for (unsigned int i = 0; i < triangles.size(); i++) {

        int vi[2] = {static_cast<int>(triangles[i].getVertex(0)), static_cast<int>(triangles[i].getVertex(1))};
        if (selected[vi[0]] && selected[vi[1]])
            glColor3f(0.8, 0., 0.);
        if (fixed[vi[0]] && fixed[vi[1]])
            glColor3f(0., 0.8, 0.);
        else
            glColor3f(0.37, 0.55, 0.82);
        glTriangle(i);

    }

    glEnd();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_DEPTH);

}


void Curve::draw() {

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEPTH);

    glBegin(GL_LINES);

    for (unsigned int i = 0; i < triangles.size(); i++) {

        glTriangle(i);

    }

    glEnd();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_DEPTH);

}
