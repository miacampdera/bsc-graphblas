//
// Created by Mia Pulido Campdera on 17/7/21.
//

#include "map.h"

GrB_Info map_add(GrB_Vector distances[], GrB_Index node, GrB_Vector vector) {
    distances[node] = vector;
}

GrB_Info map_contains(GrB_Vector distances[], GrB_Index node) {
    if (distances[node]) {
        return true;
    }
    return false;
}

GrB_Info map_get(GrB_Vector distances[], GrB_Index node) {
    return distances[node];
}

