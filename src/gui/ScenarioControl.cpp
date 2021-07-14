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
	//m_aimingReticle->setSize(Eigen::Vector2i(width/2.0f-20, 100000));
	m_aimingReticleHor->setFixedWidth(20);
	m_aimingReticleHor->setFixedHeight(-1);
	FormHelper *aimingReticleVer = new FormHelper(this);
	m_aimingReticleVer = aimingReticleVer->addWindow(Eigen::Vector2i(width/2.0f-10, height/2.0f-10), "");
	//m_aimingReticle->setSize(Eigen::Vector2i(width/2.0f-20, 100000));
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
	//m_aimingReticle->setHeight(1000);
	//aimingReticle->setSize(Eigen::Vector2i(1, 1));
	//aimingReticleHelper->setFixedSize(Eigen::Vector2i(1, 1));
	//Widget* aimRet = new Widget(aimingReticle.get());
	//aimRet->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Middle));
	//nanogui::Label* aR1 = new Label(aimRet, "  |  ", "sans", 8);
	//nanogui::Label* aR2 = new Label(aimRet, "-- --", "sans", 8);
	//nanogui::Label* aR3 = new Label(aimRet, "  |  ", "sans", 8);
	//aimRet->setFixedWidth(40);
	//aimRet->setFixedHeight(40);
	//aimingReticleHelper->addWidget("", aimRet);

//=====================================Score top right==================================================
// FormHelper *gui = new FormHelper(this);
// 	m_nanoguiWindow = gui->addWindow(Eigen::Vector2i(1, height-60), "");

// FormHelper *aimingReticleHor = new FormHelper(this);
	// m_aimingReticleHor = aimingReticleHor->addWindow(Eigen::Vector2i(width/2.0f-20, height/2.0f), "");

// FormHelper *Scoringboard = new FormHelper(this);


// 	m_scoringboard = Scoringboard->addWindow(Eigen::Vector2i(1, height-60), "");
// Widget* lifeandscore = new Widget(m_scoringboard);
// 	lifeandscore->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Minimum);
// Widget* scoring = new Widget(lifeandscore);
// 	scoring->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Minimum));
// Widget* life = new Widget(lifeandscore);
// 	life->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Minimum);

// nanogui::Label* points = new Label(scoring, "Score: ");
// nanogui::Label* leben = new Label(life, "Lives: ");
// // games->setFixedWidth(width-10);
// // 	games->setFixedHeight(50);
// gui->addWidget("",lifeandscore);

