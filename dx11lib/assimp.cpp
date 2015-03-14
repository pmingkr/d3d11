#include "include/cbs/d3d11/assimp.h"
#include "include/cbs/d3d11/device.h"
#include "include/cbs/d3d11/texture.h"
#include "include/cbs/d3d11/state.h"

#include <new>
#include <map>

#pragma comment(lib, "assimp.lib")

namespace
{
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

		void _getAnimation(aiNode * node, const XMMATRIX & mParent)
		{
			cbs::Model::NodeExtra* nodeex = (cbs::Model::NodeExtra*)node->mAttachment;

			XMMATRIX m = mParent;

			if (nodeex != nullptr)
			{
				aiNodeAnim * anim = nodeex->getAnimation(m_index);
				if (anim != nullptr)
				{
					aiVector3D pos = interpolateKey<aiVector3D>(anim->mPositionKeys, anim->mNumPositionKeys, anim->mPreState, anim->mPostState);
					aiQuaternion quat = interpolateKey<aiQuaternion>(anim->mRotationKeys, anim->mNumRotationKeys, anim->mPreState, anim->mPostState);
					aiVector3D scale = interpolateKey<aiVector3D>(anim->mScalingKeys, anim->mNumScalingKeys, anim->mPreState, anim->mPostState);

					XMVECTOR xmquat;
					xmquat.m128_f32[0] = quat.x;
					xmquat.m128_f32[1] = quat.y;
					xmquat.m128_f32[2] = quat.z;
					xmquat.m128_f32[3] = quat.w;
					
					m *= XMMatrixTranspose(XMMatrixTranslation(pos.x, pos.y, pos.z));
					m *= XMMatrixTranspose(XMMatrixRotationQuaternion(xmquat));
					m *= XMMatrixScaling(scale.x, scale.y, scale.z);
				}
				m_pose->setNodeTransform(nodeex->getId(), m);
			}
			else
			{
				XMMATRIX tm;
				(aiMatrix4x4&)tm = node->mTransformation;
				m *= tm;
			}

			for (unsigned int n = 0; n < node->mNumChildren; ++n)
			{
				_getAnimation(node->mChildren[n], m);
			}
		}
	};

}

const cbs::VertexLayout cbs::Model::DEFAULT_BASIC_LAYOUT[3] =
{
	VertexLayout(VertexLayout::Position, DXGI_FORMAT_R32G32B32_FLOAT),
	VertexLayout(VertexLayout::Normal, DXGI_FORMAT_R32G32B32_FLOAT),
	VertexLayout(VertexLayout::Texcoord, DXGI_FORMAT_R32G32_FLOAT),
};
const cbs::VertexLayout cbs::Model::DEFAULT_SKINNED_LAYOUT[5] = 
{
	VertexLayout(VertexLayout::Position, DXGI_FORMAT_R32G32B32_FLOAT),
	VertexLayout(VertexLayout::Normal, DXGI_FORMAT_R32G32B32_FLOAT),
	VertexLayout(VertexLayout::Texcoord, DXGI_FORMAT_R32G32_FLOAT),
	VertexLayout(VertexLayout::BlendIndices, DXGI_FORMAT_R8G8B8A8_UINT),
	VertexLayout(VertexLayout::BlendWeight, DXGI_FORMAT_R32G32B32A32_FLOAT),
};

cbs::AssimpLogger::AssimpLogger()
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
cbs::AssimpLogger::~AssimpLogger()
{
	aiDetachAllLogStreams();
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

	reader._getAnimation(status->animation->m_root, XMMatrixIdentity());
	status->time = time;
	return true;
}
void cbs::Model::Pose::_resize(size_t size)
{
	m_transforms.resizeWithoutKeep(size);
}

