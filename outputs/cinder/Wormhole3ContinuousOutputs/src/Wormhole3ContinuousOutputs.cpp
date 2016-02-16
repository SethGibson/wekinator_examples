#include "Wormhole3ContinuousOutputs.h"

Wormhole3ContinuousOutputs::Wormhole3ContinuousOutputs() :
	mReceiver(kListenPort)
{

}

void Wormhole3ContinuousOutputs::setup()
{
	setupParams();

	mReceived = vec3(1);
	mCamera.setPerspective(60.0f, getWindowAspectRatio(), 0.1f, 10.f);
	mCamera.lookAt(vec3(0, kEyeDepth, 0), vec3(), vec3(0, 1, 0));

	mShader = gl::GlslProg::create(loadAsset("shaders/wormhole.vert"), loadAsset("shaders/wormhole.frag"));

	auto mesh = gl::VboMesh::create(geom::Torus().center(vec3()).radius(0.5f, 0.49f).subdivisionsAxis(64).subdivisionsHeight(4));
	geom::BufferLayout attribs;
	attribs.append(geom::CUSTOM_0, 4, sizeof(Ring), offsetof(Ring, Tint), 1);
	attribs.append(geom::CUSTOM_1, 3, sizeof(Ring), offsetof(Ring, Position), 1);
	attribs.append(geom::CUSTOM_2, 2, sizeof(Ring), offsetof(Ring, Size), 1);

	mRings.push_back(Ring(Color(1, 0, 0), vec2(mReceived.x, mReceived.y)));
	mData = gl::Vbo::create(GL_ARRAY_BUFFER, mRings, GL_DYNAMIC_DRAW);
	mesh->appendVbo(attribs, mData);

	mDraw = gl::Batch::create(mesh, mShader, { {geom::CUSTOM_0, "i_Tint"},{ geom::CUSTOM_1, "i_Position" },{ geom::CUSTOM_2, "i_Size" } });

	setupListener();
}

void Wormhole3ContinuousOutputs::update()
{
	for (auto ri = begin(mRings); ri != end(mRings);) {
		if (!ri->Active) {
			ri = mRings.erase(ri);
		}
		else {
			ri->Step(mParamRingSpeed*mReceived.z);
			++ri;
		}
	}


	if (getElapsedFrames() % mParamSpawnTime == 0) {
		auto r = mReceived.x;
		auto b = mReceived.y;

		mRings.push_back(Ring(Color(r, 1, b), vec2(mReceived.x, mReceived.y)));
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
}

void Wormhole3ContinuousOutputs::setupParams()
{
	mParamRingSpeed = 0.025f;
	mParamRotSpeed = 0.01f;
	mParamSpawnTime = 10;
}

void Wormhole3ContinuousOutputs::setupListener()
{
	mReceiver.setListener(kAddress,
		[&](const osc::Message &pMsg) {
		mReceived.x = lerp<float>(0.5f,2.0f, pMsg[0].flt());
		mReceived.y = lerp<float>(0.5f, 2.0f, pMsg[1].flt());
		mReceived.z = lerp<float>(0.1f, 1.0f, pMsg[2].flt());
	});

	mReceiver.bind();
	mReceiver.listen();
}

void prepareSettings(App::Settings *pSettings)
{
	pSettings->setWindowSize(1280, 720);
	pSettings->setFrameRate(60.0f);
}

CINDER_APP( Wormhole3ContinuousOutputs, RendererGl, prepareSettings )
