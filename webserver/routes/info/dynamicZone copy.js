var express = require('express');
var router = express.Router();
var connection = require('../../lib/db');

router.get("/zone/:id/prediction", function(req, res) {
    zoneID = parseInt(req.params.id);
    var batch = [];
    connection.query("SELECT COUNT(*) AS value FROM Prediction WHERE ZoneID=" + zoneID, function(err, result) {
        if (err) {
            console.error(err.message);
            res.render('error', 404);
        } else {
            exists = result[0].value;
            if (exists != 0) {
                connection.query("SELECT * FROM Zone WHERE ZoneID=" + zoneID, function(err, result) {
                    if (err) {
                        console.log(err);
                        res.send(err);
                    } else if (result.size != 0) {
                        var zoneInfo = {
                            'ZoneID': zoneID,
                            'ZoneLatitude': result[0].ZoneLatitude,
                            'ZoneLongitude': result[0].ZoneLongitude
                        }
                        connection.query("SELECT * FROM Prediction WHERE ZoneID=" + zoneID, function(err, rows) {
                            if (err) {
                                console.log(err.message);
                            } else {
                                for (var i = 0; i < rows.length; i++) {
                                    var time = (rows[i].time).split(':');
                                    var date = (rows[i].date).split('-');

                                    var prediction = {
                                        'datetime': new Date(date[0], date[1], date[2], time[0], time[1], time[2], 00),
                                        'time': rows[i].time,
                                        'date': rows[i].date,
                                        'NO2': rows[i].NO2,
                                        'CO': rows[i].CO,
                                        'CO2': rows[i].CO2,
                                        'TVOC': rows[i].TVOC
                                    }
                                    batch.push(prediction);
                                }

                                const sortedDate = batch.sort((a, b) => b.datetime - a.datetime);

                                res.render('predictionZone', {
                                    "zoneInfo": zoneInfo,
                                    "list": sortedDate
                                })
                            }
                        })
                    }
                })
            } else
                res.render('error', { "error": 404 })
        }
    })
});

module.exports = router;