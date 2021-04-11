////////////////////////////////////////////////////////////////////////////////////////////////////

/* 
 * Arduino Sequencer
 * synthesizer module firmware
 * 
 * by TimMJN
 * 
 * v1.0 
 * 16-01-2021
 * 
 * For schematics and other information, see
 * https://github.com/TimMJN/Arduino-Sequencer
 */

////////////////////////////////////////////////////////////////////////////////////////////////////

// pin definitions
#define CLOCK_PIN     2
#define RESET_PIN     3
#define FLIP_PIN      4
#define RAND_PIN      5
#define INC_LED_PIN   6
#define DEC_LED_PIN   7
#define SER_DATA_PIN  8
#define SER_CLOCK_PIN 9
#define SER_LATCH_PIN 10

#define N_STEPS 8 // number of steps

byte cur_step  = 0; // current step
byte next_step = 0; // next step
bool dir_inc   = true;  // direction (increasing = true, decreasing = false)
bool reset     = false; // has the reset been triggered this step?
bool randomise = false; // turn on/off random sequence

// current and previous states of reset and flip pins
bool cur_reset_state  = true;
bool prev_reset_state = true;
bool cur_flip_state   = true;
bool prev_flip_state  = true;
bool cur_rand_state   = true;
bool prev_rand_state  = true;

////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  // pin modes
  // alternatively ommit external pull up resistors and use INPUT_PULLUP
  pinMode(CLOCK_PIN,     INPUT);
  pinMode(RESET_PIN,     INPUT);
  pinMode(FLIP_PIN,      INPUT);
  pinMode(RAND_PIN,      INPUT_PULLUP);
  pinMode(INC_LED_PIN,   OUTPUT);
  pinMode(DEC_LED_PIN,   OUTPUT);
  pinMode(SER_DATA_PIN,  OUTPUT);
  pinMode(SER_CLOCK_PIN, OUTPUT);
  pinMode(SER_LATCH_PIN, OUTPUT);

  // set up interrupt on clock pin
  attachInterrupt(digitalPinToInterrupt(CLOCK_PIN), take_step, FALLING);
  interrupts();

  // set up randomisation
  randomise       = digitalRead(RAND_PIN) == LOW;
  prev_rand_state = digitalRead(RAND_PIN) == HIGH;

  // set next step and direction LEDs;
  set_next_step();
  set_direction_leds();

  // set output to first step
  set_shift_register(cur_step);
  digitalWrite(SER_LATCH_PIN, HIGH);

  // set shift register to next step
  set_shift_register(next_step);

  // do a small delay to prevent startup affecting inputs
  delay(100);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void loop() {
  // read reset and flip inputs
  cur_reset_state = digitalRead(RESET_PIN) == HIGH;
  cur_flip_state  = digitalRead(FLIP_PIN ) == HIGH;
  cur_rand_state  = digitalRead(RAND_PIN ) == HIGH;

  // edge detection on reset, flip and randomise inputs
  if (prev_reset_state && (!cur_reset_state)) {
    reset = true;
    set_next_step();
    set_shift_register(next_step); // update shift register
  }

  if (prev_flip_state && (!cur_flip_state)) {
    dir_inc = !dir_inc; // flip direction

    // update next step, shift register and LEDs
    set_next_step();
    set_shift_register(next_step); // update shift register
    set_direction_leds();
  }

  if (prev_rand_state && (!cur_rand_state)) { // turn on randomisation
    randomise = true;
    set_next_step();
    set_shift_register(next_step); // update shift register
  }
  if ((!prev_rand_state) && cur_rand_state) { // turn off randomisation
    randomise = false;
    set_next_step();
    set_shift_register(next_step); // update shift register
  }

  // update previous cycle values
  prev_reset_state = cur_reset_state;
  prev_flip_state  = cur_flip_state;
  prev_rand_state  = cur_rand_state;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

// set what will be the next step
void set_next_step() {
  if (reset) {
    next_step = 0; // go back to step 0
  }
  else if (randomise) {
    next_step = random(0, N_STEPS); // pick a random step
  }
  else {
    if (dir_inc)
      next_step = (cur_step + 1) % N_STEPS; // increase 1 step
    else
      next_step = (cur_step - 1 + N_STEPS) % N_STEPS; // decrease 1 step
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

// set the direction LEDs
void set_direction_leds() {
  if (dir_inc) {
    digitalWrite(INC_LED_PIN, HIGH);
    digitalWrite(DEC_LED_PIN, LOW);
  }
  else {
    digitalWrite(DEC_LED_PIN, HIGH);
    digitalWrite(INC_LED_PIN, LOW);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

// write the required state to the shift register
void set_shift_register(byte step_index) {
  noInterrupts(); // do not interrupt writing to the shift register

  // loop over the steps, starting from the last
  for (int i = (N_STEPS - 1); i >= 0; i--) {
    if (i == step_index)
      digitalWrite(SER_DATA_PIN, HIGH); // set current step high
    else
      digitalWrite(SER_DATA_PIN, LOW); // set other steps low
  
    digitalWrite(SER_CLOCK_PIN, HIGH); // give clock pin a pulse
    digitalWrite(SER_CLOCK_PIN, LOW);
  }

  interrupts(); // re-enable interrupts
}

////////////////////////////////////////////////////////////////////////////////////////////////////}

// interrupt function
// push shift register to output, prepare for the next step
void take_step() {
  // push to output register (pulse latch pin)
  digitalWrite(SER_LATCH_PIN, HIGH);
  digitalWrite(SER_LATCH_PIN, LOW);

  // update steps
  cur_step = next_step;
  reset = false;
  set_next_step();

  // update next step to shift register
  set_shift_register(next_step);
}
