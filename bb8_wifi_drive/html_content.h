const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>Arduino Control</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
     <script>
        function sendCommand(route) {
            fetch(route)
                .then(response => {
                    if (!response.ok) {
                        console.error('Server responded with an error status:', response.status);
                    }
                })
                .catch(error => console.error('Network error or connection lost:', error));
        }
    </script>
</head>
<body>
    <h1>My Arduino Dashboard</h1>
    <button onClick= "sendCommand('/forward')">forward</button>
    <button onClick= "sendCommand('/stop')">stop</button>
    <button onClick= "sendCommand('/backword')">backword</button>
</body>
</html>
)rawliteral";



