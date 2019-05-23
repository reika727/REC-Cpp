#pragma once
#include<string>
#include<vector>
#include<set>
#include<map>
#include<utility>
namespace semantics{	
    /* 
     * 検出すべき意味論的誤り
     * 右辺値への代入
     * 未定義変数/関数の使用
     * 変数/関数の二重定義
     */
    class analyzer{
	    std::set<std::string>funcs;
	    std::map<std::string,int>signature;
	    std::vector<std::set<std::string>>vars;
	public:
	    void enter_scope();
	    void leave_scope();
	    void define_func(const std::string&name,int var_num);
	    void define_var(const std::string&name);
	    bool is_available_func(const std::string&name,int var_num);
	    bool is_available_var(const std::string&name);
	    bool is_definable_func(const std::string&nane);
	    bool is_definable_var(const std::string&name);
	    analyzer&operator=(const analyzer&)=delete;
    };
}
