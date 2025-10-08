#include <Arduino.h>

const char index_file[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>

<head>
    <style>
        body {
            margin: 0;
            background: #333;
            font-family: Helvetica, sans-serif;;
        }

        .parent-container {
            height: 70vh;
            display: flex;
            flex-direction: column;
            align-items: center;
            justify-content: center;
        }

        .container {
            position: relative;
            display: flex;
            flex-direction: row;
            align-items: center;
        }

        .tank-wrapper {
            flex-shrink: 0;
            width: 200px;
            height: 300px;
            background: #FFF;
            border-radius: 8px;
            position: relative;
            background-color: #045de9;
            background-image: linear-gradient(315deg, #045de9 0%, #09c6f9 74%);
            overflow: hidden;
        }

        .wave {
            position: absolute;
            width: 2000px;
            height: 2000px;
            margin-top: -860%;
            margin-left: -900px;
            background: rgba(255, 255, 255, .5);
            border-radius: 47.5%;
            animation: wave 10s linear infinite;
            transition: margin 1s;
        }

        .wave.one {
            animation-delay: 0;
        }

        .wave.two {
            animation-delay: -2s;
        }

        .wave.three {
            animation-delay: -4s;
        }

        .waterPercentContainer{
            display: flex; 
            flex-direction: row;
            margin-bottom: -300px;
            transition: transform 1s;
            width: 100px;
        }

        .waterPercentArrow{
            border-top: 20px solid transparent;
            border-bottom: 20px solid transparent;
            border-right: 31px solid rgba(0, 0, 0, 0.7);
        }

        .waterPercentNumeral{
            width: fit-content;
            background: rgba(0, 0, 0, 0.7);
            color: white;
            z-index: 2;
            padding: 5px 10px;
            border-radius: 5px;
            font-size: 30px;
            line-height: 1;
            margin-left: -13px;
        }

        @keyframes wave {
            from {
                transform: rotate(360deg);
            }
        }

        .canvas-container {
            position: relative;
            width: 200px;
            height: 300px;
            overflow: hidden;
            transition: height 1s;
        }

        .canvass {
            position: absolute;
            width: 200px;
            height: 300px;
            top: 0;
            left: 0;
        }

        /* Thermometer------------------------- */

        .thermometer {
            width: 50px;
            height: 165px;
            margin-bottom: -120px;
        }

        #termometer-wrapper {
            margin-bottom: 0;
            display: flex;
            flex-direction: column;
            align-items: center;
        }

        #info {
            opacity: 0.2;
            margin: 0;
            text-align: center;
        }

        #termometer {
            width: 10px;
            background: #38383f;
            height: 140px;
            position: relative;
            border: 5px solid #2a2a2e;
            border-radius: 20px;
            z-index: 1;
        }

        #termometer:before,
        #termometer:after {
            position: absolute;
            content: "";
            border-radius: 50%;
        }

        #termometer:before {
            width: 100%;
            height: 34px;
            bottom: 9px;
            background: #38383f;
            z-index: -1;
        }

        #termometer:after {
            transform: translateX(-50%);
            width: 30px;
            height: 30px;
            background-color: #3dcadf;
            bottom: -30px;
            border: 5px solid #2a2a2e;
            z-index: -3;
            left: 50%;
        }

        #termometer #graduations {
            height: 59%;
            top: 20%;
            width: 50%;
        }

        #termometer #graduations,
        #termometer #graduations:before {
            position: absolute;
            border-top: 2px solid rgba(0, 0, 0, 0.5);
            border-bottom: 2px solid rgba(0, 0, 0, 0.5);
        }

        #termometer #graduations:before {
            content: "";
            height: 34%;
            width: 100%;
            top: 32%;
        }

        #termometer #temperature {
            bottom: 0;
            background: linear-gradient(#FF0000 50%, #3dcadf) no-repeat bottom;
            width: 100%;
            border-radius: 20px;
            background-size: 100% 240px;
            transition: all 0.2s ease-in-out;
        }

        #termometer #temperature,
        #termometer #temperature:before,
        #termometer #temperature:after {
            position: absolute;
        }

        #termometer #temperature:before {
            content: attr(data-value);
            background: rgba(0, 0, 0, 0.7);
            color: white;
            z-index: 2;
            padding: 5px 10px;
            border-radius: 5px;
            font-size: 30px;
            line-height: 1;
            transform: translateY(50%);
            left: calc(100% + 1em / 1.5);
            top: calc(-1em + 9px - 5px * 2);
        }

        #termometer #temperature:after {
            content: "";
            border-top: 0.7em solid transparent;
            border-bottom: 0.7em solid transparent;
            border-right: 1.3em solid rgba(0, 0, 0, 0.7);
            left: 100%;
            top: calc(-1em / 2.2 + 5px);
        }



        #playground {
            font-size: 1.1em;
        }

        #playground #range {
            display: flex;
        }

        #playground #range input[type=text] {
            width: 2em;
            background: transparent;
            border: none;
            color: inherit;
            font: inherit;
            margin: 0 5px;
            padding: 0px 5px;
            border-bottom: 2px solid transparent;
            transition: all 0.2s ease-in-out;
        }

        #playground #range input[type=text]:focus {
            border-color: #3dcadf;
            outline: none;
        }

        #playground #range input[type=text]:first-child {
            text-align: right;
        }

        #playground #unit {
            width: 100%;
            margin: 0;
            text-align: center;
        }

        #playground #unit:hover {
            cursor: pointer;
        }

        /* --------------------------------- */

        /* WaterPipe------------------------ */
        .waterIncomingPipeContainer{
            display: flex; 
            flex-direction: row; 
            align-items: center; 
            width: 50px; 
            height: 20px; 
            margin-top: -267px; 
            background-color: #03e5b7;
            background-image: linear-gradient(0deg, #037ade 5px, #03e5b7 10px, #FFF 15px);
            background-size: 100% 200%;
        }

        .waterGradientAnimation{
            animation: waterGradient 2s linear infinite;
        }

        .waterPipeEnd{
            border: 1px solid; 
            width: 20%; 
            height: 100%;
        }

        .waterPipeMiddle{
            background-color: #333; 
            width: 101%; 
            height: 12%;
        }

        @keyframes waterGradient {
            0% {
                background-position: 100% 60%;
            }
            50% {
                background-position: 100% 100%;
            }
            100% {
                background-position: 100% 60%;
        	}
        }

        /* --------------------------------- */
        /* WaterPipeOutgoing----------------- */
        .waterPipeOutgoingContainer{
            position: absolute;
            left: 250px;
            top: 7px; 
            display: flex; 
            flex-direction: row; 
            align-items: center; 
            width: 50px; 
            height: 20px; 
            background-color: #03e5b7;
            background-image: linear-gradient(0deg, #037ade 5px, #03e5b7 10px, #FFF 15px);
            background-size: 100% 200%;
        }

        .waterGradientAnimation{
            animation: waterGradient 2s linear infinite;
        }

        .waterPipeEnd{
            border: 1px solid; 
            width: 20%; 
            height: 100%;
        }

        .waterPipeMiddle{
            background-color: #333; 
            width: 101%; 
            height: 12%;
        }

        @keyframes waterGradient {
            0% {
                background-position: 100% 60%;
            }
            50% {
                background-position: 100% 100%;
            }
            100% {
                background-position: 100% 60%;
        	}
        }

        /* --------------------------------- */

        /* Sliders ------------------------- */

        input[type=range] {
            -webkit-appearance: none;
            background: transparent;
            margin: 5.5px 0;
            width: 100%;
        }

        input[type=range]::-moz-focus-outer {
            border: 0;
        }

        input[type=range]:hover {
            cursor: pointer;
        }

        input[type=range]:focus {
            outline: 0;
        }

        input[type=range]:focus::-webkit-slider-runnable-track {
            background: #313137;
            border-color: #313137;
        }

        input[type=range]:focus::-ms-fill-lower {
            background: #2a2a2e;
        }

        input[type=range]:focus::-ms-fill-upper {
            background: #313137;
            border-color: #313137;
        }

        input[type=range]::-webkit-slider-runnable-track {
            cursor: default;
            height: 10px;
            transition: all 0.2s ease;
            width: 100%;
            cursor: pointer;
            transition: all 0.2s ease-in-out;
            box-shadow: 1px 1px 1px transparent, 0 0 1px rgba(13, 13, 13, 0);
            background: #2a2a2e;
            border: 2px solid #2a2a2e;
            border-radius: 5px;
        }

        input[type=range]::-webkit-slider-thumb {
            box-shadow: 4px 4px 4px transparent, 0 0 4px rgba(13, 13, 13, 0);
            background: #3dcadf;
            border: 0px solid #3d3d44;
            border-radius: 12px;
            cursor: pointer;
            height: 11px;
            width: 18px;
            -webkit-appearance: none;
            margin-top: -2.5px;
        }

        input[type=range]::-moz-range-track {
            box-shadow: 1px 1px 1px transparent, 0 0 1px rgba(13, 13, 13, 0);
            cursor: default;
            height: 10px;
            transition: all 0.2s ease;
            width: 100%;
            cursor: pointer;
            transition: all 0.2s ease-in-out;
            background: #2a2a2e;
            border: 2px solid #2a2a2e;
            border-radius: 5px;
            height: 5px;
        }

        input[type=range]::-moz-range-thumb {
            box-shadow: 4px 4px 4px transparent, 0 0 4px rgba(13, 13, 13, 0);
            background: #3dcadf;
            border: 0px solid #3d3d44;
            border-radius: 12px;
            cursor: pointer;
            height: 7px;
            width: 14px;
        }

        input[type=range]::-ms-track {
            cursor: default;
            height: 10px;
            transition: all 0.2s ease;
            width: 100%;
            cursor: pointer;
            transition: all 0.2s ease-in-out;
            background: transparent;
            border-color: transparent;
            border-width: 5.5px 0;
            color: transparent;
        }

        input[type=range]::-ms-fill-lower {
            box-shadow: 1px 1px 1px transparent, 0 0 1px rgba(13, 13, 13, 0);
            background: #222226;
            border: 2px solid #2a2a2e;
            border-radius: 10px;
        }

        input[type=range]::-ms-fill-upper {
            box-shadow: 1px 1px 1px transparent, 0 0 1px rgba(13, 13, 13, 0);
            background: #2a2a2e;
            border: 2px solid #2a2a2e;
            border-radius: 10px;
        }

        input[type=range]::-ms-thumb {
            box-shadow: 4px 4px 4px transparent, 0 0 4px rgba(13, 13, 13, 0);
            background: #3dcadf;
            border: 0px solid #3d3d44;
            border-radius: 12px;
            cursor: pointer;
            height: 7px;
            width: 14px;
            margin-top: 2.5px;
        }

        input[type=range]:disabled::-webkit-slider-thumb {
            cursor: not-allowed;
        }

        input[type=range]:disabled::-moz-range-thumb {
            cursor: not-allowed;
        }

        input[type=range]:disabled::-ms-thumb {
            cursor: not-allowed;
        }

        input[type=range]:disabled::-webkit-slider-runnable-track {
            cursor: not-allowed;
        }

        input[type=range]:disabled::-ms-fill-lower {
            cursor: not-allowed;
        }

        input[type=range]:disabled::-ms-fill-upper {
            cursor: not-allowed;
        }

        /* ------------------------- */
    </style>
</head>

<body>
    <div class="parent-container">
        <div class="container">
            <div id="waterIncomingPipeContainer" class="waterIncomingPipeContainer">
                <div class="waterPipeEnd"></div>
                <div style="display: flex; flex-direction: column; justify-content: space-between; border: 1px solid #333; width: inherit; height: 100%;">
                    <div class="waterPipeMiddle"></div>
                    <div class="waterPipeMiddle"></div>
                </div>
                <div class="waterPipeEnd"></div>
            </div>
            <div class="tank-wrapper">
                <div class="wave one"></div>
                <div class="wave two"></div>
                <div class="wave three"></div>
                <div id="canvas-container" class="canvas-container">
                    <canvas class="canvass"></canvas>
                </div>
            </div>
            <div id="waterPipeOutgoingContainer" class="waterPipeOutgoingContainer">
                <div class="waterPipeEnd"></div>
                <div style="display: flex; flex-direction: column; justify-content: space-between; border: 1px solid #333; width: inherit; height: 100%;">
                    <div class="waterPipeMiddle"></div>
                    <div class="waterPipeMiddle"></div>
                </div>
                <div class="waterPipeEnd"></div>
            </div>
            <div id="waterPercentContainer" class="waterPercentContainer">
                <div class="waterPercentArrow"></div>
                <div id="waterPercentNumeral" class="waterPercentNumeral">
                    0%
                </div>
            </div>
            <!-- Thermometer -->
            <div class="thermometer">
                <div id="termometer-wrapper">
                    <div id="termometer">
                        <div id="temperature" style="height:0" data-value="&deg;C"></div>
                        <div id="graduations"></div>
                    </div>
                </div>
            </div>
            <!----------------->
        </div>
    </div>
    <!-- Sliders -->
    <div style="width: 200px;">
        <span style="color: #FFF;">Flow Rate</span>
        <input id="flowRateSlider" type="range" min="0" max="100" value="50">
        <br>
        <br>
        <span style="color: #FFF;">Water Level</span>
        <input id="waterPercentSlider" type="range" min="0" max="100" value="50">
        <br>
        <br>
        <span style="color: #FFF;">Temperature</span>
        <input id="waterTemperatureSlider" type="range" min="0" max="50" value="25">
        <br>
        <br>
        <span style="color: #FFF;">Water Incoming</span>
        <input type="checkbox" id="waterIncomingCheck">
        <br>
        <br>
        <span style="color: #FFF;">Tank Full</span>
        <input type="checkbox" id="tankFullCheck">
    </div>
    <!------------>
    <script>
        // configuration
        minTemperature = -5;
        maxTemperature = 50;
        minDistance = 20;
        maxDistance = 140;
        wetnessThreshold = 2500;
        minFlowrate = 0;
        maxFlowrate = 15;
        var flowRate;
        var temperatureValue;
        var waterPercent;
        var waterIncoming;
        var tankFull;
        // -------------
        
        function setWaterPercent(percentIn) {
            waterPercent = percentIn;
            document.getElementById("canvas-container").style.height = (320 - (310 * (waterPercent * 0.01))) + "px";
            document.getElementById("waterPercentContainer").style.transform = "translateY(-"+ (waterPercent * 300 * 0.01) + "px)";
            document.getElementById("waterPercentNumeral").innerText = waterPercent + "%";

            let collection = document.getElementsByClassName("wave");
            for (let i = 0; i < collection.length; i++) {
                collection[i].style.marginTop = (-1720 - (290 * 0.01 * waterPercent)) + "px";
            }
        }

        function setWaterFlow(flowRateIn) {
            let collection = document.getElementsByClassName("wave");
            if (flowRateIn <= 0) {
                for (let i = 0; i < collection.length; i++) {
                    collection[i].style.animationPlayState = 'paused';
                }
            }
            else {
                for (let i = 0; i < collection.length; i++) {
                    collection[i].style.animationPlayState = 'running';
                }
            }
            flowRate = flowRateIn;
        }

        const canvas = document.querySelector("canvas");
        const ctx = canvas.getContext("2d");
        canvas.height = innerHeight, canvas.width = innerWidth;

        let particles;
        class Particle {
            constructor(x, y) {
                this.x = x, this.y = y;
                this.vx = 0, this.vy = 0;
                this.gravity = 0.2;
                this.r = 10, this.color = "#0099ff80";
                this.drag = 0.3 / (((flowRate * 0.01) * 1.5) + 0.2);
                this.initialVelX = Math.random() * (5 - 3 + 1) + 3;
            }

            render() {
                ctx.fillStyle = this.color;
                ctx.beginPath();
                ctx.arc(this.x, this.y, this.r, 0, Math.PI * 2);
                ctx.fill();
            }

            update() {
                this.vx -= this.vx * this.drag;
                this.vy += this.gravity + (this.vy / 100);
                this.vx += this.initialVelX;
                this.y += this.vy;
                this.x += this.vx;
            }
        }

        const init = () => {
            particles = [];
        }

        const animate = () => {
            ctx.clearRect(0, 0, innerWidth, innerHeight);

            for (let i = 0; i < particles.length; ++i) {
                if (particles[i].y > innerHeight) {
                    particles[i].r = 0;
                    particles.splice(i, 1);
                }
            }

            for (let i = 0; i < (15 * flowRate * 0.01); ++i) {
                let hw = 40;
                let width = 5;
                let y = Math.random() * ((hw + width) - (hw - width) + 1) + (hw - width);
                let x = Math.random() * ((10 + 40) - (10 - 30) + 1) + (10 - 30);
                particles.push(new Particle(0, y));
            }

            particles.forEach(particle => {
                particle.render();
                particle.update();
            });

            requestAnimationFrame(animate);
        }

        // Thermometer --------------------------------------

        const units = {
            Celcius: "\u00B0C",
            Fahrenheit: "\u00B0F"
        };

        const config = {
            minTemp: minTemperature,
            maxTemp: maxTemperature,
            unit: "Celcius"
        };

        const temperature = document.getElementById("temperature");

        function setTemperature(temperatureValueIn) {
            temperatureValue = temperatureValueIn;
            temperature.style.height = (temperatureValue - config.minTemp) / (config.maxTemp - config.minTemp) * 100 + "%";
            temperature.dataset.value = temperatureValue + units[config.unit];
        }

        function setWaterIncoming(waterIncomingIn){
            if(waterIncoming != waterIncomingIn){
                waterIncoming = waterIncomingIn;
                if(waterIncoming){
                    document.getElementById("waterIncomingPipeContainer").classList.add("waterGradientAnimation");
                }
                else{
                    document.getElementById("waterIncomingPipeContainer").classList.remove("waterGradientAnimation");;
                }
            }
        }

        function setTankFull(tankFullIn){
            if(tankFull != tankFullIn){
                tankFull = tankFullIn;
                if(tankFull){
                    document.getElementById("waterPipeOutgoingContainer").classList.add("waterGradientAnimation");
                }
                else{
                    document.getElementById("waterPipeOutgoingContainer").classList.remove("waterGradientAnimation");;
                }
            }
        }
        // --------------------------------------------------
        
        canSendNextRequest = true;
        isConnected = false;

        function httpGetAsyncInfinite(theUrl, callback)
        {
            canSendNextRequest = false;
            var xmlHttp = new XMLHttpRequest();
            xmlHttp.onreadystatechange = function() { 
                if (xmlHttp.readyState == 4){
                    if(xmlHttp.status == 200){
                        callback(xmlHttp.responseText);
                        canSendNextRequest = true;
                        isConnected = true;
                    }
                    else{
                        canSendNextRequest = true;
                        isConnected = false;
                    }
                }
            }
            xmlHttp.open("GET", theUrl, true); // true for asynchronous 
            xmlHttp.send(null);
        }

        function setDataParameters(jsonResponse){
            jsonData = JSON.parse(jsonResponse);

            if(jsonData.level >= minDistance && jsonData.level <= maxDistance){
                let distancePercent = 100 - (((jsonData.level - minDistance)/(maxDistance - minDistance)) * 100);
                console.log(distancePercent);
                setWaterPercent(distancePercent);
            }
            if(jsonData.temperature >= minTemperature && jsonData.temperature <= maxTemperature){
                setTemperature(jsonData.temperature.toFixed(2));
            }
            if(jsonData.flowRate >= minFlowrate && jsonData.flowRate <= maxFlowrate){
                let flowPercent = ((jsonData.flowRate - minFlowrate)/(maxFlowrate - minFlowrate)) * 100;
                setWaterFlow(flowPercent);
                console.log(flowPercent);
            }

            console.log(jsonResponse);
        }

        function delay(milliseconds) {
            const date = Date.now();
            let currentDate = null;
            do {
                currentDate = Date.now();
            } while (currentDate - date < milliseconds);
        }

        // Main Program-------------------------------

        init();
        animate();
        
        setInterval(() => {
            setWaterFlow(document.getElementById("flowRateSlider").value);
            setWaterPercent(document.getElementById("waterPercentSlider").value);
            setTemperature(document.getElementById("waterTemperatureSlider").value);
            setWaterIncoming(document.getElementById("waterIncomingCheck").checked);
            setTankFull(document.getElementById("tankFullCheck").checked);
            

            // if(canSendNextRequest){
            //     httpGetAsyncInfinite("http://192.168.0.132/getData", setDataParameters);
            // }
        }, 500)


    </script>
</body>

</html>
)rawliteral";