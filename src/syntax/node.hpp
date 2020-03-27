#pragma once
#include"../lexicon/token_array.hpp"
#include"../semantics/analyzer.hpp"
#include"../code/generator.hpp"
#include<memory>
#include<string>
#include<utility>
#include<vector>
namespace syntax{
    class node{
        protected:
            const int line,col;
        public:
            node(int line,int col);
            virtual ~node()=default;
            virtual void check(semantics::analyzer&analy)const=0;
            virtual void to_asm(code::generator&gen)const=0;
    };
    class expression:public node{
        private:
            static std::shared_ptr<const expression>get_order15(lexicon::token_array&ta);
            static std::shared_ptr<const expression>get_order14(lexicon::token_array&ta);
            static std::shared_ptr<const expression>get_order13(lexicon::token_array&ta);
            static std::shared_ptr<const expression>get_order12(lexicon::token_array&ta);
            static std::shared_ptr<const expression>get_order11(lexicon::token_array&ta);
            static std::shared_ptr<const expression>get_order10(lexicon::token_array&ta);
            static std::shared_ptr<const expression>get_order09(lexicon::token_array&ta);
            static std::shared_ptr<const expression>get_order08(lexicon::token_array&ta);
            static std::shared_ptr<const expression>get_order07(lexicon::token_array&ta);
            static std::shared_ptr<const expression>get_order06(lexicon::token_array&ta);
            static std::shared_ptr<const expression>get_order05(lexicon::token_array&ta);
            static std::shared_ptr<const expression>get_order04(lexicon::token_array&ta);
            static std::shared_ptr<const expression>get_order03(lexicon::token_array&ta);
            static std::shared_ptr<const expression>get_order02(lexicon::token_array&ta);
            static std::shared_ptr<const expression>get_order01(lexicon::token_array&ta);
            static std::shared_ptr<const expression>get_primary(lexicon::token_array&ta);
        public:
            using node::node;
            static std::shared_ptr<const expression>get(lexicon::token_array&ta,bool for_initialization=false);
            virtual ~expression()=default;
    };
    class numeric final:public expression{
        private:
            const int value;
        public:
            numeric(int value,int line,int col);
            void check(semantics::analyzer&analy)const noexcept override;
            void to_asm(code::generator&gen)const override;
    };
    class identifier final:public expression{
        public:
            // TODO: fcallの方の問題に対応できたらprivateにする
            const std::string name;
            identifier(const std::string&name,int line,int col);
            void check(semantics::analyzer&analy)const override;
            void to_asm(code::generator&gen)const override;
            void refer(code::generator&gen)const;
    };
    class fcall final:public expression{
        private:
            // TODO: 関数部分が識別子でなくてもよいようにする
            const std::shared_ptr<const identifier>func;
            const std::vector<std::shared_ptr<const expression>>vars;
        public:
            fcall(const std::shared_ptr<const expression>&func,const std::vector<std::shared_ptr<const expression>>&vars,int line,int col);
            void check(semantics::analyzer&analy)const override;
            void to_asm(code::generator&gen)const override;
    };
    class unopr:public expression{
        protected:
            const std::shared_ptr<const expression>arg;
        public:
            unopr(const std::shared_ptr<const expression>&arg,int line,int col);
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
        protected:
            // TODO: 引数が識別子でなくてもよいようにする
            const std::shared_ptr<const identifier>arg;
        public:
            unopr_l(const std::shared_ptr<const expression>&arg,int line,int col);
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
        protected:
            const std::shared_ptr<const expression>larg,rarg;
        public:
            biopr(const std::shared_ptr<const expression>&larg,const std::shared_ptr<const expression>&rarg,int line,int col);
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
        protected:
            // TODO: 右引数が識別子でなくてもよいようにする
            const std::shared_ptr<const identifier>larg;
            const std::shared_ptr<const expression>rarg;
        public:
            biopr_l(const std::shared_ptr<const expression>&larg,const std::shared_ptr<const expression>&rarg,int line,int col);
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
            using node::node;
            virtual ~statement()=default;
            static std::shared_ptr<const statement>get(lexicon::token_array&ta);
    };
    class expression_statement:public statement{
        private:
            std::shared_ptr<const expression>expr;
        public:
            using statement::statement;
            static std::shared_ptr<const expression_statement>get(lexicon::token_array&ta);
            void check(semantics::analyzer&analy)const override;
            void to_asm(code::generator&gen)const override;
    };
    class null_statement final:public statement{
        public:
            using statement::statement;
            static std::shared_ptr<const null_statement>get(lexicon::token_array&ta);
            void check(semantics::analyzer&analy)const override;
            void to_asm(code::generator&gen)const override;
    };
    class compound final:public statement{
        private:
            std::vector<std::shared_ptr<const statement>>stats;
        public:
            using statement::statement;
            static std::shared_ptr<const compound>get(lexicon::token_array&ta);
            void check(semantics::analyzer&analy)const override;
            void to_asm(code::generator&gen)const override;
    };
    class define_var final:public statement{
        private:
            std::vector<std::pair<std::string,std::shared_ptr<const expression>>>vars;
        public:
            using statement::statement;
            static std::shared_ptr<const define_var>get(lexicon::token_array&ta);
            void check(semantics::analyzer&analy)const override;
            void to_asm(code::generator&gen)const override;
    };
    class _if_else_ final:public statement{
        private:
            std::shared_ptr<const expression>cond;
            std::shared_ptr<const statement>stat_if,stat_else;
        public:
            using statement::statement;
            static std::shared_ptr<const _if_else_>get(lexicon::token_array&ta);
            void check(semantics::analyzer&analy)const override;
            void to_asm(code::generator&gen)const override;
    };
    class _while_ final:public statement{
        private:
            std::shared_ptr<const expression>cond;
            std::shared_ptr<const statement>stat;
        public:
            using statement::statement;
            static std::shared_ptr<const _while_>get(lexicon::token_array&ta);
            void check(semantics::analyzer&analy)const override;
            void to_asm(code::generator&gen)const override;
    };
    class _for_ final:public statement{
        private:
            std::shared_ptr<const expression>init,cond,reinit;
            std::shared_ptr<const statement>stat;
        public:
            using statement::statement;
            static std::shared_ptr<const _for_>get(lexicon::token_array&ta);
            void check(semantics::analyzer&analy)const override;
            void to_asm(code::generator&gen)const override;
    };
    class _break_ final:public statement{
        public:
            using statement::statement;
            static std::shared_ptr<const _break_>get(lexicon::token_array&ta);
            void check(semantics::analyzer&analy)const override;
            void to_asm(code::generator&gen)const override;
    };
    class _continue_ final:public statement{
        public:
            using statement::statement;
            static std::shared_ptr<const _continue_>get(lexicon::token_array&ta);
            void check(semantics::analyzer&analy)const override;
            void to_asm(code::generator&gen)const override;
    };
    class _return_ final:public statement{
        private:
            std::shared_ptr<const expression>value;
        public:
            using statement::statement;
            static std::shared_ptr<const _return_>get(lexicon::token_array&ta);
            void check(semantics::analyzer&analy)const override;
            void to_asm(code::generator&gen)const override;
    };
    class define_function final:public node{
        private:
            std::string name;
            std::vector<std::string>args;
            std::shared_ptr<const compound>com;
        public:
            using node::node;
            static std::shared_ptr<const define_function>get(lexicon::token_array&ta);
            void check(semantics::analyzer&analy)const override;
            void to_asm(code::generator&gen)const override;
    };
}
