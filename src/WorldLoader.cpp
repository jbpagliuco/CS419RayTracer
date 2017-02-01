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
	void LoadWorldElementComponent(WorldElement * pWorldElement, const std::string& line, U32 lineNum)
	{
		// Parse the line (don't need the whitespace on the ends)
		std::istringstream iss(line);

		std::string componentType;
		iss >> componentType;

		if (componentType == "Geometry")
		{
			Geometry * pGeo = CreateGeometryFromString(line);
			if (!pGeo)
				RE_LOGERROR(WORLD, INIT, "Invalid geometry specification: [Line " << lineNum << "] " << line);
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

	WorldElement* LoadWorldElement(std::ifstream& in, const std::string& name, U32& lineNum)
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
			LoadWorldElementComponent(pWE, line, lineNum);
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





	Light * LoadLight(std::ifstream& in, U32& lineNum)
	{
		Color diffuse(0.0f, 0.0f, 0.0f, 1.0f);
		Color specular(0.0f, 0.0f, 0.0f, 1.0f);

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

			if (paramName == "diffuse")
				ss >> diffuse.r >> diffuse.g >> diffuse.b;
			else if (paramName == "specular")
				ss >> specular.r >> specular.g >> specular.b;
			else if (paramName == "parallel")
			{
				VML::VECTOR4F direction;
				ss >> direction.x >> direction.y >> direction.z;
				direction.w = 0.0f;

				void * pAlignedMem = P4::AllocateAlignedMemory(sizeof(ParallelLight), 16);
				return new(pAlignedMem)ParallelLight(diffuse, specular, VML::Vector(direction));
			}
			else if (paramName == "point")
			{
				VML::VECTOR3F position;
				F32 range;
				ss >> position.x >> position.y >> position.z >> range;

				void * pAlignedMem = P4::AllocateAlignedMemory(sizeof(PointLight), 16);
				return new(pAlignedMem)PointLight(diffuse, specular, VML::Vector(position), range);
			}
		}

		RE_LOG(WORLD, INIT, "Invalid light type on line " << lineNum);

		return nullptr;
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
					world.AddLight(LoadLight(in, lineNum));
				}
			}
			else if (!isspace(trimmed[0]))
			{
				// Load world element
				WorldElement * pWE = LoadWorldElement(in, trimmed, lineNum);
				world.AddElement(pWE);
			}
		}
	}
}