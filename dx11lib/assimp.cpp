#if 0

#include "assimp.h"
#include "device.h"
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#if 0
// TODO: DX11로 구현
// ----------------------------------------------------------------------------
void apply_material(const struct aiMaterial *mtl)
{
	float c[4];

	GLenum fill_mode;
	int ret1, ret2;
	struct aiColor4D diffuse;
	struct aiColor4D specular;
	struct aiColor4D ambient;
	struct aiColor4D emission;
	float shininess, strength;
	int two_sided;
	int wireframe;
	unsigned int max;

	set_float4(c, 0.8f, 0.8f, 0.8f, 1.0f);
	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
		color4_to_float4(&diffuse, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c);

	set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specular))
		color4_to_float4(&specular, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);

	set_float4(c, 0.2f, 0.2f, 0.2f, 1.0f);
	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambient))
		color4_to_float4(&ambient, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c);

	set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &emission))
		color4_to_float4(&emission, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, c);

	max = 1;
	ret1 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max);
	if (ret1 == AI_SUCCESS) {
		max = 1;
		ret2 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS_STRENGTH, &strength, &max);
		if (ret2 == AI_SUCCESS)
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess * strength);
		else
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	}
	else {
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
		set_float4(c, 0.0f, 0.0f, 0.0f, 0.0f);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
	}

	max = 1;
	if (AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, &max))
		fill_mode = wireframe ? GL_LINE : GL_FILL;
	else
		fill_mode = GL_FILL;
	glPolygonMode(GL_FRONT_AND_BACK, fill_mode);

	max = 1;
	if ((AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED, &two_sided, &max)) && two_sided)
		glDisable(GL_CULL_FACE);
	else
		glEnable(GL_CULL_FACE);
}
#endif

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

cbs::Model::Model()
{
	m_scene = nullptr;
}
cbs::Model::Model(const char * strName)
{
	m_scene = aiImportFile(strName, aiProcessPreset_TargetRealtime_MaxQuality);
}
cbs::Model::~Model()
{
	if (m_scene != nullptr)
	{
		aiReleaseImport(m_scene);
	}
}

void cbs::Model::render(const aiMatrix4x4 & mParent)
{
	_renderNode(m_scene->mRootNode, mParent);
}
void cbs::Model::_renderNode(aiNode * node, const aiMatrix4x4 & mParent)
{
	unsigned int i;
	unsigned int n = 0, t;
	aiMatrix4x4 m = node->mTransformation;

	// update transform
	aiMultiplyMatrix4(&m, &mParent);

	//

	// draw all meshes assigned to this node
	for (; n < node->mNumMeshes; ++n) {
		const aiMesh* mesh = m_scene->mMeshes[node->mMeshes[n]];

		// TODO: 메터리얼 기능 구현
		//apply_material(m_scene->mMaterials[mesh->mMaterialIndex]);

		// TODO: 광원 기능 구현
		if (mesh->mNormals == NULL) {
			//glDisable(GL_LIGHTING);
		}
		else {
			//glEnable(GL_LIGHTING);
		}

		for (t = 0; t < mesh->mNumFaces; ++t) {
			const struct aiFace* face = &mesh->mFaces[t];
		
			D3D11_PRIMITIVE_TOPOLOGY topology;

			switch (face->mNumIndices) {
			case 1: topology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST; break;
			case 2: topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST; break;
			case 3: topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST; break;
			default:
				// POLYGON MODE
				// DX11에 폴리곤 모드 따위는 벗ㅇ다.
				DebugBreak();
				break;
			}

			g_context->IASetPrimitiveTopology(topology);

			// TODO: 버텍스 버퍼 구현
			for (i = 0; i < face->mNumIndices; i++) {
				int index = face->mIndices[i];
				if (mesh->mColors[0] != NULL) {}
					//glColor4fv((GLfloat*)&mesh->mColors[0][index]);
				if (mesh->mNormals != NULL) {}
					//glNormal3fv(&mesh->mNormals[index].x);
				//glVertex3fv(&mesh->mVertices[index].x);
			}
		}

	}

	// draw all children
	for (n = 0; n < node->mNumChildren; ++n) {
		_renderNode(node->mChildren[n], m);
	}
}

#endif