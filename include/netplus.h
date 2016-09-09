# ifndef PROGRAM_INCLUDE_NETPLUS_H_
# define PROGRAM_INCLUDE_NETPLUS_H_

# include <iostream>
# include <fstream>
# include <vector>
# include <string>
# include <complex>
# include <algorithm>	// bind1st
# include <functional>	// bind1st

using namespace std;

typedef unsigned int t_binary;
typedef int t_integer;
typedef double t_real;
typedef complex<double> t_complex;

enum signal_value_type {BinaryValue, IntegerValue, RealValue, ComplexValue};

const int MAX_NAME_SIZE = 256;  // Maximum size of names
const long int MAX_Sink_LENGTH = 100000;  // Maximum Sink Block number of values
const int MAX_BUFFER_LENGTH = 10000;  // Maximum Signal buffer length
const int MAX_TOPOLOGY_SIZE = 100;  // Maximum System topology size 
const int MAX_TAPS = 1000;  // Maximum Taps Number
const double PI = 3.1415926535897932384;
const double SPEED_OF_LIGHT = 299792458;


//########################################################################################################################################################
//############################################################## SIGNALS DECLARATION AND DEFINITION ######################################################
//########################################################################################################################################################

// Root class for signals
class Signal {

	long int firstValueToBeSaved{ 1 };				// First value (>= 1) to be saved
	bool saveSignal{ true };
							
public:

	/* Parameters */

	string type;									// Signal type
	signal_value_type valueType;					// Signal samples type

	string fileName{ "" };							// Name of the file where data values are going to be saved
	string folderName{ "signals" };					// folder where signals are going to be saved by default

	
	long int numberOfValuesToBeSaved{ -1 };			// Number of values to be saved, if -1 all values are going to be saved

	int inPosition{ 0 };							// Next position for the input values
	int outPosition{ 0 };							// Next position for the output values
	bool bufferEmpty{ true };						// Flag bufferEmpty
	bool bufferFull{ false };						// Flag bufferFull
	long int numberOfSavedValues{ 0 };				// Number of saved values
	long int count;									// Number of values that have already entered in the buffer

	void *buffer{ NULL };							// Pointer to buffer

	int bufferLength{ 512 };						// Buffer length

	double symbolPeriod{ 1 };										// Signal symbol period (it is the inverse of the symbol rate)
	double samplingPeriod{ 1 };										// Signal sampling period (it is the time space between two samples)
	double samplesPerSymbol{ symbolPeriod / samplingPeriod };		// samplesPerSymbol = symbolPeriod / samplingPeriod;

	double centralWavelength{ 1550E-9 };
	double centralFrequency{ SPEED_OF_LIGHT / centralWavelength };


	/* Methods */

	Signal() {};
	Signal(string fName) {setFileName(fName); };	// Signal constructor
	Signal(string fName, bool sSignal) { setFileName(fName); setSaveSignal(sSignal); };
	Signal(int bLength) { setBufferLength(bLength); };
										// Signal constructor

	~Signal(){ delete buffer; };					// Signal destructor

	void close();									// Empty the signal buffer and close the signal file
	int space();									// Returns the signal buffer space
	int ready();									// Returns the number of samples in the buffer ready to be processed
	void writeHeader();								// Opens the signal file in the default signals directory, \signals, and writes the signal header
	void writeHeader(string signalPath);			// Opens the signal file in the signalPath directory, and writes the signal header

	template<typename T>							// Puts a value in the buffer
	void bufferPut(T value) {
		(static_cast<T *>(buffer))[inPosition] = value;
		if (bufferEmpty) bufferEmpty = false;
		inPosition++;
		if (inPosition == bufferLength) { 
			inPosition = 0; 
			if (saveSignal) {
				if (firstValueToBeSaved <= bufferLength) {
					char *ptr = (char *) buffer;
					ptr = ptr + (firstValueToBeSaved - 1)*sizeof(T);
					ofstream fileHandler("./" + folderName + "/" + fileName, ios::out | ios::binary | ios::app);
					int auxSizeOf = sizeof(T);
					fileHandler.write(ptr, (bufferLength - (firstValueToBeSaved - 1 ))* sizeof(T));
					fileHandler.close();
					firstValueToBeSaved = 1;
				}
				else {
					firstValueToBeSaved = firstValueToBeSaved - bufferLength;
				}
			}
		}
		if (inPosition == outPosition) bufferFull = true;
	};

