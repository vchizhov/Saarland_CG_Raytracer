#ifndef CG1RAYTRACER_GROUPS_SAH_KDTREE_HEADER
#define CG1RAYTRACER_GROUPS_SAH_KDTREE_HEADER

#include <rt/groups/kdtree.h>
#include <iostream>
#include <rt/groups/btree_info.h>
#include <rt/intersection.h>

namespace rt {

	class SAHKDTree : public KDTree {
		struct SplitEvent
		{
			// coordinate value
			float val;
			// index into a Primitive* array
			uint32_t idx;
			// the type of the event: 0: end, 1: planar, 2: start
			unsigned char type;

			SplitEvent(float val, uint32_t idx, unsigned char type) : val(val), idx(idx), type(type)
			{}

			// used in std::sort and std::merge
			bool operator<(const SplitEvent& b) const
			{
				return val < b.val || (val == b.val && type < b.type);
			}
		};

		// used in rebuildIndex(...)
		// from a set of primitives builds sets of split events along x,y,z
		static void buildSplitEvents(std::vector<Primitive*>& primitives,
			std::vector<SplitEvent>& splitX, std::vector<SplitEvent>& splitY, std::vector<SplitEvent>& splitZ);


		// helper for findBestPlane(...)
		static float findBest(const std::vector<SplitEvent>& e, uint32_t primCount, 
			float mn, float mx, float coef1, float coef2, float areaInv, uint32_t& bestXIdx);

		// used in split(...)
		// given a bbox, the primitives count in that bbox, the sorted split events along x,y,z, returns the best split with the index of the axis
		// O(N)
		static std::pair<uint32_t, unsigned char> findBestSplit(const BBox& bbox, uint32_t primCount,
			const std::vector<SplitEvent>& splitX, const std::vector<SplitEvent>& splitY, const std::vector<SplitEvent>& splitZ);

		// used in buildSplit(...)
		// classifies n primitives relative to a split value as belonging to only the left, only the right, or both sets (by using an event vector)
		// fills up dst with the classification: only left: dst[i] = 1, only right: dst[i] = 2, both: dst[i] = 0
		static void classifyPrimitives(const std::vector<SplitEvent>& e, float splitVal, std::vector<unsigned char>& classified, uint32_t n,
			uint32_t& leftCount, uint32_t& rightCount, uint32_t& bothCount);

		// used in buildSplit(...)
		// splits the primitives from p into pr and pl by using the classified vector, also provides the index mappings from p to pl and pr
		static void splitPrimitives(std::vector<Primitive*>& p, std::vector<unsigned char>& classified,
			std::vector<Primitive*>& pl, std::vector<Primitive*>& pr, std::vector<uint32_t>& leftMapping, std::vector<uint32_t>& rightMapping);

		// used in buildSplit(...)
		// O(sqrt(N)log(sqrt(N))) <= O(N) (as long as the sort inside operates on sqrt(N) elements, which it should in general)
		static void splitPrimaryEvents(std::vector<SplitEvent>& e, const std::vector<unsigned char>& classified, float splitVal,
			const std::vector<uint32_t>& leftMapping, uint32_t leftCount, std::vector<SplitEvent>& leftEvents,
			const std::vector<uint32_t>& rightMapping, uint32_t rightCount, std::vector<SplitEvent>& rightEvents,
			uint32_t bothCount);

		// used in buildSplit(...)
		// O(sqrt(N)log(sqrt(N))) <= O(N) (as long as the sort inside operates on sqrt(N) elements, which it should in general)
		static void splitSecondaryEvents(std::vector<SplitEvent>& e, const std::vector<unsigned char>& classified,
			const std::vector<uint32_t>& leftMapping, uint32_t leftCount, std::vector<SplitEvent>& leftEvents,
			const std::vector<uint32_t>& rightMapping, uint32_t rightCount, std::vector<SplitEvent>& rightEvents,
			uint32_t bothCount);


