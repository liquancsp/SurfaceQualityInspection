/*
#include "rigid_transfor_function.h"
#include "zjucad/matrix/itr_matrix.h"

namespace lq 
{

rigid_transfor_function::rigid_transfor_function(const matrixd &p1, 
                                                 const matrixd &p2) : model_vertex_(p1), ref_vertex_(p2)
{
  
}

size_t rigid_transfor_function::dim_of_x(void) const
{
  return 9;
}

size_t rigid_transfor_function::dim_of_f(void) const 
{
  return 1;
}


int rigid_transfor_function::val(const value_type *x, 
                                 value_type *f, func_ctx *ctx = 0) const 
{
  //encapsulated x in a 3 * 3 matrix
  zjucad::matrix::itr_matrix<value_type*> x0(3, 3, x);
  zjucad::matrix::itr_matrix<value_type*> f0(3, 1, f);
  f0 = x0 * model_vertex_ - ref_vertex_;
  return 0;
}

int rigid_transfor_function::jac(const value_type *x, value_type *val, int_type *ptr = 0, 
                int_type *idx = 0, func_ctx *ctx = 0) const
{
  ptr[1] = ptr[0] + 9;
  for(size_t i = 0; i < 9; ++i)
  {
    val[ptr[0] + i] = 1;
    idx[ptr[0] + i] = i;
  } 
}

size_t rigid_transfor_function::jac_nnz(void) const
{
  return 9;
}

};
*/
