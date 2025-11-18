// Full IR Detection Logic with Setup and Loop

// IR Detection Logic for A2 (Right) and A3 (Left)
int baselineRight = 0;
int baselineLeft = 0;
int significantChangeRight = 0;
int significantChangeLeft = 0;

int averageRight = 0;
int averageLeft = 0;

int sumRight = 0;
int sumLeft = 0;

int sampleCounterRight = 0;
int sampleCounterLeft = 0;

const int amountOfInputsForAverage = 6;

// Setup Function
void setup() {
    Serial.begin(9600);
    Serial.println("Initializing IR Sensors...");

    initializeIR(); // Initialize IR sensors for A2 and A3

    Serial.println("Initialization Complete");
    Serial.print("Baseline Right: ");
    Serial.println(baselineRight);
    Serial.print("Baseline Left: ");
    Serial.println(baselineLeft);
}

// Main Loop Function
void loop() {
    detectIRBall(); // Continuously detect IR ball position
    delay(1000); // Delay to reduce noise
}

// Initialization for A2 and A3
void initializeIR() {
    unsigned long startTime = millis();
    while (millis() - startTime < 5000) {
        int valueRight = analogRead(A2);
        int valueLeft = analogRead(A3);

        averageRight = updateAverageIR(valueRight, true);
        averageLeft = updateAverageIR(valueLeft, false);

        baselineRight = averageRight;
        baselineLeft = averageLeft;
    }

    significantChangeRight = calculateSignificantChange(baselineRight);
    significantChangeLeft = calculateSignificantChange(baselineLeft);
}

// Update moving average for A2 and A3
int updateAverageIR(int newValue, bool isRight) {
    if (isRight) {
        sumRight += newValue;
        sampleCounterRight++;
        if (sampleCounterRight == amountOfInputsForAverage) {
            int newAverage = sumRight / amountOfInputsForAverage;
            sumRight = 0;
            sampleCounterRight = 0;
            return newAverage;
        }
        return averageRight;
    } else {
        sumLeft += newValue;
        sampleCounterLeft++;
        if (sampleCounterLeft == amountOfInputsForAverage) {
            int newAverage = sumLeft / amountOfInputsForAverage;
            sumLeft = 0;
            sampleCounterLeft = 0;
            return newAverage;
        }
        return averageLeft;
    }
}

// Calculate significant change threshold
int calculateSignificantChange(int baseline) {
    return (0.0487 * baseline + 0.5);
}

// Detect IR Ball based on A2 and A3
void detectIRBall() {
    int valueRight = analogRead(A2);
    int valueLeft = analogRead(A3);

    averageRight = updateAverageIR(valueRight, true);
    averageLeft = updateAverageIR(valueLeft, false);

    Serial.print("Values a2base (right): ");
    Serial.println(baselineRight);
    Serial.print("Values a2average (right): ");
    Serial.println(averageRight);
    Serial.print("Values a3base (left): ");
    Serial.println(baselineLeft); 
    Serial.print("Values a3average (left): ");
    Serial.println(averageLeft);
    Serial.println("");

    if (baselineRight - averageRight >= significantChangeRight) {
        Serial.println("Ball detected on the RIGHT");
    }
    if (baselineLeft - averageLeft >= significantChangeLeft) {
        Serial.println("Ball detected on the LEFT");
    }
}
