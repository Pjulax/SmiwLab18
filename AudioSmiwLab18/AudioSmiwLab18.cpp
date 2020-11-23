#include <iostream>
#include <fstream>

void echoZad1Transform(std::ifstream& input, std::ofstream& output);
void echoZad2Transform(std::ifstream& input, std::ofstream& output);

int main()
{
    std::ifstream rawInput;
    std::ofstream rawOutput;

    echoZad1Transform(rawInput, rawOutput);
    echoZad2Transform(rawInput, rawOutput);

    return 0;
}

void echoZad1Transform(std::ifstream& input, std::ofstream& output) {
    input.open("HajperPunk.raw", std::ios::binary);
    output.open("HajperPunkZad1.raw", std::ios::binary);

    char sampleInput[2];
    uint16_t sample;
    uint16_t actualSecond[44100];   // actual second of sound
    uint16_t lastSecond[44100];     // last second of sound

    int i = 0;
    if (input.is_open()) {
        // first second without echo loop
        while (i < 44100 && input.read(sampleInput, sizeof(uint16_t))) {
            sample = sampleInput[0] << 8;
            sample += sampleInput[1];
            actualSecond[i++] = sample;

            sampleInput[1] = sample;
            sampleInput[0] = sample >> 8;

            output.write(sampleInput, sizeof(uint16_t));
        }
        // all with echo loop
        i = 0;
        while (input.read(sampleInput, sizeof(uint16_t))) {
            if (i == 44100)
                i = 0;
            sample = sampleInput[0] << 8;
            sample += sampleInput[1];
            actualSecond[i] = sample;

            // == transform ==

            sample += lastSecond[i];

            // == transform end ==
            lastSecond[i] = actualSecond[i];
            sampleInput[1] = sample;
            sampleInput[0] = sample >> 8;

            output.write(sampleInput, sizeof(uint16_t));
            i++;
        }
        // last echo loop
        i = 0;
        while (i < 44100) {
            sample = lastSecond[i];
            sampleInput[1] = sample;
            sampleInput[0] = sample >> 8;

            output.write(sampleInput, sizeof(uint16_t));
            i++;
        }
        input.close();
        output.close();
    }
}

/*
int i = 0;
    if (rawInput.is_open()) {
        while (i < 44100 && rawInput.read(sampleInput, sizeof(uint16_t))) {
            sample = sampleInput[0] << 8;
            sample += sampleInput[1];
            actualSecond[i++] = sample;

            sampleInput[1] = sample;
            sampleInput[0] = sample >> 8;

            rawOutput.write(sampleInput, sizeof(uint16_t));
        }
        i = 0;
        while (rawInput.read(sampleInput, sizeof(uint16_t))) {
            if (i == 44100)
                i = 0;
            sample = sampleInput[0] << 8;
            sample += sampleInput[1];
            actualSecond[i] = sample;

            // == transform ==

            sample += lastSecond[i];

            // == transform end ==
            lastSecond[i] = actualSecond[i];
            sampleInput[1] = sample;
            sampleInput[0] = sample >> 8;

            rawOutput.write(sampleInput, sizeof(uint16_t));
            i++;
        }
        i = 0;
        while (i < 44100) {
            sample = lastSecond[i];
            sampleInput[1] = sample;
            sampleInput[0] = sample >> 8;

            rawOutput.write(sampleInput, sizeof(uint16_t));
            i++;
        }
        rawInput.close();
        rawOutput.close();
    }
*/