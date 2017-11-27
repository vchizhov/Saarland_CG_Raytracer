#include <rt/groups/median_kdtree.h>
#include <rt/groups/simplegroup.h>
namespace rt
{
	void MedianKDTree::BTree::splitPrimitivesLeft(const std::vector<Primitive*>& src, float splitVal, unsigned char splitAxis, std::vector<Primitive*>& dst)
	{
		switch (splitAxis)
		{
		case 0: // along x
		{
			for (uint32_t i = 0; i < static_cast<uint32_t>(src.size()); ++i)
			{
				if (src[i]->getBounds().min.x <= splitVal)
					dst.push_back(src[i]);
			}
			break;
		}
		case 1: // along y
		{
			for (uint32_t i = 0; i < static_cast<uint32_t>(src.size()); ++i)
			{
				if (src[i]->getBounds().min.y <= splitVal)
					dst.push_back(src[i]);
			}
			break;
		}
		case 2: // along z
		{
			for (uint32_t i = 0; i < static_cast<uint32_t>(src.size()); ++i)
			{
				if (src[i]->getBounds().min.z <= splitVal)
					dst.push_back(src[i]);
			}
			break;
		}
		}
	}

	void MedianKDTree::BTree::splitPrimitivesRight(const std::vector<Primitive*>& src, float splitVal, unsigned char splitAxis, std::vector<Primitive*>& dst)
	{
		switch (splitAxis)
		{
		case 0: // along x
		{
			for (uint32_t i = 0; i < static_cast<uint32_t>(src.size()); ++i)
			{
				if (src[i]->getBounds().max.x > splitVal)
					dst.push_back(src[i]);
			}
			break;
		}
		case 1: // along y
		{
			for (uint32_t i = 0; i < static_cast<uint32_t>(src.size()); ++i)
			{
				if (src[i]->getBounds().max.y > splitVal)
					dst.push_back(src[i]);
			}
			break;
		}
		case 2: // along z
		{
			for (uint32_t i = 0; i < static_cast<uint32_t>(src.size()); ++i)
			{
				if (src[i]->getBounds().max.z > splitVal)
					dst.push_back(src[i]);
			}
			break;
		}
		}
	}

	BTreeInfo MedianKDTree::BTree::split(const std::vector<Primitive*>& p, unsigned char splitAxis, uint32_t height, uint32_t noImprovementCount,
		uint32_t minPrim, uint32_t maxHeight, uint32_t noImprovementMax)
	{
		// terminate
		if (p.size() <= minPrim || height == maxHeight || noImprovementCount>noImprovementMax)
		{
			if (p.size() == 1)
				prim = p[0];
			else if (p.size() != 0)
			{
				prim = new SimpleGroup();
				SimpleGroup* primG = reinterpret_cast<SimpleGroup*>(prim);
				// You shouldn't really have more than 4 294 967 295 primitives here
				for (uint32_t i = 0; i < static_cast<uint32_t>(p.size()); ++i)
					primG->add(p[i]);
			}
			return BTreeInfo(1, height, height,static_cast<uint32_t>(p.size()), static_cast<uint32_t>(p.size()), p.size() == 0, 1);
		}
		
		// split more:
		left = new BTree();
		left->bbox = bbox;
		right = new BTree();
		right->bbox = bbox;
		float splitVal;
		switch (splitAxis)
		{
		case 0: // x split
		{	
			splitVal = 0.5f*(bbox.min.x + bbox.max.x);
			left->bbox.max.x = splitVal;
			right->bbox.min.x = splitVal;
			break;
		}
		case 1: // y split
		{
			splitVal = 0.5f*(bbox.min.y + bbox.max.y);
			left->bbox.max.y = splitVal;
			right->bbox.min.y = splitVal;
			break;
		}
		case 2: // z split
		{
			splitVal = 0.5f*(bbox.min.z + bbox.max.z);
			left->bbox.max.z = splitVal;
			right->bbox.min.z = splitVal;
			break;
		}
		}
		// I do this on the left first, and then on the right, to save memory
		auto* leftPrimitives = new std::vector<Primitive*>();
		splitPrimitivesLeft(p, splitVal, splitAxis, *leftPrimitives);
		uint32_t leftNoImprovementCount = (p.size() - leftPrimitives->size() == 0) ? noImprovementCount + 1 : 0;
		BTreeInfo leftTreeInfo = left->split(*leftPrimitives, (splitAxis + 1) % 3, height + 1,
			leftNoImprovementCount, minPrim, maxHeight, noImprovementMax);
		delete leftPrimitives;

		auto* rightPrimitives = new std::vector<Primitive*>();
		splitPrimitivesRight(p, splitVal, splitAxis, *rightPrimitives);
		uint32_t rightNoImprovementCount = (p.size() - rightPrimitives->size() == 0) ? noImprovementCount + 1 : 0;
		BTreeInfo rightTreeInfo = right->split(*rightPrimitives, (splitAxis + 1) % 3, height + 1,
			rightNoImprovementCount, minPrim, maxHeight, noImprovementMax);
		delete rightPrimitives;

		return BTreeInfo(leftTreeInfo, rightTreeInfo);
	}

	Intersection MedianKDTree::BTree::intersect(const Ray& r, float previousBestDistance) const
	{
		if (prim != nullptr)
		{
			// this intersection cannot be outside the bbox since we check for that on the previous step
			// of the recursion
			return prim->intersect(r, previousBestDistance);
		}

		if (left == nullptr && right == nullptr)
			return Intersection::failure();

		// check the intersection with the bounding box:
		auto interL = left->bbox.intersect(r);
		auto interR = right->bbox.intersect(r);
		if (interL.first <= interR.first)
		{
			// if the closest intersection is farther than the previousBestDistance, then we have a closer intersection anyways
			if (interL.first >= previousBestDistance) return Intersection::failure();
			// if there's no intersection with the left boudning box, try the right one
			if (interL.first >= interL.second)
			{
				if (interR.first >= interR.second || interR.first >= previousBestDistance) return Intersection::failure();
				return right->intersect(Ray(r.getPoint(interR.first), r.d), min(previousBestDistance, interR.second));
			}
			// no need to offset the beginning since we know this is the closer intersection.
			// that means that the interL.first is actually at the origin of the ray
			Intersection leftInter = left->intersect(r, min(previousBestDistance, interL.second));
			if (leftInter) return leftInter;
			// if there was no intersection to the left, try on the right
			if (interR.first >= interR.second || interR.first >= previousBestDistance) return Intersection::failure();
			return right->intersect(Ray(r.getPoint(interR.first), r.d), min(previousBestDistance, interR.second));
		}
		else //same thing but reversed
		{
			// if the closest intersection is farther than the previousBestDistance, then we have a closer intersection anyways
			if (interR.first >= previousBestDistance) return Intersection::failure();
			// if there's no intersection with the left boudning box, try the right one
			if (interR.first >= interR.second)
			{
				if (interL.first >= interL.second || interL.first >= previousBestDistance) return Intersection::failure();
				return left->intersect(Ray(r.getPoint(interL.first), r.d), min(previousBestDistance, interL.second));
			}
			// no need to offset the beginning since we know this is the closer intersection.
			// that means that the interL.first is actually at the origin of the ray
			Intersection rightInter = right->intersect(r, min(previousBestDistance, interR.second));
			if (rightInter) return rightInter;
			// if there was no intersection to the left, try on the right
			if (interL.first >= interL.second || interL.first >= previousBestDistance) return Intersection::failure();
			return left->intersect(Ray(r.getPoint(interL.first), r.d), min(previousBestDistance, interL.second));
		}
		
	}
}