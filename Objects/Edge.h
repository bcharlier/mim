// --------------------------------------------------------------------------
// gMini,
// a minimal Glut/OpenGL app to extend
//
// Copyright(C) 2007-2009
// Tamy Boubekeur
//
// All rights reserved.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License (http://www.gnu.org/licenses/gpl.txt)
// for more details.
//
// --------------------------------------------------------------------------


#pragma once

#include <map>
#include <assert.h>
// -------------------------------------------------
// Intermediate Edge structure for hashed adjacency
// -------------------------------------------------

class Edge {

public:
    inline Edge() {}
    inline virtual ~Edge() {}

    inline Edge(unsigned int v0, unsigned int v1, float _size = 1.) {
        if (v0 < v1) {
            v[0] = v0;
            v[1] = v1;
        }
        else {
            v[0] = v1;
            v[1] = v0;
        }
        size = _size;
    }

    inline Edge(const Edge &e) {
        v[0] = e.v[0];
        v[1] = e.v[1];
        size = e.size;
    }

    inline Edge &operator=(const Edge &e) {
        v[0] = e.v[0];
        v[1] = e.v[1];
        size = e.size;
        return (*this);
    }

    inline unsigned int getVertex(unsigned int i) const {
        assert(i < 2 && "Give a index between 0 and 1 as a index for Edge");
        return v[i]; }

    inline void setVertex(unsigned int i, unsigned int vertex) {
        assert(i < 2 && "Give a index between 0 and 1 as a index for Edge");
        v[i] = vertex; }

    inline void setVertices(unsigned int v0, unsigned int v1) { v[0] = v0; v[1] = v1; }

    inline bool operator==(const Edge &e) { return (v[0] == e.v[0] && v[1] == e.v[1]); }

    inline bool operator<(const Edge &e) const { return (v[0] < e.v[0] || (v[0] == e.v[0] && v[1] < e.v[1])); }

    float operator[](unsigned int c) const {
        assert(c < 2 && "Give a index between 0 and 1 as a index for Edge");
        return v[c];
    }

    inline bool contains(unsigned int i) const { return (v[0] == i || v[1] == i); }

    float size;

    static const unsigned int dimension = 2;

private:
    unsigned int v[dimension];

};

struct compareEdge {
    inline bool operator()(const Edge e1, const Edge e2) const {
        if (e1.getVertex(0) < e2.getVertex(0))
            return true;
        if (e1.getVertex(0) > e2.getVertex(0))
            return false;
        if (e1.getVertex(1) > e2.getVertex(1))
            return true;
        return false;
    }
};

typedef std::map<Edge, unsigned int, compareEdge> EdgeMapIndex;
typedef std::map<Edge, float, compareEdge> CotangentWeights;

// Some Emacs-Hints -- please don't remove:
//
//  Local Variables:
//  mode:C++
//  tab-width:4
//  End:
