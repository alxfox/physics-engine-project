#ifndef GP_GUI_SCENARIOCONTROL_H
#define GP_GUI_SCENARIOCONTROL_H

#include "Scenario.h"
#include <queue>
#include <nanogui/nanogui.h>
#include "common/messages/Message.h"
#include "common/messages/Queue.h"

struct GLFWwindow;

namespace gp
{

namespace gui
{

class ScenarioControl : public nanogui::Screen
{
private:
	/** The current scenario */
	gp::Scenario* m_scenario = nullptr;

	/** The next scenario (or null if the scenario should not change) */
	gp::Scenario* m_scenarioCandidate = nullptr;

	/** The pause button */
	nanogui::Button* m_playPause = nullptr;

	/** The next frame button */
	nanogui::Button* m_nextFrame = nullptr;

	/** <code>True</code> if the physics engine is paused */
	bool m_paused = false;

	/** The outgoing queue to the physics engine */
	gp::messages::Queue<gp::messages::Message>& m_vis2engine;



	/**
	 * Callback for the step button
	 */
	void stepEngine();

public:

	/**
	 * Callback for the pause button
	 */
	void toggleEngine();

	bool enableConstraintsRendering = true;

	nanogui::Button* m_g1 = nullptr;
	nanogui::Button* m_g2 = nullptr;
	nanogui::Button* m_g3 = nullptr;
	nanogui::Button* m_restart3 = nullptr;
	nanogui::Window* m_aimingReticleHor = nullptr;
	nanogui::Window* m_aimingReticleVer = nullptr;
	nanogui::Window* m_nanoguiWindow = nullptr;
	nanogui::Window* m_scoringboard = nullptr;
	nanogui::Window* m_endmenu = nullptr;
	nanogui::Label* m_scoreLabel = nullptr; 
	nanogui::Label* m_lifeLabel = nullptr; 
	nanogui::Label* m_moobsLabel = nullptr; 
	nanogui::Label* m_levelLabel = nullptr; 
	nanogui::Label* m_winlose = nullptr;
	nanogui::Label* m_space = nullptr;
	nanogui::Label* m_endscore = nullptr;





	int16_t m_score;
	int16_t m_life;
	bool m_reloadedScenario = false;
	
// 	gp::messages::Queue<gp::messages::Message> engineMessages;

	ScenarioControl(GLFWwindow* window, gp::messages::Queue<gp::messages::Message>& vis2engine);
	~ScenarioControl();

	gp::Scenario* peekScenario() const { return m_scenarioCandidate; }
	gp::Scenario* scenario() const { return m_scenario; }
	bool hasNewScenario() const { return m_scenarioCandidate != nullptr; }

	template<typename T>
	void loadScenario() { m_scenarioCandidate = new T; }
	void swapScenario();

	virtual bool keyboardEvent(int key, int scancode, int action, int modifiers);

	bool interactsWithMouse();
	bool isPaused();
};

}

}

#endif // GP_GUI_SCENARIOCONTROL_H
