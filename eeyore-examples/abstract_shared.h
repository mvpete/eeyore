#pragma once
class button
{
public:
    virtual ~button() = default;
    virtual void click() = 0;
};

class window
{

public:
    virtual ~window() = default;
    virtual void click() = 0;
};

class linux_button : public button
{
    std::string value_;
public:

    linux_button(const std::string& value)
        :value_(value)
    {
    }

    virtual void click() override
    {
        std::cout << "linux button: " << value_ << std::endl;
    }
};

class linux_window : public window
{
public:
    virtual void click() override
    {
        std::cout << "linux window" << std::endl;
    }
};