//
// Created by Mia Pulido Campdera on 24/7/21.
//

#include "map.h"
#include <stdbool.h>

void map_add(GrB_Vector *map, GrB_Index node, GrB_Vector vector) {
    *(map + node) = vector;
}

bool map_contains(GrB_Vector *map, GrB_Index node) {
    GrB_Vector v = *(map + node);
   /*TODO if () {
        return true;
    }
    return false;*/
}

GrB_Vector map_get(GrB_Vector *map, GrB_Index node) {
    return *(map + node);
}
