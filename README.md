# adsbAlerting

## Description
Designed to alert the user when a specific aircraft has been detected by ADSB.

It shows all aircraft around and allows reading more details about them.
Aircraft registered in the database will trigger an alert when detected for the first time.
Thanks to this database, additional information can also displayed.

Using dump1090 to access the ADS-B antenna and sent information over the network. This application then reads this information by opening a TCP socket.

## Setup
When launching the program for the first time, the settings window will show to ask you some basic information.
The resources folder is the one copied in this repository. As a consequence, this folder can be placed where you want, you just need to enter the right path.
The network values displayed below are used by dump1090. You do not need to change the default values.

You will be required to enter a location as well. This is used both to launch dump1090 (enhanced precision) and to provide the dstance and bearing of aircraft around.

## Resources
### png pictures included in this repository
They are used for the user interface, darwing a sound/mute button.

### bell.wav sound file
This sound is played regularly when an alert is raised, until this alert is acknowleged. A short sound is advised.

### aircraft pictures
The resources folder can contain jpg pictures of aircraft. They will be displayed on the right side of the window when the associated aircraft is selected. The picture must be named "registration.jpg". As a consequence, only aircraft in the database can be shown, since ADS-B messages do not always contain the registration (can be a callsign). Using the icao code instead of the registration would remove this limitation. I considered though that it is much more convenient to handle registrations than icao codes, since the registration is directely painted on the aircraft.

## Database format
The database is a .csv file. It uses , as a separator.
Information is organized in the following order:
1. Aircraft registration (e.g. F-MJDJ)
2. Aircraft type (e.g. EC35)
3. Aircraft variant (e.g. T2)
4. Aircraft serial number (e.g. 0806)
5. Operator (e.g. "Gendarmerie")
6. ICAO transponder code (e.g. "3AAC15")
7. Basis (e.g. "DAG Colmar")
8. Callsign (e.g. "F-DJ")
9. Department (French administrative subdivision) (e.g. "68")

## To be improved / added
- [ ] Separate alerting list from database.
- [ ] Large review of database, to make use of data available online.
- [x] Add a button to quickly access information on the internet about unknown aircraft.
- [x] Use Window's notifications system, to alert the user even if the sound is muted. 
- [ ] Create a UI more efficient to handle / sort a lot of aircraft.
- [x] Ensure to only loose the selection when unable to keep it.
- [x] Add a parameter in the UI to set the time after an aircraft is removed from the list.
- [x] Add a way to easily change database (not hard-coded).
- [x] Edit this readme to provide more details about "resources" folder (pictures and alarm sound).
- [x] Add a dump1090 settings window, to enter the what forms the command line.
- [x] Add a location selection when launching dump1090.
- [x] Save favorite locations described right above (GPS coords + name).
- [ ] Add a map to display aircraft with a known position.
- [x] Add relative aircraft position (heading and distance to the antenna).
