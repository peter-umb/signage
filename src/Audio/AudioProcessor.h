#ifndef AUDIOPROCESSOR_H
#define AUDIOPROCESSOR_H

#include <driver/i2s.h>
#include <ArduinoFFT.h>

#define SAMPLE_RATE 44100       // Sampling rate in Hz
#define SAMPLES 1024            // Number of samples to collect (must be a power of 2 for FFT)

class AudioProcessor {
public:
  // Constructor
  AudioProcessor(boolean debug);

  // Initializes the I2S interface for audio input
  void begin();

  // Continuously listens and detects steel drum tones
  double listenForTones();

private:
  boolean debug;

  double fftInput[SAMPLES];    // Real part of FFT input
  double fftOutput[SAMPLES];   // Imaginary part of FFT output
  ArduinoFFT<double> fft;      // FFT object with template type double

  // Sets up I2S for reading audio data
  void setupI2S();

  // Reads audio data from the microphone and fills the FFT buffer
  bool readAudioData();

  // Analyzes FFT output to find the dominant frequency
  double getDominantFrequency();
};

#endif