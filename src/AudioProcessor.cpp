#include "AudioProcessor.h"

// Constructor with correct ArduinoFFT initialization
AudioProcessor::AudioProcessor(boolean debug) : fft(fftInput, fftOutput, SAMPLES, SAMPLE_RATE), debug(debug) {}

// Initializes the I2S interface for audio input
void AudioProcessor::begin() {
    setupI2S();
}

// Continuously listens and detects steel drum tones
double AudioProcessor::listenForTones() {
    if (readAudioData()) {
        fft.windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD, true);  // Apply Hamming window with normalization
        fft.compute(FFT_FORWARD);                               // Perform FFT with forward direction
        fft.complexToMagnitude();                               // Get magnitude for each frequency component

        // Detect the dominant frequency
        return getDominantFrequency();
    }
    return 0.0; // Return 0 if no tone was detected
}

// Configures the I2S peripheral for reading audio data
void AudioProcessor::setupI2S() {
    i2s_config_t i2sConfig = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
        .sample_rate = SAMPLE_RATE,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 8,
        .dma_buf_len = 64
    };

    i2s_pin_config_t pinConfig = {
        .bck_io_num = 14,     // Serial Clock
        .ws_io_num = 15,      // Word Select
        .data_out_num = I2S_PIN_NO_CHANGE,
        .data_in_num = 32     // Data Input
    };

    esp_err_t err = i2s_driver_install(I2S_NUM_0, &i2sConfig, 0, NULL);
    if (err != ESP_OK) {
        if (debug) {
            Serial.println("Failed to install I2S driver.");
        }
        return;
    }

    i2s_set_pin(I2S_NUM_0, &pinConfig);

    if (debug) {
        Serial.println("I2S driver installed and configured.");
    }
}

// Reads audio data from the microphone and fills the FFT buffer
bool AudioProcessor::readAudioData() {
    int16_t buffer[SAMPLES];
    size_t bytesRead;
    i2s_read(I2S_NUM_0, buffer, SAMPLES * sizeof(int16_t), &bytesRead, portMAX_DELAY);

    if (bytesRead < SAMPLES * sizeof(int16_t)) {
        if (debug) {
            Serial.print("Not enough audio data read. Bytes read: ");
            Serial.println(bytesRead);
        }
        return false; // Not enough data
    }

    if (debug) {
        Serial.print("Bytes read: ");
        Serial.println(bytesRead);
    }

    // Copy audio samples to FFT input
    for (size_t i = 0; i < SAMPLES; i++) {
        fftInput[i] = (double)buffer[i];
        fftOutput[i] = 0; // Imaginary part is zero
    }

    if (debug) {
        Serial.println("Audio data successfully read into FFT buffer.");
    }

    return true;
}

// Analyzes FFT output to find the dominant frequency
double AudioProcessor::getDominantFrequency() {
    double peakFrequency = 0;
    double maxMagnitude = 0;

    for (int i = 1; i < SAMPLES / 2; i++) {  // Only need to check the first half
        double magnitude = fftInput[i]; // Since fftInput now holds magnitudes after complexToMagnitude
        // double magnitude = fftOutput[i];
        if (magnitude > maxMagnitude) {
            maxMagnitude = magnitude;
            peakFrequency = i * (SAMPLE_RATE / SAMPLES);
        }
    }

    return peakFrequency;
}