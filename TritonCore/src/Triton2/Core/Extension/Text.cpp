#include "TRpch.h"
#include "Text.h"

#include "Triton2/Limits.h"

namespace Triton
{
	namespace Extension
	{
		IO::Mesh buildVertexArray(reference<FontAsset> font, const std::string& text, float x, float y)
		{
			// Modifier for the final vertex vector length
			unsigned int stride = Triton::Limits::VAO_2D_TEXT;

			std::vector<float> result(text.length() * 6 * stride);
			std::vector<unsigned int> indices(result.size());

			auto charMetrics = font->metrics();
			auto fontWidth = font->width();
			auto fontHeight = font->height();
			auto charHeight = 32;

			// Initialize the index to the vertex array.
			int index = 0;
			float scale = 1;

			std::string::const_iterator c;
			for (c = text.begin(); c != text.end(); c++)
			{
				IO::FontData::CharMetrics metrics = charMetrics[*c];

				float xSize = metrics.End.x - metrics.Start.x;
				float ySize = metrics.End.y - metrics.Start.y;

				float xpos = x + metrics.Offset.x * scale;
				float ypos = y - (ySize - metrics.Offset.y) * scale;

				float w = xSize * scale;
				float h = ySize * scale;


				// First triangle
				result.push_back(xpos);
				result.push_back(ypos + h);
				result.push_back(1.0f);

				result.push_back(metrics.Start.x / fontWidth);
				result.push_back(metrics.Start.y / fontHeight);


				result.push_back(xpos + w);
				result.push_back(ypos);
				result.push_back(1.0f);

				result.push_back(metrics.End.x / fontWidth);
				result.push_back(metrics.End.y / fontHeight);


				result.push_back(xpos);
				result.push_back(ypos);
				result.push_back(1.0f);

				result.push_back(metrics.Start.x / fontWidth);
				result.push_back(metrics.End.y / fontHeight);


				// Second triangle in quad.
				result.push_back(xpos);
				result.push_back(ypos + h);
				result.push_back(1.0f);

				result.push_back(metrics.Start.x / fontWidth);
				result.push_back(metrics.Start.y / fontHeight);


				result.push_back(xpos + w);
				result.push_back(ypos + h);
				result.push_back(1.0f);

				result.push_back(metrics.End.x / fontWidth);
				result.push_back(metrics.Start.y / fontHeight);


				result.push_back(xpos + w);
				result.push_back(ypos);
				result.push_back(1.0f);

				result.push_back(metrics.End.x / fontWidth);
				result.push_back(metrics.End.y / fontHeight);

				x += metrics.Advance * scale;
			}

			for (int i = 0; i < indices.size(); i++)
			{
				indices[i] = i;
			}

			return IO::Mesh{1, stride, result, indices};
		}
	}
}