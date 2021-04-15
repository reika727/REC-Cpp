#pragma once
#include "lexer.hpp"
#include "writer.hpp"
#include <map>
#include <memory>
#include <stack>
#include <string>
#include <utility>
#include <vector>
namespace syntax {
    class node {
        // TODO: 関数の未定義・多重定義の検出の実装
    protected:
        inline static std::vector<std::map<std::string, int>> offset;

    public:
        const int line, col;
        node(int line, int col);
        virtual ~node() = default;
        virtual void to_asm(code::writer &wr) const = 0;
        static void enter_scope();
        static int leave_scope();
    };
    class expression : public node {
    private:
        static std::unique_ptr<const expression> get_order15(lexicon::lexer &lx);
        static std::unique_ptr<const expression> get_order14(lexicon::lexer &lx);
        static std::unique_ptr<const expression> get_order13(lexicon::lexer &lx);
        static std::unique_ptr<const expression> get_order12(lexicon::lexer &lx);
        static std::unique_ptr<const expression> get_order11(lexicon::lexer &lx);
        static std::unique_ptr<const expression> get_order10(lexicon::lexer &lx);
        static std::unique_ptr<const expression> get_order09(lexicon::lexer &lx);
        static std::unique_ptr<const expression> get_order08(lexicon::lexer &lx);
        static std::unique_ptr<const expression> get_order07(lexicon::lexer &lx);
        static std::unique_ptr<const expression> get_order06(lexicon::lexer &lx);
        static std::unique_ptr<const expression> get_order05(lexicon::lexer &lx);
        static std::unique_ptr<const expression> get_order04(lexicon::lexer &lx);
        static std::unique_ptr<const expression> get_order03(lexicon::lexer &lx);
        static std::unique_ptr<const expression> get_order02(lexicon::lexer &lx);
        static std::unique_ptr<const expression> get_order01(lexicon::lexer &lx);
        static std::unique_ptr<const expression> get_primary(lexicon::lexer &lx);

    public:
        const class type_info final {
        private:
            enum class _kind {
                INT
            };
            type_info(_kind kind, int bytes)
                : kind(kind), bytes(bytes)
            {
            }

        public:
            const _kind kind;
            const int bytes;
            static type_info get_int()
            {
                return {_kind::INT, 8};
            } // TODO: とりあえず8bytes
        } type;
        expression(int line, int col, type_info type);
        virtual ~expression() = default;
        static std::unique_ptr<const expression> get(lexicon::lexer &lx, bool for_initialization = false);
    };
    class expression_l : public expression {
        using expression::expression;

    public:
        virtual ~expression_l() = default;
        virtual std::string get_address() const = 0;
        virtual void allocate_on_stack() const = 0;
        virtual void allocate_on_stack(int offset) const = 0;
    };
    class identifier final : public expression_l {
        friend std::unique_ptr<const identifier>::deleter_type;

    private:
        ~identifier() = default;

    public:
        // TODO: fcallの方の問題に対応できたらprivateにする
        const std::string name;
        identifier(const std::string &name, int line, int col, type_info type);
        void to_asm(code::writer &wr) const override;
        std::string get_address() const override;
        virtual void allocate_on_stack() const override;
        virtual void allocate_on_stack(int offset) const override;
    };
    class numeric final : public expression {
        friend std::unique_ptr<const numeric>::deleter_type;

    private:
        const int value;
        ~numeric() = default;

    public:
        numeric(int value, int line, int col, expression::type_info type);
        void to_asm(code::writer &wr) const override;
    };
    class fcall final : public expression {
        friend std::unique_ptr<const fcall>::deleter_type;

    private:
        // TODO: 関数部分が識別子でなくてもよいようにする
        std::unique_ptr<const identifier> func;
        std::vector<std::unique_ptr<const expression>> vars;
        ~fcall() = default;

    public:
        fcall(std::unique_ptr<const expression> _func, std::vector<std::unique_ptr<const expression>> &_vars, int line, int col, type_info type);
        void to_asm(code::writer &wr) const override;
    };
    // TODO: determine_typeのために演算子をもっと細かく分類する
    class unopr : public expression {
    protected:
        const std::unique_ptr<const expression> arg;
        virtual type_info determine_type(type_info arg_type);

