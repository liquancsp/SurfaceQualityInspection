#ifndef STATUSBAR_H
#define STATUSBAR_H
#include <QStatusBar>
#include <QLabel>
#include <QProgressBar>

#if defined(_MSC_VER)
#include <memory>
#endif

#if defined(__GNUC__)
#include <tr1/memory>
#endif

namespace sqi{
namespace qt{

class StatusBar : public QStatusBar
{
  Q_OBJECT
  
public :
  StatusBar(QWidget* parent = 0);
  ~StatusBar();
  
  
public slots:
  void SlotActionDone(const char* str);
  
  void SlotChangeLabelColorGreen();
  void SlotChangeLabelColorRed();
  void SlotChangeLabelColorWhite();
  void SlotProgressRange(int beign, int end);
  void SlotProgressValue(int value);
  
private:
  void ShowSuccessAnimation();
  void ShowFailAnimation();
  std::tr1::shared_ptr<QLabel> label_;
  std::tr1::shared_ptr<QProgressBar> progress_bar_;
  size_t count;
};

}
}

#endif // STATUSBAR_H
