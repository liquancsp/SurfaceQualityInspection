#include <Algorithms/CalHeightMap.h>
#include <Algorithms/ZernikeCoefficient.h>
#include <Algorithms/CalHeightMapZBuffer.h>
#include <vtk_data/vtk_model_data.h>
#include <MeshData.h>
#include <GLDepthBuffer.h>

#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include <string>
using namespace std;
using namespace sqi::gl;

std::tr1::shared_ptr<GLDepthBuffer> depth_opt;
string z_buffer_out("zbuffer_opengl.txt");
ofstream fout(z_buffer_out.c_str());
void Draw()
{
  sqi::meshdata::MeshDataPtr mesh_data = depth_opt->GetMeshData();
  glBegin(GL_POLYGON);
  for(int i = 0; i < mesh_data->FNum(lq::kLeft); ++i)
  {
    sqi::meshdata::FaceIterator fi = mesh_data->FBeginIter(lq::kLeft) + i;
    glVertex3f(fi->cP(0)[0], fi->cP(0)[1], fi->cP(0)[2]);
    glVertex3f(fi->cP(1)[0], fi->cP(1)[1], fi->cP(1)[2]);
    glVertex3f(fi->cP(2)[0], fi->cP(2)[1], fi->cP(2)[2]);
    
  }
  glEnd();
}

float cnt = -1;
bool flag = false;
void WriteDepthValue()
{
  cout << "cnt : " << cnt << endl;
  if(cnt == depth_opt->GetMeshData()->VNum(lq::kLeft) - 1)
  {
    fout.close();
    return;
  }
  if(cnt == -1)
    return;
  for(int i = 0; i < 16; ++i)
  {
    float zdata;
    for(int j = 0; j < 16; ++j)
    {
      glReadPixels(i, j, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &zdata);
      if(zdata == 1 || !depth_opt->IsInRange(16 * i + j))
        cout << " " << -1;
      else 
      {
        cout << " " << zdata * (depth_opt->GetProjectRange()[5] - depth_opt->GetProjectRange()[4]);
      }
    }
    cout << endl;
  }
}
void RenderScene(void) 
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if(cnt > 3)
    exit(0);
  if(cnt >= 0)
    depth_opt->UpdateParameters(cnt);
  ++cnt;
  glViewport(0, 0, depth_opt->GetWindowsSize()[0], depth_opt->GetWindowsSize()[1]);
  float* project = depth_opt->GetProjectRange(); 
  glOrtho(project[0], project[1], project[2], project[3], project[4], project[5]);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(depth_opt->GetEyesPosition()[0], depth_opt->GetEyesPosition()[1],
      depth_opt->GetEyesPosition()[2], depth_opt->GetFocalPosition()[0],
      depth_opt->GetFocalPosition()[1], depth_opt->GetFocalPosition()[2],
      depth_opt->GetUpView()[0], depth_opt->GetUpView()[1], depth_opt->GetUpView()[2]);
  Draw();
  for(int i = 0; i < 16; ++i)
  {
    float zdata;
    for(int j = 0; j < 16; ++j)
    {
      glReadPixels(i, j, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &zdata);
      if(zdata == 1 || !depth_opt->IsInRange(16 * i + j))
        cout << -1 << " ";
      else 
        cout << zdata * (depth_opt->GetProjectRange()[5] - depth_opt->GetProjectRange()[4]) << " ";
    }
    cout << endl;
  }
  glEnable(GL_DEPTH_TEST);
  glutSwapBuffers();
  glFlush();
  //glReadBuffer(GL_FRONT);
}

void Mouse( int x, int y )
{
  cout << "x : " << x << " y: " << y << endl;
  float zdata;
  glEnable(GL_DEPTH_TEST);
  glReadPixels(x, glutGet( GLUT_WINDOW_HEIGHT ) - y, 
        1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &zdata);
  cout << "depth : " << zdata * (depth_opt->GetProjectRange()[5] - depth_opt->GetProjectRange()[4])  << endl;
}

void Init(int argc, char **argv)
{
  if(argc == 3 || argc == 4)
  {
    string file_name(argv[2]);
    depth_opt.reset(new GLDepthBuffer());
    depth_opt->LoadFile(file_name);
  }
}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  Init(argc, argv);
  glEnable(GL_DEPTH_TEST);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowSize(16, 16);
  glutCreateWindow("DepthBuffer");
  glutDisplayFunc(RenderScene);
  glutIdleFunc(RenderScene);
  //glutPassiveMotionFunc(Mouse);
  
  glutMainLoop();
  return 0;
}
