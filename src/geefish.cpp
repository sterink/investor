#include <unistd.h>
#include <list>
#include <map>
#include <iostream>
#include <pthread.h>
using namespace std;

#include "geefish.h"
#include "gf_cv.h"

namespace GeeFish {

std::map<const void *, void *> docker;
class ControllerImp;
std::shared_ptr<ControllerImp> ppp;

class GestureImp;
class HandImp;
class FrameImp {
public:
	Gesture ges;
	Hand hands;
	std::list<std::shared_ptr<Image> >images;
public:
	// fill images
	void fill() {

	}
};

class HandImp {
public:
	Hand::Type type;
	Hand::State state;
	std::shared_ptr<Frame> frame;
};


class GestureImp {
public:
	Gesture::Type type;
	Gesture::State state;
	std::shared_ptr<Frame> frame;
public:
	GestureImp() {}
};

Gesture::Gesture() {
	GestureImp *tmp = new GestureImp();
	docker[this] = tmp;
}
Gesture::~Gesture() {
	GestureImp *tmp = (GestureImp *)docker[this];
	delete tmp;
	// free slot in docker
	docker.erase(this);
}

Gesture::Type Gesture::type() {
	GestureImp *imp = (GestureImp *)docker[this];
	return imp->type;
}
Gesture::State Gesture::state() {
	GestureImp *imp = (GestureImp *)docker[this];
	return imp->state;
}
std::shared_ptr<Frame> Gesture::frame() {
	GestureImp *imp = (GestureImp *)docker[this];
	return imp->frame;
}
Frame::Frame() {
	FrameImp *tmp = new FrameImp();
	docker[this] = tmp;
}
Frame::~Frame() {
	FrameImp *tmp = (FrameImp *)docker[this];
	delete tmp;
	docker.erase(this);
}

Hand::Hand() {
	HandImp *tmp = new HandImp();
	docker[this] = tmp;
}
Hand::~Hand() {
	HandImp *tmp = (HandImp *)docker[this];
	delete tmp;
	docker.erase(this);
}


Hand::Type Hand::type() {
	HandImp *imp = (HandImp *)docker[this];
	return imp->type;
}
Hand::State Hand::state() {
	HandImp *imp = (HandImp *)docker[this];
	return imp->state;
}
std::shared_ptr<Frame> Hand::frame() {
	HandImp *imp = (HandImp *)docker[this];
	return imp->frame;
}

class ImageImp;

#include <pthread.h>
#include <time.h>

class ControllerImp {
	Controller *ctl;
	std::list<Listener *> obs;
	std::list<shared_ptr<Frame> > frames;

	sem_t mutex1, mutex2;

private:
	void *cv_thrd(void *p) {
		ControllerImp *imp = (ControllerImp *)p;
		while (true) {
			sem_post(&mutex2);
			sem_wait (&mutex1);
			shared_ptr<Frame> pFrame = frames.front();
			sleep(3);
			cerr << "done\n";
			// retrieve the analysis result
		}
	}
public:
	ControllerImp(Controller *parter): ctl(parter) {
		sem_init(&mutex1, 0, 1);
		sem_init(&mutex2, 0, 1);
	}
	static void handler(void *data) {
		Frame *p = new Frame();
		FrameImp *imp = (FrameImp *)docker[p];
		// set gestures and hands
		// if hand gesture middleware is busy, use the previous result
		// then use the result from middleware

		// call cv algorithm to process current frame
		shared_ptr<Frame> current(p);
		// check whether cv_thrd is busy or not
		if (0 == sem_trywait(&mutex2)) {
			// cv thrd is idle
			cerr << "prepare task for thread\n";
			// prepare the task
			sem_post(&mutex1);
			gf_cv_handler();

		} else {
			// use previous result
			cerr << "thread is busy, do sth else\n";


		}
		// notify observers of the event
		ppp->frames.push_back(current);

		list<Listener*>::iterator it = ppp->obs.begin();
		while (it != ppp->obs.end())
		{
			((Listener* )(*it))->onFrame(*ppp->ctl);
			++it;
		}
	}

	bool addObs(Listener& listener) {
		obs.push_back(&listener);
		return true;
	}
	bool removeObs(Listener& listener) {
		obs.remove(&listener);
		return true;
	}
	std::shared_ptr<Frame> frame(int history = 0) {
		return frames.front();
	}
};

bool Controller::addObs(Listener& listener) {
	ControllerImp *imp = (ControllerImp *)docker[this];
	imp->addObs(listener);
}
bool Controller::removeObs(Listener& listener) {
	ControllerImp *imp = (ControllerImp *)docker[this];
	imp->removeObs(listener);
}
std::list<std::shared_ptr<Device> > Controller::devices() const {
	ControllerImp *imp = (ControllerImp *)docker[this];
	// return device;
}
std::shared_ptr<Frame> Controller::frame(int history) const {
	return NULL;
}
void Controller::enableGesture(Gesture::Type type, bool enable) const {}
bool Controller::isGestureEnabled(Gesture::Type type) const {}

Controller::Controller() {
	// construct a device
	Device *device = new Device(this);
	ControllerImp *tmp = new ControllerImp(this);
	docker[this] = tmp;
}

Controller::~Controller() {
	ControllerImp *imp = (ControllerImp *)docker[this];
	delete imp;
	docker.erase(this);
}

void *thr_fn(void *p) {
	Device *that = (Device *)p;
	// pthread_mutex_lock(&mutex);
	while (true) {
		sleep(1);
		ppp->handler(NULL);
	}
	// pthread_mutex_unlock(&mutex);
	cout << "thread exit\n";
	return NULL;
}
Device::Device(Controller *ctl) {
	std::shared_ptr<ControllerImp> tmp((ControllerImp *)docker[ctl]);
	ppp = tmp;
	pthread_t task;
	pthread_create(&task, NULL, &thr_fn, this);
}
}