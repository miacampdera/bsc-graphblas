//
// Created by Mia Pulido Campdera on 12/6/21.
//

#ifndef UW_SSSP_H
#define UW_SSSP_H
#include "GraphBLAS.h"


    GrB_Info uw_sssp(
            GrB_Vector result,
            GrB_Index source,
            GrB_Matrix graph);

#endif //UW_SSSP_H
