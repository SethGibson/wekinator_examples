#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class RSCursorTracking3InputsXYZApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void RSCursorTracking3InputsXYZApp::setup()
{
}

void RSCursorTracking3InputsXYZApp::mouseDown( MouseEvent event )
{
}

void RSCursorTracking3InputsXYZApp::update()
{
}

void RSCursorTracking3InputsXYZApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( RSCursorTracking3InputsXYZApp, RendererGl )
