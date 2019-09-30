#define I1 4 // Input Pins
#define I2 5 //
#define I3 6 //
#define I4 7 //
#define Red 8
#define Green 9
unsigned long timeflag;
unsigned int i1, i2, i3, i4, ans, samples[13][4] = { // Number half-byte codes:
    {1, 0, 0, 0},//1
    {0, 1, 0, 0},//2
    {0, 0, 1, 0},//3
    {0, 0, 0, 1},//4
    {1, 1, 0, 0},//5
    {1, 0, 1, 0},//6
    {1, 0, 0, 1},//7
    {1, 1, 1, 0},//8
    {1, 1, 0, 1},//9
    {1, 0, 1, 1},//10
    {0, 0, 1, 1},//11
    {0, 1, 1, 1},//12
    {1, 1, 1, 1} //13
};

void playTone(int tone, int duration) { 
for (long i = 0; i < duration * 1000L; i += tone * 2) { 
digitalWrite(3, HIGH); 
delayMicroseconds(tone); 
digitalWrite(3, LOW); 
delayMicroseconds(tone); 
} 
}

void playNote(char note, int duration) { 
char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' , 'D', 'E'}; 
int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 926 , 910, 894}; 
for (int i = 0; i < 10; i++) { 
if (names[i] == note) { 
playTone(tones[i], duration); 
} 
} 
}

void playGrad(char note1, char note2, int duration, int size) { 
char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' , 'D', 'E'}; 
int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 926 , 910, 894}; 
int n1 = 0, n2 = 0;
for (int i = 0; i < size; i++) { 
if (names[i] == note1) { 
n1 = i;
} 
if (names[i] == note2) { 
n2 = i;
} 
} 
for (int i = 0; i < duration; i++) { 
playTone(tones[n1] - (((tones[n1] - tones[n2]) / duration) * i), duration / abs(tones[n1] - tones[n2])); 
} 
}

void playSound(char *notes, int *beats, int tempo, int size){
for (int i = 0, j = 0; i < size; i++, j++) { 
if (notes[j] == '/') {
playGrad(notes[j+1], notes[j+2], beats[i] * tempo, size);
j += 2;
} else if (notes[j] == ' ') {
delay(beats[i] * tempo);
} else {
playNote(notes[j], beats[i] * tempo); 
}
}
}

void setup()
{
    Serial.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(I1, INPUT);
    pinMode(I2, INPUT);
    pinMode(I3, INPUT);
    pinMode(I4, INPUT);
    pinMode(Red, OUTPUT);
    pinMode(Green, OUTPUT);
    pinMode(3, OUTPUT);
    
    timeflag = 0;
    i1 = 0;
    i2 = 0;
    i3 = 0;
    i4 = 0;
    ans = 0;
    Serial.print("connect");
    int beat[] = {1, 1, 1, 1};
    playSound("bCDE", beat, 100, 4);
    digitalWrite(Red, HIGH);
    delay(800);
    digitalWrite(Red, LOW);
    delay(200);
    digitalWrite(Green, HIGH);
    delay(100);
    digitalWrite(Green, LOW);
    while(!Serial.available()){}
    if(Serial.available() == 1)
    ans = Serial.parseInt();
    else
    ans = Serial.parseInt() * 10 + Serial.parseInt();
}

void loop()
{
    if(Serial.available() == 1)
    ans = Serial.parseInt();
    else if(Serial.available() == 2)
    ans = Serial.parseInt() * 10 + Serial.parseInt();
    if(digitalRead(I1) + digitalRead(I2) + digitalRead(I3) + digitalRead(I4) > 0) {
        timeflag = millis() + 100;
        i1 = i1 || digitalRead(I1);
        i2 = i2 || digitalRead(I2);
        i3 = i3 || digitalRead(I3);
        i4 = i4 || digitalRead(I4);
        }
    if(timeflag > millis()) {
        digitalWrite(LED_BUILTIN, HIGH);
    } else {
        digitalWrite(LED_BUILTIN, LOW);
        if((i1 > 0) || (i2 > 0) || (i3 > 0) || (i4 > 0)) {
        int val = 0;
        int check[4] = {i1, i2, i3, i4};
        for(int i = 0; i < 13; i++) {
     if(samples[i][0] == check[0] &&
        samples[i][1] == check[1] &&
        samples[i][2] == check[2] &&
        samples[i][3] == check[3])
                val = i+1; 
            }
        if(val == ans) {
        Serial.print("correct");
        digitalWrite(Green, HIGH);
        int beat[] = { 1, 1, 1, 1};
        playSound("bCDE", beat, 100, 4);
        digitalWrite(Green, LOW);
        }
        else {
        Serial.print("wrong");
        digitalWrite(Red, HIGH);
        int beat[] = { 1, 1};
        playSound("ba", beat, 150, 2);
        digitalWrite(Red, LOW);
        }
        i1 = 0;
        i2 = 0;
        i3 = 0;
        i4 = 0;
}
    }
}