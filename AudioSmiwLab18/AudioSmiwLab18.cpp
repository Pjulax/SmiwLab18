#include <iostream>
#include <fstream>

void echoZad1Transform(std::ifstream& input, std::ofstream& output);
void echoZad2Transform(std::ifstream& input, std::ofstream& output);
void echoZad3Transform(std::ifstream& input, std::ofstream& output);

int main()
{
    std::ifstream rawInput;
    std::ofstream rawOutput;

    echoZad1Transform(rawInput, rawOutput);
    echoZad2Transform(rawInput, rawOutput);
    echoZad3Transform(rawInput, rawOutput);

    return 0;
}

void echoZad1Transform(std::ifstream& input, std::ofstream& output) {
    input.open("12.raw", std::ios::binary);
    output.open("12Zad1.raw", std::ios::binary);

    char sampleInput[2];
    short int sample;
    short int* actualSecond = new short int[44100]{ 0 };   // actual second of sound
    short int* lastSecond = new short int[44100]{ 0 };     // last second of sound

    int i = 0;
    if (input.is_open()) {
        // first second without echo loop
        while (i < 44100 && input.read(sampleInput, sizeof(short int))) {
            sample = sampleInput[0] << 8;
            sample += sampleInput[1];
            actualSecond[i++] = sample;

            sampleInput[1] = sample;
            sampleInput[0] = sample >> 8;

            output.write(sampleInput, sizeof(short int));
        }
        // all with echo loop
        i = 0;
        while (input.read(sampleInput, sizeof(short int))) {
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

            output.write(sampleInput, sizeof(short int));
            i++;
        }
        // last echo loop
        i = 0;
        while (i < 44100) {
            sample = lastSecond[i];
            sampleInput[1] = sample;
            sampleInput[0] = sample >> 8;

            output.write(sampleInput, sizeof(short int));
            i++;
        }
        input.close();
        output.close();
        delete[] actualSecond;
        delete[] lastSecond;
    }
}

void echoZad2Transform(std::ifstream& input, std::ofstream& output) {
    input.open("12.raw", std::ios::binary);
    output.open("12Zad2.raw", std::ios::binary);

    char sampleInput[2];
    short int sample;
    short int echoSample;
    short int* actualSeconds = new short int[44100]{ 0 };   // actual second of sound
    short int* firstEchoSeconds = new short int[44100]{ 0 };
    short int* secondEchoSeconds = new short int[44100]{ 0 };
    short int* thirdEchoSeconds = new short int[44100]{ 0 };
    short int* fourthEchoSeconds = new short int[44100]{ 0 };


    int i = 0;
    int echoCount = 0;
    if (input.is_open()) {
        // first seconds without echo loop
        while (i < 44100 && input.read(sampleInput, sizeof(short int))) {
            sample = sampleInput[0] << 8;
            sample += sampleInput[1];
            actualSeconds[i++] = sample;

            sampleInput[1] = sample;
            sampleInput[0] = sample >> 8;

            output.write(sampleInput, sizeof(short int));
        }
        // all with echo loop
        i = 0;
        while (input.read(sampleInput, sizeof(short int))) {
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
                echoSample = fourthEchoSeconds[i];
            if (echoCount >= 3) {
                fourthEchoSeconds[i] = thirdEchoSeconds[i];
                echoSample = echoSample + thirdEchoSeconds[i];
            }
            if (echoCount >= 2) {
                thirdEchoSeconds[i] = secondEchoSeconds[i];
                echoSample = echoSample + secondEchoSeconds[i];
            }
            if (echoCount >= 1) {
                secondEchoSeconds[i] = firstEchoSeconds[i];
                echoSample = echoSample + firstEchoSeconds[i];
            }
            firstEchoSeconds[i] = actualSeconds[i];
            sample += echoSample;


            // == transform end ==

            sampleInput[1] = sample;
            sampleInput[0] = sample >> 8;

            output.write(sampleInput, sizeof(short int));
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
                sample = fourthEchoSeconds[i];
            if (echoCount >= 3)
                sample = sample + thirdEchoSeconds[i];
            if (echoCount >= 2)
                sample = sample + secondEchoSeconds[i];
            if (echoCount >= 1) 
                sample = sample + firstEchoSeconds[i];
            sampleInput[1] = sample;
            sampleInput[0] = sample >> 8;

            output.write(sampleInput, sizeof(short int));
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

void echoZad3Transform(std::ifstream& input, std::ofstream& output) {
    input.open("12.raw", std::ios::binary);
    output.open("12Zad3.raw", std::ios::binary);

    char sampleInput[2];
    short int* lastHighByte = new short int[44100]{ 0 };     // last second of sound
    short int* lastLowByte = new short int[44100]{ 0 };     // last second of sound

    int i = 0;
    if (input.is_open()) {
        // first second without echo loop
        while (i < 44100 && input.read(sampleInput, sizeof(short int))) {
            lastLowByte[i] = ((char)sampleInput[0] << 8) * 0.6f;
            lastHighByte[i] += sampleInput[1] * 0.6f;
            i++;
            output.write(sampleInput, sizeof(short int));
        }
        // all with echo loop
        i = 0;
        while (input.read(sampleInput, sizeof(short int))) {
            if (i == 44100)
                i = 0;
            lastLowByte[i] = ((char)sampleInput[0] << 8) + lastLowByte[i] * 0.6f;
            lastHighByte[i] = sampleInput[1] + lastHighByte[i] * 0.6f;

            sampleInput[1] = lastHighByte[i];
            sampleInput[0] = lastLowByte[i];

            output.write(sampleInput, sizeof(short int));
            i++;
        }
        // last echo loop
        i = 0;
        while (i < 44100) {
            if (i == 44100)
                i = 0;
            if (lastLowByte[i] <= 3 && lastHighByte[i] <= 3)
                break;
            lastLowByte[i] = lastLowByte[i] * 0.6f;
            lastHighByte[i] = lastHighByte[i] * 0.6f;

            output.write(sampleInput, sizeof(short int));
            i++;
        }
        input.close();
        output.close();
        delete[] lastLowByte;
        delete[] lastHighByte;
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