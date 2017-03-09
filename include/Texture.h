#pragma once

#include <Util.h>
#include <ImageCV.h>
#include <map>

namespace RE
{
	class Texture2D
	{
	public:
		Texture2D();

		virtual ~Texture2D();

		virtual Color GetColor(F32 u, F32 v)const = 0;
	};

	class ImageTexture2D : public Texture2D
	{
	public:
		ImageTexture2D(const std::string& filename);

		virtual ~ImageTexture2D();

		virtual Color GetColor(F32 u, F32 v)const override;

	protected:
		ImageCV image;
	};



	Texture2D * LoadTexture2D(const std::map<std::string, std::string>& params);
	Texture2D * LoadTexture2D(const std::string& params);
}