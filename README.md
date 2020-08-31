# eeyore
Elements of Reusable Software

<img src="https://github.com/mvpete/eeyore/blob/main/eeyore-head.png" width="75" height="65">

## Abstract Factory
My twist on the GoF Abstract Factory. Read my blog post [here](http://404)

```
    using namespace eeyore;

    // register base classes w/ constructor arguments
    ABSTRACT_FACTORY_BASE_CLASS(button, std::string);
    ABSTRACT_FACTORY_BASE_CLASS(window);


    class linux_widget_factory : public abstract_factory
    {
    public:
        linux_widget_factory()
        {
            // register mapping between concrete implementation and registered base class
            map<button, linux_button, std::string>();
            map<window, linux_window>();
        }
    };

    void main()
    {
        // instantiate the factory
        std::unique_ptr<abstract_factory> wf = std::make_unique<linux_widget_factory>();
        std::string value = "test";
        // create w/ arguments
        auto btn = wf->create<button>(value);
        auto btn2 = wf->create<button>(3);
        auto wnd = wf->create<window>();
        btn->click();
        wnd->click();
    }


```
