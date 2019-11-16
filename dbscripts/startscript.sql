DROP DATABASE airquam;
CREATE DATABASE IF NOT EXISTS airquam;
USE airquam;

CREATE TABLE IF NOT EXISTS Station(
    stationID INT NOT NULL AUTO_INCREMENT,
    name CHAR(20) NOT NULL,
    PRIMARY KEY (stationID)
);

CREATE TABLE IF NOT EXISTS Measurement(
    measurementID INT NOT NULL AUTO_INCREMENT,
    stationID INT NOT NULL,
    time TIME NOT NULL,
    date DATE NOT NULL,
    GPSlatitude DECIMAL(3,8) NOT NULL,
    GPSlongitude DECIMAL(3,8) NOT NULL,
    ZoneLatitude FLOAT(3,2) NOT NULL,
    ZoneLongitude FLOAT(3,2) NOT NULL,
    NO2 INT,
    CO INT,
    CO2 INT,
    TVOC INT,
    Temperature FLOAT(2,1),
    Humidity FLOAT(2,1),
    PRIMARY KEY (measurementID),
    FOREIGN KEY (stationID) REFERENCES Station(stationID),
    FOREIGN KEY (ZoneLatitude, ZoneLongitude) REFERENCES Zone (ZoneLatitude, ZoneLongitude)
);

CREATE TABLE IF NOT EXISTS Zone(
    ZoneLatitude FLOAT(3,2) NOT NULL,
    ZoneLongitude FLOAT(3,2) NOT NULL,
    PRIMARY KEY (ZoneLatitude, ZoneLongitude)
);

CREATE TABLE IF NOT EXISTS Prediction(
    ZoneLatitude FLOAT(3,2) NOT NULL,
    ZoneLongitude FLOAT(3,2) NOT NULL,
    date DATE NOT NULL,
    NO2 INT,
    CO INT,
    CO2 INT,
    TVOC INT,
    PRIMARY KEY (ZoneLatitude, ZoneLongitude, date)
);