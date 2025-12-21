//
// Created by chris on 12/21/25.
//

#ifndef ROBOTARM_RENDERQUEUE_HPP
#define ROBOTARM_RENDERQUEUE_HPP
#include "GLCommon.hpp"


struct RenderCommand
{
	MeshId mesh_id;
	InstanceData instance_data;
};
class RenderQueue
{
	std::vector<RenderCommand> m_render_commands;
public:
	void submit(RenderCommand render_command);
	std::vector<std::pair<MeshId, std::vector<InstanceData>>> get_meshes_batched();
	void clear();
};


#endif // ROBOTARM_RENDERQUEUE_HPP
