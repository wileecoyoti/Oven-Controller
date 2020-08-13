# Oven-Controller
Reflow, drying, or curing

## Some background
Code was originally built to support a telescope temperature control system, which is the hardware it normally runs on. However, there's really nothing in there that couldn't be easily breadboarded on an arduino.

- Temperature sensors are Dallas/Maxim 1-wire digital sensors, referred to as 'sensorambient' and 'sensorscope', where scope is usually whatever goes in the curing oven. If you're doing a reflow you will probably want to exchange this for a K-type thermocouple and you'll need a different library.
- There's plenty of marked out code in there for alternative things to play with, like automatically switching between agressive and relaxed PID settings based on distance-from-target.
- if you're looking to follow a temperature profile, the "tempTrack" function simply takes temperature (in F) and time (in seconds). EG 150f for 5 minutes would be tempTrack(150, 300);
- You can just stack these commands in the loop however you please.


## Creative Commons CC-BY-SA:
https://creativecommons.org/licenses/by-sa/4.0/

CC BY-SA: This license allows reusers to distribute, remix, adapt, and build upon the material in any medium or format, so long as attribution is given to the creator. The license allows for commercial use. If you remix, adapt, or build upon the material, you must license the modified material under identical terms.
