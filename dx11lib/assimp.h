#pragma once

#pragma message("이 헤더는 아직 미구현이라네")
#pragma message("만일 스스로 구현하고픈자라면, git에다가 가지 내려서 확장해주면 감사하겠네.")
#pragma message("스스로 구현했으면, Assimp쪽은 네임스페이스 이름을 그대의 이니셜로 변경해주시길 바라네.")
#error 오류라네

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