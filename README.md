# adsbAlerting

## Description
Designed to alert the user when a specific aircraft has been detected by ADSB.

It shows all aircraft around and allows reading more details about them.
Aircraft registered in the database will trigger an alert when detected for the first time.
Thanks to this database, additional information can also displayed.

## Setup
### main.cpp
Three paths are defined in this file.
- The first indicates where the database (see below) is located.
- The second where are the resources (png pictures included in this repository, a file "bell.wav" and pictures of aircraft).
- The third indicates where is the application dump1090.

### adsbframereader.h
Here you can change the ADSB server's IP, port and configure the frame (size and indices of different types of information).

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
- [ ] Add a button to quickly access information on the internet about unknown aircraft.
- [ ] Use Window's notifications system, to alert the user even if the sound is muted- 
- [ ] Create a UI more efficient to handle / sort a lot of aircraft.
- [ ] Ensure to only loose the selection when unable to keep it.
- [ ] Add a parameter in the UI to set the time after an aircraft is removed from the list.
- [ ] Add a way to easily change database (not hard-coded).
- [x] Edit this readme to provide more details about "resources" folder (pictures and alarm sound).
- [ ] Add a dump1090 settings window, to enter the what forms the command line.
- [ ] Add a location selection when launching dump1090.
- [ ] Save favorite locations described right above (GPS coords + name).
- [ ] Add a map to display aircraft with a known position.
- [ ] Add relative aircraft position (heading and distance to the antenna).
