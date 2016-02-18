#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class RSFaceTracking3InputsXYZ : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void RSFaceTracking3InputsXYZ::setup()
{
}

void RSFaceTracking3InputsXYZ::mouseDown( MouseEvent event )
{
}

void RSFaceTracking3InputsXYZ::update()
{
}

void RSFaceTracking3InputsXYZ::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( RSFaceTracking3InputsXYZ, RendererGl )
