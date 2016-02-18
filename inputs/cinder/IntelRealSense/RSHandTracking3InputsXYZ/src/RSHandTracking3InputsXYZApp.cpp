#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class RSHandTracking3InputsXYZApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void RSHandTracking3InputsXYZApp::setup()
{
}

void RSHandTracking3InputsXYZApp::mouseDown( MouseEvent event )
{
}

void RSHandTracking3InputsXYZApp::update()
{
}

void RSHandTracking3InputsXYZApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( RSHandTracking3InputsXYZApp, RendererGl )
