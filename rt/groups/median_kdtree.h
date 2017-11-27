#ifndef CG1RAYTRACER_GROUPS_MEDIAN_KDTREE_HEADER
#define CG1RAYTRACER_GROUPS_MEDIAN_KDTREE_HEADER

#include <rt/groups/kdtree.h>
#include <rt/intersection.h>
#include <rt/groups/btree_info.h>
#include <iostream>
namespace rt {

	class MedianKDTree : public KDTree {
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
			BTreeInfo split(const std::vector<Primitive*>& p, unsigned char splitAxis, uint32_t height, uint32_t noImprovementCount,
				uint32_t minPrim = 3, uint32_t maxHeight = 20, uint32_t noImprovementMax = UINT32_MAX);

			/*!
			* split function to build the vector of the primitives that overlap the left set
			* time complexity: O(src.size())
			* @param[in]  src						vector of primitives that need to be put into dst if they overlap the left set
			* @param[in]  splitVal					the coordinate component value of the split
			* @param[in]  splitAxis					the axis for the split: 0: X, 1: Y, 2: Z
			* @param[out] dst						the vector that will contain the primitives that overlap the left set
			*/
			static void splitPrimitivesLeft(const std::vector<Primitive*>& src, float splitVal, unsigned char axis, std::vector<Primitive*>& dst);

			/*!
			* split function to build the vector of the primitives that overlap the right set
			* time complexity: O(src.size()) 
			* @param[in]  src						vector of primitives that need to be put into dst if they overlap the right set
			* @param[in]  splitVal					the coordinate component value of the split
			* @param[in]  splitAxis					the axis for the split: 0: X, 1: Y, 2: Z
			* @param[out] dst						the vector that will contain the primitives that overlap the right set
			*/
			static void splitPrimitivesRight(const std::vector<Primitive*>& src, float splitVal, unsigned char axis, std::vector<Primitive*>& dst);

			Intersection intersect(const Ray& r, float previousBestDistance = FLT_MAX) const;

			
		};
	private:
		BTree root;
		std::vector<Primitive*> primitives;
	public:
		MedianKDTree() {}
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
			//cleanup from before
			delete root.left;
			delete root.right;

			std::cout << "Build started with " << primitives.size() << " primitives.";
			BTreeInfo info = root.split(primitives, 0, 0, 0, 3, 20);
			std::cout << "\n\nTree info:\n" << info << "\n\n";
		}
		virtual ~MedianKDTree() {}
		virtual void add(Primitive* p)
		{
			primitives.push_back(p);
			root.bbox.extend(p->getBounds());
		}
		virtual void setMaterial(Material* m)
		{

		}
		virtual void setCoordMapper(CoordMapper* cm)
		{

		}

	};

}

#endif
