var express = require('express');
var router = express.Router();
var connection = require('../../lib/db');

router.get('/station/:id', function(req, res) {
    connection.query("SELECT COUNT(*) AS value FROM Station WHERE stationID=" + parseInt(req.params.id), function(err, result) {
        if (err) {
            console.error(err.message);
            res.send(400);
        } else {
            exists = result[0].value;
            if (exists != 0) {
                connection.query("SELECT * FROM Station WHERE StationID=" + parseInt(req.params.id), function(err, result) {
                    if (err) {
                        console.log(err);
                    } else {
                        var station = {
                            'stationID': result[0].stationID,
                            'name': result[0].name,
                            'sampleRate': result[0].sampleRate,
                            'state': result[0].state
                        }
                        res.render('stationinfo', { station })
                    }
                });
            } else
                res.render('error', { "error": 404 });
        }
    });
})

router.post('/station/:id', function(req, res) {
    connection.query("UPDATE Station SET name='" + req.body.stationName + "', sampleRate='" + parseInt(req.body.sampleRate) + "', state='" + parseInt(req.body.state) + "' WHERE stationID=" + parseInt(req.params.id), function(err, result, fields) {
        if (err) {
            console.log(err);
        }
        res.redirect('/info/station')
    });
})

router.post('/station/:id/delete', function(req, res) {
    connection.query("DELETE FROM Station WHERE stationID=" + parseInt(req.params.id), function(err, result) {
        if (err) {
            console.log(err);
        }
        res.redirect('/info/station/')
    });
})

module.exports = router;