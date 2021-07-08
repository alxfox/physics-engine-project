#include "ScenarioControl.h"
#include <GLFW/glfw3.h>
#include "Scenarios.h"
#include "CustomScenarios.h"
#include "engine/messages/ControlMessage.h"

using namespace nanogui;

gp::gui::ScenarioControl::ScenarioControl(GLFWwindow* window, gp::messages::Queue<gp::messages::Message>& vis2engine)
	: m_vis2engine(vis2engine)
{
	initialize(window, true);

	FormHelper *gui = new FormHelper(this);
	ref<Window> nanoguiWindow = gui->addWindow(Eigen::Vector2i(10, 10), "Menu");

  gui->addGroup("Choose difficulty");
//   gui->addButton("Scenario 1", [this]() { loadScenario<gp::Scenario1>(); });
//   gui->addButton("Box box test", [this]() { loadScenario<gp::BoxBoxTest>(); });
//   gui->addButton("Many spheres", [this]() { loadScenario<gp::ManySpheres>(); });
//   gui->addButton("Many cubes", [this]() { loadScenario<gp::ManyCubes>(); });
//   gui->addButton("Hanging bridge", [this]() { loadScenario<gp::HangingBridge>(); });
//   gui->addButton("Pendulum", [this]() { loadScenario<gp::Pendulum>(); });
  gui->addButton("Easy peasy", [this]() { loadScenario<gp::Custom1>(); });
  gui->addButton("Normal", [this]() { loadScenario<gp::Custom2>(); });
  gui->addButton("Insane", [this]() { loadScenario<gp::Custom3>(); });

	gui->addGroup("Pause Game (P)");
	Widget* simulation = new Widget(nanoguiWindow.get());
	simulation->setLayout(new BoxLayout(Orientation::Horizontal, Alignment::Middle));
	m_playPause = new Button(simulation, "");
	m_playPause->setIcon(ENTYPO_ICON_PAUS);
	m_playPause->setCallback(std::bind(&ScenarioControl::toggleEngine, this));
	// m_nextFrame = new Button(simulation, "");
	// m_nextFrame->setIcon(ENTYPO_ICON_TO_END);
	// m_nextFrame->setEnabled(false);
	// m_nextFrame->setCallback(std::bind(&ScenarioControl::stepEngine, this));
	gui->addWidget("", simulation);

	// gui->addGroup("Render options");
	// gui->addVariable("Ropes", enableConstraintsRendering);

	nanogui::Label *instructions = new nanogui::Label(nanoguiWindow.get(),
		 "Press W, A, S, D to move and Left Mouseclick to shoot. \n \nTry to shoot the flying objects as far away from you as possible so that they disappear. \n \n Press H to hide the Menue.");
	instructions->setFixedWidth(150);
	gui->addGroup("Instructions");
	gui->addWidget("", instructions);

	setVisible(true);
	performLayout();
}



gp::gui::ScenarioControl::~ScenarioControl()
{
	delete m_scenarioCandidate;
	delete m_scenario;
}

void gp::gui::ScenarioControl::toggleEngine()
{
	m_paused = !m_paused;
	if (!m_paused) {
		m_playPause->setIcon(ENTYPO_ICON_PAUS);
	} else {
		m_playPause->setIcon(ENTYPO_ICON_PLAY);
	}
	// m_nextFrame->setEnabled(m_paused);
	m_vis2engine.push(gp::engine::messages::ControlMessage(m_paused));
}

void gp::gui::ScenarioControl::stepEngine()
{
	if (m_paused) {
		m_vis2engine.push(gp::engine::messages::StepMessage());
	}
}

void gp::gui::ScenarioControl::swapScenario()
{
	delete m_scenario;
	m_scenario = m_scenarioCandidate;
	m_scenarioCandidate = nullptr;
}

bool gp::gui::ScenarioControl::keyboardEvent(int key, int scancode, int action, int modifiers)
{
	bool handled = Screen::keyboardEvent(key, scancode, action, modifiers);
	if (!handled) {
		if (key == GLFW_KEY_P && action == GLFW_PRESS) {
			toggleEngine();
			handled = true;
		} else if (key == GLFW_KEY_COMMA && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
			stepEngine();
			handled = true;
		} else if (key == GLFW_KEY_H && action == GLFW_PRESS) {
			Widget::setVisible(!visible());
			handled = true;
		}
	}
	return handled;
}

bool gp::gui::ScenarioControl::interactsWithMouse()
{
	nanogui::Widget const* widget = findWidget(mousePos());
	return widget != nullptr && widget != this;
}
