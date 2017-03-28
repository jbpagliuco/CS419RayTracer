#pragma once

#include <Util.h>
#include <VML.h>
#include <Geometry.h>
#include <KDTree.h>

#include <vector>

namespace RE
{
	struct Index
	{
		Index() : bHasUV(false) {}
		Index(U64 vertex, U64 normal, U64 uv) : vertex(vertex), normal(normal), uv(uv), bHasUV(true) {}
		Index(U64 vertex, U64 normal, bool bHasUV) : vertex(vertex), normal(normal), uv(uv), bHasUV(bHasUV) {}

		U64 vertex, normal, uv;
		bool bHasUV;
	};

	struct Face
	{
		Face() {}
		Face(const Index& v1, const Index& v2, const Index& v3) { v[0] = v1; v[1] = v2; v[2] = v3; }
		
		Index v[3];
	};

	class Mesh;
	RE_ALIGN_MS(16) class MeshTriangle : public Geometry
	{
	public:
		MeshTriangle() = default;
		MeshTriangle(Mesh * pMesh, const Face& face);

		virtual ~MeshTriangle();

		// Checks to see if a ray intersects this geometry.
		// @param [out] outHitInfo - A list of the intersection points.
		// @param ray - The ray to check against.
		// @param elementTransform - The transform of the WorldElement that uses this geometry.
		// @return Does the ray intersect this geometry?
		virtual bool Intersects(RayIntersectionList& outHitInfo, const Ray& ray)const override;

		// Checks to see if a ray intersects this geometry, but doesn't calculate any normals, etc.
		// @param [out] t - The t value for this ray, if there is an intersection point.
		// @param ray - The ray to check against.
		// @param elementTransform - The transform of the WorldElement that uses this geometry.
		// @return Does the ray intersect this geometry?
		virtual bool Intersects(F32& t, const Ray& ray)const override;

		// Gets the bounding box for this geometry.
		virtual BoundingBox GetBoundingBox()const override;

	private:
		Mesh * pMesh;
		Face face;
	} RE_ALIGN_GCC(16);



	struct MeshTriangleIntersection
	{
		MeshTriangleIntersection() : bHit(false) {}

		bool bHit;
		RayIntersectionList rl;
	};

	class MeshGrid : public RegularGrid<MeshTriangle, MeshTriangleIntersection>
	{
	public:
		MeshGrid();

		virtual ~MeshGrid();

	protected:
		// Adds an object to the grid.
		// @param object - The object to add.
		virtual void AddObject(const MeshTriangle& object) override;

		// Places an object in its cell.
		// @param pObject - A pointer to the object to place.
		virtual void PlaceObject(const MeshTriangle* pObject) override;

		// Trace a ray through the objects and do the necessary calculations.
		// @param [out] out - The output.
		// @param ray - The ray to trace.
		// @param objects - The objects to test intersections.
		// @return { bHit, t }
		virtual ObjectIntersectionOutput CheckObjectIntersections(MeshTriangleIntersection& out, const Ray& ray, const PObjects objects)const override;

		// Trace a ray through the objects and and check for any intersections.
		// @param ray - The ray to trace.
		// @param d - The max distance for an intersection.
		// @param objects - The objects to test intersections.
		// @return Did the ray hit anything?
		virtual bool CheckShallowObjectIntersections(const Ray& ray, F32 d, const PObjects objects)const override;

		// Called after traversing through the grid.
		// @param [out] out - The output.
		// @param ray - The ray to trace.
		virtual void PostTraverse(MeshTriangleIntersection& out, const Ray& ray)const override;

		// Called after traversing through the grid.
		// @param ray - The ray to trace.
		// @return Did the ray hit anything?
		virtual bool PostTraverseShallow(const Ray& ray, F32 d)const override;
	};




	class KDTypeMesh : public KDType
	{
	public:
		KDTypeMesh() = default;
		KDTypeMesh(MeshTriangle t);

		virtual ~KDTypeMesh();

		virtual Geometry* GetGeometry()const override;
		virtual Transform GetTransform()const override;
		virtual BoundingBox GetBoundingBox()const override;

	private:
		MeshTriangle t;
	};






	class Mesh : public Geometry
	{
	public:
		Mesh();

		virtual ~Mesh();

		// Add a vertex position.
		// @param pos - The world coordinates of the vertex.
		void AddVertexPosition(const VML::VECTOR3F& pos);

		// Add a vertex normal.
		// @param normal - The normal of the vertex.
		void AddVertexNormal(const VML::VECTOR3F& normal);

		// Add a uv coordinate.
		// @param uv - The uv texture coordinate.
		void AddVertexUV(const VML::VECTOR2F& uv);

		void ComputeNormals();

		// Add a triangle face.
		// @param face - The triangle face.
		void AddFace(const Face& face);

		// Builds the mesh from the vertex and face data.
		void Build();

		// Checks to see if a ray intersects this geometry.
		// @param [out] outHitInfo - A list of the intersection points.
		// @param ray - The ray to check against.
		// @param elementTransform - The transform of the WorldElement that uses this geometry.
		// @return Does the ray intersect this geometry?
		virtual bool Intersects(RayIntersectionList& outHitInfo, const Ray& ray)const override;

		// Checks to see if a ray intersects this geometry, but doesn't calculate any normals, etc.
		// @param [out] t - The t value for this ray, if there is an intersection point.
		// @param ray - The ray to check against.
		// @param elementTransform - The transform of the WorldElement that uses this geometry.
		// @return Does the ray intersect this geometry?
		virtual bool Intersects(F32& t, const Ray& ray)const override;

		// Gets the bounding box for this geometry.
		virtual BoundingBox GetBoundingBox()const override;

	private:
		std::vector<VML::VECTOR3F> positions;
		std::vector<VML::VECTOR3F> normals;
		std::vector<VML::VECTOR2F> uvs;

		std::vector<KDTypeMesh> triangles;

		KDTree<KDTypeMesh> tree;

		friend class MeshTriangle;
		friend Mesh * LoadMeshFromFile(const std::string& filename);
	};

	Mesh * LoadMeshFromFile(const std::string& filename);
}