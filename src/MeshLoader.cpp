#include <Mesh.h>

#include <sstream>
#include <P4Mem.h>
#include <Log.h>

namespace RE
{
	void LoadOBJ(Mesh ** ppMesh, FileReader& reader)
	{
		Mesh * mesh = *ppMesh;
		reader.AddCommentString("#");

		std::string line;
		while (reader.ReadLine(line))
		{
			if (line[0] == 'v')
			{
				if (line[1] == ' ')
				{
					// Position
					VML::VECTOR3F pos;
					std::stringstream ss(line.substr(2));
					ss >> pos.x >> pos.y >> pos.z;

					mesh->AddVertexPosition(pos);
				}
				else if (line[1] == 'n')
				{
					// Normal
					VML::VECTOR3F normal;
					std::stringstream ss(line.substr(3));
					ss >> normal.x >> normal.y >> normal.z;

					mesh->AddVertexNormal(normal);
				}
				else if (line[1] == 't')
				{
					// Texture coordinate
					VML::VECTOR2F uv;
					std::stringstream ss(line.substr(2));
					ss >> uv.x >> uv.y;

					mesh->AddVertexUV(uv);
				}
			}
			else if (line[0] == 'f')
			{
				std::stringstream ss(line.substr(2));

				if (line.find("//") != line.npos)
				{
					std::string v1, v2, v3;
					ss >> v1 >> v2 >> v3;
					std::vector<std::string> v1s = SplitString(v1, "/");
					Index i1(std::stoull(v1s[0]) - 1, std::stoull(v1s[2]) - 1, false);

					std::vector<std::string> v2s = SplitString(v2, "/");
					Index i2(std::stoull(v2s[0]) - 1, std::stoull(v2s[2]) - 1, false);

					std::vector<std::string> v3s = SplitString(v3, "/");
					Index i3(std::stoull(v3s[0]) - 1, std::stoull(v3s[2]) - 1, false);

					mesh->AddFace(Face(i1, i2, i3));
				}
				else if (line.find("/") != line.npos)
				{
					std::string v1, v2, v3;
					ss >> v1 >> v2 >> v3;
					std::vector<std::string> v1s = SplitString(v1, "/");
					Index i1(std::stoull(v1s[0]) - 1, std::stoull(v1s[2]) - 1, std::stoull(v1s[1]) - 1);

					std::vector<std::string> v2s = SplitString(v2, "/");
					Index i2(std::stoull(v2s[0]) - 1, std::stoull(v2s[2]) - 1, std::stoull(v2s[1]) - 1);

					std::vector<std::string> v3s = SplitString(v3, "/");
					Index i3(std::stoull(v3s[0]) - 1, std::stoull(v3s[2]) - 1, std::stoull(v3s[1]) - 1);

					mesh->AddFace(Face(i1, i2, i3));
				}
			}
		}
	}


	Mesh * LoadMeshFromFile(const std::string& filename)
	{
		RE_LOG_SAMELINE(MESH, INIT, "Loading " << filename);
		std::string extension = filename.substr(filename.find_last_of("."));
		FileReader reader(filename);

		auto start = GetTime();

		void *pAlignedMem = P4::AllocateAlignedMemory(sizeof(Mesh), 16);
		Mesh * mesh = new(pAlignedMem)Mesh();

		if (extension == ".obj")
			LoadOBJ(&mesh, reader);

		mesh->Build();

		auto end = GetTime();
		BoundingBox bb = mesh->GetBoundingBox();
		RE_LOG_SAMELINE(MESH, INIT, "Loading " << filename <<
			"\n\t\t- Took " << (DiffTime(start, end) / 1000) << "ms" <<
			"\n\t\t- Number of triangles: " << mesh->triangles.size() <<
			"\n\t\t- Dimensions: " << (bb.max.x - bb.min.x) << " x " << (bb.max.y - bb.min.y) << " x " << (bb.max.z - bb.min.z));
		RE_LOG_NEWLINE();

		return mesh;
	}
}