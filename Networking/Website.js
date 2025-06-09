<style>
.sensor-button {
    padding: 10px;
    margin: 10px;
    background: blue;
    color: white;
    border: none;
    border-radius: 4px;
    cursor: pointer;
}
.sensor-button.enabled {
    background: #22c55e;
}
.sensor-input {
    padding: 8px;
    margin: 10px;
    width: 200px;
    border: 1px solid #ccc;
    border-radius: 4px;
}
.sensor-input:disabled {
    background: #f3f4f6;
    cursor: not-allowed;
}
</style>
<div>
    <input type="text" id="username" class="sensor-input" placeholder="Enter your username">
    <button id="sensorButton" class="sensor-button" onclick="requestSensorPermission()" disabled>
        Enable Sensors
    </button>
    <div>
        <p>Tilt F/B: <span id="tilt-fb">0</span>°</p>
        <p>Tilt L/R: <span id="tilt-lr">0</span>°</p>
    </div>
</div>
<script>
// Generate a random userId upon opening page
const ID = `${Date.now() % 1000}${Math.floor(Math.random() * 100)}`;
let ws = null;

// Handle username input
const usernameInput = document.getElementById('username');
const sensorButton = document.getElementById('sensorButton');
usernameInput.addEventListener('input', function() {
    sensorButton.disabled = !this.value.trim();
});

function connectWebSocket(username) {
    console.log('Attempting to connect to WebSocket...');
    ws = new WebSocket('wss://calm-concise-longhorn.ngrok.app/ws/web');
    
    ws.onopen = () => {
        console.log('WebSocket connected successfully');
        console.log('WebSocket state:', ws.readyState);
    };

    ws.onclose = (event) => {
        console.log('WebSocket closed with code:', event.code);
        console.log('Close reason:', event.reason);
    };

    ws.onerror = (error) => {
        console.error('WebSocket error occurred:', error);
        console.log('WebSocket state:', ws.readyState);
    };
}

// Function to request permission and enable sensors
async function requestSensorPermission() {
    console.log('Permission request started');
    const username = document.getElementById('username').value.trim();
    
    if (!username) {
        alert('Please enter a username first');
        return;
    }
    
    if (typeof DeviceOrientationEvent.requestPermission === 'function') {
        try {
            const permission = await DeviceOrientationEvent.requestPermission();
            console.log('Sensor permission:', permission);
            if (permission === 'granted') {
                connectWebSocket(username);
                enableSensors(username);
                sensorButton.classList.add('enabled');
                // Disable username input after enabling sensors
                usernameInput.disabled = true;
                sensorButton.disabled = true;
            }
        } catch (error) {
            console.error('Error requesting permission:', error);
        }
    } else {
        console.log('No permission needed, enabling sensors directly');
        connectWebSocket(username);
        enableSensors(username);
        sensorButton.classList.add('enabled');
        // Disable username input after enabling sensors
        usernameInput.disabled = true;
        sensorButton.disabled = true;
    }
}

function enableSensors(username) {
    console.log('Enabling sensors...');
    sensorButton.textContent = 'Sensors Enabled';
    
    let lastFBThreshold = 0;
    let lastLRThreshold = 0;
    let lastUpdate = 0;
    const THROTTLE_MS = 0; // This may or may not help the server processs all the data beign sent from various users.

    function clamp(value, min, max) {
        return Math.min(Math.max(value, min), max);
    }
		
    // Return any new 3 degree threshold past, otherwise return null if
    // no threshold has been past.
    function getThresholdCrossed(currentValue, lastThreshold) {
        // Get the current 3-degree threshold the value has crossed
        const currentThreshold = Math.floor(currentValue / 3) * 3;
        if (currentThreshold !== lastThreshold) {
            return currentThreshold;
        }
        return null;
    }

    window.addEventListener('deviceorientation', (event) => {
        // Clamp the values to their respective ranges
        const fbTilt = clamp(Math.round(event.beta), -40, 40);
        const lrTilt = clamp(Math.round(event.gamma), -50, 50);

        // Update HTML display
        document.getElementById('tilt-fb').textContent = fbTilt;
        document.getElementById('tilt-lr').textContent = lrTilt;
        
        // Check if we've crossed any 3-degree thresholds
        const newFBThreshold = getThresholdCrossed(fbTilt, lastFBThreshold);
        const newLRThreshold = getThresholdCrossed(lrTilt, lastLRThreshold);
        
        // Check time for throttling and if we've crossed a threshold
        const now = Date.now();
        if ((newFBThreshold !== null || newLRThreshold !== null) && 
            (now - lastUpdate >= THROTTLE_MS) && 
            ws && ws.readyState === WebSocket.OPEN) {
            
            lastUpdate = now;
            
            // If we've crossed a 3 degree threshold, update variables.
            if (newFBThreshold !== null) lastFBThreshold = newFBThreshold;
            if (newLRThreshold !== null) lastLRThreshold = newLRThreshold;
            
            // Send JSON to server with user device data.
            ws.send(JSON.stringify({
                ID: ID,
                username: username,
                beta: lastFBThreshold,
                gamma: lastLRThreshold
            }));
        }
    });
}
</script>