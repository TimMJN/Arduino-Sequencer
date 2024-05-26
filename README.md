# Arduino Sequencer
This projects contains schematics and firmware for a step sequencer compatible with Eurorack and Kosmo modular synthesizers, based on an Arduino Nano. This is a DIY project, suitable for beginners, although a general knowledge of electronics, basic tools and skills are required to build this sequencer. The schematics presented here feature:

- 8 steps, 2 voices
- Clock/reset/flip direction inputs
- Random sequence switch
- Buffered gate outputs per step
- Buffered CV outputs per voice
- Scale/offset controls
- Glide controls

Any builder is encouraged to experiment and make changed to the project to suit their needs. By the use of 74HC595 Serial-In-Parallel-Out shift registers to drive the step signals, this sequencer can be easily extended to 16, 32, 64 or even more steps. Similarly, additional voice can be easily added. I'll keep updating the project and documentation as I go.

A [short demonstration](https://youtu.be/yhleVjD8cbQ) is available in YouTube. View the [schematic](schematic+BOM/arduino_sequencer.pdf) and [bill of materials](https://htmlpreview.github.io/?https://github.com/TimMJN/Arduino-Sequencer/blob/main/schematic%2BBOM/arduino_sequencer_BOM.html) on this repository.

# Frequently Asked Questions
## The build
#### Do you have PCBs / panels available?
A PCB design for Kosmo format is available, along with a panel drill guide. I might have some PCBs available, please get in touch. However, it is also possible to build this design on perfboard/stripboard. I want to encourage any builder to come up with their own panel layout, make whatever feels natural to them. If you document your work, I'll be happy to add it here for future reference.

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

# License
All designs in this repository are released under GPL-3.0 licence. Feel free to use and adapt them to your heart's desire. The only ask to refrain from mass-production and commercialisation of PCBs/modules, as I rely on PCB sales for funding new module designs. If you use (parts of) my designs in your own modules, please credit me on your schematics and PCBs to help users find the original creator. I call upon your good conscience to make fair use of my work shared here.

# Support
If you like the resources I have made available here, and wish to support the development of new modules, feel free to buy me a few components through a small donation. I mainly design modules for fun, but you probably already know it can be a costly endeavour. All small contributions help me, thank you very much!

[![paypal](https://www.paypalobjects.com/en_US/i/btn/btn_donateCC_LG.gif)](https://www.paypal.com/donate?hosted_button_id=FZJELWSAH4UKU)
