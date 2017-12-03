#include <rt/groups/sah_kdtree.h>
#include <rt/groups/simplegroup.h>

namespace rt
{
	void SAHKDTree::buildSplitEvents(std::vector<Primitive*>& primitives,
		std::vector<SplitEvent>& splitX, std::vector<SplitEvent>& splitY, std::vector<SplitEvent>& splitZ)
	{
		for (uint32_t i = 0; i < primitives.size(); ++i)
		{
			Primitive* currPrim = primitives[i];
			// yz planes:
			float minx = currPrim->getBounds().min.x;
			float maxx = currPrim->getBounds().max.x;
			splitX.push_back(SplitEvent(currPrim->getBounds().max.x, i, 0));
			splitX.push_back(SplitEvent(currPrim->getBounds().min.x, i, 2));

			// xz planes:
			float miny = currPrim->getBounds().min.y;
			float maxy = currPrim->getBounds().max.y;
			splitY.push_back(SplitEvent(currPrim->getBounds().max.y, i, 0));
			splitY.push_back(SplitEvent(currPrim->getBounds().min.y, i, 2));

			// xy planes:
			float minz = currPrim->getBounds().min.z;
			float maxz = currPrim->getBounds().max.z;
			splitZ.push_back(SplitEvent(currPrim->getBounds().max.z, i, 0));
			splitZ.push_back(SplitEvent(currPrim->getBounds().min.z, i, 2));
		}
	}

	float SAHKDTree::findBest(const std::vector<SplitEvent>& e, uint32_t primCount, 
		float mn, float mx, float coef1, float coef2, float areaInv,
		uint32_t& bestIdx)
	{
		uint32_t leftPrimCount = 0;
		uint32_t rightPrimCount = primCount;
		float baseArea = 2.0f*coef1*coef2;
		// natural termination criteria from Ingo Wald's paper
		float bestCost = static_cast<float>(primCount);
		uint32_t bestPrimCountL = primCount;
		uint32_t bestPrimCountR = primCount;
		for (uint32_t i = 0; i < e.size();)
		{
			uint32_t start = 0;
			uint32_t end = 0;
			const SplitEvent& curr = e[i];
			// many primitives may start/end at the same plane - add them all:
			while (i < e.size() && e[i].val == curr.val)
			{
				switch (e[i].type)
				{
				case 0: // end event
					++end;
					break;
				case 1: // planar event

					break;
				case 2: // start event
					++start;
					break;
				}
				++i;
			}
			// all the primitives that end at that plane cannot be in the right set
			rightPrimCount -= end;
			// splits where one of the resulting sets has all the primitives while the other resulting set is nonempty
			// should not be permitted, since they actually worsen the traversal
			//if (!(leftPrimCount == primCount && rightPrimCount!=0) && !(rightPrimCount==primCount && leftPrimCount!=0))
			//{
				// Surface area heuristic
				float probL = (baseArea + 2.0f*(curr.val - mn)*(coef1 + coef2))*areaInv;
				float probR = (baseArea + 2.0f*(mx - curr.val)*(coef1 + coef2))*areaInv;
				//let the cost for hitting a triangle be 1, and let's assume the cost for
				// doing one more step of the traversal is also one
				float cost = 1.0f + (probL*float(leftPrimCount) + probR*float(rightPrimCount));
				// the split should result into a more favorable situation, not in something that's the same or worse
				if (cost < bestCost)
				{
					bestCost = cost;
					bestIdx = i - 1;
				}
			//}
			// all the primitives that start at that plane should be added into the left set for the next iteration
			leftPrimCount += start;
		}
		return bestCost;
	}

