#ifndef LEFT_REGION_WIN_H
#define LEFT_REGION_WIN_H

#include <widget_plugin/ui_resource/ui_MainPanel.h>
#include <widget_operate/left_region_operate.h>
#include <vtk_data/vtk_model_data.h>
#include <string>
#include <QMainWindow>
#include <vtkSmartPointer.h>

namespace lq {

  
  class left_region_win : public QDockWidget, public Ui::MainPanel
  {
    Q_OBJECT

 public:
    left_region_win(QWidget* parent = 0);
    ~left_region_win();
    void set_tree(const std::vector<std::string> &model_list);
    void add_tree_leaf(const std::string &file_name);
    void set_data_set(const vtkSmartPointer<vtk_model_data> &other_data_set);
    QTreeWidgetItem *root;

 signals:
    void delete_tree_leaf(const QString &);
    void signal_adjust_color(const int &);
    
 public slots:
    
    void select_item(QTreeWidgetItem *item, int column);
    void delete_btn_down();
    void slot_delete_tree_item(int);
    void adjust_color();
    
 private:
    // pointer to the main window (Optional)
    // QMainWindow* m_pMainWindow;
    void init();
    void create_action();
    vtkSmartPointer<vtk_model_data> data_set_;
    left_region_operate left_operate_;
    
  };
  
}

#endif
