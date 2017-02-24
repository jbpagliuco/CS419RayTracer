#include <World.h>

#include <fstream>
#include <Log.h>
#include <Util.h>
#include <cctype>
#include <P4Mem.h>
#include <Camera.h>
#include <Light.h>

namespace RE
{
	void LoadWorldElementComponent(WorldElement * pWorldElement, const std::string& line, U32 lineNum, World& world)
	{
		// Parse the line (don't need the whitespace on the ends)
		std::istringstream iss(line);

		std::string componentType;
		iss >> componentType;

		if (componentType == "Transform")
		{
			pWorldElement->transform = CreateTransformFromString(line);
		}
		else if (componentType == "Geometry")
		{
			Geometry * pGeo;

			std::string geoType;
			iss >> geoType;
			if (geoType[0] == '@')
			{
				pGeo = world.assetManager.GetGeometry(geoType.substr(1));
			}
			else
			{
				pGeo = CreateGeometryFromString(line);
				if (!pGeo)
					RE_LOGERROR(WORLD, INIT, "Invalid geometry specification: [Line " << lineNum << "] " << line);
				world.assetManager.AddGeometry(pGeo);
			}

			pWorldElement->pGeometry = pGeo;
		}
		else if (componentType == "Material")
		{
			Material * pMat = CreateMaterialFromString(line);
			if (!pMat)
				RE_LOGERROR(WORLD, INIT, "Invalid material specification: [Line " << lineNum << "] " << line);
			pWorldElement->pMaterial = pMat;
		}
	}

	WorldElement* LoadWorldElement(std::ifstream& in, const std::string& name, U32& lineNum, World& world)
	{
		WorldElement * pWE = new WorldElement(name);

		std::string line;
		while (std::getline(in, line))
		{
			lineNum++;

			std::string trimmed = line;
			RE::RemoveCapWhitespace(trimmed);

			// If we're at an empty line
			if (trimmed == "")
				break;

			// Otherwise, this line contains a component
			LoadWorldElementComponent(pWE, line, lineNum, world);
		}

		return pWE;
	}



	Camera * LoadCamera(std::ifstream& in, U32& lineNum)
	{
		VML::VECTOR3F eyePos, lookat, up;

		std::string line;
		while (std::getline(in, line))
		{
			lineNum++;

			std::string trimmed = line;
			RE::RemoveCapWhitespace(trimmed);

			// If we're at an empty line
			if (trimmed == "")
				break;

			std::stringstream ss(trimmed);
			std::string paramName;
			ss >> paramName;

			if (paramName == "pos")
				ss >> eyePos.x >> eyePos.y >> eyePos.z;
			else if (paramName == "lookat")
				ss >> lookat.x >> lookat.y >> lookat.z;
			else if (paramName == "up")
				ss >> up.x >> up.y >> up.z;
			else if (paramName == "ortho")
			{
				F32 halfWidth;
				ss >> halfWidth;

				void * pAlignedMem = P4::AllocateAlignedMemory(sizeof(OrthographicCamera), 16);
				return new(pAlignedMem)OrthographicCamera(eyePos, lookat, up, halfWidth);
			}
			else if (paramName == "pinhole")
			{
				F32 distanceToViewPlane, pwScale;
				ss >> distanceToViewPlane >> pwScale;

				void * pAlignedMem = P4::AllocateAlignedMemory(sizeof(PinholeCamera), 16);
				return new(pAlignedMem)PinholeCamera(eyePos, lookat, up, distanceToViewPlane, pwScale);
			}
		}

		RE_LOG(WORLD, INIT, "Invalid camera type on line " << lineNum);

		return nullptr;
	}





	void LoadLight(const std::string& line, U32& lineNum, World& world)
	{
		std::string trimmed = line;
		RE::RemoveCapWhitespace(trimmed);

		std::stringstream ss(trimmed);
		std::string lightType;
		ss >> lightType >> lightType;

		if (lightType == "ambient")
		{
			F32 ls;
			Color color;

			ss >> ls >> color.r >> color.g >> color.b;
			color.a = 1.0f;

			world.ambientLight = AmbientLight(ls, color);
		}
		else if (lightType == "parallel")
		{
			F32 ls;
			Color color;
			VML::VECTOR4F direction;

			ss >> ls >>
				color.r >> color.g >> color.b >>
				direction.x >> direction.y >> direction.z;
			color.a = 1.0f;
			direction.w = 0.0f;


			void * pAlignedMem = P4::AllocateAlignedMemory(sizeof(ParallelLight), 16);
			ParallelLight* p = new(pAlignedMem)ParallelLight(ls, color, VML::Vector(direction));
			world.AddLight(p);
		}
		else if (lightType == "point")
		{
			F32 ls;
			Color color;
			VML::VECTOR3F position;
				
			ss >> ls >>
				color.r >> color.g >> color.b >>
				position.x >> position.y >> position.z;

			void * pAlignedMem = P4::AllocateAlignedMemory(sizeof(PointLight), 16);
			PointLight* p = new(pAlignedMem)PointLight(ls, color, VML::Vector(position));
			world.AddLight(p);
		}
		else
			RE_LOG(WORLD, INIT, "Invalid light type on line " << lineNum);
	}


	



	void LoadWorldFromFile(World& world, const std::string& file)
	{
		RE_LOG(WORLD, INIT, "Loading world");
		std::ifstream in(file);

		if (!in.is_open())
		{
			RE_LOG(WORLD, ERROR, "Could not open world file: " << file);
			return;
		}

		std::string line;
		U32 lineNum = 0;
		while (std::getline(in, line))
		{
			lineNum++;

			std::string trimmed = line;
			RE::RemoveCapWhitespace(trimmed);

			// If we're at an empty line
			if (trimmed == "")
				continue;

			// Check first character
			// # - Comment, skip
			// @ - Special world element, ie camera/light
			// Not space - World element
			if (trimmed[0] == '#')
				continue;
			else if (trimmed[0] == '@')
			{
				std::stringstream ss(line);
				std::string type;
				ss >> type;

				if (type == "@Camera")
				{
					// Load camera
					world.SetCamera(LoadCamera(in, lineNum));
				}
				else if (type == "@Light")
				{
					// Load light
					LoadLight(line, lineNum, world);
				}
				else if (type == "@Geometry")
				{
					// Load named geometry
					std::string name, geometryDesc;
					ss >> name;
					geometryDesc = trimmed.substr(std::string("Geometry").length() + 1);
					Geometry * p = CreateGeometryFromString(geometryDesc);
					world.assetManager.AddGeometry(name, p);
				}
			}
			else if (!isspace(trimmed[0]))
			{
				// Load world element
				WorldElement * pWE = LoadWorldElement(in, trimmed, lineNum, world);

				// Add the element to the world
				world.AddElement(pWE);
			}
		}
	}
}