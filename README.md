# Arduino-Sequencer
This projects contains schematics and firmware for a step sequencer compatible with Eurorack modular synthesizers, based on an Arduino Nano. This is a DIY project, suitable for beginners, allthough a general knowledge of electronics, basic tools and skills are required to build this sequencer. The schematics presented here feature:

- 8 steps, 2 voices
- Clock/reset/flip direction inputs
- Random sequence switch
- Buffered gate outputs per step
- Buffered CV outputs per voice
- Scale/offset controls
- Glide controls

Any builder is encouraged to experiment and make changed to the project to suit their needs. By the use of 74HC595 Serial-In-Parallel-Out shift registers to drive the step signals, this sequencer can be easily extended to 16, 32, 64 or even more steps. Similarly, additional voice can be easily added.
