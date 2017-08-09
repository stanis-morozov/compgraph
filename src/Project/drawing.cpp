#include "drawing.h"

void calculateTangent(Vertex &v0, Vertex &v1, Vertex &v2)
{
	vec3 edge1 = v1.position - v0.position;
	vec3 edge2 = v2.position - v0.position;

	float deltaU1 = v1.texture.x - v0.texture.x;
	float deltaV1 = v1.texture.y - v0.texture.y;
	float deltaU2 = v2.texture.x - v0.texture.x;
	float deltaV2 = v2.texture.y - v0.texture.y;

	float f = 1.0f / (deltaU1 * deltaV2 - deltaU2 * deltaV1);

	vec3 tangent;

	tangent.x = f * (deltaV2 * edge1.x - deltaV1 * edge2.x);
	tangent.y = f * (deltaV2 * edge1.y - deltaV1 * edge2.y);
	tangent.z = f * (deltaV2 * edge1.z - deltaV1 * edge2.z);

	v0.tangent = tangent.normalized();
	v1.tangent = tangent.normalized();
	v2.tangent = tangent.normalized();
}

void constructBox(GLuint &VBO_DOWN, GLuint &VBO_UP, GLuint &VBO_LEFT, GLuint &VBO_RIGHT, GLuint &VBO_FRONT, GLuint &VBO_BACK)
{
	std::vector <Vertex> vertices;
	int k;

	vertices.clear();
	k = 0;
	for (int i = 0; i < RECTANGLES_NUMBER; i++) {
		for (int j = 0; j < RECTANGLES_NUMBER; j++) {
			vertices.push_back(Vertex(vec3(BLOCKLENGTH * i, 0.0f, BLOCKLENGTH * j), vec2(0.0f, 0.0f), vec3(0.0, 1.0, 0.0)));
			vertices.push_back(Vertex(vec3(BLOCKLENGTH * (i + 1), 0.0f, BLOCKLENGTH * j), vec2(1.0f, 0.0f), vec3(0.0, 1.0, 0.0)));
			vertices.push_back(Vertex(vec3(BLOCKLENGTH * i, 0.0f, BLOCKLENGTH * (j + 1)), vec2(0.0f, 1.0f), vec3(0.0, 1.0, 0.0)));

			vertices.push_back(Vertex(vec3(BLOCKLENGTH * (i + 1), 0.0f, BLOCKLENGTH * j), vec2(1.0f, 0.0f), vec3(0.0, 1.0, 0.0)));
			vertices.push_back(Vertex(vec3(BLOCKLENGTH * i, 0.0f, BLOCKLENGTH * (j + 1)), vec2(0.0f, 1.0f), vec3(0.0, 1.0, 0.0)));
			vertices.push_back(Vertex(vec3(BLOCKLENGTH * (i + 1), 0.0f, BLOCKLENGTH * (j + 1)), vec2(1.0f, 1.0f), vec3(0.0, 1.0, 0.0)));
			calculateTangent(vertices[k], vertices[k + 1], vertices[k + 2]);
			calculateTangent(vertices[k + 3], vertices[k + 4], vertices[k + 5]);
			k += 6;
		}
	}

	glGenBuffers(1, &VBO_DOWN);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_DOWN);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	vertices.clear();
	for (int i = 0; i < RECTANGLES_NUMBER; i++) {
		for (int j = 0; j < RECTANGLES_NUMBER; j++) {
			vertices.push_back(Vertex(vec3(BLOCKLENGTH * i, BLOCKLENGTH * RECTANGLES_NUMBER, BLOCKLENGTH * j), vec2(0.0f, 0.0f), vec3(0.0, -1.0, 0.0)));
			vertices.push_back(Vertex(vec3(BLOCKLENGTH * (i + 1), BLOCKLENGTH * RECTANGLES_NUMBER, BLOCKLENGTH * j), vec2(0.0f, 0.0f), vec3(0.0, -1.0, 0.0)));
			vertices.push_back(Vertex(vec3(BLOCKLENGTH * i, BLOCKLENGTH * RECTANGLES_NUMBER, BLOCKLENGTH * (j + 1)), vec2(0.0f, 0.0f), vec3(0.0, -1.0, 0.0)));

			vertices.push_back(Vertex(vec3(BLOCKLENGTH * (i + 1), BLOCKLENGTH * RECTANGLES_NUMBER, BLOCKLENGTH * j), vec2(0.0f, 0.0f), vec3(0.0, -1.0, 0.0)));
			vertices.push_back(Vertex(vec3(BLOCKLENGTH * i, BLOCKLENGTH * RECTANGLES_NUMBER, BLOCKLENGTH * (j + 1)), vec2(0.0f, 0.0f), vec3(0.0, -1.0, 0.0)));
			vertices.push_back(Vertex(vec3(BLOCKLENGTH * (i + 1), BLOCKLENGTH * RECTANGLES_NUMBER, BLOCKLENGTH * (j + 1)), vec2(0.0f, 0.0f), vec3(0.0, -1.0, 0.0)));
		}
	}

	glGenBuffers(1, &VBO_UP);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_UP);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);


	vertices.clear();
	k = 0;
	for (int i = 0; i < RECTANGLES_NUMBER; i++) {
		for (int j = 0; j < RECTANGLES_NUMBER; j++) {
			vertices.push_back(Vertex(vec3(0.0f, BLOCKLENGTH * i, BLOCKLENGTH * j), vec2(1.0f, 0.0f), vec3(1.0, 0.0, 0.0)));
			vertices.push_back(Vertex(vec3(0.0f, BLOCKLENGTH * (i + 1), BLOCKLENGTH * j), vec2(1.0f, 1.0f), vec3(1.0, 0.0, 0.0)));
			vertices.push_back(Vertex(vec3(0.0f, BLOCKLENGTH * i, BLOCKLENGTH * (j + 1)), vec2(0.0f, 0.0f), vec3(1.0, 0.0, 0.0)));

			vertices.push_back(Vertex(vec3(0.0f, BLOCKLENGTH * (i + 1), BLOCKLENGTH * j), vec2(1.0f, 1.0f), vec3(1.0, 0.0, 0.0)));
			vertices.push_back(Vertex(vec3(0.0f, BLOCKLENGTH * i, BLOCKLENGTH * (j + 1)), vec2(0.0f, 0.0f), vec3(1.0, 0.0, 0.0)));
			vertices.push_back(Vertex(vec3(0.0f, BLOCKLENGTH * (i + 1), BLOCKLENGTH * (j + 1)), vec2(0.0f, 1.0f), vec3(1.0, 0.0, 0.0)));
			calculateTangent(vertices[k], vertices[k + 1], vertices[k + 2]);
			calculateTangent(vertices[k + 3], vertices[k + 4], vertices[k + 5]);
			k += 6;
		}
	}

	glGenBuffers(1, &VBO_LEFT);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_LEFT);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);


	vertices.clear();
	k = 0;
	for (int i = 0; i < RECTANGLES_NUMBER; i++) {
		for (int j = 0; j < RECTANGLES_NUMBER; j++) {
			vertices.push_back(Vertex(vec3(BLOCKLENGTH * RECTANGLES_NUMBER, BLOCKLENGTH * i, BLOCKLENGTH * j), vec2(0.0f, 0.0f), vec3(-1.0, 0.0, 0.0)));
			vertices.push_back(Vertex(vec3(BLOCKLENGTH * RECTANGLES_NUMBER, BLOCKLENGTH * (i + 1), BLOCKLENGTH * j), vec2(0.0f, 1.0f), vec3(-1.0, 0.0, 0.0)));
			vertices.push_back(Vertex(vec3(BLOCKLENGTH * RECTANGLES_NUMBER, BLOCKLENGTH * i, BLOCKLENGTH * (j + 1)), vec2(1.0f, 0.0f), vec3(-1.0, 0.0, 0.0)));

			vertices.push_back(Vertex(vec3(BLOCKLENGTH * RECTANGLES_NUMBER, BLOCKLENGTH * (i + 1), BLOCKLENGTH * j), vec2(0.0f, 1.0f), vec3(-1.0, 0.0, 0.0)));
			vertices.push_back(Vertex(vec3(BLOCKLENGTH * RECTANGLES_NUMBER, BLOCKLENGTH * i, BLOCKLENGTH * (j + 1)), vec2(1.0f, 0.0f), vec3(-1.0, 0.0, 0.0)));
			vertices.push_back(Vertex(vec3(BLOCKLENGTH * RECTANGLES_NUMBER, BLOCKLENGTH * (i + 1), BLOCKLENGTH * (j + 1)), vec2(1.0f, 1.0f), vec3(-1.0, 0.0, 0.0)));
			calculateTangent(vertices[k], vertices[k + 1], vertices[k + 2]);
			calculateTangent(vertices[k + 3], vertices[k + 4], vertices[k + 5]);
			k += 6;
		}
	}

	glGenBuffers(1, &VBO_RIGHT);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_RIGHT);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);


	vertices.clear();
	k = 0;
	for (int i = 0; i < RECTANGLES_NUMBER; i++) {
		for (int j = 0; j < RECTANGLES_NUMBER; j++) {
			vertices.push_back(Vertex(vec3(BLOCKLENGTH * i, BLOCKLENGTH * j, 0.0f), vec2(0.0f, 0.0f), vec3(0.0, 0.0, 1.0)));
			vertices.push_back(Vertex(vec3(BLOCKLENGTH * (i + 1), BLOCKLENGTH * j, 0.0f), vec2(1.0f, 0.0f), vec3(0.0, 0.0, 1.0)));
			vertices.push_back(Vertex(vec3(BLOCKLENGTH * i, BLOCKLENGTH * (j + 1), 0.0f), vec2(0.0f, 1.0f), vec3(0.0, 0.0, 1.0)));

			vertices.push_back(Vertex(vec3(BLOCKLENGTH * (i + 1), BLOCKLENGTH * j, 0.0f), vec2(1.0f, 0.0f), vec3(0.0, 0.0, 1.0)));
			vertices.push_back(Vertex(vec3(BLOCKLENGTH * i, BLOCKLENGTH * (j + 1), 0.0f), vec2(0.0f, 1.0f), vec3(0.0, 0.0, 1.0)));
			vertices.push_back(Vertex(vec3(BLOCKLENGTH * (i + 1), BLOCKLENGTH * (j + 1), 0.0f), vec2(1.0f, 1.0f), vec3(0.0, 0.0, 1.0)));
			calculateTangent(vertices[k], vertices[k + 1], vertices[k + 2]);
			calculateTangent(vertices[k + 3], vertices[k + 4], vertices[k + 5]);
			k += 6;
		}
	}

	glGenBuffers(1, &VBO_FRONT);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_FRONT);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);



	vertices.clear();
	k = 0;
	for (int i = 0; i < RECTANGLES_NUMBER; i++) {
		for (int j = 0; j < RECTANGLES_NUMBER; j++) {
			vertices.push_back(Vertex(vec3(BLOCKLENGTH * i, BLOCKLENGTH * j, BLOCKLENGTH * RECTANGLES_NUMBER), vec2(1.0f, 0.0f), vec3(0.0, 0.0, -1.0)));
			vertices.push_back(Vertex(vec3(BLOCKLENGTH * (i + 1), BLOCKLENGTH * j, BLOCKLENGTH * RECTANGLES_NUMBER), vec2(0.0f, 0.0f), vec3(0.0, 0.0, -1.0)));
			vertices.push_back(Vertex(vec3(BLOCKLENGTH * i, BLOCKLENGTH * (j + 1), BLOCKLENGTH * RECTANGLES_NUMBER), vec2(1.0f, 1.0f), vec3(0.0, 0.0, -1.0)));

			vertices.push_back(Vertex(vec3(BLOCKLENGTH * (i + 1), BLOCKLENGTH * j, BLOCKLENGTH * RECTANGLES_NUMBER), vec2(0.0f, 0.0f), vec3(0.0, 0.0, -1.0)));
			vertices.push_back(Vertex(vec3(BLOCKLENGTH * i, BLOCKLENGTH * (j + 1), BLOCKLENGTH * RECTANGLES_NUMBER), vec2(1.0f, 1.0f), vec3(0.0, 0.0, -1.0)));
			vertices.push_back(Vertex(vec3(BLOCKLENGTH * (i + 1), BLOCKLENGTH * (j + 1), BLOCKLENGTH * RECTANGLES_NUMBER), vec2(0.0f, 1.0f), vec3(0.0, 0.0, -1.0)));
			calculateTangent(vertices[k], vertices[k + 1], vertices[k + 2]);
			calculateTangent(vertices[k + 3], vertices[k + 4], vertices[k + 5]);
			k += 6;
		}
	}

	glGenBuffers(1, &VBO_BACK);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_BACK);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
}

void drawBox(GLuint VBO_UP, GLuint VBO_LEFT, GLuint VBO_RIGHT, GLuint VBO_FRONT, GLuint VBO_BACK)
{	
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_UP);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);
	glDrawArrays(GL_TRIANGLES, 0, RECTANGLES_NUMBER * RECTANGLES_NUMBER * 6);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(2);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_LEFT);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)32);
	glDrawArrays(GL_TRIANGLES, 0, RECTANGLES_NUMBER * RECTANGLES_NUMBER * 6);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_RIGHT);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)32);
	glDrawArrays(GL_TRIANGLES, 0, RECTANGLES_NUMBER * RECTANGLES_NUMBER * 6);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_FRONT);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)32);
	glDrawArrays(GL_TRIANGLES, 0, RECTANGLES_NUMBER * RECTANGLES_NUMBER * 6);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_BACK);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)32);
	glDrawArrays(GL_TRIANGLES, 0, RECTANGLES_NUMBER * RECTANGLES_NUMBER * 6);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
}


void drawBoxDown(GLuint VBO_DOWN)
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_DOWN);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)32);
	glDrawArrays(GL_TRIANGLES, 0, RECTANGLES_NUMBER * RECTANGLES_NUMBER * 6);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
}