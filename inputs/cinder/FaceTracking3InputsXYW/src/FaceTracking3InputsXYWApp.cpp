#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class FaceTracking3InputsXYWApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void FaceTracking3InputsXYWApp::setup()
{
}

void FaceTracking3InputsXYWApp::mouseDown( MouseEvent event )
{
}

void FaceTracking3InputsXYWApp::update()
{
}

void FaceTracking3InputsXYWApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( FaceTracking3InputsXYWApp, RendererGl )
