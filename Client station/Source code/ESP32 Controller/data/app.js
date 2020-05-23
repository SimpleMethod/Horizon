var app = angular.module("myApp", ["ngRoute"]).controller("MultiController");

app.config(function($routeProvider) {
    $routeProvider
        .when("/", {
            templateUrl : './home.html',
            controller: 'HomeController'
        })
        .when("/settings", {
            templateUrl : './settings.html',
            controller: 'SettingsController'
        })
});

app.controller('HomeController', function ($scope, $http,  $interval) {
    $interval( function(){ $scope.get_telemetry(); }, 5000);
    $scope.get_telemetry = function () {
        $http({
            url: './horizon/sensors/data',
            method: 'GET'
        }).then(
            function (response) {
                $scope.horizon_id = response.data.local_network;
                $scope.air_temperature = response.data.air_temperature;
                $scope.air_humidity = response.data.air_humidity;
                $scope.atmospheric_pressure = response.data.atmospheric_pressure;
                $scope.soil_moisture = response.data.soil_moisture;
                $scope.fine_particles25 = response.data.fine_particles25;
                $scope.fine_particles100 = response.data.fine_particles100;
                $scope.wind_direction = response.data.wind_direction+" ";
                $scope.current_load = response.data.current_load;
                $scope.battery_voltage = response.data.battery_voltage;
                //$scope.solar_charging = response.data.solar_charging;
                $scope.rssi = response.data.rssi;
                $scope.loaded = true;
            },
            function () {
            }
        );


        let particle2;
        let particle10;


        if($scope.fine_particles25<=10)
        {
            particle2=0;
        }
        else if($scope.fine_particles25>=11 && $scope.fine_particles25<=20)
        {
            particle2=1;
        }
        else if($scope.fine_particles25>=21 && $scope.fine_particles25<=30)
        {
            particle2=2;
        }
        else if($scope.fine_particles25>=31 && $scope.fine_particles25<=60)
        {
            particle2=3;
        }
        else
        {
            particle2=4;
        }

        if($scope.fine_particles100<=15)
        {
            particle10=0;
        }
        else if($scope.fine_particles100>=16 && $scope.fine_particles100<=30)
        {
            particle10=1;
        }
        else if($scope.fine_particles100>=31 && $scope.fine_particles100<=50)
        {
            particle10=2;
        }
        else if($scope.fine_particles100>=51 && $scope.fine_particles100<=100)
        {
            particle10=3;
        }
        else
        {
            particle10=4;
        }
        let average_particle= (particle2+particle10/2);

        if(average_particle<=1.5)
        {
            $scope.smog="Bardzo niski";
        }
        else if(average_particle>=1.6 && average_particle<=2.0)
        {
            $scope.smog="Niski";
        }
        else if(average_particle>=2.1 && average_particle<=2.5)
        {
            $scope.smog="Średni";
        }
        else if(average_particle>=2.6 && average_particle<=3.0)
        {
            $scope.smog="Wysoki";
        }
        else
        {
            $scope.smog="Bardzo wysoki";
        }
		
		if($scope.soil_moisture===0)
        {
            $scope.soil_moisture_text="Sucha";
        }
		else if($scope.soil_moisture===1)
		{
			 $scope.soil_moisture_text="Umiarkowana";
		}
		else
		{
			$scope.soil_moisture_text="Wilgotna";
		}
    };

});

app.controller('SettingsController', function ($scope, $http) {
    $scope.update_settings = function (network) {

        if(!network.ssid || !network.password || !network.refreshtime)
        {
            console.log("Problem with reading all data")
        }

        $http({
            url: './connection',
            method: 'POST',
            params: {ssid: network.ssid,
                     password: network.password,
                     refreshtime: network.refreshtime}
        }).then(
            function (response) {
                $scope.devices = response.data;
            },
            function () {
                $scope.errors = "Problem with getting data from a esp32";
            }
        );
    };
});
