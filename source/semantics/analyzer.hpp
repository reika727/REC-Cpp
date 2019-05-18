#pragma once
#include<string>
#include<set>
#include<vector>
#include<stack>
namespace semantics{	
    /* 
     * 検出すべき意味論的誤り
     * 右辺値への代入
     * 未定義変数の使用
     * 変数の二重定義
     */
    class analyzer{
	    std::stack<std::vector<std::set<std::string>>>vars;
	public:
	    analyzer();
	    void enter_func();
	    void leave_func();
	    void enter_scope();
	    void leave_scope();
	    void declare(const std::string&name);
	    bool is_available(const std::string&name);
	    bool is_declarable(const std::string&name);
    };
}
