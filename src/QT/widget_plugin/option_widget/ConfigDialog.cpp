#include <QWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include <QStackedWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <widget_plugin/option_widget/ConfigDialog.h>
#include <widget_plugin/option_widget/ConfigPages.h>

namespace sqi {
namespace WidgetAction {

ConfigDialog::ConfigDialog()
{
  content_widget_ = new QListWidget;
  content_widget_->setViewMode(QListView::IconMode);
  content_widget_->setIconSize(QSize(96, 84));
  content_widget_->setMovement(QListView::Static);
  content_widget_->setMaximumWidth(128);
  content_widget_->setSpacing(12);
  
  pages_widget_ = new QStackedWidget;
  pages_widget_->addWidget(new ConfigurationPage);
  pages_widget_->addWidget(new UpdatePage);
//  pagesWidget->addWidget(new QueryPage);
  
  QPushButton* apply_btn = new QPushButton(tr("Apply"));
  QPushButton* close_btn = new QPushButton(tr("Cancel"));
  QPushButton* ok_btn = new QPushButton(tr("Ok"));
 
  content_widget_->setCurrentRow(0);
  
  QHBoxLayout *horizontalLayout = new QHBoxLayout;
  horizontalLayout->addWidget(content_widget_);
  horizontalLayout->addWidget(pages_widget_, 1);
  
  QHBoxLayout *buttonsLayout = new QHBoxLayout;
  
  buttonsLayout->addStretch(15);
  buttonsLayout->addWidget(apply_btn);
  buttonsLayout->addStretch();
  buttonsLayout->addWidget(close_btn);
  buttonsLayout->addStretch();
  buttonsLayout->addWidget(ok_btn);
  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addLayout(horizontalLayout);
  mainLayout->addStretch(1);
  mainLayout->addSpacing(12);
  mainLayout->addLayout(buttonsLayout);
  setLayout(mainLayout);
  setWindowTitle(tr("Config Dialog"));
  
  CreateIcons();
  CreateAction();
}

void ConfigDialog::SlotChangePage(QListWidgetItem *current, QListWidgetItem *previous)
{
  if (!current)
    current = previous;
  
  pages_widget_->setCurrentIndex(content_widget_->row(current));
}

void ConfigDialog::CreateIcons()
{
  QListWidgetItem *config_list_item = new QListWidgetItem(content_widget_);
  config_list_item->setIcon(QIcon("./images/config.png"));
  config_list_item->setText(tr("Configuration"));
  config_list_item->setTextAlignment(Qt::AlignHCenter);
  config_list_item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
  QListWidgetItem *environment_list_item = new QListWidgetItem(content_widget_);
  environment_list_item->setIcon(QIcon("./images/color_config.png"));
  environment_list_item->setText(tr("Environment"));
  environment_list_item->setTextAlignment(Qt::AlignHCenter);
  environment_list_item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
  connect(content_widget_,
          SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
          this, SLOT(SlotChangePage(QListWidgetItem*,QListWidgetItem*)));
}

void ConfigDialog::CreateAction()
{
  //connect(close_btn, SIGNAL(clicked()), this, SLOT(close()));
}

}

}