//======================================INTERFACE BOTTOM================================================
	FormHelper *gui = new FormHelper(this);
	m_nanoguiWindow = gui->addWindow(Eigen::Vector2i(1, height-60), "");

	//m_nanoguiWindow->theme()->mWindowFillUnfocused = Color(engine::Vector4f(0.0f, 0.0f, 200.0f, 255.0f));
	//m_nanoguiWindow->theme()->mWindowFillFocused = Color(engine::Vector4f(21.0f, 101.0f, 192.0f, 255.0f));

	Widget* games = new Widget(m_nanoguiWindow);
	games->setLayout(new BoxLayout(Orientation::Horizontal, Alignment::Middle));
	nanogui::Button* g1 = new Button(games, "G1");
	nanogui::Button* g2 = new Button(games, "G2");
	nanogui::Button* g3 = new Button(games, "G3");
	g1->setCallback([this]() { loadScenario<gp::Custom1>(); });
	g2->setCallback([this]() { loadScenario<gp::Custom2>(); });
	g3->setCallback([this]() { loadScenario<gp::Custom3>(); });
	//nanogui::Label* pause = new Label(games, "Pause Game (P)", "sans", 24);
	Widget* instructions = new Widget(games);
	instructions->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Minimum));
	nanogui::Label* instr = new Label(instructions, "  Press W, A, S, D to move and Left Mouseclick to shoot.");
	//nanogui::Label* instr = new Label(instructions, "  Score: " + std::to_string(score) + "Life: " + std::to_string(life));
	nanogui::Label* instr1 = new Label(instructions, "  Try to shoot the flying objects as far away from you as possible so that they disappear.");
	nanogui::Label* instr2 = new Label(instructions, "  Press H to hide the Menue. Press Esc to pause the game");
	games->setFixedWidth(width-10);
	games->setFixedHeight(50);
	//games->theme()->mWindowFillUnfocused = Color(engine::Vector4f(255.0f, 255.0f, 0.0f, 255.0f));
	gui->addWidget("",games);

	m_score = 0;
	m_life = 3;
	

	FormHelper *gui2 = new FormHelper(this);
	m_scoringboard = gui2->addWindow(Eigen::Vector2i(1, height-100), "");

	//m_nanoguiWindow->theme()->mWindowFillUnfocused = Color(engine::Vector4f(0.0f, 0.0f, 200.0f, 255.0f));
	//m_nanoguiWindow->theme()->mWindowFillFocused = Color(engine::Vector4f(21.0f, 101.0f, 192.0f, 255.0f));

	Widget* games2 = new Widget(m_scoringboard);
	games2->setLayout(new BoxLayout(Orientation::Horizontal, Alignment::Middle));
	nanogui::Button* g12 = new Button(games2, "G1");
	nanogui::Button* g22 = new Button(games2, "G2");
	nanogui::Button* g32 = new Button(games2, "G3");
	g12->setCallback([this]() { loadScenario<gp::Custom1>(); });
	g22->setCallback([this]() { loadScenario<gp::Custom2>(); });
	g32->setCallback([this]() { loadScenario<gp::Custom3>(); });
	//nanogui::Label* pause = new Label(games, "Pause Game (P)", "sans", 24);
	Widget* instructions2 = new Widget(games2);
	instructions2->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Minimum));
	nanogui::Label* instr0 = new Label(instructions2, "  Press W, A, S, D to move and Left Mouseclick to shoot.");
	//nanogui::Label* instr = new Label(instructions, "  Score: " + std::to_string(score) + "Life: " + std::to_string(life));
	nanogui::Label* instr12 = new Label(instructions2, "  Try to shoot the flying objects as far away from you as possible so that they disappear.");
	nanogui::Label* instr22 = new Label(instructions2, "  Press H to hide the Menue. Press Esc to pause the game");
	games2->setFixedWidth(width-10);
	games2->setFixedHeight(50);
	//games->theme()->mWindowFillUnfocused = Color(engine::Vector4f(255.0f, 255.0f, 0.0f, 255.0f));
	gui2->addWidget("",games2);
//  gui->setFixedSize(Eigen::Vector2i(width-100, height));
//  gui->addGroup("Choose difficulty");
//   gui->addButton("Scenario 1", [this]() { loadScenario<gp::Scenario1>(); });
//   gui->addButton("Box box test", [this]() { loadScenario<gp::BoxBoxTest>(); });
//   gui->addButton("Many spheres", [this]() { loadScenario<gp::ManySpheres>(); });
//   gui->addButton("Many cubes", [this]() { loadScenario<gp::ManyCubes>(); });
//   gui->addButton("Hanging bridge", [this]() { loadScenario<gp::HangingBridge>(); });
//   gui->addButton("Pendulum", [this]() { loadScenario<gp::Pendulum>(); });
//  gui->addButton("Easy peasy", [this]() { loadScenario<gp::Custom1>(); });
//  gui->addButton("Normal", [this]() { loadScenario<gp::Custom2>(); });
//  gui->addButton("Insane", [this]() { loadScenario<gp::Custom3>(); });

	//gui->addGroup("Pause Game (P)");
	//Widget* simulation = new Widget(nanoguiWindow.get());
	//simulation->setLayout(new BoxLayout(Orientation::Horizontal, Alignment::Middle));
	//simulation->setFixedWidth(100);
	//m_playPause = new Button(simulation, "");
	// m_playPause->setIcon(ENTYPO_ICON_PAUS);
	// m_playPause->setCallback(std::bind(&ScenarioControl::toggleEngine, this));
	 //m_nextFrame = new Button(simulation, "");
	 //m_nextFrame->setIcon(ENTYPO_ICON_TO_END);
	 //m_nextFrame->setEnabled(false);
	// m_nextFrame->setCallback(std::bind(&ScenarioControl::stepEngine, this));
	//gui->addWidget("", simulation);

	// gui->addGroup("Render options");
	// gui->addVariable("Ropes", enableConstraintsRendering);

	//nanogui::Label *instructions = new nanogui::Label(nanoguiWindow.get(),
	//	 "Press W, A, S, D to move and Left Mouseclick to shoot. \n \nTry to shoot the flying objects as far away from you as possible so that they disappear. \n \n Press H to hide the Menue.");
	////instructions->setFixedWidth(width-100);
	////instructions->setFixedHeight(150);
	//gui->addGroup("Instructions");
	//gui->addWidget("", instructions);

  	//gui->setFixedSize(Eigen::Vector2i(width-100, height));
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
