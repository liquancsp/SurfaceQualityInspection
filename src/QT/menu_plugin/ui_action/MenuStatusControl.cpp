#include <menu_plugin/ui_action/MenuStatusControl.h>
#include <menu_plugin/ui_action/menu_inspect_item.h>
#include <menu_plugin/ui_action/menu_view_item.h>

namespace lq{


void MenuStatusControl::InvalidInspectMenu()
{
  this->inspect_item_handle_->GetUiHandle()->SetInspectActionEable(false);
  this->inspect_item_handle_->GetUiHandle()->SetSinglePropertyActionEnable(false);
}

void MenuStatusControl::ResumeInspectMenu(bool flag)
{
  this->inspect_item_handle_->GetUiHandle()->SetInspectActionEable(flag);
  this->inspect_item_handle_->GetUiHandle()->SetSinglePropertyActionEnable(true);
}


void MenuStatusControl::InvalidViewMenu()
{
  this->view_item_handle_->GetUiHandle()->SetActionEnable(false);
}

void MenuStatusControl::ResumeViewMenu()
{
  this->view_item_handle_->GetUiHandle()->SetActionEnable(true);
}

void MenuStatusControl::InvalidMenuStatus()
{
  MenuStatusControl::InvalidInspectMenu();
  MenuStatusControl::InvalidViewMenu();
}

void MenuStatusControl::ResumeMenuStatus(bool flag)
{
  MenuStatusControl::ResumeInspectMenu(flag);
  MenuStatusControl::ResumeViewMenu();
}


}
