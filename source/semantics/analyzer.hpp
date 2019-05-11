#include"../syntax/statements.hpp"
#include<set>
namespace semantics{	
    /* 
     * 検出すべき意味論的誤り
     * 右辺値への代入
     * 未定義変数の使用
     * 変数の二重定義
     */    
    class analyzer{
	    std::set<std::string>vars;
	public:
	    analyzer(syntax::compound*rt);
    };
}
