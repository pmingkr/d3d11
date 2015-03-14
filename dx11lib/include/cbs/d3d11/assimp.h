#pragma once

#include "common.h"
#include "buffer.h"
#include "texture.h"
#include "state.h"
#include "vertexlayout.h"
#include "../alignedarray.h"
#include "../datalist.h"

#include <vector>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4819)
#endif
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#ifdef _MSC_VER
#pragma warning(pop)
#endif

namespace cbs
{
	struct Material;
	class AssimpLogger;
	class Model;
	class ModelRenderer;
	typedef unsigned short vindex_t;

	const double DEFAULT_TICK_PER_SECOND = 10.0;

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
		private:
			void _resize(size_t size);

			AlignedArray<XMMATRIX, sizeof(__m128)> m_transforms;

		public:
			inline Pose()
			{
			}
			inline ~Pose()
			{
			}
			inline Pose(const Pose& _copy)
			{
				m_transforms = _copy.m_transforms;
			}
			inline Pose(Pose&& _move)
			{
				m_transforms = std::move(_move.m_transforms);
			}
			inline Pose& operator =(const Pose& _copy)
			{
				this->~Pose();
				new(this) Pose(_copy);
				return *this;
			}
			inline Pose& operator =(Pose&& _move)
			{
				this->~Pose();
				new(this) Pose(std::move(_move));
				return *this;
			}

			// 할당되어있는 메모리를 지웁니다.
			void clear()
			{
				m_transforms.remove();
			}

			// 특정 노드의 행렬을 변경합니다.
			// Row-Based 행렬
			inline void setNodeTransform(size_t idx, const XMMATRIX& m)
			{
				assert(idx < m_transforms.size());
				m_transforms[idx] = m;
			}

			// 특정 노드의 행렬을 변경합니다.
			// Row-Based 행렬
			inline void setNodeTransform(size_t idx, const aiMatrix4x4& m)
			{
				assert(idx < m_transforms.size());
				(aiMatrix4x4&)m_transforms[idx] = m;
			}

			// 특정 노드의 행렬을 가져옵니다.
			// Row-Based 행렬
			inline const XMMATRIX& getNodeTransform(size_t idx) const
			{
				assert(idx < m_transforms.size());
				return m_transforms[idx];
			}

			// 해당 행렬로 모델의 포즈를 변형시킵니다.
			inline void transform(const XMMATRIX& m)
			{
				XMMATRIX tm = XMMatrixTranspose(m);
				for (size_t i = 0; i < m_transforms.size(); i++)
				{
					XMMATRIX& dest = m_transforms[i];
					dest = tm * dest;
				}
			}

			// 해당 행렬로 모델의 포즈를 변형시킵니다.
			inline void transform(const aiMatrix4x4& m)
			{
				XMMATRIX nm;
				(aiMatrix4x4&)nm = m;
				for (size_t i = 0; i < m_transforms.size(); i++)
				{
					XMMATRIX& dest = m_transforms[i];
					dest = nm * dest;
				}
			}
			
			// 애니메이션에서 포즈 가져오기
			CBS_DX11LIB_EXPORT bool set(AnimationStatus* status);
			
			// 행렬 상수 배
			inline Pose& operator *= (float weight)
			{
				XMVECTOR * dst = (XMVECTOR*)m_transforms.data();
				XMVECTOR * end = dst + m_transforms.size() * (sizeof(XMMATRIX) / sizeof(XMVECTOR));
				while (dst != end)
				{
					*dst = XMVectorScale(*dst, weight);
					dst++;
				}
				return *this;
			}
			
			// 행렬 덧셈
			inline Pose& operator += (const Pose & other)
			{
				assert(m_transforms.size() == other.m_transforms.size());

				XMVECTOR * dst = (XMVECTOR*)m_transforms.data();
				XMVECTOR * end = dst + m_transforms.size() * (sizeof(XMMATRIX) / sizeof(XMVECTOR));
				const XMVECTOR * src = (XMVECTOR*)other.m_transforms.data();

				while (dst != end)
				{
					*dst = XMVectorAdd(*dst, *src);
					dst++;
					src++;
				}
				return *this;
			}
			
			// 해당 행렬 곱셈
			inline Pose& operator *= (const XMMATRIX& m)
			{
				XMMATRIX * dst = m_transforms.data();
				XMMATRIX * end = dst + m_transforms.size();

				while (dst != end)
				{
					*dst *= m;
				}
				return *this;
			}

			// 해당 행렬 곱셈
			inline Pose& operator *= (const aiMatrix4x4& m)
			{
				XMMATRIX tm;
				(aiMatrix4x4&)tm = m;
				return *this *= tm;
			}

			// 행렬 상수 배
			inline Pose operator * (float weight) const
			{
				Pose npose = *this;
				npose *= weight;
				return npose;
			}

			// 행렬 덧셈
			inline const Pose operator + (const Pose & other) const
			{
				Pose npose = *this;
				npose += other;
				return npose;
			}

