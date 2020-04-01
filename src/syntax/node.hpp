#pragma once
#include"../lexicon/token_array.hpp"
#include"../code/generator.hpp"
#include<memory>
#include<string>
#include<utility>
#include<vector>
namespace syntax{
    class node{
        public:
            const int line,col;
            node(int line,int col);
            virtual ~node()=default;
            virtual void to_asm(code::generator&gen)const=0;
    };
    class expression:public node{
        using node::node;
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
            static std::shared_ptr<const expression>get(lexicon::token_array&ta,bool for_initialization=false);
            virtual ~expression()=default;
    };
    class expression_l:public expression{
        using expression::expression;
        public:
            virtual ~expression_l()=default;
            virtual std::string get_address(code::generator&gen)const=0;
            virtual void allocate_on_stack(code::generator&gen)const=0;
            virtual void allocate_on_stack(code::generator&gen,int offset)const=0;
    };
    class numeric final:public expression{
        private:
            const int value;
        public:
            numeric(int value,int line,int col);
            void to_asm(code::generator&gen)const override;
    };
    class identifier final:public expression_l{
        public:
            // TODO: fcallの方の問題に対応できたらprivateにする
            const std::string name;
            identifier(const std::string&name,int line,int col);
            void to_asm(code::generator&gen)const override;
            std::string get_address(code::generator&gen)const override;
            virtual void allocate_on_stack(code::generator&gen)const override;
            virtual void allocate_on_stack(code::generator&gen,int offset)const override;
    };
    class fcall final:public expression{
        private:
            // TODO: 関数部分が識別子でなくてもよいようにする
            const std::shared_ptr<const identifier>func;
            const std::vector<std::shared_ptr<const expression>>vars;
        public:
            fcall(const std::shared_ptr<const expression>&func,const std::vector<std::shared_ptr<const expression>>&vars,int line,int col);
            void to_asm(code::generator&gen)const override;
    };
    class unopr:public expression{
        protected:
            const std::shared_ptr<const expression>arg;
        public:
            unopr(const std::shared_ptr<const expression>&arg,int line,int col);
            virtual ~unopr()=default;
    };
    class uplus final:public unopr{
        using unopr::unopr;
        public:
            void to_asm(code::generator&gen)const override;
    };
    class uminus final:public unopr{
        using unopr::unopr;
        public:
            void to_asm(code::generator&gen)const override;
    };
    class lognot final:public unopr{
        using unopr::unopr;
        public:
            void to_asm(code::generator&gen)const override;
    };
    class unopr_l:public expression{
        protected:
            const std::shared_ptr<const expression_l>arg;
        public:
            unopr_l(const std::shared_ptr<const expression>&arg,int line,int col);
            virtual ~unopr_l()=default;
    };
    class preinc final:public unopr_l{
        using unopr_l::unopr_l;
        public:
            void to_asm(code::generator&gen)const override;
    };
    class predec final:public unopr_l{
        using unopr_l::unopr_l;
        public:
            void to_asm(code::generator&gen)const override;
    };
    class postinc final:public unopr_l{
        using unopr_l::unopr_l;
        public:
            void to_asm(code::generator&gen)const override;
    };
    class postdec final:public unopr_l{
        using unopr_l::unopr_l;
        public:
            void to_asm(code::generator&gen)const override;
    };
    class biopr:public expression{
        protected:
            const std::shared_ptr<const expression>larg,rarg;
        public:
            biopr(const std::shared_ptr<const expression>&larg,const std::shared_ptr<const expression>&rarg,int line,int col);
            virtual ~biopr()=default;
    };
    class bplus final:public biopr{
        using biopr::biopr;
        public:
            void to_asm(code::generator&gen)const override;
    };
    class bminus final:public biopr{
        using biopr::biopr;
        public:
            void to_asm(code::generator&gen)const override;
    };
    class multiply final:public biopr{
        using biopr::biopr;
        public:
            void to_asm(code::generator&gen)const override;
    };
    class divide final:public biopr{
        using biopr::biopr;
        public:
            void to_asm(code::generator&gen)const override;
    };
    class remain final:public biopr{
        using biopr::biopr;
        public:
            void to_asm(code::generator&gen)const override;
    };
    class equal final:public biopr{
        using biopr::biopr;
        public:
            void to_asm(code::generator&gen)const override;
    };
    class nequal final:public biopr{
        using biopr::biopr;
        public:
            void to_asm(code::generator&gen)const override;
    };
    class less final:public biopr{
        using biopr::biopr;
        public:
            void to_asm(code::generator&gen)const override;
    };
    class greater final:public biopr{
        using biopr::biopr;
        public:
            void to_asm(code::generator&gen)const override;
    };
    class leeq final:public biopr{
        using biopr::biopr;
        public:
            void to_asm(code::generator&gen)const override;
    };
    class greq final:public biopr{
        using biopr::biopr;
        public:
            void to_asm(code::generator&gen)const override;
    };
    class logand final:public biopr{
        using biopr::biopr;
        public:
            void to_asm(code::generator&gen)const override;
    };
    class logor final:public biopr{
        using biopr::biopr;
        public:
            void to_asm(code::generator&gen)const override;
    };
    class comma final:public biopr{
        using biopr::biopr;
        public:
            void to_asm(code::generator&gen)const override;
    };
    class biopr_l:public expression{
        protected:
            const std::shared_ptr<const expression_l>larg;
            const std::shared_ptr<const expression>rarg;
        public:
            biopr_l(const std::shared_ptr<const expression>&larg,const std::shared_ptr<const expression>&rarg,int line,int col);
            virtual ~biopr_l()=default;
    };
    class assign final:public biopr_l{
        using biopr_l::biopr_l;
        public:
            void to_asm(code::generator&gen)const override;
    };
    class plasgn final:public biopr_l{
        using biopr_l::biopr_l;
        public:
            void to_asm(code::generator&gen)const override;
    };
    class miasgn final:public biopr_l{
        using biopr_l::biopr_l;
        public:
            void to_asm(code::generator&gen)const override;
    };
    class muasgn final:public biopr_l{
        using biopr_l::biopr_l;
        public:
            void to_asm(code::generator&gen)const override;
    };
    class diasgn final:public biopr_l{
        using biopr_l::biopr_l;
        public:
            void to_asm(code::generator&gen)const override;
    };
    class rmasgn final:public biopr_l{
        using biopr_l::biopr_l;
        public:
            void to_asm(code::generator&gen)const override;
    };
    class statement:public node{
        using node::node;
        public:
            virtual ~statement()=default;
            static std::shared_ptr<const statement>get(lexicon::token_array&ta);
    };
    class expression_statement final:public statement{
        using statement::statement;
        private:
            std::shared_ptr<const expression>expr;
        public:
            expression_statement(lexicon::token_array&ta);
            void to_asm(code::generator&gen)const override;
    };
    class null_statement final:public statement{
        using statement::statement;
        public:
            null_statement(lexicon::token_array&ta);
            void to_asm(code::generator&gen)const override;
    };
    class compound final:public statement{
        using statement::statement;
        private:
            std::vector<std::shared_ptr<const statement>>stats;
        public:
            compound(lexicon::token_array&ta);
            void to_asm(code::generator&gen)const override;
    };
    class var_difinition final:public statement{
        using statement::statement;
        private:
            std::vector<std::pair<std::shared_ptr<const identifier>,std::shared_ptr<const expression>>>vars;
        public:
            var_difinition(lexicon::token_array&ta);
            void to_asm(code::generator&gen)const override;
    };
    class _if_else_ final:public statement{
        using statement::statement;
        private:
            std::shared_ptr<const expression>cond;
            std::shared_ptr<const statement>stat_if,stat_else;
        public:
            _if_else_(lexicon::token_array&ta);
            void to_asm(code::generator&gen)const override;
    };
    class _while_ final:public statement{
        using statement::statement;
        private:
            std::shared_ptr<const expression>cond;
            std::shared_ptr<const statement>stat;
        public:
            _while_(lexicon::token_array&ta);
            void to_asm(code::generator&gen)const override;
    };
    class _for_ final:public statement{
        using statement::statement;
        private:
            std::shared_ptr<const expression>init,cond,reinit;
            std::shared_ptr<const statement>stat;
        public:
            _for_(lexicon::token_array&ta);
            void to_asm(code::generator&gen)const override;
    };
    class _break_ final:public statement{
        using statement::statement;
        public:
            _break_(lexicon::token_array&ta);
            void to_asm(code::generator&gen)const override;
    };
    class _continue_ final:public statement{
        using statement::statement;
        public:
            _continue_(lexicon::token_array&ta);
            void to_asm(code::generator&gen)const override;
    };
    class _return_ final:public statement{
        using statement::statement;
        private:
            std::shared_ptr<const expression>value;
        public:
            _return_(lexicon::token_array&ta);
            void to_asm(code::generator&gen)const override;
    };
    class function_difinition final:public node{
        using node::node;
        private:
            std::string name;
            std::vector<std::shared_ptr<const identifier>>args;
            std::vector<std::shared_ptr<const statement>>stats;
        public:
            function_difinition(lexicon::token_array&ta);
            void to_asm(code::generator&gen)const override;
    };
    class translation_unit final:public node{
        using node::node;
        private:
            std::vector<std::shared_ptr<const function_difinition>>funcs;
        public:
            translation_unit(lexicon::token_array&ta);
            void to_asm(code::generator&gen)const override;
    };
}
