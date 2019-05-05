#include"writer.hpp"
#include"../abstract_syntax_tree/nodes.hpp"
#include<map>
#include<string>
#include<vector>
namespace assembly_source{
    class parser{
	    writer wr;
	    int var_size;
	    std::map<std::string,int>offset;
	private:
	    void enumerate_var(abstract_syntax_tree::node*const node);
	    void refer_var(abstract_syntax_tree::node*const node);
	    void RDP(abstract_syntax_tree::node*const node);
	    void eval(abstract_syntax_tree::statement*const st);
	    void eval(abstract_syntax_tree::single*const sg);
	    void eval(const std::vector<abstract_syntax_tree::statement*>&sv);
	public:
	    parser(abstract_syntax_tree::compound*const rt,std::string filename);
    };
}
