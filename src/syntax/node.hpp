#pragma once
#include"../semantics/analyzer.hpp"
#include"../code/generator.hpp"
#include<memory>
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
            const std::shared_ptr<const expression>func;
            const std::vector<std::shared_ptr<const expression>>vars;
            fcall(const std::shared_ptr<const expression>&func,const std::vector<std::shared_ptr<const expression>>&vars);
            void check(semantics::analyzer&analy)const override;
            void to_asm(code::generator&cg)const override;
    };
    class unopr:public expression{
        public:
            const std::shared_ptr<const expression>arg;
            unopr(const std::shared_ptr<const expression>&arg);
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
            const std::shared_ptr<const ident>arg;
            unopr_l(const std::shared_ptr<const expression>&arg);
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
            const std::shared_ptr<const expression>larg,rarg;
            biopr(const std::shared_ptr<const expression>&larg,const std::shared_ptr<const expression>&rarg);
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
            const std::shared_ptr<const ident>larg;
            const std::shared_ptr<const expression>rarg;
            biopr_l(const std::shared_ptr<const expression>&larg,const std::shared_ptr<const expression>&rarg);
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
            const std::shared_ptr<const expression>stat;
            single(const std::shared_ptr<const expression>&stat);
            void check(semantics::analyzer&analy)const override;
            void to_asm(code::generator&cg)const override;
    };
    class compound final:public statement{
        public:
            const std::vector<std::shared_ptr<const statement>>stats;
            compound(const std::vector<std::shared_ptr<const statement>>&stats);
            void check(semantics::analyzer&analy)const override;
            void to_asm(code::generator&cg)const override;
    };
    class define_var final:public statement{
        public:
            const std::vector<std::pair<std::string,std::shared_ptr<const expression>>>vars;
            define_var(const std::vector<std::pair<std::string,std::shared_ptr<const expression>>>&vars);
            void check(semantics::analyzer&analy)const override;
            void to_asm(code::generator&cg)const override;
    };
    class _if_else_ final:public statement{
        public:
            const std::shared_ptr<const single>cond;
            const std::shared_ptr<const statement>st1,st2;
            _if_else_(const std::shared_ptr<const single>&cond,const std::shared_ptr<const statement>&st1,const std::shared_ptr<const statement>&st2);
            void check(semantics::analyzer&analy)const override;
            void to_asm(code::generator&cg)const override;
    };
    class _while_ final:public statement{
        public:
            const std::shared_ptr<const single>cond;
            const std::shared_ptr<const statement>st;
            _while_(const std::shared_ptr<const single>&cond,const std::shared_ptr<const statement>&st);
            void check(semantics::analyzer&analy)const override;
            void to_asm(code::generator&cg)const override;
    };
    class _for_ final:public statement{
        public:
            const std::shared_ptr<const single>init,cond,reinit;
            const std::shared_ptr<const statement>st;
            _for_(const std::shared_ptr<const single>&init,const std::shared_ptr<const single>&cond,const std::shared_ptr<const single>&reinit,const std::shared_ptr<const statement>&st);
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
            const std::shared_ptr<const single>val;
            _return_(const std::shared_ptr<const single>&val);
            void check(semantics::analyzer&analy)const override;
            void to_asm(code::generator&cg)const override;
    };
    class function final:public node{
        public:
            const std::string name;
            const std::vector<std::string>args;
            const std::shared_ptr<const compound>com;
            function(std::string name,const std::vector<std::string>&args,const std::shared_ptr<const compound>&com);
            void check(semantics::analyzer&analy)const override;
            void to_asm(code::generator&cg)const override;
    };
}
