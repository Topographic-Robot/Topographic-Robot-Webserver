import threading
import asyncio
import websockets
import dash.dash
import dash
from dash import dcc, html
from dash.dependencies import Input,Output
import pandas as pd
import plotly.express as px
import json

#dash dashboard
app= dash.Dash(__name__)
app.layout= html.Div([
    html.H1("analyzed data"),
    dcc.Interval(id='update-interval',interval=5000,n_intervals=0),
    dcc.Graph(id='temp-graph'),
    dcc.Graph(id='gyro-graph'),
])

# WebSocket 
sensor_data = []  # List to store real-time data

async def websocket_listener():
    uri = "ws://localhost:8000/ws"
    async with websockets.connect(uri) as websocket:
        while True:
            message = await websockets.recv()
            data = json.loads(message)
            sensor_data.append(data)  # Store received data

#Listener 
def start_websocket_client():
    loop = asyncio.new_event_loop()
    asyncio.set_event_loop(loop)
    loop.run_until_complete(websocket_listener())

# Run WebSocket listener in background
threading.Thread(target=start_websocket_client, daemon=True).start()

#callback function for dynamical update
@app.callback(
        [
            Output('temp-graph','figure'),
            Output('gyro-graph','figure')

        ],
        [Input('update-interval', 'n_intervals')]
    )
def update_graphs(n):
    if not sensor_data:
        return px.line(title="Waiting for Data"), px.line(title="Waiting for Data")
    df = pd.DataFrame(sensor_data)
    df["timestamp"] = pd.to_datetime(df["timestamp"])
    temp_df = df[df["sensor_type"] == "temperature_humidity"]
    gyro_df = df[df["sensor_type"] == "gyro"]
    temp_fig = px.line(temp_df, x="timestamp", y="temperature_c", title="Temperature Over Time")
    gyro_fig = px.line(gyro_df, x="timestamp", y=["gyro_x", "gyro_y", "gyro_z"], title="Gyro Sensor Readings")

    return temp_fig, gyro_fig
# Run app
if __name__ == '__main__':
    app.run_server(debug=True)
