# include "netplus.h"

# include "binary_source.h"
# include "m_qam_mapper.h"
# include "discrete_to_continuous_time.h"
# include "pulse_shaper.h"
# include "iq_modulator.h"
# include "sink.h"

int main(){

	// #####################################################################################################
	// ########################### Signals Declaration and Inicialization ##################################
	// #####################################################################################################
	
	Binary S1{ "S1.sgn" };

	TimeDiscreteAmplitudeDiscreteReal S2{ "S2.sgn" };

	TimeDiscreteAmplitudeDiscreteReal S3{ "S3.sgn" };
	
	TimeContinuousAmplitudeDiscreteReal S4{ "S4.sgn" };

	TimeContinuousAmplitudeDiscreteReal S5{ "S5.sgn" };
	
	TimeContinuousAmplitudeContinuousReal S6{ "S6.sgn" };

	TimeContinuousAmplitudeContinuousReal S7{ "S7.sgn" };
	
	BandpassSignal S8{ "S8.sgn" };

	// #####################################################################################################
	// ########################### Blocks Declaration and Inicialization ###################################
	// #####################################################################################################


	BinarySource B1{ vector<Signal*> {}, vector<Signal*> { &S1 } };
	B1.setMode(PseudoRandom);
	B1.setBitPeriod(1.0 / 50e9);
	B1.setPatternLength(5);
	B1.setNumberOfBits(10000);
	
	MQamMapper B2{ vector<Signal*> { &S1 }, vector<Signal*> { &S2, &S3 } };
	//B2.setIqAmplitudes({ { 1, 1 }, { -1, 1 }, { 1, -1 }, { -1, -1 } });

	DiscreteToContinuousTime B3{ vector<Signal*> { &S2 }, vector<Signal*> { &S4 } };
	B3.setNumberOfSamplesPerSymbol(16);
	
	DiscreteToContinuousTime B4{ vector<Signal*> { &S3 }, vector<Signal*> { &S5 } };
	B4.setNumberOfSamplesPerSymbol(16);

	PulseShaper B5{ vector<Signal*> { &S4 }, vector<Signal*> { &S6 } };
	B5.setRollOffFactor(0.3);
	B5.setImpulseResponseTimeLength(16);
	B5.setSeeBeginningOfImpulseResponse(false);

	PulseShaper B6{ vector<Signal*> { &S5 }, vector<Signal*> { &S7 } };
	B6.setRollOffFactor(0.3);
	B6.setImpulseResponseTimeLength(16);
	B6.setSeeBeginningOfImpulseResponse(false);

	IqModulator B7{ vector<Signal*> { &S6, &S7 }, vector<Signal*> { &S8 } };

	Sink B8{ vector<Signal*> { &S8 }, vector<Signal*> {} };
	B8.setNumberOfSamples(5000);
	B8.setDisplayNumberOfSamples(true);

	// #####################################################################################################
	// ########################### System Declaration and Inicialization ###################################
	// #####################################################################################################

	System MainSystem{ vector<Block*> { &B1, &B2, &B3, &B4, &B5, &B6, &B7, &B8 } };

	// #####################################################################################################
	// #################################### System Run #####################################################
	// #####################################################################################################
	
	MainSystem.run();

	return 0;

}