#define I1 4 // Input Pins
#define I2 5 //
#define I3 6 //
#define I4 7 //
unsigned int timeflag, i1, i2, i3, i4, ans, samples[16][4] = { // Number half-byte codes:
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

void setup()
{
    Serial.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(I1, INPUT);
    pinMode(I2, INPUT);
    pinMode(I3, INPUT);
    pinMode(I4, INPUT);
    
    timeflag = 0;
    i1 = 0;
    i2 = 0;
    i3 = 0;
    i4 = 0;
    ans = 0;
    Serial.print("connect");
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
        for(int i = 0; i < 16; i++) {
     if(samples[i][0] == check[0] &&
        samples[i][1] == check[1] &&
        samples[i][2] == check[2] &&
        samples[i][3] == check[3])
                val = i+1; 
            }
        if(val == ans)
        Serial.print("correct");
        else
        Serial.print("wrong");
        i1 = 0;
        i2 = 0;
        i3 = 0;
        i4 = 0;
}
    }
}