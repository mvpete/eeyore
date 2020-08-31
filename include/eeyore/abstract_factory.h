#ifndef __abstract_factory_h__
#define __abstract_factory_h__

#include <memory>
#include <map>
#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <utility>

namespace eeyore
{
namespace detail
{
    template <typename T, typename ...ArgsT>
    struct base_class_type_traits
    {
        static constexpr const char* type_id()
        {
            static_assert(false, "not registered as a factory base class");
        }
        static constexpr bool is_registered = false;
        using ctor_args_t = std::tuple<ArgsT...>;
    };

    class arguments_base
    {
    public:
        virtual ~arguments_base() = default;
        virtual void* get() = 0;
    };

    template <typename T>
    struct arguments : arguments_base
    {
        arguments(T&&args)
            :arguments_(std::move(args))
        {
        }
        virtual void* get()
        {
            return &arguments_;
        }
        T arguments_;
    };

    class constructor_base
    {
    public:
        virtual ~constructor_base() = default;
        virtual void* create(arguments_base* erased_args) = 0;
    };

    template <typename T, typename ...ArgsT>
    class constructor : public constructor_base
    {
    public:
        virtual void* create(arguments_base* erased_args) override
        {
            auto tuple_args = reinterpret_cast<typename detail::base_class_type_traits<T, std::remove_cvref_t<ArgsT>...>::ctor_args_t*>(erased_args->get());
            auto concrete = std::apply([](auto&&... args) { return std::make_unique<T>(args...); }, *tuple_args);
            return concrete.release();
        }
    };

}

class abstract_factory
{
public:
    template <typename T, typename ...ArgsT>
    std::unique_ptr<T> create(ArgsT&&...args)
    {
        static_assert(detail::base_class_type_traits<T, std::remove_cvref_t<ArgsT>...>::is_registered, "arguments not registered for factory creation");

        const char* type_id = detail::base_class_type_traits<T, std::remove_cvref_t<ArgsT>...>::type_id();
        auto creator = creators_.find(type_id);
        if (creator == creators_.end())
            throw std::logic_error("no creator registered for type.");
        detail::arguments<typename detail::base_class_type_traits<T, std::remove_cvref_t<ArgsT>...>::ctor_args_t> arg_wrapper(std::forward_as_tuple(args...));
        void* obj = creator->second->create(&arg_wrapper);
        return std::unique_ptr<T>(reinterpret_cast<T*>(obj));
    }

protected:
    template<typename BaseT, typename ConcreteT, typename ...ArgsT>
    void map()
    {
        static_assert(std::is_base_of_v<BaseT, ConcreteT>, "invalid hierarchy");
        const char* type_id = detail::base_class_type_traits<BaseT, std::remove_cvref_t<ArgsT>...>::type_id();
        creators_[type_id] = std::make_unique<detail::constructor<ConcreteT, ArgsT...>>();
    }

private:
    std::map<const char*, std::unique_ptr<detail::constructor_base>> creators_;
};
}

#define MAKE_STRING(str) #str

#define ABSTRACT_FACTORY_BASE_CLASS(class_name, ...) \
template <>\
struct eeyore::detail::base_class_type_traits<class_name, __VA_ARGS__>\
{\
    static constexpr const char* type_id() \
    {\
       return  MAKE_STRING(class_name ## ( ## __VA_ARGS__ ## ));\
    }\
\
    static constexpr bool is_registered = true; \
    using ctor_args_t = std::tuple<__VA_ARGS__>;\
}\

#endif //__abstract_factory_h__