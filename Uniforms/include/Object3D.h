#pragma once
#include <memory>
#include <glm/ext.hpp>
#include "Mesh3D.h"
class Object3D {
private:
	// The object's mesh.
	std::shared_ptr<Mesh3D> m_mesh;

	// The object's position, orientation, and scale in world space.
	glm::vec3 m_position;
	glm::vec3 m_orientation;
	glm::vec3 m_scale;

	// The object's cached local->world transformation matrix.
	mutable glm::mat4 m_modelMatrix;
	mutable bool m_isDirty;

	// Recomputes the local->world transformation matrix.
	void rebuildModelMatrix() const;


public:
	// No default constructor; you must have a mesh to initialize an object.
	Object3D() = delete;

	Object3D(std::shared_ptr<Mesh3D>&& mesh);

	// Simple accessors.
	const glm::vec3& getPosition() const;
	const glm::vec3& getOrientation() const;
	const glm::vec3& getScale() const;

	// Simple mutators.
	void setPosition(const glm::vec3& position);
	void setOrientation(const glm::vec3& orientation);
	void setScale(const glm::vec3& scale);

	// Transformations.
	void move(const glm::vec3& offset);
	void rotate(const glm::vec3& rotation);
	void grow(const glm::vec3& growth);

	// Rendering.
	void render() const;
};