			// 해당 행렬 곱셈
			inline const Pose operator * (const XMMATRIX& m) const
			{
				Pose tmp = *this;
				return tmp *= m;
			}

			// 해당 행렬 곱셈
			inline const Pose operator * (const aiMatrix4x4& m) const
			{
				XMMATRIX tm;
				(aiMatrix4x4&)tm = m;
				return *this * tm;
			}

			// 행렬 상수 배
			friend inline const Pose operator * (float weight, const Pose & other)
			{
				return other * weight;
			}

			// 행렬 행렬 곱셈
			friend inline const Pose operator * (const XMMATRIX& m, const Pose & other)
			{
				Pose pose = other;
				pose.transform(m);
				return pose;
			}
			
		};

		// 애니메이션 정보
		class Animation
		{
			friend Model;
			friend Pose;
		private:
			double m_tps;
			double m_duration;
			aiAnimation * m_animation;
			aiNode * m_root;
			size_t m_nodeCount;
			size_t m_index;

		public:
			inline Animation()
				: m_nodeCount(0), m_animation(nullptr), m_index(0), m_root(nullptr), m_tps(DEFAULT_TICK_PER_SECOND)
			{
			}
			inline Animation(Model * model, size_t idx)
				: m_nodeCount(model->m_nodeCount), m_animation(model->m_scene->mAnimations[idx]), m_index(idx), m_root(model->m_scene->mRootNode)
			{
				if (m_animation->mTicksPerSecond == 0)
				{
					m_tps = DEFAULT_TICK_PER_SECOND;
				}
				else
				{
					m_tps = m_animation->mTicksPerSecond;
				}
				m_duration = m_animation->mDuration / m_tps;
			}

			// 애니메이션 길이 (초 단위)
			inline double getDuration() const
			{
				return m_duration;
			}
			inline double getTPS() const
			{
				return m_tps;
			}
			inline void setTPS(double tps)
			{
				m_tps = tps;
				m_duration = m_animation->mDuration / tps;
			}
		};

		class NodeExtra
		{
		private:
			const size_t m_id;
			AutoDeleteArray<aiNodeAnim *> m_nodeanim;

		public:
			inline NodeExtra(size_t id)
				:m_id(id), m_nodeanim(nullptr)
			{
			}
			inline ~NodeExtra()
			{
			}

			inline size_t getId()
			{
				return m_id;
			}
			inline bool hasAnimation()
			{
				return m_nodeanim != nullptr;
			}
			inline void setAnimationCount(size_t count)
			{
				m_nodeanim = new aiNodeAnim*[count];
				memset(m_nodeanim, 0, sizeof(aiNodeAnim*) * count);
			}
			inline void setAnimation(size_t id, aiNodeAnim * anim)
			{
				m_nodeanim[id] = anim;
			}
			inline aiNodeAnim * getAnimation(size_t id)
			{
				if (m_nodeanim == nullptr) return nullptr;
				return m_nodeanim[id];
			}
		};

	private:
		CBS_DX11LIB_EXPORT void _create(const char * strName, DataList<VertexLayout> basic_vl, DataList<VertexLayout> skinned_vl);
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

	public:
		CBS_DX11LIB_EXPORT static const VertexLayout DEFAULT_BASIC_LAYOUT[3];
		CBS_DX11LIB_EXPORT static const VertexLayout DEFAULT_SKINNED_LAYOUT[5];

		inline Model()
		{
			m_scene = nullptr;
			m_nodeCount = 0;
		}
		inline explicit Model(const char * strName)
		{
			_create(strName, DEFAULT_BASIC_LAYOUT, DEFAULT_SKINNED_LAYOUT);
		}
		inline Model(const char * strName, DataList<VertexLayout> basic_vl, DataList<VertexLayout> skinned_vl)
		{
			_create(strName, basic_vl, skinned_vl);
		}
		CBS_DX11LIB_EXPORT ~Model();

		inline Model(Model&& _move)
		{
			m_meshes = std::move(_move.m_meshes);
			m_materials = std::move(_move.m_materials);
			m_animations = std::move(_move.m_animations);
			m_vb = std::move(_move.m_vb);
			m_ib = std::move(_move.m_ib);
			m_scene = _move.m_scene;
			m_nodeCount = _move.m_nodeCount;
			_move.m_scene = nullptr;
			_move.m_nodeCount = 0;
		}
		inline Model & operator =(Model && _move)
		{
			this->~Model();
			new(this) cbs::Model(std::move(_move));
			return *this;
		}

		// 전체 애니메이션의 초당 Tick Count를 변경
		inline void setAnimationTPS(double tps)
		{
			for (unsigned int i = 0; i < m_scene->mNumAnimations; i++)
			{
				m_animations[i].setTPS(tps);
			}
		}

		// 애니메이션 개수 가져오기
		inline size_t getAnimationCount() const
		{
			return m_scene->mNumAnimations;
		}

		// 애니메이션 정보 가져오기
		inline Animation * getAnimation(size_t animation) const
		{
			assert(animation < m_scene->mNumAnimations);
			Animation * anim = &m_animations[animation];
			if (anim->m_animation == nullptr) return nullptr;
			return anim;
		}

		inline operator bool()
		{
			return m_scene != nullptr;
		}
		inline bool operator !()
		{
			return m_scene == nullptr;
		}

	};

	class ModelRenderer
	{
	private:
		inline void _renderNode(const Model & model, aiNode * node, const XMMATRIX & mParent)
		{
			XMMATRIX transform;
			(aiMatrix4x4&)transform = node->mTransformation;

			XMMATRIX m = mParent * transform;

			if (node->mNumMeshes != 0)
			{
				setWorld(m);

				for (unsigned int n = 0; n < node->mNumMeshes; ++n)
				{
					const aiMesh* mesh = model.m_scene->mMeshes[node->mMeshes[n]];
					const Model::MeshExtra& vbregion = model.m_meshes[node->mMeshes[n]];
					const cbs::Material& mtl = model.m_materials[mesh->mMaterialIndex];

					g_context->IASetVertexBuffers(0, 1, &model.m_vb, &vbregion.stride, &vbregion.offset);
					g_context->RSSetState(mtl.rasterizer);
					setMaterial(mtl);
					g_context->IASetPrimitiveTopology(vbregion.topology);
					g_context->DrawIndexed(vbregion.icount, vbregion.ioffset, 0);
				}
			}

			for (unsigned int n = 0; n < node->mNumChildren; ++n)
			{
				_renderNode(model, node->mChildren[n], m);
			}
		}
		inline void _renderNode(const Model & model, aiNode * node, const Model::Pose & pose)
		{
			Model::NodeExtra* nodeex = (Model::NodeExtra*)node->mAttachment;

			for (unsigned int n = 0; n < node->mNumMeshes; ++n)
			{
				const aiMesh* mesh = model.m_scene->mMeshes[node->mMeshes[n]];
				const Model::MeshExtra& meshex = model.m_meshes[node->mMeshes[n]];
				const cbs::Material& mtl = model.m_materials[mesh->mMaterialIndex];

				if (mesh->mBones != nullptr)
				{
					Matrix4x3 bones[AI_BONE_LIMIT];
					if (mesh->mNumBones == 1)
					{
						size_t nodeId = meshex.boneToNode[0];

						XMMATRIX offsetm;
						(aiMatrix4x4&)offsetm = mesh->mBones[0]->mOffsetMatrix;
						setWorld(pose.m_transforms[nodeId] * offsetm);
					}
					else
					{
						for (unsigned int bi = 0; bi < mesh->mNumBones; bi++)
						{
							size_t nodeId = meshex.boneToNode[bi];

							XMMATRIX offsetm;
							(aiMatrix4x4&)offsetm = mesh->mBones[bi]->mOffsetMatrix;
							bones[bi] = pose.m_transforms[nodeId] * offsetm;
						}
						setBoneWorlds(bones, mesh->mNumBones);
					}
				}
				else
				{
					assert(nodeex != nullptr);
					setWorld(pose.m_transforms[nodeex->getId()]);
				}

				g_context->IASetVertexBuffers(0, 1, &model.m_vb, &meshex.stride, &meshex.offset);
				g_context->RSSetState(mtl.rasterizer);
				setMaterial(mtl);
				g_context->IASetPrimitiveTopology(meshex.topology);
				g_context->DrawIndexed(meshex.icount, meshex.ioffset, 0);
			}

			for (unsigned int n = 0; n < node->mNumChildren; ++n)
			{
				_renderNode(model, node->mChildren[n], pose);
			}
		}

	public:
		// 애니메이션 없이 렌더링
		inline void render(const Model & model, const aiMatrix4x4 & world)
		{
			g_context->IASetIndexBuffer(model.m_ib, DXGI_FORMAT_R16_UINT, 0);

			XMMATRIX m;
			(aiMatrix4x4&)m = world;
			_renderNode(model, model.m_scene->mRootNode, m);
		}

		// 애니메이션 없이 렌더링
		inline void render(const Model & model, const XMMATRIX & world)
		{
			g_context->IASetIndexBuffer(model.m_ib, DXGI_FORMAT_R16_UINT, 0);
			_renderNode(model, model.m_scene->mRootNode, XMMatrixTranspose(world));
		}

		// 해당 포즈로 렌더링
		inline void render(const Model & model, const Model::Pose & pose)
		{
			g_context->IASetIndexBuffer(model.m_ib, DXGI_FORMAT_R16_UINT, 0);
			_renderNode(model, model.m_scene->mRootNode, pose);
		}

		// 렌더러가 렌더링시 사용할 제질을 받아온다.
		virtual void setMaterial(const Material & mtl) = 0;

		// 렌더러가 렌더링시 사용할 행렬을 받아온다.
		// Row-Based 행렬
		virtual void setWorld(const XMMATRIX & matrix) = 0;

		// 렌더러가 렌더링시 사용할 행렬 목록을 받아온다.
		// 4x3 Row-Based 행렬
		virtual void setBoneWorlds(const Matrix4x3 * matrix, size_t m4x3count) = 0;

	};

}