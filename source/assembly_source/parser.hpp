#include"writer.hpp"
#include"../abstract_syntax_tree/nodes.hpp"
#include"registries.hpp"
#include"instructions.hpp"
#include<map>
#include<string>
#include<vector>
namespace assembly_source{
    class parser{
	    writer wr;
	    int var_size;
	    std::map<std::string,int>offset;
	private:
	    void refer(abstract_syntax_tree::node*node);
	    void RDP(abstract_syntax_tree::node*node);
	    void eval(abstract_syntax_tree::statement*st);
	    void eval(const std::vector<abstract_syntax_tree::statement*>&sv);
	public:
	    parser(abstract_syntax_tree::compound*rt,std::string filename);
    };
}
