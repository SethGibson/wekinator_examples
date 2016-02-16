#pragma once

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/Camera.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

#include "Osc.h"

using namespace ci;
using namespace ci::app;
using namespace std;

const uint16_t kListenPort = 12000;
const string kAddress = "/wek/outputs";

const float kEyeDepth = 5.0f;

struct Ring
{
	Ring() {}
	Ring(Color pTint, vec2 pSize) :
		Size(pSize), Position(vec3()), BaseTint(pTint), Active(true)
	{
		Speed = randFloat(0.5f, 2.0f);
	}

	void Step(float pSpeed)
	{
		if (Position.y >= kEyeDepth)
			Active = false;
		else {
			Position.y += pSpeed*Speed;
			auto tint = lerp<Color>(Color(1, 0, 0), BaseTint, (Position.y / kEyeDepth));
			Tint.r = tint.r; Tint.g = tint.g; Tint.b = tint.b;
			Tint.a = Position.y / (kEyeDepth*2.0f);
		}
	}

	bool	Active;
	ColorA	Tint;
	Color	BaseTint;
	vec3	Position;
	vec2	Size;
	float	Speed;
};

class Wormhole3ContinuousOutputs : public App {
  public:
	Wormhole3ContinuousOutputs();

	void setup() override;
	void update() override;
	void draw() override;

  private:
	void setupParams();
	void setupListener();

	vec3			mReceived; //x=mRingSize.x
	vec2			mRingSize;
	vector<Ring>	mRings;
	gl::GlslProgRef	mShader;
	gl::BatchRef	mDraw;
	gl::VboRef		mData;

	CameraPersp		mCamera;

	float					mParamRingSpeed,
							mParamRotSpeed;

	int						mParamSpawnTime;

	osc::ReceiverUdp	mReceiver;
};