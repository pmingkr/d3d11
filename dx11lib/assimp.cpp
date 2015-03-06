#include "include/cbs/d3d11/assimp.h"
#include "include/cbs/d3d11/device.h"
#include "include/cbs/d3d11/texture.h"
#include "include/cbs/d3d11/state.h"

#pragma warning(push)
#pragma warning(disable:4819)
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#pragma warning(pop)

#include <new>
#include <map>

#pragma comment(lib, "assimp.lib")

namespace
{
	struct BoneWeight
	{
		unsigned char blendidx[cbs::AI_VERTEX_WEIGHT_COUNT];
		float blendwgt[cbs::AI_VERTEX_WEIGHT_COUNT];
	};

	struct AnimationReader
	{
		size_t m_index;
		cbs::Model::Pose * m_pose;
		double m_time;
		double m_duration;

		template <typename T, typename KEY>
		T interpolateKey(KEY * begin, unsigned int keynum, aiAnimBehaviour pre, aiAnimBehaviour post)
		{
			::Assimp::Interpolator<T> inter;

			KEY * end = begin + keynum - 1;
			KEY * l = begin;
			KEY * r = end;
			T out;

			for (;;)
			{
				KEY * c = (r - l) / 2 + l;
				if (c->mTime < m_time)
				{
					l = c + 1;
				}
				else
				{
					r = c - 1;
				}
				if (l <= r) continue;
				if (r < begin)
				{
					switch (pre)
					{
					case aiAnimBehaviour_REPEAT:
						inter(out, end->mValue, begin->mValue, (float)((m_duration + m_time - end->mTime) / (m_duration - end->mTime + begin->mTime)));
						return out;
					default:
						return begin->mValue;
					}
				}
				if (l > end)
				{
					switch (post)
					{
					case aiAnimBehaviour_REPEAT:
						inter(out, end->mValue, begin->mValue, (float)((m_time - end->mTime) / (m_duration - end->mTime + begin->mTime)));
						return out;
					default:
						return end->mValue;
					}
				}
				inter(out, r->mValue, l->mValue, (float)((m_time - r->mTime) / (l->mTime - r->mTime)));
				return out;
			}
		}

		void _getAnimation(aiNode * node, const aiMatrix4x4 & mParent)
		{
			cbs::Model::NodeExtra* nodeex = (cbs::Model::NodeExtra*)node->mAttachment;

			aiMatrix4x4 m = mParent;

			if (nodeex != nullptr)
			{
				//m *= node->mTransformation;
				aiNodeAnim * anim = nodeex->getAnimation(m_index);
				if (anim != nullptr)
				{
					aiVector3D pos = interpolateKey<aiVector3D>(anim->mPositionKeys, anim->mNumPositionKeys, anim->mPreState, anim->mPostState);
					aiQuaternion quat = interpolateKey<aiQuaternion>(anim->mRotationKeys, anim->mNumRotationKeys, anim->mPreState, anim->mPostState);
					aiVector3D scale = interpolateKey<aiVector3D>(anim->mScalingKeys, anim->mNumScalingKeys, anim->mPreState, anim->mPostState);

					aiMatrix4x4 m2;
					m *= aiMatrix4x4::Translation(pos, m2);
					m *= aiMatrix4x4(quat.GetMatrix());
					m *= aiMatrix4x4::Scaling(scale, m2);
				}
				m_pose->setNodeTransform(nodeex->getId(), m);
			}
			else
			{
				m *= node->mTransformation;
			}

			for (unsigned int n = 0; n < node->mNumChildren; ++n)
			{
				_getAnimation(node->mChildren[n], m);
			}
		}
	};

	const double DEFAULT_TICK_PER_SECOND = 10.0;
}

cbs::Assimp::Assimp()
{
	aiLogStream stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, NULL);

	// get a handle to the predefined STDOUT log stream and attach
	// it to the logging system. It remains active for all further
	// calls to aiImportFile(Ex) and aiApplyPostProcessing.
	// stream = aiGetPredefinedLogStream(aiDefaultLogStream_STDOUT, NULL);

	// 디버거에 로그 출력을 사용, VS 전용 기능
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, NULL);
	aiAttachLogStream(&stream);
}
cbs::Assimp::~Assimp()
{
	aiDetachAllLogStreams();
}

