var createError = require('http-errors');
var express = require('express');
var path = require('path');
var cookieParser = require('cookie-parser');
var bodyParser = require('body-parser');
var logger = require('morgan');

var homeRouter = require('./routes/home');

//info requires
var infoRouter = require('./routes/info/infoMain');
var addStationRouter = require('./routes/info/addStation');
var dynamicZoneRouter = require('./routes/info/dynamicZone');
var dynamicStationRouter = require('./routes/info/dynamicStation');

//database requires
var zoneRouter = require('./routes/database/zone');
var stationRouter = require('./routes/database/station');
var measurementRouter = require('./routes/database/measurement');

//app definition
var app = express();

// view engine setup
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'ejs');

app.use(logger('dev'));
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));
app.use(express.json());
app.use(express.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));
app.use(express.static(path.join(__dirname, 'scripts')));
app.use(express.static(path.join(__dirname, 'views')));

//general route
app.use('/', homeRouter);

//info routes
app.use('/info', infoRouter);
app.use('/info', addStationRouter);
app.use('/info', dynamicZoneRouter);
app.use('/info', dynamicStationRouter);

//database routes
app.use('/data', zoneRouter);
app.use('/data', stationRouter);
app.use('/data', measurementRouter);

// catch 404 and forward to error handler
app.use(function(req, res, next) {
  next(createError(404));
});

// error handler
app.use(function(err, req, res, next) {
  // set locals, only providing error in development
  res.locals.message = err.message;
  res.locals.error = req.app.get('env') === 'development' ? err : {};

  // render the error page
  res.status(err.status || 500);
  res.render('error', { error: err });
});

module.exports = app;
