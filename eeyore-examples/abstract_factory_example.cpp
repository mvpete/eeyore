
#include <eeyore/abstract_factory.h>

#include <string>
#include <iostream>

#include "abstract_shared.h"

using namespace eeyore;

ABSTRACT_FACTORY_BASE_CLASS(button, std::string);
ABSTRACT_FACTORY_BASE_CLASS(button, int);
ABSTRACT_FACTORY_BASE_CLASS(window);


class linux_widget_factory : public abstract_factory
{
public:
    linux_widget_factory()
    {
        map<button, linux_button, std::string>();
        map<window, linux_window>();
    }
};

void main()
{
    std::unique_ptr<abstract_factory> wf = std::make_unique<linux_widget_factory>();
    std::string value = "test";
    auto btn = wf->create<button>(value);
    auto btn2 = wf->create<button>(3);
    auto wnd = wf->create<window>();
    btn->click();
    wnd->click();
}


