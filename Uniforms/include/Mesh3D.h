#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <vector>
struct Vertex3D {
	float x;
	float y;
	float z;

	Vertex3D(float a, float b, float c) : x(a), y(b), z(c) {}
};

class Mesh3D {
private:
	uint32_t m_vao;
	uint32_t m_vertexCount;
	uint32_t m_faceCount;

public:
	Mesh3D() = delete;

	/**
	 * @brief Construcst a Mesh3D using existing vectors of vertices and faces.
	*/
	Mesh3D(const std::vector<Vertex3D>& vertices, const std::vector<uint32_t>& faces);

	/**
	 * @brief Constructs a 1x1 square centered at the origin in world space.
	*/
	static Mesh3D square();
	/**
	 * @brief Constructs a 1x1x1 cube centered at the origin in world space.
	*/
	static Mesh3D cube();
	/**
	 * @brief Constructs the upper-left half of the 1x1 square centered at the origin.
	*/
	static Mesh3D triangle();

	/**
	 * @brief Renders the mesh to the given context.
	 * @param model the local->world model transformation matrix.
	 * @param view the world->view camera matrix.
	 * @param proj the view->clip projection matrix.
	*/
	void render();
	
};