	void virtual bufferGet();
	void virtual bufferGet(t_binary *valueAddr);
	void virtual bufferGet(t_integer *valueAddr);
	void virtual bufferGet(t_real *valueAddr);
	void virtual bufferGet(t_complex *valueAddr);
	
	void setSaveSignal(bool sSignal){ saveSignal = sSignal; };
	bool const getSaveSignal(){ return saveSignal; };

	void setType(string sType, signal_value_type vType) { type = sType; valueType = vType; };
	void setType(string sType) { type = sType; };
	string getType(){ return type; };

	void setValueType(signal_value_type vType) { valueType = vType; };
	signal_value_type getValueType(){ return valueType; };

	void setFileName(string fName) { fileName = fName; };
	string getFileName(){ return fileName; };

	void setFolderName(string fName) { folderName = fName; };
	string getFolderName(){ return folderName; };
	
	void setBufferLength(int bLength) { bufferLength = bLength; };
	int getBufferLength(){ return bufferLength; };

	void setFirstValueToBeSaved(long int fValueToBeSaved) { firstValueToBeSaved = fValueToBeSaved; };
	long int getFirstValueToBeSaved(){ return firstValueToBeSaved; };

	void setNumberOfValuesToBeSaved(long int nOfValuesToBeSaved) { numberOfValuesToBeSaved = nOfValuesToBeSaved; };
	long int getNumberOfValuesToBeSaved(){ return numberOfValuesToBeSaved; };

	void setSymbolPeriod(double sPeriod) { symbolPeriod = sPeriod; samplesPerSymbol = symbolPeriod / samplingPeriod; };
	double getSymbolPeriod() { return symbolPeriod; };

	void setSamplingPeriod(double sPeriod) { samplingPeriod = sPeriod; samplesPerSymbol = symbolPeriod / samplingPeriod; };
	double getSamplingPeriod(){ return samplingPeriod; };

	void setSamplesPerSymbol(double sPeriod) { samplesPerSymbol = sPeriod; symbolPeriod = samplesPerSymbol * samplingPeriod; samplingPeriod = symbolPeriod / samplesPerSymbol; };
	double getSamplesPerSymbol(){ return samplesPerSymbol; };

	void setCentralFrequency(double cFrequency){ centralFrequency = cFrequency; centralWavelength = SPEED_OF_LIGHT / centralFrequency; }
	double getCentralFrequency(){ return centralFrequency; };

	void setCentralWavelength(double cWavelength){ centralWavelength = cWavelength; centralFrequency = SPEED_OF_LIGHT / centralWavelength; }
	double getCentralWavelength(){ return centralWavelength; }

};


class TimeDiscrete : public Signal {
public:
	TimeDiscrete(string fName) { setFileName(fName); }
	TimeDiscrete(){}
};


class TimeDiscreteAmplitudeDiscrete : public TimeDiscrete {
public:
	TimeDiscreteAmplitudeDiscrete(string fName) { setFileName(fName); }
	TimeDiscreteAmplitudeDiscrete(){}
};


class TimeDiscreteAmplitudeContinuous : public TimeDiscrete {
public:
	TimeDiscreteAmplitudeContinuous(string fName) { setFileName(fName); }
	TimeDiscreteAmplitudeContinuous(){}
};


class TimeDiscreteAmplitudeDiscreteReal : public TimeDiscreteAmplitudeDiscrete {
public:
	TimeDiscreteAmplitudeDiscreteReal(string fName) { setType("TimeDiscreteAmplitudeDiscreteReal", RealValue); setFileName(fName); if (buffer == nullptr) buffer = new t_real[bufferLength]; }
	TimeDiscreteAmplitudeDiscreteReal(string fName, int bLength) { setType("TimeDiscreteAmplitudeDiscreteReal", RealValue); setFileName(fName); setBufferLength(bLength);  if (buffer == nullptr) buffer = new t_real[bLength]; }
	TimeDiscreteAmplitudeDiscreteReal(int bLength) { setType("TimeDiscreteAmplitudeDiscreteReal", RealValue); setBufferLength(bLength); if (buffer == nullptr) buffer = new t_real[bLength]; }
	TimeDiscreteAmplitudeDiscreteReal(){ setType("TimeDiscreteAmplitudeDiscreteReal", RealValue); if (buffer == nullptr) buffer = new t_real[bufferLength]; }
};


