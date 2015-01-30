/**
 *  \file parse.hpp
 *
 *  Copyright 2014 Michael Caisse : ciere.com
 *
 *  Generic parser entry point. Call parse with the input format
 *  as the template parameter:
 *
 *     auto ast = parse<format::stache>(...);
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_BOOSTACHE_FRONTEND_PARSE_HPP
#define BOOST_BOOSTACHE_FRONTEND_PARSE_HPP

#include <boost/spirit/include/qi_parse.hpp>
#include <boost/spirit/include/support_istream_iterator.hpp>
#include <iostream>

namespace boost { namespace boostache { namespace frontend
{
   template <typename Format, typename Iterator>
   typename Format::ast_t parse(Iterator & begin, Iterator const & end)
   {
      typename Format::ast_t ast;
      typename Format::template grammar_t<Iterator> grammar;

      // TODO mjc : should throw with parse error location
      if(!boost::spirit::qi::phrase_parse( begin, end
                                         , grammar
                                         , typename Format::skipper_t{}
                                         , ast ))
      {
         ast = typename Format::ast_t{};
      }
      return ast;
   }

   template <typename Format>
   typename Format::ast_t parse(std::istream& input)
   {
      // TODO mjc : store/restor ios state?
      //input.unset(std::ios::skipws);

      return parse<Format>( boost::spirit::istream_iterator(input)
                          , boost::spirit::istream_iterator() );
   }
}}}

#endif // BOOST_BOOSTACHE_FRONTEND_PARSE_HPP

