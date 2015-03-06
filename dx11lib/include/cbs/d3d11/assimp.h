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
struct aiAnimation;

#pragma warning(push)
#pragma warning(disable:4819)
#include <assimp/types.h>
#pragma warning(pop)

namespace cbs
{
	struct Material;
	class Assimp;
	class Model;
	class ModelRenderer;
	typedef unsigned short vindex_t;

	struct Matrix4x3
	{
		float _11, _21, _31, _41;
		float _12, _22, _32, _42;
		float _13, _23, _33, _43;
	};

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

	const int AI_VERTEX_WEIGHT_COUNT = 4;
	const int AI_BONE_LIMIT = 80;

	// 3D 모델
	class Model
	{
		friend ModelRenderer;

	private:
		struct AnimationExtra;

	public:
		class Animation;

		// 애니메이션 상태
		struct AnimationStatus
		{
			// 재생할 애니메이션 (in)
			Animation * animation; 

			// 현재 애니메이션 재생 위치 (in out)
			double time;

			// 애니메이션이 넘어간 여부 (out)
			bool overed;

		};

		// 3D 모델의 포즈
		class Pose
		{
			friend Model;
			friend ModelRenderer;
		public:
			Pose();
			~Pose();

			Pose(const Pose& _copy);
			Pose(Pose&& _move);
			Pose& operator =(const Pose& _copy);
			Pose& operator =(Pose&& _move);

			// 할당되어있는 메모리를 지웁니다.
			void clear();

			// 특정 노드의 행렬을 변경합니다.
			void setNodeTransform(size_t idx, const aiMatrix4x4& m);

			// 특정 노드의 행렬을 가져옵니다.
			const aiMatrix4x4& getNodeTransform(size_t idx) const;

			// 해당 행렬로 모델의 포즈를 변형시킵니다.
			void transform(const aiMatrix4x4& m);

			// 애니메이션에서 포즈 가져오기
			bool set(AnimationStatus* status);
			
		private:
			void _resize(size_t size);

			AutoDeleteArray<aiMatrix4x4> m_nodeTransform;
			size_t m_nodeCount;
		};

		// 애니메이션 정보
		class Animation
		{
			friend Model;
			friend Pose;
		public:
			Animation();
			Animation(Model * model, size_t idx);

			// 애니메이션 길이 (초 단위)
			double getDuration() const;
			double getTPS() const;
			void setTPS(double tps);

		private:
			double m_tps;
			double m_duration;
			aiAnimation * m_animation;
			aiNode * m_root;
			size_t m_nodeCount;
			size_t m_index;
		};

		class NodeExtra
		{
		public:
			NodeExtra(size_t id);
			~NodeExtra();

			size_t getId();
			bool hasAnimation();
			void setAnimationCount(size_t count);
			void setAnimation(size_t id, aiNodeAnim * anim);
			aiNodeAnim * getAnimation(size_t id);

		private:
			const size_t m_id;
			AutoDeleteArray<aiNodeAnim *> m_nodeanim;
		};

		Model();
		explicit Model(const char * strName);
		~Model();

		Model(Model&& _move);
		Model & operator =(Model && _move);

		// 전체 애니메이션의 초당 Tick Count를 변경
		void setAnimationTPS(double tps);

		// 애니메이션 개수 가져오기
		size_t getAnimationCount() const;

		// 애니메이션 정보 가져오기
		Animation * getAnimation(size_t animation) const;
		
		operator bool();
		bool operator !();

	private:
		Model(const Model& _copy); // = delete
		Model & operator =(const Model & _copy); // = delete
		void _makeTexture(const char * strName);
		void _makeBuffer();
		template <typename LAMBDA> void _callEachNode(aiNode * node, LAMBDA &lambda);

		struct MeshExtra
		{
			UINT stride;
			UINT offset;
			UINT ioffset;
			UINT icount;
			D3D11_PRIMITIVE_TOPOLOGY topology;
			AutoDeleteArray<unsigned int> boneToNode;
		};
		
		const aiScene* m_scene;
		AutoDeleteArray<MeshExtra> m_meshes;
		AutoDeleteArray<Material> m_materials;
		AutoDeleteArray<Animation> m_animations;
		Buffer m_vb;
		Buffer m_ib;
		size_t m_nodeCount;
	};

	class ModelRenderer
	{
	public:
		// 애니메이션 없이 렌더링
		void render(const Model & model, const aiMatrix4x4 & world);

		// 해당 포즈로 렌더링
		void render(const Model & model, const Model::Pose & pose);

		virtual void setMaterial(const Material & mtl) = 0;
		virtual void setWorld(const aiMatrix4x4 & matrix) = 0;
		virtual void setBoneWorlds(const Matrix4x3 * matrix, size_t m4x3count) = 0;

	private:
		void _renderNode(const Model & model, aiNode * node, const aiMatrix4x4 & mParent);
		void _renderNode(const Model & model, aiNode * node, const Model::Pose & pose);

	};

}