cbs::Model::Pose::Pose()
{
	m_nodeCount = 0;
}
cbs::Model::Pose::~Pose()
{
}
cbs::Model::Pose::Pose(const Pose& _copy)
{
	m_nodeCount = _copy.m_nodeCount;
	if (m_nodeCount != 0)
	{
		m_nodeTransform.allocate(m_nodeCount);
		memcpy(m_nodeTransform, _copy.m_nodeTransform, sizeof(aiMatrix4x4) * m_nodeCount);
	}
	else
	{
		m_nodeTransform = nullptr;
	}
}
cbs::Model::Pose::Pose(Pose&& _move)
{
	m_nodeCount = _move.m_nodeCount;
	m_nodeTransform = std::move(_move.m_nodeTransform);
}
cbs::Model::Pose& cbs::Model::Pose::operator =(const Pose& _copy)
{
	this->~Pose();
	new(this) Pose(_copy);
	return *this;
}
cbs::Model::Pose& cbs::Model::Pose::operator =(Pose&& _move)
{
	this->~Pose();
	new(this) Pose(std::move(_move));
	return *this;
}
void cbs::Model::Pose::clear()
{
	m_nodeCount = 0;
	m_nodeTransform = nullptr;
}
void cbs::Model::Pose::setNodeTransform(size_t idx, const aiMatrix4x4& m)
{
	assert(m_nodeTransform != nullptr);
	m_nodeTransform[idx] = m;
}
const aiMatrix4x4& cbs::Model::Pose::getNodeTransform(size_t idx) const
{
	assert(m_nodeTransform != nullptr);
	return m_nodeTransform[idx];
}
void cbs::Model::Pose::transform(const aiMatrix4x4& m)
{
	for (size_t i = 0; i < m_nodeCount; i++)
	{
		aiMatrix4x4& nodem = m_nodeTransform[i];
		nodem = m * nodem;
	}
}
bool cbs::Model::Pose::set(AnimationStatus* status)
{
	assert(status != nullptr);
	assert(status->animation != nullptr);

	AnimationReader reader;
	reader.m_index = status->animation->m_index;
	reader.m_pose = this;
	double tps = status->animation->m_tps;
	double time = status->time;

	if(time >= status->animation->m_duration)
	{
		time = fmod(time, status->animation->m_duration);
		status->overed = true;
	}
	else
	{
		status->overed = false;
	}
	reader.m_time = tps * time;
	reader.m_duration = tps * status->animation->m_duration;

	_resize(status->animation->m_nodeCount);
	reader._getAnimation(status->animation->m_root, aiMatrix4x4());
	status->time = time;
	return true;
}
void cbs::Model::Pose::_resize(size_t size)
{
	if(m_nodeCount >= size) return;

	m_nodeTransform.allocate(size);
	m_nodeCount = size;
}

cbs::Model::NodeExtra::NodeExtra(size_t id)
	:m_id(id), m_nodeanim(nullptr)
{
}
cbs::Model::NodeExtra::~NodeExtra()
{
}
size_t cbs::Model::NodeExtra::getId()
{
	return m_id;
}
bool cbs::Model::NodeExtra::hasAnimation()
{
	return m_nodeanim != nullptr;
}
void cbs::Model::NodeExtra::setAnimationCount(size_t count)
{
	m_nodeanim = new aiNodeAnim*[count];
	memset(m_nodeanim, 0, sizeof(aiNodeAnim*) * count);
}
void cbs::Model::NodeExtra::setAnimation(size_t id, aiNodeAnim * anim)
{
	m_nodeanim[id] = anim;
}
aiNodeAnim * cbs::Model::NodeExtra::getAnimation(size_t id)
{
	if(m_nodeanim == nullptr) return nullptr;
	return m_nodeanim[id];
}