		// used in split(...)
		// given a splitting plane, a vector of Primitive pointers, and 3 event lists,
		// builds a left and right triangle sets, and a left and right event sets for all 3 lists
		// the first list is the list along which the split occurs
		static void buildSplit(std::vector<Primitive*>& p, uint32_t split,
			std::vector<SplitEvent>& pX, std::vector<SplitEvent>& pY, std::vector<SplitEvent>& pZ,
			std::vector<Primitive*>& pl, std::vector<Primitive*>& pr,
			std::vector<SplitEvent>& plX, std::vector<SplitEvent>& prX,
			std::vector<SplitEvent>& plY, std::vector<SplitEvent>& prY,
			std::vector<SplitEvent>& plZ, std::vector<SplitEvent>& prZ);

		struct BTree
		{
			BTree() : left(nullptr), right(nullptr), prim(nullptr) {}
			BTree* left;
			BTree* right;
			Primitive* prim;
			BBox bbox;
			~BTree()
			{
				delete left;
				delete right;
			}

			

			/*!
			* split function used to split the tree more
			* time complexity: O(p.size()*log2(p.size()))
			* @param[in]  p							vector of primitives that need to distributed over the leaves
			* @param[in]  splitAxis					the axis for the split: 0: X, 1: Y, 2: Z
			* @param[in]  height					at what level is the recursion in the tree
			* @param[in]  noImprovementCount		counts for how many recursive calls there was no decrease in the number of primitives
			* @param[in]  minPrim					for p.size()<=minPrim the recursion stops and we get a leaf
			* @param[in]  maxHeight					for height>=maxHeight the recursion stops and we get a leaf
			* @param[in]  noImprovementMax			for noImprovementCount>noImprovementMax the recursion stops and we get a leaf
			*
			* \return								the maximum number of elements in a leaf
			*/
			BTreeInfo split(std::vector<Primitive*>& p, std::vector<SplitEvent>& eventX, std::vector<SplitEvent>& eventY, std::vector<SplitEvent>& eventZ,
				uint32_t height);


			Intersection intersect(const Ray& r, float previousBestDistance = FLT_MAX) const;


		};
	private:
		std::vector<Primitive*> primitives;
		BTree root;
	public:
		SAHKDTree() {}
		virtual BBox getBounds() const { return root.bbox; }
		virtual Intersection intersect(const Ray& ray, float previousBestDistance = FLT_MAX) const
		{
			// the base is here, so that I can evaluate the left and right in the right order in the recursion
			auto inter = root.bbox.intersect(ray);
			if (inter.first >= inter.second || inter.first >= previousBestDistance) return Intersection::failure();
			return root.intersect(Ray(ray.getPoint(inter.first), ray.d), min(inter.second, previousBestDistance));
		}
		virtual void rebuildIndex()
		{
			// clean up from last time
			delete root.left;
			delete root.right;
			std::cout << "Build started with " << primitives.size() << " primitives.";

			// the events along the x,y,z axes
			std::vector<SplitEvent> eventX;
			std::vector<SplitEvent> eventY;
			std::vector<SplitEvent> eventZ;

			// build split events: O(N)
			buildSplitEvents(primitives, eventX, eventY, eventZ);

			// sort split events: O(NlogN)
			// order splitting planes by ascending value, and then by the type of event (end < planar < start)
			std::sort(eventX.begin(), eventX.end());
			std::sort(eventY.begin(), eventY.end());
			std::sort(eventZ.begin(), eventZ.end());
			// build tree: O(NlogN)
			BTreeInfo info = root.split(primitives, eventX, eventY, eventZ, 0);
			std::cout << "\n\nTree info:\n" << info << "\n\n";
		}
		virtual ~SAHKDTree() {}
		virtual void add(Primitive* p)
		{
			primitives.push_back(p);
			root.bbox.extend(p->getBounds());
		}
		virtual void setMaterial(Material* m) {}
		virtual void setCoordMapper(CoordMapper* cm) {}

	};

}

#endif
