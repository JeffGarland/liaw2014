/**
 *  \file stache_model.hpp
 *
 *  Copyright 2014 Kevin Harris
 *  Copyright 2014 Michael Caisse : ciere.com
 *  Copyright 2014 Michal Bukovsky
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_BOOSTACHE_MODEL_STACHE_HPP
#define BOOST_BOOSTACHE_MODEL_STACHE_HPP

#include <boost/spirit/include/support_extended_variant.hpp>
#include <string>
#include <map>
#include <vector>
#include <functional>
#include <initializer_list>

namespace boost { namespace boostache { namespace model
{
   typedef std::function<std::string()> stache_string_function;
   typedef std::function<bool()> stache_bool_function;

   struct stache_variant : boost::spirit::extended_variant<
        std::string
      , stache_string_function
      , stache_bool_function
      , boost::recursive_wrapper<std::map<std::string, stache_variant>>
      , boost::recursive_wrapper<
            std::vector<std::map<std::string, stache_variant>>>
      >
   {
       typedef std::map<std::string, stache_variant> stache_model;

       stache_variant() : base_type() {}
       stache_variant(std::string const & rhs) : base_type(rhs) {}
       stache_variant(const char * rhs) : base_type(std::string{rhs}) {}
       stache_variant(stache_string_function const & rhs) : base_type(rhs) {}
       stache_variant(stache_bool_function const & rhs) : base_type(rhs) {}
       stache_variant(std::vector<stache_model> const & rhs) : base_type(rhs) {}
       stache_variant(stache_model const & rhs) : base_type(rhs) {}

       template <typename T>
       stache_variant(std::initializer_list<T> l) : base_type(l) {}

       template <typename T>
       stache_variant & operator=(T const & rhs)
       {
           base_type::operator=(rhs);
           return *this;
       }
   };

   typedef std::map<std::string, stache_variant> stache_model;

namespace customization
{

namespace detail
{

//template <typename Sink>
//struct lookup_visitor
//{
//    typedef void result_type;
//
//    lookup_visitor(Sink &sink): sink(sink) {}
//
//    template <typename param_type>
//    void operator()(const param_type &) const
//    {
//        sink("<<<invalid usage>>>");
//    }
//
//    //void operator()(const std::vector<frontend::stache_variant> &model) const
//    //{
//    //    sink(model);
//    //}
//
//    //void operator()(const frontend::stache_model &model) const
//    //{
//    //    sink(std::array<decltype(&model), 1>{{&model}});
//    //}
//
//    Sink &sink;
//};

template <typename Stream>
struct variable_lookup_visitor
{
    typedef void result_type;

    variable_lookup_visitor(vm::variable_sink<Stream> &sink): sink(sink) {}

    template <typename param_type>
    void operator()(const param_type &value) const
    {
        sink(value);
    }

    template <typename return_type>
    void operator()(std::function<return_type()> function) const
    {
        (*this)(function());
    }

    void operator()(const std::vector<stache_model> &) const
    {
        sink("<<<vector of entries is not variable>>>");
    }

    void operator()(const stache_model &) const
    {
        sink("<<<map of entries is not variable>>>");
    }

    vm::variable_sink<Stream> &sink;
};

} // namespace detail

template <typename Stream>
struct variable_lookup<Stream, stache_model>
{
    void operator()(const stache_model &model,
                    const std::string &key,
                    vm::variable_sink<Stream> &sink)
    {
        auto ivar = model.find(key);
        if (ivar != model.end()) {
            detail::variable_lookup_visitor<Stream> visitor(sink);
            boost::apply_visitor(visitor, ivar->second);
        }
    }
};

//template <>
//void get_section_value(const frontend::stache_model &model,
//                       const std::string &key,
//                       section_range_sink &sink)
//{
//    auto isubmodel = model.find(key);
//    if (isubmodel != model.end()) {
//        detail::section_getter_visitor get_section(sink);
//        boost::apply_visitor(get_section, isubmodel->second);
//    }
//}

}}}}

#endif // BOOST_BOOSTACHE_MODEL_STACHE_HPP

