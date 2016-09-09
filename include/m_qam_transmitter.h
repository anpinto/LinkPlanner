# ifndef M_QAM_TRANSMITTER_H_
# define M_QAM_TRANSMITTER_H_

# include "netplus.h"
# include "binary_source.h"
# include "m_qam_mapper.h"
# include "discrete_to_continuous_time.h"
# include "pulse_shaper.h"
# include "iq_modulator.h"

//using namespace std;


class MQamTransmitter : public SuperBlock {

	/* State Variables */

	// #####################################################################################################
	// ################## Internal Signals Declaration and Inicialization ##################################
	// #####################################################################################################

	Binary S1{ "MQAM1.sgn" };

	TimeDiscreteAmplitudeDiscreteReal S2{ "MQAM2.sgn" };

	TimeDiscreteAmplitudeDiscreteReal S3{ "MQAM3.sgn" };

	TimeContinuousAmplitudeDiscreteReal S4{ "MQAM4.sgn" };

	TimeContinuousAmplitudeDiscreteReal S5{ "MQAM5.sgn" };

	TimeContinuousAmplitudeContinuousReal S6{ "MQAM6.sgn" };

	TimeContinuousAmplitudeContinuousReal S7{ "MQAM7.sgn" };

	BandpassSignal S8{ "MQAM8.sgn" };


	// #####################################################################################################
	// ########################### Blocks Declaration and Inicialization ###################################
	// #####################################################################################################


	BinarySource B1{ vector<Signal*> {}, vector<Signal*> { &S1 } };

	MQamMapper B2{ vector<Signal*> { &S1 }, vector<Signal*> { &S2, &S3 } };

	DiscreteToContinuousTime B3{ vector<Signal*> { &S2 }, vector<Signal*> { &S4 } };

	DiscreteToContinuousTime B4{ vector<Signal*> { &S3 }, vector<Signal*> { &S5 } };

	PulseShaper B5{ vector<Signal*> { &S4 }, vector<Signal*> { &S6 } };

	PulseShaper B6{ vector<Signal*> { &S5 }, vector<Signal*> { &S7 } };

	IqModulator B7{ vector<Signal*> { &S6, &S7 }, vector<Signal*> { &S8 } };



	/* Input Parameters */


public:

	/* Methods */

	MQamTransmitter(vector<Signal *> &inputSignal, vector<Signal *> &outputSignal):SuperBlock(inputSignal, outputSignal){ setModuleBlocks({ &B1, &B2, &B3, &B4, &B5, &B6, &B7 }); };

	/* Set Methods */

	void set(int opt);

	void setMode(BinarySourceMode m) { B1.setMode(m); };
	BinarySourceMode const getMode(void) { return B1.getMode(); };

	void setProbabilityOfZero(double pZero) { B1.setProbabilityOfZero(pZero); };
	double const getProbabilityOfZero(void) { return B1.getProbabilityOfZero(); };

	void setBitStream(string bStream) { B1.setBitStream(bStream); };
	string const getBitStream(void) { return B1.getBitStream(); };

	void setNumberOfBits(long int nOfBits) { B1.setNumberOfBits(nOfBits); }
	long int const getNumberOfBits(void) { return B1.getNumberOfBits();  }

	void setPatternLength(int pLength) { B1.setPatternLength(pLength); }
	int const getPatternLength(void) { return B1.getPatternLength(); }

	void setBitPeriod(double bPeriod) {B1.setBitPeriod(bPeriod);};
	double const getBitPeriod(void) { return B1.getBitPeriod(); }

	void setM(int mValue){B2.m = mValue;};
	int const getM(void) { return B2.m; };

	void setIqAmplitudes(vector<t_iqValues> iqAmplitudesValues){ B2.m = iqAmplitudesValues.size(); B2.iqAmplitudes.resize(B2.m); B2.iqAmplitudes = iqAmplitudesValues; };
	vector<t_iqValues> const getIqAmplitudes(void){ return B2.iqAmplitudes; };

	void setNumberOfSamplesPerSymbol(int n){ B3.setNumberOfSamplesPerSymbol(n); B4.setNumberOfSamplesPerSymbol(n); };
	int const getNumberOfSamplesPerSymbol(void){ return B3.getNumberOfSamplesPerSymbol(); };

	void setRollOffFactor(double rOffFactor){ B5.setRollOffFactor(rOffFactor); B6.setRollOffFactor(rOffFactor); };
	double const getRollOffFactor(void){ return B5.getRollOffFactor(); };

	void setSeeBeginningOfImpulseResponse(bool sBeginningOfImpulseResponse){ B5.setSeeBeginningOfImpulseResponse(sBeginningOfImpulseResponse); B6.setSeeBeginningOfImpulseResponse(sBeginningOfImpulseResponse); };
	double const getSeeBeginningOfImpulseResponse(void){ return B5.getSeeBeginningOfImpulseResponse(); };

	void setOutputOpticalPower(t_real outOpticalPower) { B7.outputOpticalPower = outOpticalPower; };
	t_real const getOutputOpticalPower(void) { return B7.outputOpticalPower; };

	void setOutputOpticalPower_dBm(t_real outOpticalPower_dBm) { B7.outputOpticalPower = 1e-3*pow(10, outOpticalPower_dBm / 10); };
	t_real const getOutputOpticalPower_dBm(void) { return 10*log10(B7.outputOpticalPower/1e-3); }

};

#endif