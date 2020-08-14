# adsbAlerting

## Description
Designed to alert the user when a specific aircraft has been detected by ADSB.

It shows all aircraft around and allows reading more details about them.
Aircraft registered in the database will trigger an alert when detected for the first time.
Thnaks to this database, additional information can also displayed.

## Database format
The database is a .csv file. It uses , as a seperator.
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
- Seperate alerting list from database.
- Large review of database, to make use of data available online.
- Add a button to quickly access information on the internet about unknown aircraft.
- Use Window's notifications system, to alert the user even if the sound is muted.
- Create a UI more efficient to handle / sort a lot of aircraft.
- Ensure to only loose the selection when unable to keep it.
- Add a parameter in the UI to set the time after an aircraft is removed from the list.
- Add a way to easily change database (not hard-coded).
- Edit this readme to provide more details about "resources" folder (pictures and alarm sound).
