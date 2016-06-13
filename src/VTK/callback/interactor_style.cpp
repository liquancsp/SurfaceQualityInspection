#include "interactor_style.h"
#include <iostream>
#include <vtkActor.h>
#include <vtkPropPicker.h>

#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkActorCollection.h>
#include <vtkActor.h>
#include <vtkAssemblyPath.h>
#include <vtkInteractorObserver.h>
#include <vtkProperty.h>

#include <string.h>

namespace lq 
{

using namespace std;
using namespace sqi::meshdata;

void interactor_style::set_data_set(const vtkSmartPointer<vtk_model_data> &other_data_set)
{
  this->data_set_ = other_data_set;
  inspect_director_ptr_->SetDataSet(data_set_);
}

void interactor_style::OnMouseMove()
{
  
  cal_error();
  vtkInteractorStyleTrackballCamera::OnMouseMove();
}

void interactor_style::OnMouseWheelForward()
{
  cal_error();
  vtkInteractorStyleTrackballCamera::OnMouseWheelForward();
}

void interactor_style::OnMouseWheelBackward()
{
  cal_error();
  vtkInteractorStyleTrackballCamera::OnMouseWheelBackward();
}

void interactor_style::OnRightButtonDown()
{
  cout << "[#Info]Mouse Right Button Down on Interactor_style!" << endl;
}

void interactor_style::OnLeftButtonUp()
{
  cal_error();
  vtkInteractorStyleTrackballCamera::OnLeftButtonUp();
}

void interactor_style::OnChar()
{
  //vtkInteractorStyleTrackballCamera::OnChar();
  rwi_ = this->Interactor;
  switch(rwi_->GetKeyCode())
  {
  case 'w':
  case 'W':
  {
    set_wireframe();
    break;
  }
  case 's':
  case 'S':
  {
    set_face();
    break;
  }
  case 'p':
  case 'P':
  {
    set_points();
    break;
  }
  }
}

void interactor_style::OnKeyDown()
{
  
}

void interactor_style::OnKeyPress()
{

}

bool interactor_style::is_position_equal(const double p1[3], const double p2[3])
{
  for(size_t i = 0; i < 3; ++i)
  {
    if(!equal(p1[i], p2[i]))
      return false;
  }
  return true;
}


void interactor_style::cal_error()
{
  cout << "cal  error" << endl;
  assert(mesh_data_.use_count() > 0);
  if(data_set_->AlgType() == lq::kReflectionMap)
    inspect_director_ptr_->Execute(lq::kReflectionMap, lq::kCalTwoDiff);
  if(data_set_->AlgType() == lq::kMixReflConvex)
    inspect_director_ptr_->Execute(lq::kMixReflConvex, lq::kCalTwoDiff);
}

//This function was written in reference to the function OnChar()
//in file tkInteractorStyle.cxx  
void interactor_style::set_points()
{
  //set the two model in the same represent
  for(size_t i = 0; i < 2; ++i)
  {
    data_set_->Actor(i)->GetProperty()->SetRepresentationToPoints();
  }
//  if(rwi_ == NULL)
//    rwi_ = this->Interactor;
//  vtkActorCollection *ac;
//  vtkActor *anActor, *aPart;
//  vtkAssemblyPath *path;
//  this->FindPokedRenderer(rwi_->GetEventPosition()[0],
//                          rwi_->GetEventPosition()[1]);
//  ac = this->CurrentRenderer->GetActors();
//  vtkCollectionSimpleIterator ait;
//  for (ac->InitTrav //set the two model in the same represent
//  for(size_t i = 0; i < 2; ++i)
//  {
//    data_set_->Actor(i)->GetProperty()->EdgeVisibilityOff();
//    data_set_->Actor(i)->GetProperty()->SetRepresentationToSurface();
//  }ersal(ait); (anActor = ac->GetNextActor(ait)); ) 
//  {
//    for (anActor->InitPathTraversal(); (path = anActor->GetNextPath()); ) 
//    {
//      aPart=static_cast<vtkActor *>(path->GetLastNode()->GetViewProp());
//      aPart->GetProperty()->SetRepresentationToPoints();
//    }
//  }
  rwi_->Render();
}

//This function was written in reference to the function OnChar()
//in file tkInteractorStyle.cxx  
void interactor_style::set_face()
{
  //set the two model in the same represent
  for(size_t i = 0; i < 2; ++i)
  {
    data_set_->Actor(i)->GetProperty()->EdgeVisibilityOff();
    data_set_->Actor(i)->GetProperty()->SetRepresentationToSurface();
  }
//  if(rwi_ == NULL)
//    rwi_ = this->Interactor;
//  vtkActorCollection *ac;
//  vtkActor *anActor, *aPart;
//  vtkAssemblyPath *path;
//  this->FindPokedRenderer(rwi_->GetEventPosition()[0],
//                          rwi_->GetEventPosition()[1]);
//  ac = this->CurrentRenderer->GetActors();
//  vtkCollectionSimpleIterator ait;
//  for (ac->InitTraversal(ait); (anActor = ac->GetNextActor(ait)); ) 
//  {
//    for (anActor->InitPathTraversal(); (path = anActor->GetNextPath()); ) 
//    {
//      aPart=static_cast<vtkActor *>(path->GetLastNode()->GetViewProp());
//      aPart->GetProperty()->EdgeVisibilityOff();
//      aPart->GetProperty()->SetRepresentationToSurface();
//    }
//  }
  rwi_->Render();
}

//This function was written in reference to the function OnChar()
//in file tkInteractorStyle.cxx  
void interactor_style::set_wireframe()
{
  //set the two model in the same represent
  for(size_t i = 0; i < 2; ++i)
  {
    data_set_->Actor(i)->GetProperty()->SetRepresentationToWireframe();
  }
//  if(rwi_ == NULL)
//    rwi_ = this->Interactor;
//  vtkActorCollection *ac;
//  vtkActor *anActor, *aPart;
//  vtkAssemblyPath *path;
//  this->FindPokedRenderer(rwi_->GetEventPosition()[0],
//                          rwi_->GetEventPosition()[1]);
//  ac = this->CurrentRenderer->GetActors();
//  vtkCollectionSimpleIterator ait;
//  for (ac->InitTraversal(ait); (anActor = ac->GetNextActor(ait)); ) 
//  {
//    for (anActor->InitPathTraversal(); (path=anActor->GetNextPath()); ) 
//    {
//      aPart=static_cast<vtkActor *>(path->GetLastNode()->GetViewProp());
//      aPart->GetProperty()->SetRepresentationToWireframe();
//    }
//  }
  rwi_->Render();
}

//This function was written in reference to the function OnChar()
//in file tkInteractorStyle.cxx  
void interactor_style::set_surface_with_edge()
{
  
  //set the two model in the same represent
  for(size_t i = 0; i < 2; ++i)
  {
    cout << "-=-==-=-" << std::endl;
    data_set_->Actor(i)->GetProperty()->SetRepresentationToSurface();
    cout << "-=-==-=-" << std::endl;
    data_set_->Actor(i)->GetProperty()->EdgeVisibilityOn();
    cout << "-=-==-=-" << std::endl;
  }
//  //this method just set the represent of the position of current cursor
//  if(rwi_ == NULL)
//    rwi_ = this->Interactor; 
//  vtkActorCollection *ac;
//  vtkActor *anActor, *aPart;
//  vtkAssemblyPath *path;
//  this->FindPokedRenderer(rwi_->GetEventPosition()[0],
//                          rwi_->GetEventPosition()[1]);
//  ac = this->CurrentRenderer->GetActors();
//  vtkCollectionSimpleIterator ait;
//  for (ac->InitTraversal(ait); (anActor = ac->GetNextActor(ait)); ) 
//  {
//    for (anActor->InitPathTraversal(); (path = anActor->GetNextPath()); ) 
//    {
//      aPart=static_cast<vtkActor *>(path->GetLastNode()->GetViewProp());
//      aPart->GetProperty()->SetRepresentationToSurface();
//      aPart->GetProperty()->EdgeVisibilityOn();
//    }
//  }
  rwi_->Render();
}

int interactor_style::get_representation()
{
  if(rwi_ == NULL)
    rwi_ = this->Interactor;
  cout << "=========" << endl;
  vtkActorCollection *ac;
  vtkActor *anActor, *aPart;
  vtkAssemblyPath *path;
  vtkRenderer* render;
  /*it must judge whether the render windows have vtkrender, if there is no vtkrender
    return -1. if not the program will collapse when user press the right button of mouse*/
  render = this->CurrentRenderer;
  cout << "=========" << endl;
  if(render == NULL)
    return -1;
  cout << "=========" << endl;
  this->FindPokedRenderer(rwi_->GetEventPosition()[0],
                          rwi_->GetEventPosition()[1]);
  cout << "=========" << endl;
  ac = this->CurrentRenderer->GetActors();
  cout << "=========" << endl;
  vtkCollectionSimpleIterator ait;
  for (ac->InitTraversal(ait); (anActor = ac->GetNextActor(ait)); ) 
  {
    for (anActor->InitPathTraversal(); (path = anActor->GetNextPath()); ) 
    {
      aPart=static_cast<vtkActor *>(path->GetLastNode()->GetViewProp());
      if(aPart->GetProperty()->GetEdgeVisibility())
        return kSurfaceWithEdge;
      else 
        return aPart->GetProperty()->GetRepresentation();
    }
  }
  return 0;
}

};
