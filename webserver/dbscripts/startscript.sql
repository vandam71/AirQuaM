CREATE TABLE IF NOT EXISTS Station(
    stationID INT NOT NULL AUTO_INCREMENT,
    name CHAR(20) NOT NULL,
    sampleRate INT NOT NULL,
    state INT check (state between 0 and 1),
    PRIMARY KEY (stationID)
);

CREATE TABLE IF NOT EXISTS Zone(
    ZoneID INT NOT NULL AUTO_INCREMENT,
    ZoneLatitude FLOAT(5,2) NOT NULL,
    ZoneLongitude FLOAT(5,2) NOT NULL,
    PRIMARY KEY (ZoneID)
);

CREATE TABLE IF NOT EXISTS Prediction(
    ZoneID INT NOT NULL,
    time TIME NOT NULL,
    date DATE NOT NULL,
    NO2 INT,
    CO INT,
    CO2 INT,
    TVOC INT,
    FOREIGN KEY (ZoneID) REFERENCES Zone(ZoneID)
);

CREATE TABLE IF NOT EXISTS Measurement(
    measurementID INT NOT NULL AUTO_INCREMENT,
    stationID INT NOT NULL,
    time TIME NOT NULL,
    date DATE NOT NULL,
    GPSlatitude FLOAT(11,8) NOT NULL,
    GPSlongitude FLOAT(11,8) NOT NULL,
    ZoneID INT NOT NULL,
    NO2 INT,
    CO INT,
    CO2 INT,
    TVOC INT,
    Temperature FLOAT(4,1),
    Humidity FLOAT(4,1),
    PRIMARY KEY (measurementID),
    FOREIGN KEY (stationID) REFERENCES Station(stationID),
    FOREIGN KEY (ZoneID) REFERENCES Zone(ZoneID)
);