class TimeDiscreteAmplitudeDiscreteComplex : public TimeDiscreteAmplitudeDiscrete {
	
public:
	TimeDiscreteAmplitudeDiscreteComplex(string fName) { setFileName(fName); }
	TimeDiscreteAmplitudeDiscreteComplex() {}
};


class Binary : public TimeDiscreteAmplitudeDiscrete {
	
public:
	Binary(string fName) { setType("Binary", BinaryValue);  setFileName(fName); if (buffer == nullptr) buffer = new t_binary[bufferLength]; }
	Binary(string fName, int bLength) { setType("Binary", BinaryValue);  setFileName(fName); setBufferLength(bLength); if (buffer == nullptr) buffer = new t_binary[bLength]; }
	Binary(int bLength) { setType("Binary", BinaryValue);  setBufferLength(bLength); if (buffer == nullptr) buffer = new t_binary[bLength]; }
	Binary() { setType("Binary", BinaryValue); if (buffer == nullptr) buffer = new t_binary[bufferLength]; }

};


class TimeDiscreteAmplitudeContinuousReal : public TimeDiscreteAmplitudeContinuous {
public:
	TimeDiscreteAmplitudeContinuousReal(string fName) { setType("TimeDiscreteAmplitudeContinuousReal", RealValue); setFileName(fName); if (buffer == nullptr) buffer = new t_real[bufferLength]; }
	TimeDiscreteAmplitudeContinuousReal(string fName, int bLength) { setType("TimeDiscreteAmplitudeContinuousReal", RealValue); setFileName(fName); setBufferLength(bLength); if (buffer == nullptr) buffer = new t_real[bLength]; }
	TimeDiscreteAmplitudeContinuousReal(int bLength) { setType("TimeDiscreteAmplitudeContinuousReal", RealValue); setBufferLength(bLength); if (buffer == nullptr) buffer = new t_real[bLength]; }
	TimeDiscreteAmplitudeContinuousReal(){ setType("TimeDiscreteAmplitudeContinuousReal", RealValue); if (buffer == nullptr) buffer = new t_real[bufferLength]; }

};


class TimeDiscreteAmplitudeContinuousComplex : public TimeDiscreteAmplitudeContinuous {
public:
	TimeDiscreteAmplitudeContinuousComplex(string fName) { setType("TimeDiscreteAmplitudeContinuousComplex", ComplexValue); setFileName(fName); if (buffer == nullptr) buffer = new t_complex[bufferLength]; }
	TimeDiscreteAmplitudeContinuousComplex(string fName, int bLength) { setType("TimeDiscreteAmplitudeContinuousComplex", ComplexValue); setFileName(fName); setBufferLength(bLength); if (buffer == nullptr) buffer = new t_complex[bLength]; }
	TimeDiscreteAmplitudeContinuousComplex(int bLength) { setType("TimeDiscreteAmplitudeContinuousComplex", ComplexValue); setBufferLength(bLength); if (buffer == nullptr) buffer = new t_complex[bLength]; }
	TimeDiscreteAmplitudeContinuousComplex(){ setType("TimeDiscreteAmplitudeContinuousComplex", ComplexValue); if (buffer == nullptr) buffer = new t_complex[bufferLength]; }
};


class TimeContinuous : public Signal {
public:
	TimeContinuous(){}
};


class TimeContinuousAmplitudeDiscrete : public TimeContinuous {
public:
	TimeContinuousAmplitudeDiscrete(){}
};


class TimeContinuousAmplitudeContinuous : public TimeContinuous {
public:
	TimeContinuousAmplitudeContinuous(){}
};


class TimeContinuousAmplitudeDiscreteReal : public TimeContinuousAmplitudeDiscrete {
public:
	TimeContinuousAmplitudeDiscreteReal(string fName) { setType("TimeContinuousAmplitudeDiscreteReal", RealValue);  setFileName(fName); if (buffer == nullptr) buffer = new t_real[bufferLength]; }
	TimeContinuousAmplitudeDiscreteReal(string fName, int bLength) { setType("TimeContinuousAmplitudeDiscreteReal", RealValue);  setFileName(fName); setBufferLength(bLength); if (buffer == nullptr) buffer = new t_real[bLength]; }
	TimeContinuousAmplitudeDiscreteReal(int bLength) { setType("TimeContinuousAmplitudeDiscreteReal", RealValue);  setBufferLength(bLength); if (buffer == nullptr) buffer = new t_real[bLength]; }
	TimeContinuousAmplitudeDiscreteReal(){ setType("TimeContinuousAmplitudeDiscreteReal", RealValue); if (buffer == nullptr) buffer = new t_real[bufferLength]; }

};


