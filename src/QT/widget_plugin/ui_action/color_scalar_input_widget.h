#ifndef COLOR_SCALAR_INPUT_WIDGET_H
#define COLOR_SCALAR_INPUT_WIDGET_H

#include <QDialog>
#include <widget_plugin/ui_resource/ui_color_scalar_input.h>
#include <MeshData.h>
#if defined(_MSC_VER)
#include <memory>
#endif

#if defined(__GNUC__)
#include <tr1/memory>
#endif

namespace lq
{

  class color_scalar_input_widget : public QDialog
  {
    Q_OBJECT

  public:

    color_scalar_input_widget(sqi::meshdata::MeshDataPtr mesh);
    ~color_scalar_input_widget();
    void InitParmeters();
 signals:

    void signal_color_scalar(sqi::meshdata::ColorScalarType flag, double max_threshold,
                             double min_threshold);

 public slots:

    void slot_color_scalar();
    void slot_cancel_btn();
    
 private:

    std::tr1::shared_ptr<Ui::ui_color_scalar_input> color_scalar_input_handle_;
    sqi::meshdata::MeshDataPtr mesh_data_;
    void create_action();
    
  };
}


#endif // COLOR_SCALAR_INPUT_WIDGET_H
