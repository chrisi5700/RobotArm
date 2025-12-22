//
// Created by chris on 12/21/25.
//
#include <algorithm>
#include <ranges>
#include <RobotArm/Rendering/RenderQueue.hpp>
void RenderQueue::submit(RenderCommand render_command)
{
	m_render_commands.push_back(render_command);
}
std::vector<std::pair<MeshId, std::vector<InstanceData>>> RenderQueue::get_meshes_batched()
{
	namespace v = std::views;
	namespace r = std::ranges;
	r::sort(m_render_commands, {}, &RenderCommand::mesh_id);
	return m_render_commands
	| v::chunk_by([](const auto& cmd1, const auto& cmd2) {return cmd1.mesh_id == cmd2.mesh_id; })
	| v::transform([](const r::range auto& chunks)
	{
		auto id = chunks.begin()->mesh_id; // Cant be empty since empty chunk -> empty commands
		auto instance_data = chunks | v::transform(&RenderCommand::instance_data) | r::to<std::vector>();
		return std::make_pair(id, std::move(instance_data));
	})
	| v::as_rvalue // Move chunk vectors into new vector
	| r::to<std::vector>();
}
void RenderQueue::clear()
{
	return m_render_commands.clear();
}