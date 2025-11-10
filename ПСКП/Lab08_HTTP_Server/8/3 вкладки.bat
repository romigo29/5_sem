@echo off
set CHROME="C:\Program Files\Google\Chrome\Application\chrome.exe"
start "" %CHROME% --new-window "http://localhost:5000/connection" "http://localhost:5000/headers" "http://localhost:5000/parameter" "http://localhost:5000/formparameter" "http://localhost:5000/upload"
