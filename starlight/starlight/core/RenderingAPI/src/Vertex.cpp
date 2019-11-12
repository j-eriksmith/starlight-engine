#include "Vertex.h"

void Vertex::print(std::ostream& out) const
{
	out << "Position: " << Position.x << ", " << Position.y << ", " << Position.z << std::endl
		<< "Normal: " << Normal.x << ", " << Normal.y << ", " << Normal.z << std::endl
		<< "TexCoords: " << TexCoords.x << ", " << TexCoords.y << std::endl;
}

std::ostream& operator << (std::ostream& out, const Vertex& v)
{
	v.print(out);
	return out;
}