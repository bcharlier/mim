#ifndef MIM_DEFORMABLEOBJECTMANAGER_H
#define MIM_DEFORMABLEOBJECTMANAGER_H

#include <vector>

#include "Objects/Curve.h"
#include "Objects/Mesh.h"

class DeformableObjectManager {

public:
    DeformableObjectManager();
    ~DeformableObjectManager();

    void Add();
    void Remove();

protected:
    std::vector<Curve> curves_list;
    std::vector<Mesh> meshes_list;

};


#endif //MIM_DEFORMABLEOBJECTMANAGER_H
