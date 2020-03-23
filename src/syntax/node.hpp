#pragma once
#include"../semantics/analyzer.hpp"
#include"../code/generator.hpp"
#include<string>
#include<utility>
#include<vector>
namespace syntax{
    class node{
        public:
            virtual ~node()=default;
            virtual void check(semantics::analyzer&analy)const=0;
            virtual void to_asm(code::generator&cg)const=0;
    };
    class expression:public node{
        public:
            virtual ~expression()=default;
    };
    class numeric final:public expression{
        public:
            const int value;
            numeric(int value);
            void check(semantics::analyzer&analy)const noexcept override;
            void to_asm(code::generator&cg)const override;
    };
    class ident final:public expression{
        public:
            const std::string name;
            ident(const std::string&name);
            void check(semantics::analyzer&analy)const override;
            void to_asm(code::generator&cg)const override;
            void refer(code::generator&cg)const;
    };
    class fcall final:public expression{
        public:
            const expression*const func;
            const std::vector<const expression*>*const vars;
            fcall(const expression*func,decltype(vars)vars);
            ~fcall()override;
            void check(semantics::analyzer&analy)const override;
            void to_asm(code::generator&cg)const override;
    };
    class unopr:public expression{
        public:
            const expression*const arg;
            unopr(const expression*arg);
            virtual ~unopr()override;
            void check(semantics::analyzer&analy)const override final;
    };
    class uplus final:public unopr{
        public:
            using unopr::unopr;
            void to_asm(code::generator&cg)const override;
    };
    class uminus final:public unopr{
        public:
            using unopr::unopr;
            void to_asm(code::generator&cg)const override;
    };
    class lognot final:public unopr{
        public:
            using unopr::unopr;
            void to_asm(code::generator&cg)const override;
    };
    class unopr_l:public expression{
        public:
            const ident*const arg;
            unopr_l(const expression*arg);
            virtual ~unopr_l()override;
            void check(semantics::analyzer&analy)const override final;
    };
    class preinc final:public unopr_l{
        public:
            using unopr_l::unopr_l;
            void to_asm(code::generator&cg)const override;
    };
    class predec final:public unopr_l{
        public:
            using unopr_l::unopr_l;
            void to_asm(code::generator&cg)const override;
    };
    class postinc final:public unopr_l{
        public:
            using unopr_l::unopr_l;
            void to_asm(code::generator&cg)const override;
    };
    class postdec final:public unopr_l{
        public:
            using unopr_l::unopr_l;
            void to_asm(code::generator&cg)const override;
    };
    class biopr:public expression{
        public:
            const expression*const larg,*const rarg;
            biopr(const expression*larg,const expression*rarg);
            virtual ~biopr()override;
            void check(semantics::analyzer&analy)const override final;
    };
    class plus final:public biopr{
        public:
            using biopr::biopr;
            void to_asm(code::generator&cg)const override;
    };
    class minus final:public biopr{
        public:
            using biopr::biopr;
            void to_asm(code::generator&cg)const override;
    };
    class multi final:public biopr{
        public:
            using biopr::biopr;
            void to_asm(code::generator&cg)const override;
    };
    class divide final:public biopr{
        public:
            using biopr::biopr;
            void to_asm(code::generator&cg)const override;
    };
    class remain final:public biopr{
        public:
            using biopr::biopr;
            void to_asm(code::generator&cg)const override;
    };
    class equal final:public biopr{
        public:
            using biopr::biopr;
            void to_asm(code::generator&cg)const override;
    };
    class nequal final:public biopr{
        public:
            using biopr::biopr;
            void to_asm(code::generator&cg)const override;
    };
    class less final:public biopr{
        public:
            using biopr::biopr;
            void to_asm(code::generator&cg)const override;
    };
    class greater final:public biopr{
        public:
            using biopr::biopr;
            void to_asm(code::generator&cg)const override;
    };
    class leeq final:public biopr{
        public:
            using biopr::biopr;
            void to_asm(code::generator&cg)const override;
    };
    class greq final:public biopr{
        public:
            using biopr::biopr;
            void to_asm(code::generator&cg)const override;
    };
    class logand final:public biopr{
        public:
            using biopr::biopr;
            void to_asm(code::generator&cg)const override;
    };
    class logor final:public biopr{
        public:
            using biopr::biopr;
            void to_asm(code::generator&cg)const override;
    };
    class comma final:public biopr{
        public:
            using biopr::biopr;
            void to_asm(code::generator&cg)const override;
    };
    class biopr_l:public expression{
        public:
            const ident*const larg;
            const expression*const rarg;
            biopr_l(const expression*larg,const expression*rarg);
            virtual ~biopr_l()override;
            void check(semantics::analyzer&analy)const override final;
    };
    class assign final:public biopr_l{
        public:
            using biopr_l::biopr_l;
            void to_asm(code::generator&cg)const override;
    };
    class plasgn final:public biopr_l{
        public:
            using biopr_l::biopr_l;
            void to_asm(code::generator&cg)const override;
    };
    class miasgn final:public biopr_l{
        public:
            using biopr_l::biopr_l;
            void to_asm(code::generator&cg)const override;
    };
    class muasgn final:public biopr_l{
        public:
            using biopr_l::biopr_l;
            void to_asm(code::generator&cg)const override;
    };
    class diasgn final:public biopr_l{
        public:
            using biopr_l::biopr_l;
            void to_asm(code::generator&cg)const override;
    };
    class rmasgn final:public biopr_l{
        public:
            using biopr_l::biopr_l;
            void to_asm(code::generator&cg)const override;
    };
    class statement:public node{
        public:
            virtual ~statement()=default;
    };
    class single final:public statement{
        public:
            const expression*const stat;
            single(const expression*stat);
            ~single()override;
            void check(semantics::analyzer&analy)const override;
            void to_asm(code::generator&cg)const override;
    };
    class compound final:public statement{
        public:
            const std::vector<const statement*>*const stats;
            compound(decltype(stats)stats);
            ~compound()override;
            void check(semantics::analyzer&analy)const override;
            void to_asm(code::generator&cg)const override;
    };
    class define_var final:public statement{
        public:
            const std::vector<std::pair<std::string,const expression*>>*vars;
            define_var(decltype(vars)vars);
            ~define_var()override;
            void check(semantics::analyzer&analy)const override;
            void to_asm(code::generator&cg)const override;
    };
    class _if_else_ final:public statement{
        public:
            const single*const cond;
            const statement*const st1,*const st2;
            _if_else_(const single*cond,const statement*st1,const statement*st2);
            ~_if_else_()override;
            void check(semantics::analyzer&analy)const override;
            void to_asm(code::generator&cg)const override;
    };
    class _while_ final:public statement{
        public:
            const single*const cond;
            const statement*const st;
            _while_(const single*cond,const statement*st);
            ~_while_()override;
            void check(semantics::analyzer&analy)const override;
            void to_asm(code::generator&cg)const override;
    };
    class _for_ final:public statement{
        public:
            const single*const init,*const cond,*const reinit;
            const statement*const st;
            _for_(const single*init,const single*cond,const single*reinit,const statement*st);
            ~_for_()override;
            void check(semantics::analyzer&analy)const override;
            void to_asm(code::generator&cg)const override;
    };
    class _break_ final:public statement{
        public:
            void check(semantics::analyzer&analy)const override;
            void to_asm(code::generator&cg)const override;
    };
    class _continue_ final:public statement{
        public:
            void check(semantics::analyzer&analy)const override;
            void to_asm(code::generator&cg)const override;
    };
    class _return_ final:public statement{
        public:
            const single*const val;
            _return_(const single*val);
            ~_return_();
            void check(semantics::analyzer&analy)const override;
            void to_asm(code::generator&cg)const override;
    };
    class function final:public node{
        public:
            const std::string name;
            const std::vector<std::string>*const args;
            const compound*const com;
            function(std::string name,decltype(args)args,const compound*com);
            ~function()override;
            void check(semantics::analyzer&analy)const override;
            void to_asm(code::generator&cg)const override;
    };
}
