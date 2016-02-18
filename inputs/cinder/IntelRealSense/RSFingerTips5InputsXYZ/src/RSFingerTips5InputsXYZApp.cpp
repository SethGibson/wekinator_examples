#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class RSFingerTips5InputsXYZApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void RSFingerTips5InputsXYZApp::setup()
{
}

void RSFingerTips5InputsXYZApp::mouseDown( MouseEvent event )
{
}

void RSFingerTips5InputsXYZApp::update()
{
}

void RSFingerTips5InputsXYZApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( RSFingerTips5InputsXYZApp, RendererGl )
