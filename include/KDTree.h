#pragma once

#include <BoundingVolume.h>
#include <memory>
#include <Geometry.h>

namespace RE
{
	// Base class for what to store in a KDNode
	class KDType
	{
	public:
		KDType() = default;

		virtual ~KDType() = default;

		virtual Geometry* GetGeometry()const = 0;
		virtual Transform GetTransform()const = 0;
		virtual BoundingBox GetBoundingBox()const = 0;
		virtual BoundingBox GetTransformedBBox()const;
	};

	// A node of a KDTree.
	template <typename T>
	class KDNode
	{
	public:
		KDNode() : left(nullptr), right(nullptr) { }

		void Build(const std::vector<T>& elements, I32 depth);

		bool Traverse(RayIntersectionList& outHitInfo, const Ray& ray, T& hit)const;
		bool TraverseShallow(const Ray& ray, F32 maxDist)const;

	private:
		F32 FindMidpoint(const std::vector<T>& elements, U8 axis)const;

		void Split(std::vector<T>& left, std::vector<T>& right,	const std::vector<T>& elements, F32 midpoint, U8 axis)const;

	public:
		BoundingBox bb;
		std::unique_ptr<KDNode> left;
		std::unique_ptr<KDNode> right;
		std::vector<T> elements;
	};

	template <typename T>
	using KDNodeP = std::unique_ptr<KDNode<T>>;

	
	// A KDTree.
	template <typename T>
	class KDTree
	{
	public:
		KDTree();
		KDTree(const std::vector<T>& elements);

		bool Traverse(RayIntersectionList& outHitInfo, const Ray& ray, T& hit)const;
		bool TraverseShallow(const Ray& ray, F32 maxDist)const;

		BoundingBox GetBoundingBox()const;

	private:
		KDNode<T> root;
		std::vector<T> unbounded;
	};














	/////////////////////////////////////////////////////
	// Implementation

	template <typename T>
	void KDNode<T>::Build(const std::vector<T>& elements, I32 depth)
	{
		if (elements.size() == 0)
			return;

		this->bb = elements[0].GetBoundingBox();

		if (elements.size() == 1)
		{
			this->elements = elements;
			return;
		}

		for (auto it = elements.begin() + 1; it != elements.end(); it++)
		{
			T element = *it;
			this->bb.Extend(element.GetBoundingBox());
		}

		U8 axis = this->bb.LongestAxis();
		F32 median = FindMidpoint(elements, axis);

		std::vector<T> left, right;
		Split(left, right, elements, median, axis);

		if (left.size() == 0 && right.size() > 0)
			left = right;
		if (right.size() == 0 && left.size() > 0)
			right = left;

		// If 50% of the geometry matches, stop subdiving
		U64 matches = 0;
		for (auto l = left.begin(); l != left.end(); l++)
		{
			if (l->GetBoundingBox().max.v[axis] >= median)
				matches++;
		}
		for (auto r = right.begin(); r != right.end(); r++)
		{
			if (r->GetBoundingBox().min.v[axis] <= median)
				matches++;
		}

		if ((F32)matches / left.size() < 0.5f && (F32)matches / right.size() < 0.5f)
		{
			this->left = KDNodeP<T>(new KDNode);
			this->left->Build(left, depth + 1);

			this->right = KDNodeP<T>(new KDNode);
			this->right->Build(right, depth + 1);
		}
		else
		{
			this->elements = elements;
		}
	}

	template <typename T>
	F32 KDNode<T>::FindMidpoint(const std::vector<T>& elements, U8 axis)const
	{
		F32 sum = 0.0f;
		for (auto it = elements.begin(); it != elements.end(); it++)
		{
			BoundingBox bb = it->GetBoundingBox();
			sum += bb.GetCenter().v[axis];
		}

		return (sum / elements.size());
	}

