#ifndef GP_GAME_H
#define GP_GAME_H

#include <pthread.h>
#include "common/messages/Message.h"
#include "common/messages/Queue.h"
#include "gui/CameraControl.h"
#include "gui/ScenarioControl.h"
#include "gui/graphics/Render.h"

class GLFWwindow;
namespace gp
{
struct EngineData
{
  gp::messages::Queue<gp::messages::Message>& vis2engine;
  gp::messages::Queue<gp::messages::Message>& engine2vis;
};
void* runEngine(void* data);

/**
 * @brief Main class
 *
 * Holds everything together
 */
class Game
{
private:
	GLFWwindow* m_window;
	pthread_t m_engineThread;
	gp::messages::Queue<gp::messages::Message> m_vis2engine;
	gp::messages::Queue<gp::messages::Message> m_engine2vis;

	gp::graphics::Render m_render;
	gp::graphics::TextureManager m_textureMan;

	gp::gui::ScenarioControl m_scenarioControl;
	gp::gui::CameraControl m_cameraControl;


	struct WindowPointer {
		gp::graphics::Camera* camera;
		nanogui::Screen* screen;
	};
	WindowPointer wp;

	void setupNewScenario();
	void setupCallbacks();

public:
	Game(GLFWwindow* window);
	~Game();

  void run();
};

}

#endif // GP_GAME_H
