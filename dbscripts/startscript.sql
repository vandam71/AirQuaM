DROP DATABASE airquam;
CREATE DATABASE IF NOT EXISTS airquam;
USE airquam;

CREATE TABLE IF NOT EXISTS Station(
    stationID INT NOT NULL AUTO_INCREMENT,
    name CHAR(20) NOT NULL,
    PRIMARY KEY (stationID)
);

CREATE TABLE IF NOT EXISTS Zone(
    ZoneID INT NOT NULL AUTO_INCREMENT,
    ZoneLatitude FLOAT(5,2) NOT NULL,
    ZoneLongitude FLOAT(5,2) NOT NULL,
    PRIMARY KEY (ZoneLatitude, ZoneLongitude)
);

CREATE TABLE IF NOT EXISTS Prediction(
    ZoneLatitude FLOAT(5,2) NOT NULL,
    ZoneLongitude FLOAT(5,2) NOT NULL,
    date DATE NOT NULL,
    NO2 INT,
    CO INT,
    CO2 INT,
    TVOC INT,
    PRIMARY KEY (ZoneLatitude, ZoneLongitude, date),
    FOREIGN KEY (ZoneLatitude, ZoneLongitude) REFERENCES Zone(ZoneLatitude, ZoneLongitude)
);

CREATE TABLE IF NOT EXISTS Measurement(
    measurementID INT NOT NULL AUTO_INCREMENT,
    stationID INT NOT NULL,
    time TIME NOT NULL,
    date DATE NOT NULL,
    GPSlatitude FLOAT(11,8) NOT NULL,
    GPSlongitude FLOAT(11,8) NOT NULL,
    ZoneLatitude FLOAT(5,2) NOT NULL,
    ZoneLongitude FLOAT(5,2) NOT NULL,
    NO2 INT,
    CO INT,
    CO2 INT,
    TVOC INT,
    Temperature FLOAT(3,1),
    Humidity FLOAT(3,1),
    PRIMARY KEY (measurementID),
    FOREIGN KEY (stationID) REFERENCES Station(stationID),
    FOREIGN KEY (ZoneLatitude, ZoneLongitude) REFERENCES Zone(ZoneLatitude, ZoneLongitude)
);