# Arduino Sequencer
This projects contains schematics and firmware for a step sequencer compatible with Eurorack and Kosmo modular synthesizers, based on an Arduino Nano. This is a DIY project, suitable for beginners, allthough a general knowledge of electronics, basic tools and skills are required to build this sequencer. The schematics presented here feature:

- 8 steps, 2 voices
- Clock/reset/flip direction inputs
- Random sequence switch
- Buffered gate outputs per step
- Buffered CV outputs per voice
- Scale/offset controls
- Glide controls

Any builder is encouraged to experiment and make changed to the project to suit their needs. By the use of 74HC595 Serial-In-Parallel-Out shift registers to drive the step signals, this sequencer can be easily extended to 16, 32, 64 or even more steps. Similarly, additional voice can be easily added. I'll keep updating the project and documentation as I go.

# Frequently Asked Questions
## The build
#### Do you have PCBs / panels available?
Not at this point. I'm working on a PCB design for Kosmo format, though it is possible to build on perfboard/stripboard. I want to encourage any builder to come up with their own panel layout, make whatever feels natural to them. If you document your work, I'll be happy to add it here for future reference.

#### How to I add more steps?
Simply copy the main sequencer section as many times as you like. The connections between the 74HC595s are noted in the schematic. The latch and clock pins simply connect together, the serial out of each register connects to the serial in of the next. In the code, change the value of `N_STEPS` and voila!

#### How to I add more voices?
Copy the row of potentiometers/diodes in the main sequencer section as many times as you like. Don't forget to also copy the asociated output circuitry!

#### Can I leave out the gate buffers?
Yes you can. It would be best practice the keep them, but they could be replaced by just a protection diode (e.g. 1N1418).

#### Can I leave out the offset/scale/glide?
I would advice against using no buffering at all, but you could replace this section with just a unity gain buffer. It's also possible to leave out only the glide components, in which case you can also do without the non-inverting buffer behind it.

#### Can I leave out the input protection?
This I would strongly advice against. It's very easy to fry the Arduino by plugging in the wrong signal. You could simplify to a voltage clamp using two Schottky diodes, but I don't see the point in not doing it properly. If you do go with that option, don't forget to change to input logic to track rising instead of falling edges, as the current set-up inverts the inputs.

#### Can I leave out the randomise switch?
Yes, just simply don't include it.

#### Can I change the gate voltage?
Yes. On the schematic it is attached to 9V, such that the gate voltage is 9V minus one transistor drop, so about 8.4V. This conforms to Eurorack standards. Alternatively, you could omit these parts and connect V_GATE to 5V. This should be enough to trigger almost any module, as a common trigger voltage is between 2-3V.

## The code
#### Why don't you use `shiftOut`?
The serial output code I wrote here does the exact same thing as `shiftOut`. I only adapted it to output any number of steps, as the standard 8 bits by `shiftOut`.

#### What's this `interrupt` business?
This allows that Arduino to stop whatever it's doing when it receives a pulse on the clock input and update to the next step. That means that the polling of the other input pins will never slow down the progressing to the next step. Neat!

## The patch
#### Why can't I change the sequence length?
You can, simply patch the gate of the last step to the reset input.

#### Why can't I set a back-and-forth pattern?
You can, simply patch one or two of the gates to the flip input.
