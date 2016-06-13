#ifndef HEIGHTMAPRADIUSRATIOWIDGET_H
#define HEIGHTMAPRADIUSRATIOWIDGET_H

#include <widget_plugin/ui_resource/ui_HeightMapRadiusRatio.h>
#if defined(_MSC_VER)
#include <memory>
#endif

#if defined(__GNUC__)
#include <tr1/memory>
#endif

namespace sqi {
namespace ui{

class HeightMapRadiusRatioWidget : public QDialog
{
  Q_OBJECT
public :
  
  HeightMapRadiusRatioWidget();
  ~HeightMapRadiusRatioWidget();
  
signals:
  void SignalRadiusRatio(double multipiler, double ratio, int setp);
public slots:
  
  void SlotOkAction();
  void SlotCancelAction();
  
private:
  std::tr1::shared_ptr<Ui_HeightMapRadiusRatio> ratio_input_handle_;
  void CreateAction();
  
};

}
}

#endif // HEIGHTMAPRADIUSRATIOWIDGET_H
