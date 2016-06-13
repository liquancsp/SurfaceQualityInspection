#include<iostream>
#include"matrix.h"
#include"io.h"
using namespace std;
using namespace zjucad::matrix;

int main(int argc, char** argv)
{
  matrix<double> vertex1 = zeros<double>(1, 3);
  matrix<double> vertex2 = zeros<double>(1, 3);
  matrix<double> vertex3 ;
  for(int i = 0; i < 3; ++i)
    vertex1(0, i) = i;
  for(int i = 0 ; i < 3; ++i)
    vertex2(0, i) = i + 1;
  cout << vertex2 << endl;
  vertex3 = trans(cross(vertex1, vertex2));
  cout << vertex3 << endl;
  return 0;
}
