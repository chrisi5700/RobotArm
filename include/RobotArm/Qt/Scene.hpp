//
// Created by chris on 12/21/25.
//

#ifndef ROBOTARM_SCENE_HPP
#define ROBOTARM_SCENE_HPP
#include "RobotArm/Rendering/Camera.hpp"
#include "RobotArm/Rendering/RenderQueue.hpp"
#include "RobotArm/Simulation/Simulation.hpp"

class Scene
{
	Simulation m_simulation;
	Camera m_camera;

	public:
	Scene() = default;
	void submit_to(RenderQueue& queue) const;
	Camera& get_camera();
	Simulation& get_simulation();
};

#endif // ROBOTARM_SCENE_HPP
