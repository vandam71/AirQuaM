var express = require('express');
var router = express.Router();
var connection = require('./lib/db');

router.post('/measurement', function (req, res){
    connection.query("");
});