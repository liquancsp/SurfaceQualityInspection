#ifndef QTVTKWIDGET_H
#define QTVTKWIDGET_H
#include <menu_plugin/ui_action/menu_context_event_item.h>
#include <data_type.h>

#include "QVTKWidget.h"
#include <QVTKInteractor.h>
#include <vtkObjectBase.h>

#if defined(_MSC_VER)
#include <memory>
#endif

#if defined(__GNUC__)
#include <tr1/memory>
#endif

namespace sqi
{

namespace ui
{

class QtVtkWidget : public QVTKWidget
{
  Q_OBJECT
public:

  QtVtkWidget(QWidget *parent=NULL, const char *name=NULL, Qt::WFlags f=0);
 
  ~QtVtkWidget()
  {
  }
  
 // Ui::ui_menu_context_event* context_menu() {return context_menu_.get();}
  
  QVTKInteractor* GetInteractor()
  {
    return QVTKWidget::GetInteractor();
  }
  
  vtkRenderWindow* GetRenderWindow()
  {
    return QVTKWidget::GetRenderWindow();
  }
  
  virtual void paintEvent(QPaintEvent *event)
  {
    QVTKWidget::paintEvent(event);
  }
  
  
signals:
  
  void signal_open_model();
  void singal_new();
  void signal_delete_model(lq::WindowFlag flag);
  void signal_resize_win();
  
public slots:
  
  void slot_open_file();
  void slot_new_action();
  void slot_show_points();
  void slot_show_surface();
  void slot_show_wireframe();
  void slot_show_surface_with_edge();
  void slot_delete_model();
  
protected:
  
  virtual void contextMenuEvent (QContextMenuEvent *e);
  virtual void resizeEvent(QResizeEvent *event);
  std::tr1::shared_ptr<lq::menu_context_event_item> context_menu_item_;
  QMenu* pop_menu_;
 
  
private:
  void create_action();
  int* screen_size_;
  QPoint cursor_pos_;
  bool is_has_style_;
  
};

}

}

#endif // QTVTKWIDGET_H
