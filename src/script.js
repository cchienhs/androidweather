var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

function locationSuccess(pos) {
  console.log( "Location Success" );
  // Construct URL
  var url = 'http://api.openweathermap.org/data/2.5/weather?lat=' +
      pos.coords.latitude + '&lon=' + pos.coords.longitude;
  // Send request to OpenWeatherMap
  xhrRequest(url, 'GET', 
    function(responseText) {

      var json = JSON.parse(responseText);

      var temperature =  json.main.temp;
      var maxTemperature = json.main.temp_max;
      var minTemperature = json.main.temp_min;
      var conditions = json.weather[0].icon;

      
      // Assemble dictionary using our keys
      var dictionary = {
        'KEY_TEMPERATURE': temperature,
        'KEY_TEMPERATURE_MAX': maxTemperature,
        'KEY_TEMPERATURE_MIN': minTemperature,
        'KEY_CONDITIONS': conditions
      };
      
      // Send to Pebble
      Pebble.sendAppMessage(dictionary,
        function(e) {
          console.log('Weather info sent to Pebble successfully!');
        },
        function(e) {
          console.log('Error sending weather info to Pebble!');
        }
      );
    }      
  );
}

function locationError(err) {
  console.log('Error requesting location!');
  getWeather();
}

function getWeather() {
  navigator.geolocation.getCurrentPosition(
    locationSuccess,
    locationError,
    {timeout: 15000, maximumAge: 60000}
  );
}

Pebble.addEventListener('showConfiguration', function(e) {
  var temperatureUnit = localStorage.getItem("KEY_TEMPERATURE_UNIT") ? localStorage.getItem("KEY_TEMPERATURE_UNIT"):"";
  var blinkingColons = localStorage.getItem("KEY_BLINKING_COLONS") ? localStorage.getItem("KEY_BLINKING_COLONS"):"";
  var weatherUpdateInterval = localStorage.getItem("KEY_WEATHER_UPDATE_INTERVAL") ? localStorage.getItem("KEY_WEATHER_UPDATE_INTERVAL"):"";
  var url = 'http://coolskies.net/pebble/androidweather-config.html?temperatureUnit=' + temperatureUnit + "&blinkingColons=" + blinkingColons + "&weatherUpdateInterval=" + weatherUpdateInterval;
  console.log( url );
  Pebble.openURL(url);
});

Pebble.addEventListener('webviewclosed',
  function(e) {
    var configuration = JSON.parse(decodeURIComponent(e.response));
    
    localStorage.setItem( 'KEY_TEMPERATURE_UNIT', configuration.temperatureUnit );
    localStorage.setItem( 'KEY_BLINKING_COLONS', configuration.blinkingColons );
    localStorage.setItem( 'KEY_WEATHER_UPDATE_INTERVAL', configuration.weatherUpdateInterval );
    
    //Send to Pebble, persist there
    Pebble.sendAppMessage(
      {"KEY_TEMPERATURE_UNIT": configuration.temperatureUnit,
       "KEY_BLINKING_COLONS": configuration.blinkingColons,
       "KEY_WEATHER_UPDATE_INTERVAL": configuration.weatherUpdateInterval },
        function(e) {
          console.log("Sending settings data...");
        },
        function(e) {
          console.log("Settings feedback failed!");
        }
    );
  }
);

// Listen for when the watchface is opened
Pebble.addEventListener('ready', 
  function(e) {
    console.log('PebbleKit JS ready!');
    getWeather();
  }
);


// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    if( e.payload["KEY_TEMPERATURE"] == 1 ) 
      getWeather();
  }                     
);