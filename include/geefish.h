/******************************************************************************\
* Copyright (C) 2015-2017 Leap Motion, Inc. All rights reserved.               *
* Leap Motion proprietary and confidential. Not for distribution.              *
* Use subject to the terms of the JeeFish SDK Agreement available at           *
* https://developer.geefish.com/sdk_agreement, or another agreement            *
* between GeeFish Motion and you, your company or other organization.          *
\******************************************************************************/
#pragma once
#include <list>

#include "GeeFishMath.h"

using namespace std;

namespace GeeFish {
class gfObject {
public:
	int64_t st;
public:
	gfObject() {

	}
public:
	int64_t timestamp() const;
	int32_t id() const;
};
class Controller;
class Device;
class Image;
class Gesture;
class Bone;
class Hand;
class Finger;
class Frame;

class Frame: public gfObject {
public:
	Frame();
	~Frame();
	std::list<Image> images() const;
	std::list<Gesture> gestures() const;
	std::list<Hand> hands() const;
public:
	class Impl;
	std::shared_ptr<Impl> pImpl;
};
class Image: public gfObject {
public:
	enum Type {
		TYPE_CLOUDPOINT    = 0,
		TYPE_GRAY = 1,
		TYPE__RGB = 2
	};
	Type type() const;
	const void* data();
	int height() const;
	int width() const;
public:
	class Impl;
	std::shared_ptr<Impl> pImpl;
};

class Gesture: public gfObject {
public:
	enum Type {
		/**
		 * An invalid type.
		 * @since 1.0
		 */
		TYPE_INVALID    = -1,
		/**
		 * A straight line movement by the hand with fingers extended.
		 * @since 1.0
		 */
		TYPE_SWIPE      = 1,
		/**
		 * A circular movement by a finger.
		 * @since 1.0
		 */
		TYPE_CIRCLE     = 4,
		/**
		 * A forward tapping movement by a finger.
		 * @since 1.0
		 */
		TYPE_SCREEN_TAP = 5,
		/**
		 * A downward tapping movement by a finger.
		 * @since 1.0
		 */
		TYPE_KEY_TAP    = 6
	};

	enum State {
		/**
		 * An invalid state
		 * @since 1.0
		 */
		STATE_INVALID = -1,
		/**
		 * The gesture is starting. Just enough has happened to recognize it.
		 * @since 1.0
		 */
		STATE_START   = 1,
		/**
		 * The gesture is in progress. (Note: not all gestures have updates).
		 * @since 1.0
		 */
		STATE_UPDATE  = 2,
		/**
		 * The gesture has completed or stopped.
		 * @since 1.0
		 */
		STATE_STOP    = 3
	};
public:
	Gesture();
	~Gesture();
	Type type();
	State state();
	Frame frame();
public:
	class Impl;
	std::shared_ptr<Impl> pImpl;
};
class Bone {
public:
	/**
	 * Enumerates the names of the bones.
	 *
	 * Members of this enumeration are returned by Bone::type() to identify a
	 * Bone object.
	 * @since 2.0
	 */
	enum Type {
		TYPE_METACARPAL = 0,   /**< Bone connected to the wrist inside the palm */
		TYPE_PROXIMAL = 1,     /**< Bone connecting to the palm */
		TYPE_INTERMEDIATE = 2, /**< Bone between the tip and the base*/
		TYPE_DISTAL = 3,       /**< Bone at the tip of the finger */
	};

public:
	class Impl;
	std::shared_ptr<Impl> pImpl;
};

class Finger: public gfObject {
	Bone bone(Bone::Type boneIx) const;
public:
	class Impl;
	std::shared_ptr<Impl> pImpl;
};

class Hand: public gfObject {
public:
	enum Type {
	};

	enum State {
	};
	Hand();
	Type type();
	State state();
	Frame frame();

	std::list<Finger> fingers() const;
public:
	class Impl;
	std::shared_ptr<Impl> pImpl;
};

class Listener {
public:
	virtual void onInit(const Controller&) {}
	virtual void onConnect(const Controller&) {}
	virtual void onDisconnect(const Controller&) {}
	virtual void onExit(const Controller&) {}
	virtual void onFrame(const Controller&) {}
	virtual void onImages(const Controller&) {}
	virtual void onFocusGained(const Controller&) {}
	virtual void onFocusLost(const Controller&) {}
	virtual void onDeviceChange(const Controller&) {}
	virtual void onServiceConnect(const Controller&) {}
	virtual void onServiceDisconnect(const Controller&) {}
};

class Controller {
public:
	Controller();
	bool addObs(Listener& listener);
	bool removeObs(Listener& listener);
	std::list<Device> devices() const;
	Frame frame(int history = 0) const;
	void enableGesture(Gesture::Type type, bool enable = true) const;
	bool isGestureEnabled(Gesture::Type type) const;

public:
	class Impl;
	std::shared_ptr<Impl> pImpl;
};
class Device {
	enum Type {};
public:
	Device();
public:
	class Impl;
	std::shared_ptr<Impl> pImpl;
};
void test();
};