#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class RSFaceTracking78InputsXYZApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void RSFaceTracking78InputsXYZApp::setup()
{
}

void RSFaceTracking78InputsXYZApp::mouseDown( MouseEvent event )
{
}

void RSFaceTracking78InputsXYZApp::update()
{
}

void RSFaceTracking78InputsXYZApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( RSFaceTracking78InputsXYZApp, RendererGl )
