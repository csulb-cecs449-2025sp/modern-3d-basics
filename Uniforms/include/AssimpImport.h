#pragma once
#include "Mesh3D.h"
#include "Object3D.h"
#include <assimp/scene.h>

Mesh3D fromAssimpMesh(const aiMesh* mesh);
Object3D assimpLoad(const std::string& path);
