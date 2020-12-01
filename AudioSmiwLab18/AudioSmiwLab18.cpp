#include <iostream>
#include <fstream>

void echoZad1Transform(std::ifstream& input, std::ofstream& output, std::string filename);
void echoZad2Transform(std::ifstream& input, std::ofstream& output, std::string filename);
void echoZad3Transform(std::ifstream& input, std::ofstream& output, std::string filename);

int main()
{
    std::ifstream rawInput;
    std::ofstream rawOutput;
    std::string filename = "123";
    echoZad1Transform(rawInput, rawOutput, filename);
    echoZad2Transform(rawInput, rawOutput, filename);
    echoZad3Transform(rawInput, rawOutput, filename);

    return 0;
}

void echoZad1Transform(std::ifstream& input, std::ofstream& output, std::string filename) {
    input.open(filename + ".raw", std::ios::binary);
    output.open(filename + "Zad1.raw", std::ios::binary);

    char sampleInput[2];
    short int sample;                                      // zmienna przechouwjaca 16 bitow
    short int* actualSecond = new short int[44100]{ 0 };   // przechowuje aktualna sekunde
    short int* lastSecond = new short int[44100]{ 0 };     // przechowuje ostatnia analizowana sekunde

    int i = 0;
    if (input.is_open()) {
        // pobranie pierwszej sekundy bez echa
        while (i < 44100 && input.read(sampleInput, sizeof(short int))) {
                                        // pobranie dolnych i gornych bitow 
            sample = sampleInput[0] << 8;  
            sample += sampleInput[1];

            lastSecond[i++] = sample; // zapisanie pobranych bitow do zm. przechowujacej ostatnia sekudne
                                    
                                      // zapisanie pobranych bitow do pliku wejsciowego
            sampleInput[1] = sample;  
            sampleInput[0] = sample >> 8;

            output.write(sampleInput, sizeof(short int));
        }
        // pobranie wszystkich sekund ktore beda zawierac echo
        i = 0;
        while (input.read(sampleInput, sizeof(short int))) {
            if (i == 44100) // sprawdzenie czy wczytano cala sekunde jesli tak to wyzerowanie           
                i = 0;
                                        
                                        // pobranie dolnych i gornych bitow 
            sample = sampleInput[0] << 8;  
            sample += sampleInput[1];

            actualSecond[i] = sample;  // dodanie pobranych bitow do tablicy przechowujacej aktualna sekunde

            // == transfromacja ==

            sample += lastSecond[i];    // dodanie do pobranych bitow przeprzednio pobranych bitow

            // == koniec transformacji ==
            lastSecond[i] = actualSecond[i];    // przypisanie aktualnej sekundy do ostatnej sekudny w celu skorzystania z niej w nastepnej petli
            sampleInput[1] = sample;            // zapisanie przetworzonych danych do pliku
            sampleInput[0] = sample >> 8;

            output.write(sampleInput, sizeof(short int));
            i++;  // inkrementacja licznika tablic przechowujacych sekund
        }
        // pobranie ostatniej sekundy i zapisanie do pliku bez echa
        i = 0;
        while (i < 44100) {
            sample = lastSecond[i]; // 
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

void echoZad2Transform(std::ifstream& input, std::ofstream& output, std::string filename) {
    input.open(filename + ".raw", std::ios::binary);
    output.open(filename + "Zad2.raw", std::ios::binary);

    char sampleInput[2];
    short int sample;
    short int* actualSeconds = new short int[44100]{ 0 };   // aktualna sekunda dzwieku
    short int* firstEchoSeconds = new short int[44100]{ 0 }; // pierwsze sekunda echa
    short int* secondEchoSeconds = new short int[44100]{ 0 }; // drugia sekunda echa
    short int* thirdEchoSeconds = new short int[44100]{ 0 };  // trzecia sekunda echa
    short int* fourthEchoSeconds = new short int[44100]{ 0 }; // czwarta sekunda echa


    int i = 0; // licznik tablicy sekund
    int echoCount = 0; // ilosc zapisanych ech
    if (input.is_open()) {
        // pobranie pierwszej sekundy identycznie jak w poprzednim zadaniu
        while (i < 44100 && input.read(sampleInput, sizeof(short int))) {
            sample = sampleInput[0] << 8;
            sample += sampleInput[1];

            actualSeconds[i] = sample;
            firstEchoSeconds[i] = sample; // dodanie do tablicy zawierajace pierwsze echo pobranych bitow

            sampleInput[1] = sample;
            sampleInput[0] = sample >> 8;
            i++;
            output.write(sampleInput, sizeof(short int));
        }
        // all with echo loop
        i = 0;
        while (input.read(sampleInput, sizeof(short int))) {
            if (i == 44100) {   // sprawdzenie czy pobrano ponad sekunde, jesli tak to wyzerowanie
                i = 0;
            }
            sample = sampleInput[0] << 8; // pobranie 16 bitow z pliku 
            sample += sampleInput[1];
            actualSeconds[i] = sample;    // dodanie pobranych bitow do aktualnej sekundy
            // == transform ==

            sample += fourthEchoSeconds[i]; // dodanie do pobranych bitow czwartego echa
            sample += thirdEchoSeconds[i];  // dodanie do pobranych bitow trzeciego echa
            sample += secondEchoSeconds[i]; // dodanie do pobranych bitow drugiego echa
            sample += firstEchoSeconds[i];  // dodanie do pobranych bitow pierwszego echa

            // == transform end ==
            fourthEchoSeconds[i] = thirdEchoSeconds[i]; // dodanie do tablicy czwartego echa trzeciego echa
            thirdEchoSeconds[i] = secondEchoSeconds[i]; // analogicznie dla poprzednich tablic zawierajacych echa
            secondEchoSeconds[i] = firstEchoSeconds[i];
            firstEchoSeconds[i] = actualSeconds[i];

            sampleInput[1] = sample;    // zapisanie zmodyfykowanych danych do pliku
            sampleInput[0] = sample >> 8;

            output.write(sampleInput, sizeof(short int));
            i++;
        }

        // ostantia sekunda dodanie czterech ech
        i = 0;
        int echoCount = 4;
        while (echoCount > 0) {
            if (i == 44100) {   // sprawdzenie czy pobrano ponad sekunde, jesli tak to wyzerowanie
                i = 0;
                if (echoCount <= 4) // sprawdzenie czy nalezy zmniejszyc ilsoc ech
                    echoCount--;
            }
            sample = 0; // wyzerowanie zmmiennj wynikowej
            // dodanie pozostalych ech do wyniku
            if (echoCount >= 4)
                sample = fourthEchoSeconds[i];
            if (echoCount >= 3)
                sample = sample + thirdEchoSeconds[i];
            if (echoCount >= 2)
                sample = sample + secondEchoSeconds[i];
            if (echoCount >= 1) 
                sample = sample + firstEchoSeconds[i];
            // wyprowadzenie wyniku do pliku
            sampleInput[1] = sample;
            sampleInput[0] = sample >> 8;

            output.write(sampleInput, sizeof(short int));
            i++; // inkrementacja indeksu tablicy sekund
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

void echoZad3Transform(std::ifstream& input, std::ofstream& output, std::string filename) {
    input.open(filename + ".raw", std::ios::binary);
    output.open(filename + "Zad3.raw", std::ios::binary);

    char sampleInput[2];
    short int* lastHighByte = new short int[44100]{ 0 };     // tablica zawierajaca pierwsza polowke sekunde
    short int* lastLowByte = new short int[44100]{ 0 };     // tablica zawierajaca druga polowke sekunde

    int i = 0;
    if (input.is_open()) {
        // pobranie pierwszej sekundy bez echa i zapisanie do odpowiedich polowek
        while (i < 44100 && input.read(sampleInput, sizeof(short int))) {
            lastLowByte[i] = ((char)sampleInput[0] << 8) * 0.6f; // pobranie bitow i wyciszenie ich
            lastHighByte[i] += sampleInput[1] * 0.6f; // pobranie bitow i wyciszenie ich
            i++;
            output.write(sampleInput, sizeof(short int)); // zapisanie do plikow pierwszej sekundy bez echa
        }
        // pozostale sekundy
        i = 0;
        while (input.read(sampleInput, sizeof(short int))) {
            if (i == 44100) // sprawdzenie czy pobrano ponad sekunde, jesli tak to wyzerowanie
                i = 0;
            lastLowByte[i] = ((char)sampleInput[0] << 8) + lastLowByte[i] * 0.6f; // aktualizacja pierwszej polowki echa
            lastHighByte[i] = sampleInput[1] + lastHighByte[i] * 0.6f;  // aktualizacja drugiej polowki echa

            sampleInput[1] = lastHighByte[i]; // pobranie echa do zmiennej wynikowej
            sampleInput[0] = lastLowByte[i];

            output.write(sampleInput, sizeof(short int));
            i++; // inkremenctacja licznika sekundy
        }
        // ostatnia sekunda
        i = 0;
        while (i < 44100) {
            if (i == 44100) // sprawdzenie czy pobrano ponad sekunde, jesli tak to wyzerowanie
                i = 0;
            if (lastLowByte[i] <= 3 && lastHighByte[i] <= 3) // sprawdzenie czy echo jest dostatecznie nieslyszalne jesli tak to koniec petli
                break;
            lastLowByte[i] = lastLowByte[i] * 0.6f; // wyciszenie dolnej polowki echa
            lastHighByte[i] = lastHighByte[i] * 0.6f; // wyciszenie gornej polowki echa

            sampleInput[1] = lastHighByte[i]; // pobranie echa do zmiennej wynikowej
            sampleInput[0] = lastLowByte[i];

            output.write(sampleInput, sizeof(short int)); // zapisanie wyniku do pliku
            i++; // inkremenctacja licznika sekundy
        }
        input.close();
        output.close();
        delete[] lastLowByte;
        delete[] lastHighByte;
    }
}