cbs::Model::Animation::Animation()
	: m_nodeCount(0), m_animation(nullptr), m_index(0), m_root(nullptr), m_tps(DEFAULT_TICK_PER_SECOND)
{
}
cbs::Model::Animation::Animation(Model * model, size_t idx)
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
double cbs::Model::Animation::getDuration() const
{
	return m_duration;
}
double cbs::Model::Animation::getTPS() const
{
	return m_tps;
}
void cbs::Model::Animation::setTPS(double tps)
{
	m_tps = tps;
	m_duration = m_animation->mDuration / tps;
}

cbs::Model::Model()
{
	m_scene = nullptr;
	m_nodeCount = 0;
}
cbs::Model::Model(const char * strName)
{
	m_scene = aiImportFile(strName, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_FlipUVs | aiProcess_SortByPType | aiProcess_LimitBoneWeights);
	if(m_scene == nullptr)
	{
		wchar_t temp[1024];
		swprintf_s(temp, L"%hs 모델을 찾을 수 없습니다.\r\n", strName);
		MessageBox(g_hWnd,temp, nullptr, MB_OK | MB_ICONERROR);
		return;
	}

	_makeTexture(strName);
	_makeBuffer();
}
cbs::Model::~Model()
{
	if (m_scene != nullptr)
	{
		_callEachNode(m_scene->mRootNode, [&](aiNode * node) {
			delete (NodeExtra*)node->mAttachment;
		});
		aiReleaseImport(m_scene);
	}
}
cbs::Model::Model(Model&& _move)
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
cbs::Model & cbs::Model::operator =(Model && _move)
{
	this->~Model();
	new(this) cbs::Model(std::move(_move));
	return *this;
}
void cbs::Model::setAnimationTPS(double tps)
{
	for (unsigned int i = 0; i < m_scene->mNumAnimations; i++)
	{
		m_animations[i].setTPS(tps);
	}
}
size_t cbs::Model::getAnimationCount() const
{
	return m_scene->mNumAnimations;
}
cbs::Model::Animation * cbs::Model::getAnimation(size_t animation) const
{
	assert(animation < m_scene->mNumAnimations);
	Animation * anim = &m_animations[animation];
	if(anim->m_animation == nullptr) return nullptr;
	return anim;
}
cbs::Model::operator bool()
{
	return m_scene != nullptr;
}
bool cbs::Model::operator !()
{
	return m_scene == nullptr;
}
void cbs::Model::_makeTexture(const char * strName)
{
	// search path
	char strpath[MAX_PATH];
	char * strpathname;
	size_t pathleft;
	{
		const char * pathend1 = strrchr(strName, '\\');
		const char * pathend2 = strrchr(strName, '/');
		size_t len;
		if (pathend1 < pathend2) len = pathend2 - strName + 1;
		else if (pathend1 != nullptr) len = pathend1 - strName + 1;
		else len = 0;
		assert(len < MAX_PATH - 1);
		memcpy(strpath, strName, len);
		strpathname = strpath + len;
		pathleft = MAX_PATH - len - 1;
	}

	if (m_scene->mNumMaterials == 0)
	{
		m_materials.allocate(1);
		Material& mtl = m_materials[0];
		mtl.diffuse = aiColor4D(1.f,1.f,1.f,1.f);
		mtl.specular = aiColor4D(0.f, 0.f, 0.f, 0.f);
		mtl.ambient = aiColor4D(0.2f, 0.2f, 0.2f, 1.f);
		mtl.emissive = aiColor4D(0.f, 0.f, 0.f, 0.f);
		mtl.shininess = 0.f;
		mtl.rasterizer = RasterizerState(D3D11_CULL_BACK, D3D11_FILL_SOLID);
	}
	else
	{
		m_materials.allocate(m_scene->mNumMaterials);
		for (unsigned int m = 0; m < m_scene->mNumMaterials; m++)
		{
			aiMaterial * mtl = m_scene->mMaterials[m];
			Material& mtld = m_materials[m];

			{
				unsigned int cnt = mtl->GetTextureCount(aiTextureType_DIFFUSE);
				mtld.texCount = cnt;

				for (unsigned int t = 0; t < cnt; t++)
				{
					aiString path;
					if(AI_SUCCESS != mtl->GetTexture(aiTextureType_DIFFUSE, t, &path)) continue;

					assert(path.length < pathleft);
					memcpy(strpathname, path.data, path.length + 1);
					mtld.textures[t] = Texture(strpath);
				}
			}


			if (AI_SUCCESS != aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &mtld.diffuse))
			{
				mtld.diffuse = aiColor4D(1.f, 1.f, 1.f, 1.f);
			}
			if (AI_SUCCESS != aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &mtld.specular))
			{
				mtld.specular = aiColor4D(0.f, 0.f, 0.f, 0.f);
			}
			if (AI_SUCCESS != aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &mtld.ambient))
			{
				mtld.ambient = aiColor4D(0.2f, 0.2f, 0.2f, 1.f);
			}
			if (AI_SUCCESS != aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &mtld.emissive))
			{
				mtld.emissive = aiColor4D(0.f, 0.f, 0.f, 0.f);
			}

			unsigned int max;
			{
				float shininess, strength;
				if (AI_SUCCESS == aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &(max = 1)))
				{
					if (AI_SUCCESS == aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS_STRENGTH, &strength, &(max = 1)))
						shininess *= strength;
				}
				else
				{
					shininess = 0.f;
				}
				mtld.shininess = shininess;
			}

			D3D11_FILL_MODE fillmode;
			D3D11_CULL_MODE cullmode;
			{
				int wireframe;
				if (AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, &(max = 1)))
				{
					fillmode = wireframe ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID;
				}
				else
				{
					fillmode = D3D11_FILL_SOLID;
				}
			}


			{
				int twosided;
				if (AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED, &twosided, &(max = 1)))
				{
					cullmode = twosided ? D3D11_CULL_NONE : D3D11_CULL_BACK;
				}
				else
				{
					cullmode = D3D11_CULL_BACK;
				}
			}

			mtld.rasterizer = RasterizerState(cullmode, fillmode);
		}
	}

}
void cbs::Model::_makeBuffer()
{
	char * vbuffer;
	vindex_t * ibuffer;

	#pragma region 뼈대 이름 및 ID 지정
	std::map<std::string, aiNode*> nodemap;
	size_t id = 0;
	_callEachNode(m_scene->mRootNode, [&](aiNode * node) {
		if (node->mName.length != 0)
		{
			node->mAttachment = new NodeExtra(id++);
			nodemap[node->mName.data] = node;
		}
		else if (node->mNumMeshes != 0)
		{
			node->mAttachment = new NodeExtra(id++);
		}
	});
	m_nodeCount = id;
	#pragma endregion

	#pragma region 메쉬 전처리
	{
		m_meshes = new MeshExtra[m_scene->mNumMeshes];
		UINT icount = 0;
		UINT vsize = 0;
		UINT facecount = 0;
		
		for (size_t i = 0; i < m_scene->mNumMeshes; i++)
		{
			MeshExtra& region = m_meshes[i];
			const aiMesh * mesh = m_scene->mMeshes[i];

			UINT stride = sizeof(aiVector3D);
			if (mesh->mNormals != nullptr)
			{
				stride += sizeof(aiVector3D);
			}
			for (size_t ti = 0; ti < AI_MAX_NUMBER_OF_TEXTURECOORDS; ti++)
			{
				if (mesh->mTextureCoords[ti] == nullptr) continue;
				stride += sizeof(float) * mesh->mNumUVComponents[ti];
			}
						
			for (unsigned int ci = 0; ci < AI_MAX_NUMBER_OF_COLOR_SETS; ci++)
			{
				if (mesh->mColors[ci] == nullptr) continue;
				stride += sizeof(aiColor4D);
			}

			if (mesh->mBones != nullptr)
			{
				stride += sizeof(BoneWeight);
			}
			region.stride = stride;
			region.ioffset = icount;

			unsigned int faceSize;
			switch (mesh->mPrimitiveTypes)
			{
			case aiPrimitiveType_POINT:
				faceSize = 1;
				region.topology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
				break;
			case aiPrimitiveType_LINE:
				faceSize = 2;
				region.topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
				break;
			case aiPrimitiveType_TRIANGLE:
				faceSize = 3;
				region.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
				break;
			//case aiPrimitiveType_POLYGON:
			default:
				faceSize = 3;
				region.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
				OutputDebugString(L"모델 파일이 폴리곤을 필요로 하지만, 지원되지 않습니다.\r\n");
				DebugBreak();
				break;
			}
			icount += region.icount = mesh->mNumFaces * faceSize;

			region.offset = vsize;
			vsize += mesh->mNumVertices * stride;
		}

		size_t vfullsize = vsize + icount * sizeof(vindex_t);
		vbuffer = new char[vfullsize];
		memset(vbuffer, 0, vfullsize);
		ibuffer = (vindex_t*)(vbuffer + vsize);
	}
	#pragma endregion

	#pragma region 메쉬 후처리
	char * vptr = vbuffer;
	vindex_t * iptr = ibuffer;
	AutoDelete<char> __autodel = vbuffer;

	{
		UINT voffset = 0;
		UINT ioffset = 0;
		for (size_t i = 0; i < m_scene->mNumMeshes; i++)
		{
			MeshExtra& meshex = m_meshes[i];
			const aiMesh * mesh = m_scene->mMeshes[i];
			UINT stride = meshex.stride;

			unsigned int vcount = mesh->mNumVertices;
			unsigned int vsize = vcount * stride;
			voffset += vsize;

			// fill index
			unsigned int faceSize;
			switch (mesh->mPrimitiveTypes)
			{
			case aiPrimitiveType_POINT: faceSize = 1; break;
			case aiPrimitiveType_LINE: faceSize = 2;  break;
			case aiPrimitiveType_TRIANGLE: faceSize = 3; break;
			//case aiPrimitiveType_POLYGON:
			default: faceSize = 3; break;
			}
			ioffset += faceSize * mesh->mNumFaces;
			for (unsigned int fi = 0; fi < mesh->mNumFaces; fi++)
			{
				const aiFace * face = &mesh->mFaces[fi];
				for (unsigned int j = 0; j < faceSize; j++)
				{
					unsigned int idx = face->mIndices[j];
					assert(idx <= (1 << sizeof(vindex_t) * 8) - 1);
					*iptr++ = (vindex_t)idx;
				}
			}

			// fill vertex
			for (unsigned int vi = 0; vi < vcount; vi++)
			{
				*((aiVector3D*)vptr) = mesh->mVertices[vi];
				vptr += stride;
			}
			vptr -= vsize;
			vptr += sizeof(aiVector3D);

			// fill normal
			if (mesh->mNormals != nullptr)
			{
				for (unsigned int vi = 0; vi < vcount; vi++)
				{
					*((aiVector3D*)vptr) = mesh->mNormals[vi];
					vptr += stride;
				}
				vptr -= vsize;
				vptr += sizeof(aiVector3D);
			}

			// fill texcoord
			for (unsigned int ti = 0; ti < AI_MAX_NUMBER_OF_TEXTURECOORDS; ti++)
			{
				aiVector3D * texcoords = mesh->mTextureCoords[ti];
				if (texcoords == nullptr) continue;
				unsigned int tsz = mesh->mNumUVComponents[ti] * sizeof(float);
				for (unsigned int vi = 0; vi < vcount; vi++)
				{
					memcpy(vptr, &texcoords[vi], tsz);
					vptr += stride;
				}
				vptr -= vsize;
				vptr += tsz;
			}

			// fill color
			for (unsigned int ci = 0; ci < AI_MAX_NUMBER_OF_COLOR_SETS; ci++)
			{
				aiColor4D * colors = mesh->mColors[ci];
				if (colors == nullptr) continue;
				for (unsigned int vi = 0; vi < vcount; vi++)
				{
					*((aiColor4D*)vptr) = colors[vi];
					vptr += stride;
				}
				vptr -= vsize;
				vptr += sizeof(aiColor4D);
			}

			// fill bone
			if (mesh->mBones != nullptr)
			{
				meshex.boneToNode.allocate(mesh->mNumBones);
				assert(mesh->mNumBones <= 0xff); // 바이트 범위 제한
				for (unsigned int bi = 0; bi < mesh->mNumBones; bi++)
				{
					aiBone * bone = mesh->mBones[bi];
					auto res = nodemap.find(bone->mName.data);
					if (res == nodemap.end())
					{
						OutputDebugString(L"노드 이름을 찾을 수 없습니다.\r\n");
						DebugBreak();
						continue;
					}
					meshex.boneToNode[bi] = ((NodeExtra*)res->second->mAttachment)->getId();

					for (unsigned int wi = 0; wi < bone->mNumWeights; wi++)
					{
						aiVertexWeight &w = bone->mWeights[wi];
						BoneWeight* bw = (BoneWeight*)(vptr + w.mVertexId * stride);
						for (int j = 0; j < AI_VERTEX_WEIGHT_COUNT; j ++ )
						{
							if(bw->blendwgt[j] != 0.f) continue;
							bw->blendidx[j] = (unsigned char)bi;
							bw->blendwgt[j] = w.mWeight;
							break;
						}
					}
				}
				vptr += sizeof(BoneWeight);
			}

			vptr -= stride;
			vptr += vsize;
		}

		m_vb = Buffer(D3D11_BIND_VERTEX_BUFFER, voffset, vbuffer);
		m_ib = Buffer(D3D11_BIND_INDEX_BUFFER, ioffset * sizeof(vindex_t), ibuffer);
	}
	#pragma endregion

	unsigned int animcount = m_scene->mNumAnimations;

	m_animations.allocate(animcount);

	for (unsigned int i = 0; i < animcount; i++)
	{
		aiAnimation * anim = m_scene->mAnimations[i];
		m_animations[i] = Animation(this, i);

		anim->mMeshChannels; // TODO: 메쉬 애니메이션, 정점 단위의 애니메이션 기능

		for (unsigned int c = 0; c < anim->mNumChannels; c++)
		{
			aiNodeAnim * na = anim->mChannels[c];

			auto res = nodemap.find(na->mNodeName.data);
			if (res == nodemap.end())
			{
				OutputDebugString(L"노드 이름을 찾을 수 없습니다.\r\n");
				DebugBreak();
				continue;
			}
			aiNode * node = res->second;
			NodeExtra* nodeex = (NodeExtra*)node->mAttachment;
			if (!nodeex->hasAnimation())
			{
				nodeex->setAnimationCount(animcount);
			}
			nodeex->setAnimation(i,na);
		}
	}

}
template <typename LAMBDA> 
void cbs::Model::_callEachNode(aiNode * node, LAMBDA &lambda)
{
	lambda(node);
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		_callEachNode(node->mChildren[i], lambda);
	}
}

