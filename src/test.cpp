#include "geefish.h"
#include <iostream>
using namespace std;
using namespace GeeFish;

class SampleListener : public Listener {
  public:
    virtual void onFrame(const Controller&){
    	cout << "sample listener gets a new frame" << endl;
    }
};

int main(){
	SampleListener listener;
	GeeFish::Controller ctl;
	ctl.addObs(listener);
	cout << "press any key to quit" << endl;
	getchar();
	return 0;
}