void cbs::Model::_create(const char * strName, DataList<VertexLayout> basic_vl, DataList<VertexLayout> skinned_vl)
{
	m_scene = aiImportFile(strName, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_FlipUVs | aiProcess_SortByPType | aiProcess_LimitBoneWeights);
	if (m_scene == nullptr)
	{
		wchar_t temp[1024];
		swprintf_s(temp, L"%hs 모델을 찾을 수 없습니다.\r\n", strName);
		MessageBox(nullptr, temp, nullptr, MB_OK | MB_ICONERROR);
		return;
	}

	_makeTexture(strName);
	_makeBuffer(basic_vl, skinned_vl);
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
		mtl.rasterizer = RasterizerState(D3D11_CULL_BACK, D3D11_FILL_SOLID, true);
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

			mtld.rasterizer = RasterizerState(cullmode, fillmode, true);
		}
	}

}
void cbs::Model::_makeBuffer(DataList<VertexLayout> basic_vl, DataList<VertexLayout> skinned_vl)
{
	char * vbuffer;
	vindex_t * ibuffer;

	unsigned int basic_stride = VertexLayout::getFullSize(basic_vl);
	unsigned int skinned_stride = VertexLayout::getFullSize(skinned_vl);


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

	#pragma region 메쉬 처리 1 Pass
	{
		m_meshes = new MeshExtra[m_scene->mNumMeshes];
		UINT icount = 0;
		UINT vsize = 0;
		UINT facecount = 0;
		
		for (size_t i = 0; i < m_scene->mNumMeshes; i++)
		{
			MeshExtra& meshex = m_meshes[i];
			const aiMesh * mesh = m_scene->mMeshes[i];

			meshex.skinned = (mesh->mBones != nullptr);
			meshex.stride = (meshex.skinned ? skinned_stride : basic_stride);
			meshex.ioffset = icount;

			unsigned int faceSize;
			switch (mesh->mPrimitiveTypes)
			{
			case aiPrimitiveType_POINT:
				faceSize = 1;
				meshex.topology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
				break;
			case aiPrimitiveType_LINE:
				faceSize = 2;
				meshex.topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
				break;
			case aiPrimitiveType_TRIANGLE:
				faceSize = 3;
				meshex.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
				break;
			//case aiPrimitiveType_POLYGON:
			default:
				faceSize = 3;
				meshex.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
				OutputDebugString(L"모델 파일이 폴리곤을 필요로 하지만, 지원되지 않습니다.\r\n");
				DebugBreak();
				break;
			}
			icount += meshex.icount = mesh->mNumFaces * faceSize;

			meshex.offset = vsize;
			vsize += mesh->mNumVertices * meshex.stride;
		}

		size_t vfullsize = vsize + icount * sizeof(vindex_t);
		vbuffer = new char[vfullsize];
		memset(vbuffer, 0, vfullsize);
		ibuffer = (vindex_t*)(vbuffer + vsize);
	}
	#pragma endregion

	#pragma region 메쉬 처리 2 Pass
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

			unsigned int stride = meshex.stride;
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

			char * ptr_blendwgt = nullptr, * ptr_blendidx = nullptr;
			DataList<VertexLayout> vls = meshex.skinned ? skinned_vl : basic_vl;
			for (size_t vli = 0; vli < vls.size(); vli++)
			{
				const VertexLayout & vl = vls[vli];

				switch (vl.semantic)
				{
				case VertexLayout::Position:
					assert(vl.format == DXGI_FORMAT_R32G32B32_FLOAT); // 다른 타입 미지원
					if (mesh->mVertices != nullptr)
					{
						for (unsigned int vi = 0; vi < vcount; vi++)
						{
							*((aiVector3D*)vptr) = mesh->mVertices[vi];
							vptr += stride;
						}
						vptr -= vsize;
					}
					vptr += sizeof(aiVector3D);
					break;
				case VertexLayout::Normal:
					assert(vl.format == DXGI_FORMAT_R32G32B32_FLOAT); // 다른 타입 미지원
					if (mesh->mNormals != nullptr)
					{
						for (unsigned int vi = 0; vi < vcount; vi++)
						{
							*((aiVector3D*)vptr) = mesh->mNormals[vi];
							vptr += stride;
						}
						vptr -= vsize;
					}
					vptr += sizeof(aiVector3D);
					break;
				case VertexLayout::Texcoord:
					{
						assert(vl.slot < AI_MAX_NUMBER_OF_TEXTURECOORDS);
						aiVector3D * texcoords = mesh->mTextureCoords[vl.slot];
						VertexFormatInfo vinfo(vl.format);
						assert(vinfo.elementType == VertexFormatInfo::Float); // 다른 타입 미지원
						assert(vinfo.special == VertexFormatInfo::Normal);
						assert(vinfo.elementSize == sizeof(float));
						if (texcoords != nullptr)
						{
							unsigned int tsz = mesh->mNumUVComponents[vl.slot] * sizeof(float);
							if(tsz > vinfo.byteSize) tsz = vinfo.byteSize;
							for (unsigned int vi = 0; vi < vcount; vi++)
							{
								memcpy(vptr, &texcoords[vi], tsz);
								vptr += stride;
							}
							vptr -= vsize;
						}
						vptr += vinfo.byteSize;
						break;
					}
				case VertexLayout::Color:
					{
						assert(vl.slot < AI_MAX_NUMBER_OF_COLOR_SETS);
						assert(vl.format == DXGI_FORMAT_R32G32B32A32_FLOAT); // 다른 타입 미지원
						aiColor4D * colors = mesh->mColors[vl.slot];
						if (colors != nullptr)
						{
							for (unsigned int vi = 0; vi < vcount; vi++)
							{
								*((aiColor4D*)vptr) = colors[vi];
								vptr += stride;
							}
							vptr -= vsize;
						}
						vptr += sizeof(aiColor4D);
						break;
					}
				case VertexLayout::BlendWeight:
					assert(vl.format == DXGI_FORMAT_R32G32B32A32_FLOAT); // 다른 타입 미지원
					ptr_blendwgt = vptr;
					vptr += sizeof(float) * 4;
					break;
				case VertexLayout::BlendIndices:
					assert(vl.format == DXGI_FORMAT_R8G8B8A8_UINT); // 다른 타입 미지원
					ptr_blendidx = vptr;
					vptr += sizeof(unsigned char) * 4;
					break;
				default:
					assert(!"Unsupported"); // 미지원 시멘틱
				}
			}

			vptr -= stride;
			vptr += vsize;

			// fill bone
			if (mesh->mBones != nullptr && 
				ptr_blendwgt != nullptr && 
				ptr_blendidx != nullptr)
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
						float* pwgt				= (float*)(ptr_blendwgt + w.mVertexId * stride);
						unsigned char * pidx	= (unsigned char *)(ptr_blendidx + w.mVertexId * stride);
						for (int j = 0; j < AI_VERTEX_WEIGHT_COUNT; j ++ )
						{
							if(pwgt[j] != 0.f) continue;
							pidx[j] = (unsigned char)bi;
							pwgt[j] = w.mWeight;
							break;
						}
					}
				}
			}
		}

		m_vb = Buffer(D3D11_BIND_VERTEX_BUFFER, AnyData(vbuffer, voffset));
		m_ib = Buffer(D3D11_BIND_INDEX_BUFFER, AnyData(ibuffer, ioffset * sizeof(vindex_t)));
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
