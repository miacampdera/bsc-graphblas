//
// Created by Mia Pulido Campdera on 24/7/21.
//

#ifndef MAP_H
#define MAP_H
#include <GraphBLAS.h>

GrB_Info map_add(GrB_Index node, GrB_Vector vector);

GrB_Info map_contains(GrB_Index node);

GrB_Info map_get(GrB_Index node);

#endif //MAP_H
