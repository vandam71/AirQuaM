CREATE TABLE IF NOT EXISTS Station(
    stationID INT(10) NOT NULL AUTO_INCREMENT,
    name CHAR(20) NOT NULL,
    sampleRate INT(10) NOT NULL,
    state INT check (state between 0 and 1),
    PRIMARY KEY (stationID)
);

CREATE TABLE IF NOT EXISTS Zone(
    ZoneID INT(10) NOT NULL AUTO_INCREMENT,
    ZoneLatitude FLOAT(5,2) NOT NULL,
    ZoneLongitude FLOAT(5,2) NOT NULL,
    PRIMARY KEY (ZoneID)
);

CREATE TABLE IF NOT EXISTS Prediction(
    ZoneID INT(10) NOT NULL,
    time TIME NOT NULL,
    date DATE NOT NULL,
    NO2 INT(10),
    CO INT(10),
    CO2 INT(10),
    TVOC INT(10),
    PRIMARY KEY (ZoneID, date),
    FOREIGN KEY (ZoneID) REFERENCES Zone(ZoneID)
);

CREATE TABLE IF NOT EXISTS Measurement(
    measurementID INT(10) NOT NULL AUTO_INCREMENT,
    stationID INT(10) NOT NULL,
    time TIME NOT NULL,
    date DATE NOT NULL,
    GPSlatitude FLOAT(11,8) NOT NULL,
    GPSlongitude FLOAT(11,8) NOT NULL,
    ZoneID INT(10) NOT NULL,
    NO2 INT(10) UNSIGNED,
    CO INT(10),
    CO2 INT(10),
    TVOC INT(10),
    Temperature FLOAT(3,1),
    Humidity FLOAT(3,1),
    PRIMARY KEY (measurementID),
    FOREIGN KEY (stationID) REFERENCES Station(stationID),
    FOREIGN KEY (ZoneID) REFERENCES Zone(ZoneID)
);