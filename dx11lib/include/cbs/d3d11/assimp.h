#pragma once

#include "common.h"
#include "buffer.h"
#include "texture.h"
#include "state.h"
#include "vertexlayout.h"
#include "../alignedarray.h"
#include "../datalist.h"

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
	class AssimpLogger;
	class Model;
	class ModelRenderer;
	typedef unsigned short vindex_t;

	struct Matrix4x3
	{
		float _11, _21, _31, _41;
		float _12, _22, _32, _42;
		float _13, _23, _33, _43;

		inline Matrix4x3()
		{
		}
		inline Matrix4x3(const aiMatrix4x4& m)
		{
			memcpy(this, &m, sizeof(Matrix4x3));
		}
		inline Matrix4x3(const XMMATRIX& m)
		{
			memcpy(this, &m, sizeof(Matrix4x3));
		}
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
		unsigned int texCount;
		float shininess;
	};

	class AssimpLogger
	{
	public:
		CBS_DX11LIB_EXPORT AssimpLogger();
		CBS_DX11LIB_EXPORT ~AssimpLogger();
	};

	class CBS_DEPRECATED Assimp: public AssimpLogger
	{
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
			CBS_DX11LIB_EXPORT Pose();
			CBS_DX11LIB_EXPORT ~Pose();

			CBS_DX11LIB_EXPORT Pose(const Pose& _copy);
			CBS_DX11LIB_EXPORT Pose(Pose&& _move);
			CBS_DX11LIB_EXPORT Pose& operator =(const Pose& _copy);
			CBS_DX11LIB_EXPORT Pose& operator =(Pose&& _move);

			// �Ҵ�Ǿ��ִ� �޸𸮸� ����ϴ�.
			CBS_DX11LIB_EXPORT void clear();

			// Ư�� ����� ����� �����մϴ�.
			// Row-Based ���
			CBS_DX11LIB_EXPORT void setNodeTransform(size_t idx, const XMMATRIX& m);

			// Ư�� ����� ����� �����մϴ�.
			// Row-Based ���
			CBS_DX11LIB_EXPORT void setNodeTransform(size_t idx, const aiMatrix4x4& m);

			// Ư�� ����� ����� �����ɴϴ�.
			// Row-Based ���
			CBS_DX11LIB_EXPORT const XMMATRIX& getNodeTransform(size_t idx) const;

			// �ش� ��ķ� ���� ��� ������ŵ�ϴ�.
			CBS_DX11LIB_EXPORT void transform(const XMMATRIX& m);

			// �ش� ��ķ� ���� ��� ������ŵ�ϴ�.
			CBS_DX11LIB_EXPORT void transform(const aiMatrix4x4& m);
			
			// �ִϸ��̼ǿ��� ���� ��������
			CBS_DX11LIB_EXPORT bool set(AnimationStatus* status);
			
			// ��� ��� ��
			CBS_DX11LIB_EXPORT Pose& operator *= (float weight);
			
			// ��� ����
			CBS_DX11LIB_EXPORT Pose& operator += (const Pose & other);
			
			// �ش� ��� ����
			CBS_DX11LIB_EXPORT Pose& operator *= (const XMMATRIX& m);

			// �ش� ��� ����
			inline Pose& operator *= (const aiMatrix4x4& m)
			{
				XMMATRIX tm;
				(aiMatrix4x4&)tm = m;
				return *this *= tm;
			}

			// ��� ��� ��
			inline Pose operator * (float weight) const
			{
				Pose npose = *this;
				npose *= weight;
				return npose;
			}

			// ��� ����
			inline const Pose operator + (const Pose & other) const
			{
				Pose npose = *this;
				npose += other;
				return npose;
			}

			// �ش� ��� ����
			inline const Pose operator * (const XMMATRIX& m) const
			{
				Pose tmp = *this;
				return tmp *= m;
			}

			// �ش� ��� ����
			inline const Pose operator * (const aiMatrix4x4& m) const
			{
				XMMATRIX tm;
				(aiMatrix4x4&)tm = m;
				return *this * tm;
			}

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
			CBS_DX11LIB_EXPORT Animation();
			CBS_DX11LIB_EXPORT Animation(Model * model, size_t idx);

			// �ִϸ��̼� ���� (�� ����)
			CBS_DX11LIB_EXPORT double getDuration() const;
			CBS_DX11LIB_EXPORT double getTPS() const;
			CBS_DX11LIB_EXPORT void setTPS(double tps);

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
			CBS_DX11LIB_EXPORT NodeExtra(size_t id);
			CBS_DX11LIB_EXPORT ~NodeExtra();

			CBS_DX11LIB_EXPORT size_t getId();
			CBS_DX11LIB_EXPORT bool hasAnimation();
			CBS_DX11LIB_EXPORT void setAnimationCount(size_t count);
			CBS_DX11LIB_EXPORT void setAnimation(size_t id, aiNodeAnim * anim);
			CBS_DX11LIB_EXPORT aiNodeAnim * getAnimation(size_t id);

		private:
			const size_t m_id;
			AutoDeleteArray<aiNodeAnim *> m_nodeanim;
		};

		CBS_DX11LIB_EXPORT Model();
		CBS_DX11LIB_EXPORT explicit Model(const char * strName);
		CBS_DX11LIB_EXPORT Model(const char * strName, DataList<VertexLayout> basic_vl, DataList<VertexLayout> skinned_vl);
		CBS_DX11LIB_EXPORT ~Model();

		CBS_DX11LIB_EXPORT Model(Model&& _move);
		CBS_DX11LIB_EXPORT Model & operator =(Model && _move);

		// ��ü �ִϸ��̼��� �ʴ� Tick Count�� ����
		CBS_DX11LIB_EXPORT void setAnimationTPS(double tps);

		// �ִϸ��̼� ���� ��������
		CBS_DX11LIB_EXPORT size_t getAnimationCount() const;

		// �ִϸ��̼� ���� ��������
		CBS_DX11LIB_EXPORT Animation * getAnimation(size_t animation) const;
		
		CBS_DX11LIB_EXPORT operator bool();
		CBS_DX11LIB_EXPORT	bool operator !();

		CBS_DX11LIB_EXPORT static const VertexLayout DEFAULT_BASIC_LAYOUT[3];
		CBS_DX11LIB_EXPORT static const VertexLayout DEFAULT_SKINNED_LAYOUT[5];

	private:
		void _create(const char * strName, DataList<VertexLayout> basic_vl, DataList<VertexLayout> skinned_vl);
		Model(const Model& _copy); // = delete
		Model & operator =(const Model & _copy); // = delete
		void _makeTexture(const char * strName);
		void _makeBuffer(DataList<VertexLayout> vl, DataList<VertexLayout> skinned_vl);
		template <typename LAMBDA> void _callEachNode(aiNode * node, LAMBDA &lambda);
		
		struct MeshExtra
		{
			UINT stride;
			UINT offset;
			UINT ioffset;
			UINT icount;
			AutoDeleteArray<size_t> boneToNode;
			D3D11_PRIMITIVE_TOPOLOGY topology;
			bool skinned;
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
		CBS_DX11LIB_EXPORT void render(const Model & model, const aiMatrix4x4 & world);

		// �ִϸ��̼� ���� ������
		CBS_DX11LIB_EXPORT void render(const Model & model, const XMMATRIX & world);

		// �ش� ����� ������
		CBS_DX11LIB_EXPORT void render(const Model & model, const Model::Pose & pose);

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