#pragma once

#include "common.h"
#include "buffer.h"
#include "texture.h"
#include "state.h"
#include "../alignedarray.h"

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

		Matrix4x3();
		Matrix4x3(const aiMatrix4x4& m);
		Matrix4x3(const XMMATRIX& m);
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

	// 3D ��
	class Model
	{
		friend ModelRenderer;

	private:
		struct AnimationExtra;

	public:
		class Animation;

		// �ִϸ��̼� ����
		struct AnimationStatus
		{
			// ����� �ִϸ��̼� (in)
			Animation * animation; 

			// ���� �ִϸ��̼� ��� ��ġ (in out)
			double time;

			// �ִϸ��̼��� �Ѿ ���� (out)
			bool overed;

		};

		// 3D ���� ����
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

			// �Ҵ�Ǿ��ִ� �޸𸮸� ����ϴ�.
			void clear();

			// Ư�� ����� ����� �����մϴ�.
			// Row-Based ���
			void setNodeTransform(size_t idx, const XMMATRIX& m);

			// Ư�� ����� ����� �����մϴ�.
			// Row-Based ���
			void setNodeTransform(size_t idx, const aiMatrix4x4& m);

			// Ư�� ����� ����� �����ɴϴ�.
			// Row-Based ���
			const XMMATRIX& getNodeTransform(size_t idx) const;

			// �ش� ��ķ� ���� ��� ������ŵ�ϴ�.
			void transform(const XMMATRIX& m);

			// �ش� ��ķ� ���� ��� ������ŵ�ϴ�.
			void transform(const aiMatrix4x4& m);
			
			// �ִϸ��̼ǿ��� ���� ��������
			bool set(AnimationStatus* status);
			
			// ��� ��� ��
			const Pose operator * (float weight) const;

			// ��� ��� ��
			Pose& operator *= (float weight);

			// ��� ����
			const Pose operator + (const Pose & other) const;

			// ��� ����
			Pose& operator += (const Pose & other);
			
			// �ش� ��� ����
			const Pose operator * (const XMMATRIX& m) const;

			// �ش� ��� ����
			Pose& operator *= (const XMMATRIX& m);

			// �ش� ��� ����
			const Pose operator * (const aiMatrix4x4& m) const;

			// �ش� ��� ����
			Pose& operator *= (const aiMatrix4x4& m);

			// ��� ��� ��
			friend inline const Pose operator * (float weight, const Pose & other)
			{
				return other * weight;
			}

			// ��� ��� ����
			friend inline const Pose operator * (const XMMATRIX& m, const Pose & other)
			{
				Pose pose = other;
				pose.transform(m);
				return pose;
			}
			
		private:
			void _resize(size_t size);

			AlignedArray<XMMATRIX, sizeof(__m128)> m_transforms;
		};

		// �ִϸ��̼� ����
		class Animation
		{
			friend Model;
			friend Pose;
		public:
			Animation();
			Animation(Model * model, size_t idx);

			// �ִϸ��̼� ���� (�� ����)
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

		// ��ü �ִϸ��̼��� �ʴ� Tick Count�� ����
		void setAnimationTPS(double tps);

		// �ִϸ��̼� ���� ��������
		size_t getAnimationCount() const;

		// �ִϸ��̼� ���� ��������
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
		// �ִϸ��̼� ���� ������
		void render(const Model & model, const aiMatrix4x4 & world);

		// �ִϸ��̼� ���� ������
		void render(const Model & model, const XMMATRIX & world);

		// �ش� ����� ������
		void render(const Model & model, const Model::Pose & pose);

		// �������� �������� ����� ������ �޾ƿ´�.
		virtual void setMaterial(const Material & mtl) = 0;

		// �������� �������� ����� ����� �޾ƿ´�.
		// Row-Based ���
		virtual void setWorld(const XMMATRIX & matrix) = 0;

		// �������� �������� ����� ��� ����� �޾ƿ´�.
		// 4x3 Row-Based ���
		virtual void setBoneWorlds(const Matrix4x3 * matrix, size_t m4x3count) = 0;

	private:
		void _renderNode(const Model & model, aiNode * node, const XMMATRIX & mParent);
		void _renderNode(const Model & model, aiNode * node, const Model::Pose & pose);

	};

}