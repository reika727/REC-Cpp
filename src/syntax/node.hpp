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
            virtual void to_asm(code::generator&gen)const=0;
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
            void to_asm(code::generator&gen)const override;
    };
    class identifier final:public expression{
        public:
            const std::string name;
            identifier(const std::string&name);
            void check(semantics::analyzer&analy)const override;
            void to_asm(code::generator&gen)const override;
            void refer(code::generator&gen)const;
    };
    class fcall final:public expression{
        public:
            const std::shared_ptr<const expression>func;
            const std::vector<std::shared_ptr<const expression>>vars;
            fcall(const std::shared_ptr<const expression>&func,const std::vector<std::shared_ptr<const expression>>&vars);
            void check(semantics::analyzer&analy)const override;
            void to_asm(code::generator&gen)const override;
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
            void to_asm(code::generator&gen)const override;
    };
    class uminus final:public unopr{
        public:
            using unopr::unopr;
            void to_asm(code::generator&gen)const override;
    };
    class lognot final:public unopr{
        public:
            using unopr::unopr;
            void to_asm(code::generator&gen)const override;
    };
    class unopr_l:public expression{
        public:
            const std::shared_ptr<const identifier>arg;
            unopr_l(const std::shared_ptr<const expression>&arg);
            void check(semantics::analyzer&analy)const override final;
    };
    class preinc final:public unopr_l{
        public:
            using unopr_l::unopr_l;
            void to_asm(code::generator&gen)const override;
    };
    class predec final:public unopr_l{
        public:
            using unopr_l::unopr_l;
            void to_asm(code::generator&gen)const override;
    };
    class postinc final:public unopr_l{
        public:
            using unopr_l::unopr_l;
            void to_asm(code::generator&gen)const override;
    };
    class postdec final:public unopr_l{
        public:
            using unopr_l::unopr_l;
            void to_asm(code::generator&gen)const override;
    };
    class biopr:public expression{
        public:
            const std::shared_ptr<const expression>larg,rarg;
            biopr(const std::shared_ptr<const expression>&larg,const std::shared_ptr<const expression>&rarg);
            void check(semantics::analyzer&analy)const override final;
    };
    class bplus final:public biopr{
        public:
            using biopr::biopr;
            void to_asm(code::generator&gen)const override;
    };
    class bminus final:public biopr{
        public:
            using biopr::biopr;
            void to_asm(code::generator&gen)const override;
    };
    class multiply final:public biopr{
        public:
            using biopr::biopr;
            void to_asm(code::generator&gen)const override;
    };
    class divide final:public biopr{
        public:
            using biopr::biopr;
            void to_asm(code::generator&gen)const override;
    };
    class remain final:public biopr{
        public:
            using biopr::biopr;
            void to_asm(code::generator&gen)const override;
    };
    class equal final:public biopr{
        public:
            using biopr::biopr;
            void to_asm(code::generator&gen)const override;
    };
    class nequal final:public biopr{
        public:
            using biopr::biopr;
            void to_asm(code::generator&gen)const override;
    };
    class less final:public biopr{
        public:
            using biopr::biopr;
            void to_asm(code::generator&gen)const override;
    };
    class greater final:public biopr{
        public:
            using biopr::biopr;
            void to_asm(code::generator&gen)const override;
    };
    class leeq final:public biopr{
        public:
            using biopr::biopr;
            void to_asm(code::generator&gen)const override;
    };
    class greq final:public biopr{
        public:
            using biopr::biopr;
            void to_asm(code::generator&gen)const override;
    };
    class logand final:public biopr{
        public:
            using biopr::biopr;
            void to_asm(code::generator&gen)const override;
    };
    class logor final:public biopr{
        public:
            using biopr::biopr;
            void to_asm(code::generator&gen)const override;
    };
    class comma final:public biopr{
        public:
            using biopr::biopr;
            void to_asm(code::generator&gen)const override;
    };
    class biopr_l:public expression{
        public:
            const std::shared_ptr<const identifier>larg;
            const std::shared_ptr<const expression>rarg;
            biopr_l(const std::shared_ptr<const expression>&larg,const std::shared_ptr<const expression>&rarg);
            void check(semantics::analyzer&analy)const override final;
    };
    class assign final:public biopr_l{
        public:
            using biopr_l::biopr_l;
            void to_asm(code::generator&gen)const override;
    };
    class plasgn final:public biopr_l{
        public:
            using biopr_l::biopr_l;
            void to_asm(code::generator&gen)const override;
    };
    class miasgn final:public biopr_l{
        public:
            using biopr_l::biopr_l;
            void to_asm(code::generator&gen)const override;
    };
    class muasgn final:public biopr_l{
        public:
            using biopr_l::biopr_l;
            void to_asm(code::generator&gen)const override;
    };
    class diasgn final:public biopr_l{
        public:
            using biopr_l::biopr_l;
            void to_asm(code::generator&gen)const override;
    };
    class rmasgn final:public biopr_l{
        public:
            using biopr_l::biopr_l;
            void to_asm(code::generator&gen)const override;
    };
    class statement:public node{
        public:
            virtual ~statement()=default;
    };
    class single_statement:public statement{
        public:
            virtual ~single_statement()=default;
    };
    class expression_statement:public single_statement{
        public:
            const std::shared_ptr<const expression>expr;
            expression_statement(const std::shared_ptr<const expression>&expr);
            void check(semantics::analyzer&analy)const override;
            void to_asm(code::generator&gen)const override;
    };
    class null_statement final:public single_statement{
        public:
            void check(semantics::analyzer&analy)const override;
            void to_asm(code::generator&gen)const override;
    };
    class compound final:public statement{
        public:
            const std::vector<std::shared_ptr<const statement>>stats;
            compound(const std::vector<std::shared_ptr<const statement>>&stats);
            void check(semantics::analyzer&analy)const override;
            void to_asm(code::generator&gen)const override;
    };
    class define_var final:public statement{
        public:
            const std::vector<std::pair<std::string,std::shared_ptr<const expression>>>vars;
            define_var(const std::vector<std::pair<std::string,std::shared_ptr<const expression>>>&vars);
            void check(semantics::analyzer&analy)const override;
            void to_asm(code::generator&gen)const override;
    };
    class _if_else_ final:public statement{
        public:
            const std::shared_ptr<const expression_statement>cond;
            const std::shared_ptr<const statement>stat_if,stat_else;
            _if_else_(const std::shared_ptr<const expression_statement>&cond,const std::shared_ptr<const statement>&stat_if,const std::shared_ptr<const statement>&stat_else);
            void check(semantics::analyzer&analy)const override;
            void to_asm(code::generator&gen)const override;
    };
    class _while_ final:public statement{
        public:
            const std::shared_ptr<const expression_statement>cond;
            const std::shared_ptr<const statement>stat;
            _while_(const std::shared_ptr<const expression_statement>&cond,const std::shared_ptr<const statement>&stat);
            void check(semantics::analyzer&analy)const override;
            void to_asm(code::generator&gen)const override;
    };
    class _for_ final:public statement{
        public:
            const std::shared_ptr<const single_statement>init,cond,reinit;
            const std::shared_ptr<const statement>stat;
            _for_(const std::shared_ptr<const single_statement>&init,const std::shared_ptr<const single_statement>&cond,const std::shared_ptr<const single_statement>&reinit,const std::shared_ptr<const statement>&stat);
            void check(semantics::analyzer&analy)const override;
            void to_asm(code::generator&gen)const override;
    };
    class _break_ final:public statement{
        public:
            void check(semantics::analyzer&analy)const override;
            void to_asm(code::generator&gen)const override;
    };
    class _continue_ final:public statement{
        public:
            void check(semantics::analyzer&analy)const override;
            void to_asm(code::generator&gen)const override;
    };
    class _return_ final:public statement{
        public:
            const std::shared_ptr<const expression_statement>value;
            _return_(const std::shared_ptr<const expression_statement>&value);
            void check(semantics::analyzer&analy)const override;
            void to_asm(code::generator&gen)const override;
    };
    class define_function final:public node{
        public:
            const std::string name;
            const std::vector<std::string>args;
            const std::shared_ptr<const compound>com;
            define_function(std::string name,const std::vector<std::string>&args,const std::shared_ptr<const compound>&com);
            void check(semantics::analyzer&analy)const override;
            void to_asm(code::generator&gen)const override;
    };
}
