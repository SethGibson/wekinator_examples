#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class RSFaceTracking3InputsXYWApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void RSFaceTracking3InputsXYWApp::setup()
{
}

void RSFaceTracking3InputsXYWApp::mouseDown( MouseEvent event )
{
}

void RSFaceTracking3InputsXYWApp::update()
{
}

void RSFaceTracking3InputsXYWApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( RSFaceTracking3InputsXYWApp, RendererGl )
