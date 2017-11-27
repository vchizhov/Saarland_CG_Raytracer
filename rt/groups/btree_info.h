#ifndef CG1RAYTRACER_GROUPS_BTREE_INFO_HEADER
#define CG1RAYTRACER_GROUPS_BTREE_INFO_HEADER

#include <vector>
#include <rt/groups/group.h>
#include <rt/bbox.h>

namespace rt {

	struct BTreeInfo
	{
		BTreeInfo(uint32_t nodesCount = 0, uint32_t maxHeight = 0, uint32_t minHeight = 0,
			uint32_t maxPrimCountPerLeaf = 0,
			uint32_t primCountSum = 0,
			uint32_t emptyLeavesCount = 0,
			uint32_t leavesCount = 0)
			: nodesCount(nodesCount), maxHeight(maxHeight), minHeight(minHeight), maxPrimCountPerLeaf(maxPrimCountPerLeaf), primCountSum(primCountSum),
			emptyLeavesCount(emptyLeavesCount), leavesCount(leavesCount) {}

		BTreeInfo(const BTreeInfo& left, const BTreeInfo& right)
			: nodesCount(1 + left.nodesCount + right.nodesCount), maxHeight(max(left.maxHeight, right.maxHeight)), minHeight(min(left.minHeight, right.minHeight)),
			maxPrimCountPerLeaf(max(left.maxPrimCountPerLeaf, right.maxPrimCountPerLeaf)),
			primCountSum(left.primCountSum + right.primCountSum),
			emptyLeavesCount(left.emptyLeavesCount + right.emptyLeavesCount), leavesCount(left.leavesCount + right.leavesCount)
		{

		}

		float averagePrimCountPerLeaf() const
		{
			return float(primCountSum) / float(leavesCount - emptyLeavesCount);
		}
		uint32_t nodesCount;
		uint32_t maxHeight;
		uint32_t minHeight;
		uint32_t maxPrimCountPerLeaf;
		uint32_t primCountSum;
		uint32_t emptyLeavesCount;
		uint32_t leavesCount;

	};
	std::ostream& operator<<(std::ostream& os, const rt::BTreeInfo& i);

}

#endif

