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
    input.open("123.raw", std::ios::binary);
    output.open("123Zad1.raw", std::ios::binary);

    char sampleInput[2];
    uint16_t sample;
    uint16_t* actualSecond = new uint16_t[44100];   // actual second of sound
    uint16_t* lastSecond = new uint16_t[44100];     // last second of sound

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
        delete[] actualSecond;
        delete[] lastSecond;
    }
}

void echoZad2Transform(std::ifstream& input, std::ofstream& output) {
    input.open("123.raw", std::ios::binary);
    output.open("123Zad2.raw", std::ios::binary);

    char sampleInput[2];
    uint16_t sample;
    uint16_t echoSample;
    uint16_t* actualSeconds = new uint16_t[44100];   // actual second of sound
    uint16_t* firstEchoSeconds = new uint16_t[44100];
    uint16_t* secondEchoSeconds = new uint16_t[44100];
    uint16_t* thirdEchoSeconds = new uint16_t[44100];
    uint16_t* fourthEchoSeconds = new uint16_t[44100];


    int i = 0;
    int echoCount = 0;
    if (input.is_open()) {
        // first seconds without echo loop
        while (i < 44100 && input.read(sampleInput, sizeof(uint16_t))) {
            sample = sampleInput[0] << 8;
            sample += sampleInput[1];
            actualSeconds[i++] = sample;

            sampleInput[1] = sample;
            sampleInput[0] = sample >> 8;

            output.write(sampleInput, sizeof(uint16_t));
        }
        // all with echo loop
        i = 0;
        while (input.read(sampleInput, sizeof(uint16_t))) {
            if (i == 44100) {
                i = 0;
                if (echoCount <= 3 && echoCount >= 0)
                    echoCount++;
            }
            sample = sampleInput[0] << 8;
            sample += sampleInput[1];
            actualSeconds[i] = sample;
            echoSample = 0;
            // == transform ==
            if (echoCount >= 4)
                echoSample = uint16_t(fourthEchoSeconds[i] * 0.1f);
            if (echoCount >= 3) {
                fourthEchoSeconds[i] = thirdEchoSeconds[i];
                echoSample = echoSample + uint16_t(thirdEchoSeconds[i] * 0.2f);
            }
            if (echoCount >= 2) {
                thirdEchoSeconds[i] = secondEchoSeconds[i];
                echoSample = echoSample + uint16_t(secondEchoSeconds[i] * 0.3f);
            }
            if (echoCount >= 1) {
                secondEchoSeconds[i] = firstEchoSeconds[i];
                echoSample = echoSample + uint16_t(firstEchoSeconds[i] * 0.4f);
            }
            firstEchoSeconds[i] = actualSeconds[i];
            sample += echoSample;


            // == transform end ==

            sampleInput[1] = sample;
            sampleInput[0] = sample >> 8;

            output.write(sampleInput, sizeof(uint16_t));
            i++;
        }

        // last four echo seconds loop
        i = 0;
        while (echoCount > 0) {
            if (i == 44100) {
                i = 0;
                if (echoCount <= 4)
                    echoCount--;
            }
            sample = 0;
            if (echoCount >= 4)
                sample = uint16_t(fourthEchoSeconds[i] * 0.1f);
            if (echoCount >= 3)
                sample = sample +  uint16_t(thirdEchoSeconds[i] * 0.2f);
            if (echoCount >= 2)
                sample = sample + uint16_t(secondEchoSeconds[i] * 0.3f);
            if (echoCount >= 1) 
                sample = sample + uint16_t(firstEchoSeconds[i] * 0.4f);
            sampleInput[1] = sample;
            sampleInput[0] = sample >> 8;

            output.write(sampleInput, sizeof(uint16_t));
            i++;
        }

        input.close();
        output.close();
        delete[] actualSeconds;
        delete[] firstEchoSeconds;
        delete[] secondEchoSeconds;
        delete[] thirdEchoSeconds;
        delete[] fourthEchoSeconds;
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