	std::pair<uint32_t, unsigned char> SAHKDTree::findBestSplit(const BBox& bbox, uint32_t primCount,
		const std::vector<SplitEvent>& splitX, const std::vector<SplitEvent>& splitY, const std::vector<SplitEvent>& splitZ)
	{

		float bboxAreaInv = 0.5f / (bbox.diagonal().x*(bbox.diagonal().y + bbox.diagonal().z) + bbox.diagonal().y*bbox.diagonal().z);
		// find the best split along X
		uint32_t bestIdxX;
		float bestCostX = findBest(splitX, primCount, bbox.min.x, bbox.max.x, bbox.max.y - bbox.min.y, bbox.max.z - bbox.min.z, bboxAreaInv, bestIdxX);
		// find the best split along Y
		uint32_t bestIdxY;
		float bestCostY = findBest(splitY, primCount, bbox.min.y, bbox.max.y, bbox.max.x - bbox.min.x, bbox.max.z - bbox.min.z, bboxAreaInv, bestIdxY);
		// find the best split along Z
		uint32_t bestIdxZ;
		float bestCostZ = findBest(splitZ, primCount, bbox.min.z, bbox.max.z, bbox.max.x - bbox.min.x, bbox.max.y - bbox.min.y, bboxAreaInv, bestIdxZ);
		
		
		unsigned char axis;
		uint32_t bestIdx;
		// if all 3 of the splits didn't find a good split -> terminate
		if (bestCostX == primCount && bestCostY == primCount && bestCostZ == primCount)
		{
			// 4 indicates that we should terminate at this node
			bestIdx = 0; axis = 4;
		}
		else
		{
			if (bestCostX <= bestCostY && bestCostX <= bestCostZ)
			{
				bestIdx = bestIdxX;
				axis = 0;
			}
			else if (bestCostY <= bestCostX && bestCostY <= bestCostZ)
			{
				bestIdx = bestIdxY;
				axis = 1;
			}
			else if (bestCostZ <= bestCostX && bestCostZ <= bestCostY)
			{
				bestIdx = bestIdxZ;
				axis = 2;
			}
		}
		return std::make_pair(bestIdx, axis);
	}

	void SAHKDTree::classifyPrimitives(const std::vector<SplitEvent>& e, float splitVal, std::vector<unsigned char>& classified, uint32_t n,
		uint32_t& leftCount, uint32_t& rightCount, uint32_t& bothCount)
	{
		classified = std::vector<unsigned char>(n, 0);
		// 0 - in both the left and the right set, 1 - in the left set only, 2 - in the right set only
		leftCount = 0;
		rightCount = 0;
		// classify primitives as left only, right only, or both (each is classified as such initially)
		for (uint32_t i = 0; i < e.size(); ++i)
		{
			// end event that is in the left set -> is only in the left set
			if (e[i].type == 0 && e[i].val <= splitVal)
			{
				classified[e[i].idx] = 1;
				++leftCount;
			} // start event that is in the right set -> is only in the right set
			else if (e[i].type == 2 && e[i].val >= splitVal)
			{
				classified[e[i].idx] = 2;
				++rightCount;
			}
		}

		bothCount = n - (leftCount + rightCount);
	}

	void SAHKDTree::splitPrimitives(std::vector<Primitive*>& p, std::vector<unsigned char>& classified,
		std::vector<Primitive*>& pl, std::vector<Primitive*>& pr, std::vector<uint32_t>& leftMapping, std::vector<uint32_t>& rightMapping)
	{
		// vectors for index mapping from p to pl and pr
		leftMapping = std::vector<uint32_t>(p.size());
		rightMapping = std::vector<uint32_t>(p.size());

		// create the two new primitive subsets along with their mapping
		for (uint32_t i = 0; i < p.size(); ++i)
		{
			switch (classified[i])
			{
			case 0:
				leftMapping[i] = static_cast<uint32_t>(pl.size());
				pl.push_back(p[i]);
				rightMapping[i] = static_cast<uint32_t>(pr.size());
				pr.push_back(p[i]);
				break;
			case 1:
				leftMapping[i] = static_cast<uint32_t>(pl.size());
				pl.push_back(p[i]);
				break;
			case 2:
				rightMapping[i] = static_cast<uint32_t>(pr.size());
				pr.push_back(p[i]);
				break;
			}
		}
	}

