# ifndef LASER_H_
# define LASER_H_

# include "netplus.h"

class Laser : public Block {

	// State variables

 public:

	 // Input parameters
	 double centralWavelength{ 1550e-9 };
	 double initialPhase{ 0 };

	 double samplingPeriod{ 1e-12 };

	// Methods
	Laser(vector<Signal *> &InputSig, vector<Signal *> &OutputSig);
	
	bool runBlock(void);

	void setCentralWavelength(double cWavelength) { centralWavelength = cWavelength; };
	void setInitialPhase(double iPhase) { initialPhase = iPhase; };
	void setSamplingPeriod(double sampPeriod) {  samplingPeriod = sampPeriod; };
};

# endif


