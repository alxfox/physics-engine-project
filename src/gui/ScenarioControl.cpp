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

	int width, height;
	glfwGetWindowSize(window, &width, &height);

//=============================================AIMING CROSS=========================================================

	FormHelper *aimingReticleHor = new FormHelper(this);
	m_aimingReticleHor = aimingReticleHor->addWindow(Eigen::Vector2i(width/2.0f-20, height/2.0f), "");
	m_aimingReticleHor->setFixedWidth(20);
	m_aimingReticleHor->setFixedHeight(-1);
	FormHelper *aimingReticleVer = new FormHelper(this);
	m_aimingReticleVer = aimingReticleVer->addWindow(Eigen::Vector2i(width/2.0f-10, height/2.0f-10), "");
	m_aimingReticleVer->setFixedWidth(-1.f);
	m_aimingReticleVer->setFixedHeight(18);
	NVGcontext* cont = this->nvgContext();
	Theme* base = new Theme(cont);
	m_aimingReticleHor->setTheme(base);
	m_aimingReticleVer->setTheme(base);
	m_aimingReticleHor->theme()->mWindowDropShadowSize = 10;
	m_aimingReticleHor->theme()->mWindowCornerRadius = 4;
	m_aimingReticleHor->theme()->mWindowFillUnfocused = Color(engine::Vector4f(255.0f, 255.0f, 255.0f, 255.0f));
	m_aimingReticleVer->theme()->mWindowFillUnfocused = Color(engine::Vector4f(255.0f, 255.0f, 255.0f, 255.0f));
	m_aimingReticleHor->theme()->mWindowFillFocused = Color(engine::Vector4f(255.0f, 255.0f, 255.0f, 255.0f));
	m_aimingReticleVer->theme()->mWindowFillFocused = Color(engine::Vector4f(255.0f, 255.0f, 255.0f, 255.0f));

//======================================INTERFACE BOTTOM================================================
	FormHelper *gui = new FormHelper(this);
	m_nanoguiWindow = gui->addWindow(Eigen::Vector2i(1, height-60), "");

	NVGcontext* cont2 = this->nvgContext();
	Theme* base2 = new Theme(cont2);
	m_nanoguiWindow->setTheme(base2);

	m_nanoguiWindow->theme()->mWindowFillUnfocused = Color(engine::Vector4f(.1f, 0.33f, 0.66f, .7f));
	m_nanoguiWindow->theme()->mWindowFillFocused = Color(engine::Vector4f(.2f, 0.43f, 0.76f, .7f));

	Widget* games = new Widget(m_nanoguiWindow);
	
	//games->setLayout(new GridLayout(Orientation::Horizontal, 3, Alignment::Middle, 0, 50));
	m_g1 = new Button(games, "G1");
	m_g2 = new Button(games, "G2");
	m_g3 = new Button(games, "G3");
	//m_playPause->setPosition(Vector2i(40, 40));
	//m_playPause->setIcon(ENTYPO_ICON_PLAY);
	m_g1->setCallback([this]() { loadScenario<gp::Custom1>(); });
	m_g2->setCallback([this]() { loadScenario<gp::Custom2>(); });
	m_g3->setCallback([this]() { loadScenario<gp::Custom3>(); });//m_nanoguiWindow->setVisible(m_paused); });
	//Widget* instructions = new Widget(games);
	//instructions->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Minimum));
	//nanogui::Label* instr = new Label(instructions, "  Press W, A, S, D to move and Left Mouseclick to shoot.");
	//nanogui::Label* instr1 = new Label(instructions, "  Try to shoot the flying objects as far away from you as possible so that they disappear.");
	//nanogui::Label* instr2 = new Label(instructions, "  Press H to hide the Menue. Press Esc to pause the game");
	games->setFixedWidth(width);
	games->setFixedHeight(height);
	gui->addWidget("",games);

//=====================================Score top right==================================================
	//m_nanoguiWindow->theme()->mWindowFillUnfocused = Color(engine::Vector4f(0.0f, 0.0f, 200.0f, 255.0f));
	//m_nanoguiWindow->theme()->mWindowFillFocused = Color(engine::Vector4f(21.0f, 101.0f, 192.0f, 255.0f));
	 
	m_life = 3;	
	m_score = 0;	

	FormHelper *gui2 = new FormHelper(this);
	m_scoringboard = gui2->addWindow(Eigen::Vector2i(width-150, 1), "");

	Widget* games2 = new Widget(m_scoringboard);
	games2->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Minimum));
	//Widget* points = new Widget(games2);
	//points->setLayout(new BoxLayout(Orientation::Horizontal, Alignment::Minimum));
	m_scoreLabel= new Label(games2, "SCORE: 0                                        " );//+ std::to_string(m_score));

	//Widget* live = new Widget(games2);
	//live->setLayout(new BoxLayout(Orientation::Horizontal, Alignment::Minimum));
	m_lifeLabel= new Label(games2, "LIVES: 3                                        " );//+ std::to_string(m_life));
	games2->setFixedWidth(300);
	games2->setFixedHeight(30);
	gui2->addWidget("",games2);


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
	//if (!m_paused) {
	//	m_playPause->setIcon(ENTYPO_ICON_PAUS);
	//	//===========================================einfügen dass sich das menü öffnet=========================================================

	//} else {
	//	m_playPause->setIcon(ENTYPO_ICON_PLAY);
	//	//===========================================einfügen dass sich das menü schließt=========================================================
	//}
	//m_nextFrame->setEnabled(m_paused);
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
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
			toggleEngine();
			//===========Menu opens,Closes when pressing Pause========================
			m_nanoguiWindow->setVisible(m_paused);
			handled = true;
		} 
		// geting pictur after pictur
		// else if (key == GLFW_KEY_COMMA && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		// 	stepEngine();
		// 	handled = true;
		// }
		 else if (key == GLFW_KEY_H && action == GLFW_PRESS) {
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
bool gp::gui::ScenarioControl::isPaused()
{
	return m_paused;


}