class TimeContinuousAmplitudeDiscreteComplex : public TimeContinuousAmplitudeDiscrete {
public:
	TimeContinuousAmplitudeDiscreteComplex(string fName) { setType("TimeContinuousAmplitudeDiscreteComplex", ComplexValue); setFileName(fName); if (buffer == nullptr) buffer = new t_complex[bufferLength]; }
	TimeContinuousAmplitudeDiscreteComplex(string fName, int bLength) { setType("TimeContinuousAmplitudeDiscreteComplex", ComplexValue); setFileName(fName); setBufferLength(bLength); if (buffer == nullptr) buffer = new t_complex[bLength]; }
	TimeContinuousAmplitudeDiscreteComplex(int bLength) { setType("TimeContinuousAmplitudeDiscreteComplex", ComplexValue); setBufferLength(bLength); if (buffer == nullptr) buffer = new t_complex[bLength]; }
	TimeContinuousAmplitudeDiscreteComplex(){ setType("TimeContinuousAmplitudeDiscreteComplex", ComplexValue); if (buffer == nullptr) buffer = new t_complex[bufferLength]; }
};


class TimeContinuousAmplitudeContinuousReal : public TimeContinuousAmplitudeContinuous {
public:
	TimeContinuousAmplitudeContinuousReal(string fName) { setType("TimeContinuousAmplitudeContinuousReal", RealValue); setFileName(fName); if (buffer == nullptr) buffer = new t_real[bufferLength]; }
	TimeContinuousAmplitudeContinuousReal(string fName, int bLength) { setType("TimeContinuousAmplitudeContinuousReal", RealValue); setFileName(fName); setBufferLength(bLength); if (buffer == nullptr) buffer = new t_real[bLength]; }
	TimeContinuousAmplitudeContinuousReal(int bLength) { setType("TimeContinuousAmplitudeContinuousReal", RealValue); setBufferLength(bLength); if (buffer == nullptr) buffer = new t_real[bLength]; }
	TimeContinuousAmplitudeContinuousReal(){ setType("TimeContinuousAmplitudeContinuousReal", RealValue); if (buffer == nullptr) buffer = new t_real[bufferLength]; }

};


class TimeContinuousAmplitudeContinuousComplex : public TimeContinuousAmplitudeContinuous {
public:
	TimeContinuousAmplitudeContinuousComplex(string fName) { setType("TimeContinuousAmplitudeContinuousComplex", ComplexValue); setFileName(fName); if (buffer == nullptr) buffer = new t_complex[bufferLength]; }
	TimeContinuousAmplitudeContinuousComplex(string fName, int bLength) { setType("TimeContinuousAmplitudeContinuousComplex", ComplexValue); setFileName(fName); setBufferLength(bLength); if (buffer == nullptr) buffer = new t_complex[bLength]; }
	TimeContinuousAmplitudeContinuousComplex(int bLength) { setType("TimeContinuousAmplitudeContinuousComplex", ComplexValue); setBufferLength(bLength); if (buffer == nullptr) buffer = new t_complex[bLength]; }
	TimeContinuousAmplitudeContinuousComplex(){ setType("TimeContinuousAmplitudeContinuousComplex", ComplexValue); if (buffer == nullptr) buffer = new t_complex[bufferLength]; }
};

class BandpassSignal : public TimeContinuousAmplitudeContinuousComplex {
public:
	BandpassSignal(string fName) { setType("BandpassSignal", ComplexValue); setFileName(fName); if (buffer == nullptr) buffer = new t_complex[bufferLength]; }
	BandpassSignal(string fName, int bLength) { setType("BandpassSignal", ComplexValue); setFileName(fName); setBufferLength(bLength); if (buffer == nullptr) buffer = new t_complex[bLength]; }
	BandpassSignal(int bLength) { setType("BandpassSignal", ComplexValue); setBufferLength(bLength); if (buffer == nullptr)buffer = new t_complex[bLength]; }
	BandpassSignal(){ setType("BandpassSignal", ComplexValue); if (buffer == nullptr) buffer = new t_complex[bufferLength]; }

};

