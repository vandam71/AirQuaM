var express = require('express');
var router = express.Router();
var connection = require('../lib/db')

router.get('/', function (req, res) {
  res.render('home');
});

module.exports = router;
