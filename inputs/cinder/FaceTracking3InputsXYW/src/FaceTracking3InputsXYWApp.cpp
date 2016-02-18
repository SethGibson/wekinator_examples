#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/Capture.h"
#include "cinder/gl/gl.h"
#include "cinder/Log.h"
#include "cinder/params/Params.h"

#include "CinderOpenCV.h"
#include "Osc.h"

using namespace ci;
using namespace ci::app;
using namespace std;

const uint16_t kPort = 6448;
const string kHost = "127.0.0.1";
const string kAddress = "/wek/inputs";

class FaceTracking3InputsXYW : public App {
  public:
	FaceTracking3InputsXYW();
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
	void cleanup() override;

	void StartDevice();

  private:
	void setupGUI();
	void getFirstFace();
	void sendFace();


	cv::CascadeClassifier	mFaceCascade;
	Rectf	mFirstFace;

	CaptureRef			mCam;
	SurfaceRef			mSurfCam;

	bool			mIsRunning,
					mIsSending;
	osc::SenderUdp	mSender;

	Font	mFont;

	params::InterfaceGlRef	mGUI;
	vector<string>			mParamDeviceNames;
	int						mParamDeviceId;
};

FaceTracking3InputsXYW::FaceTracking3InputsXYW():
	mSender(12000,kHost,kPort), mIsSending(false), mIsRunning(false)
{

}

void FaceTracking3InputsXYW::setup()
{
	setupGUI();

	mFont = Font("Calibri", 24);
	mFaceCascade.load(getAssetPath("haarcascade_frontalface_alt.xml").string());
	mSender.bind();
}

void FaceTracking3InputsXYW::mouseDown( MouseEvent event )
{
}

void FaceTracking3InputsXYW::update()
{
	mIsSending = false;
	if (mIsRunning) {
		if (mCam->checkNewFrame()) {
			mSurfCam = mCam->getSurface();
			getFirstFace();
			mIsSending = mFirstFace.getWidth() > 0;
			if (mIsSending) {
				sendFace();
			}
		}
	}
}

void FaceTracking3InputsXYW::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
	if (mSurfCam) {
		auto camTex = gl::Texture2d::create(*mSurfCam);

		gl::setMatricesWindow(getWindowSize());
		gl::color(Color::white());
		gl::draw(camTex);
		gl::drawStrokedRect(mFirstFace);
	}

	string stat = "Not Sending";
	ColorA statColor(1, 0, 0, 1);
	if (mIsSending) {
		auto xy = mFirstFace.getCenter();
		auto w = mFirstFace.getWidth();
		stat = "Sending Face: X=" + to_string(xy.x) + ", Y=" + to_string(xy.y) + ", W=" + to_string(w);
		statColor = ColorA(0, 1, 0, 1);
	}

	gl::drawString(stat, vec2(20, getWindowHeight() - 20), statColor, mFont);
	
	if (!mIsRunning) {
		mGUI->draw();
	}
}

void FaceTracking3InputsXYW::cleanup()
{
	if (mIsRunning) {
		mCam->stop();
	}

	mSender.close();
}

void FaceTracking3InputsXYW::StartDevice()
{
	auto deviceName = mParamDeviceNames[mParamDeviceId];

	bool hasCam = false;
	Capture::DeviceRef cam;
	for (const auto &device : Capture::getDevices()) {
		if (device->getName()==deviceName) {
			cam = device;
			hasCam = true;
			break;
		}
	}
	if (hasCam) {
		try {
			mCam = Capture::create(640, 480, cam);
			mCam->start();
			mIsRunning = true;
		}
		catch (ci::Exception &exc) {
			CI_LOG_EXCEPTION("Failed to init capture ", exc);
		}
	}
}

void FaceTracking3InputsXYW::getFirstFace()
{
	mFirstFace = Rectf();

	cv::Mat grey(toOcv(*mSurfCam, CV_8UC1));
	cv::Mat camScaled(mSurfCam->getHeight()/2, mSurfCam->getWidth() / 2, CV_8UC1);
	cv::resize(grey, camScaled, camScaled.size(), 0, 0, cv::INTER_LINEAR);
	cv::equalizeHist(camScaled, camScaled);
	vector<cv::Rect> faces;

	mFaceCascade.detectMultiScale(camScaled, faces);
	for (const auto &f : faces) {
		Rectf faceRect(fromOcv(f));
		if (faceRect.getWidth() > 0) {
			mFirstFace =  faceRect * 2;
		}
	}
}

void FaceTracking3InputsXYW::sendFace()
{
	auto xy = mFirstFace.getCenter();
	auto w = mFirstFace.getWidth();
	
	auto msg = osc::Message(kAddress);
	msg.append((float)xy.x);
	msg.append((float)xy.y);
	msg.append((float)w);

	mSender.send(msg);
}

void FaceTracking3InputsXYW::setupGUI()
{
	mParamDeviceId = 0;
	for (const auto &device : Capture::getDevices()) {
		mParamDeviceNames.push_back(device->getName());
	}

	mGUI = params::InterfaceGl::create("Params", { 400,200 });
	mGUI->addParam("Cameras", mParamDeviceNames, &mParamDeviceId, false);
	mGUI->addButton("Start", std::bind(&FaceTracking3InputsXYW::StartDevice, this), "");

}

void prepareSettings(App::Settings *pSettings)
{
	pSettings->setWindowSize(640, 480);
	pSettings->setFrameRate(30);
}

CINDER_APP( FaceTracking3InputsXYW, RendererGl )