    public:
        unopr(std::unique_ptr<const expression> arg, int line, int col);
        virtual ~unopr() = default;
    };
    class uplus final : public unopr {
        using unopr::unopr;
        friend std::unique_ptr<const uplus>::deleter_type;

    private:
        ~uplus() = default;

    public:
        void to_asm(code::writer &wr) const override;
    };
    class uminus final : public unopr {
        using unopr::unopr;
        friend std::unique_ptr<const uminus>::deleter_type;

    private:
        ~uminus() = default;

    public:
        void to_asm(code::writer &wr) const override;
    };
    class lognot final : public unopr {
        using unopr::unopr;
        friend std::unique_ptr<const lognot>::deleter_type;

    private:
        ~lognot() = default;

    public:
        void to_asm(code::writer &wr) const override;
    };
    class unopr_l : public expression {
    protected:
        std::unique_ptr<const expression_l> arg;
        virtual type_info determine_type(type_info arg_type);

    public:
        unopr_l(std::unique_ptr<const expression> _arg, int line, int col);
        virtual ~unopr_l() = default;
    };
    class preinc final : public unopr_l {
        using unopr_l::unopr_l;
        friend std::unique_ptr<const preinc>::deleter_type;

    private:
        ~preinc() = default;

    public:
        void to_asm(code::writer &wr) const override;
    };
    class predec final : public unopr_l {
        using unopr_l::unopr_l;
        friend std::unique_ptr<const predec>::deleter_type;

    private:
        ~predec() = default;

    public:
        void to_asm(code::writer &wr) const override;
    };
    class postinc final : public unopr_l {
        using unopr_l::unopr_l;
        friend std::unique_ptr<const postinc>::deleter_type;

    private:
        ~postinc() = default;

    public:
        void to_asm(code::writer &wr) const override;
    };
    class postdec final : public unopr_l {
        using unopr_l::unopr_l;
        friend std::unique_ptr<const postdec>::deleter_type;

    private:
        ~postdec() = default;

    public:
        void to_asm(code::writer &wr) const override;
    };
    class biopr : public expression {
    protected:
        const std::unique_ptr<const expression> larg, rarg;
        virtual type_info determine_type(type_info larg_type, type_info rarg_type);

    public:
        biopr(std::unique_ptr<const expression> larg, std::unique_ptr<const expression> rarg, int line, int col);
        virtual ~biopr() = default;
    };
    class bplus final : public biopr {
        using biopr::biopr;
        friend std::unique_ptr<const bplus>::deleter_type;

    private:
        ~bplus() = default;

    public:
        void to_asm(code::writer &wr) const override;
    };
    class bminus final : public biopr {
        using biopr::biopr;
        friend std::unique_ptr<const bminus>::deleter_type;

    private:
        ~bminus() = default;

    public:
        void to_asm(code::writer &wr) const override;
    };
    class multiply final : public biopr {
        using biopr::biopr;
        friend std::unique_ptr<const multiply>::deleter_type;

    private:
        ~multiply() = default;

    public:
        void to_asm(code::writer &wr) const override;
    };
    class divide final : public biopr {
        using biopr::biopr;
        friend std::unique_ptr<const divide>::deleter_type;

    private:
        ~divide() = default;

    public:
        void to_asm(code::writer &wr) const override;
    };
    class remain final : public biopr {
        using biopr::biopr;
        friend std::unique_ptr<const remain>::deleter_type;

    private:
        ~remain() = default;

    public:
        void to_asm(code::writer &wr) const override;
    };
    class equal final : public biopr {
        using biopr::biopr;
        friend std::unique_ptr<const equal>::deleter_type;

    private:
        ~equal() = default;

    public:
        void to_asm(code::writer &wr) const override;
    };
    class nequal final : public biopr {
        using biopr::biopr;
        friend std::unique_ptr<const nequal>::deleter_type;

    private:
        ~nequal() = default;

    public:
        void to_asm(code::writer &wr) const override;
    };
    class less final : public biopr {
        using biopr::biopr;
        friend std::unique_ptr<const less>::deleter_type;

