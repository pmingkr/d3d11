#pragma once

#include "common.h"
#include "buffer.h"
#include "texture.h"
#include "state.h"

#include <vector>

struct aiScene;
struct aiNode;
struct aiMaterial;
struct aiNodeAnim;

#include <assimp/types.h>

namespace cbs
{
	class ModelRenderer;
	typedef unsigned short vindex_t;

	struct Material
	{
		enum {MAX_TEXTURE = 8};
		Texture textures[MAX_TEXTURE];
		aiColor4D diffuse;
		aiColor4D specular;
		aiColor4D ambient;
		aiColor4D emissive;
		RasterizerState rasterizer;
		size_t texCount;
		float shininess;
	};


	class Assimp
	{
	public:
		Assimp();
		~Assimp();
	};

	class Model
	{
		friend ModelRenderer;

	public:
		Model();
		explicit Model(const char * strName);
		~Model();

		Model(Model&& _move);
		Model & operator =(Model && _move);

		operator bool();
		bool operator !();

	private:
		Model(const Model& _copy); // = delete
		Model & operator =(const Model & _copy); // = delete
		void _makeTexture(const char * strName);
		void _makeBuffer();
		template <typename LAMBDA> void _callEachNode(aiNode * node, LAMBDA &lambda);

		struct Mesh
		{
			UINT stride;
			UINT offset;
			UINT ioffset;
			UINT icount;
			D3D11_PRIMITIVE_TOPOLOGY topology;
		};

		const aiScene* m_scene;
		std::vector<Mesh> m_meshes;
		std::vector<Material> m_materials;
		Buffer m_vb;
		Buffer m_ib;
	};

	class ModelRenderer
	{
	public:
		// 애니메이션 없이 렌더링
		void render(const Model & model, const aiMatrix4x4 & world);

		// 애니메이션으로 렌더링
		void render(const Model & model, const aiMatrix4x4 & world, size_t animation, double time);

		virtual void setMaterial(const Material & mtl) = 0;
		virtual void setWorld(const aiMatrix4x4 & matrix) = 0;

	private:
		void _renderNode(const Model & model, aiNode * node, const aiMatrix4x4 & mParent);
		void _renderNode(const Model & model, aiNode * node, const aiMatrix4x4 & mParent, size_t animation, double time);

	};

}