	template <typename T>
	void KDNode<T>::Split(std::vector<T>& left, std::vector<T>& right, const std::vector<T>& elements, F32 midpoint, U8 axis)const
	{
		for (auto it = elements.begin(); it != elements.end(); it++)
		{
			BoundingBox bb = it->GetBoundingBox();

			if (bb.GetCenter().v[axis] <= midpoint)
				left.push_back(*it);
			else
				right.push_back(*it);
		}
	}

	template <typename T>
	bool KDNode<T>::Traverse(RayIntersectionList& outHitInfo, const Ray& ray, T& hit)const
	{
		VML::Vector wo = ray.GetDirection().negate();

		if (bb.Intersects(outHitInfo, ray))
		{
			if ((left  && left->Traverse(outHitInfo, ray, hit)) ||
				(right && right->Traverse(outHitInfo, ray, hit)))
				return true;

			F32 tMin = F32_MAX;
			bool bHit = false;
			for (auto it = elements.begin(); it != elements.end(); it++)
			{
				T e = *it;
				Geometry * g = e.GetGeometry();

				RayIntersectionList out;
				if (g->Intersects(out, ray) && out.closestIntersection.t < tMin)
				{
					bHit = true;
					outHitInfo = out;
					tMin = out.closestIntersection.t;
					hit = e;

					// Check if we have to reverse the normal
					/*if (outHitInfo.closestIntersection.normal.v3Dot(wo) < 0.0f)
						outHitInfo.closestIntersection.normal.negate();*/
				}
			}
			return bHit;
		}
		return false;
	}

	template <typename T>
	bool KDNode<T>::TraverseShallow(const Ray& ray, F32 maxDist)const
	{
		RayIntersectionList outHitInfo;
		if (bb.Intersects(outHitInfo, ray))
		{
			if ((left  && left->TraverseShallow(ray, maxDist)) ||
				(right && right->TraverseShallow(ray, maxDist)))
				return true;

			for (auto it = elements.begin(); it != elements.end(); it++)
			{
				T e = *it;
				Geometry * g = e.GetGeometry();
				
				F32 t;
				bool bHit = g->Intersects(t, ray);
				if (bHit && t < maxDist)
					return true;
			}
		}
		return false;
	}




	template <typename T>
	KDTree<T>::KDTree()
	{

	}

	template <typename T>
	KDTree<T>::KDTree(const std::vector<T>& elements)
	{
		std::vector<T> bounded;
		for (auto it = elements.begin(); it != elements.end(); it++)
		{
			T e = *it;
			if (e.GetGeometry()->HasBounds())
				bounded.push_back(e);
			else
				unbounded.push_back(e);
		}
		root.Build(bounded, 0);
	}

	template <typename T>
	bool KDTree<T>::Traverse(RayIntersectionList& outHitInfo, const Ray& ray, T& hit)const
	{
		bool bHit = root.Traverse(outHitInfo, ray, hit);
		if (!bHit)
			outHitInfo.closestIntersection.t = F32_MAX;

		for (auto it = unbounded.begin(); it != unbounded.end(); it++)
		{
			T e = *it;
			RayIntersectionList out;
			if (e.GetGeometry()->Intersects(out, ray) && out.closestIntersection.t < outHitInfo.closestIntersection.t)
			{
				outHitInfo = out;
				bHit = true;
				hit = e;
			}
		}

		return bHit;
	}

	template <typename T>
	bool KDTree<T>::TraverseShallow(const Ray& ray, F32 maxDist)const
	{
		if (root.TraverseShallow(ray, maxDist))
			return true;

		for (auto it = unbounded.begin(); it != unbounded.end(); it++)
		{
			T e = *it;
			Geometry* g = e.GetGeometry();

			F32 t;
			bool bHit = g->Intersects(t, ray);
			if (bHit && t < maxDist)
				return true;
		}
		return false;
	}

	template <typename T>
	BoundingBox KDTree<T>::GetBoundingBox()const
	{
		return root.bb;
	}
}