	void SAHKDTree::splitPrimaryEvents(std::vector<SplitEvent>& e, const std::vector<unsigned char>& classified, float splitVal,
		const std::vector<uint32_t>& leftMapping, uint32_t leftCount, std::vector<SplitEvent>& leftEvents,
		const std::vector<uint32_t>& rightMapping, uint32_t rightCount, std::vector<SplitEvent>& rightEvents,
		uint32_t bothCount)
	{
		// create the left only and the right only event lists and the left new and right new event lists
		std::vector<SplitEvent> leftOnlyEvents;
		//leftOnlyEvents.reserve(leftCount);
		std::vector<SplitEvent> leftNewEvents;
		//leftNewEvents.reserve(bothCount);
		std::vector<SplitEvent> rightOnlyEvents;
		//rightOnlyEvents.reserve(rightCount);
		std::vector<SplitEvent> rightNewEvents;
		//rightNewEvents.reserve(bothCount);

		// check how each event is classified and put it in its respective vector
		for (uint32_t i = 0; i < e.size(); ++i)
		{
			switch (classified[e[i].idx])
			{
			case 0: //is in both
					// if it's an end, we must add the clipped beginning to the right events
				if (e[i].type == 0)
				{
					// in the right set the only type of new events may be starting ones
					rightNewEvents.push_back(SplitEvent(splitVal, rightMapping[e[i].idx], 2));
					// don't forget to add the unmodified event too
					rightOnlyEvents.push_back(SplitEvent(e[i].val, rightMapping[e[i].idx], 0));
				}
				else if (e[i].type == 2)
				{
					// in the left set the only type of new events may be ending ones
					leftNewEvents.push_back(SplitEvent(splitVal, leftMapping[e[i].idx], 0));
					// don't forget to add the unmodified event too
					leftOnlyEvents.push_back(SplitEvent(e[i].val, leftMapping[e[i].idx], 2));
				}
				break;
			case 1: //if it's in the left set only, we make sure to remap the indices to the ones from the left primitives subset
				leftOnlyEvents.push_back(SplitEvent(e[i].val, leftMapping[e[i].idx], e[i].type));

				break;
			case 2: //if it's in the right set only, we make sure to remap the indices to the ones from the right primitives subset
				rightOnlyEvents.push_back(SplitEvent(e[i].val, rightMapping[e[i].idx], e[i].type));
				break;
			}
		}

		// sort the new events
		std::sort(leftNewEvents.begin(), leftNewEvents.end());
		std::sort(rightNewEvents.begin(), rightNewEvents.end());

		// merge the new events with the old ones
		std::merge(leftOnlyEvents.begin(), leftOnlyEvents.end(), leftNewEvents.begin(), leftNewEvents.end(), std::back_inserter(leftEvents));
		std::merge(rightOnlyEvents.begin(), rightOnlyEvents.end(), rightNewEvents.begin(), rightNewEvents.end(), std::back_inserter(rightEvents));
	}

	void SAHKDTree::splitSecondaryEvents(std::vector<SplitEvent>& e, const std::vector<unsigned char>& classified,
		const std::vector<uint32_t>& leftMapping, uint32_t leftCount, std::vector<SplitEvent>& leftEvents,
		const std::vector<uint32_t>& rightMapping, uint32_t rightCount, std::vector<SplitEvent>& rightEvents,
		uint32_t bothCount)
	{
		// create the left only and the right only event lists and the left new and right new event lists
		std::vector<SplitEvent> leftOnlyEvents;
		//leftOnlyEvents.reserve(leftCount);
		std::vector<SplitEvent> leftNewEvents;
		//leftNewEvents.reserve(bothCount);
		std::vector<SplitEvent> rightOnlyEvents;
		//rightOnlyEvents.reserve(rightCount);
		std::vector<SplitEvent> rightNewEvents;
		//rightNewEvents.reserve(bothCount);

		// check how each event is classified and put it in its respective vector
		for (uint32_t i = 0; i < e.size(); ++i)
		{
			switch (classified[e[i].idx])
			{
			case 0: //is in both
				rightNewEvents.push_back(SplitEvent(e[i].val, rightMapping[e[i].idx], e[i].type));
				leftNewEvents.push_back(SplitEvent(e[i].val, leftMapping[e[i].idx], e[i].type));

				break;
			case 1: //in the left set only
				leftOnlyEvents.push_back(SplitEvent(e[i].val, leftMapping[e[i].idx], e[i].type));

				break;
			case 2: //in the right set only
				rightOnlyEvents.push_back(SplitEvent(e[i].val, rightMapping[e[i].idx], e[i].type));
				break;
			}
		}

		// sort the new events
		std::sort(leftNewEvents.begin(), leftNewEvents.end());
		std::sort(rightNewEvents.begin(), rightNewEvents.end());

		// merge the new events with the old ones
		std::merge(leftOnlyEvents.begin(), leftOnlyEvents.end(), leftNewEvents.begin(), leftNewEvents.end(), std::back_inserter(leftEvents));
		std::merge(rightOnlyEvents.begin(), rightOnlyEvents.end(), rightNewEvents.begin(), rightNewEvents.end(), std::back_inserter(rightEvents));
	}

