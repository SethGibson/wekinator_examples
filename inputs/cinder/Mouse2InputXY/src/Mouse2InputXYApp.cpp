#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "Osc.h"

using namespace ci;
using namespace ci::app;
using namespace std;

const uint16_t kPort = 6448;
const string kHost = "127.0.0.1";
const string kAddress = "/wek/inputs";

class Mouse2InputXY : public App {
  public:
	Mouse2InputXY();
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void mouseMove(MouseEvent event) override;
	void keyDown(KeyEvent event) override;
	void update() override;
	void draw() override;

  private:
	  void	sendMousePos();

	  bool				mIsSending,
						mIsRunning;
	  vec2				mMousePos;
	  osc::SenderUdp	mSender;
	  Font				mFont;
};

Mouse2InputXY::Mouse2InputXY() :
	mSender(8888, kHost, kPort), mIsSending(false), mIsRunning(false), mMousePos(vec2())
{
	mFont = Font("Calibri", 24);
}

void Mouse2InputXY::setup()
{
	mSender.bind();
}

void Mouse2InputXY::mouseDown( MouseEvent event )
{
	if (!mIsRunning) {
		mIsSending = !mIsSending;
	}
}

void Mouse2InputXY::mouseMove(MouseEvent event)
{
	if (!mIsSending) {
		mMousePos = event.getPos();
	}
}

void Mouse2InputXY::keyDown(KeyEvent event)
{
	auto code = event.getCode();
	switch(code)
	{
	case KeyEvent::KEY_f:
		getWindow()->setFullScreen(!getWindow()->isFullScreen());
		break;
	case KeyEvent::KEY_q:
		quit();
		break;
	case KeyEvent::KEY_r:
		mIsRunning = !mIsRunning;
		break;
	}
}

void Mouse2InputXY::update()
{
	if(mIsSending||mIsRunning) {
		sendMousePos();
	}
}

void Mouse2InputXY::draw()
{
	gl::clear(Color(0, 0.1f, 0));

	// Draw crosshair
	{
		gl::color(Color(0, .25f, 0));
		gl::ScopedLineWidth(2.0f);
		gl::drawLine(vec2(0, mMousePos.y), vec2(getWindowWidth(), mMousePos.y));
		gl::drawLine(vec2(mMousePos.x, 0), vec2(mMousePos.x, getWindowHeight()));
	}

	//draw center
	{
		if (mIsSending||mIsRunning) {
			gl::color(Color(0, 1, 0));
			gl::drawSolidCircle(mMousePos, 20);
		}
		else {
			gl::color(Color(1, 1, 0));
			gl::drawSolidCircle(mMousePos, 5);
		}
	}

	//Draw debug text
	{
		auto mid = getWindowCenter();
		string msg = "'F' to toggle fullscreen, 'R' to toggle recording/running, 'Q' to quit";
		gl::drawString(msg, vec2(10, getWindowHeight() - 20), Color(0, 1, 0), mFont);

		auto strPos = vec2(mid.x + 10, getWindowHeight() - 20);
		if (mIsSending||mIsRunning) {
			msg = "Sending Position X=" + to_string((int)mMousePos.x) + ", Y=" + to_string((int)mMousePos.y);
			gl::drawString(msg, strPos, Color(0, 1, 0), mFont);
		}
		else {
			msg = "Not Sending";
			gl::drawString(msg, strPos, Color(1, 0, 0), mFont);
		}
	}
}

void Mouse2InputXY::sendMousePos()
{
	auto msg = osc::Message(kAddress);
	msg.append((float)mMousePos.x);
	msg.append((float)mMousePos.y);

	mSender.send(msg);
}

void prepareSettings(App::Settings *pSettings)
{
	pSettings->setWindowSize(1280, 720);
	pSettings->setFrameRate(30.0f);
	pSettings->setResizable();
}
CINDER_APP( Mouse2InputXY, RendererGl, prepareSettings )
