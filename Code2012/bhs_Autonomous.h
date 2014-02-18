#ifndef BHS_AUTONOMOUS_H
#define BHS_AUTONOMOUS_H

class bhs_Autonomous {
public:
	virtual ~bhs_Autonomous() {}
	
	virtual void init() = 0;
	virtual void run() = 0;
	
private:
	
};

#endif
