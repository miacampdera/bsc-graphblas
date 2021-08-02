//
// Created by Mia Pulido Campdera on 24/7/21.
//

#ifndef MAP_H
#define MAP_H
#include <GraphBLAS.h>

void map_add(GrB_Vector *map, GrB_Index node, GrB_Vector vector);

bool map_contains(GrB_Vector *map, GrB_Index node);

GrB_Vector map_get(GrB_Vector *map, GrB_Index node);

#endif //MAP_H
