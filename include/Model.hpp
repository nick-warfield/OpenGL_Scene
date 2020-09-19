#ifndef MODEL_HPP
#define MODEL_HPP

#include <glm/glm.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Texture.hpp"
#include "Mesh.hpp"

#include <iostream>
#include <string>
#include <vector>

struct Model {
	std::vector<Mesh> mesh;
};

void draw_model(const Model &model, unsigned int shader, glm::mat4 model_matrix) {
	glUseProgram(shader);
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model_matrix));
	for (auto m : model.mesh) {
		draw_mesh(m, shader);
	}
}

std::vector<Texture> load_material_textures(aiMaterial *material, aiTextureType type, std::string name) {
	std::vector<Texture> t;
	for (unsigned int i = 0; i < material->GetTextureCount(type); ++i) {
		aiString str;
		material->GetTexture(type, i, &str);
		t.push_back(make_texture(std::string(str.C_Str()), name));
	}
	return t;
}

Mesh process_mesh(std::string directory, aiMesh *mesh, const aiScene *scene) {
	Mesh m;

	// process vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
		Vertex v;

		v.position.x = mesh->mVertices[i].x;
		v.position.y = mesh->mVertices[i].y;
		v.position.z = mesh->mVertices[i].z;

		v.normal.x = mesh->mNormals[i].x;
		v.normal.y = mesh->mNormals[i].y;
		v.normal.z = mesh->mNormals[i].z;

		if (mesh->mTextureCoords[0]) {
			v.texture_coordinate.x = mesh->mTextureCoords[0][i].x;
			v.texture_coordinate.y = mesh->mTextureCoords[0][i].y;
		} else {
			v.texture_coordinate = glm::vec2(0.0f);
		}

		m.vertex.push_back(v);
	}

	// proccess indices
	for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; ++j) {
			m.index.push_back(face.mIndices[j]);
		}
	}

	// process materials
	if (mesh->mMaterialIndex >= 0) {
		aiMaterial *mat = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuse_maps = load_material_textures(mat, aiTextureType_DIFFUSE, "texture_diffuse");
		std::vector<Texture> specular_maps = load_material_textures(mat, aiTextureType_SPECULAR, "texture_specular");

		m.texture.insert(m.texture.end(), diffuse_maps.begin(), diffuse_maps.end());
		m.texture.insert(m.texture.end(), specular_maps.begin(), specular_maps.end());
	}

	return m;
}

void process_node(std::vector<Mesh> output, std::string directory, aiNode *node, const aiScene *scene) {
	for (uint i = 0; i < node->mNumMeshes; ++i) {
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		output.push_back(process_mesh(directory, mesh, scene));
	}

	for (uint i = 0; i < node->mNumChildren; ++i) {
		process_node(output, directory, node->mChildren[i], scene);
	}
}

Model load_model(std::string model_path) {
	Model m;

	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(model_path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags * AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "Error: Assimp: " << importer.GetErrorString() << std::endl;
		return m;
	}
	std::string directory = model_path.substr(0, model_path.find_last_of('/'));
	process_node(m.mesh, directory, scene->mRootNode, scene);

	return m;
}

#endif
