#include "TRpch.h"
#include "Text.h"

namespace Triton
{
	namespace Extension
	{
		IO::Mesh buildVertexArray(reference<FontAsset> font, const std::string& text, float x, float y)
		{
			std::vector<IO::Vertex> result(text.length() * 6);
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

				result[index].Vertice = Vector3(xpos, ypos + h, 1.0f);  // Top left.
				result[index].UV = Vector2(metrics.Start.x / fontWidth, metrics.Start.y / fontHeight);
				result[index].Normal = Vector3(0.0, 1.0, 0.0);
				index++;

				result[index].Vertice = Vector3(xpos + w, ypos, 1.0f);  // Bottom right.
				result[index].UV = Vector2(metrics.End.x / fontWidth, metrics.End.y / fontHeight);
				result[index].Normal = Vector3(0.0, 1.0, 0.0);
				index++;

				result[index].Vertice = Vector3(xpos, ypos, 1.0f);  // Bottom left.
				result[index].UV = Vector2(metrics.Start.x / fontWidth, metrics.End.y / fontHeight);
				result[index].Normal = Vector3(0.0, 1.0, 0.0);
				index++;

				// Second triangle in quad.
				result[index].Vertice = Vector3(xpos, ypos + h, 1.0f);  // Top left.
				result[index].UV = Vector2(metrics.Start.x / fontWidth, metrics.Start.y / fontHeight);
				result[index].Normal = Vector3(0.0, 1.0, 0.0);
				index++;

				result[index].Vertice = Vector3(xpos + w, ypos + h, 1.0f);  // Top right.
				result[index].UV = Vector2(metrics.End.x / fontWidth, metrics.Start.y / fontHeight);
				result[index].Normal = Vector3(0.0, 1.0, 0.0);
				index++;

				result[index].Vertice = Vector3(xpos + w, ypos, 1.0f);  // Bottom right.
				result[index].UV = Vector2(metrics.End.x / fontWidth, metrics.End.y / fontHeight);
				result[index].Normal = Vector3(0.0, 1.0, 0.0);
				index++;

				x += metrics.Advance * scale;
			}

			for (int i = 0; i < indices.size(); i++)
			{
				indices[i] = i;
			}

			return IO::Mesh{1, result, indices};
		}
	}
}