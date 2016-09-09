# ifndef SINK_H_
# define SINK_H_

# include "netplus.h"

class Sink : public Block {

	/* State Variables */

	bool displayNumberOfSamples{ false };

public:

	/* Input Parameters */

	long int numberOfSamples{ -1 };

	/* Methods */

	Sink(vector<Signal *> &InputSig, vector<Signal *> &OutputSig);

	bool runBlock(void);

	void setNumberOfSamples(long int nOfSamples){ numberOfSamples = nOfSamples; };

	void setDisplayNumberOfSamples(bool opt) { displayNumberOfSamples = opt; };

};

#endif