#ifndef RIGID_TRANSFOR_FUNCTION_H
#define RIGID_TRANSFOR_FUNCTION_H
/*
#include "hjlib/function/function.h"
#include "hjlib/function/func_aux.h"
#include "zjucad/matrix/io.h"
#include "zjucad/matrix/matrix.h"
#include "data_type.h"
#include <stdint.h>

namespace lq 
{
using namespace hj::function;
using namespace zjucad::matrix;

class rigid_transfor_function : public function_t<double, int32_t>
{
public:
  
  rigid_transfor_function(const matrixd &p1, const matrixd &p2);
  virtual ~rigid_transfor_function(){}
  virtual size_t dim_of_x(void) const;
  virtual size_t dim_of_f(void) const;
  virtual int val(const value_type *x, value_type *f, func_ctx *ctx = 0) const;
  virtual int jac(const value_type *x, value_type *val, int_type *ptr = 0, 
                  int_type *idx = 0, func_ctx *ctx = 0) const;
  
  virtual size_t jac_nnz(void) const;

private:
  const matrixd model_vertex_;
  const matrixd ref_vertex_;
};

};

*/
#endif // RIGID_TRANSFOR_FUNCTION_H
