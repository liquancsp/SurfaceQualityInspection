#include "QtVtkWidget.h"
#include <callback/interactor_style.h>


#include <QtGui/QAction>
#include <QtGui/QMenu>
#include <vtkRenderWindow.h>
#include <vtkInteractorObserver.h>

#include <iostream>

namespace sqi
{
namespace ui
{
using namespace lq;
using namespace std;
using namespace std::tr1;

QtVtkWidget::QtVtkWidget(QWidget *parent, const char *name, Qt::WFlags f) : 
  QVTKWidget(parent),
  context_menu_item_(new lq::menu_context_event_item(this)),
  pop_menu_(new QMenu(this)),
  is_has_style_(false)
{
  pop_menu_ = new QMenu(this);
  create_action();
}

void QtVtkWidget::contextMenuEvent(QContextMenuEvent *e)
{
  vtkSmartPointer<lq::interactor_style> iter_style ;
  iter_style = static_cast<lq::interactor_style*>(this->GetRenderWindow()->
                                                  GetInteractor()->GetInteractorStyle());
  if(iter_style == NULL)
    return;
  RepresentationType res = static_cast<RepresentationType>(iter_style->get_representation());
  if(res == kPoints)
  {
    context_menu_item_->show_points_action()->setChecked(true);
  }
  else if(res == kWireframe)
  {
    context_menu_item_->show_wireframe_action()->setChecked(true);
  }
  else if(res == kSurfaces)
  {
    context_menu_item_->show_surface_action()->setChecked(true);
  }
  else if(res == kSurfaceWithEdge)
  {
    context_menu_item_->show_surface_with_edge_action()->setChecked(true);
  }
  cursor_pos_ = QCursor::pos();
  context_menu_item_->exec(cursor_pos_);
}

void QtVtkWidget::resizeEvent(QResizeEvent *event)
{
  QVTKWidget::resizeEvent(event);
  emit signal_resize_win();
}

//------------------------------------------------------------------------------
//begin slots

void QtVtkWidget::slot_open_file()
{
  emit signal_open_model();
}

void QtVtkWidget::slot_new_action()
{
  emit singal_new();
}

void QtVtkWidget::slot_show_points()
{
  vtkSmartPointer<lq::interactor_style> iter_style ;
  iter_style = static_cast<lq::interactor_style*>(this->GetRenderWindow()->
                                                  GetInteractor()->GetInteractorStyle());
  iter_style->set_points();
}

void QtVtkWidget::slot_show_surface()
{
  vtkSmartPointer<lq::interactor_style> iter_style; 
  iter_style = static_cast<lq::interactor_style*>(this->GetRenderWindow()->
                                                  GetInteractor()->GetInteractorStyle());
  iter_style->set_face();
}

void QtVtkWidget::slot_show_wireframe()
{
  vtkSmartPointer<lq::interactor_style> iter_style;
  iter_style = static_cast<lq::interactor_style*>(this->GetRenderWindow()->
                                                  GetInteractor()->GetInteractorStyle());
  iter_style->set_wireframe();
}

void QtVtkWidget::slot_show_surface_with_edge()
{
  vtkSmartPointer<lq::interactor_style> iter_style;
  iter_style = static_cast<lq::interactor_style*>(this->GetRenderWindow()->
                                                  GetInteractor()->GetInteractorStyle());
  iter_style->set_surface_with_edge();
}

void QtVtkWidget::slot_delete_model()
{
  int* cursor_pos;
  cursor_pos = this->GetRenderWindow()->GetInteractor()->GetEventPosition();
  screen_size_ = this->GetRenderWindow()->GetActualSize();
 
  if(cursor_pos[0] > screen_size_[0] / 2)
    emit signal_delete_model(lq::kRight);
  else 
    emit signal_delete_model(lq::kLeft);
}

//end of slots
//------------------------------------------------------------------------------

void QtVtkWidget::create_action()
{
  connect(this->context_menu_item_->open_file_action(), SIGNAL(triggered()),
          this, SLOT(slot_open_file()));
  connect(this->context_menu_item_->new_action(), SIGNAL(triggered()),
          this, SLOT(slot_new_action()));
  
  connect(this->context_menu_item_->show_points_action(), SIGNAL(triggered()),
          this, SLOT(slot_show_points()));
  connect(this->context_menu_item_->show_surface_action(), SIGNAL(triggered()),
          this, SLOT(slot_show_surface()));
  connect(this->context_menu_item_->show_wireframe_action(), SIGNAL(triggered()),
          this, SLOT(slot_show_wireframe()));
  connect(this->context_menu_item_->show_surface_with_edge_action(), SIGNAL(triggered()),
          this, SLOT(slot_show_surface_with_edge()));
  
  connect(this->context_menu_item_->delete_model_action(), SIGNAL(triggered()),
          this, SLOT(slot_delete_model()));
}

}

}
