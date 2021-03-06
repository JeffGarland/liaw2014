/**
 *  \file test_utils.hpp
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_TEST_TEST_UTILS_HPP
#define BOOST_TEST_TEST_UTILS_HPP

#include <boost/boostache/simple_parser.hpp>
#include <boost/boostache/model/dynamic_model_printer.hpp>

template <typename model_type> inline
std::string print(const boost::boostache::frontend::stache::ast::root &ast,
                  const model_type &model)
{
    std::ostringstream out;
    boost::boostache::model::print(out, ast, model);
    return out.str();
}

inline boost::boostache::frontend::stache::ast::root parse(const std::string &text)
{
    boost::boostache::frontend::stache::ast::root ast;
    if( !boost::boostache::simple_parse_template(text, ast) )
    {
        throw std::runtime_error("Parse failed");
    }
    return ast;
}

#endif

