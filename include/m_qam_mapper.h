# ifndef M_QAM_MAPPER_H_
# define M_QAM_MAPPER_H_

# include <vector>	     // vector container
# include <math.h>       // log2 

# include "netplus.h"

using namespace std;

struct t_iqValues {
	t_real i;
	t_real q;
};

/* Realizes the M-QAM mapping. */
class MQamMapper : public Block {

	/* State Variables */

	t_integer auxBinaryValue{ 0 };
	t_integer auxSignalNumber{ 0 };


public:

	/* Input Parameters */

	t_integer m{ 4 };
	vector<t_iqValues> iqAmplitudes;


	/* Methods */

	MQamMapper(vector<Signal *> &InputSig, vector<Signal *> &OutputSig) :Block(InputSig, OutputSig) {};

	void initialize(void);

	bool runBlock(void);

	void setM(int mValue);		// m should be of the form m = 2^n, with n integer;

	void setIqAmplitudes(vector<t_iqValues> iqAmplitudesValues);

};

#endif