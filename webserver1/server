from fastapi import FastAPI, WebSocket, WebSocketDisconnect
from fastapi.responses import HTMLResponse
from pydantic import BaseModel
import pandas as pd
import uvicorn
from datetime import datetime
import json

app = FastAPI()

# Storage for sensor data
data_store = []
connected_clients = []  # List to store active WebSocket connections

# Define request schema for sensor data
class SensorData(BaseModel):
    sensor_type: str
    temperature_c: float = None
    temperature_f: float = None
    humidity: float = None
    gyro_x: float = None
    gyro_y: float = None
    gyro_z: float = None
    light_lux: float = None
    gas_concentration: float = None
    mag_x: float=None
    mag_y: float=None
    mag_z: float=None
    heading: float=None


# Route to Receive Sensor Data via HTTP POST
@app.post("/submit-data")
async def receive_data(sensor_data: SensorData):
    entry = sensor_data.dict()
    entry["timestamp"] = datetime.utcnow().isoformat()
    data_store.append(entry)

    # Send data to all connected WebSocket clients
    for client in connected_clients:
        await client.send_text(json.dumps(entry))

    return {"message": "Data received successfully"}

#  Route to Retrieve Data via HTTP GET
@app.get("/get-data")
async def get_data():
    if not data_store:
        return {"message": "No data available"}
    df = pd.DataFrame(data_store)
    return df.tail(100).to_dict(orient="records")

# WebSocket Endpoint for Real-Time Data Streaming
@app.websocket("/ws")
async def websocket_endpoint(websocket: WebSocket):
    await websocket.accept()
    connected_clients.append(websocket)
    
    try:
        while True:
            await websocket.receive_text() 
    except WebSocketDisconnect:
        connected_clients.remove(websocket)  

#  Run the Server
if __name__ == "__main__":
    uvicorn.run(app, host="0.0.0.0", port=8000)