void cbs::ModelRenderer::render(const Model & model, const aiMatrix4x4 & world)
{
	g_context->IASetIndexBuffer(model.m_ib, DXGI_FORMAT_R16_UINT, 0);
	_renderNode(model, model.m_scene->mRootNode, world);
}
void cbs::ModelRenderer::render(const Model & model, const Model::Pose & pose)
{
	g_context->IASetIndexBuffer(model.m_ib, DXGI_FORMAT_R16_UINT, 0);
	_renderNode(model, model.m_scene->mRootNode, pose);
}
void cbs::ModelRenderer::_renderNode(const Model & model, aiNode * node, const aiMatrix4x4 & mParent)
{
	aiMatrix4x4 m = mParent * node->mTransformation;

	if(node->mNumMeshes != 0)
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
void cbs::ModelRenderer::_renderNode(const Model & model, aiNode * node, const Model::Pose & pose)
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
				unsigned int nodeId = meshex.boneToNode[0];
				setWorld(pose.getNodeTransform(nodeId) *mesh->mBones[0]->mOffsetMatrix);
			}
			else
			{
				for (unsigned int bi = 0; bi < mesh->mNumBones; bi++)
				{
					unsigned int nodeId = meshex.boneToNode[bi];
					bones[bi] = (Matrix4x3&)(pose.m_nodeTransform[nodeId] * mesh->mBones[bi]->mOffsetMatrix);
				}
				setBoneWorlds(bones, mesh->mNumBones);
			}
		}
		else
		{
			assert(nodeex != nullptr);
			setWorld(pose.getNodeTransform(nodeex->getId()));
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