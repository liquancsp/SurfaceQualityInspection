#pragma once
#ifndef CONFIGPAGE_H
#define CONFIGPAGE_H

#include <QWidget>

namespace sqi {

namespace WidgetAction{

class ConfigurationPage : public QWidget
{
public:
  ConfigurationPage(QWidget *parent = 0);
};

class QueryPage : public QWidget
{
public:
  QueryPage(QWidget *parent = 0);
};

class UpdatePage : public QWidget
{
public:
  UpdatePage(QWidget *parent = 0);
};

}
}
#endif // CONFIGPAGE_H
