#pragma once

#pragma message("�� ����� ���� �̱����̶��")
#pragma message("���� ������ �����ϰ����ڶ��, git���ٰ� ���� ������ Ȯ�����ָ� �����ϰڳ�.")
#pragma message("������ ����������, Assimp���� ���ӽ����̽� �̸��� �״��� �̴ϼȷ� �������ֽñ� �ٶ��.")
#error �������

#include "common.h"

struct aiScene;
struct aiNode;

#include <assimp/types.h>

namespace cbs
{
	class Assimp
	{
	public:
		Assimp();
	};
	
	class Model
	{
	public:
		Model();
		Model(const char * strName);
		~Model();

		void render(const aiMatrix4x4 & mParent);

	private:

		void _renderNode(aiNode * node, const aiMatrix4x4 & mParent);

		const aiScene* m_scene;

	};
}