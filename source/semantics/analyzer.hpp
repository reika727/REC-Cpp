#pragma once
#include<string>
#include<vector>
#include<set>
namespace semantics{	
    /* 
     * 検出すべき意味論的誤り
     * 右辺値への代入
     * 未定義変数の使用
     * 変数の二重定義
     */
    class analyzer{
	    std::vector<std::set<std::string>>vars;
	public:
	    void enter_scope();
	    void leave_scope();
	    void define(const std::string&name);
	    bool is_available(const std::string&name);
	    bool is_definable(const std::string&name);
	    analyzer&operator=(const analyzer&)=delete;
    };
}
