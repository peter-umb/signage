#include <driver/i2s.h>
#include <ArduinoFFT.h>

#define SAMPLE_RATE 44100       // Sampling rate in Hz
#define SAMPLES 1024            // Number of samples to collect (must be a power of 2 for FFT)

class AudioProcessor {
public:
  // Constructor with correct ArduinoFFT initialization
  AudioProcessor() : fft(fftInput, fftOutput, SAMPLES, SAMPLE_RATE) {}  // Remove fifth argument

  // Initializes the I2S interface for audio input
  void begin() {
    setupI2S();
  }

  // Continuously listens and detects steel drum tones
  double listenForTones() {
    if (readAudioData()) {
      fft.windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD, true); // Apply Hamming window with normalization
      fft.compute(FFT_FORWARD);                              // Perform FFT with forward direction
      fft.complexToMagnitude();                              // Get magnitude for each frequency component

      // Detect the dominant frequency
      double dominantFrequency = getDominantFrequency();

      // Print the detected frequency (or process it further)
      Serial.print("Detected Frequency: ");
      Serial.println(dominantFrequency);

      return dominantFrequency;
    }
    return 0.0; // Return 0 if no tone was detected
  }

private:
  double fftInput[SAMPLES];                    // Real part of FFT input
  double fftOutput[SAMPLES];                   // Imaginary part of FFT output
  ArduinoFFT<double> fft;                      // Specify template type as double for FFT object

  // Configures the I2S peripheral for reading audio data
  void setupI2S() {
    i2s_config_t i2sConfig = {
        (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
        SAMPLE_RATE,
        I2S_BITS_PER_SAMPLE_16BIT,
        I2S_CHANNEL_FMT_ONLY_LEFT,
        I2S_COMM_FORMAT_STAND_I2S,   // Updated to avoid deprecated warning
        ESP_INTR_FLAG_LEVEL1,
        8,
        64
    };

    i2s_pin_config_t pinConfig = {
      14,                               // bck_io_num (Serial Clock)
      15,                               // ws_io_num (Word Select)
      32,                               // data_in_num (Data Input)
      I2S_PIN_NO_CHANGE                 // data_out_num (not used)
    };

    i2s_driver_install(I2S_NUM_0, &i2sConfig, 0, NULL);
    i2s_set_pin(I2S_NUM_0, &pinConfig);
  }

  // Reads audio data from the microphone and fills the FFT buffer
  bool readAudioData() {
    int16_t buffer[SAMPLES];
    size_t bytesRead;
    i2s_read(I2S_NUM_0, buffer, SAMPLES * sizeof(int16_t), &bytesRead, portMAX_DELAY);

    if (bytesRead < SAMPLES * sizeof(int16_t)) {
      return false; // Not enough data
    }

    // Copy audio samples to FFT input
    for (size_t i = 0; i < SAMPLES; i++) {
      fftInput[i] = (double)buffer[i];
      fftOutput[i] = 0; // Imaginary part is zero
    }

    return true;
  }

  // Analyzes FFT output to find the dominant frequency
  double getDominantFrequency() {
    double peakFrequency = 0;
    double maxMagnitude = 0;

    for (int i = 1; i < SAMPLES / 2; i++) {  // Only need to check the first half
      double magnitude = fftOutput[i];
      if (magnitude > maxMagnitude) {
        maxMagnitude = magnitude;
        peakFrequency = i * (SAMPLE_RATE / SAMPLES);
      }
    }

    return peakFrequency;
  }
};
