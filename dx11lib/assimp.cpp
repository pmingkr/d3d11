#include "include/cbs/d3d11/assimp.h"
#include "include/cbs/d3d11/device.h"
#include "include/cbs/d3d11/texture.h"
#include "include/cbs/d3d11/state.h"
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <new>
#include <map>

#pragma comment(lib, "assimp.lib")

namespace
{
	template <typename T, typename KEY> T interpolateKey(KEY * begin, unsigned int keynum, double time)
	{
		KEY * end = begin + keynum - 1;
		KEY * l = begin;
		KEY * r = end;

		for (;;)
		{
			KEY * c = (r - l) / 2 + l;
			if (c->mTime < time)
			{
				l = c + 1;
			}
			else
			{
				r = c - 1;
			}
			if (l < r) continue;
			if (r == l)
			{
				l = r + 1;
			}
			else if (r <= begin)
			{
				return begin->mValue;
			}
			if (l >= end)
			{
				return end->mValue;
			}
			T out;
			::Assimp::Interpolator<T> inter;
			inter(out, r->mValue, l->mValue, (float)((time - l->mTime) / (r->mTime - l->mTime)));
			return out;
		}
	}
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

cbs::Model::Model()
{
	m_scene = nullptr;
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
			delete [] ((aiNodeAnim**)node->mAttachment);
		});
		aiReleaseImport(m_scene);
	}
}
cbs::Model::Model(Model&& _move)
{
	m_meshes = std::move(_move.m_meshes);
	m_materials = std::move(_move.m_materials);
	m_vb = std::move(_move.m_vb);
	m_ib = std::move(_move.m_ib);
	m_scene = _move.m_scene;
	_move.m_scene = nullptr;
}
cbs::Model & cbs::Model::operator =(Model && _move)
{
	this->~Model();
	new(this) cbs::Model(std::move(_move));
	return *this;
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
		m_materials.resize(1);
		Material& mtl = m_materials.front();
		mtl.diffuse = aiColor4D(1.f,1.f,1.f,1.f);
		mtl.specular = aiColor4D(0.f, 0.f, 0.f, 0.f);
		mtl.ambient = aiColor4D(0.2f, 0.2f, 0.2f, 1.f);
		mtl.emissive = aiColor4D(0.f, 0.f, 0.f, 0.f);
		mtl.shininess = 0.f;
		mtl.rasterizer = RasterizerState(D3D11_CULL_BACK, D3D11_FILL_SOLID);
	}
	else
	{
		m_materials.resize(m_scene->mNumMaterials);
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

	#pragma region 메쉬 전처리
	{
		m_meshes.resize(m_scene->mNumMeshes);
		UINT icount = 0;
		UINT vsize = 0;
		UINT facecount = 0;
		
		for (size_t i = 0; i < m_meshes.size(); i++)
		{
			Mesh& region = m_meshes[i];
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

		vbuffer = new char[vsize + icount * sizeof(vindex_t)];
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
		for (size_t i = 0; i < m_meshes.size(); i++)
		{
			Mesh& region = m_meshes[i];
			const aiMesh * mesh = m_scene->mMeshes[i];
			UINT stride = region.stride;

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

			vptr -= stride;
			vptr += vsize;
		}

		m_vb = Buffer(D3D11_BIND_VERTEX_BUFFER, voffset, vbuffer);
		m_ib = Buffer(D3D11_BIND_INDEX_BUFFER, ioffset * sizeof(vindex_t), ibuffer);
	}
	#pragma endregion

	std::map<std::string, aiNode*> nodemap;
	_callEachNode(m_scene->mRootNode, [&](aiNode * node) { nodemap[node->mName.data] = node; });

	unsigned int animcount = m_scene->mNumAnimations;
	for (unsigned int i = 0; i < animcount; i++)
	{
		aiAnimation * anim = m_scene->mAnimations[i];
		anim->mMeshChannels; // TODO: 메쉬 애니메이션, 정점 단위의 애니메이션 기능

		for (unsigned int c = 0; c < anim->mNumChannels; c++)
		{
			aiNodeAnim * na = anim->mChannels[c];
			auto res = nodemap.find(na->mNodeName.data);
			if (res == nodemap.end())
			{
				OutputDebugString(L"애니메이션으로 움직일 노드를 찾을 수 없습니다.\r\n");
				DebugBreak();
				continue;
			}
			
			aiNode* node = res->second;
			aiNodeAnim ** &nodeanim = (aiNodeAnim **&)node->mAttachment;
			if (nodeanim == nullptr)
			{
				nodeanim = new aiNodeAnim*[animcount];
				memset(nodeanim, 0, sizeof(aiNodeAnim*) * animcount);
			}
			nodeanim[i] = na;
			
			//na->mPreState; // TODO: 애니메이션 전의 상태 결정
			//na->mPostState; // TODO: 애니메이션 후의 상태 결정
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
	_renderNode(model, model.m_scene->mRootNode, world);
}
void cbs::ModelRenderer::render(const Model & model, const aiMatrix4x4 & world, size_t animation, double time)
{
	assert(animation < model.m_scene->mNumAnimations); // 애니메이션 번호가 넘어갔는지
	_renderNode(model, model.m_scene->mRootNode, world, animation, time);
}
void cbs::ModelRenderer::_renderNode(const Model & model, aiNode * node, const aiMatrix4x4 & mParent)
{
	aiMatrix4x4 m = mParent * node->mTransformation;

	// update transform
	setWorld(m);

	g_context->IASetIndexBuffer(model.m_ib, DXGI_FORMAT_R16_UINT, 0);

	// draw all meshes assigned to this node
	for (unsigned int n = 0; n < node->mNumMeshes; ++n)
	{
		const aiMesh* mesh = model.m_scene->mMeshes[node->mMeshes[n]];
		const Model::Mesh& vbregion = model.m_meshes[node->mMeshes[n]];
		const cbs::Material& mtl = model.m_materials[mesh->mMaterialIndex];

		g_context->IASetVertexBuffers(0, 1, &model.m_vb, &vbregion.stride, &vbregion.offset);
		g_context->RSSetState(mtl.rasterizer);
		setMaterial(mtl);
		g_context->IASetPrimitiveTopology(vbregion.topology);
		g_context->DrawIndexed(vbregion.icount, vbregion.ioffset, 0);
	}

	// draw all children
	for (unsigned int n = 0; n < node->mNumChildren; ++n)
	{
		_renderNode(model, node->mChildren[n], m);
	}
}
void cbs::ModelRenderer::_renderNode(const Model & model, aiNode * node, const aiMatrix4x4 & mParent, size_t animation, double time)
{
	aiMatrix4x4 m = mParent * node->mTransformation;

	// update transform
	setWorld(m);

	g_context->IASetIndexBuffer(model.m_ib, DXGI_FORMAT_R16_UINT, 0);

	if(node->mAttachment != nullptr)
	{
		aiNodeAnim* anim = ((aiNodeAnim**)node->mAttachment)[animation];
		if(anim != nullptr)
		{
			aiVector3D pos = interpolateKey<aiVector3D>(anim->mPositionKeys, anim->mNumPositionKeys, time);
			aiQuaternion quat = interpolateKey<aiQuaternion>(anim->mRotationKeys, anim->mNumRotationKeys, time);
			aiVector3D scale = interpolateKey<aiVector3D>(anim->mScalingKeys, anim->mNumScalingKeys, time);

			aiMatrix4x4 m2;
			m *= aiMatrix4x4::Translation(pos, m2);
			m *= aiMatrix4x4(quat.GetMatrix());
			m *= aiMatrix4x4::Scaling(scale, m2);
		}
	}

	// draw all meshes assigned to this node
	for (unsigned int n = 0; n < node->mNumMeshes; ++n)
	{
		const aiMesh* mesh = model.m_scene->mMeshes[node->mMeshes[n]];
		const Model::Mesh& vbregion = model.m_meshes[node->mMeshes[n]];
		const cbs::Material& mtl = model.m_materials[mesh->mMaterialIndex];
		
		//mesh->mBones;

		mesh->mNumBones;
		
		g_context->IASetVertexBuffers(0, 1, &model.m_vb, &vbregion.stride, &vbregion.offset);
		g_context->RSSetState(mtl.rasterizer);
		setMaterial(mtl);
		g_context->IASetPrimitiveTopology(vbregion.topology);
		g_context->DrawIndexed(vbregion.icount, vbregion.ioffset, 0);
	}

	// draw all children
	for (unsigned int n = 0; n < node->mNumChildren; ++n)
	{
		_renderNode(model, node->mChildren[n], m, animation, time);
	}
}