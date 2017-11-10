#include <unistd.h>
#include <list>
#include <map>
#include <iostream>
#include <pthread.h>
using namespace std;

#include <semaphore.h>
#include <time.h>

#include "geefish.h"
#include "gf_cv.h"

namespace GeeFish {
Controller *dancer = NULL;

class Controller::Impl {
	std::list<Listener *> obs;
	std::list<Frame> frames_;
	std::list<Device> devices_;

	sem_t mutex1, mutex2;
	friend Controller;
private:
	void *cv_thrd(void *p) {
		Controller::Impl *imp = (Controller::Impl *)p;
		while (true) {
			sem_post(&mutex2);
			sem_wait (&mutex1);
			Frame frame = frames_.front();
			sleep(3);
			cerr << "done\n";
			// retrieve the analysis result
		}
	}
public:
	Impl() {
		sem_init(&mutex1, 0, 1);
		sem_init(&mutex2, 0, 1);
		Device device;
		devices_.push_back(device);
	}

	void handler(void *data) {
		// set gestures and hands
		// if hand gesture middleware is busy, use the previous result
		// then use the result from middleware

		// call cv algorithm to process current frame
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
		Frame current;
		// notify observers of the event
		dancer->pImpl->frames_.push_back(current);

		list<Listener*>::iterator it = dancer->pImpl->obs.begin();
		while (it != dancer->pImpl->obs.end())
		{
			((Listener* )(*it))->onFrame(*dancer);
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
	Frame frame(int history = 0) {
		return frames_.front();
	}
	std::list<Device> devices() const {
		return devices_;
	}
};

class Device::Impl {
	bool running;
private:
	pthread_t task;
	static void *thr_fn(void *p) {
		Device::Impl *that = (Device::Impl *)p;
		// pthread_mutex_lock(&mutex);
		while (that->running) {
			sleep(1);
			cerr << "device new frame arrives \n";
			if (dancer) dancer->pImpl->handler(NULL);
		}
		// pthread_mutex_unlock(&mutex);
		return NULL;
	}
public:
	Impl() {
		running = true;
		pthread_create(&task, NULL, &thr_fn, this);
	}
	~Impl() {
		running = false;
	}
};

class Image::Impl {
public:
	Frame frame;

};
class Frame::Impl {
public:
	std::list<Gesture> gestures;
	Gesture sss;
	std::list<Hand> hands;
	std::list<Image>images;
public:
	// fill images
	void fill() {

	}
	Impl() {
		cerr << "Frame::Impl constructor \n";
	}
	~Impl() {
		gestures.clear();
		cerr << "Frame::Impl destructor \n";
	}
};

class Hand::Impl {
public:
	Hand::Type type;
	Hand::State state;
	Frame frame;
};


class Gesture::Impl {
public:
	Gesture::Type type;
	Gesture::State state;
	Frame frame;
public:
	Impl() {
		cerr << "Gesture::Impl constructor \n";
	}
	~Impl() {
		cerr << "Gesture::Impl destructor \n";
	}
};

Gesture::Gesture(): pImpl(new Impl()) {
}
Gesture::~Gesture() {
	cerr << pImpl.use_count() << endl;
}

Gesture::Type Gesture::type() {
	return pImpl->type;
}
Gesture::State Gesture::state() {
	return pImpl->state;
}
Frame Gesture::frame() {
	return pImpl->frame;
}
Frame::Frame(): pImpl(new Impl) {
}
Frame::~Frame() {
}

Hand::Hand(): pImpl(new Impl) {
}
Frame Hand::frame() {
	return pImpl->frame;
}


Hand::Type Hand::type() {
	return pImpl->type;
}
Hand::State Hand::state() {
	return pImpl->state;
}


Controller::Controller(): pImpl(new Impl()) {
	dancer = this;
}
bool Controller::addObs(Listener& listener) {
	return pImpl->addObs(listener);
}
bool Controller::removeObs(Listener& listener) {
	return pImpl->removeObs(listener);
}
std::list<Device> Controller::devices() const {
	return pImpl->devices_;
}
Frame Controller::frame(int history) const {
	return pImpl->frames_.front();
}
void Controller::enableGesture(Gesture::Type type, bool enable) const {}
bool Controller::isGestureEnabled(Gesture::Type type) const {}

Device::Device(): pImpl(new Impl()) {
}


void test()
{
	Frame frame;
	Gesture gesture;
	cerr << "xx\n";
	frame.pImpl->sss = gesture;
	cerr << "xx\n";
	gesture.pImpl->frame = frame;
}
}