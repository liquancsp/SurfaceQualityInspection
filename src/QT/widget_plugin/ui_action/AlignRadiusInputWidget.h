#ifndef ALIGNRADIUSINPUTWIDGET_H
#define ALIGNRADIUSINPUTWIDGET_H

#include <widget_plugin/ui_resource/ui_LoaclRadiusInput.h>


#if defined(_MSC_VER)
#include <memory>
#endif

#if defined(__GNUC__)
#include <tr1/memory>
#endif

namespace lq {

class AlignRadiusInputWidget : public QDialog
{
  Q_OBJECT
  
public:
  AlignRadiusInputWidget();
  ~AlignRadiusInputWidget();
  

signals:
  void SignalAlignRadius(double radius);
  
public slots:
  
  void slot_ok_btn_action();
  void slot_cancel_btn_action();
  
private:
  
  std::tr1::shared_ptr<Ui_AlignRadiusInput> radius_input_handle_;
  void create_action();
};

}

#endif // ALIGNRADIUSINPUTWIDGET_H
