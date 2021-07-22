//
// Created by Mia Pulido Campdera on 20/6/21.
//

#ifndef W_SSSP_H
#define W_SSSP_H
#include <GraphBLAS.h>

    GrB_Info w_sssp(
            GrB_Vector result,
            GrB_Index source,
            GrB_Matrix graph);

#endif //W_SSSP_H
