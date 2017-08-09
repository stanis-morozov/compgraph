#include "vertex.h"

Vertex::Vertex(vec3 pos_, vec2 text_)
{
	position = pos_;
	texture = text_;
	normal = vec3();
}

Vertex::Vertex(vec3 pos_, vec2 text_, vec3 norm_)
{
	position = pos_;
	texture = text_;
	normal = norm_;
}

Vertex::Vertex(vec3 pos_, vec2 text_, vec3 norm_, vec3 tang_)
{
	position = pos_;
	texture = text_;
	normal = norm_;
	tangent = tang_;
}

void calculateNormals(unsigned * ind, size_t ind_size, Vertex * v, size_t v_sz)
{
	for (size_t i = 0; i < ind_size; i += 3) {
		size_t i1 = ind[i];
		size_t i2 = ind[i + 1];
		size_t i3 = ind[i + 2];
		vec3 v1 = v[i2].position - v[i1].position;
		vec3 v2 = v[i3].position - v[i1].position;
		vec3 norm = cross(v1, v2).normalized();

		v[i1].normal = v[i1].normal + norm;
		v[i2].normal = v[i2].normal + norm;
		v[i3].normal = v[i3].normal + norm;
	}

	for (size_t i = 0; i < v_sz; i++) {
		v[i].normal = v[i].normal.normalized();
	}
}
