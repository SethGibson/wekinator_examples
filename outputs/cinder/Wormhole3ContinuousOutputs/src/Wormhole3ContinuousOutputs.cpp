#include "Wormhole3ContinuousOutputs.h"

void Wormhole3ContinuousOutputs::setup()
{
	setupGUI();

	mRingSize = vec2(1);
	mCamera.setPerspective(60.0f, getWindowAspectRatio(), 0.1f, 10.f);
	mCamera.lookAt(vec3(0, kEyeDepth, 0), vec3(), vec3(0, 1, 0));

	mShader = gl::GlslProg::create(loadAsset("shaders/wormhole.vert"), loadAsset("shaders/wormhole.frag"));

	auto mesh = gl::VboMesh::create(geom::Torus().center(vec3()).radius(0.5f, 0.49f).subdivisionsAxis(32));
	geom::BufferLayout attribs;
	attribs.append(geom::CUSTOM_0, 4, sizeof(Ring), offsetof(Ring, Tint), 1);
	attribs.append(geom::CUSTOM_1, 3, sizeof(Ring), offsetof(Ring, Position), 1);
	attribs.append(geom::CUSTOM_2, 2, sizeof(Ring), offsetof(Ring, Size), 1);

	mRings.push_back(Ring(Color(1, 0, 0), mRingSize));
	mData = gl::Vbo::create(GL_ARRAY_BUFFER, mRings, GL_DYNAMIC_DRAW);
	mesh->appendVbo(attribs, mData);

	mDraw = gl::Batch::create(mesh, mShader, { {geom::CUSTOM_0, "i_Tint"},{ geom::CUSTOM_1, "i_Position" },{ geom::CUSTOM_2, "i_Size" } });
}

void Wormhole3ContinuousOutputs::mouseDown( MouseEvent event )
{
	mouseMove(event);
	mRingSize.x = lmap<float>(event.getPos().x, 0, getWindowWidth(), 0.1f, 1.0f);
	mRingSize.y = lmap<float>(event.getPos().y, 0, getWindowHeight(), 0.1f, 1.0f);

	console() << "RingSize: " << to_string(mRingSize.x) << ", " << to_string(mRingSize.y) << endl;

}

void Wormhole3ContinuousOutputs::mouseMove(MouseEvent event)
{
	mMousePos = event.getPos();
}

void Wormhole3ContinuousOutputs::mouseDrag(MouseEvent event)
{
	mouseDown(event);
}

void Wormhole3ContinuousOutputs::update()
{
	for (auto ri = begin(mRings); ri != end(mRings);) {
		if (!ri->Active) {
			ri = mRings.erase(ri);
		}
		else {
			ri->Step(mParamRingSpeed);
			++ri;
		}
	}

	if (getElapsedFrames() % mParamSpawnTime == 0) {
		auto r = mMousePos.x / getWindowWidth();
		auto b = mMousePos.y / getWindowHeight();

		mRings.push_back(Ring(Color(r, 1, b), mRingSize));
	}

	mData->bufferData(mRings.size()*sizeof(Ring), mRings.data(), GL_DYNAMIC_DRAW);
}

void Wormhole3ContinuousOutputs::draw()
{
	gl::clear( Color( 0, 0, 0 ) );

	gl::setMatrices(mCamera);
	{
		gl::ScopedGlslProg shdr(mShader);
		gl::pushModelMatrix();

		gl::rotate(getElapsedFrames()*mParamRotSpeed, vec3(0, 1, 0));
		mDraw->drawInstanced(mRings.size());
		gl::popModelMatrix();
	}

	gl::setMatricesWindow(getWindowSize());
	mGUI->draw();
}

void Wormhole3ContinuousOutputs::setupGUI()
{
	mParamRingSpeed = 0.1f;
	mParamRotSpeed = 0.01f;
	mParamSpawnTime = 30;

	mGUI = params::InterfaceGl::create("Params", ivec2(300, 200));
	mGUI->addParam("paramRingSpeed", &mParamRingSpeed, "label='Ring Speed'", false);
	mGUI->addParam("paramRotSpeed", &mParamRotSpeed, "label='Spin Speed'", false);
	mGUI->addParam("paramSpawnTime", &mParamSpawnTime, "label='Spawn Time'", false);
}

void prepareSettings(App::Settings *pSettings)
{
	pSettings->setWindowSize(1280, 720);
	pSettings->setFrameRate(60.0f);
}

CINDER_APP( Wormhole3ContinuousOutputs, RendererGl, prepareSettings )
