// Arduino Nano RPM and Speed Signal Generator for Gauge Testing
//
// This sketch generates TWO INDEPENDENT pulse signals:
// 1. SPEED (Pin 9): Sweeps automatically between 10 km/h and 160 km/h.
// 2. RPM (Pin 8): Sweeps automatically between 1000 RPM and 7500 RPM.
//
// KEY DISCOVERY: This specific aftermarket gauge works directly with the 0V-5V pulse
// output from the Arduino Nano. NO EXTERNAL 12V BOOSTING CIRCUIT is required
// for the signal line, simplifying the project significantly.
//
// REMINDER: Ensure the Arduino GND is connected to the gauge's GND for a common reference.

const int RPM_SIGNAL_PIN = 8;    // Output Pin for TACHOMETER (RPM) -> D8
const int SPEED_SIGNAL_PIN = 9;  // Output Pin for SPEEDOMETER (Speed) -> D9

// --- 1. RPM SWEEP SETTINGS (Engine Speed) ---
const long MIN_RPM = 1000;
const long MAX_RPM = 7500;
long currentRPM = MIN_RPM;
int rpmStep = 100; // 100 RPM step change
// Variables for RPM pulse generation timing
unsigned long lastRpmPulseTime = 0;
bool rpmPinState = LOW;

// --- 2. SPEED SWEEP SETTINGS (Vehicle Speed) ---
// Vehicle Speed Sensor Pulse calculation requires tire circumference.
const long TIRE_CIRCUMFERENCE_MM = 1902; // 1902 millimeters (Your requested value)

// PPK (Pulses Per Kilometer) based on 1902mm circumference (1 pulse/rev):
// PPK = 1,000,000 mm / 1902 mm/rev = 525.76 pulses/km
//
// Half Period (us) Formula Constant: C = 1,800,000,000 / PPK
// C = 1,800,000,000 / 525.76 = 3,423,519 (approx)
const unsigned long SPEED_PULSE_CONSTANT = 3423520UL; 

const int MIN_SPEED_KPH = 10;
const int MAX_SPEED_KPH = 160;
int currentSpeedKPH = MIN_SPEED_KPH;
int speedStep = 5; // 5 KPH step change
// Variables for Speed pulse generation timing
unsigned long lastSpeedPulseTime = 0;
bool speedPinState = LOW;


// --- 3. COMMON SWEEP TIMER ---
const long SWEEP_INTERVAL_MS = 50; // Update RPM and Speed values every 50 milliseconds
unsigned long lastSweepTime = 0;

void setup() {
  // Initialize both signal pins and the built-in LED as outputs.
  pinMode(RPM_SIGNAL_PIN, OUTPUT);
  pinMode(SPEED_SIGNAL_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  
  // Start serial communication to check the calculated values (for debugging)
  Serial.begin(9600); 
}

void loop() {
  unsigned long currentTime = micros();
  unsigned long currentTimeMs = millis();
  
  // ===========================================
  // 1. AUTOMATIC SWEEP LOGIC (Updates both RPM and Speed values every 50ms)
  // ===========================================
  if (currentTimeMs - lastSweepTime >= SWEEP_INTERVAL_MS) {
    lastSweepTime = currentTimeMs; 

    // --- RPM Sweep ---
    currentRPM += rpmStep;
    if (currentRPM >= MAX_RPM) {
      currentRPM = MAX_RPM;
      rpmStep = -100; 
    } else if (currentRPM <= MIN_RPM) {
      currentRPM = MIN_RPM;
      rpmStep = 100; 
    }
    
    // --- Speed Sweep ---
    currentSpeedKPH += speedStep;
    if (currentSpeedKPH >= MAX_SPEED_KPH) {
      currentSpeedKPH = MAX_SPEED_KPH;
      speedStep = -5; // Switch to decreasing speed
    } else if (currentSpeedKPH <= MIN_SPEED_KPH) {
      currentSpeedKPH = MIN_SPEED_KPH;
      speedStep = 5; // Switch to increasing speed
    }

    // Print current readings for debugging (Requires Serial Monitor open)
    Serial.print("RPM: ");
    Serial.print(currentRPM);
    Serial.print(" | Speed (KPH): ");
    Serial.println(currentSpeedKPH);
  }

  // ===========================================
  // 2. RPM PULSE GENERATION (Pin 8)
  // ===========================================
  // Formula: Half Period (us) = 30,000,000 / currentRPM
  // This assumes 1 Pulse Per Revolution for the engine.
  long rpm_halfPeriod_US = 30000000L / currentRPM;

  if (currentTime - lastRpmPulseTime >= rpm_halfPeriod_US) {
    lastRpmPulseTime += rpm_halfPeriod_US; 
    rpmPinState = !rpmPinState;
    digitalWrite(RPM_SIGNAL_PIN, rpmPinState); // D8 is the RPM output
    digitalWrite(LED_BUILTIN, rpmPinState); // Use LED for RPM visual pulse
  }

  // ===========================================
  // 3. SPEED PULSE GENERATION (Pin 9)
  // ===========================================
  // The constant is C = 1,800,000,000 / PPK. Half Period (us) = C / SpeedKPH
  long speed_halfPeriod_US = SPEED_PULSE_CONSTANT / currentSpeedKPH;

  if (currentTime - lastSpeedPulseTime >= speed_halfPeriod_US) {
    lastSpeedPulseTime += speed_halfPeriod_US; 
    speedPinState = !speedPinState;
    digitalWrite(SPEED_SIGNAL_PIN, speedPinState); // D9 is the Speed output
  }
}