    private:
        ~less() = default;

    public:
        void to_asm(code::writer &wr) const override;
    };
    class greater final : public biopr {
        using biopr::biopr;
        friend std::unique_ptr<const greater>::deleter_type;

    private:
        ~greater() = default;

    public:
        void to_asm(code::writer &wr) const override;
    };
    class leeq final : public biopr {
        using biopr::biopr;
        friend std::unique_ptr<const leeq>::deleter_type;

    private:
        ~leeq() = default;

    public:
        void to_asm(code::writer &wr) const override;
    };
    class greq final : public biopr {
        using biopr::biopr;
        friend std::unique_ptr<const greq>::deleter_type;

    private:
        ~greq() = default;

    public:
        void to_asm(code::writer &wr) const override;
    };
    class logand final : public biopr {
        using biopr::biopr;
        friend std::unique_ptr<const logand>::deleter_type;

    private:
        ~logand() = default;

    public:
        void to_asm(code::writer &wr) const override;
    };
    class logor final : public biopr {
        using biopr::biopr;
        friend std::unique_ptr<const logor>::deleter_type;

    private:
        ~logor() = default;

    public:
        void to_asm(code::writer &wr) const override;
    };
    class comma final : public biopr {
        using biopr::biopr;
        friend std::unique_ptr<const comma>::deleter_type;

    private:
        ~comma() = default;

    public:
        void to_asm(code::writer &wr) const override;
    };
    class biopr_l : public expression {
    protected:
        std::unique_ptr<const expression_l> larg;
        const std::unique_ptr<const expression> rarg;
        virtual type_info determine_type(type_info larg_type, type_info rarg_type);

    public:
        biopr_l(std::unique_ptr<const expression> _larg, std::unique_ptr<const expression> _rarg, int line, int col);
        virtual ~biopr_l() = default;
    };
    class assign final : public biopr_l {
        using biopr_l::biopr_l;
        friend std::unique_ptr<const assign>::deleter_type;

    private:
        ~assign() = default;

    public:
        void to_asm(code::writer &wr) const override;
    };
    class plasgn final : public biopr_l {
        using biopr_l::biopr_l;
        friend std::unique_ptr<const plasgn>::deleter_type;

    private:
        ~plasgn() = default;

    public:
        void to_asm(code::writer &wr) const override;
    };
    class miasgn final : public biopr_l {
        using biopr_l::biopr_l;
        friend std::unique_ptr<const miasgn>::deleter_type;

    private:
        ~miasgn() = default;

    public:
        void to_asm(code::writer &wr) const override;
    };
    class muasgn final : public biopr_l {
        using biopr_l::biopr_l;
        friend std::unique_ptr<const muasgn>::deleter_type;

    private:
        ~muasgn() = default;

    public:
        void to_asm(code::writer &wr) const override;
    };
    class diasgn final : public biopr_l {
        using biopr_l::biopr_l;
        friend std::unique_ptr<const diasgn>::deleter_type;

    private:
        ~diasgn() = default;

    public:
        void to_asm(code::writer &wr) const override;
    };
    class rmasgn final : public biopr_l {
        using biopr_l::biopr_l;
        friend std::unique_ptr<const rmasgn>::deleter_type;

    private:
        ~rmasgn() = default;

    public:
        void to_asm(code::writer &wr) const override;
    };
    class statement : public node {
        using node::node;

    public:
        virtual ~statement() = default;
        static std::unique_ptr<const statement> get(lexicon::lexer &lx);
    };
    class expression_statement final : public statement {
        using statement::statement;
        friend std::unique_ptr<const expression_statement>::deleter_type;

    private:
        std::unique_ptr<const expression> expr;
        ~expression_statement() = default;

    public:
        expression_statement(lexicon::lexer &lx);
        void to_asm(code::writer &wr) const override;
    };
    class null_statement final : public statement {
        using statement::statement;
        friend std::unique_ptr<const null_statement>::deleter_type;

    private:
        ~null_statement() = default;

