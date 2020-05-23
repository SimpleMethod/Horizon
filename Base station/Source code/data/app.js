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

app.controller('HomeController', function ($scope, $http) {
    $scope.get_gsm_information = function () {
        $http({
            url: './gsm/info',
            method: 'GET'
        }).then(
            function (response) {
                $scope.localnetwork = response.data.local_network;
                $scope.signalstrenght = response.data.signal_strenght;
                $scope.ipaddress = response.data.IPAddress;
                $scope.attachednetwork = response.data.attached_to_network;
                $scope.loaded = true;
            },
            function () {
            }
        );
    };

    $scope.get_devices_lists = function () {
        $http({
            url: './devices/list',
            method: 'GET'
        }).then(
            function (response) {
                $scope.devices = response.data.devices;
            },
            function () {
                $scope.errors = "Problem with getting data from a esp32";
            }
        );
    };
    $scope.devices = "{\"devices\":[\"192.168.1.2\",\"192.168.1.3\"]}"
});

app.controller('SettingsController', function ($scope, $http) {
    $scope.update_settings = function (network) {

        if(!network.ssid || !network.password)
        {
            console.log("Problem with reading all data")
        }

        $http({
            url: './connection',
            method: 'POST',
            params: {ssid: network.ssid,
                     password: network.password}
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
