/**
 *  \file detail/engine_visitor.hpp
 *
 *  Copyright 2014 Michael Caisse : ciere.com
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef BOOST_BOOSTACHE_VM_DETAIL_ENGINE_VISITOR_HPP
#define BOOST_BOOSTACHE_VM_DETAIL_ENGINE_VISITOR_HPP

#include <boost/boostache/vm/engine_ast.hpp>
#include <boost/boostache/vm/lookups.hpp>

////////// some test hackery ////////////////////////

namespace boost { namespace boostache { namespace model {
namespace customization {

    template <typename Stream, typename Object>
    void render(Stream & stream, Object const & v)
    {
        stream << v;
    }

    template <typename Stream, typename Model>
    struct variable_lookup {
        void operator()(Model &,
                        const std::string &key,
                        vm::variable_sink<Stream> &)
        {
            //std::cout << __PRETTY_FUNCTION__;
            throw std::runtime_error("you should write specialization for "
                                     "variable_lookup(" + key + ") for type: "
                                     + typeid(Model).name());
        }
    };

   // template <typename Stream, typename Context>
   // void render(Stream & stream, Context const & v, std::string const & name);
   // {
   //    stream << "name{" << name << "} : " << v;
   // }

   //template <typename Context>
   //bool test(std::string const & name, Context const & c);
}}}}

/////////////////////////////////////////////////////


namespace boost { namespace boostache { namespace vm { namespace detail
{
    template <typename Stream, typename Context>
    class engine_visitor_base
    {
    public:
        typedef void result_type;

        engine_visitor_base(Stream & s, Context const & c)
            : stream(s)
            , context(c)
        {}

        void operator()(ast::undefined) const
        {}

        void operator()(ast::literal const & v) const
        {
            using boost::boostache::model::customization::render;
            render(stream, v.value);
        }

        void operator()(ast::variable const & v) const
        {}

        void operator()(ast::render const & v) const;

        void operator()(ast::for_each const & v) const
        {
        }

        void operator()(ast::if_then_else const & v) const
        {
            //using boost::boostache::model::customization::test;

            //if(test(v.condition_.name,context))
            //{
            //   boost::apply_visitor(*this, v.then_);
            //}
            //else
            //{
            //   boost::apply_visitor(*this, v.else_);
            //}
        }

        void operator()(ast::node_list const & nodes) const
        {
            for(auto const & node : nodes.nodes)
            {
                boost::apply_visitor(*this, node);
            }
        }

        void operator()(ast::node const & v) const
        {
            boost::apply_visitor(*this, v);
        }

    private:
        Stream & stream;
        Context const & context;
    };

} // namespace detail

    template <typename Stream>
    struct variable_sink: public boost::noncopyable
    {
        variable_sink(Stream &out)
            : out(out), printed(false)
        {}

        template <typename Object>
        void operator()(Object const & value)
        {
            using boost::boostache::model::customization::render;
            printed = true;
            render(out, value);
        }

        bool isprinted() const { return printed;}

    private:
        Stream &out;
        bool printed;
    };

    template <typename Stream, typename Context>
    void detail::engine_visitor_base<Stream, Context>::operator()
    (ast::render const & v) const
    {
        using boost::boostache::model::customization::variable_lookup;
        vm::variable_sink<Stream> sink(stream);
        variable_lookup<Stream, Context>()(context, v.name, sink);
        if (!sink.isprinted())
        {
            // if user don't call sink it means that no variable exist
            //if (parent_lookup) parent_lookup(v);
        }
    }

}}}

#endif // BOOST_BOOSTACHE_VM_DETAIL_ENGINE_VISITOR_HPP

