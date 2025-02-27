/**
This application displays a mesh in wireframe using "Modern" OpenGL 3.0+.
The Mesh3D class now initializes a "vertex array" on the GPU to store the vertices
	and faces of the mesh. To render, the Mesh3D object simply triggers the GPU to draw
	the stored mesh data.
We now transform local space vertices to clip space using uniform matrices in the vertex shader.
	See "simple_perspective.vert" for a vertex shader that uses uniform model, view, and projection
		matrices to transform to clip space.
	See "uniform_color.frag" for a fragment shader that sets a pixel to a uniform parameter.
*/

#include <glad/glad.h>
#include <filesystem>
#include <iostream>
#include "ShaderProgram.h"
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Window.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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
		shader.load("shaders/simple_perspective.vert", "shaders/uniform_color.frag");
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

const size_t FLOATS_PER_VERTEX = 3;
const size_t VERTICES_PER_FACE = 3;

// Reads the vertices and faces of an Assimp mesh, and uses them to initialize mesh structures
// compatible with the rest of our application.
void fromAssimpMesh(const aiMesh* mesh, std::vector<Vertex3D>& vertices,
	std::vector<uint32_t>& faces) {
	for (size_t i = 0; i < mesh->mNumVertices; i++) {
		// Each "vertex" from Assimp has to be transformed into a Vertex3D in our application.
		vertices.push_back({ mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z });
	}

	faces.reserve(mesh->mNumFaces * VERTICES_PER_FACE);
	for (size_t i = 0; i < mesh->mNumFaces; i++) {
		// We assume the faces are triangular, so we push three face indexes at a time into our faces list.
		faces.push_back(mesh->mFaces[i].mIndices[0]);
		faces.push_back(mesh->mFaces[i].mIndices[1]);
		faces.push_back(mesh->mFaces[i].mIndices[2]);
	}
}

// Loads an asset file supported by Assimp, extracts the first mesh in the file, and fills in the 
// given vertices and faces lists with its data.
Mesh assimpLoad(const std::string& path) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

	// If the import failed, report it
	if (nullptr == scene) {
		std::cout << "ASSIMP ERROR" << importer.GetErrorString() << std::endl;
		exit(1);
	}
	else {
		std::vector<Vertex3D> vertices;
		std::vector<uint32_t> faces;
		fromAssimpMesh(scene->mMeshes[0], vertices, faces);
		return constructMesh(vertices, faces);
	}
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

glm::mat4 buildModelMatrix(const glm::vec3& position, const glm::vec3& orientation, const glm::vec3& scale) {
	auto m = glm::translate(glm::mat4(1), position);
	m = glm::scale(m, scale);
	m = glm::rotate(m, orientation[2], glm::vec3(0, 0, 1));
	m = glm::rotate(m, orientation[0], glm::vec3(1, 0, 0));
	m = glm::rotate(m, orientation[1], glm::vec3(0, 1, 0));
	return m;
}

int main() {
	std::cout << std::filesystem::current_path() << std::endl;
	// Initialize the window and OpenGL.
	sf::ContextSettings settings;
	settings.depthBits = 24; // Request a 24 bits depth buffer
	settings.stencilBits = 8;  // Request a 8 bits stencil buffer
	settings.antialiasingLevel = 2;  // Request 2 levels of antialiasing
	settings.majorVersion = 3;
	settings.minorVersion = 3;
	sf::Window window(sf::VideoMode{ 1200, 800 }, "Modern OpenGL", sf::Style::Resize | sf::Style::Close, settings);

	gladLoadGL();
	// Draw in wireframe mode for now.
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	// Inintialize scene objects.
	//Mesh obj = cube();
	Mesh obj = assimpLoad("models/bunny.obj");
	ShaderProgram program = perspectiveUniformColorShader();

	// Activate the shader program.
	program.activate();
	program.setUniform("color", glm::vec3(1, 0.2, 0.9));

	glm::vec3 objectPosition = { 0, -0.1, -2 };
	glm::vec3 objectOrientation = { 0, 0, 0 };
	glm::vec3 objectScale = { 1, 1, 1 };

	// Ready, set, go!
	bool running = true;
	sf::Clock c;
	auto last = c.getElapsedTime();
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

		// Set up the view and projection matrices.
		glm::mat4 camera = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
		glm::mat4 perspective = glm::perspective(glm::radians(45.0), static_cast<double>(window.getSize().x) / window.getSize().y, 0.1, 100.0);
		program.setUniform("view", camera);
		program.setUniform("projection", perspective);

		// Adjust object position and rebuild model matrix.
		objectOrientation += glm::vec3(0, 0.0003, 0);
		objectPosition += glm::vec3(0, 0, 0.00005);
		glm::mat4 model = buildModelMatrix(objectPosition, objectOrientation, objectScale);
		program.setUniform("model", model);

		// Clear the OpenGL "context".
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		drawMesh(obj);
		window.display();


		
	}

	return 0;
}