class MultiModeBandpassSignal : BandpassSignal {
public:
	MultiModeBandpassSignal(int nBandpassSignals) {

	};
private:
	int numberOfBandpassSignals;
	vector<BandpassSignal> bandpasslSignals;
	vector<double> centralWavelengths;
	vector<double> centralFrequencies;
};

//########################################################################################################################################################
//########################################################## GENERIC BLOCK DECLARATIONS AND DEFINITIONS ##################################################
//########################################################################################################################################################


// Descrives a generic Block.
class Block {

 public:

	/* Input Parameters */
	int numberOfInputSignals;
	int numberOfOutputSignals;
	vector<Signal *> inputSignals;
	vector<Signal *> outputSignals;

	/* Methods */
	Block(){};
	Block(vector<Signal*> &InputSig, vector<Signal*> &OutputSig);

	void initializeBlock();
	virtual void initialize(void){};

	virtual bool runBlock();

	void terminateBlock();
	virtual void terminate(void){};
	
};


class SuperBlock : public Block {

	/* State Variables */

	vector<Block*> moduleBlocks;

	/* Input Parameters */

	bool saveInternalSignals{ false };

public:


	/* Methods */

	SuperBlock(vector<Signal *> &inputSignal, vector<Signal *> &outputSignal) :Block(inputSignal, outputSignal){ setSaveInternalSignals(false); };

	void initialize(void);

	bool runBlock(void);

	void terminate(void);

	/* Set Methods */

	void setModuleBlocks(vector<Block*> mBlocks){ moduleBlocks = mBlocks; };

	void setSaveInternalSignals(bool sSignals);
	bool const getSaveInternalSignals(void){ return saveInternalSignals; };

};

class FIR_Filter : public Block {

	/* State Variable */
	vector<t_real> delayLine;

	bool saveImpulseResponse{ true };
	string impulseResponseFilename{ "impulse_response.imp" };

	/* Input Parameters */
	bool seeBeginningOfImpulseResponse{ false };

public:

	/* State Variable */
	vector<t_real> impulseResponse;

	/* Input Parameters */
	int impulseResponseLength;							// filter order + 1 (filter order = number of delays)


	/* Methods */

	FIR_Filter(vector<Signal *> &InputSig, vector<Signal *> OutputSig) :Block(InputSig, OutputSig){};

	void initializeFIR_Filter(void);

	bool runBlock(void);

	void terminate(void){};

	void setSaveImpulseResponse(bool sImpulseResponse) { saveImpulseResponse = sImpulseResponse; };
	bool getSaveImpulseResponse(void){ return saveImpulseResponse; };

	void setImpulseResponseLength(int iResponseLength) { impulseResponseLength = iResponseLength; };
	int const getImpulseResponseLength(){ return impulseResponseLength; }

	void setSeeBeginningOfImpulseResponse(bool sBeginning){ seeBeginningOfImpulseResponse = sBeginning; };
	bool const getSeeBeginningOfImpulseResponse(){ return seeBeginningOfImpulseResponse; };

};



// Generates a complex signal knowing the real part and the complex part.
class RealToComplex : public Block {
 public:
	 RealToComplex(vector<Signal *> &InputSig, vector<Signal *> &OutputSig);
	 bool runBlock(void);
 //private:
};

// Separates the complex signal into two parts: real and imaginary.
class ComplexToReal : public Block {
 public:
	 ComplexToReal(vector<Signal *> &InputSig, vector<Signal *> &OutputSig);
  bool runBlock(void);
 //private:
};




//########################################################################################################################################################
//########################################################## GENERIC SYSTEM DECLARATIONS AND DEFINITIONS #################################################
//########################################################################################################################################################


class System {

 public:
  System(vector<Block *> &MainSystem);	
  void terminate();										
  void run();
  void run(string signalPath);

  string signalsFolder{ "signals" };
  char fileName[MAX_NAME_SIZE];  // Name of the file with system description (.sdf)
  char name[MAX_NAME_SIZE];  // System Name
  int numberOfBlocks;  // Number of system Blocks
  int (*topology)[MAX_TOPOLOGY_SIZE];  // Relationship matrix
  vector<Block *> SystemBlocks;  // Pointer to an array of pointers to Block objects
};

# endif // PROGRAM_INCLUDE_NETPLUS_H_