	void SAHKDTree::buildSplit(std::vector<Primitive*>& p, uint32_t split,
		std::vector<SplitEvent>& pX, std::vector<SplitEvent>& pY, std::vector<SplitEvent>& pZ,
		std::vector<Primitive*>& pl, std::vector<Primitive*>& pr,
		std::vector<SplitEvent>& plX, std::vector<SplitEvent>& prX,
		std::vector<SplitEvent>& plY, std::vector<SplitEvent>& prY,
		std::vector<SplitEvent>& plZ, std::vector<SplitEvent>& prZ)
	{

		uint32_t leftCount;
		uint32_t rightCount;
		uint32_t bothCount;

		float splitVal = pX[split].val;
		std::vector<unsigned char> classified;
		// classify the primitives:
		classifyPrimitives(pX, splitVal, classified, static_cast<uint32_t>(p.size()), leftCount, rightCount, bothCount);

		// split the primitives into left and right:
		// reserve enough space for the primitives in the left set
		pl.reserve(leftCount + bothCount);
		// reserve enougn space for the primitives in the right set
		pr.reserve(rightCount + bothCount);
		std::vector<uint32_t> leftMapping;
		std::vector<uint32_t> rightMapping;
		splitPrimitives(p, classified, pl, pr, leftMapping, rightMapping);
		// split the events
		// the primary events (the events along the axis along which the split happens)
		splitPrimaryEvents(pX, classified, splitVal, leftMapping, leftCount, plX, rightMapping, rightCount, prX, bothCount);
		// the secondary events (the events not along the axis along which the split happens)
		splitSecondaryEvents(pY, classified, leftMapping, leftCount, plY, rightMapping, rightCount, prY, bothCount);
		splitSecondaryEvents(pZ, classified, leftMapping, leftCount, plZ, rightMapping, rightCount, prZ, bothCount);

	}


	BTreeInfo SAHKDTree::BTree::split(std::vector<Primitive*>& p, std::vector<SplitEvent>& eventX, std::vector<SplitEvent>& eventY, std::vector<SplitEvent>& eventZ,
		uint32_t height)
	{
		bool leafNode = false;
		if (p.size() <= 3 || height >= 20)
			leafNode = true;

		std::pair<uint32_t, unsigned char> bestPlane;

		if(!leafNode)
			bestPlane = findBestSplit(bbox, static_cast<uint32_t>(p.size()), eventX, eventY, eventZ); //find best split: O(N)

		// check prim count, height and SAH terimantion criteria
		if(leafNode || bestPlane.second==4)
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
			return BTreeInfo(1, height, height, static_cast<uint32_t>(p.size()), static_cast<uint32_t>(p.size()), p.size() == 0, 1);
		}


		//split the primitives and events into left and right: O(N)
		std::vector<Primitive*> pl, pr;
		std::vector<SplitEvent> plX, prX, plY, prY, plZ, prZ;

		left = new BTree;
		left->bbox = bbox;
		right = new BTree;
		right->bbox = bbox;
		switch (bestPlane.second)
		{
		case 0: //along x
		{
			float splitVal = eventX[bestPlane.first].val;
			left->bbox.max.x = splitVal;
			right->bbox.min.x = splitVal;
			buildSplit(p, bestPlane.first, eventX, eventY, eventZ, pl, pr, plX, prX, plY, prY, plZ, prZ);
			break;
		}
		case 1: //along y
		{
			float splitVal = eventY[bestPlane.first].val;
			left->bbox.max.y = splitVal;
			right->bbox.min.y = splitVal;
			buildSplit(p, bestPlane.first, eventY, eventX, eventZ, pl, pr, plY, prY, plX, prX, plZ, prZ);
			break;
		}
		case 2: //along z
		{
			float splitVal = eventZ[bestPlane.first].val;
			left->bbox.max.z = splitVal;
			right->bbox.min.z = splitVal;
			buildSplit(p, bestPlane.first, eventZ, eventY, eventX, pl, pr, plZ, prZ, plY, prY, plX, prX);
			break;
		}
		}

		// split the left node
		BTreeInfo lc = left->split(pl, plX, plY, plZ, height + 1);
		BTreeInfo rc = right->split(pr, prX, prY, prZ, height + 1);
		return BTreeInfo(lc, rc);
	}

	Intersection SAHKDTree::BTree::intersect(const Ray& r, float previousBestDistance) const
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