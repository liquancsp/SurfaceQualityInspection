#include <widget_plugin/option_widget/ConfigPages.h>
#include <widget_plugin/option_widget/ConfigEditWidget.h>

#include <QGroupBox>
#include <QLabel>
#include <QComboBox>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QListWidget>
#include <QPushButton>
#include <QDateTimeEdit>
#include <QSpinBox>
#include <QtGui/QLineEdit>

#include <vtk_data/VtkPropManage.h>

namespace sqi {

namespace WidgetAction{


using namespace VtkData;

ConfigurationPage::ConfigurationPage(QWidget *parent)
  : QWidget(parent)
{
  QGroupBox *configGroup = new QGroupBox(tr("Server configuration"));
  
//  QLabel *serverLabel = new QLabel(tr("Server:"));
//  QComboBox *serverCombo = new QComboBox;
//  serverCombo->addItem(tr("Qt (Australia)"));
//  serverCombo->addItem(tr("Qt (Germany)"));
//  serverCombo->addItem(tr("Qt (Norway)"));
//  serverCombo->addItem(tr("Qt (People's Republic of China)"));
//  serverCombo->addItem(tr("Qt (USA)"));
  
//  QHBoxLayout *serverLayout = new QHBoxLayout;
//  serverLayout->addWidget(serverLabel);
//  serverLayout->addWidget(serverCombo);
  
//  QVBoxLayout* configLayout = new QVBoxLayout;
//  configLayout->addLayout(serverLayout);
//  configGroup->setLayout(configLayout);
  
  //create color group
  QGroupBox* color_group = new QGroupBox(tr("Color Edit"));
  QVBoxLayout* color_edit_layout = new QVBoxLayout;
  
  QHBoxLayout* back_ground_layout = new QHBoxLayout;
  QColor color;
  int* background;
  background = VtkPropManage::BackGround(); 
  
  color.setRgb(background[0], background[1], background[2]);
  QRgb rgb = color.rgb();
  QLabel* back_ground_label = new QLabel(tr("Background color:")); 
  
  ColorEdit* back_ground = new ColorEdit(rgb, kBackGround);
  back_ground_layout->addWidget(back_ground_label);
  back_ground_layout->addWidget(back_ground);
  
  int* edge;
  edge = VtkPropManage::CellEdge();
  color.setRgb(edge[0], edge[1], edge[2]);
  rgb = color.rgb();
  QHBoxLayout* edge_color_layout = new QHBoxLayout;
  QLabel* edge_color_label = new QLabel(tr("Edge color:"));
  ColorEdit* edge_color = new ColorEdit(rgb, kCellEdge);
  edge_color_layout->addWidget(edge_color_label);
  edge_color_layout->addWidget(edge_color);
  
  int* cell;
  cell = VtkPropManage::Cell();
  color.setRgb(cell[0], cell[1], cell[2]);
  rgb = color.rgb();
  
  QHBoxLayout* cell_color_layout = new QHBoxLayout;
  QLabel* cell_color_label = new QLabel(tr("Cell color:"));
  ColorEdit* cell_color = new ColorEdit(rgb, kCell);
  cell_color_layout->addWidget(cell_color_label);
  cell_color_layout->addWidget(cell_color);
  
  //color_edit_layout->addWidget(back_ground);
  color_edit_layout->addLayout(back_ground_layout);
  color_edit_layout->addLayout(edge_color_layout);
  color_edit_layout->addLayout(cell_color_layout);
  
  color_group->setLayout(color_edit_layout);
  
  QVBoxLayout *mainLayout = new QVBoxLayout;
  //mainLayout->addWidget(configGroup);
  mainLayout->addWidget(color_group);
  mainLayout->addStretch(1);
  setLayout(mainLayout);
}

UpdatePage::UpdatePage(QWidget *parent)
  : QWidget(parent)
{
  QGroupBox *updateGroup = new QGroupBox(tr("Package selection"));
  QCheckBox *systemCheckBox = new QCheckBox(tr("Update system"));
  QCheckBox *appsCheckBox = new QCheckBox(tr("Update applications"));
  QCheckBox *docsCheckBox = new QCheckBox(tr("Update documentation"));
  
  QGroupBox *packageGroup = new QGroupBox(tr("Existing packages"));
  
  QListWidget *packageList = new QListWidget;
  QListWidgetItem *qtItem = new QListWidgetItem(packageList);
  qtItem->setText(tr("Qt"));
  QListWidgetItem *qsaItem = new QListWidgetItem(packageList);
  qsaItem->setText(tr("QSA"));
  QListWidgetItem *teamBuilderItem = new QListWidgetItem(packageList);
  teamBuilderItem->setText(tr("Teambuilder"));
  
  QPushButton *startUpdateButton = new QPushButton(tr("Start update"));
  
  QVBoxLayout *updateLayout = new QVBoxLayout;
  updateLayout->addWidget(systemCheckBox);
  updateLayout->addWidget(appsCheckBox);
  updateLayout->addWidget(docsCheckBox);
  updateGroup->setLayout(updateLayout);
  
  QVBoxLayout *packageLayout = new QVBoxLayout;
  packageLayout->addWidget(packageList);
  packageGroup->setLayout(packageLayout);
  
  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget(updateGroup);
  mainLayout->addWidget(packageGroup);
  mainLayout->addSpacing(12);
  mainLayout->addWidget(startUpdateButton);
  mainLayout->addStretch(1);
  setLayout(mainLayout);
}

QueryPage::QueryPage(QWidget *parent)
  : QWidget(parent)
{
  QGroupBox *packagesGroup = new QGroupBox(tr("Look for packages"));
  
  QLabel *nameLabel = new QLabel(tr("Name:"));
  QLineEdit *nameEdit = new QLineEdit;
  
  QLabel *dateLabel = new QLabel(tr("Released after:"));
  QDateTimeEdit *dateEdit = new QDateTimeEdit(QDate::currentDate());
  
  QCheckBox *releasesCheckBox = new QCheckBox(tr("Releases"));
  QCheckBox *upgradesCheckBox = new QCheckBox(tr("Upgrades"));
  
  QSpinBox *hitsSpinBox = new QSpinBox;
  hitsSpinBox->setPrefix(tr("Return up to "));
  hitsSpinBox->setSuffix(tr(" results"));
  hitsSpinBox->setSpecialValueText(tr("Return only the first result"));
  hitsSpinBox->setMinimum(1);
  hitsSpinBox->setMaximum(100);
  hitsSpinBox->setSingleStep(10);
  
  QPushButton *startQueryButton = new QPushButton(tr("Start query"));
  
  QGridLayout *packagesLayout = new QGridLayout;
  packagesLayout->addWidget(nameLabel, 0, 0);
  packagesLayout->addWidget(nameEdit, 0, 1);
  packagesLayout->addWidget(dateLabel, 1, 0);
  packagesLayout->addWidget(dateEdit, 1, 1);
  packagesLayout->addWidget(releasesCheckBox, 2, 0);
  packagesLayout->addWidget(upgradesCheckBox, 3, 0);
  packagesLayout->addWidget(hitsSpinBox, 4, 0, 1, 2);
  packagesGroup->setLayout(packagesLayout);
  
  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget(packagesGroup);
  mainLayout->addSpacing(12);
  mainLayout->addWidget(startQueryButton);
  mainLayout->addStretch(1);
  setLayout(mainLayout);
}
}
}
