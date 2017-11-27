#include <rt/groups/btree_info.h>



namespace rt
{
	std::ostream& operator<<(std::ostream& os, const rt::BTreeInfo& i)
	{
		os << "Max. Height: " << i.maxHeight << "\n"
			<< "Min. Height: " << i.minHeight << "\n"
			<< "Nodes Count: " << i.nodesCount << "\n"
			<< "Leaves Count: " << i.leavesCount << "\n"
			<< "Empty Leaves: " << i.emptyLeavesCount << "\n"
			<< "Max. Prim Leaf: " << i.maxPrimCountPerLeaf << "\n"
			<< "Prim Count Sum: " << i.primCountSum << "\n"
			<< "Avg. Prim. Leaf: " << i.averagePrimCountPerLeaf() << "\n";

		return os;
	}
}