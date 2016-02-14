#pragma once

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/Camera.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

const float kEyeDepth = 5.0f;

struct Ring
{
	Ring() {}
	Ring(Color pTint, vec2 pSize) :
		Size(pSize), Position(vec3()), BaseTint(pTint), Active(true)
	{
	}

	void Step(float pSpeed)
	{
		if (Position.y >= kEyeDepth)
			Active = false;
		else {
			Position.y += pSpeed;
			auto zRatio = Position.y / kEyeDepth;
			auto tint = lerp<Color>(Color(1, 0, 0), BaseTint, zRatio);
			Tint.r = tint.r; Tint.g = tint.g; Tint.b = tint.b;
			Tint.a = zRatio;
		}
	}

	bool	Active;
	ColorA	Tint;
	Color	BaseTint;
	vec3	Position;
	vec2	Size;
};
class Wormhole3ContinuousOutputs : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void mouseMove(MouseEvent event) override; //Debug for now
	void mouseDrag(MouseEvent event) override; //Debug for now
	void update() override;
	void draw() override;

  private:
	  vec2				mMousePos,
						mRingSize;

	  vector<Ring>		mRings;
	  gl::GlslProgRef	mShader;
	  gl::BatchRef		mDraw;
	  gl::VboRef		mData;

	  CameraPersp		mCamera;
};