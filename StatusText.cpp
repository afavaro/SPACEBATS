
#include "StatusText.h"
#include "StatusBar.h"

#include <sstream>

#define INSET 0.01

using namespace std;

StatusText::StatusText(sf::RenderWindow *window, btVector4 rect)
: score(0), rect(rect), window(window) {}

StatusText::~StatusText() {}

void StatusText::addScore(int points) {
	score += points;
}

void StatusText::updateTime(float tstep) {
	this->time += tstep;
}

void StatusText::loadFont(const string &filename) {
	if (!font.LoadFromFile(filename))
		cerr << "Error loading font " << filename << endl;
}

void StatusText::render() {
	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE); //GL_CHECK

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //GL_CHECK

	glUseProgram(StatusBar::BarShader->programID());

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1.0, 1.0, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	GLint pos = glGetAttribLocation(StatusBar::BarShader->programID(), "positionIn");
	GLint col = glGetAttribLocation(StatusBar::BarShader->programID(), "colorIn");

	glBegin(GL_QUADS);
	glVertexAttrib4f(col, 0.6, 0.81, 0.92, 0.5);
	glVertexAttrib2f(pos, rect.x(), rect.y());
	glVertexAttrib4f(col, 0.6, 0.81, 0.92, 0.0);
	glVertexAttrib2f(pos, rect.x() + rect.z(), rect.y());
	glVertexAttrib4f(col, 0.6, 0.81, 0.92, 0.0);
	glVertexAttrib2f(pos, rect.x() + rect.z(), rect.y() - rect.w());
	glVertexAttrib4f(col, 0.6, 0.81, 0.92, 0.5);
	glVertexAttrib2f(pos, rect.x(), rect.y() - rect.w());
	glEnd();

	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);

	stringstream ss;
	ss << "Score: " << score << endl;
	int seconds = (int)time;
	ss << "Time: " << seconds / 60 << ":";
	ss.width(2);
	ss.fill('0');
	ss << right << seconds % 60;

	sf::String text(ss.str(), font, 30);	
	float x = (0.5 * (rect.x() + INSET) + 0.5) * (float)window->GetWidth();
	float y = (0.5 - 0.5 * rect.y()) * (float)window->GetHeight();
	text.SetPosition(x, y);
	text.SetColor(sf::Color(0, 0, 0));

	window->PreserveOpenGLStates(true);

	glUseProgram(0);

	window->Draw(text);
}