    public:
        null_statement(lexicon::lexer &lx);
        void to_asm(code::writer &wr) const override;
    };
    class compound final : public statement {
        using statement::statement;
        friend std::unique_ptr<const compound>::deleter_type;

    private:
        std::vector<std::unique_ptr<const statement>> stats;
        ~compound() = default;

    public:
        compound(lexicon::lexer &lx);
        void to_asm(code::writer &wr) const override;
    };
    class var_difinition final : public statement {
        using statement::statement;
        friend std::unique_ptr<const var_difinition>::deleter_type;

    private:
        std::vector<std::pair<std::unique_ptr<const identifier>, std::unique_ptr<const expression>>> vars;
        ~var_difinition() = default;

    public:
        var_difinition(lexicon::lexer &lx);
        void to_asm(code::writer &wr) const override;
    };
    class _if_else_ final : public statement {
        using statement::statement;
        friend std::unique_ptr<const _if_else_>::deleter_type;

    private:
        std::unique_ptr<const expression> cond;
        std::unique_ptr<const statement> stat_if, stat_else;
        ~_if_else_() = default;

    public:
        _if_else_(lexicon::lexer &lx);
        void to_asm(code::writer &wr) const override;
    };
    class iteration_statement : public statement {
        using statement::statement;
        friend class _break_;
        friend class _continue_;

    private:
        inline static std::stack<std::string> break_labels, continue_labels;

    protected:
        static void enter_break(const std::string &label);
        static void leave_break();
        static void enter_continue(const std::string &label);
        static void leave_continue();
    };
    class _while_ final : public iteration_statement {
        using iteration_statement::iteration_statement;
        friend std::unique_ptr<const _while_>::deleter_type;

    private:
        std::unique_ptr<const expression> cond;
        std::unique_ptr<const statement> stat;
        ~_while_() = default;

    public:
        _while_(lexicon::lexer &lx);
        void to_asm(code::writer &wr) const override;
    };
    class _for_ final : public iteration_statement {
        using iteration_statement::iteration_statement;
        friend std::unique_ptr<const _for_>::deleter_type;

    private:
        std::unique_ptr<const expression> init, cond, reinit;
        std::unique_ptr<const statement> stat;
        ~_for_() = default;

    public:
        _for_(lexicon::lexer &lx);
        void to_asm(code::writer &wr) const override;
    };
    class _break_ final : public statement {
        using statement::statement;
        friend std::unique_ptr<const _break_>::deleter_type;

    private:
        ~_break_() = default;

    public:
        _break_(lexicon::lexer &lx);
        void to_asm(code::writer &wr) const override;
    };
    class _continue_ final : public statement {
        using statement::statement;
        friend std::unique_ptr<const _continue_>::deleter_type;

    private:
        ~_continue_() = default;

    public:
        _continue_(lexicon::lexer &lx);
        void to_asm(code::writer &wr) const override;
    };
    class _return_ final : public statement {
        using statement::statement;
        friend std::unique_ptr<const _return_>::deleter_type;

    private:
        std::unique_ptr<const expression> value;
        ~_return_() = default;

    public:
        _return_(lexicon::lexer &lx);
        void to_asm(code::writer &wr) const override;
    };
    /**
    * function definition = type specifier, identifier, argument list, compound ;
    * argument list = "(", ( "void" | argument declaretions ), ")" ;
    * argument declaretion = type specifier, identifier, { ",", type specifier, identifier } ;
    */
    class function_difinition final : public node {
        using node::node;
        friend std::unique_ptr<const function_difinition>::deleter_type;

    private:
        std::string name;
        std::vector<std::unique_ptr<const identifier>> args;
        std::unique_ptr<const compound> comp;
        void set_argument_list(lexicon::lexer &lx);
        ~function_difinition() = default;

    public:
        function_difinition(lexicon::lexer &lx);
        void to_asm(code::writer &wr) const override;
    };
    /**
    * translation unit = { function definition } ;
    */
    class translation_unit final : public node {
        using node::node;

    private:
        std::vector<std::unique_ptr<const function_difinition>> funcs;

    public:
        translation_unit(lexicon::lexer &lx);
        void to_asm(code::writer &wr) const override;
    };
}
