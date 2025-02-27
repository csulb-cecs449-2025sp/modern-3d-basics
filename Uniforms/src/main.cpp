/**
This application displays a mesh in wireframe using "Modern" OpenGL 3.0+.
The Mesh3D class now initializes a "vertex array" on the GPU to store the vertices
	and faces of the mesh. To render, the Mesh3D object simply triggers the GPU to draw
	the stored mesh data.
We now use explicit vertex and fragment shaders instead of the default OpenGL implementations.
	See "no_transform.vert" for a vertex shader that sets a vertex's clip-space coordinates
		equal to its local-space coordinates.
	See "uniform_color.frag" for a fragment shader that sets a pixel to a uniform parameter.
*/


#include <glad/glad.h>
#include <iostream>
#include <memory>
#include "ShaderProgram.h"
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Window.hpp>

struct Mesh {
	uint32_t vao;
	uint32_t faces;
};

struct Vertex3D {
	float x;
	float y;
	float z;
};

ShaderProgram perspectiveUniformColorShader() {
	ShaderProgram shader;
	try {
		shader.load("shaders/no_transform.vert", "shaders/uniform_color.frag");
		shader.activate();
	}
	catch (std::runtime_error& e) {
		std::cout << "ERROR: " << e.what() << std::endl;
		exit(1);
	}
	return shader;
}

Mesh constructMesh(const std::vector<Vertex3D> vertices, const std::vector<uint32_t> faces) {
	Mesh m{};
	m.faces = faces.size();

	// Generate a vertex array object on the GPU.
	glGenVertexArrays(1, &m.vao);
	// "Bind" the newly-generated vao, which makes future functions operate on that specific object.
	glBindVertexArray(m.vao);

	// Generate a vertex buffer object on the GPU.
	uint32_t vbo;
	glGenBuffers(1, &vbo);

	// "Bind" the newly-generated vbo, which makes future functions operate on that specific object.
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// This vbo is now associated with m_vao.
	// Copy the contents of the vertices list to the buffer that lives on the GPU.
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex3D), &vertices[0], GL_STATIC_DRAW);
	// Inform OpenGL how to interpret the buffer: each vertex is 3 contiguous floats (4 bytes each)
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	// Generate a second buffer, to store the indices of each triangle in the mesh.
	uint32_t ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(uint32_t), &faces[0], GL_STATIC_DRAW);

	// Unbind the vertex array, so no one else can accidentally mess with it.
	glBindVertexArray(0);

	return m;
}

void drawMesh(Mesh m) {
	glBindVertexArray(m.vao);
	// Draw the vertex array, using is "element buffer" to identify the faces.

	// Our shader "no_transform.vert" will be executed on the GPU, once for each vertex
	// in the vertex array. The output of that shader is used as the vertex's clip-space
	// coordinates.

	// Our shader "all_green.frag" will be executed next on the GPU, once for each pixel
	// (fragment) along the lines formed by each triangle in screen space. The output of
	// that shader is the color to assign that pixel.
	glDrawElements(GL_TRIANGLES, m.faces, GL_UNSIGNED_INT, nullptr);
	// Deactivate the mesh's vertex array.
	glBindVertexArray(0);
}

// Constructs a VAO of a single cube.
Mesh cube() {
	std::vector<Vertex3D> vertices = {
		/*BUR*/{ 0.5, 0.5, -0.5},
		/*BUL*/{ -0.5, 0.5, -0.5},
		/*BLL*/{ -0.5, -0.5, -0.5},
		/*BLR*/{ 0.5, -0.5, -0.5},
		/*FUR*/{ 0.5, 0.5, 0.5},
		/*FUL*/{-0.5, 0.5, 0.5},
		/*FLL*/{-0.5, -0.5, 0.5},
		/*FLR*/{0.5, -0.5, 0.5}
	};
	std::vector<uint32_t> faces = {
		0, 1, 2,
		0, 2, 3,
		4, 0, 3,
		4, 3, 7,
		5, 4, 7,
		5, 7, 6,
		1, 5, 6,
		1, 6, 2,
		4, 5, 1,
		4, 1, 0,
		2, 6, 7,
		2, 7, 3
	};
	return constructMesh(vertices, faces);
}

int main() {
	// Initialize the window and OpenGL.
	sf::ContextSettings settings;
	settings.depthBits = 24; // Request a 24 bits depth buffer
	settings.stencilBits = 8;  // Request a 8 bits stencil buffer
	settings.antialiasingLevel = 2;  // Request 2 levels of antialiasing
	settings.majorVersion = 3;
	settings.minorVersion = 3;
	sf::Window window(sf::VideoMode{ 1000, 1000 }, "Modern OpenGL", sf::Style::Resize | sf::Style::Close, settings);

	gladLoadGL();
	// Draw in wireframe mode for now.
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	// Inintialize scene objects.
	Mesh obj = cube();
	ShaderProgram program = perspectiveUniformColorShader();
	// Activate the shader program.
	program.activate();
	program.setUniform("color", glm::vec3(1.0, 0.0, 1.0));

	// Ready, set, go!
	bool running = true;
	sf::Clock c;
	auto last = c.getElapsedTime();
	int loops = 0;
	while (running) {
		sf::Event ev;
		while (window.pollEvent(ev)) {
			if (ev.type == sf::Event::Closed) {
				running = false;
			}
		}
		
		auto now = c.getElapsedTime();
		auto diff = now - last;
		std::cout << 1 / diff.asSeconds() << " FPS " << std::endl;
		last = now;


		float progress = loops / 10000.0;
		program.setUniform("color", glm::vec3(progress, 0, 0));
		std::cout << progress << std::endl;
		program.activate();
		// Clear the OpenGL "context".
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		drawMesh(obj);
		window.display();
		loops++;
	}
	
	return 0;
}


