#pragma once
#ifndef CONFIGEDITWIDGET_H
#define CONFIGEDITWIDGET_H
#include <QRgb>
#include <QWidget>
class QLineEdit;
class QGraphicsScene;
class QFrame;

namespace sqi 
{
namespace WidgetAction
{

class ParameterEdit : public QWidget
{
public:
  virtual void EmitChange() = 0;
};

class ColorEdit : public ParameterEdit
{
  Q_OBJECT
  
public:

  ColorEdit(QRgb initialColor, int id);
  QRgb color() const {return color_;}
  virtual void EmitChange() {emit SignalColorChanged(color_, id_);}

public slots:

  void SlotEditDone();

signals:

  void SignalColorChanged(QRgb color, int id);

protected:

  virtual void mousePressEvent(QMouseEvent *event);
  void SetColor(QRgb color); // also emits colorChanged()

private:
  
  QGraphicsScene *m_dialogParentScene;
  QLineEdit* line_edit_;
  QFrame* button_;
  QRgb color_;
  int id_;
};

//class FloatEdit : public ParameterEdit
//{
//  Q_OBJECT
  
//public:
  
//  FloatEdit(float initialValue, int id);
//  float value() const {return m_value;}
//  virtual void emitChange() {emit valueChanged(m_value, m_id);}

//public slots:

//  void editDone();

//signals:

//  void valueChanged(float value, int id);

//private:

//  QGraphicsScene *m_dialogParentScene;
//  QLineEdit *m_lineEdit;
//  float m_value;
//  int m_id;
//};

}
}
#endif // CONFIGEDITWIDGET_H
