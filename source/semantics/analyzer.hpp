#pragma once
#include<string>
#include<set>
namespace semantics{	
    /* 
     * 検出すべき意味論的誤り
     * 右辺値への代入
     * 未定義変数の使用
     * 変数の二重定義
     */    
    class analyzer{
	    mutable std::set<std::string>vars;
	public:
	    analyzer();
	    void declare(const std::string&name)const;
	    bool is_declared(const std::string&name)const;
    };
}
