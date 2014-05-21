/**
 *  \file detail/stache_compiler.hpp
 *
 *  Copyright 2014 Michael Caisse : ciere.com
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_BOOSTACHE_BACKEND_DETAIL_STACHE_COMPILER_HPP
#define BOOST_BOOSTACHE_BACKEND_DETAIL_STACHE_COMPILER_HPP

#include <boost/boostache/vm/engine_ast.hpp>
#include <boost/boostache/frontend/stache/ast.hpp>
#include <boost/boostache/backend/detail/stache_compiler.hpp>

namespace boost { namespace boostache { namespace backend { namespace stache_compiler
{
   namespace fe = boost::boostache::frontend;
   namespace detail
   {
      class stache_visit
      {
      public:
         typedef vm::ast::node result_type;

         stache_visit(std::ostream& out)
            : out(out)
         {}

         vm::ast::node operator()(fe::stache::ast::undefined) const
         {
            out << "WHOA! we have an undefined" << std::endl;
            return vm::ast::node{};
         }

         vm::ast::node operator()(fe::stache::ast::literal_text const & v) const
         {
            return vm::ast::literal{v};
         }

         vm::ast::node operator()(fe::stache::ast::variable const & v) const
         {
            return vm::ast::render{v.value};
         }

         vm::ast::node operator()(fe::stache::ast::section const & v) const
         {
            vm::ast::node_list vm_ast;
            for(auto const & node : v.nodes)
            {
               vm_ast.nodes.push_back(boost::apply_visitor(*this, node));
            }

            vm::ast::if_then_else if_block;
            if_block.condition_.name = v.name;

            if(v.is_inverted)  { if_block.else_ = vm_ast; }
            else               { if_block.then_ = vm_ast; }

            return if_block;
         }

         vm::ast::node operator()(fe::stache::ast::comment const & v) const
         {
            return vm::ast::literal{};
         }

         vm::ast::node operator()(fe::stache::ast::partial const & v) const
         {
            return vm::ast::literal{};
         }

         vm::ast::node operator()(fe::stache::ast::node_list const & nodes) const
         {
            vm::ast::node_list node_list;
            for(auto const & node : nodes)
            {
               node_list.nodes.push_back(boost::apply_visitor(*this, node));
            }
            return node_list;
         }

         vm::ast::node operator()(fe::stache::ast::root const & nodes) const
         {
            vm::ast::node_list node_list;
            for(auto const & node : nodes)
            {
               node_list.nodes.push_back(boost::apply_visitor(*this, node));
            }
            return node_list;
         }

      private:
         std::ostream& out;
      };
   }

   inline vm::ast::node compile(fe::stache::ast::root const & ast)
   {
      detail::stache_visit visit(std::cout);
      return visit(ast);
   }
}}}}

#endif

