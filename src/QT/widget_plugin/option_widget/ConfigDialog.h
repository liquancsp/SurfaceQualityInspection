#pragma once
#ifndef CONFIG_DIALOG_H
#define CONFIG_DIALOG_H

#include <QDialog>

class QListWidget;
class QListWidgetItem;
class QStackedWidget;

namespace sqi {
namespace WidgetAction {


class ConfigDialog : public QDialog
{
  Q_OBJECT

public:
  
  ConfigDialog();

public slots:
  
  void SlotChangePage(QListWidgetItem *current, QListWidgetItem *previous);
  
private:
  
  void CreateIcons();
  void CreateAction();
  QListWidget *content_widget_;
  QStackedWidget *pages_widget_;
  
  
};

}
}

#endif // CONFIGDIALOG_H
