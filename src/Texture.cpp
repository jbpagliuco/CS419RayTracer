#include <Texture.h>

namespace RE
{
	Texture2D::Texture2D()
	{

	}

	Texture2D::~Texture2D()
	{

	}








	ImageTexture2D::ImageTexture2D(const std::string& filename) : image(filename)
	{
		
	}

	ImageTexture2D::~ImageTexture2D()
	{

	}

	Color ImageTexture2D::GetColor(F32 u, F32 v)const
	{
		u = Clamp(u, 0.0f, 1.0f);
		v = Clamp(v, 0.0f, 1.0f);
		return image.GetColor(v * (image.Height() - 1), u * (image.Width() - 1));
	}










	Texture2D * LoadTexture2D(const std::map<std::string, std::string>& params)
	{
		std::string type = params.at("type");

		std::stringstream ss(params.at("params"));

		if (type == "image")
		{
			std::string filename;
			ss >> filename;

			return new ImageTexture2D(filename);
		}

		return nullptr;
	}

	Texture2D * LoadTexture2D(const std::string& params)
	{
		std::stringstream ss(params);

		std::map<std::string, std::string> p;
		ss >> p["type"];

		std::string curr;
		while (!(ss >> curr).eof())
			p["params"] += curr + " ";
		p["params"] += curr;

		return LoadTexture2D(p);
	}
}