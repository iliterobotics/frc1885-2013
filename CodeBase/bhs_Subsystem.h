#ifndef BHS_SUBSYSTEM_H_
#define BHS_SUBSYSTEM_H_

class bhs_Subsystem {
	public:
		//virtual void init() = 0;
		void init();
		virtual void run() = 0;
};

#endif //BHS_SUBSYSTEM_H_
