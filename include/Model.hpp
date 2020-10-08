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
	std::vector<Texture> loaded_texture;
	std::string directory;
};

void draw_model(const Model &model, unsigned int shader, glm::mat4 model_matrix) {
	glUseProgram(shader);
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model_matrix));
	for (auto m : model.mesh) {
		draw_mesh(m, shader);
	}
}

// loading model functions
void load_material_textures(Model &model, aiMaterial *material, aiTextureType type, std::string name) {
	for (unsigned int i = 0; i < material->GetTextureCount(type); ++i) {
		aiString str;
		material->GetTexture(type, i, &str);
		bool is_loaded = false;

		for (auto tex : model.loaded_texture) {
			if (std::strcmp(tex.path.data(), (model.directory + "/" + name + ".jpg").c_str()) == 0) {
				model.mesh.back().texture.push_back(tex);
				is_loaded = true;
				break;
			}
		}

		if (!is_loaded) {
			auto texture = make_texture(name, model.directory + "/" + name + ".jpg");
			model.mesh.back().texture.push_back(texture);
			model.loaded_texture.push_back(texture);
		}
	}
}

void process_node(Model &model, aiNode *node, const aiScene *scene) {
	for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		model.mesh.push_back(Mesh());

		// process vertices
		for (unsigned int j = 0; j < mesh->mNumVertices; ++j) {
			Vertex v;

			v.position.x = mesh->mVertices[j].x;
			v.position.y = mesh->mVertices[j].y;
			v.position.z = mesh->mVertices[j].z;

			if (mesh->HasNormals()) {
				v.normal.x = mesh->mNormals[j].x;
				v.normal.y = mesh->mNormals[j].y;
				v.normal.z = mesh->mNormals[j].z;
			}

			if (mesh->mTextureCoords[0]) {
				v.texture_coordinate.x = mesh->mTextureCoords[0][j].x;
				v.texture_coordinate.y = mesh->mTextureCoords[0][j].y;
			} else {
				v.texture_coordinate = glm::vec2(0.0f);
			}

			model.mesh.back().vertex.push_back(v);
		}

		// proccess indices
		for (unsigned int j = 0; j < mesh->mNumFaces; ++j) {
			aiFace face = mesh->mFaces[j];
			for (unsigned int k = 0; k < face.mNumIndices; ++k) {
				model.mesh.back().index.push_back(face.mIndices[k]);
			}
		}

		// process materials
		if (mesh->mMaterialIndex >= 0) {
			aiMaterial *mat = scene->mMaterials[mesh->mMaterialIndex];
			load_material_textures(model, mat, aiTextureType_DIFFUSE, "diffuse");
			load_material_textures(model, mat, aiTextureType_SPECULAR, "specular");
		}
	}

	for (uint i = 0; i < node->mNumChildren; ++i) {
		process_node(model, node->mChildren[i], scene);
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
	m.directory = model_path.substr(0, model_path.find_last_of('/'));
	process_node(m, scene->mRootNode, scene);

	uint count = 0;
	for (uint i = 0; i < scene->mNumMeshes; ++i) {
		for (uint j = 0; j < scene->mMeshes[i]->mNumVertices; ++j) {
			// check positions
			if (scene->mMeshes[i]->mVertices[j].x != m.mesh[i].vertex[j].position.x) {
				std::cout << "Error: position.x not loaded correctly" << std::endl;
			}
			if (scene->mMeshes[i]->mVertices[j].y != m.mesh[i].vertex[j].position.y) {
				std::cout << "Error: position.y not loaded correctly" << std::endl;
			}
			if (scene->mMeshes[i]->mVertices[j].z != m.mesh[i].vertex[j].position.z) {
				std::cout << "Error: position.z not loaded correctly" << std::endl;
			}

			// check normals
			if (scene->mMeshes[i]->mNormals[j].x != m.mesh[i].vertex[j].normal.x) {
				std::cout << "Error: normal.x not loaded correctly" << std::endl;
			}
			if (scene->mMeshes[i]->mNormals[j].y != m.mesh[i].vertex[j].normal.y) {
				std::cout << "Error: normal.y not loaded correctly" << std::endl;
			}
			if (scene->mMeshes[i]->mNormals[j].z != m.mesh[i].vertex[j].normal.z) {
				std::cout << "Error: normal.z not loaded correctly" << std::endl;
			}

			// check texture coordinates
			if (scene->mMeshes[i]->mTextureCoords[0][j].x != m.mesh[i].vertex[j].texture_coordinate.x) {
				std::cout << "Error: texture_coordinate.x not loaded correctly" << std::endl;
			}
			if (scene->mMeshes[i]->mTextureCoords[0][j].y != m.mesh[i].vertex[j].texture_coordinate.y) {
				std::cout << "Error: texture_coordinate.y not loaded correctly" << std::endl;
			}
		}

		uint offset = 0;
		for (uint j = 0; j < scene->mMeshes[i]->mNumFaces; ++j) {
			auto face = scene->mMeshes[i]->mFaces[j];
			for (uint k = 0; k < face.mNumIndices; ++k) {
				if (face.mIndices[k] != m.mesh[i].index[offset]) {
					std::cout << "Error: index not equal: ";
					std::cout << scene->mMeshes[i]->mFaces[j].mIndices[k] << " != " << m.mesh[i].index[offset] << std::endl;
				}
				offset++;
			}
		}
	}

	for (auto mesh : m.mesh) {
		count += mesh.vertex.size();
	}

	return m;
}

#endif
