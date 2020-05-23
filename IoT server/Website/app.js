var app = angular.module("myApp", ["ngRoute", "ngCookies"]).controller("MultiController");

app.config(function($routeProvider) {
    $routeProvider
        .when("/", {
            templateUrl : './home.html',
            controller: 'HomeController'
        })
        .when("/archivedata", {
            templateUrl : './archiveData.html',
            controller: 'ArchiveDataController'
        })
});

app.controller('CheckController', function ($scope, $http, $cookies) {
    let hash =$cookies.get("hash");
    let deviceID =$cookies.get("deviceid");
    console.log(hash+deviceID);

    if(hash===undefined || deviceID===undefined || deviceID.length===0 || hash.length===0)
    {
        window.location.href = "/login.html";
    }
});

app.controller('LogoutController', function ($scope, $http, $cookies) {
    $scope.logout = function() {
        $cookies.remove("hash");
        $cookies.remove("deviceid");
        window.location.href = "/login.html";
    }
});

app.controller('HomeController', function ($scope, $http, $cookies, $interval) {
    $scope.check_login_status = function () {
        $cookies.get("hash");
        $cookies.get("deviceid");
    };
    $interval( function(){ $scope.get_telemetry(); }, 50000);
    $scope.get_telemetry = function () {
        let tempDate = new Date(); tempDate.setDate(tempDate.getDate() - 2);
        const date = Math.round(+new Date());
        const fromDate = Math.round(+tempDate);

        let obj = {};
        obj.device_id = $cookies.get("deviceid");
        obj.hash  = $cookies.get("hash");
        obj.fromdate = tempDate.valueOf();
        obj.todate = Date.now();
        obj.sorttype = false;
        obj.limit = 1;
        let jsonString= JSON.stringify(obj);
        console.log(jsonString);
        $http({
            url: 'https://sl1ty574c4.execute-api.eu-central-1.amazonaws.com/latest/data',
            method: 'POST',
            data: JSON.stringify(obj),
            contentType: 'application/json'
        }).then(
            function (response) {
                console.log(response.data);
                $scope.horizon_id = response.data.measurement.Items[0].local_network;
                $scope.air_temperature = response.data.measurement.Items[0].air_temperature;
                $scope.air_humidity = response.data.measurement.Items[0].air_humidity;
                $scope.atmospheric_pressure = response.data.measurement.Items[0].atmospheric_pressure;
                $scope.soil_moisture = response.data.measurement.Items[0].soil_moisture;
                $scope.fine_particles25 = response.data.measurement.Items[0].fine_particles25;
                $scope.fine_particles100 = response.data.measurement.Items[0].fine_particles100;
                $scope.wind_direction = response.data.measurement.Items[0].wind_direction+" ";
                $scope.current_load = response.data.measurement.Items[0].current_load;
                $scope.battery_voltage = response.data.measurement.Items[0].battery_voltage;
               // $scope.solar_charging = response.data.measurement.Items[0].solar_charging;
                $scope.rssi = response.data.measurement.Items[0].rssi;
                $scope.timestamp = response.data.measurement.Items[0].measurement_timestamp;
                var date = new Date(response.data.measurement.Items[0].measurement_timestamp);
                $scope.timestamp = date.toString();
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
        else if(average_particle>=3.1 )
        {
            $scope.smog="Bardzo wysoki";
        }
        else {
            $scope.smog="Bardzo niski";
        }
    };
});

app.controller('ArchiveDataController', function ($scope, $http, $cookies) {

    $scope.searchData = function (date) {

        if(!date.from || !date.to)
        {
            console.log("Problem with reading all data");
        }
        if(new Date(date.from).valueOf() <0 || new Date(date.to).valueOf()< 0)
        {
            alert("Data nie może być wcześniejsza niż przed godziną 1 dnia 1 stycznia 1970 roku");
        }

            let obj = {};
            obj.device_id = $cookies.get("deviceid");
            obj.hash  = $cookies.get("hash");
            obj.fromdate =  new Date(date.from).valueOf();
            obj.todate = new Date(date.to).valueOf();
            obj.sorttype = false;
            obj.limit = 244;

            $http({
                url: 'https://sl1ty574c4.execute-api.eu-central-1.amazonaws.com/latest/data',
                method: 'POST',
                data: JSON.stringify(obj),
                contentType: 'application/json'
            }).then(
                function (response) {
                    $scope.records = response.data.measurement.Items;
                    $scope.show=1;
                },
                function () {
                }
            );
        console.log(JSON.stringify(obj));
    };
});
